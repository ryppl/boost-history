
#ifndef BOOST_MPL_AUX_HAS_REBIND_HPP_INCLUDED
#define BOOST_MPL_AUX_HAS_REBIND_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#include <boost/mpl/aux_/config/workaround.hpp>
#include <boost/mpl/aux_/config/msvc.hpp>

#if BOOST_WORKAROUND(BOOST_MSVC, <= 1200)
#   include <boost/mpl/has_xxx.hpp>
#   include <boost/mpl/if.hpp>
#   include <boost/mpl/bool.hpp>
#   include <boost/mpl/aux_/msvc_is_class.hpp>
#else
#   include <boost/mpl/aux_/type_wrapper.hpp>
#   include <boost/mpl/aux_/config/static_constant.hpp>
#endif

namespace boost { namespace mpl { namespace aux {

#if BOOST_WORKAROUND(BOOST_MSVC, <= 1200)

BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_rebind_impl, rebind, false)

template< typename T >
struct has_rebind
    : if_< 
          msvc_is_class<T>
        , has_rebind_impl<T>
        , bool_<false>
        >::type
{
};

#else 

struct has_rebind_tag {};
yes_tag operator|(has_rebind_tag, void const volatile*);

template< typename T >
struct has_rebind
{
    static has_rebind<T>* get();
    BOOST_STATIC_CONSTANT(bool, value = 
          sizeof(has_rebind_tag() | get()) == sizeof(char)
        );
};

#endif // BOOST_MSVC

}}}

#endif // BOOST_MPL_AUX_HAS_REBIND_HPP_INCLUDED
