/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_OBJECT_NEW_HPP
#define PHOENIX_OBJECT_NEW_HPP

#include <boost/spirit/phoenix/core/compose.hpp>
#include <boost/spirit/phoenix/core/impl/macro_utils.hpp>

namespace boost { namespace phoenix
{
    namespace impl
    {
        template <typename T>
        struct new_eval
        {
            template <typename Args, typename Env,
                PHOENIX_TEMPLATE_WITH_DEFAULT(PHOENIX_COMPOSITE_LIMIT, A)>
            struct apply
            {
                typedef T* type;
            };

            template <typename Args, typename Env>
            static T*
            eval(Args const& args, Env const& env)
            {
                return new T;
            }

            template <typename Args, typename Env, typename A0>
            static T*
            eval(Args const& args, Env const& env, A0& _0)
            {
                return new T(_0.eval(args, env));
            }

            template <typename Args, typename Env, typename A0, typename A1>
            static T*
            eval(Args const& args, Env const& env, A0& _0, A1& _1)
            {
                return new T(_0.eval(args, env), _1.eval(args, env));
            }

            //  Bring in the rest of the evals
            #include <boost/spirit/phoenix/object/impl/new_eval.ipp>
        };
    }

    template <typename T>
    inline actor<typename as_composite<impl::new_eval<T> >::type>
    new_()
    {
        return compose<impl::new_eval<T> >();
    }

    template <typename T, typename A0>
    inline actor<typename as_composite<impl::new_eval<T>, A0>::type>
    new_(A0 const& _0)
    {
        return compose<impl::new_eval<T> >(_0);
    }

    template <typename T, typename A0, typename A1>
    inline actor<typename as_composite<impl::new_eval<T>, A0, A1>::type>
    new_(A0 const& _0, A1 const& _1)
    {
        return compose<impl::new_eval<T> >(_0, _1);
    }

    //  Bring in the rest of the new_ functions
    #include <boost/spirit/phoenix/object/impl/new.ipp>
}}

#endif
