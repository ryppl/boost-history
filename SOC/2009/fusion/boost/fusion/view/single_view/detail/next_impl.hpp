/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_SINGLE_VIEW_DETAIL_NEXT_IMPL_HPP
#define BOOST_FUSION_VIEW_SINGLE_VIEW_DETAIL_NEXT_IMPL_HPP

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct next_impl;

    template <>
    struct next_impl<single_view_iterator_tag>
    {
        template <typename It>
        struct apply
        {
            typedef
                single_view_iterator_end<
                    typename detail::remove_reference<
                        It
                    >::type::view_type
                >
            type;

            static type
            call(It)
            {
                return type();
            }
        };
    };
}}}

#endif
