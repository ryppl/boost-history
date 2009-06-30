/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_INTRINSIC_BACK_HPP
#define BOOST_FUSION_SEQUENCE_INTRINSIC_BACK_HPP

#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/iterator/prior.hpp>
#include <boost/fusion/iterator/deref.hpp>

#include <boost/mpl/bool.hpp>

namespace boost { namespace fusion
{
    // Special tags:
    struct fusion_sequence_tag;

    namespace result_of
    {
        template <typename Sequence>
        struct back
            : result_of::deref<typename result_of::prior<
                typename result_of::end<Sequence>::type>::type
                >
        {};
    }

    template <typename Sequence>
    inline typename result_of::back<BOOST_FUSION_R_ELSE_CLREF(Sequence)>::type
    back(BOOST_FUSION_R_ELSE_CLREF(Sequence) seq)
    {
        return *fusion::prior(fusion::end(BOOST_FUSION_FORWARD(Sequence,seq)));
    }
}}

#endif
