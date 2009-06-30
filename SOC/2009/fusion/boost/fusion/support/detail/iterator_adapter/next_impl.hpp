// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_SUPPORT_ITERATOR_ADAPTER_NEXT_IMPL_HPP
#define BOOST_FUSION_SUPPORT_ITERATOR_ADAPTER_NEXT_IMPL_HPP

#include <boost/fusion/iterator/next.hpp>

namespace boost { namespace fusion
{
    template<typename Iterator, typename NewCategory>
    struct iterator_adapter;

    struct iterator_adapter_tag;

    namespace extension
    {
        template <typename Tag>
        struct next_impl;

        template <>
        struct next_impl<iterator_adapter_tag>
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
                        typename result_of::next<iterator_type>::type
                      , typename identity_iterator::category
                    >
                type;

                static type
                call(Iterator i)
                {
                    return fusion::next(static_cast<iterator_type>(i.iterator));
                }
            };
        };
    }
}}

#endif
