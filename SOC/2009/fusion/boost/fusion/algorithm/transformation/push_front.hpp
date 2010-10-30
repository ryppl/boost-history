/*==============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_PUSH_FRONT_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_PUSH_FRONT_HPP

#include <boost/fusion/support/internal/base.hpp>
#include <boost/fusion/view/joint_view/joint_view.hpp>
#include <boost/fusion/view/single_view/single_view.hpp>
#include <boost/fusion/support/deduce.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template<typename Seq, typename T>
        struct push_front
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>))
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>))

            typedef
                joint_view<
                    single_view<typename traits::deduce<T>::type>
                  , Seq
                >
            type;
        };
    }

    template<typename Seq, typename T>
    inline typename result_of::push_front<
        BOOST_FUSION_R_ELSE_CLREF(Seq)
      , BOOST_FUSION_R_ELSE_CLREF(T)
    >::type
    push_front(
        BOOST_FUSION_R_ELSE_CLREF(Seq) seq,
        BOOST_FUSION_R_ELSE_CLREF(T) x)
    {
        return typename result_of::push_front<
            BOOST_FUSION_R_ELSE_CLREF(Seq)
          , BOOST_FUSION_R_ELSE_CLREF(T)
        >::type(fusion::make_single_view(BOOST_FUSION_FORWARD(T,x)),
            BOOST_FUSION_FORWARD(Seq,seq));
    }

#ifdef BOOST_FUSION_NO_RVALUE_REFERENCES
    template<typename Seq, typename T>
    inline typename result_of::push_front<Seq&, T const&>::type
    push_front(Seq& seq, T const& x)
    {
        return typename result_of::push_front<Seq&, T const&>::type(
                fusion::make_single_view(BOOST_FUSION_FORWARD(T,x)),
                BOOST_FUSION_FORWARD(Seq,seq));
    }
#endif
}}

#endif
