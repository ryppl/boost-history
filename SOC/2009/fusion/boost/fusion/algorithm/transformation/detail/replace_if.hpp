/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_DETAIL_REPLACE_IF_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_DETAIL_REPLACE_IF_HPP

#include <boost/fusion/support/result_of.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_convertible.hpp>

namespace boost { namespace fusion { namespace detail
{
    //TODO cschmidt: update doc. according to real behavior!
    //TODO cschmidt: conv based on ptrs?!
    //TODO const?!
    template <typename F, typename NewValue>
    struct replace_if_helper
    {
        template<typename Params>
        struct result;

        template<typename Self, typename U>
        struct result<Self(U)>
        {
            typedef typename detail::identity<U>::type u;

            typedef typename
                mpl::if_<is_convertible<NewValue, u>, u, U>::type
            type;
        };

        template<typename OtherF, typename OtherNewValue>
        replace_if_helper(
            BOOST_FUSION_R_ELSE_CLREF(OtherF) f,
            BOOST_FUSION_R_ELSE_CLREF(OtherNewValue) new_value)
          : f(BOOST_FUSION_FORWARD(OtherF,f))
          , new_value(BOOST_FUSION_FORWARD(OtherNewValue,new_value))
        {}

        template<typename Replacer>
        replace_if_helper(BOOST_FUSION_R_ELSE_CLREF(Replacer) replacer)
          : f(BOOST_FUSION_FORWARD(Replacer,replacer).f)
          , new_value(BOOST_FUSION_FORWARD(Replacer,replacer).new_value)
        {}

        template<typename Replacer>
        replace_if_helper&
        operator=(BOOST_FUSION_R_ELSE_CLREF(Replacer) replacer)
        {
            f=BOOST_FUSION_FORWARD(Replacer,replacer).f;
            new_value=BOOST_FUSION_FORWARD(Replacer,replacer).new_value;
            return *this;
        }

        template <typename U>
        BOOST_FUSION_R_ELSE_LREF(U)
        call_impl(BOOST_FUSION_R_ELSE_LREF(U) x, mpl::false_) const
        {
            return x;
        }

        template <typename U>
        typename result<replace_if_helper(BOOST_FUSION_R_ELSE_LREF(U))>::type
        call_impl(BOOST_FUSION_R_ELSE_LREF(U) x, mpl::true_) const
        {
            return f(BOOST_FUSION_FORWARD(U,x)) ? new_value : x;
        }

        template <typename U>
        typename result<replace_if_helper(BOOST_FUSION_R_ELSE_LREF(U))>::type
        operator()(BOOST_FUSION_R_ELSE_LREF(U) x) const
        {
            return call_impl(
                BOOST_FUSION_FORWARD(U, x),
                typename is_convertible<
                    NewValue
#ifdef BOOST_NO_RVALUE_REFERENCES
                  , U
#else
                  , typename detail::remove_reference<U>::type
#endif
                >::type());
        }

        F f;
        NewValue new_value;
    };
}}}

#endif
