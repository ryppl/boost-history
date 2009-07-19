/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_PUSH_FRONT_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_PUSH_FRONT_HPP

#include <boost/fusion/support/detail/as_fusion_element.hpp>
#include <boost/fusion/view/joint_view/joint_view.hpp>
#include <boost/fusion/view/single_view/single_view.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq, typename T>
        struct push_front
        {
            typedef
                joint_view<
                    fusion::single_view<
                        typename detail::as_fusion_element<T>::type
                    >
                  , Seq
                >
            type;
        };
    }

    template <typename Seq, typename T>
    inline typename
        result_of::push_front<
            BOOST_FUSION_R_ELSE_LREF(Seq)
          , BOOST_FUSION_R_ELSE_CLREF(T)
        >::type
    push_front(
            BOOST_FUSION_R_ELSE_LREF(Seq) seq,
            BOOST_FUSION_R_ELSE_CLREF(T) x)
    {
        return typename
            result_of::push_front<
                BOOST_FUSION_R_ELSE_LREF(Seq)
              , BOOST_FUSION_R_ELSE_CLREF(T)
            >::type(BOOST_FUSION_FORWARD(T,x), BOOST_FUSION_FORWARD(Seq,seq));
    }
}}

#endif
