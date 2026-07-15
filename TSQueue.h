
#include <mutex>
#include <condition_variable>
#include <deque>
#include <chrono>
namespace MBUtility
{


    template<typename T>
    class TSQueue
    {
        std::mutex m_DataMutex;
        std::condition_variable m_DataConditional;
        std::deque<T> m_Data;

        bool m_Closed = false;
    public:
        bool PopFront(T& OutValue)
        {
            return PopFront(OutValue,std::chrono::seconds(std::chrono::seconds(std::numeric_limits<int>::max())));
        }
        template<typename Period,typename Rep>
        bool PopFront(T& OutValue, std::chrono::duration<Rep,Period> const& Duration)
        {
            bool ReturnValue = true;
            std::unique_lock Lock(m_DataMutex);
            m_DataConditional.wait_for(Lock,Duration,[&](){ return m_Closed || m_Data.size() > 0; });
            if(m_Data.size() == 0)
            {
                return false;
            }
            OutValue = std::move(m_Data.front());
            m_Data.pop_front();
            return ReturnValue;
        }
        void Enqueue(T Value)
        {
            Emplace(std::move(Value));
        }
        template<typename... ArgTypes>
        void Emplace(ArgTypes&&... Args)
        {
            std::lock_guard Lock(m_DataMutex);
            m_Data.emplace_back(std::forward<ArgTypes>(Args)...);
            m_DataConditional.notify_one();
        }
        ~TSQueue()
        {
            std::lock_guard Lock(m_DataMutex);
            m_Closed = true;
            m_DataConditional.notify_one();
        }
    };
}
