// Win32 specific config options:
#define BOOST_PLATFORM "Win32"

#if defined BOOST_DECL_EXPORTS
#  if defined BOOST_DECL_IMPORTS
#     error Not valid to define both BOOST_DECL_EXPORTS and BOOST_DECL_IMPORTS
#  endif
#  define BOOST_DECL __declspec(dllexport)
#elif defined BOOST_DECL_IMPORTS
#  define BOOST_DECL __declspec(dllimport)
#else
#  define BOOST_DECL
#endif

#if defined(__GNUC__) && !defined(BOOST_NO_SWPRINTF)
#  define BOOST_NO_SWPRINTF
#endif

#define BOOST_HAS_WINTHREADS
