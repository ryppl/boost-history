//-----------------------------------------------------------------------------
// boost mpl/fold.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2001-02
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_FOLD_HPP_INCLUDED
#define BOOST_MPL_FOLD_HPP_INCLUDED

#include <boost/mpl/apply.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/begin_end.hpp>
#include "boost/mpl/lambda.hpp"

namespace boost {
namespace mpl {

namespace aux {

template<
      typename Iterator
    , typename LastIterator
    , typename State
    , typename ForwardOp
    , typename BackwardOp
    >
struct iter_fold_impl
{
    typedef typename apply2<
          BackwardOp
        , Iterator
        , typename iter_fold_impl<
              typename Iterator::next
            , LastIterator
            , typename apply2<ForwardOp,State,Iterator>::type
            , ForwardOp
            , BackwardOp
            >::type
        >::type type;
};

template<
      typename LastIterator
    , typename State
    , typename ForwardOp
    , typename BackwardOp
    >
struct iter_fold_impl<
      LastIterator
    , LastIterator
    , State
    , ForwardOp
    , BackwardOp
    >
{
    typedef State type;
};

struct select2nd
{
    template <class T, class U>
    struct apply
    {
        typedef U type;
    };
};

} // namespace aux

template<
      typename Sequence
    , typename State
    , typename ForwardOp
    , typename BackwardOp = aux::select2nd
    >
struct iter_fold
{
    typedef typename aux::iter_fold_impl<
          typename begin<Sequence>::iterator
        , typename end<Sequence>::iterator
        , State
        , ForwardOp
        , BackwardOp
        >::type type;
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_FOLD_HPP_INCLUDED
