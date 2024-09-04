#pragma once
#include <memory>
#include <mutex>
#include <atomic>
#include <chrono>
#include <limits>
#include <deque>
#include <functional>
#include <condition_variable>

namespace MBUtility
{

    template<typename T>
    class Future;
    class WaitContext
    {
        template<typename f>
        friend class SharedFutureState;
        struct SharedState
        {
            std::mutex StateMutex;
            std::condition_variable StateConditional;
            std::deque<size_t> UpdatedFutures;

            std::atomic<int> RemainingFutures = 0;

            void Notify(size_t ID)
            {
                std::lock_guard InternalsLock(StateMutex);
                UpdatedFutures.push_back(ID);
                StateConditional.notify_one();
                RemainingFutures.fetch_add(-1);
            }
        };

        std::shared_ptr<SharedState> m_State;
    public:
        template<typename T>
        void AddFuture(size_t  ID,Future<T>& FutureToAdd);

        size_t GetNextUpdate()
        {
            std::unique_lock InternalsLock(m_State->StateMutex);
            while(m_State->UpdatedFutures.size() == 0)
            {
                m_State->StateConditional.wait(InternalsLock);
            }
            auto ReturnValue = m_State->UpdatedFutures.front();
            m_State->UpdatedFutures.pop_back();
            return  ReturnValue;
        }
        template<typename Rep,typename  Period>
        size_t GetNextUpdate(std::chrono::duration<Rep,Period> WaitTime )
        {
            std::unique_lock InternalsLock(m_State->StateMutex);
            //while(m_State->UpdatedFutures.size() == 0)
            //{
            m_State->StateConditional.wait_for(InternalsLock,WaitTime);
            if(m_State->UpdatedFutures.size() == 0)
            {
                return -1;   
            }
            //}
            auto ReturnValue = m_State->UpdatedFutures.front();
            m_State->UpdatedFutures.pop_back();
            return  ReturnValue;
        }

    };


    template<typename T>
    class SharedFutureState
    {
        template<typename f>
        friend class Future;
        template<typename f>
        friend class Promise;
        friend class WaitContext;



        std::mutex ValueMutex;
        std::condition_variable ValueSetConditional;
        std::atomic<bool> ValueSet = false;
        std::atomic<bool> ValueNotPossible = false;
        T Value;
        std::function<void()> OnError;
        std::function<void(T const&)> ThenFunc;

        std::shared_ptr<WaitContext::SharedState> MultiWaiter = nullptr;
        size_t WaitIndex = -1;

        void Notify()
        {
            ValueSetConditional.notify_one();
            if(MultiWaiter != nullptr)
            {
                MultiWaiter->Notify(WaitIndex);
            }
        }
    };

    template<typename T>
    class Future
    {
        template<typename  t>
        friend class Promise;
        friend class WaitContext;

        std::shared_ptr<SharedFutureState<T>> m_InternalState;

        Future(std::shared_ptr<SharedFutureState<T>>  State)
        {
            m_InternalState = std::move(State);
        }
    public:
        Future(){};
        void Wait()
        {
            while(m_InternalState->ValueSet.load() == false && m_InternalState->ValueNotPossible.load() == false)
            {
                WaitFor(std::chrono::seconds(std::numeric_limits<int>::max()));
            }
        }
        template<typename  Rep,typename Period>
        void WaitFor(std::chrono::duration<Rep,Period> const& Duration)
        {
            std::unique_lock Lock(m_InternalState->ValueMutex);
            if(m_InternalState->ValueSet || m_InternalState->ValueNotPossible.load()) 
            {
                return;
            }
            m_InternalState->ValueSetConditional.wait_for(Lock,Duration);

        }

        void Then(std::function<void(T )> Func)
        {
            std::lock_guard ValueLock(m_InternalState->ValueMutex);
            if(m_InternalState->ValueSet)
            {
                Func(m_InternalState->Value);
            }
            else
            {
                m_InternalState->ThenFunc = std::move(Func);
            }
        }
        void OnError(std::function<void()> Func)
        {
            std::lock_guard ValueLock(m_InternalState->ValueMutex);
            if(m_InternalState->ValueNotPossible)
            {
                Func();
            }
            else
            {
                m_InternalState->OnError = std::move(Func);
            }
        }

        bool ValueAvailable()
        {
            return m_InternalState->ValueSet.load();
        }

        T Get()
        {
            Wait();
            std::lock_guard ValueLock(m_InternalState->ValueMutex);
            if(m_InternalState->ValueSet.load() == false)
            {
                throw std::runtime_error("Error getting value from future: value unavailable");
            }
            return  m_InternalState->Value;
        }

        T const& GetRef()
        {
            Wait();
            std::lock_guard ValueLock(m_InternalState->ValueMutex);
            if(m_InternalState->ValueSet.load() == false)
            {
                throw std::runtime_error("Error getting value from future: value unavailable");
            }
            return  m_InternalState->Value;
        }
    };

    template<typename T>
    class Promise
    {
        std::shared_ptr<SharedFutureState<T>> m_InternalState;
    public:
        Promise()
        {
            m_InternalState = std::make_shared<SharedFutureState<T>>();
        }
        Promise(Promise const& Promise) = delete;
        Promise(Promise&& Promise)
        {
            std::swap(Promise.m_InternalState,m_InternalState);
        }
        Future<T> GetFuture()
        {
            return Future<T>(m_InternalState);
        }

        void SetValue(T NewValue)
        {
            std::lock_guard ValueLock(m_InternalState->ValueMutex);
            m_InternalState->ValueSet = true;
            m_InternalState->Value = std::move(NewValue);
            m_InternalState->Notify();
            if(m_InternalState->ThenFunc)
            {
                m_InternalState->ThenFunc(std::move(m_InternalState->Value));
            }
        }
        void SetInvalid()
        {
            std::lock_guard ValueLock(m_InternalState->ValueMutex);
            m_InternalState->ValueNotPossible.store(true);
            m_InternalState->Notify();
            if(m_InternalState->OnError)
            {
                m_InternalState->OnError();
            }
        }
        ~Promise()
        {
            if(m_InternalState != nullptr && !m_InternalState->ValueSet.load())
            {
                SetInvalid();
            }
        }
    };



    //
    template<typename T>
    void WaitContext::AddFuture(size_t  ID,Future<T>& FutureToAdd)
    {
        std::lock_guard FutureLock(FutureToAdd.m_InternalState->ValueMutex);
        FutureToAdd.m_InternalState->WaitIndex = ID;
        FutureToAdd.m_InternalState->MultiWaiter = m_State;
        if(FutureToAdd.m_InternalState->ValueSet || FutureToAdd.m_InternalState->ValueNotPossible)
        {
            std::lock_guard StateLock(m_State->StateMutex);
            m_State->UpdatedFutures.push_back(ID);
        }
        else
        {
            m_State->RemainingFutures.fetch_add(1);
        }
    }
}
