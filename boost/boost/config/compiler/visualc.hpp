// Microsoft Visual C++ compiler setup:

#   define BOOST_MSVC _MSC_VER

    // turn off the warnings before we #include anything
#   pragma warning( disable : 4786 ) // ident trunc to '255' chars in debug info
#   pragma warning( disable : 4503 ) // warning: decorated name length exceeded

#   if _MSC_VER <= 1200  // 1200 == VC++ 6.0
#     define BOOST_NO_INCLASS_MEMBER_INITIALIZATION
#     define BOOST_NO_PRIVATE_IN_AGGREGATE
#     define BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
#     define BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS

#     define BOOST_NO_INTEGRAL_INT64_T
#     define BOOST_NO_INTRINSIC_WCHAR_T

//    VC++ 6.0 has member templates but they have numerous problems including
//    cases of silent failure, so for safety we define:
#     define BOOST_NO_MEMBER_TEMPLATES
//    For VC++ experts wishing to attempt workarounds, we define:
#     define BOOST_MSVC6_MEMBER_TEMPLATES

#     define BOOST_NO_MEMBER_TEMPLATE_FRIENDS
#     define BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#     define BOOST_NO_DEPENDENT_TYPES_IN_TEMPLATE_VALUE_PARAMETERS

#     define BOOST_NO_CV_VOID_SPECIALIZATIONS
#     define BOOST_WEAK_FUNCTION_TEMPLATE_ORDERING
#     define BOOST_NO_USING_TEMPLATE
#     define BOOST_NO_SWPRINTF

#    endif

#if _MSC_VER == 1300  // VC7 Beta 1
#  define BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
#  define BOOST_NO_CV_VOID_SPECIALIZATIONS
#  define BOOST_WEAK_FUNCTION_TEMPLATE_ORDERING
#  define BOOST_NO_INCLASS_MEMBER_INITIALIZATION
#  define BOOST_NO_MEMBER_TEMPLATE_FRIENDS
#  define BOOST_NO_MEMBER_TEMPLATES
#  define BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#  define BOOST_NO_PRIVATE_IN_AGGREGATE
#  define BOOST_NO_POINTER_TO_MEMBER_CONST
#  define BOOST_NO_INTEGRAL_INT64_T
#  define BOOST_NO_INTRINSIC_WCHAR_T
#  define BOOST_MSVC6_MEMBER_TEMPLATES
#  define BOOST_NO_SWPRINTF
#endif

#define BOOST_COMPILER "Microsoft Visual C++ version " BOOST_STRINGIZE(_MSC_VER)


