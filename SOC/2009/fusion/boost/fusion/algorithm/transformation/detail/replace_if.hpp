/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_DETAIL_REPLACE_IF_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_DETAIL_REPLACE_IF_HPP

#include <boost/fusion/support/result_of.hpp>

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_convertible.hpp>

namespace boost { namespace fusion { namespace detail
{
    //TODO cschmidt: update doc. according to real behavior!
    template <bool is_convertible>
    struct replacer_if_helper;

    template <>
    struct replacer_if_helper<false>
    {
        template <typename U, typename F, typename NewValue>
        static BOOST_FUSION_R_ELSE_LREF(U)
        call(BOOST_FUSION_R_ELSE_LREF(U) x,
             BOOST_FUSION_R_ELSE_LREF(F),
             BOOST_FUSION_R_ELSE_LREF(NewValue))
        {
            return x;
        }
    };

    template <>
    struct replacer_if_helper<true>
    {
        template <typename U, typename F, typename NewValue>
        static typename remove_reference<U>::type
        call(BOOST_FUSION_R_ELSE_LREF(U) x,
             BOOST_FUSION_R_ELSE_LREF(F) f,
             BOOST_FUSION_R_ELSE_LREF(NewValue) new_value)
        {
            return f(BOOST_FUSION_FORWARD(U,x)) ? new_value : x;
        }
    };

    //TODO !!!
    template <typename F, typename NewValue>
    struct replacer_if
    {
        replacer_if(BOOST_FUSION_R_ELSE_CLREF(replacer_if) replacer)
          : f(BOOST_FUSION_FORWARD(
                  BOOST_FUSION_R_ELSE_CLREF(replacer_if),replacer).f)
          , new_value(BOOST_FUSION_FORWARD(
                  BOOST_FUSION_R_ELSE_CLREF(replacer_if),replacer).new_value)
        {}

        template<typename OtherF, typename OtherNewValue>
        replacer_if(
                BOOST_FUSION_R_ELSE_LREF(OtherF) other_f,
                BOOST_FUSION_R_ELSE_LREF(OtherNewValue) other_new_value)
          : f(BOOST_FUSION_FORWARD(OtherF,other_f))
          , new_value(BOOST_FUSION_FORWARD(OtherNewValue,other_new_value))
        {}

        template<typename Params>
        struct result;

        template <typename U>
        struct result<replacer_if<F, NewValue>(U)>
        {
            typedef typename
                mpl::if_<
                    is_convertible<
                        NewValue
                      , typename remove_reference<U>::type
                    >
                  , NewValue
                  , U
                >::type
            type;
        };

        template <typename U>
        typename result<replacer_if(U)>::type
        operator()(BOOST_FUSION_R_ELSE_LREF(U) x) const
        {
            typedef
                replacer_if_helper<
                    is_convertible<
                        NewValue
                      , typename remove_reference<U>::type
                    >::value
                >
            gen;

            return gen::call(
                    BOOST_FUSION_FORWARD(U, x),
                    f,
                    new_value);
        }

        F f;
        NewValue new_value;
    };
}}}

#endif
