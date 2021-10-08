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
}