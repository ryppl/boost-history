
#ifndef BOOST_MPL_TAG_HPP_INCLUDED
#define BOOST_MPL_TAG_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#include <boost/mpl/if.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/aux_/has_tag.hpp>
#include <boost/mpl/aux_/msvc_eti_base.hpp>
#include <boost/mpl/aux_/config/eti.hpp>

namespace boost { namespace mpl {

namespace aux {
template< typename T > struct tag_impl
{
    typedef typename T::tag type;
};
}

template< typename T > struct tag
#if defined(BOOST_MPL_CFG_MSVC_ETI_BUG)
    : aux::msvc_eti_base< typename if_< 
          aux::has_tag<T>
        , aux::tag_impl<T>
        , void_
        >::type >
#else
    : if_< 
          aux::has_tag<T>
        , aux::tag_impl<T>
        , void_
        >::type
#endif
{
};

}}

#endif // BOOST_MPL_TAG_HPP_INCLUDED
