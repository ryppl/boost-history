/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_CORE_VALUE_HPP
#define PHOENIX_CORE_VALUE_HPP

#include <boost/spirit/phoenix/core/actor.hpp>
#include <boost/spirit/phoenix/core/as_actor.hpp>
#include <boost/call_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/mpl/not.hpp>

namespace boost { namespace phoenix
{
///////////////////////////////////////////////////////////////////////////////
//
//  value class
//
//      Lazy values
//
//      A bound actual parameter is kept in a value class for deferred access
//      later when needed. A value object is immutable. Value objects are
//      typically created through the val(x) free function which returns a
//      value<T> with T deduced from the type of x. x is held in the value<T>
//      object by value.
//
//      Lazy values are actors. As such, lazy values can be evaluated through
//      the actor's operator(). Such invocation gives the value's identity.
//      Example:
//
//          cout << val(3)() << val("Hello World")();
//
//      prints out "3 Hello World"
//
///////////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct value
    {
        BOOST_STATIC_ASSERT(
            mpl::not_<is_reference<T> >::value != 0);

        template <typename Args, typename Env>
        struct apply
        {
            typedef T type;
        };

        value(typename call_traits<T>::param_type arg)
            : val(arg) {}

        template <typename Args, typename Env>
        typename call_traits<T>::const_reference
        eval(Args const&, Env const&) const
        {
            return val;
        }

        T val;
    };

///////////////////////////////////////////////////////////////////////////////
//
//  Arbitrary types T are typically converted to an actor<value<T> > (see
//  as_actor.hpp). A specialization is also provided for arrays. T[N] arrays
//  are converted to actor<value<T const*> >.
//
///////////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct as_actor
    {
        typedef actor<value<T> > type;

        static actor<value<T> >
        convert(T const& x)
        {
            return value<T>(x);
        }
    };

    template <typename T, int N>
    struct as_actor<T[N]>
    {
        typedef actor<value<T const*> > type;

        static actor<value<T const*> >
        convert(T const x[N])
        {
            return value<T const*>(x);
        }
    };

///////////////////////////////////////////////////////////////////////////////
//
//  val function
//
//      value<T> objects are typically created through the val(x) free
//      function which returns a value<T> with T deduced from the type of x.
//
///////////////////////////////////////////////////////////////////////////////
    template <typename T>
    inline typename as_actor<T>::type
    val(T const& v)
    {
        return as_actor<T>::convert(v);
    }
}}

#endif
