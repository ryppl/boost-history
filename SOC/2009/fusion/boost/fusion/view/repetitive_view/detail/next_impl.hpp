/*=============================================================================
    Copyright (c) 2007 Tobias Schwinger
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_REPETITIVE_VIEW_DETAIL_NEXT_IMPL_HPP
#define BOOST_FUSION_VIEW_REPETITIVE_VIEW_DETAIL_NEXT_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/equal_to.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/identity.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <>
    struct next_impl<repetitive_view_iterator_tag>
    {
        template <typename It>
        struct apply
        {
            typedef typename detail::remove_reference<It>::type it;
            typedef typename
                result_of::next<typename it::it_type>::type
            next_it;
            typedef
                result_of::equal_to<
                    next_it
                  , typename result_of::end<typename it::seq_type>::type
                >
            new_iterator;

            typedef
                repetitive_view_iterator<
                    typename it::seq_type
                  , typename mpl::eval_if<
                        new_iterator
                      , result_of::begin<typename it::seq_type>
                      , mpl::identity<next_it>
                    >::type
                  , typename boost::mpl::next<typename it::index>::type
                >
            type;

            static type
            call_impl(It it, mpl::false_ /*new_iterator*/)
            {
                return type(*it.seq,fusion::next(it.it));
            }

            static type
            call_impl(It it, mpl::true_ /*new_iterator*/)
            {
                return type(*it.seq,begin(*it.seq));
            }

            static type
            call(It it)
            {
                return call_impl(it,new_iterator());
            }
        };
    };
}}}

#endif
