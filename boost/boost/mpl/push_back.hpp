
#ifndef BOOST_MPL_PUSH_BACK_HPP_INCLUDED
#define BOOST_MPL_PUSH_BACK_HPP_INCLUDED

// + file: boost/mpl/push_back.hpp
// + last modified: 10/jun/03

// Copyright (c) 2000-03
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.
//
// See http://www.boost.org/libs/mpl for documentation.

#include "boost/mpl/push_back_fwd.hpp"
#include "boost/mpl/sequence_tag.hpp"
#include "boost/mpl/aux_/push_back_impl.hpp"
#include "boost/mpl/aux_/void_spec.hpp"
#include "boost/mpl/aux_/lambda_support.hpp"

namespace boost {
namespace mpl {

template<
      typename BOOST_MPL_AUX_VOID_SPEC_PARAM(Seq)
    , typename BOOST_MPL_AUX_VOID_SPEC_PARAM(T)
    >
struct push_back
    : push_back_impl< typename sequence_tag<Seq>::type >
        ::template apply< Seq,T >
{
    BOOST_MPL_AUX_LAMBDA_SUPPORT(2,push_back,(Seq,T))
};


template< 
      typename BOOST_MPL_AUX_VOID_SPEC_PARAM(Seq)
    >
struct has_push_back
    : has_push_back_impl< typename sequence_tag<Seq>::type >
        ::template apply< Seq >
{
    BOOST_MPL_AUX_LAMBDA_SUPPORT(1,has_push_back,(Seq))
};

BOOST_MPL_AUX_VOID_SPEC(2, push_back)
BOOST_MPL_AUX_VOID_SPEC(1, has_push_back)

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_PUSH_BACK_HPP_INCLUDED
