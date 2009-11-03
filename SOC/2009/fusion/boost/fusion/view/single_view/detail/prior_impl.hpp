/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_SINGLE_VIEW_DETAIL_PRIOR_IMPL_HPP
#define BOOST_FUSION_VIEW_SINGLE_VIEW_DETAIL_PRIOR_IMPL_HPP

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct prior_impl;

    template <>
    struct prior_impl<single_view_iterator_tag>
    {
        template <typename It>
        struct apply
        {
            typedef typename detail::remove_reference<It>::type it;

            typedef
                single_view_iterator<
                    typename it::value_type
                  , typename it::value_ref_type
                  , false
                >
            type;

            static type
            call(It it)
            {
                return type(*it.val);
            }
        };
    };
}}}

#endif
