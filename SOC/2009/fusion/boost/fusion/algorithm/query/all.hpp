/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2007 Dan Marsden
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_QUERY_ALL_HPP
#define BOOST_FUSION_ALGORITHM_QUERY_ALL_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>

#include <boost/fusion/algorithm/query/detail/all.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq, typename>
        struct all
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));

            typedef bool type;
        };
    }

    template <typename Seq, typename F>
    inline typename
        result_of::all<
            BOOST_FUSION_R_ELSE_CLREF(Seq)
          , BOOST_FUSION_RREF_ELSE_OBJ(F)
        >::type
    all(BOOST_FUSION_R_ELSE_CLREF(Seq) seq, BOOST_FUSION_RREF_ELSE_OBJ(F) f)
    {
        return
            detail::unrolled_all<
                result_of::size<BOOST_FUSION_R_ELSE_CLREF(Seq)>::value
            >::call(
                    fusion::begin(BOOST_FUSION_FORWARD(Seq,seq))
                  , BOOST_FUSION_FORWARD(F,f));
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename Seq, typename F>
    inline typename result_of::all<Seq&, F>::type
    all(Seq& seq, F f)
    {
        return
            detail::unrolled_all<
                result_of::size<Seq const&>::value
            >::call(fusion::begin(BOOST_FUSION_FORWARD(Seq,seq)), f);
    }
#endif
}}

#endif
