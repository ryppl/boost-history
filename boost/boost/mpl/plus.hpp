
#ifndef BOOST_MPL_PLUS_HPP_INCLUDED
#define BOOST_MPL_PLUS_HPP_INCLUDED

// Copyright (c) Aleksey Gurtovoy 2000-2003
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#include <boost/mpl/numeric_cast.hpp>
#include <boost/mpl/apply_wrap.hpp>
#include <boost/mpl/aux_/largest_int.hpp>
#include <boost/mpl/aux_/value_wknd.hpp>

namespace boost { namespace mpl {

namespace aux {

template< 
      bool c_
    , typename Tag1
    , typename Tag2
    >
struct plus_impl_cast
{
    template< typename N1, typename N2 > struct apply
        : plus_impl<Tag2,Tag2>::template apply<
              typename apply_wrap1< numeric_cast<Tag2,Tag1>,N2 >::type
            , N2
            >
    {
    };
};

template< 
        typename Tag1
    , typename Tag2
    >
struct plus_impl_cast<false,Tag1,Tag2>
{
    template< typename N1, typename N2 > struct apply
        : plus_impl<Tag1,Tag1>::template apply<
              N1
            , typename apply_wrap1< numeric_cast<Tag1,Tag2>,N2 >::type
            >
    {
    };
};

} // namespace aux 


template< 
      typename Tag1
    , typename Tag2
    >
struct plus_impl
    : aux::plus_impl_cast< (Tag1::value < Tag2::value),Tag1,Tag2 >
{
};

template<> 
struct plus_impl<integral_c_tag,integral_c_tag>
{
    template< typename N1, typename N2 > struct apply
        : aux::plus_c<
              typename aux::largest_int< 
                  typename N1::value_type
                , typename N2::value_type
                >::type
            , BOOST_MPL_AUX_MSVC_VALUE_WKND(T1)::value
            , BOOST_MPL_AUX_MSVC_VALUE_WKND(T2)::value
            >
    {
    };
};


template< typename T > struct plus_tag
{
    typedef typename T::tag type;
};

template< 
      typename N1
    , typename N2
    >
struct plus2
    : plus_impl<
          typename plus_tag<N1>::type
        , typename plus_tag<N2>::type
        >::template apply<N1,N2>::type
{
};

template<
      typename N1
    , typename N2 = na
    , typename N3 = na
    >
struct plus
    : plus2< N1, plus2<N3,N2> >
{
};

}}

#endif // BOOST_MPL_PLUS_HPP_INCLUDED
