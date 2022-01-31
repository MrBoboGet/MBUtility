#pragma once
namespace MBUtility
{
	constexpr bool IsWindows()
	{
#ifdef _WIN32
		return(true);
#else
		return(false);
#endif // _WIN32

	}
#ifdef _WIN32
	#define MBWindows
#endif // _WIN32

#ifdef __linux__
	#define MBLinux
#endif

}