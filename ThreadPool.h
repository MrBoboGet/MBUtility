#pragma once
#include <vector>
#include <deque>
#include <thread>
#include <future>
#include <condition_variable>

#include "MOFunction.h"

namespace MBUtility
{
    class ThreadPool
    {
    private:
        std::atomic<bool> m_Stopping{false};
        std::vector<std::thread> m_Threads;
        bool m_Dynamic = false;
        
        //used for spurrious wakeups
        std::condition_variable m_TasksAvailableConditional;
        std::mutex m_TaskMutex; 

        std::deque<MOFunction<void()>> m_Tasks;
        std::atomic<int>  m_BusyThreads = 0;

        std::condition_variable m_TaskFinishedConditional;
        
        void p_TaskThread()
        {
            while(!m_Stopping.load())
            {
                MOFunction<void()> TaskToExecute;
                {
                    std::unique_lock<std::mutex> WaitLock(m_TaskMutex);
                    while(!m_Stopping.load() && m_Tasks.size() == 0)
                    {
                        m_TasksAvailableConditional.wait(WaitLock);
                    }
                    if(m_Stopping.load() || m_Tasks.size() == 0)
                    {
                        m_TaskFinishedConditional.notify_one();
                        break;   
                    }
                    m_BusyThreads.fetch_add(1);
                    TaskToExecute = std::move(m_Tasks.front());
                    m_Tasks.pop_front();
                }
                TaskToExecute();
                m_BusyThreads.fetch_add(-1);
                {
                    m_TaskFinishedConditional.notify_one();
                }
            }
            m_TaskFinishedConditional.notify_one();
        }
    public:
        ThreadPool(ThreadPool const&) = delete;
        ThreadPool(ThreadPool&) = delete;
        ThreadPool& operator=(ThreadPool const&) = delete;

        ThreadPool(int ThreadCount,bool Dynamic)
        {
            for(int i = 0; i < ThreadCount;i++)
            {
                m_Threads.push_back(std::thread(&ThreadPool::p_TaskThread,this));
            }
            m_Dynamic = Dynamic;
        }
        ThreadPool(int ThreadCount)
        {
            for(int i = 0; i < ThreadCount;i++)
            {
                m_Threads.push_back(std::thread(&ThreadPool::p_TaskThread,this));
            }
        }
        //default, cores + 1
        ThreadPool() : ThreadPool(std::thread::hardware_concurrency()+1)
        {
               
        }
        ~ThreadPool()
        {
            Stop();
            for(auto& Thread : m_Threads)
            {
                Thread.join();
            }
        }
        void Stop()
        {
           m_Stopping.store(true);    
           {
               std::lock_guard<std::mutex> Lock(m_TaskMutex);
               m_Tasks.clear();
               m_TasksAvailableConditional.notify_all();
           }
        }

        template<typename Func,typename... Args>
        decltype(auto) AddTask(Func&& TaskToAdd,Args&&... Arguments)
        {
            //function returning 
            typedef decltype(TaskToAdd(Arguments...)) ReturnType;
            std::promise<ReturnType> AssociatedPromise;
            auto ReturnValue = AssociatedPromise.get_future();
            auto BoundFunction = std::bind(std::move(TaskToAdd),std::move(Arguments)...);
            {
                std::lock_guard<std::mutex> Lock(m_TaskMutex);
                m_Tasks.emplace_back([BoundFunction=std::move(BoundFunction),AssociatedPromise=std::move(AssociatedPromise)]() mutable
                        {
                            try
                            {
                                if constexpr(! std::is_same_v<ReturnType,void>)
                                {
                                    AssociatedPromise.set_value(BoundFunction());
                                }
                                else
                                {
                                    BoundFunction();
                                    AssociatedPromise.set_value();
                                }
                            }
                            catch(...)
                            {
                                try
                                {
                                    AssociatedPromise.set_exception(std::current_exception());
                                }
                                catch(...)
                                {
                                    
                                }
                            }
                        });

                if(m_Dynamic && m_Tasks.size() > m_BusyThreads.load())
                {
                    m_Threads.push_back(std::thread(&ThreadPool::p_TaskThread,this));
                }
            }
            m_TasksAvailableConditional.notify_one();
            return ReturnValue;
        }
        template<typename Func,typename... Args>
        decltype(auto) AddWhenAvailable(Func&& TaskToAdd,Args&&... Arguments)
        {
            //function returning 
            typedef decltype(TaskToAdd(Arguments...)) ReturnType;
            std::promise<ReturnType> AssociatedPromise;
            auto ReturnValue = AssociatedPromise.get_future();
            auto BoundFunction = std::bind(std::move(TaskToAdd),std::move(Arguments)...);
            {
                std::unique_lock<std::mutex> Lock(m_TaskMutex);
                while(m_BusyThreads.load() == m_Threads.size())
                {
                    m_TaskFinishedConditional.wait(Lock);
                }
                m_Tasks.emplace_back([BoundFunction=std::move(BoundFunction),AssociatedPromise=std::move(AssociatedPromise)]() mutable
                        {
                            try
                            {
                                AssociatedPromise.set_value(BoundFunction());
                            }
                            catch(...)
                            {
                                try
                                {
                                    AssociatedPromise.set_exception(std::current_exception());
                                }
                                catch(...)
                                {
                                    
                                }
                            }
                        });

            }
            m_TasksAvailableConditional.notify_one();
            return ReturnValue;
        }
        void Join()
        {
            std::unique_lock<std::mutex> Lock(m_TaskMutex);
            while(m_Tasks.size() > 0 || m_BusyThreads.load() > 0)
            {
                m_TaskFinishedConditional.wait(Lock);
            }
        }
        //void WaitAll()
        //{
        //    std::unique_lock<std::mutex> TaskFinishedLock(m_TaskFinishedMutex);
        //}
        //decltype(auto) 
    };
};
