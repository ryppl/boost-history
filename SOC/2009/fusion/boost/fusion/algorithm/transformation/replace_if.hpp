/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_REPLACE_IF_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_REPLACE_IF_HPP

#include <boost/fusion/view/transform_view/transform_view.hpp>
#include <boost/fusion/support/deduce.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>

#include <boost/fusion/algorithm/transformation/detail/replace_if.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq, typename F, typename NewValue>
        struct replace_if
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));

            typedef
                detail::replace_if_helper<
                    typename traits::deduce<F>::type
                  , typename traits::deduce<NewValue>::type
                >
            replacer;

            typedef transform_view<Seq, replacer> type;
        };
    }

    template <typename Seq, typename F, typename NewValue>
    inline typename result_of::replace_if<
        BOOST_FUSION_R_ELSE_CLREF(Seq)
      , BOOST_FUSION_R_ELSE_CLREF(F)
      , BOOST_FUSION_R_ELSE_CLREF(NewValue)
    >::type
    replace_if(BOOST_FUSION_R_ELSE_CLREF(Seq) seq,
            BOOST_FUSION_R_ELSE_CLREF(F) pred,
            BOOST_FUSION_R_ELSE_CLREF(NewValue) new_value)
    {
        typedef
            result_of::replace_if<
                BOOST_FUSION_R_ELSE_CLREF(Seq)
              , BOOST_FUSION_R_ELSE_CLREF(F)
              , BOOST_FUSION_R_ELSE_CLREF(NewValue)
            >
        result;

        return typename result::type(BOOST_FUSION_FORWARD(Seq,seq),
                typename result::replacer(
                    BOOST_FUSION_FORWARD(F,pred),
                    BOOST_FUSION_FORWARD(NewValue,new_value)));
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename Seq, typename F, typename NewValue>
    inline typename result_of::replace_if<Seq&, F const&, NewValue const&>::type
    replace_if(Seq& seq,F const& pred,NewValue const& new_value)
    {
        typedef result_of::replace_if<Seq&, F const&, NewValue const&> result;

        return typename result::type(seq,
                typename result::replacer(pred,new_value));
    }
#endif
}}

#endif
