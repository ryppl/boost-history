/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_CORE_NOTHING_HPP
#define PHOENIX_CORE_NOTHING_HPP

#include <boost/spirit/phoenix/core/actor.hpp>

namespace boost { namespace phoenix
{
///////////////////////////////////////////////////////////////////////////////
//
//  null_actor
//
//      A actor that does nothing (a "bum", if you will :-).
//
///////////////////////////////////////////////////////////////////////////////
    struct null_actor
    {
        template <typename Args, typename Env>
        struct apply
        {
            typedef void type;
        };

        template <typename Args, typename Env>
        void
        eval(Args const&, Env const&) const
        {
        }
    };

    null_actor const nothing = null_actor();
}}

#endif
