// Metrowerks C++ compiler setup:

#   if __MWERKS__ <= 0x2401  // 6.2
#     define BOOST_NO_MEMBER_TEMPLATE_FRIENDS
#   endif
#   if __MWERKS__ <= 0x2301  // 5.3
#     define BOOST_WEAK_FUNCTION_TEMPLATE_ORDERING
#     define BOOST_NO_POINTER_TO_MEMBER_CONST
#     define BOOST_NO_DEPENDENT_TYPES_IN_TEMPLATE_VALUE_PARAMETERS
#     define BOOST_NO_MEMBER_TEMPLATE_KEYWORD
#   endif

#if !__option(wchar_type)
#   define BOOST_NO_INTRINSIC_WCHAR_T
#endif


#define BOOST_COMPILER "Metrowerks CodeWarrior C++ version " BOOST_STRINGIZE(__MWERKS__)






