/*=============================================================================
    Copyright (c) 1999-2003 Jaakko Jarvi
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_COMPARISON_LESS_HPP
#define BOOST_FUSION_SEQUENCE_COMPARISON_LESS_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>

#include <boost/fusion/sequence/comparison/detail/less.hpp>
#include <boost/fusion/sequence/comparison/detail/enable_comparison.hpp>

namespace boost { namespace fusion
{
    template <typename Seq1, typename Seq2>
    inline bool
    less(Seq1 const& seq1, Seq2 const& seq2)
    {
        return
            detail::sequence_less<
                Seq1 const&
              , Seq2 const&
            >::call(fusion::begin(seq1), fusion::begin(seq2));
    }

    namespace operators
    {
        template <typename Seq1, typename Seq2>
        inline typename
            enable_if<
                detail::enable_equality<Seq1 const&, Seq2 const&>
              , bool
            >::type
        operator<(Seq1 const& seq1, Seq2 const& seq2)
        {
            return fusion::less(seq1, seq2);
        }
    }
    using operators::operator<;
}}

#endif
