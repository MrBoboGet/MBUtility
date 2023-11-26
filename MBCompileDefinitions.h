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

//Hack, assume C17
#define MBCpp17
#define MBCppVersion 17

#if __cplusplus == 201703L
#define MBCpp17
#define MBCppVersion 17
#endif


#ifdef _WIN32
	#define MBWindows
#endif // _WIN32

#ifdef __linux__
	#define MBLinux
#endif

#ifdef __linux
	#define MBLinux
#endif

#ifdef linux
#define MBLinux
#endif

#ifdef MBLinux
	#define MBPosix
#endif

    constexpr bool IsPOSIX()
    {
#ifdef MBPosix
        return(true);
#else 
        return(false);
#endif
    }
}
