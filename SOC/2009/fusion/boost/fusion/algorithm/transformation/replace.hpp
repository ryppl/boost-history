/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_REPLACE_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_REPLACE_HPP

#include <boost/fusion/algorithm/transformation/replace_if.hpp>
#include <boost/fusion/support/deduce.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>

#ifdef BOOST_FUSION_ENABLE_STATIC_ASSERTS
#   include <boost/type_traits/is_convertible.hpp>
#endif

namespace boost { namespace fusion
{
    namespace detail
    {
        template <typename OldValue>
        struct replace_helper
        {
            template <typename OtherOldValue>
            replace_helper(
                    BOOST_FUSION_R_ELSE_CLREF(OtherOldValue) old_value
                  , int)
              : old_value(BOOST_FUSION_FORWARD(OtherOldValue,old_value))
            {}

            template<typename Replacer>
            replace_helper(BOOST_FUSION_R_ELSE_CLREF(Replacer) replacer)
              : old_value(BOOST_FUSION_FORWARD(Replacer,replacer).old_value)
            {}

            template<typename Replacer>
            replace_helper&
            operator=(BOOST_FUSION_R_ELSE_CLREF(Replacer) replacer)
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
        template <typename Seq, typename T>
        struct replace
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));

            typedef
                detail::replace_helper<typename traits::deduce<T>::type>
            replacer;

            typedef typename
                replace_if<Seq, BOOST_FUSION_R_ELSE_CLREF(replacer), T>::type
            type;
        };
    }

    template <typename Seq, typename OldValue, typename NewValue>
    inline typename
        result_of::replace<
            BOOST_FUSION_R_ELSE_CLREF(Seq)
          , BOOST_FUSION_R_ELSE_CLREF(OldValue)
        >::type
    replace(BOOST_FUSION_R_ELSE_CLREF(Seq) seq,
            BOOST_FUSION_R_ELSE_CLREF(OldValue) old_value,
            BOOST_FUSION_R_ELSE_CLREF(NewValue) new_value)
    {
        BOOST_FUSION_MPL_ASSERT((
            is_convertible<
                typename traits::deduce<
                    BOOST_FUSION_R_ELSE_CLREF(NewValue)
                >::type*
              , typename traits::deduce<
                    BOOST_FUSION_R_ELSE_CLREF(OldValue)
                >::type*
            >));

        typedef typename
            result_of::replace<
                BOOST_FUSION_R_ELSE_CLREF(Seq)
              , BOOST_FUSION_R_ELSE_CLREF(OldValue)
            >::replacer
        replacer;

        return replace_if(
                    BOOST_FUSION_FORWARD(Seq,seq)
                  , replacer(BOOST_FUSION_FORWARD(OldValue,old_value),0)
                  , BOOST_FUSION_FORWARD(NewValue,new_value));
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename Seq, typename OldValue, typename NewValue>
    inline typename result_of::replace<Seq&, OldValue const&>::type
    replace(Seq& seq, OldValue const& old_value, NewValue const& new_value)
    {
        BOOST_FUSION_MPL_ASSERT((
            is_convertible<
                typename traits::deduce<NewValue const&>::type*
              , typename traits::deduce<OldValue const&>::type*
            >));

        typedef typename
            result_of::replace<Seq&, OldValue const&>::replacer
        replacer;

        return replace_if(seq,replacer(old_value,0), new_value);
    }
#endif
}}

#endif
