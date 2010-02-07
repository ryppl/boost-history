/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_INTRINSIC_FRONT_HPP
#define BOOST_FUSION_SEQUENCE_INTRINSIC_FRONT_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>

#include <boost/mpl/bool.hpp>

namespace boost { namespace fusion
{
    struct fusion_sequence_tag;

    namespace result_of
    {
        template <typename Seq>
        struct front
          : deref<typename begin<Seq>::type>
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));
            BOOST_FUSION_MPL_ASSERT_NOT((empty<Seq>));
        };
    }

    template <typename Seq>
    inline typename result_of::front<BOOST_FUSION_R_ELSE_CLREF(Seq)>::type
    front(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return fusion::deref(fusion::begin(BOOST_FUSION_FORWARD(Seq,seq)));
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename Seq>
    inline typename result_of::front<Seq&>::type
    front(Seq& seq)
    {
        return fusion::deref(fusion::begin(seq));
    }
#endif
}}

#endif
