/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_CORE_ACTOR_HPP
#define PHOENIX_CORE_ACTOR_HPP

#if !defined(PHOENIX_LIMIT)
# define PHOENIX_LIMIT 10
#endif

#if !defined(PHOENIX_ACTOR_LIMIT)
# define PHOENIX_ACTOR_LIMIT PHOENIX_LIMIT
#endif

#if !defined(FUSION_MAX_TUPLE_SIZE)
# define FUSION_MAX_TUPLE_SIZE PHOENIX_LIMIT
#elif (FUSION_MAX_TUPLE_SIZE < PHOENIX_LIMIT)
# error "FUSION_MAX_TUPLE_SIZE is set too low"
#endif

#include <boost/spirit/phoenix/core/arg_list.hpp>

namespace boost { namespace phoenix
{
    namespace impl
    {
        //  Forward declarations. These will come in when we get to the
        //  operator module, yet, the actor's assignment operator and index
        //  operator are required to be members.

        template <typename T0, typename T1>
        struct make_assign_composite;

        template <typename T0, typename T1>
        struct make_index_composite;
    }

    struct empty {}; // An empty environment

///////////////////////////////////////////////////////////////////////////////
//
//  actor class
//
//      This class is a protocol class for all actors. This class is
//      essentially an interface contract. The actor class does not really
//      know how how to "act" on anything but instead relies on the template
//      parameter Base (from which the actor will derive from) to do the
//      actual action.
//
//      An actor is a functor that is capable of accepting arguments up to
//      a predefined maximum (PHOENIX_LIMIT). It is the responsibility of
//      the base class to do the actual processing. Upon invocation of the
//      functor through a supplied operator(),
//
//          1) the actor funnels the arguments passed in by the client into
//             an arg_list (Basically a tuple, see arg_list.hpp),
//
//          2) creates an initially empty environment (see empty above), and
//
//          3) calls the base eval member function passing in the tupled
//             arg_list and the initially empty environment.
//
//      Schematically:
//
//          arg0 ---------|
//          arg1 ---------|
//          arg3 ---------|---> (arg_list, empty) ---> base.eval
//          ...           |
//          argN ---------|
//
//          actor::operator()(arg0, arg1... argN)
//              ---> BaseT::eval(arg_list, empty());
//
//      Actor base classes, from which this class inherits from, are expected
//      to have a corresponding member function eval compatible with the
//      conceptual Interface:
//
//          template <typename Args, typename Env>
//          actor_return_type
//          eval(Args const& args, Env const&) const
//
//      where args are the actual arguments passed in by the client
//      funneled into an arg_list (see arg_list.hpp for details).
//
//      The actor_return_type can be anything. Base classes are free to
//      return any type (typically argument dependent types; i.e. types that
//      are deduced from the types of the arguments). After evaluating the
//      parameters and doing some computation or "action"s, the eval member
//      function concludes by returning something back to the client. To do
//      this, the forwarding function (the actor's operator()) needs to know
//      the return type of the eval member function that it is calling. For
//      this purpose, actor base classes are required to provide a nested
//      template class:
//
//          template <typename Args, typename Env>
//          struct apply;
//
//      This auxiliary class provides the result type information returned by
//      the eval member function of a base actor class. The nested template
//      class "apply" should have a typedef 'type' that reflects the return
//      type of its member function eval. It is basically a metafunction
//      (type computer) that answers the question "given arguments packed
//      into an arglist of type Args, and an environment of type Env, what
//      will be the result type of the eval member function of the actor?".
//
//      To recap, here's a skeleton class that exemplifies an actor base
//      class:
//
//          struct an_actor_base
//          {
//              template <typename Args, typename Env>
//              struct apply
//              {
//                  typedef /*eval's return type*/ type;
//              };
//
//              template <typename Args, typename Env>
//              typename apply<Args, Env>::type
//              eval(Args const& args, Env const&) const;
//          };
//
///////////////////////////////////////////////////////////////////////////////
    template <typename Base>
    struct actor : Base
    {
        typedef actor<Base> self_t;

        actor()
            : Base() {}

        actor(Base const& base)
            : Base(base) {}

        template <typename T0>
        explicit actor(T0 const& _0)
            : Base(_0) {}

        template <typename T0, typename T1>
        actor(T0 const& _0, T1 const& _1)
            : Base(_0, _1) {}

        typename Base::template apply<arg_list<>, empty>::type
        operator()() const
        {
            return Base::eval(arg_list<>(), empty());
        }

        template <typename T0>
        typename Base::template apply<arg_list<T0>, empty>::type
        operator()(T0& _0) const
        {
            return Base::eval(arg_list<T0>(_0), empty());
        }

        template <typename T0, typename T1>
        typename Base::template apply<arg_list<T0, T1>, empty>::type
        operator()(T0& _0, T1& _1) const
        {
            return Base::eval(arg_list<T0, T1>(_0, _1), empty());
        }

        template <typename T1>
        typename impl::make_assign_composite<self_t, T1>::type
        operator=(T1 const& a1) const;

        template <typename T1>
        typename impl::make_index_composite<self_t, T1>::type
        operator[](T1 const& a1) const;

        //  Bring in the rest of the constructors and function call operators
        #include <boost/spirit/phoenix/core/impl/actor_constructor.ipp>
        #include <boost/spirit/phoenix/core/impl/actor_call.ipp>
    };
}}

#endif
