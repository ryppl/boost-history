/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_CORE_ARGUMENT_HPP
#define PHOENIX_CORE_ARGUMENT_HPP

#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/spirit/phoenix/core/actor.hpp>
#include <boost/spirit/fusion/sequence/get.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/apply_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/less.hpp>

#if !defined(PHOENIX_ARG_LIMIT)
# define PHOENIX_ARG_LIMIT PHOENIX_LIMIT
#endif

#define PHOENIX_DECLARE_ARG(z, n, data)                                         \
    actor<argument<n> > const BOOST_PP_CAT(arg, n) = argument<n>();             \
    actor<argument<n> > const BOOST_PP_CAT(_, n) = argument<n>();

namespace boost { namespace phoenix
{
    namespace impl
    {
        template <typename Arg>
        struct error_argument_not_found {};
        inline void test_invalid_argument(int) {}
    }

///////////////////////////////////////////////////////////////////////////////
//
//  argument class
//
//      argument place-holders
//
//      An actor base class that extracts and returns the Nth argument from
//      the argument list passed in the arg_list in the eval member function
//      (see actor.hpp). There are some predefined argument constants that
//      can be used as actors (arg1..argN) and for compatibility with
//      boost::lambda, (_1.._N). Where N is a predefined limit
//      (PHOENIX_ARG_LIMIT)
//
//      The argument actor is a place-holder for the actual arguments passed
//      by the client. For example, wherever arg1 is seen placed in a lazy
//      function (see functions module) or lazy operator (see operators
//      module), this will be replaced by the actual first argument in the
//      actual function, or operator, evaluation. Argument actors are
//      essentially lazy arguments. A lazy argument is a full actor in its
//      own right and can be evaluated through the actor's operator().
//
//      Example:
//
//          char        c = 'A';
//          int         i = 123;
//          const char* s = "Hello World";
//
//          cout << arg1(c) << ' ';
//          cout << arg1(i, s) << ' ';
//          cout << arg2(i, s) << ' ';
//
//       will print out "A 123 Hello World"
//
///////////////////////////////////////////////////////////////////////////////
    template <int N>
    struct argument
    {
        template <typename Args, typename Env>
        struct apply
        {
            typedef typename
                mpl::apply_if<
                    mpl::less<mpl::int_<N>, arity<Args> >
                  , arg<N, Args>
                  , mpl::identity<void_t>
                >::type
            type;
        };

        template <typename Args, typename Env>
        typename arg<N, Args>::type&
        eval(Args const& args, Env const&) const
        {
            typedef typename
                mpl::if_<
                    mpl::less<mpl::int_<N>, arity<Args> >
                  , int
                  , impl::error_argument_not_found<argument<N> >
                >::type
            check_out_of_bounds;

            impl::test_invalid_argument(check_out_of_bounds());
            return fusion::get<N>(args);
        }
    };

//  Phoenix style names
    actor<argument<0> > const arg1 = argument<0>();
    actor<argument<1> > const arg2 = argument<1>();
    actor<argument<2> > const arg3 = argument<2>();

//  BLL style names
    actor<argument<0> > const _1 = argument<0>();
    actor<argument<1> > const _2 = argument<1>();
    actor<argument<2> > const _3 = argument<2>();

//  Bring in the rest or the Phoenix style arguments (arg4 .. argN+1)
//  and BLL style arguments (_4 .. _N+1), using PP
    BOOST_PP_REPEAT_FROM_TO(
        4, BOOST_PP_INC(PHOENIX_ARG_LIMIT), PHOENIX_DECLARE_ARG, _)
}}

#undef PHOENIX_DECLARE_ARG
#endif
