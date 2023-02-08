#include <functional>
#include <utility>
#include <vector>
namespace MBUtility
{
    template<typename ItType>
    bool MergeFinished(ItType Begin,ItType End)
    {
        bool ReturnValue = true; 
        while(Begin != End)
        {
            if(Begin.first != Begin.second)
            {
                ReturnValue = false;
                break;
            }   
        }
        return(ReturnValue);
    }
    template<typename T,typename ItType,typename CompClass = std::less<T>>
    std::vector<T> Merge(ItType Begin,ItType End,CompClass Compare = std::less<T>())
    {
        typedef decltype(Begin->begin()) ScalarItType;
        std::vector<T> ReturnValue;
        std::vector<std::pair<ScalarItType,ScalarItType>> Iterators;
        while(Begin != End)
        {
            Iterators.push_back(std::pair<ScalarItType,ScalarItType>(Begin->begin(),Begin->end()));
            ++Begin;
        }
        while(true)
        {
            ScalarItType* ElementToAddIt;
            bool IsAssigned = false;
            for(auto& It : Iterators)
            {
                if(!IsAssigned && It.first != It.second)
                {
                    ElementToAddIt = &It.first;
                    IsAssigned = true;
                }
                else if(IsAssigned && It.first != It.second)
                {
                    if(Compare(*It.first,**ElementToAddIt))
                    {
                        ElementToAddIt = &It.first;   
                    }
                }
            } 
            if(!IsAssigned)
            {
                break;   
            }
            ReturnValue.push_back(**ElementToAddIt);
            ++(*ElementToAddIt);
        } 
        return(ReturnValue);
    }
    template<typename T,typename ItType,typename Compare = std::less<T>>
    std::vector<T> Merge_NoDuplicates(ItType Begin,ItType End)
    {
        std::vector<T> ReturnValue;
        
        return(ReturnValue);
    }
};
