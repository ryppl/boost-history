
#if defined(__IBMCPP__)
#define BOOST_NO_CV_TEMPLATE_TEMPLATES
#elif defined(_MSC_VER)
#else
#error wrong compiler
#endif
