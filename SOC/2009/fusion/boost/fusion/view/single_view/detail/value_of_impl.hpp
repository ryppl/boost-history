/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_SINGLE_VIEW_DETAIL_VALUE_OF_IMPL_HPP
#define BOOST_FUSION_VIEW_SINGLE_VIEW_DETAIL_VALUE_OF_IMPL_HPP

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct value_of_impl;

    template <>
    struct value_of_impl<single_view_iterator_tag>
    {
        template <typename It>
        struct apply
        {
            typedef typename
                detail::remove_reference<
                    typename detail::remove_reference<
                        It
                    >::type::view_type
                >::type::value_type
            type;
        };
    };
}}}

#endif
