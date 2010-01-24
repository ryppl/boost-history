/*=============================================================================
    Copyright (c) 2006 Eric Niebler
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_INTRINSIC_EXT_SIZE_S_HPP
#define BOOST_FUSION_SEQUENCE_INTRINSIC_EXT_SIZE_S_HPP

#include <boost/fusion/sequence/intrinsic/ext_/segments.hpp>
#include <boost/fusion/algorithm/iteration/fold.hpp>
#include <boost/fusion/support/ext_/is_segmented.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/size_t.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template<
            typename Sequence
          , bool IsSegmented = traits::is_segmented<Sequence>::value
        >
        struct segmented_size;
    }

    namespace detail
    {
        struct size_plus
        {
            template<typename Sig>
            struct result;

            template<typename Self, typename State, typename Seq>
            struct result<Self(State, Seq)>
              : mpl::plus<
                    result_of::segmented_size<
                        typename detail::remove_reference<Seq>::type
                    >
                  , typename detail::remove_reference<State>::type
                >
            {};

            template<typename State, typename Seq>
            typename result<size_plus(State,Seq)>::type
            operator()(State,Seq);
        };
    }

    namespace result_of
    {
        template<typename Seq>
        struct segmented_size<Seq, true>
          : detail::remove_reference<
                typename result_of::fold<
                    typename result_of::segments<Seq>::type
                  , mpl::size_t<0>
                  , detail::size_plus
                >::type
            >::type
        {};

        template<typename Seq>
        struct segmented_size<Seq, false>
          : result_of::size<Seq>
        {};
    }
}}

#endif
