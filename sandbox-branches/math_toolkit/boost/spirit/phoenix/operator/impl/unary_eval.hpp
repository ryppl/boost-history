/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_OPERATOR_IMPL_UNARY_EVAL_HPP
#define PHOENIX_OPERATOR_IMPL_UNARY_EVAL_HPP

#include <boost/mpl/apply_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>

#define PHOENIX_UNARY_EVAL(eval_name, op_result, expr)                          \
    struct eval_name                                                            \
    {                                                                           \
        template <typename Args, typename Env, typename A0>                     \
        struct apply                                                            \
        {                                                                       \
            typedef typename A0::template apply<Args, Env>::type x_type;        \
                                                                                \
            typedef typename                                                    \
                mpl::apply_if<                                                  \
                    is_same<x_type, void_t>                                     \
                  , mpl::identity<void_t>                                       \
                  , op_result<x_type>                                           \
                >::type                                                         \
            type;                                                               \
        };                                                                      \
                                                                                \
        template <typename Args, typename Env, typename A0>                     \
        static typename apply<Args, Env, A0>::type                              \
        eval(Args const& args, Env const& env, A0& a0)                          \
        {                                                                       \
            return expr;                                                        \
        }                                                                       \
    };

#endif


