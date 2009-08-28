/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_SINGLE_VIEW_DETAIL_END_IMPL_HPP
#define BOOST_FUSION_VIEW_SINGLE_VIEW_DETAIL_END_IMPL_HPP

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct end_impl;

    template <>
    struct end_impl<single_view_tag>
    {
        template <typename Seq>
        struct apply
        {
            typedef
                single_view_iterator_end<typename detail::add_lref<Seq>::type>
            type;

            static type
            call(Seq)
            {
                return type();
            }
        };
    };
}}}

#endif
