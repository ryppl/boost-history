//-----------------------------------------------------------------------------
// boost mpl/sort.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002
// Eric Friedman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_SORT_HPP_INCLUDED
#define BOOST_MPL_SORT_HPP_INCLUDED

#include "boost/mpl/apply_if.hpp"
#include "boost/mpl/bind.hpp"
#include "boost/mpl/copy_backward.hpp"
#include "boost/mpl/empty.hpp"
#include "boost/mpl/front.hpp"
#include "boost/mpl/identity.hpp"
#include "boost/mpl/if.hpp"
#include "boost/mpl/lambda.hpp"
#include "boost/mpl/comparison/less.hpp"
#include "boost/mpl/partition.hpp"
#include "boost/mpl/pop_front.hpp"
#include "boost/mpl/push_front.hpp"
#include "boost/mpl/sequence_tag.hpp"
#include "boost/mpl/void.hpp"
#include "boost/type_traits/is_same.hpp"
#include "boost/mpl/aux_/void_spec.hpp"
#include "boost/mpl/aux_/lambda_support.hpp"

namespace boost {
namespace mpl {

namespace aux {

template <
      typename Sequence
    , typename Predicate
    >
struct sort_impl; // forward declared

template <
      typename Sequence
    , typename Predicate
    >
struct quick_sort_impl
{
private:
    typedef typename front<Sequence>::type
        pivot_;
    typedef typename pop_front<Sequence>::type
        seq_;

//    typedef typename partition<
//          seq_
//        , bind2nd<
//              typename lambda<Predicate>::type
//            , pivot_
//            >
//        >::type partitioned;

    typedef pair< // temporary workaround for broken mpl::bind
          typename remove_if<
              seq_
            , bind2nd<typename lambda< logical_not<Predicate> >::type, pivot_>
            >::type
        , typename remove_if<
              seq_
            , bind2nd<typename lambda<Predicate>::type, pivot_>
            >::type
        > partitioned;

    typedef typename sort_impl<
          typename partitioned::first
        , Predicate
        >::type first_part;

    typedef typename sort_impl<
          typename partitioned::second
        , Predicate
        >::type second_part;

public:
    typedef typename copy_backward<
          first_part
        , typename push_front<second_part, pivot_>::type
        , push_front<_,_>
        >::type type;
};

template <
      typename Sequence
    , typename Predicate
    >
struct sort_impl
{
    typedef typename apply_if<
          empty<Sequence>
        , identity<Sequence>
        , quick_sort_impl<Sequence, Predicate>
        >::type type;
};

} // namespace aux

template < typename Tag >
struct sort_algorithm_traits
{
    template < typename Sequence, typename Predicate > struct algorithm
    {
        typedef typename aux::sort_impl<Sequence, Predicate>::type
            type;
    };
};

template <
      typename BOOST_MPL_AUX_VOID_SPEC_PARAM(Sequence)
    , typename BOOST_MPL_AUX_VOID_SPEC_PARAM(Predicate)
    >
struct sort
{
private:
    typedef typename if_<
          is_same<Predicate, void_>
        , less<_,_>
        , Predicate
        >::type pred_;
    typedef typename sequence_tag<Sequence>::type
        tag_;

public:
    typedef typename sort_algorithm_traits<tag_>
        ::template algorithm< Sequence,pred_ >::type type;

    BOOST_MPL_AUX_LAMBDA_SUPPORT(2,sort,(Sequence,Predicate))
};

BOOST_MPL_AUX_VOID_SPEC(2, sort)

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_SORT_HPP_INCLUDED
