/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2007 Dan Marsden
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_QUERY_DETAIL_FIND_IF_HPP
#define BOOST_FUSION_ALGORITHM_QUERY_DETAIL_FIND_IF_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/advance_c.hpp>
#include <boost/fusion/iterator/distance.hpp>
#include <boost/fusion/support/category_of.hpp>

#include <boost/mpl/identity.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/identity.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        template <typename Begin, typename End, typename Pred>
        struct main_find_if;

        template <typename Begin, typename End, typename Pred>
        struct next_find_if
          : main_find_if<
                typename result_of::next<Begin>::type
              , End
              , Pred
            >
        {};

        template <typename Begin, typename End, typename Pred>
        struct main_find_if
          : mpl::eval_if<
                mpl::or_<
                    result_of::equal_to<Begin, End>
                  , mpl::apply1<Pred,Begin>
                >
              , mpl::identity<Begin>
              , next_find_if<Begin, End, Pred>
            >
        {};

        template<
            typename Begin
          , typename End
          , typename Pred
          , bool /*IsRandomAccess=false*/
        >
        struct choose_find_if
          : main_find_if<Begin, End, Pred>
        {};

        template<typename It, typename Pred, int N>
        struct unroll_again;

        template <typename It, typename Pred, int Offset>
        struct apply_offset_filter
          : mpl::apply1<
                Pred
              , typename result_of::advance_c<It, Offset>::type
            >::type
        {};

        template<typename It, typename Pred, int N>
        struct unrolled_find_if
          : mpl::eval_if<
                typename mpl::apply1<Pred,It>::type
              , mpl::identity<It>
              , mpl::eval_if<
                    apply_offset_filter<It, Pred, 1>
                  , result_of::next<It>
                  , mpl::eval_if<
                        apply_offset_filter<It, Pred, 2>
                      , result_of::advance_c<It, 2>
                      , mpl::eval_if<
                            apply_offset_filter<It, Pred, 3>
                          , result_of::advance_c<It, 3>
                          , unroll_again<
                                It
                              , Pred
                              , N
                            >
                        >
                    >
                >
            >
        {};

        template<typename It, typename Pred, int n>
        struct unroll_again
          : unrolled_find_if<
                typename result_of::advance_c<It, 4>::type
              , Pred
              , n-4
            >
        {};

        template<typename It, typename Pred>
        struct unrolled_find_if<It, Pred, 3>
          : mpl::eval_if<
                typename mpl::apply1<Pred,It>::type
              , mpl::identity<It>
              , mpl::eval_if<
                    apply_offset_filter<It, Pred, 1>
                  , result_of::next<It>
                  , mpl::eval_if<
                        apply_offset_filter<It, Pred, 2>
                      , result_of::advance_c<It, 2>
                      , result_of::advance_c<It, 3>
                    >
                >
            >
        {};

        template<typename It, typename Pred>
        struct unrolled_find_if<It, Pred, 2>
          : mpl::eval_if<
                typename mpl::apply1<Pred,It>::type
              , mpl::identity<It>
              , mpl::eval_if<
                    apply_offset_filter<It, Pred, 1>
                  , result_of::next<It>
                  , result_of::advance_c<It, 2>
                >
            >
        {};

        template<typename It, typename Pred>
        struct unrolled_find_if<It, Pred, 1>
          : mpl::eval_if<
                typename mpl::apply1<Pred,It>::type
              , mpl::identity<It>
              , result_of::next<It>
            >
        {};

        template<typename It, typename Pred>
        struct unrolled_find_if<It, Pred, 0>
        {
            typedef It type;
        };

        template<typename Begin, typename End, typename Pred>
        struct choose_find_if<Begin, End, Pred, /*IsRandomAccess*/true>
          : unrolled_find_if<
                Begin
              , Pred
              , result_of::distance<Begin, End>::value
            >
        {};

        template <typename Begin, typename End, typename Pred>
        struct static_find_if
        {
            typedef typename
                choose_find_if<
                    Begin
                  , End
                  , Pred
                  , traits::is_random_access<Begin>::type::value
                >::type
            type;

            template <typename It>
            static type
            call(It const& it)
            {
                return
                    fusion::advance<
                        typename result_of::distance<It, type>::type
                    >(it);
            }
        };

        template <typename Begin, typename End, typename Pred>
        struct static_seq_find_if
          : static_find_if<Begin, End, Pred>
        {
            template <typename Seq>
            static typename static_find_if<Begin, End, Pred>::type
            call(BOOST_FUSION_R_ELSE_LREF(Seq) seq)
            {
                return static_find_if<Begin, End, Pred>::call(
                        fusion::begin(BOOST_FUSION_FORWARD(Seq,seq)));
            }
        };
    }
}}

#endif
