
#ifndef BOOST_MPL_TRANSFORM_HPP_INCLUDED
#define BOOST_MPL_TRANSFORM_HPP_INCLUDED

// + file: boost/mpl/transform.hpp
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

#include "boost/mpl/fold.hpp"
#include "boost/mpl/fold_backward.hpp"
#include "boost/mpl/back_inserter.hpp"
#include "boost/mpl/front_inserter.hpp"
#include "boost/mpl/push_back.hpp"
#include "boost/mpl/push_front.hpp"
#include "boost/mpl/clear.hpp"
#include "boost/mpl/if.hpp"
#include "boost/mpl/lambda.hpp"
#include "boost/mpl/bind.hpp"
#include "boost/mpl/aux_/void_spec.hpp"

namespace boost {
namespace mpl {

BOOST_MPL_AUX_AGLORITHM_NAMESPACE_BEGIN

/*
it's gonna be a little bit more complicated, eventually:
template< 
      typename BOOST_MPL_AUX_VOID_SPEC_PARAM(Seq1)
    , typename BOOST_MPL_AUX_VOID_SPEC_PARAM(Seq2_or_Op)
    , typename Op_or_Ins = void_
    , typename Ins = void_
    >

*/

template< 
      typename BOOST_MPL_AUX_VOID_SPEC_PARAM(Seq)
    , typename BOOST_MPL_AUX_VOID_SPEC_PARAM(Op)
    , typename Ins = void_
    >
struct transform
    : fold< 
          Seq
        , typename Ins::first
        , bind2< typename Ins::second, _1, bind1< typename lambda<Op>::type, _2> > 
        >
{
};

// fwd for 'transform' specialization below
template< typename Seq, typename Op, typename Ins = void_ >
struct transform_backward;

template< 
      typename Seq
    , typename Op
    >
struct transform<Seq,Op,void_>
    : if_< has_push_back<Seq>
        , transform< Seq,Op,back_inserter< typename clear<Seq>::type > >
        , transform_backward<Seq,Op,front_inserter< typename clear<Seq>::type > >
        >::type
{
};

template< 
      typename Seq
    , typename Op
    , typename Ins
    >
struct transform_backward
    : fold_backward< 
          Seq
        , typename Ins::first
        , bind2< typename Ins::second, _1, bind1<typename lambda<Op>::type, _2> > 
        >
{
};

template< 
      typename Seq
    , typename Op
    >
struct transform_backward<Seq,Op,void_>
    : if_< has_push_front<Seq>
        , transform_backward<Seq,Op,front_inserter< typename clear<Seq>::type > >
        , transform< Seq,Op,back_inserter< typename clear<Seq>::type > >
        >::type
{
};

BOOST_MPL_AUX_AGLORITHM_NAMESPACE_END

BOOST_MPL_AUX_ALGORITHM_VOID_SPEC(2, transform)

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_TRANSFORM_HPP_INCLUDED
