/*=============================================================================
    Copyright (c) 2001-2007 Joel de Guzman
    Copyright (c) 2007 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_ITERATION_FOLD_HPP
#define BOOST_FUSION_ALGORITHM_ITERATION_FOLD_HPP

#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/ref.hpp>

#include <boost/type_traits/is_base_of.hpp>

#include <boost/fusion/algorithm/iteration/detail/fold.hpp>

namespace boost { namespace fusion {

    struct random_access_traversal_tag;

    namespace result_of
    {
        template <typename Seq, typename State, typename F>
        struct fold
          : fusion::detail::choose_fold<
                typename detail::add_lref<Seq>::type
              , typename detail::add_lref<State>::type
              , typename detail::add_lref<F>::type
              , is_base_of<
                    random_access_traversal_tag
                  , typename traits::category_of<Seq>::type
                >::value
            >
        {};
    }

    template <typename Sequence, typename State, typename F>
    inline typename result_of::fold<
        BOOST_FUSION_R_ELSE_LREF(Sequence)
      , BOOST_FUSION_R_ELSE_LREF(State)
      , BOOST_FUSION_R_ELSE_LREF(F)
    >::type
    fold(BOOST_FUSION_R_ELSE_LREF(Sequence) seq,
         BOOST_FUSION_R_ELSE_LREF(State) state,
         BOOST_FUSION_R_ELSE_LREF(F) f)
    {
        return detail::fold(
                BOOST_FUSION_FORWARD(Sequence,seq),
                BOOST_FUSION_FORWARD(State,state),
                BOOST_FUSION_FORWARD(F,f),
                typename traits::category_of<Sequence>::type());
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename Sequence, typename State, typename F>
    inline typename result_of::fold<
        BOOST_FUSION_R_ELSE_LREF(Sequence)
      , BOOST_FUSION_R_ELSE_CLREF(State)
      , BOOST_FUSION_R_ELSE_LREF(F)
    >::type
    fold(BOOST_FUSION_R_ELSE_LREF(Sequence) seq,
         BOOST_FUSION_R_ELSE_CLREF(State) state,
         BOOST_FUSION_R_ELSE_LREF(F) f)
    {
        return detail::fold(
                BOOST_FUSION_FORWARD(Sequence,seq),
                BOOST_FUSION_FORWARD(State,state),
                BOOST_FUSION_FORWARD(F,f),
                typename traits::category_of<Sequence>::type());
    }
#endif
}}

#endif
