/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_CORE_VARIABLE_HPP
#define PHOENIX_CORE_VARIABLE_HPP

#include <boost/spirit/phoenix/core/actor.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/mpl/not.hpp>

namespace boost { namespace phoenix
{
///////////////////////////////////////////////////////////////////////////////
//
//  variable class
//
//      Lazy variables
//
//      A bound actual parameter may also be held by non-const reference in a
//      variable class for deferred access later when needed. A variable
//      object is mutable, i.e. its referenced variable can be modified.
//      Variable objects are typically created through the var(x) free
//      function which returns a variable<T> with T deduced from the type of
//      x. x is held in the value<T> object by reference.
//
//      Lazy variables are actors. As such, lazy variables can be evaluated
//      through the actor's operator(). Such invocation gives the variables's
//      identity. Example:
//
//          int i = 3;
//          char const* s = "Hello World";
//          cout << var(i)() << var(s)();
//
//      prints out "3 Hello World"
//
//      Another free function invar(x) may also be used. invar(x) creates a
//      variable<T const&> object using a constant reference.
//
///////////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct variable
    {
        BOOST_STATIC_ASSERT(
            mpl::not_<is_reference<T> >::value != 0);

        template <typename Args, typename Env>
        struct apply
        {
            typedef T& type;
        };

        variable(T& arg)
            : ref(arg) {}

        template <typename Args, typename Env>
        T& eval(Args const&, Env const&) const
        {
            return ref;
        }

        T& ref;
    };

///////////////////////////////////////////////////////////////////////////////
//
//  var and invar functions
//
//      variable<T> objects are typically created through the var(x) free
//      function which returns a variable<T> with T deduced from the type
//      of x. x is held in the value<T> object by reference. Another free
//      function invar(x) may also be used. invar(x) creates a variable<T
//      const&> object using a constant reference.
//
///////////////////////////////////////////////////////////////////////////////
    template <typename T>
    inline actor<variable<T> > const
    var(T& v)
    {
        return variable<T>(v);
    }

    template <typename T>
    inline actor<variable<T const> > const
    invar(T const& v)
    {
        return variable<T const>(v);
    }

    namespace impl
    {
        struct error_attempting_to_convert_an_actor_to_a_reference;
    }

    template <typename Base>
    void
    var(actor<Base> const& v
        , impl::error_attempting_to_convert_an_actor_to_a_reference
            = impl::error_attempting_to_convert_an_actor_to_a_reference());

    template <typename Base>
    void
    invar(actor<Base> const& v
        , impl::error_attempting_to_convert_an_actor_to_a_reference
            = impl::error_attempting_to_convert_an_actor_to_a_reference());
}}

#endif
