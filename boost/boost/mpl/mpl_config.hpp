//-----------------------------------------------------------------------------
// boost mpl/mpl_config.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-01
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_CONFIG_HPP
#define BOOST_MPL_CONFIG_HPP

#include "boost/config.hpp"

#if defined(BOOST_MSVC)
#   include "boost/type_traits/same_traits.hpp"
#endif

namespace boost {
namespace mpl {

#if defined(__MWERKS__) && (__MWERKS__ <= 0x4000)
#   define BOOST_NO_WORKING_IS_CONVERTIBLE_IMPLEMENTATION
#endif

#if (defined(BOOST_MSVC) && (BOOST_MSVC <= 1200)) || defined(__BORLANDC__)
#   define BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS
#endif

#if defined(BOOST_MSVC)
#   define BOOST_MSVC_OUTSIDE_TYPENAME
#else
#   define BOOST_MSVC_OUTSIDE_TYPENAME typename
#endif

#if defined(BOOST_MSVC)

namespace detail {
template<class T>
struct msvc_never_true
{
    struct fake_type;
    BOOST_STATIC_CONSTANT(bool, value = (boost::is_same<T, fake_type>::value));
};
}

#   define BOOST_MPL_DEPENDENT_TEMPLATE_TYPEDEF(type_name, template_name, param, result_type)           \
        template<bool> struct type_name##_wrapper : type_name {};                                       \
        template<>     struct type_name##_wrapper<true> { template<class> struct template_name; };      \
        typedef typename type_name##_wrapper< mpl::detail::msvc_never_true<type_name>::value >          \
                    ::template template_name<param> result_type

#   define BOOST_MPL_NON_TYPE_DEPENDENT_TEMPLATE_TYPEDEF(type_name, template_name, param, result_type)  \
        template<bool> struct type_name##_wrapper : type_name {};                                       \
        template<>     struct type_name##_wrapper<true> { template<long> struct template_name; };       \
        typedef typename type_name##_wrapper< mpl::detail::msvc_never_true<type_name>::value >          \
                    ::template template_name<param> result_type

#   define BOOST_MPL_DEPENDENT_TEMPLATE_TYPEDEF2(type_name, template_name, param1, param2, result_type) \
        template<bool> struct type_name##_wrapper : type_name {};                                       \
        template<> struct type_name##_wrapper<true> { template<class P_, class> struct template_name; };\
        typedef typename type_name##_wrapper< mpl::detail::msvc_never_true<type_name>::value >          \
                    ::template template_name<param1, param2> result_type

#else
#   define BOOST_MPL_DEPENDENT_TEMPLATE_TYPEDEF(type_name, template_name, param, result_type)           \
        typedef typename type_name::template template_name<param> result_type

#   define BOOST_MPL_NON_TYPE_DEPENDENT_TEMPLATE_TYPEDEF(type_name, template_name, param, result_type)  \
        typedef typename type_name::template template_name<param> result_type

#   define BOOST_MPL_DEPENDENT_TEMPLATE_TYPEDEF2(type_name, template_name, param1, param2, result_type) \
        typedef typename type_name::template template_name<param1, param2> result_type

#endif // BOOST_MSVC

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_CONFIG_HPP
