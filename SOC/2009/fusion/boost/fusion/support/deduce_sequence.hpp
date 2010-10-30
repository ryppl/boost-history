/*==============================================================================
    Copyright (c) 2007 Tobias Schwinger
    Copyright (c) 2009-2010 Christopher Schmidt

    Use modification and distribution are subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_DEDUCE_SEQUENCE_HPP
#define BOOST_FUSION_SUPPORT_DEDUCE_SEQUENCE_HPP

#include <boost/fusion/support/internal/base.hpp>
#include <boost/fusion/container/vector/convert.hpp>
#include <boost/fusion/algorithm/transformation/transform.hpp>
#include <boost/fusion/support/deduce.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        struct deducer
        {
            template<typename Sig>
            struct result;

            template<typename Self, typename T>
            struct result<Self(T)>
              : traits::deduce<T>
            {};

            template<typename T>
            typename result<deducer const&(T const&)>::type
            operator()(T const&)
            {}
        };
    }

    namespace traits
    {
        template<typename Seq>
        struct deduce_sequence
          : result_of::as_vector<
                typename result_of::transform<Seq, detail::deducer>::type
            >
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>))
        };
    }
}}

//#include <boost/fusion/mpl.hpp>
//#include <boost/fusion/container/vector/convert.hpp>
//#include <boost/fusion/adapted/mpl.hpp>
//#include <boost/fusion/support/deduce.hpp>
//
//#include <boost/mpl/transform_view.hpp>
//#include <boost/mpl/quote.hpp>
//
//namespace boost { namespace fusion { namespace traits
//{
//    template<typename Seq>
//    struct deduce_sequence
//      : result_of::as_vector<
//            mpl::transform_view<
//                typename detail::remove_reference<Seq>::type
//              , mpl::quote1<traits::deduce>
//            >
//        >
//    {
//        BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>))
//    };
//}}}

#endif
