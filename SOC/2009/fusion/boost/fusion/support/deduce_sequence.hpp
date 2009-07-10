/*=============================================================================
    Copyright (c) 2007 Tobias Schwinger

    Use modification and distribution are subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_DEDUCE_SEQUENCE_HPP
#define BOOST_FUSION_SUPPORT_DEDUCE_SEQUENCE_HPP

#include <boost/fusion/support/deduce.hpp>
#include <boost/fusion/container/vector/convert.hpp>
#include <boost/fusion/view/transform_view.hpp>

namespace boost { namespace fusion { namespace traits
{
    template <class Sequence> struct deduce_sequence;

    namespace detail
    {
        struct deducer
        {
            template <typename Sig>
            struct result;

            template<typename T>
            struct result<deducer(T)>
              : fusion::traits::deduce<T>
            {};
        };
    }

    template <class Seq>
    struct deduce_sequence
      : result_of::as_vector<fusion::transform_view<Seq, detail::deducer> >
    {};

}}}

#endif
