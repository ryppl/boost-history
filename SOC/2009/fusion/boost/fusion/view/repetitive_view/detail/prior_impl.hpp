/*==============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_REPETITIVE_VIEW_DETAIL_PRIOR_IMPL_HPP
#define BOOST_FUSION_VIEW_REPETITIVE_VIEW_DETAIL_PRIOR_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/iterator/prior.hpp>
#include <boost/fusion/iterator/equal_to.hpp>

#include <boost/mpl/prior.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/equal_to.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<>
    struct prior_impl<repetitive_view_iterator_tag>
    {
        template<typename It>
        struct apply
        {
            typedef typename detail::remove_reference<It>::type it;
            typedef typename
                result_of::end<typename it::seq_type>::type
            seq_end;
            typedef
                result_of::equal_to<
                    typename it::it_type
                  , typename result_of::begin<typename it::seq_type>::type
                >
            new_iterator;

            typedef
                repetitive_view_iterator<
                    typename it::seq_type
                  , typename result_of::prior<
                        typename mpl::if_c<
                            new_iterator::value
                          , seq_end
                          , typename it::it_type
                        >::type
                    >::type
                  , typename boost::mpl::prior<typename it::index>::type
                >
            type;

            static type
            call(It it, mpl::false_ /*new_iterator*/)
            {
                return type(
                    it.seq.template get<typename it::seq_type>(),
                    fusion::prior(it.it));
            }

            static type
            call(It it, mpl::true_ /*new_iterator*/)
            {
                return type(
                    it.seq.template get<typename it::seq_type>(),
                    fusion::prior(fusion::end(*it.seq)));
            }

            static type
            call(It it)
            {
                return call(it,new_iterator());
            }
        };
    };
}}}

#endif
