// Borland C++ compiler setup:

#   if __BORLANDC__ <= 0x0550
// Borland C++ Builder 4 and 5:
#     define BOOST_NO_MEMBER_TEMPLATE_FRIENDS
#     if __BORLANDC__ == 0x0550
// Borland C++ Builder 5, command-line compiler 5.5:
#       define BOOST_NO_OPERATORS_IN_NAMESPACE
#     endif
#   endif

#if (__BORLANDC__ >= 0x550)
// <climits> is partly broken, some macos define symbols that are really in
// namespace std, so you end up having to use illegal constructs like
// std::DBL_MAX, as a fix we'll just include float.h and have done with:
#include <float.h>
#endif

// all versions so far:
#define BOOST_NO_CV_SPECIALIZATIONS
#define BOOST_NO_CV_VOID_SPECIALIZATIONS
#define BOOST_NO_INTEGRAL_INT64_T
#define BOOST_NO_PRIVATE_IN_AGGREGATE
#define BOOST_NO_DEPENDENT_NESTED_DERIVATIONS
#define BOOST_NO_SWPRINTF
#define BOOST_NO_USING_TEMPLATE

#define BOOST_COMPILER "Borland C++ version " BOOST_STRINGIZE(__BORLANDC__)


