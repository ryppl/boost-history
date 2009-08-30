
#ifndef BOOST_MPL_AUX_NUMERIC_CAST_HPP_INCLUDED
#define BOOST_MPL_AUX_NUMERIC_CAST_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /home/evansl/prog_dev/boost-cvs/ro/boost/sandbox/variadic-templates/libs/mpl/test/../../../boost/mpl/aux_/RCS/numeric_cast_utils.hpp,v $
// $Date: 2007/03/09 04:04:38 $
// $Revision: 1.2 $
//ChangeLog:
//  2006-10-23.1203:Larry Evans
//    WHAT:
//      1) cp'ed from $BOOST_ROOT/boost/mpl/aux_/numeric_cast_utils.hpp
//      2) replaced all apply_wrapn with apply_wrap.
//    WHY:
//      1-2) avoid compiler errors at use of apply_wrapn during variadic template compiles.

#include <boost/mpl/numeric_cast.hpp>
#include <boost/mpl/apply_wrap.hpp>
#include <boost/mpl/aux_/config/forwarding.hpp>

namespace boost { namespace mpl { namespace aux {

template<
      typename F
    , typename Tag1
    , typename Tag2
    >
struct cast1st_impl
{
    template< typename N1, typename N2 > struct apply
#if !defined(BOOST_MPL_CFG_NO_NESTED_FORWARDING)
        : apply_wrap< 
              F
            , typename apply_wrap< BOOST_MPL_AUX_NUMERIC_CAST<Tag1,Tag2>,N1 >::type
            , N2
            >
    {
#else
    {
    typedef typename apply_wrap< 
              F
            , typename apply_wrap< BOOST_MPL_AUX_NUMERIC_CAST<Tag1,Tag2>,N1 >::type
            , N2
            >::type type;
#endif
    };
};

template<
      typename F
    , typename Tag1
    , typename Tag2
    >
struct cast2nd_impl
{
    template< typename N1, typename N2 > struct apply
#if !defined(BOOST_MPL_CFG_NO_NESTED_FORWARDING)
        : apply_wrap< 
              F
            , N1
            , typename apply_wrap< BOOST_MPL_AUX_NUMERIC_CAST<Tag2,Tag1>,N2 >::type
            >
    {
#else
    {
        typedef typename apply_wrap< 
              F
            , N1
            , typename apply_wrap< BOOST_MPL_AUX_NUMERIC_CAST<Tag2,Tag1>,N2 >::type
            >::type type;
#endif
    };
};

}}}

#endif // BOOST_MPL_AUX_NUMERIC_CAST_HPP_INCLUDED
