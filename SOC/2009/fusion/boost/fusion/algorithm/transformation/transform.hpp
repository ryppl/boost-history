/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_TRANSFORM_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_TRANSFORM_HPP

#include <boost/fusion/view/transform_view/transform_view.hpp>

namespace boost { namespace fusion
{
    struct void_;

    namespace result_of
    {
        template <typename Seq1, typename Seq2, typename F = void_>
        struct transform
        {
            typedef
                transform_view<
                    typename detail::add_lref<Seq1>::type
                  , typename detail::add_lref<Seq2>::type
                  , typename detail::add_lref<F>::type
                >
            type;
        };

        template <typename Seq, typename F>
#if defined(BOOST_NO_PARTIAL_SPECIALIZATION_IMPLICIT_DEFAULT_ARGS)
        struct transform<Seq, F, void_>
#else
        struct transform<Seq, F>
#endif
        {
            typedef transform_view<Seq, F> type;
        };
    }

    template <typename Seq, typename F>
    inline typename
        result_of::transform<
            BOOST_FUSION_R_ELSE_LREF(Seq)
          , BOOST_FUSION_R_ELSE_LREF(F)
        >::type
    transform(BOOST_FUSION_R_ELSE_LREF(Seq) seq, BOOST_FUSION_R_ELSE_LREF(F) f)
    {
        return typename
            result_of::transform<
                BOOST_FUSION_R_ELSE_LREF(Seq)
              , BOOST_FUSION_R_ELSE_LREF(F)
            >::type(BOOST_FUSION_FORWARD(Seq,seq), BOOST_FUSION_FORWARD(F,f));
    }

    template <typename Seq1, typename Seq2, typename F>
    inline typename
        result_of::transform<
            BOOST_FUSION_R_ELSE_LREF(Seq1)
          , BOOST_FUSION_R_ELSE_LREF(Seq2)
          , BOOST_FUSION_R_ELSE_LREF(F)
        >::type
    transform(
            BOOST_FUSION_R_ELSE_LREF(Seq1) seq1
          , BOOST_FUSION_R_ELSE_LREF(Seq2) seq2
          , BOOST_FUSION_R_ELSE_LREF(F) f)
    {
        return typename
            result_of::transform<
                BOOST_FUSION_R_ELSE_LREF(Seq1)
              , BOOST_FUSION_R_ELSE_LREF(Seq2)
              , BOOST_FUSION_R_ELSE_LREF(F)
            >::type(
                    BOOST_FUSION_FORWARD(Seq1,seq1)
                  , BOOST_FUSION_FORWARD(Seq2,seq2)
                  , BOOST_FUSION_FORWARD(F,f));
    }
}}

#endif
