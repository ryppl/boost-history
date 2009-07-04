// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_SUPPORT_ITERATOR_ADAPTER_DEREF_IMPL_HPP
#define BOOST_FUSION_SUPPORT_ITERATOR_ADAPTER_DEREF_IMPL_HPP

#include <boost/fusion/iterator/deref.hpp>

namespace boost { namespace fusion
{
    struct iterator_adapter_tag;

    namespace extension
    {
        template <typename Tag>
        struct deref_impl;

        template <>
        struct deref_impl<iterator_adapter_tag>
        {
            template <typename Iterator>
            struct apply
            {
                typedef typename
                    detail::result_of_forward_as<
                        Iterator
                      , typename detail::remove_reference<Iterator>::type::iterator_type
                    >::type
                iterator_type;

                typedef typename
                    result_of::deref<iterator_type>::type
                type;

                static type
                call(Iterator i)
                {
                    return fusion::deref(i.it);
                }
            };
        };
    }
}}

#endif
