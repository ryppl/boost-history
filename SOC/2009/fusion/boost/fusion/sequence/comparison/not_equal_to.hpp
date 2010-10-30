/*==============================================================================
    Copyright (c) 1999-2003 Jaakko Jarvi
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_COMPARISON_NOT_EQUAL_TO_HPP
#define BOOST_FUSION_SEQUENCE_COMPARISON_NOT_EQUAL_TO_HPP

#include <boost/fusion/support/internal/base.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>

#include <boost/fusion/sequence/comparison/detail/enable_comparison.hpp>
#ifdef FUSION_DIRECT_OPERATOR_USAGE
#   include <boost/fusion/sequence/comparison/detail/not_equal_to.hpp>
#else
#   include <boost/fusion/sequence/comparison/equal_to.hpp>
#endif

namespace boost { namespace fusion
{
    template<typename Seq1, typename Seq2>
    inline bool
    not_equal_to(Seq1 const& seq1, Seq2 const& seq2)
    {
        BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq1>))
        BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq1>))
        BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq2>))
        BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq2>))

#ifdef FUSION_DIRECT_OPERATOR_USAGE
        return
            detail::sequence_not_equal_to<
                Seq1 const&
              , Seq2 const&
              , result_of::size<Seq1 const&>::value==
                    result_of::size<Seq2 const&>::value
            >::call(fusion::begin(seq1), fusion::begin(seq2));
#else
        return !(seq1 == seq2);
#endif
    }

    namespace operators
    {
        template<typename Seq1, typename Seq2>
        inline typename
            enable_if<
                detail::enable_equality<Seq1 const&, Seq2 const&>
              , bool
            >::type
        operator!=(Seq1 const& seq1, Seq2 const& seq2)
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq1>))
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq1>))
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq2>))
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq2>))

            return fusion::not_equal_to(seq1, seq2);
        }
    }
    using operators::operator!=;
}}

#endif
