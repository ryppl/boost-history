/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005 Eric Niebler
    Copyright (c) 2007 Dan Marsden
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_QUERY_ANY_HPP
#define BOOST_FUSION_ALGORITHM_QUERY_ANY_HPP

#include <boost/fusion/algorithm/query/all.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        template<typename F>
        struct any_helper
        {
            typedef bool result_type;

            any_helper(F f)
              : f(f)
            {}

            template<typename E>
            inline bool
            operator()(BOOST_FUSION_R_ELSE_CLREF(E) e)
            {
                return !f(BOOST_FUSION_FORWARD(E,e));
            }

            F f;
        };
    }

    namespace result_of
    {
        template <typename Seq, typename>
        struct any
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));

            typedef bool type;
        };
    }

    template <typename Seq, typename F>
    inline typename
        result_of::any<
            BOOST_FUSION_R_ELSE_CLREF(Seq)
          , BOOST_FUSION_RREF_ELSE_OBJ(F)
        >::type
    any(BOOST_FUSION_R_ELSE_CLREF(Seq) seq, BOOST_FUSION_RREF_ELSE_OBJ(F) f)
    {
        return !fusion::all(
                BOOST_FUSION_FORWARD(Seq,seq),
                detail::any_helper<BOOST_FUSION_RREF_ELSE_OBJ(F)>(f));
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename Seq, typename F>
    inline typename result_of::any<Seq&,F>::type
    any(Seq& seq, F f)
    {
        return !fusion::all(seq,detail::any_helper<F>(f));
    }
#endif
}}

#endif
