/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_OBJECT_DELETE_HPP
#define PHOENIX_OBJECT_DELETE_HPP

#include <boost/spirit/phoenix/core/compose.hpp>

namespace boost { namespace phoenix
{
    namespace impl
    {
        struct delete_eval
        {
            template <typename Args, typename Env, typename P>
            struct apply
            {
                typedef void type;
            };

            template <typename Args, typename Env, typename P>
            static void
            eval(Args const& args, Env const& env, P& p)
            {
                delete p.eval(args, env);
            }
        };
    }

    template <typename P>
    inline actor<typename as_composite<impl::delete_eval, P>::type>
    delete_(P const& p)
    {
        return compose<impl::delete_eval>(p);
    }
}}

#endif
