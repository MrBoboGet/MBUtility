#include <deque>
namespace MBUtility
{
	template<typename T> 
	class FIFOBuffer
	{
	private:
		std::deque<T> m_InternalBuffer = {};
	public:
		void InsertData(const T* DataToInsert, size_t NumberOfElements)
		{
			m_InternalBuffer.insert(m_InternalBuffer.end(), DataToInsert, DataToInsert + NumberOfElements);
		}
		template<typename ITType>
		void InsertData(ITType Begin, ITType End)
		{
			m_InternalBuffer.insert(m_InternalBuffer.end(), Begin, End);
		}

		void DiscardFront(size_t ElementsToDiscard)
		{
			for (size_t i = 0; i < ElementsToDiscard; i++)
			{
				m_InternalBuffer.pop_front();
			}
		}
		void ExtractData(T* OutBuffer, size_t NumberOfElements)
		{
			for (size_t i = 0; i < NumberOfElements; i++)
			{
				if (m_InternalBuffer.size() == 0)
				{
					break;
				}
				OutBuffer[i] = std::move(m_InternalBuffer.front());
				m_InternalBuffer.pop_front();
			}
		}
		template<typename IteratorType>
		void ExtractData(IteratorType Begin, IteratorType End)
		{
			while (Begin != End && m_InternalBuffer.size() > 0)
			{
				*Begin = std::move(m_InternalBuffer.front());
				m_InternalBuffer.pop_front();
				Begin++;
			}
		}

		size_t size()
		{
			return(m_InternalBuffer.size());
		}
	};
}