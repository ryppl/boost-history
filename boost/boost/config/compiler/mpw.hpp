// MPW C++ compilers setup:

#   if    defined(__SC__)
#     define BOOST_COMPILER "MPW SCpp version " BOOST_STRINGIZE(__SC__)
#   elif defined(__MRC__)
#     define BOOST_COMPILER "MPW MrCpp version " BOOST_STRINGIZE(__MRC__)
#   else
#     error "Using MPW compiler configuration by mistake.  Please update."
#   endif

#if defined(MPW_CPLUS) && (MPW_CPLUS <= 0x890)
#define BOOST_NO_CV_SPECIALIZATIONS
#define BOOST_NO_DEPENDENT_NESTED_DERIVATIONS
#define BOOST_NO_DEPENDENT_TYPES_IN_TEMPLATE_VALUE_PARAMETERS
#define BOOST_NO_INCLASS_MEMBER_INITIALIZATION
#define BOOST_NO_INTRINSIC_WCHAR_T
#define BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#define BOOST_NO_USING_TEMPLATE

#define BOOST_NO_CWCHAR
#define BOOST_NO_LIMITS_COMPILE_TIME_CONSTANTS

#define BOOST_NO_STD_ALLOCATOR /* actually a bug with const reference overloading */
#endif // 0x890

