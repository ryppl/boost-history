/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_SINGLE_VIEW_DETAIL_BEGIN_IMPL_HPP
#define BOOST_FUSION_VIEW_SINGLE_VIEW_DETAIL_BEGIN_IMPL_HPP

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct begin_impl;

    template <>
    struct begin_impl<single_view_tag>
    {
        template <typename Seq>
        struct apply
        {
            typedef
                single_view_iterator<typename detail::add_lref<Seq>::type>
            type;

            static type
            call(Seq s)
            {
                return type(s,0);
            }
        };
    };
}}}

#endif
