/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_REPLACE_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_REPLACE_HPP

#include <boost/fusion/algorithm/transformation/replace_if.hpp>
#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/support/detail/as_fusion_element.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        template <typename OldValue>
        struct replace_helper
        {
            template <typename OtherOldValue>
            replace_helper(
                    BOOST_FUSION_R_ELSE_LREF(OtherOldValue) old_value
                  , int)
              : old_value(BOOST_FUSION_FORWARD(OtherOldValue,old_value))
            {}

            template<typename Replacer>
            replace_helper(BOOST_FUSION_R_ELSE_LREF(Replacer) replacer)
              : old_value(BOOST_FUSION_FORWARD(Replacer,replacer).old_value)
            {}

            template<typename Replacer>
            replace_helper&
            operator=(BOOST_FUSION_R_ELSE_LREF(Replacer) replacer)
            {
                old_value=BOOST_FUSION_FORWARD(Replacer,replacer).old_value;
                return *this;
            }

            template <typename U>
            bool
            operator()(BOOST_FUSION_R_ELSE_LREF(U) x) const
            {
                return x==old_value;
            }

            OldValue old_value;
        };
    }

    namespace result_of
    {
        //TODO New arg?!
        template <typename Seq, typename OldValue, typename NewValue>
        struct replace
        {
            typedef
                detail::replace_helper<
                    typename detail::as_fusion_element<OldValue>::type
                >
            replacer;

            typedef typename replace_if<Seq, replacer, NewValue>::type type;
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
            >::replacer
        replacer;

        return replace_if(
                    BOOST_FUSION_FORWARD(Seq,seq)
                  , replacer(BOOST_FUSION_FORWARD(OldValue,old_value),0)
                  , BOOST_FUSION_FORWARD(NewValue,new_value));
    }
}}

#endif
