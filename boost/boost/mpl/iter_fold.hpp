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

template <class Iterator, class State>
struct iter_fold_done
{
    typedef State type;
    typedef Iterator iterator;

    static void execute()
    {
    }

    template <class T>
    static void execute(T)
    {
    }
};

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
template <class Iterator, class LastIterator, class ForwardOp, class State>
struct iter_fold_next : apply2<ForwardOp,State,Iterator>
{
    typedef typename Iterator::next iterator;
};

template <class LastIterator, class ForwardOp, class State>
struct iter_fold_next<LastIterator, LastIterator, ForwardOp, State>
    : iter_fold_done<LastIterator,State>
{
};

template <class Iterator, class LastIterator, class State, class BackwardOp>
struct iter_fold_prev : apply2<BackwardOp,Iterator,State>
{
};

template <class LastIterator, class State, class BackwardOp>
struct iter_fold_prev<LastIterator,LastIterator,State,BackwardOp>
    : iter_fold_done<LastIterator,State>
{
    typedef State type;
};
#else 
template <class Iterator, class ForwardOp, class State>
struct iter_fold_more : apply2<ForwardOp,State,Iterator>
{
    typedef typename Iterator::next iterator;
};

template <class Iterator, class LastIterator, class ForwardOp, class State>
struct iter_fold_next
    : select_type<is_same<Iterator,LastIterator>::value
                  , iter_fold_done<Iterator,State>
                  , iter_fold_more<Iterator,ForwardOp,State>
      >::type
{};

template <class Iterator, class LastIterator, class State, class BackwardOp>
struct iter_fold_prev
    : select_type<is_same<Iterator,LastIterator>::value
                  , iter_fold_done<Iterator,State>
                  , apply2<BackwardOp,Iterator,State>
      >::type
{};
#endif

template<
      typename Iterator
    , typename LastIterator
    , typename State
    , typename ForwardOp
    , typename BackwardOp
    >
struct iter_fold_impl
{
 private:
    typedef iter_fold_next<Iterator,LastIterator,ForwardOp,State> next1;
    typedef iter_fold_next<typename next1::iterator,LastIterator,ForwardOp,typename next1::type> next2;
    typedef iter_fold_next<typename next2::iterator,LastIterator,ForwardOp,typename next2::type> next3;
    typedef iter_fold_next<typename next3::iterator,LastIterator,ForwardOp,typename next3::type> next4;
    
    typedef typename select_type<
        is_same<typename next3::iterator,LastIterator>::value
        , iter_fold_done<Iterator,typename next4::type>
        , iter_fold_impl<
          typename next4::iterator
            , LastIterator
            , typename next4::type
            , ForwardOp
            , BackwardOp>
        >::type recursion;

    typedef typename recursion::type inner_t;
    
    typedef iter_fold_prev<typename next3::iterator,LastIterator,inner_t,BackwardOp> prev4;
    typedef iter_fold_prev<typename next2::iterator,LastIterator,typename prev4::type,BackwardOp> prev3;
    typedef iter_fold_prev<typename next1::iterator,LastIterator,typename prev3::type,BackwardOp> prev2;
    typedef iter_fold_prev<Iterator,LastIterator,typename prev2::type,BackwardOp> prev1;
 public:
    typedef typename prev1::type type;

    static void execute()
    {
        next1::execute();
        next2::execute();
        next3::execute();
        next4::execute();
        
        recursion::execute();
        
        prev4::execute();
        prev3::execute();
        prev2::execute();
        prev1::execute();
    }

    template <class T>
    static void execute(T x)
    {
        next1::execute(x);
        next2::execute(x);
        next3::execute(x);
        next4::execute(x);
        
        recursion::execute(x);
        
        prev4::execute(x);
        prev3::execute(x);
        prev2::execute(x);
        prev1::execute(x);
    }
};

struct select2nd
{
    template <class T, class U>
    struct apply : iter_fold_done<T,U>
    {
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
