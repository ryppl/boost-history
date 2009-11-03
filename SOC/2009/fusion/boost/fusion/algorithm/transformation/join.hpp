/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_JOIN_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_JOIN_HPP

#include <boost/fusion/view/joint_view.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>

#include <boost/preprocessor/empty.hpp>

namespace boost { namespace fusion {

    namespace result_of
    {
        template<typename Seq1, typename Seq2>
        struct join
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq1>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq1>));
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq2>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq2>));

            typedef joint_view<Seq1,Seq2> type;
        };
    }

#define BOOST_FUSION_JOIN(SEQ1_CV_REF_MODIFIER,SEQ2_CV_REF_MODIFIER)\
    template<typename Seq1, typename Seq2>\
    inline typename\
        result_of::join<\
            Seq1 SEQ1_CV_REF_MODIFIER\
          , Seq2 SEQ2_CV_REF_MODIFIER\
        >::type\
    join(Seq1 SEQ1_CV_REF_MODIFIER seq1,\
         Seq2 SEQ2_CV_REF_MODIFIER seq2)\
    {\
        return typename\
            result_of::join<\
                Seq1 SEQ1_CV_REF_MODIFIER\
              , Seq2 SEQ2_CV_REF_MODIFIER\
            >::type(static_cast<Seq1 SEQ1_CV_REF_MODIFIER>(seq1)\
                  , static_cast<Seq2 SEQ2_CV_REF_MODIFIER>(seq2));\
    }

    BOOST_FUSION_JOIN(
            BOOST_FUSION_R_ELSE_CLREF(BOOST_PP_EMPTY()),
            BOOST_FUSION_R_ELSE_CLREF(BOOST_PP_EMPTY()));
#ifdef BOOST_NO_RVALUE_REFERENCES
    BOOST_FUSION_JOIN(&,const&);
    BOOST_FUSION_JOIN(const&,&);
    BOOST_FUSION_JOIN(&,&);
#endif

#undef BOOST_FUSION_JOIN
}}

#endif
