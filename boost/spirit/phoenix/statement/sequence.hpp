/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_STATEMENT_SEQUENCE_HPP
#define PHOENIX_STATEMENT_SEQUENCE_HPP

#include <boost/spirit/phoenix/core/composite.hpp>
#include <boost/spirit/phoenix/core/compose.hpp>

namespace boost { namespace phoenix
{
    struct sequence_eval
    {
        template <typename Args, typename Env, typename A0, typename A1>
        struct apply
        {
            typedef void type;
        };

        template <typename Args, typename Env, typename A0, typename A1>
        static void
        eval(Args const& args, Env const& env, A0& a0, A1& a1)
        {
            a0.eval(args, env);
            a1.eval(args, env);
        }
    };

    template <typename BaseT0, typename BaseT1>
    inline actor<
        composite<
            sequence_eval
          , fusion::tuple<actor<BaseT0>, actor<BaseT1> >
        >
    >
    operator,(actor<BaseT0> const& a0, actor<BaseT1> const& a1)
    {
        return compose<sequence_eval>(a0, a1);
    }
}}

#endif
