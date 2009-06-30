// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_SUPPORT_ITERATOR_ADAPTER_PRIOR_IMPL_HPP
#define BOOST_FUSION_SUPPORT_ITERATOR_ADAPTER_PRIOR_IMPL_HPP

#include <boost/fusion/iterator/prior.hpp>

namespace boost { namespace fusion
{
    template<typename Iterator, typename NewCategory>
    struct iterator_adapter;

    struct iterator_adapter_tag;

    namespace extension
    {
        template <typename Tag>
        struct prior_impl;

        template <>
        struct prior_impl<iterator_adapter_tag>
        {
            template <typename Iterator>
            struct apply
            {
                typedef typename
                    detail::remove_reference<Iterator>::type
                identity_iterator;
                typedef typename
                    detail::result_of_forward_as<
                        Iterator
                      , typename identity_iterator::iterator_type
                    >::type
                iterator_type;

                typedef
                    iterator_adapter<
                        typename result_of::prior<iterator_type>::type
                      , typename identity_iterator::category
                    >
                type;

                static type
                call(Iterator i)
                {
                    return fusion::prior(
                            static_cast<iterator_type>(i.iterator));
                }
            };
        };
    }
}}

#endif
