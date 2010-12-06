/*=============================================================================
    Copyright (c) 2005-2010 Joel de Guzman
    Copyright (c) 2010 Eric Niebler
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_CORE_META_GRAMMAR_HPP
#define PHOENIX_CORE_META_GRAMMAR_HPP

#include <boost/mpl/deref.hpp>
#include <boost/phoenix/core/environment.hpp>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/matches.hpp>
#include <boost/proto/traits.hpp>
#include <boost/proto/tags.hpp>
#include <boost/proto/transform/arg.hpp>
#include <boost/proto/transform/default.hpp>
#include <boost/proto/transform/when.hpp>

namespace boost { namespace phoenix
{
    ////////////////////////////////////////////////////////////////////////////
    // The grammar defining valid phoenix expressions
    struct meta_grammar
        : proto::switch_<meta_grammar>
    {
        template <typename Tag, typename Dummy = void>
        struct case_
            : proto::not_<proto::_>
        {};
    };

    ////////////////////////////////////////////////////////////////////////////
    // An evaluation transform you can call to dispatch to different actions
    struct evaluator
        : proto::call<
            meta_grammar(
                proto::_
              , functional::args(proto::_state)
              , functional::actions(proto::_state)
            )
        >
    {};
    
    ////////////////////////////////////////////////////////////////////////////
    // Set of default actions. Extend this whenever you add a new phoenix construct
    struct default_actions
    {
        template <typename Rule, typename Grammar = meta_grammar>
        struct when
            : proto::_default<Grammar>
        {};
    };

    ////////////////////////////////////////////////////////////////////////////
    // A function object we can call to evaluate our expression
    evaluator const eval = evaluator();
}}

#endif
