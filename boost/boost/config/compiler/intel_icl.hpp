// Intel on Win32 C++ compiler setup:

#   if __ICL <= 500
#     define BOOST_WEAK_FUNCTION_TEMPLATE_ORDERING
#     define BOOST_NO_INTRINSIC_WCHAR_T // tentative addition - required for VC6 compatibility? (added by JM 19 Feb 2001)
#   endif

#define BOOST_NO_SWPRINTF

#define BOOST_COMPILER "Intel C++ version " BOOST_STRINGIZE(__ICL)

