/*==============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_PUSH_BACK_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_PUSH_BACK_HPP

#include <boost/fusion/support/internal/base.hpp>
#include <boost/fusion/view/joint_view/joint_view.hpp>
#include <boost/fusion/view/single_view/single_view.hpp>
#include <boost/fusion/support/deduce.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template<typename Seq, typename T>
        struct push_back
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>))
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>))

            typedef
                joint_view<
                    Seq
                  , single_view<typename traits::deduce<T>::type>
                >
            type;
        };
    }

    template<typename Seq, typename T>
    inline typename
        result_of::push_back<
            BOOST_FUSION_R_ELSE_CLREF(Seq)
          , BOOST_FUSION_R_ELSE_CLREF(T)
        >::type
    push_back(BOOST_FUSION_R_ELSE_CLREF(Seq) seq, BOOST_FUSION_R_ELSE_CLREF(T) x)
    {
        return typename
            result_of::push_back<
                BOOST_FUSION_R_ELSE_CLREF(Seq)
              , BOOST_FUSION_R_ELSE_CLREF(T)
            >::type(BOOST_FUSION_FORWARD(Seq,seq),
                  fusion::make_single_view(BOOST_FUSION_FORWARD(T,x)));
    }

#ifdef BOOST_FUSION_NO_RVALUE_REFERENCES
    template<typename Seq, typename T>
    inline typename result_of::push_back<Seq&, T const&>::type
    push_back(Seq& seq, T const& x)
    {
        return typename result_of::push_back<Seq&, T const&>::type(
                seq,fusion::make_single_view(x));
    }
#endif
}}

#endif
