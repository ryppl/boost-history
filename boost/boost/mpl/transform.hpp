
#ifndef BOOST_MPL_TRANSFORM_HPP_INCLUDED
#define BOOST_MPL_TRANSFORM_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
// Copyright David Abrahams 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#include <boost/mpl/fold.hpp>
#include <boost/mpl/reverse_fold.hpp>
#include <boost/mpl/pair_view.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/bind.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/aux_/na.hpp>
#include <boost/mpl/aux_/inserter_algorithm.hpp>

namespace boost { namespace mpl {

namespace aux { 

template< 
      typename Seq
    , typename Op
    , typename In
    >
struct transform1_impl
    : fold< 
          Seq
        , typename In::state
        , bind2< typename lambda< typename In::operation >::type
            , _1
            , bind1< typename lambda<Op>::type, _2>
            > 
        >
{
};

template< 
      typename Seq
    , typename Op
    , typename In
    >
struct reverse_transform1_impl
    : reverse_fold< 
          Seq
        , typename In::state
        , bind2< typename lambda< typename In::operation >::type
            , _1
            , bind1< typename lambda<Op>::type, _2>
            > 
        >
{
};

template< 
      typename Seq1
    , typename Seq2
    , typename Op
    , typename In
    >
struct transform2_impl
    : fold< 
          pair_view<Seq1,Seq2>
        , typename In::state
        , bind2< typename In::operation
            , _1
            , bind2<Op
                , bind1<first<>,_2>
                , bind1<second<>,_2>
                >
            > 
        >
{
};

template< 
      typename Seq1
    , typename Seq2
    , typename Op
    , typename In
    >
struct reverse_transform2_impl
    : reverse_fold< 
          pair_view<Seq1,Seq2>
        , typename In::state
        , bind2< typename lambda< typename In::operation >::type
            , _1
            , bind2< typename lambda< Op >::type
                , bind1<first<>,_2>
                , bind1<second<>,_2>
                >
            > 
        >
{
};

} // namespace aux 

BOOST_MPL_AUX_INSERTER_ALGORITHM_DEF(3, transform1)                    
BOOST_MPL_AUX_INSERTER_ALGORITHM_DEF(4, transform2)
    
#define BOOST_MPL_transform_def(transform_)                                     \
                                                                                \
  template<                                                                     \
        typename BOOST_MPL_AUX_NA_PARAM(Seq1)                                   \
      , typename BOOST_MPL_AUX_NA_PARAM(Seq2OrOperation)                        \
      , typename BOOST_MPL_AUX_NA_PARAM(OperationOrInserter)                    \
      , typename BOOST_MPL_AUX_NA_PARAM(Inserter)                               \
      >                                                                         \
  struct transform_                                                             \
    : if_<                                                                      \
            or_<                                                                \
                is_na<OperationOrInserter>                                      \
              , not_< is_sequence<Seq2OrOperation> >                            \
              >                                                                 \
          , transform_##1<Seq1,Seq2OrOperation,OperationOrInserter>             \
          , transform_##2<Seq1,Seq2OrOperation,OperationOrInserter,Inserter>    \
          >::type                                                               \
  {                                                                             \
  };                                                                            \
                                                                                \
  BOOST_MPL_AUX_NA_SPEC(4, transform_)                                          \
  /**/
    
BOOST_MPL_transform_def(transform)
BOOST_MPL_transform_def(reverse_transform)
    
#undef BOOST_MPL_transform_def
    
}}

#endif // BOOST_MPL_TRANSFORM_HPP_INCLUDED
