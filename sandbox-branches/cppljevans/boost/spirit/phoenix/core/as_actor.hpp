/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_CORE_AS_ACTOR_HPP
#define PHOENIX_CORE_AS_ACTOR_HPP

#include <boost/spirit/phoenix/core/actor.hpp>

namespace boost { namespace phoenix
{
///////////////////////////////////////////////////////////////////////////////
//
//  as_actor
//
//      as_actor is a meta-program that converts an arbitrary type into an
//      actor. All participants in the framework must be first-class actors.
//      This meta-program is used all throughout the framework whenever an
//      unknown type needs to be converted to an actor. as_actor
//      specializations are expected to have a typedef 'type'. This is the
//      destination actor type. A static member function 'convert' converts
//      an object to this target type.
//
//      The meta-program does no conversion if the object to be converted is
//      already an actor.
//
///////////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct as_actor; // defined in value.hpp

    template <typename Base>
    struct as_actor<actor<Base> >
    {
        typedef actor<Base> type;

        static actor<Base> const&
        convert(actor<Base> const& x)
        {
            return x;
        }
    };

    template <>
    struct as_actor<void_t>
    {
        typedef void_t type;
        struct error_attempting_to_convert_void_t_to_an_actor {};

        static void
        convert(error_attempting_to_convert_void_t_to_an_actor);
    };

    template <>
    struct as_actor<void>
    {
        typedef void type;
        struct error_attempting_to_convert_void_to_an_actor {};

        static void
        convert(error_attempting_to_convert_void_to_an_actor);
    };
}}

#endif
