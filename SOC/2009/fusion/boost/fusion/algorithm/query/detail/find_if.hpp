/*==============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2007 Dan Marsden
    Copyright (c) 2009-2010 Christopher Schmidt

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
#include <boost/fusion/support/config.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/identity.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        template<typename Begin, typename End, typename Pred>
        struct main_find_if;

        template<typename Begin, typename End, typename Pred>
        struct next_find_if
          : main_find_if<
                typename result_of::next<Begin>::type
              , End
              , Pred
            >
        {};

        template<typename Begin, typename End, typename Pred>
        struct main_find_if
          : mpl::eval_if_c<
                mpl::or_<
                    result_of::equal_to<Begin, End>
                  , mpl::apply1<Pred,Begin>
                >::value
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

        template<typename It, typename Pred, int Offset>
        struct apply_offset_filter
          : mpl::apply1<
                Pred
              , typename result_of::advance_c<It, Offset>::type
            >::type
        {};

        template<typename It0, typename Pred, int N>
        struct unrolled_find_if;

        template<typename It, typename Pred, int N>
        struct unroll_again
          : unrolled_find_if<
                typename result_of::advance_c<
                    It
                  , BOOST_FUSION_UNROLLED_DEPTH
                >::type
              , Pred
              , N-BOOST_FUSION_UNROLLED_DEPTH
            >
        {};

#define BOOST_FUSION_UNROLLED_FIND_IF_IMPL_DECLERATION(_)                       \
        template<typename It0, typename Pred, int N>                            \
        struct unrolled_find_if

#define BOOST_FUSION_UNROLLED_FIND_IF_IMPL_SPECIALIZATION(N)                    \
        template<typename It0, typename Pred>                                   \
        struct unrolled_find_if<It0, Pred, N>

#define BOOST_FUSION_UNROLLED_FIND_IF_HIERARCHY(Z,N,_)                          \
      , mpl::eval_if_c<                                                         \
            apply_offset_filter<It0, Pred, N>::value                            \
          , result_of::advance_c<It0,N>

#define BOOST_FUSION_UNROLLED_FIND_IF_HIERARCHY_CLOSE(Z,N,_) >

#define BOOST_FUSION_UNROLLED_FIND_IF_IMPL(Z,N_,_)                              \
        BOOST_PP_IIF(                                                           \
            BOOST_PP_EQUAL(BOOST_PP_INC(N_),BOOST_FUSION_UNROLLED_DEPTH),       \
            BOOST_FUSION_UNROLLED_FIND_IF_IMPL_DECLERATION,                     \
            BOOST_FUSION_UNROLLED_FIND_IF_IMPL_SPECIALIZATION)(N_)              \
          : mpl::eval_if_c<                                                     \
                mpl::apply1<Pred,It0>::type::value                              \
              , mpl::identity<It0>                                              \
              , mpl::eval_if<                                                   \
                    apply_offset_filter<It0, Pred, 1>                           \
                  , result_of::next<It0>                                        \
                    BOOST_PP_REPEAT_FROM_TO(                                    \
                        2,                                                      \
                        N_,                                                     \
                        BOOST_FUSION_UNROLLED_FIND_IF_HIERARCHY,                \
                        _)                                                      \
                      , result_of::advance_c<It0, N_>                           \
            BOOST_PP_REPEAT(                                                    \
                N_,BOOST_FUSION_UNROLLED_FIND_IF_HIERARCHY_CLOSE,_)             \
        {};

        BOOST_PP_REPEAT_FROM_TO(
            2,
            BOOST_FUSION_UNROLLED_DEPTH,
            BOOST_FUSION_UNROLLED_FIND_IF_IMPL,
            _)

#undef BOOST_FUSION_UNROLLED_FIND_IF_IMPL
#undef BOOST_FUSION_UNROLLED_FIND_IF_HIERARCHY_CLOSE
#undef BOOST_FUSION_UNROLLED_FIND_IF_HIERARCHY
#undef BOOST_FUSION_UNROLLED_FIND_IF_IMPL_DECLERATION
#undef BOOST_FUSION_UNROLLED_FIND_IF_IMPL_SPECIALIZATION

        template<typename It, typename Pred>
        struct unrolled_find_if<It, Pred, 1>
          : mpl::eval_if_c<
                mpl::apply1<Pred,It>::type::value
              , mpl::identity<It>
              , result_of::next<It>
            >
        {};

        template<typename It0, typename Pred>
        struct unrolled_find_if<It0, Pred, 0>
        {
            typedef It0 type;
        };

        template<typename Begin, typename End, typename Pred>
        struct choose_find_if<Begin, End, Pred, /*IsRandomAccess*/true>
          : unrolled_find_if<
                Begin
              , Pred
              , result_of::distance<Begin, End>::value
            >
        {};

        template<typename Begin, typename End, typename Pred>
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

            template<typename It>
            static type
            call(It const& it)
            {
                return fusion::advance<
                    typename result_of::distance<It, type>::type
                >(it);
            }
        };

        template<typename Begin, typename End, typename Pred>
        struct static_seq_find_if
          : static_find_if<Begin, End, Pred>
        {
            template<typename Seq>
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
