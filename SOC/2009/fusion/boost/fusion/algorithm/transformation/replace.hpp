/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_REPLACE_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_REPLACE_HPP

#include <boost/fusion/view/transform_view/transform_view.hpp>
#include <boost/fusion/algorithm/transformation/replace_if.hpp>
#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/support/detail/as_fusion_element.hpp>

#include <boost/fusion/algorithm/transformation/detail/replace.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
    //TODO New arg?!
        template <typename Seq, typename OldValue, typename NewValue>
        struct replace
          : replace_if<
                Seq
              , BOOST_FUSION_R_ELSE_CLREF(
                    detail::replacer<
                        typename detail::as_fusion_element<OldValue>::type
                    >)
              , NewValue
            >
        {
        };
    }

    //TODO cschmidt: Assert compatibility Old-/NewValue!
    template <typename Seq, typename OldValue, typename NewValue>
    inline typename
        result_of::replace<
            BOOST_FUSION_R_ELSE_LREF(Seq)
          , BOOST_FUSION_R_ELSE_CLREF(OldValue)
          , BOOST_FUSION_R_ELSE_CLREF(NewValue)
        >::type
    replace(BOOST_FUSION_R_ELSE_LREF(Seq) seq,
            BOOST_FUSION_R_ELSE_CLREF(OldValue) old_value,
            BOOST_FUSION_R_ELSE_CLREF(NewValue) new_value)
    {
        typedef typename
            result_of::replace<
                BOOST_FUSION_R_ELSE_LREF(Seq)
              , BOOST_FUSION_R_ELSE_CLREF(OldValue)
              , BOOST_FUSION_R_ELSE_CLREF(NewValue)
            >::type
        type;
        typedef
            detail::replacer<
                typename detail::as_fusion_element<
                    BOOST_FUSION_R_ELSE_CLREF(OldValue)
                >::type
            >
        replacer;

        return replace_if(
                    BOOST_FUSION_FORWARD(Seq,seq)
                  , replacer(BOOST_FUSION_FORWARD(OldValue,old_value),0)
                  , BOOST_FUSION_FORWARD(NewValue,new_value));
    }
}}

#endif
