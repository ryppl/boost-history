/*=============================================================================
    Copyright (c) 2001-2007 Joel de Guzman
    Copyright (c) 2007 Dan Marsden
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_ITERATION_FOR_EACH_HPP
#define BOOST_FUSION_ALGORITHM_ITERATION_FOR_EACH_HPP

#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>

#include <boost/fusion/algorithm/iteration/detail/for_each.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq, typename>
        struct for_each
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));

            typedef void type;
        };
    }

    template <typename Seq, typename F>
    inline typename
        result_of::for_each<
            BOOST_FUSION_R_ELSE_CLREF(Seq)
          , BOOST_FUSION_RREF_ELSE_OBJ(F)
        >::type
    for_each(BOOST_FUSION_R_ELSE_CLREF(Seq) seq,
            BOOST_FUSION_RREF_ELSE_OBJ(F) f)
    {
        detail::for_each_unrolled<
            result_of::size<BOOST_FUSION_R_ELSE_CLREF(Seq)>::value
        >::call(fusion::begin(BOOST_FUSION_FORWARD(Seq,seq)),
                BOOST_FUSION_FORWARD(F,f));
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename Seq, typename F>
    inline typename result_of::for_each<Seq&,F>::type
    for_each(Seq& seq,F f)
    {
        detail::for_each_unrolled<result_of::size<Seq>::value>::call(
                fusion::begin(seq),f);
    }
#endif
}}

#endif
