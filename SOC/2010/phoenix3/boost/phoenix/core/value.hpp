/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_CORE_VALUE_HPP
#define PHOENIX_CORE_VALUE_HPP

#include <boost/phoenix/core/compose.hpp>
#include <boost/phoenix/core/meta_grammar.hpp>
#include <boost/utility/result_of.hpp>

namespace boost { namespace phoenix
{
    ////////////////////////////////////////////////////////////////////////////
    //
    // values
    //
    //      function for evaluating values, e.g. val(123)
    //
    ////////////////////////////////////////////////////////////////////////////
    
    namespace result_of
    {
        template <typename Env, typename T>
        struct value
            : boost::result_of<eval_grammar(T const&)>
        {};
        
        template <typename Env, typename Expr>
        struct actor_value
            : remove_const<
                typename remove_reference<
                    typename boost::result_of<eval_grammar(Expr const&, Env&)>::type
                >::type
              >
        {};
    }

    struct value
    {
        template<typename Sig>
        struct result;

        template<typename This, typename Env, typename T>
        struct result<This(Env&,T const&)>
            : result_of::value<Env, T>
        {};

        template<typename Env, typename T>
        typename result_of::value<Env, T>::type
        operator()(Env& env, T const& t) const
        {
            return eval(t);
        }
    };

    struct actor_value
    {
        template<typename Sig>
        struct result;

        template<typename This, typename Env, typename Actor>
        struct result<This(Env&, Actor const&)>
            : result_of::actor_value<Env, Actor>
        {};

        template<typename Env, typename Actor>
        typename result_of::actor_value<Env, Actor>::type
        operator()(Env& env, Actor const& actor) const
        {
            return eval(actor, env);
        }
    };

    namespace detail
    {
        template <typename T>
        struct convert_value
        {
            typedef T type;
        };

        template <typename T, int N>
        struct convert_value<T[N]>
        {
            typedef T const* type;
        };
    }

    template <typename T>
    struct make_value : compose<value, typename detail::convert_value<T>::type> {};

    template <typename Actor>
    struct make_actor_value : compose<actor_value, Actor> {};

    template <typename Expr>
    typename make_actor_value<actor<Expr> >::type const
    val(actor<Expr> const& a)
    {
        return make_actor_value<actor<Expr> >()(a);
    }

    template <typename T>
    typename make_value<T>::type const
    val(T const & t )
    {
        return make_value<T>()(t);
    }

    template <typename T>
    typename make_value<T>::type const
    val(T & t )
    {
        return make_value<T>()(t);
    }

}}

#endif
