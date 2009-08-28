/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_REPETITIVE_VIEW_DETAIL_PRIOR_IMPL_HPP
#define BOOST_FUSION_VIEW_REPETITIVE_VIEW_DETAIL_PRIOR_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/iterator/prior.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/equal_to.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <>
    struct prior_impl<repetitive_view_iterator_tag>
    {
        template <typename It>
        struct apply
        {
            typedef typename detail::remove_reference<It>::type it;
            typedef typename
                detail::remove_reference<typename it::seq_type>::seq_type
            real_seq_type;

            typedef
                repetitive_view_iterator<
                    typename it::seq_type
                  , typename mpl::eval_if<
                        typename it::index
                      , result_of::prior<typename it::it>
                      , result_of::end<real_seq_type>
                    >::type
                  , mpl::next<typename it::index>::value
                >
            type;

            static type
            call_impl(It it, mpl::true_ /*need_end_it*/)
            {
                return type(it.seq,fusion::end(it.seq.seq.get()));
            }

            static type
            call_impl(It it, mpl::false_ /*need_end_it*/)
            {
                return type(it.seq,fusion::prior(it.it));
            }

            static type
            call(It it)
            {
                return call_impl(it,mpl::bool_<it::index::value>());
            }
        };
    };
}}}

#endif
