/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_JOIN_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_JOIN_HPP

#include <boost/fusion/view/joint_view.hpp>

#include <boost/preprocessor/empty.hpp>

namespace boost { namespace fusion {

    namespace result_of
    {
        template<typename Lhs, typename Rhs>
        struct join
        {
            typedef joint_view<Lhs,Rhs> type;
        };
    }

#define BOOST_FUSION_JOIN(LHS_CV_REF_MODIFIER,RHS_CV_REF_MODIFIER)\
    template<typename Lhs, typename Rhs>\
    inline typename\
        result_of::join<\
            Lhs LHS_CV_REF_MODIFIER\
          , Rhs RHS_CV_REF_MODIFIER\
        >::type\
    join(Lhs LHS_CV_REF_MODIFIER lhs,\
            Rhs RHS_CV_REF_MODIFIER rhs)\
    {\
        return typename\
            result_of::join<\
                Lhs LHS_CV_REF_MODIFIER\
              , Rhs RHS_CV_REF_MODIFIER\
            >::type(BOOST_FUSION_FORWARD(Lhs LHS_CV_REF_MODIFIER,lhs)\
                  , BOOST_FUSION_FORWARD(Rhs RHS_CV_REF_MODIFIER,rhs));\
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
