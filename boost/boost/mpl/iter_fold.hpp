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

#include "boost/mpl/apply.hpp"
#include "boost/mpl/begin_end.hpp"
#include <boost/mpl/select_type.hpp>

namespace boost {
namespace mpl {

namespace aux {

template <
      typename Iterator
    , typename LastIterator
    , typename State
    , typename ForwardOp
    , typename BackwardOp
    >
struct iter_fold_impl_more;

template <
    typename State
    >
struct iter_fold_impl_done;

template <
      typename Iterator
    , typename LastIterator
    , typename State
    , typename ForwardOp
    , typename BackwardOp
    >
struct iter_fold_impl
    : select_type<
      is_same<Iterator,LastIterator>::value
      , iter_fold_impl_done<State>
      , iter_fold_impl_more<Iterator, LastIterator, State, ForwardOp, BackwardOp>
        >::type
{
};
    
template<
      typename Iterator
    , typename LastIterator
    , typename State
    , typename ForwardOp
    , typename BackwardOp
    >
struct iter_fold_impl_more
{
 private:
    typedef apply2<ForwardOp,State,Iterator> forward;
    
    typedef iter_fold_impl<
        typename Iterator::next
        , LastIterator
        , typename forward::type
        , ForwardOp
        , BackwardOp
        > recursion;

    typedef typename recursion::type inner_t;
    
    typedef apply2<
          BackwardOp
        , Iterator
        , inner_t
        > backward;

 public:
    typedef typename backward::type type;

    static void execute()
    {
        ForwardOp::template apply<State,Iterator>::execute();
        recursion::execute();
        BackwardOp::template apply<Iterator,inner_t>::execute();
    }

    template <class T>
    static void execute(T x)
    {
        ForwardOp::template apply<State,Iterator>::execute(x);
        recursion::execute(x);
        BackwardOp::template apply<Iterator,inner_t>::execute(x);
    }

    // We're not ready for this yet, are we?
#if 0
    template <class T>
    typename backward::template result_type<
        typename recursion::template result_type<
            typename forward::template result_type<T>::type
        >::type
    >::type execute2(T x)
    {
        return backward::execute(
            recursion::execute(
                forward::execute(x)));
    }
#endif 
};

template<
    typename State
    >
struct iter_fold_impl_done
{
    typedef State type;

    static void execute()
    {
    }

    template <class T>
    static void execute(T)
    {
    }
};

struct select2nd
{
    template <class T, class U>
    struct apply
    {
        typedef U type;
        
        static void execute() {}
        
        template <class V>
        static void execute(V) {}
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
    : aux::iter_fold_impl<
          typename begin<Sequence>::iterator
        , typename end<Sequence>::iterator
        , State
        , ForwardOp
        , BackwardOp
        >
{
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_FOLD_HPP_INCLUDED
