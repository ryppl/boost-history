/*==============================================================================
    Copyright (c) 1999-2003 Jaakko Jarvi
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_COMPARISON_LESS_EQUAL_HPP
#define BOOST_FUSION_SEQUENCE_COMPARISON_LESS_EQUAL_HPP

#include <boost/fusion/support/internal/base.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>

#include <boost/fusion/sequence/comparison/detail/enable_comparison.hpp>
#   ifdef FUSION_DIRECT_OPERATOR_USAGE
#include <boost/fusion/sequence/comparison/detail/less_equal.hpp>
#else
#   include <boost/fusion/sequence/comparison/less.hpp>
#endif

namespace boost { namespace fusion
{
    template<typename Seq1, typename Seq2>
    inline bool
    less_equal(Seq1 const& seq1, Seq2 const& seq2)
    {
        BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq1>))
        BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq1>))
        BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq2>))
        BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq2>))

#ifdef FUSION_DIRECT_OPERATOR_USAGE
        return
            detail::sequence_less_equal<
                Seq1 const&
              , Seq2 const&
              , result_of::size<Seq1 const&>::value==
                    result_of::size<Seq2 const&>::value
            >::call(fusion::begin(seq1), fusion::begin(seq2));
#else
        return !(seq2 < seq1);
#endif
    }

    namespace operators
    {
#if defined(BOOST_MSVC) && (BOOST_MSVC <= 1400) 
// Workaround for  VC8.0 and VC7.1
        template<typename Seq1, typename Seq2>
        inline bool
        operator<=(sequence_base<Seq1> const& seq1, sequence_base<Seq2 const&> const& seq2)
        {
            return less_equal(seq1.derived(), seq2.derived());
        }

        template<typename Seq1, typename Seq2>
        inline typename disable_if<detail::is_native_fusion_sequence<Seq2 const&>, bool>::type
        operator<=(sequence_base<Seq1> const& seq1, Seq2 const& seq2)
        {
            return less_equal(seq1.derived(), seq2);
        }

        template<typename Seq1, typename Seq2>
        inline typename disable_if<detail::is_native_fusion_sequence<Seq1 const&>, bool>::type
        operator<=(Seq1 const& seq1, sequence_base<Seq2> const& b)
        {
            return less_equal(seq1, seq2.derived());
        }

#else
// Somehow VC8.0 and VC7.1 does not like this code
// but barfs somewhere else. 
        template<typename Seq1, typename Seq2>
        inline typename
            enable_if<
                detail::enable_comparison<Seq1 const&, Seq2 const&>
              , bool
            >::type
        operator<=(Seq1 const& seq1, Seq2 const& seq2)
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq1>))
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq1>))
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq2>))
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq2>))

            return fusion::less_equal(seq1, seq2);
        }
#endif
    }
    using operators::operator<=;
}}

#endif
