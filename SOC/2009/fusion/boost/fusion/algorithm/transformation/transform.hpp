/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_TRANSFORM_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_TRANSFORM_HPP

#include <boost/fusion/view/transform_view/transform_view.hpp>
#include <boost/fusion/support/deduce.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>

#include <boost/preprocessor/empty.hpp>

namespace boost { namespace fusion
{
    struct void_;

    namespace result_of
    {
        template<
            typename Seq1
          , typename Seq2
          , typename F=mpl::false_
          , typename IsAssociative=mpl::false_
        >
        struct transform
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq1>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq1>));
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq2>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq2>));

            typedef
                transform_view<
                    Seq1
                  , Seq2
                  , typename traits::deduce<F>::type
                  , IsAssociative
                >
            type;
        };

        template <typename Seq, typename F>
        struct transform<Seq, F, mpl::false_, mpl::false_>
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));

            typedef
                transform_view<
                    Seq
                  , typename traits::deduce<F>::type
                  , mpl::false_
                >
            type;
        };

        template <typename Seq, typename F>
        struct transform<Seq, F, mpl::true_, mpl::false_>
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));

            typedef
                transform_view<
                    Seq
                  , typename traits::deduce<F>::type
                  , mpl::true_
                >
            type;
        };
    }

    //TODO boost config macro for default arguments for function templates

    template <typename IsAssociative,typename Seq, typename F>
    inline typename
        result_of::transform<
            BOOST_FUSION_R_ELSE_CLREF(Seq)
          , BOOST_FUSION_RREF_ELSE_OBJ(F)
          , IsAssociative
        >::type
    transform(BOOST_FUSION_R_ELSE_CLREF(Seq) seq,
            BOOST_FUSION_RREF_ELSE_OBJ(F) f)
    {
        return typename
            result_of::transform<
                BOOST_FUSION_R_ELSE_CLREF(Seq)
              , BOOST_FUSION_RREF_ELSE_OBJ(F)
              , IsAssociative
            >::type(BOOST_FUSION_FORWARD(Seq,seq), BOOST_FUSION_FORWARD(F,f));
    }

    template <typename Seq, typename F>
    inline typename
        result_of::transform<
            BOOST_FUSION_R_ELSE_CLREF(Seq)
          , BOOST_FUSION_RREF_ELSE_OBJ(F)
        >::type
    transform(BOOST_FUSION_R_ELSE_CLREF(Seq) seq,
            BOOST_FUSION_RREF_ELSE_OBJ(F) f)
    {
        return typename
            result_of::transform<
                BOOST_FUSION_R_ELSE_CLREF(Seq)
              , BOOST_FUSION_RREF_ELSE_OBJ(F)
            >::type(BOOST_FUSION_FORWARD(Seq,seq), BOOST_FUSION_FORWARD(F,f));
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename IsAssociative,typename Seq, typename F>
    inline typename result_of::transform<Seq&, F>::type
    transform(Seq& seq, F f)
    {
#ifdef BOOST_MSVC
#   pragma warning(push)
#   pragma warning(disable: 4180)
#endif
        return typename
            result_of::transform<Seq&, F,IsAssociative>::type(seq, f);
#ifdef BOOST_MSVC
#   pragma warning(pop)
#endif
    }

    template <typename Seq, typename F>
    inline typename result_of::transform<Seq&, F>::type
    transform(Seq& seq, F f)
    {
        return typename result_of::transform<Seq&, F>::type(seq, f);
    }
#endif

#define BOOST_FUSION_TRANSFORM_BINARY(SEQ1_CV_REF_MODIFIER,SEQ2_CV_REF_MODIFIER)\
    template <typename IsAssociative,typename Seq1, typename Seq2, typename F>\
    inline typename\
        result_of::transform<\
            Seq1 SEQ1_CV_REF_MODIFIER\
          , Seq2 SEQ2_CV_REF_MODIFIER\
          , BOOST_FUSION_RREF_ELSE_OBJ(F)\
          , IsAssociative\
        >::type\
    transform(Seq1 SEQ1_CV_REF_MODIFIER seq1\
          , Seq2 SEQ2_CV_REF_MODIFIER seq2\
          , BOOST_FUSION_RREF_ELSE_OBJ(F) f)\
    {\
        return typename\
            result_of::transform<\
                Seq1 SEQ1_CV_REF_MODIFIER\
              , Seq2 SEQ2_CV_REF_MODIFIER\
              , BOOST_FUSION_RREF_ELSE_OBJ(F)\
              , IsAssociative\
            >::type(static_cast<Seq1 SEQ1_CV_REF_MODIFIER>(seq1)\
                  , static_cast<Seq2 SEQ2_CV_REF_MODIFIER>(seq2)\
                  , BOOST_FUSION_FORWARD(F,f));\
    }\
    \
    template <typename Seq1, typename Seq2, typename F>\
    inline typename\
        result_of::transform<\
            Seq1 SEQ1_CV_REF_MODIFIER\
          , Seq2 SEQ2_CV_REF_MODIFIER\
          , BOOST_FUSION_RREF_ELSE_OBJ(F)\
        >::type\
    transform(Seq1 SEQ1_CV_REF_MODIFIER seq1\
          , Seq2 SEQ2_CV_REF_MODIFIER seq2\
          , BOOST_FUSION_RREF_ELSE_OBJ(F) f)\
    {\
        return typename\
            result_of::transform<\
                Seq1 SEQ1_CV_REF_MODIFIER\
              , Seq2 SEQ2_CV_REF_MODIFIER\
              , BOOST_FUSION_RREF_ELSE_OBJ(F)\
            >::type(BOOST_FUSION_FORWARD(Seq1 SEQ1_CV_REF_MODIFIER,seq1)\
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
