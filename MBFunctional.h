#include <vector>
#include <type_traits>
namespace MBUtility
{
	// SFINAE test
	template <typename T>
	class HasBegin
	{
	private:
		typedef char YesType[1];
		typedef char NoType[2];

		template <typename C> static YesType& test(decltype(void(std::declval<C&>().begin()), int{}));
		template <typename C> static NoType& test(...);


	public:
		enum { value = sizeof(test<T>(0)) == sizeof(YesType) };
	};

	template <typename T>
	class HasSize
	{
	private:
		typedef char YesType[1];
		typedef char NoType[2];

		template <typename C> static YesType& test(decltype(&C::size));
		template <typename C> static NoType& test(...);


	public:
		enum { value = sizeof(test<T>(0)) == sizeof(YesType) };
	};

	template<typename ReturnType,typename FunctionType,typename IterationType>
	std::vector<ReturnType> Apply(IterationType Begin, IterationType End, FunctionType FunctionTouse)
	{
		std::vector<ReturnType> ReturnValue = {};
		while (Begin != End)
		{
			ReturnValue.push_back(FunctionTouse(*Begin));
			Begin++;
		}
		return(ReturnValue);
	}
	template<typename ReturnType, typename FunctionType, typename IterationType,typename ... ExtraArguments>
	std::vector<ReturnType> Apply(IterationType Begin, IterationType End, FunctionType FunctionTouse,ExtraArguments ... Args)
	{
		std::vector<ReturnType> ReturnValue = {};
		while (Begin != End)
		{
			ReturnValue.push_back(FunctionTouse(*Begin,Args...));
			Begin++;
		}
		return(ReturnValue);
	}

	template<typename ReturnType,typename TypeToFlatten>
	std::vector<ReturnType> Flatten(TypeToFlatten const& Container)
	{
		std::vector<ReturnType> ReturnValue = {};
		auto Begin = Container.begin();
		auto End = Container.end();
		while (Begin != End)
		{
			if constexpr (HasBegin<decltype(*Begin)>::value && !std::is_same<ReturnType,std::remove_const_t<std::remove_reference_t<decltype(*Begin)>>>::value)
			{
				std::vector<ReturnType> IntermediaryValues = Flatten<ReturnType>(*Begin);
				for (size_t i = 0; i < IntermediaryValues.size(); i++)
				{
					ReturnValue.push_back(std::move(IntermediaryValues[i]));
				}
			}
			else
			{
				ReturnValue.push_back(*Begin);
			}
			Begin++;
		}
		return(ReturnValue);
	}
}