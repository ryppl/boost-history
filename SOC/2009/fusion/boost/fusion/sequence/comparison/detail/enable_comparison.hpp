/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_COMPARISON_DETAIL_ENABLE_COMPARISON_HPP
#define BOOST_FUSION_SEQUENCE_COMPARISON_DETAIL_ENABLE_COMPARISON_HPP

#include <boost/fusion/support/is_sequence.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>

#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/type_traits/is_base_of.hpp>

namespace boost { namespace fusion { namespace detail
{
    template <typename SeqRef>
    struct is_native_fusion_sequence
      : is_base_of<sequence_root, typename detail::identity<SeqRef>::type>
    {};

    template <typename Seq1Ref, typename Seq2Ref>
    struct enable_equality
      : mpl::or_<
            is_native_fusion_sequence<Seq1Ref>
          , is_native_fusion_sequence<Seq2Ref>
        >
    {};

    template <typename Seq1Ref, typename Seq2Ref>
    struct enable_comparison
        : mpl::and_<
            mpl::or_<
                is_native_fusion_sequence<Seq1Ref>
              , is_native_fusion_sequence<Seq2Ref>
            >
          , mpl::equal_to<result_of::size<Seq1Ref>, result_of::size<Seq2Ref> >
        >
    {};

}}}

#endif
