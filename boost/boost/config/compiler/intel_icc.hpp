// Intel on Linux C++ compiler setup:

#   if __ICL <= 500
#     define BOOST_WEAK_FUNCTION_TEMPLATE_ORDERING
#   endif

#define BOOST_COMPILER "Intel C++ version " BOOST_STRINGIZE(__ICC)


