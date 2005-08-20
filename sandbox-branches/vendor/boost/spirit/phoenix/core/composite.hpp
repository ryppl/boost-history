/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_CORE_COMPOSITE_HPP
#define PHOENIX_CORE_COMPOSITE_HPP

#include <boost/spirit/fusion/sequence/tuple.hpp>
#include <boost/spirit/fusion/sequence/tuple_element.hpp>
#include <boost/spirit/fusion/sequence/get.hpp>
#include <boost/spirit/phoenix/core/actor.hpp>

#if !defined(PHOENIX_COMPOSITE_LIMIT)
# define PHOENIX_COMPOSITE_LIMIT PHOENIX_LIMIT
#endif

namespace boost { namespace phoenix
{
///////////////////////////////////////////////////////////////////////////////
//
//  composite class
//
//      A composite is an actor base class composed of zero or more actors
//      (see actor.hpp) packed in a tuple (ActorTuple) and an evaluator
//      (Eval). A composite is itself an actor superclass and conforms to its
//      conceptual interface. Its eval member function un-funnels the tupled
//      actual arguments from the arg_list (see arg_list.hpp) by invoking
//      each of the actors' eval member function. The results of each are
//      then passed on as arguments to the evaluator (Eval). Composites can
//      have 0..PHOENIX_COMPOSITE_LIMIT actors.
//
//      Schematically:
//
//          actor0.eval(arg_list, env) --> _0 --> |
//          actor1.eval(arg_list, env) --> _1 --> |
//          actor2.eval(arg_list, env) --> _2 --> | --> Eval::eval(_0..._N)
//            ...                                 |
//          actorN.eval(arg_list, env) --> _N --> |
//
//      The evaluator is expected to have a static "eval" function with the
//      interface:
//
//          template <typename Args, typename Env, typename A0..., typename AN>
//          static apply<Args, Env, A0,... AN>::type
//          eval(Args const& args, Env const& env, A0& _0..., AN& _N)
//
//      where apply<Args, Env, A0,... AN> is a nested metafunction that
//      computes the return type of the eval function. Notice that A0..AN are
//      the un-funneled arguments taken from the argument list Args. This is
//      essentially a type computer that answers the question "Given
//      arguments of type A0...AN, what will be its eval's return type?".
//
//          template <typename Args, typename Env, typename A0..., typename AN>
//          struct apply;
//
//      The evaluator accepts the arguments passed in by the composite. There
//      should be a one to one correspondence between actors of the composite
//      and the arguments of the operation's member operator().
//
//      Composites are, almost always, not created directly. Instead, there
//      are meta-programs provided that indirectly create composites. See
//      compose.hpp.
//
///////////////////////////////////////////////////////////////////////////////
    namespace impl
    {
        template <int N>
        struct composite_eval;
    }

    template <typename Eval, typename ActorTuple>
    struct composite : ActorTuple
    {
        typedef ActorTuple base_t;
        typedef composite<Eval, ActorTuple> self_t;
        typedef Eval eval_t;

        template <typename Args, typename Env>
        struct apply
        {
            typedef
                typename impl::composite_eval<
                    fusion::tuple_size<base_t>::value>::
                template apply<self_t, Args, Env>::type
            type;
        };

        composite()
            : base_t() {}

        composite(base_t const& actors)
            : base_t(actors) {}

        template <typename TB0>
        composite(TB0& _0)
            : base_t(_0) {}

        template <typename TB0, typename TB1>
        composite(TB0& _0, TB1& _1)
            : base_t(_0, _1) {}

        template <typename Args, typename Env>
        typename apply<Args, Env>::type
        eval(Args const& args, Env const& env) const
        {
            return impl::composite_eval<fusion::tuple_size<base_t>::value>::
                call(*this, args, env);
        }

        //  Bring in the rest of the constructors
        #include <boost/spirit/phoenix/core/impl/composite_constructor.ipp>
    };

    //  Bring in the impl::composite_eval<0..N> definitions
    #include <boost/spirit/phoenix/core/impl/composite_eval.ipp>
}}

#undef PHOENIX_COMPOSITE_TYPE_LIST
#endif
