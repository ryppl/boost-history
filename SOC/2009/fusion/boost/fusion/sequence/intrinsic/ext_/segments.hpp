/*=============================================================================
    Copyright (c) 2006 Eric Niebler
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_INTRINSIC_EXT_SEGMENTS_HPP
#define BOOST_FUSION_SEQUENCE_INTRINSIC_EXT_SEGMENTS_HPP

#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/support/internal/assert.hpp>
#ifdef BOOST_FUSION_ENABLE_STATIC_ASSERTS
#   include <boost/fusion/supported/ext_/is_segmented.hpp>
#endif

namespace boost { namespace fusion
{
    namespace extension
    {
        template <typename>
        struct segments_impl;
    }

    namespace result_of
    {
        template<typename Seq>
        struct segments
          : extension::segments_impl<
                typename traits::tag_of<Seq>::type
            >::template apply<Seq>
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_segmented<Seq>));
        };
    }

    template<typename Seq>
    typename result_of::segments<Seq>::type
    segments(Seq& seq)
    {
        return result_of::segments<Seq>::call(seq);
    }

    template<typename Seq>
    typename result_of::segments<Seq const>::type
    segments(Seq const& seq)
    {
        return result_of::segments<Seq const>::call(seq);
    }

    /*template<typename Seq>
    typename result_of::segments<BOOST_FUSION_R_ELSE_CLREF(Seq)>::type
    segments(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return result_of::segments<BOOST_FUSION_R_ELSE_CLREF(Seq)>::call(
                    BOOST_FUSION_FORWARD(Seq,seq));
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template<typename Seq>
    typename result_of::segments<Seq&>::type
    segments(Seq& seq)
    {
        return result_of::segments<Seq&>::call(seq);
    }
#endif*/
}}

#endif
