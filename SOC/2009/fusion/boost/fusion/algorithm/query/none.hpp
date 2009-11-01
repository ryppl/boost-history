/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2007 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_QUERY_NONE_HPP
#define BOOST_FUSION_ALGORITHM_QUERY_NONE_HPP

#include <boost/fusion/algorithm/query/any.hpp>
#include <boost/fusion/support/internal/assert.hpp>
#include <boost/fusion/support/internal/ref.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq, typename F>
        struct none
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));

            typedef bool type;
        };
    }

    template <typename Seq, typename F>
    inline typename
        result_of::none<
            BOOST_FUSION_R_ELSE_CLREF(Seq)
          , BOOST_FUSION_RREF_ELSE_OBJ(F)
        >::type
    none(BOOST_FUSION_R_ELSE_CLREF(Seq) seq, BOOST_FUSION_RREF_ELSE_OBJ(F) f)
    {
        return !fusion::any(
                BOOST_FUSION_FORWARD(Seq,seq),
                BOOST_FUSION_FORWARD(F,f));
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename Seq, typename F>
    inline typename result_of::none<Seq&, F>::type
    none(Seq& seq, F f)
    {
        return !fusion::any(seq,f);
    }
#endif
}}

#endif
