/*=============================================================================
    Copyright (c) 2001-2007 Joel de Guzman
    Copyright (c) 2007 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_ITERATION_FOLD_HPP
#define BOOST_FUSION_ALGORITHM_ITERATION_FOLD_HPP

#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/support/assert.hpp>

#include <boost/fusion/algorithm/iteration/detail/fold.hpp>

namespace boost { namespace fusion {

    struct random_access_traversal_tag;

    namespace result_of
    {
        template <typename Seq, typename State, typename F>
        struct fold
        {
            //BOOST_FUSION_MPL_ASSERT((traits_is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));

            typedef
                detail::fold_impl<
                    size<Seq>::value
                  , typename begin<Seq>::type
                  , typename detail::add_lref<State>::type
                  , typename detail::add_lref<F>::type
                >
            gen;

            typedef typename gen::type type;
        };
    }

    template <typename Seq, typename State, typename F>
    inline typename
        result_of::fold<
            BOOST_FUSION_R_ELSE_CLREF(Seq)
          , BOOST_FUSION_R_ELSE_CLREF(State)
          , BOOST_FUSION_R_ELSE_CLREF(F)
        >::type
    fold(BOOST_FUSION_R_ELSE_CLREF(Seq) seq,
         BOOST_FUSION_R_ELSE_CLREF(State) state,
         BOOST_FUSION_R_ELSE_CLREF(F) f)
    {
        return
            result_of::fold<
                BOOST_FUSION_R_ELSE_CLREF(Seq)
              , BOOST_FUSION_R_ELSE_CLREF(State)
              , BOOST_FUSION_R_ELSE_CLREF(F)
            >::gen::call(fusion::begin(seq), state, f);
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename Seq, typename State, typename F>
    inline typename result_of::fold<Seq&,State const&,F const&>::type
    fold(Seq& seq,
         State const& state,
         F const& f)
    {
#ifdef BOOST_MSVC
#   pragma warning(push)
#   pragma warning(disable: 4180)
#endif
        return result_of::fold<Seq&,State const&,F const&>::gen::call(
                fusion::begin(seq), state, f);
#ifdef BOOST_MSVC
#   pragma warning(pop)
#endif
    }
#endif
}}

#endif
