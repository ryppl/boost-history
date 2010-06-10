/*==============================================================================
    Copyright (c) 2005-2010 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_CORE_IS_ACTOR_HPP
#define PHOENIX_CORE_IS_ACTOR_HPP

namespace boost { namespace phoenix
{
///////////////////////////////////////////////////////////////////////////////
//
//  is_actor<T>
//
//      Tests if T is an actor. Evaluates to mpl::true_ or mpl::false_
//
///////////////////////////////////////////////////////////////////////////////
    template <typename T, typename Enable = void>
    struct is_actor : mpl::false_ {};

    template <typename Expr>
    struct is_actor<actor<Expr> > : mpl::true_ {};
}}

#endif
