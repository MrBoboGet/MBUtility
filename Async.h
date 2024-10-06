#include <future>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <deque>
#include <type_traits>
#include "MOFunction.h"

namespace MBUtility
{
    template<typename T>
    class Async
    {
        typedef MBUtility::MOFunction<void(T& Object)> TaskFunc;
        struct SharedState
        {
            std::mutex TaskMutex;
            T Object;
            std::thread TaskThread;
            std::condition_variable TaskAddedConditional;
            std::atomic<bool> Stopping = false;
            std::deque<MBUtility::MOFunction<void(T& Object)>> Tasks;
        
            template<typename... ArgTypes>
            SharedState(ArgTypes&&... Args)
                : Object(std::forward<ArgTypes>(Args)...)
            {
                   
            }

        };
        std::shared_ptr<SharedState> m_State;

        static void p_TaskThread(std::shared_ptr<SharedState> State)
        {
            while(!State->Stopping.load())
            {
                MBUtility::MOFunction<void(T& Object)> Task;
                {
                    std::unique_lock Lock(State->TaskMutex);
                    while(!State->Stopping.load() && State->Tasks.size() == 0)
                    {
                        State->TaskAddedConditional.wait(Lock);
                    }
                    if(State->Stopping.load())
                    {
                        break;   
                    }
                    Task = std::move(State->Tasks.front());
                    State->Tasks.pop_front();
                }
                Task(State->Object);
            }
        }
    public:
        template<typename FuncType,typename... ArgTypes>
        auto AddTask(FuncType Func,ArgTypes... Arguments)
        {
            typedef decltype(std::invoke(Func,std::declval<T>(),Arguments...)) ReturnType;
            std::promise<ReturnType> Promise;
            auto ReturnValue = Promise.get_future();
            std::lock_guard Lock(m_State->TaskMutex);
            auto Lambda = [Promise=std::move(Promise),Func=Func](ArgTypes... Arguments,T& Object) mutable
                    {
                        if constexpr(!std::is_same_v<void,ReturnType>)
                        {
                            Promise.set_value((Object.*Func)(Arguments...));
                        }
                        else
                        {
                            (Object.*Func)(Arguments...);
                            Promise.set_value();
                        }
                    };
            auto BoundFunc = std::bind(std::move(Lambda),std::move(Arguments)...,std::placeholders::_1);
            static_assert(std::is_invocable_v<decltype(BoundFunc),T&>,"Bound func not invocable with T&");
            TaskFunc NewFunc = std::move(BoundFunc);
            static_assert(std::is_invocable_v<decltype(NewFunc),T&>,"Task func not invocable with T&");
            m_State->Tasks.emplace_back(std::move(NewFunc));
            m_State->TaskAddedConditional.notify_one();
            return ReturnValue;
        }
        template<typename... ArgTypes>
        Async(ArgTypes&&... Args)
        {
            m_State = std::make_shared<SharedState>(std::forward<ArgTypes>(Args)...);
            m_State->TaskThread = std::thread(p_TaskThread,m_State);
        }
        ~Async()
        {
            std::lock_guard Lock(m_State->TaskMutex);
            m_State->Stopping.store(true);
            m_State->TaskAddedConditional.notify_one();
            m_State->TaskThread.detach();
        }
    };
}
