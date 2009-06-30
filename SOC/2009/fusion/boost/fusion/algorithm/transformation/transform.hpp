/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_TRANSFORM_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_TRANSFORM_HPP

#include <boost/fusion/view/transform_view/transform_view.hpp>

namespace boost { namespace fusion
{
    struct void_;

    namespace result_of
    {
        template <typename Seq1, typename Seq2, typename F = void_>
        struct transform
        {
            typedef transform_view<Seq1, Seq2, F> type;
        };

        template <typename Seq, typename F>
#if defined(BOOST_NO_PARTIAL_SPECIALIZATION_IMPLICIT_DEFAULT_ARGS)
        struct transform<Sequence, F, void_>
#else
        struct transform<Seq, F>
#endif
        {
            typedef transform_view<Seq, F> type;
        };
    }

    template <typename Seq, typename F>
    inline typename result_of::transform<Seq const&, F>::type
    transform(Seq const& seq, F f)
    {
        return typename result_of::transform<Seq const&, F>::type(seq, f);
    }

    template <typename Sequence1, typename Sequence2, typename F>
    inline typename result_of::transform<Sequence1 const&, Sequence2 const&, F>::type
    transform(Sequence1 const& seq1, Sequence2 const& seq2, F f)
    {
        return typename result_of::transform<Sequence1 const&, Sequence2 const&, F>::type(seq1, seq2, f);
    }
}}

#endif
