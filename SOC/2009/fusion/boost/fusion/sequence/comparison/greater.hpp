/*=============================================================================
    Copyright (c) 1999-2003 Jaakko Jarvi
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_COMPARISON_GREATER_HPP
#define BOOST_FUSION_SEQUENCE_COMPARISON_GREATER_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/support/internal/assert.hpp>

#include <boost/fusion/sequence/comparison/detail/enable_comparison.hpp>
#ifdef FUSION_DIRECT_OPERATOR_USAGE
#   include <boost/fusion/sequence/comparison/detail/greater.hpp>
#else
#   include <boost/fusion/sequence/comparison/less.hpp>
#endif

namespace boost { namespace fusion
{
    template<typename Seq1, typename Seq2>
    inline bool
    greater(Seq1 const& seq1, Seq2 const& seq2)
    {
        BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq1>));
        BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq1>));
        BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq2>));
        BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq2>));

#ifdef FUSION_DIRECT_OPERATOR_USAGE
        return
            detail::sequence_greater<
                Seq1 const&
              , Seq2 const&
            >::call(fusion::begin(seq1), fusion::begin(seq2));
#else
        return seq2 < seq1;
#endif
    }

    namespace operators
    {
        template<typename Seq1, typename Seq2>
        inline typename
            enable_if<
                detail::enable_comparison<Seq1 const&, Seq2 const&>
              , bool
            >::type
        operator>(Seq1 const& seq1, Seq2 const& seq2)
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq1>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq1>));
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq2>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq2>));

            return fusion::greater(seq1, seq2);
        }
    }
    using operators::operator>;
}}

#endif
