/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_DETAIL_REPLACE_IF_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_DETAIL_REPLACE_IF_HPP

#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/remove_const.hpp>

namespace boost { namespace fusion { namespace detail
{
    template <typename F, typename NewValue>
    struct replace_if_helper
    {
        template<typename Sig>
        struct result;

        template<typename Self, typename U>
        struct result<Self(U)>
          : mpl::eval_if<
                is_convertible<
                    typename remove_reference<NewValue>::type*
                  , typename remove_reference<U>::type*
                >
              , mpl::if_<
                    mpl::and_<
                        is_rref<NewValue>
                      , is_lrref<U>
                    >
                  , U
                  , typename remove_reference<U>::type&
                >
              , mpl::identity<U>
            >
        {};

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
        U&
        call_impl(U& x, mpl::false_ /*is_convertible*/) const
        {
            return x;
        }

        template <typename U>
        U&
        call_impl(U& x, mpl::true_ /*is_convertible*/) const
        {
            if(f(x))
            {
                return new_value;
            }

            return x;
        }

        template <typename U>
        typename result<replace_if_helper(U&)>::type
        operator()(U& x) const
        {
            //TODO cschmidt: update doc. according to real behavior!
            return call_impl(
                x,
                typename is_convertible<
                    typename detail::remove_reference<NewValue>::type*
                  , U*
                >::type());
        }

        F f;
        mutable typename remove_const<NewValue>::type new_value;
    };
}}}

#endif
