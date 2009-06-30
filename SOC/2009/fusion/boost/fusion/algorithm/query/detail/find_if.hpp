/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2007 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_QUERY_DETAIL_FIND_IF_HPP
#define BOOST_FUSION_ALGORITHM_QUERY_DETAIL_FIND_IF_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/iterator/value_of.hpp>
#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/advance.hpp>
#include <boost/fusion/iterator/distance.hpp>
#include <boost/fusion/support/category_of.hpp>

#include <boost/mpl/identity.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/identity.hpp>

namespace boost { namespace fusion {
    struct random_access_traversal_tag;
namespace detail
{
    template <typename It, typename Pred>
    struct apply_filter
      : mpl::apply1<Pred, typename result_of::value_of<It>::type>::type
    {
    };

    template <typename First, typename Last, typename Pred>
    struct main_find_if;

    template <typename First, typename Last, typename Pred>
    struct recursive_find_if
    {
        typedef typename
            main_find_if<
                typename result_of::next<First>::type
              , Last
              , Pred
            >::type
        type;
    };

    template <typename First, typename Last, typename Pred>
    struct main_find_if
    {
        typedef typename
            mpl::eval_if<
                mpl::or_<
                    result_of::equal_to<First, Last>
                  , apply_filter<First, Pred>
                >
              , mpl::identity<First>
              , recursive_find_if<First, Last, Pred>
            >::type
        type;
    };

    template<typename First, typename Last, typename Pred, bool>
    struct choose_find_if;

    template<typename First, typename Last, typename Pred>
    struct choose_find_if<First, Last, Pred, false>
      : main_find_if<First, Last, Pred>
    {};

    template<typename It, typename Pred, int n, int unrolling>
    struct unroll_again;

    template <typename It, typename Pred, int offset>
    struct apply_offset_filter
      : mpl::apply1<
            Pred
          , typename result_of::value_of<
                typename result_of::advance_c<It, offset>::type
            >::type
        >::type
    {
    };

    template<typename It, typename Pred, int n>
    struct unrolled_find_if
    {
        typedef typename
            mpl::eval_if<
                apply_filter<It, Pred>
              , mpl::identity<It>
              , mpl::eval_if<
                    apply_offset_filter<It, Pred, 1>
                  , result_of::advance_c<It, 1>
                  , mpl::eval_if<
                        apply_offset_filter<It, Pred, 2>
                      , result_of::advance_c<It, 2>
                      , mpl::eval_if<
                            apply_offset_filter<It, Pred, 3>
                          , result_of::advance_c<It, 3>
                          , unroll_again<
                                It
                              , Pred
                              , n
                              , 4
                            >
                        >
                    >
                >
            >::type
        type;
    };

    template<typename It, typename Pred>
    struct unrolled_find_if<It, Pred, 3>
    {
        typedef typename
            mpl::eval_if<
                apply_filter<It, Pred>
              , mpl::identity<It>
              , mpl::eval_if<
                    apply_offset_filter<It, Pred, 1>
                  , result_of::advance_c<It, 1>
                  , mpl::eval_if<
                        apply_offset_filter<It, Pred, 2>
                      , result_of::advance_c<It, 2>
                      , result_of::advance_c<It, 3>
                    >
                >
            >::type
        type;
    };

    template<typename It, typename Pred>
    struct unrolled_find_if<It, Pred, 2>
    {
        typedef typename
            mpl::eval_if<
                apply_filter<It, Pred>
              , mpl::identity<It>
              , mpl::eval_if<
                    apply_offset_filter<It, Pred, 1>
                  , result_of::advance_c<It, 1>
                  , result_of::advance_c<It, 2>
                >
            >::type
        type;
    };

    template<typename It, typename Pred>
    struct unrolled_find_if<It, Pred, 1>
    {
        typedef typename
            mpl::eval_if<
                apply_filter<It, Pred>
              , mpl::identity<It>
              , result_of::advance_c<It, 1>
            >::type
        type;
    };

    template<typename It, typename Pred, int n, int unrolling>
    struct unroll_again
    {
        typedef typename
            unrolled_find_if<
                typename result_of::advance_c<It, unrolling>::type
              , Pred
              , n-unrolling
            >::type
        type;
    };

    template<typename It, typename Pred>
    struct unrolled_find_if<It, Pred, 0>
    {
        typedef It type;
    };

    template<typename First, typename Last, typename Pred>
    struct choose_find_if<First, Last, Pred, true>
    {
        typedef typename
            unrolled_find_if<
                First
              , Pred
              , result_of::distance<First, Last>::value
            >::type
        type;
    };

    template <typename First, typename Last, typename Pred>
    struct static_find_if
    {
        typedef typename
            choose_find_if<
                First
              , Last
              , typename mpl::lambda<Pred>::type
              , is_base_of<
                    random_access_traversal_tag
                  , typename traits::category_of<First>::type
                >::value
            >::type
        type;

        template <typename It>
        static type
        call(It const& it)
        {
            typedef typename result_of::distance<It, type>::type N;
            return fusion::advance<N>(it);
        }
    };

    template <typename First, typename Last, typename Pred>
    struct static_seq_find_if
      : static_find_if<First, Last, Pred>
    {
        template <typename Seq>
        static typename static_find_if<First, Last, Pred>::type
        call(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
        {
            typedef static_find_if<First, Last, Pred> gen;

            return gen::call(fusion::begin(BOOST_FUSION_FORWARD(Seq,seq)));
        }
    };
}}}

#endif
