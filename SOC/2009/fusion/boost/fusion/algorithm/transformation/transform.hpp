/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_TRANSFORM_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_TRANSFORM_HPP

#include <boost/fusion/view/transform_view/transform_view.hpp>

#include <boost/preprocessor/empty.hpp>

namespace boost { namespace fusion
{
    struct void_;

    namespace result_of
    {
        template <typename Seq1, typename Seq2, typename F = void_>
        struct transform
        {
            typedef transform_view<Seq1,Seq2,F> type;
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
            BOOST_FUSION_R_ELSE_CLREF(Seq)
          , BOOST_FUSION_R_ELSE_CLREF(F)
        >::type
    transform(BOOST_FUSION_R_ELSE_CLREF(Seq) seq, BOOST_FUSION_R_ELSE_CLREF(F) f)
    {
        return typename
            result_of::transform<
                BOOST_FUSION_R_ELSE_CLREF(Seq)
              , BOOST_FUSION_R_ELSE_CLREF(F)
            >::type(BOOST_FUSION_FORWARD(Seq,seq), BOOST_FUSION_FORWARD(F,f));
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename Seq, typename F>
    inline typename result_of::transform<Seq&, F const&>::type
    transform(Seq& seq, F const& f)
    {
        return typename result_of::transform<Seq&, F const&>::type(seq, f);
    }
#endif

#define BOOST_FUSION_TRANSFORM_BINARY(SEQ1_CV_REF_MODIFIER,SEQ2_CV_REF_MODIFIER)\
    template <typename Seq1, typename Seq2, typename F>\
    inline typename\
        result_of::transform<\
            Seq1 SEQ1_CV_REF_MODIFIER\
          , Seq2 SEQ2_CV_REF_MODIFIER\
          , BOOST_FUSION_R_ELSE_CLREF(F)\
        >::type\
    transform(\
            Seq1 SEQ1_CV_REF_MODIFIER seq1\
          , Seq2 SEQ2_CV_REF_MODIFIER seq2\
          , BOOST_FUSION_R_ELSE_CLREF(F) f)\
    {\
        return typename\
            result_of::transform<\
                Seq1 SEQ1_CV_REF_MODIFIER\
              , Seq2 SEQ2_CV_REF_MODIFIER\
              , BOOST_FUSION_R_ELSE_CLREF(F)\
            >::type(\
                    BOOST_FUSION_FORWARD(Seq1 SEQ1_CV_REF_MODIFIER,seq1)\
                  , BOOST_FUSION_FORWARD(Seq2 SEQ2_CV_REF_MODIFIER,seq2)\
                  , BOOST_FUSION_FORWARD(F,f));\
    }

    BOOST_FUSION_TRANSFORM_BINARY(
            BOOST_FUSION_R_ELSE_CLREF(BOOST_PP_EMPTY()),
            BOOST_FUSION_R_ELSE_CLREF(BOOST_PP_EMPTY()));
#ifdef BOOST_NO_RVALUE_REFERENCES
    BOOST_FUSION_TRANSFORM_BINARY(&,const&);
    BOOST_FUSION_TRANSFORM_BINARY(const&,&);
    BOOST_FUSION_TRANSFORM_BINARY(&,&);
#endif

#undef BOOST_FUSION_TRANSFORM_BINARY
}}

#endif
