/*==============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_INTERNAL_BASE_REF_IMPL_HPP
#define BOOST_FUSION_SUPPORT_INTERNAL_BASE_REF_IMPL_HPP

#include <boost/mpl/bool.hpp>

namespace boost { namespace fusion { namespace detail
{
    //cschmidt: workaround until boost::is_reference supports rvalues
#if BOOST_WORKAROUND(BOOST_MSVC, == 1600)
    template<typename T>
    struct is_lrref_stage2
      : mpl::false_
    {};

    template<typename T>
    struct is_lrref_stage2<T&&>
      : mpl::true_
    {};

    template<typename T>
    struct is_lrref
      : is_lrref_stage2<T>
    {};
#else
    template<typename T>
    struct is_lrref
      : mpl::false_
    {};

#   ifndef BOOST_FUSION_NO_RVALUE_REFERENCES
    template<typename T>
    struct is_lrref<T&&>
      : mpl::true_
    {};
#   endif
#endif

    template<typename T>
    struct is_lrref<T&>
      : mpl::true_
    {};

#ifndef BOOST_FUSION_NO_RVALUE_REFERENCES
    template<typename T>
    struct is_rref
      : mpl::false_
    {};

    template<typename T>
    struct is_rref<T&&>
      : mpl::true_
    {};
#endif

#if BOOST_WORKAROUND(BOOST_MSVC, == 1600)
    template<typename T>
    struct remove_reference_stage2
    {
        typedef T type;
    };

    template<typename T>
    struct remove_reference_stage2<T&&>
    {
        typedef T type;
    };

    template<typename T>
    struct remove_reference
      : remove_reference_stage2<T>
    {};
#else
    template<typename T>
    struct remove_reference
    {
        typedef T type;
    };

#   ifndef BOOST_FUSION_NO_RVALUE_REFERENCES
    template<typename T>
    struct remove_reference<T&&>
    {
        typedef T type;
    };
#   endif
#endif

    template<typename T>
    struct remove_reference<T&>
    {
        typedef T type;
    };

#if BOOST_WORKAROUND(BOOST_MSVC, == 1600)
    template<typename T>
    struct add_lref_stage2
    {
        typedef T& type;
    };

    template<typename T>
    struct add_lref_stage2<T&&>
    {
        typedef T&& type;
    };

    template<typename T>
    struct add_lref
      : add_lref_stage2<T>
    {};
#else
    template<typename T>
    struct add_lref
    {
        typedef T& type;
    };

#   ifndef BOOST_FUSION_NO_RVALUE_REFERENCES
    template<typename T>
    struct add_lref<T&&>
    {
        typedef T&& type;
    };
#   endif
#endif

    template<typename T>
    struct add_lref<T&>
    {
        typedef T& type;
    };
}}}

#endif
