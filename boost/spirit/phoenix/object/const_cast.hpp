/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_OBJECT_CONST_CAST_HPP
#define PHOENIX_OBJECT_CONST_CAST_HPP

#include <boost/spirit/phoenix/core/compose.hpp>

namespace boost { namespace phoenix
{
    namespace impl
    {
        template <typename T>
        struct const_cast_eval
        {
            template <typename Args, typename Env, typename U>
            struct apply
            {
                typedef T type;
            };

            template <typename Args, typename Env, typename U>
            static T
            eval(Args const& args, Env const& env, U& obj)
            {
                return const_cast<T>(obj.eval(args, env));
            }
        };
    }

    template <typename T, typename U>
    inline actor<typename as_composite<impl::const_cast_eval<T>, U>::type>
    const_cast_(U const& obj)
    {
        return compose<impl::const_cast_eval<T> >(obj);
    }
}}

#endif
