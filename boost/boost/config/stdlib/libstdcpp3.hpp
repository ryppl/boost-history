// config for libstdc++ v3
// not much to go in here:

#define BOOST_STDLIB "GNU libstdc++ version " BOOST_STRINGIZE(__GLIBCPP__)

#ifndef _GLIBCPP_USE_WCHAR_T
#  define BOOST_NO_CWCHAR
#  define BOOST_NO_CWCTYPE
#endif
 

