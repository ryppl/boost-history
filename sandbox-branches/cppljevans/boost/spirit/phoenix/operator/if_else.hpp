/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_OPERATOR_IF_ELSE_HPP
#define PHOENIX_OPERATOR_IF_ELSE_HPP

#include <boost/mpl/and.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/spirit/phoenix/core/composite.hpp>
#include <boost/spirit/phoenix/core/compose.hpp>
#include <boost/spirit/phoenix/utility/type_deduction.hpp>

namespace boost { namespace phoenix
{
    BOOST_BINARY_RESULT_OF(true ? x : y, result_of_if_else);

    struct if_else_op_eval
    {
        template <
            typename Args
          , typename Env
          , typename Cond
          , typename Then
          , typename Else
        >
        struct apply
        {
            typedef typename Then::template apply<Args, Env>::type then_t;
            typedef typename Else::template apply<Args, Env>::type else_t;

            typedef typename
                result_of_if_else<then_t, else_t>::type
            ite_result;

            // Note: c ? x : y can return an lvalue! Allow if_else_op_eval
            // to return an lvalue IFF then_t and else_t are both lvalues
            // with the same type.

            typedef typename
                mpl::if_<
                    mpl::and_<
                        is_same<then_t, else_t>
                      , is_reference<then_t>
                    >
                  , ite_result
                  , typename remove_reference<ite_result>::type
                >::type
            type;
        };

        template <
            typename Args
          , typename Env
          , typename Cond
          , typename Then
          , typename Else
        >
        static typename apply<Args, Env, Cond, Then, Else>::type
        eval(
            Args const& args, Env const& env
          , Cond& cond, Then& then, Else& else_)
        {
            return cond.eval(args, env)
                ? then.eval(args, env) : else_.eval(args, env);
        }
    };

    template <typename Cond, typename Then, typename Else>
    inline actor<typename as_composite<if_else_op_eval, Cond, Then, Else>::type>
    if_else(Cond const& cond, Then const& then, Else const& else_)
    {
        return compose<if_else_op_eval>(cond, then, else_);
    }
}}

#endif
