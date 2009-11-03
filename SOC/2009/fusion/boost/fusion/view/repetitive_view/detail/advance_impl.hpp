/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_REPETITIVE_VIEW_DETAIL_ADVANCE_IMPL_HPP
#define BOOST_FUSION_VIEW_REPETITIVE_VIEW_DETAIL_ADVANCE_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/empty.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/iterator/advance.hpp>

#include <boost/mpl/less.hpp>
#include <boost/mpl/modulus.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/or.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        template<
            typename It
          , typename N
          , bool /*ReuseOldIterator*/=
                mpl::or_<
                    result_of::empty<typename It::seq_type>
                  , mpl::less<
                        mpl::plus<typename It::index, N>
                      , result_of::size<typename It::seq_type>
                    >
                >::value /*true*/
        >
        struct get_cyclic_next_iterator
        {
            typedef typename
                result_of::advance<typename It::it_type, N>::type
            type;

            static type
            call(It const& it)
            {
                return fusion::advance<N>(it.it);
            }
        };

        template<typename It, typename N>
        struct get_cyclic_next_iterator<It,N,/*ReuseOldIterator*/false>
        {
            typedef
                mpl::modulus<
                    mpl::plus<typename It::index, N>
                  , result_of::size<typename It::seq_type>
                >
            real_n;

            typedef typename
                result_of::advance<
                    typename result_of::begin<typename It::seq_type>::type
                  , real_n
                >::type
            type;

            static type
            call(It const& it)
            {
                return fusion::advance<real_n>(fusion::begin(*it.seq));
            }
        };
    }

    namespace extension
    {
        template<typename>
        struct advance_impl;

        template<>
        struct advance_impl<repetitive_view_iterator_tag>
        {
            template<typename It, typename N>
            struct apply
            {
                typedef typename detail::remove_reference<It>::type it;
                typedef detail::get_cyclic_next_iterator<it, N> gen;

                typedef
                    repetitive_view_iterator<
                        typename it::seq_type
                      , typename gen::type
                      , mpl::plus<typename it::index, N>::value
                    >
                type;

                static type
                call(It it)
                {
                    return type(it.seq.get(),gen::call(it));
                }
            };
        };

    }
}}

#endif
