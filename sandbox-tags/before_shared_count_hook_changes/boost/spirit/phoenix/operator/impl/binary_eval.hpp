/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_OPERATOR_IMPL_BINARY_EVAL_HPP
#define PHOENIX_OPERATOR_IMPL_BINARY_EVAL_HPP

#include <boost/mpl/or.hpp>
#include <boost/mpl/apply_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>

#define PHOENIX_BINARY_EVAL(eval_name, op_result, expr)                         \
    struct eval_name                                                            \
    {                                                                           \
        template <typename Args, typename Env, typename A0, typename A1>        \
        struct apply                                                            \
        {                                                                       \
            typedef typename A0::template apply<Args, Env>::type x_type;        \
            typedef typename A1::template apply<Args, Env>::type y_type;        \
                                                                                \
            typedef typename                                                    \
                mpl::apply_if<                                                  \
                    mpl::or_<                                                   \
                        is_same<x_type, void_t>                                 \
                      , is_same<y_type, void_t>                                 \
                    >                                                           \
                  , mpl::identity<void_t>                                       \
                  , op_result<x_type, y_type>                                   \
                >::type                                                         \
            type;                                                               \
        };                                                                      \
                                                                                \
        template <typename Args, typename Env, typename A0, typename A1>        \
        static typename apply<Args, Env, A0, A1>::type                          \
        eval(Args const& args, Env const& env, A0& a0, A1& a1)                  \
        {                                                                       \
            return expr;                                                        \
        }                                                                       \
    };

#undef x
#undef y
#endif
