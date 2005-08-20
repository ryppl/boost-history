/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_STATEMENT_IMPL_SWITCH_EVAL_HPP
#define PHOENIX_STATEMENT_IMPL_SWITCH_EVAL_HPP

namespace boost { namespace phoenix { namespace impl
{
    template <int N>
    struct switch_eval;

    template <>
    struct switch_eval<0>
    {
        template <
            typename Args, typename Env
          , typename Cond, typename Default
        >
        struct apply
        {
            typedef void type;
        };

        template <
            typename Args, typename Env
          , typename Cond, typename Default
        >
        static void
        eval(
            Args const& args, Env const& env
          , Cond& cond, Default& default_
        )
        {
            default_.eval(args, env);
        }
    };

    template <>
    struct switch_eval<1>
    {
        template <
            typename Args, typename Env
          , typename Cond, typename Default
          , typename Case0
        >
        struct apply
        {
            typedef void type;
        };

        template <
            typename Args, typename Env
          , typename Cond, typename Default
          , typename Case0
        >
        static void
        eval(
            Args const& args, Env const& env
          , Cond& cond, Default& default_
          , Case0& _0
        )
        {
            switch (cond.eval(args, env))
            {
                case Case0::value:
                    _0.eval(args, env);
                    break;
                default:
                    default_.eval(args, env);
            }
        }
    };

    template <>
    struct switch_eval<2>
    {
        template <
            typename Args, typename Env
          , typename Cond, typename Default
          , typename Case0, typename Case1
        >
        struct apply
        {
            typedef void type;
        };

        template <
            typename Args, typename Env
          , typename Cond, typename Default
          , typename Case0, typename Case1
        >
        static void
        eval(
            Args const& args, Env const& env
          , Cond& cond, Default& default_
          , Case0& _0, Case1& _1
        )
        {
            switch (cond.eval(args, env))
            {
                case Case0::value:
                    _0.eval(args, env);
                    break;
                case Case1::value:
                    _1.eval(args, env);
                    break;
                default:
                    default_.eval(args, env);
            }
        }
    };

    //  Bring in the rest of the switch_evals
    #include <boost/spirit/phoenix/statement/impl/switch_eval.ipp>
}}}

#endif
