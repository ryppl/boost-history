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
#include "boost/mpl/select_type.hpp"

#include "boost/preprocessor/repeat.hpp"
#include "boost/preprocessor/inc.hpp"
#include "boost/preprocessor/dec.hpp"
#include "boost/preprocessor/cat.hpp"

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
template <class Iterator, class LastIterator, class State, class ForwardOp>
struct iter_fold_next : apply2<ForwardOp,State,Iterator>
{
    typedef typename Iterator::next iterator;
};

template <class LastIterator, class State, class ForwardOp>
struct iter_fold_next<LastIterator, LastIterator, State, ForwardOp>
    : iter_fold_done<LastIterator,State>
{
};

template <class Iterator, class LastIterator, class State, class BackwardOp>
struct iter_fold_prev : apply2<BackwardOp,State,Iterator>
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

template <class Iterator, class LastIterator, class State, class ForwardOp>
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
                  , apply2<BackwardOp,State,Iterator>
      >::type
{};
#endif

template<
      typename Iterator
    , typename State
    >
struct iter_fold_first
{
    typedef Iterator iterator;
    typedef State type;
};

#define BOOST_MPL_LIMIT_UNROLLING 4

#define BOOST_MPL_AUX_ITER_FOLD_NEXT(i, unused) \
    typedef iter_fold_next< \
          typename BOOST_PP_CAT(next,i)::iterator \
        , LastIterator \
        , typename BOOST_PP_CAT(next,i)::type \
        , ForwardOp \
        > BOOST_PP_CAT(next, BOOST_PP_INC(i)); \
/**/

#define BOOST_MPL_AUX_ITER_FOLD_PREV_FUNC(i) \
    typedef iter_fold_prev< \
          typename BOOST_PP_CAT(next,BOOST_PP_DEC(i))::iterator \
        , LastIterator \
        , typename BOOST_PP_CAT(prev,i)::type \
        , BackwardOp \
        > BOOST_PP_CAT(prev,BOOST_PP_DEC(i)); \
/**/

#define BOOST_MPL_AUX_ITER_FOLD_PREV(i, unused) \
    BOOST_MPL_AUX_ITER_FOLD_PREV_FUNC( \
        BOOST_PP_SUB(BOOST_MPL_LIMIT_UNROLLING,i) \
        ) \
/**/

#define BOOST_MPL_AUX_ITER_FOLD_EXECUTE(i, tf) \
    BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(2, 0, tf), BOOST_PP_INC(i)) \
        ::BOOST_PP_TUPLE_ELEM(2, 1, tf); \
/**/

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
    typedef iter_fold_first<Iterator,State> next0;
    BOOST_PP_REPEAT(BOOST_MPL_LIMIT_UNROLLING, BOOST_MPL_AUX_ITER_FOLD_NEXT, unused)

    #define BOOST_MPL_AUX_ITER_FOLD_LAST BOOST_PP_CAT(next, BOOST_MPL_LIMIT_UNROLLING)
    #define BOOST_MPL_AUX_ITER_FOLD_LAST_PREV BOOST_PP_CAT(prev, BOOST_MPL_LIMIT_UNROLLING)
    
    typedef typename select_type_t<
          is_same<typename BOOST_MPL_AUX_ITER_FOLD_LAST::iterator, LastIterator>
        , iter_fold_done<LastIterator,typename BOOST_MPL_AUX_ITER_FOLD_LAST::type>
        , iter_fold_impl<
              typename BOOST_MPL_AUX_ITER_FOLD_LAST::iterator
            , LastIterator
            , typename BOOST_MPL_AUX_ITER_FOLD_LAST::type
            , ForwardOp
            , BackwardOp
            >
        >::type BOOST_MPL_AUX_ITER_FOLD_LAST_PREV;

    BOOST_PP_REPEAT(BOOST_MPL_LIMIT_UNROLLING, BOOST_MPL_AUX_ITER_FOLD_PREV, unused)

 public:
    typedef typename prev0::type type;

    static void execute()
    {
        BOOST_PP_REPEAT(BOOST_MPL_LIMIT_UNROLLING, BOOST_MPL_AUX_ITER_FOLD_EXECUTE, (next, execute()))
        BOOST_MPL_AUX_ITER_FOLD_LAST_PREV::execute();
        BOOST_PP_REPEAT(BOOST_MPL_LIMIT_UNROLLING, BOOST_MPL_AUX_ITER_FOLD_EXECUTE, (prev, execute()))
    }

    template <class T>
    static void execute(T x)
    {
        BOOST_PP_REPEAT(BOOST_MPL_LIMIT_UNROLLING, BOOST_MPL_AUX_ITER_FOLD_EXECUTE, (next, execute(x)))
        BOOST_MPL_AUX_ITER_FOLD_LAST_PREV::execute(x);
        BOOST_PP_REPEAT(BOOST_MPL_LIMIT_UNROLLING, BOOST_MPL_AUX_ITER_FOLD_EXECUTE, (prev, execute(x)))
    }
};

#undef BOOST_MPL_AUX_ITER_FOLD_EXECUTE
#undef BOOST_MPL_AUX_ITER_FOLD_PREV
#undef BOOST_MPL_AUX_ITER_FOLD_PREV_FUNC
#undef BOOST_MPL_AUX_ITER_FOLD_NEXT

struct select2nd
{
    template <class T, class U>
    struct apply : iter_fold_done<U,T>
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
