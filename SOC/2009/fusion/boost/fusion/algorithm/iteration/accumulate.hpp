/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_ITERATION_ACCUMULATE_HPP
#define BOOST_FUSION_ALGORITHM_ITERATION_ACCUMULATE_HPP

#include <boost/fusion/algorithm/iteration/fold.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq, typename State, typename F>
        struct accumulate
          : fold<Seq, State, F>
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));
        };
    }

    template <typename Seq, typename State, typename F>
    inline typename result_of::accumulate<
        BOOST_FUSION_R_ELSE_CLREF(Seq)
      , BOOST_FUSION_R_ELSE_CLREF(State)
      , BOOST_FUSION_RREF_ELSE_OBJ(F)
    >::type
    accumulate(BOOST_FUSION_R_ELSE_CLREF(Seq) seq,
            BOOST_FUSION_R_ELSE_CLREF(State) state,
            BOOST_FUSION_RREF_ELSE_OBJ(F) f)
    {
        return fusion::fold(
                BOOST_FUSION_FORWARD(Seq,seq),
                BOOST_FUSION_FORWARD(State,state),
                BOOST_FUSION_FORWARD(F,f));
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename Seq, typename State, typename F>
    inline typename
        result_of::accumulate<Seq&, State const&, F>::type
    accumulate(Seq& seq,State const& state,F f)
    {
        return fusion::fold(seq,state,f);
    }
#endif
}}

#endif
