/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_CORE_COMPOSE_HPP
#define PHOENIX_CORE_COMPOSE_HPP

#include <boost/mpl/apply_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/spirit/phoenix/core/composite.hpp>
#include <boost/spirit/phoenix/core/value.hpp>
#include <boost/spirit/phoenix/core/as_actor.hpp>

#define PHOENIX_AS_ACTOR(z, n, data)                                            \
    typename mpl::apply_if<                                                     \
        is_same<BOOST_PP_CAT(T, n), void_t>                                     \
      , mpl::identity<void_t>                                                   \
      , as_actor<BOOST_PP_CAT(T, n)> >::type

namespace boost { namespace phoenix
{
///////////////////////////////////////////////////////////////////////////////
//
//  as_composite<Eval, T0,... TN> metafunction
//
//      Create a composite given an evaluator Eval and types T0..TN.
//      The types are converted to an actor through the as_actor
//      metafunction (see as_actor.hpp).
//
///////////////////////////////////////////////////////////////////////////////
    template <
        typename Eval
      , PHOENIX_TEMPLATE_WITH_DEFAULT(PHOENIX_COMPOSITE_LIMIT, T)>
    struct as_composite
    {
        typedef composite<
            Eval
          , fusion::tuple<
                BOOST_PP_ENUM(PHOENIX_COMPOSITE_LIMIT, PHOENIX_AS_ACTOR, _)>
        >
        type;
    };

///////////////////////////////////////////////////////////////////////////////
//
//  compose functions
//
//      Usage:
//
//          compose<Eval>(_0, _1,... _N)
//
//      Returns a composite given an evaluator Eval and arguments _0.._N. The
//      arguments are converted to an actor through the as_actor
//      metafunction (see as_actor.hpp).
//
///////////////////////////////////////////////////////////////////////////////
    template <typename Eval>
    inline actor<typename as_composite<Eval>::type>
    compose()
    {
        return actor<typename as_composite<Eval>::type>();
    }

    template <typename Eval, typename T0>
    inline actor<typename as_composite<Eval, T0>::type>
    compose(T0 const& _0)
    {
        return actor<typename as_composite<Eval, T0>::type>(
            as_actor<T0>::convert(_0)
        );
    }

    template <typename Eval, typename T0, typename T1>
    inline actor<typename as_composite<Eval, T0, T1>::type>
    compose(T0 const& _0, T1 const& _1)
    {
        return actor<typename as_composite<Eval, T0, T1>::type>(
            as_actor<T0>::convert(_0)
          , as_actor<T1>::convert(_1)
        );
    }

    //  Bring in the the rest of the compose overloads
    #include <boost/spirit/phoenix/core/impl/compose.ipp>
}}

#undef PHOENIX_AS_ACTOR
#endif
