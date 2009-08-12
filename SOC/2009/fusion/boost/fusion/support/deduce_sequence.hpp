/*=============================================================================
    Copyright (c) 2007 Tobias Schwinger

    Use modification and distribution are subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_DEDUCE_SEQUENCE_HPP
#define BOOST_FUSION_SUPPORT_DEDUCE_SEQUENCE_HPP

#include <boost/fusion/container/vector/convert.hpp>
#include <boost/fusion/view/transform_view.hpp>
#include <boost/fusion/support/deduce.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        struct deducer
        {
            template <typename Sig>
            struct result;

            template<typename Self,typename T>
            struct result<Self(T)>
              : fusion::traits::deduce<T>
            {};
        };
    }

    namespace traits
    {
        template <typename Seq>
        struct deduce_sequence
          : result_of::as_vector<fusion::transform_view<Seq, detail::deducer> >
        {};
    }
}}

#endif
