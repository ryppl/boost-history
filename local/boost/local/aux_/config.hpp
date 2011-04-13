
#ifndef BOOST_LOCAL_AUX_CONFIG_HPP_
#define BOOST_LOCAL_AUX_CONFIG_HPP_

#include "../config.hpp"

// If it is possible to pass a local class as a template parameter. This is
// not possible on ISO C++ but it is possible on C++03 extensions, MSVC, etc.
#if !defined(BOOST_LOCAL_CONFIG_COMPLIANT) && defined(_MSC_VER)
#   define BOOST_LOCAL_AUX_CONFIG_LOCAL_CLASS_AS_TEMPLATE_PARAMETER
#else
#   undef BOOST_LOCAL_AUX_CONFIG_LOCAL_CLASS_AS_TEMPLATE_PARAMETER
#endif

#endif // #include guard

