/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_MPL_DEREF_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_MPL_DEREF_IMPL_HPP

#include <boost/mpl/deref.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct deref_impl;

    template <>
    struct deref_impl<mpl_iterator_tag>
    {
        template <typename It>
        struct apply
        {
            typedef typename
                mpl::deref<typename detail::identity<It>::type>::type
            type;

            template<typename It2>
            static type
            call(It2)
            {
                return type();
            }
        };
    };
}}}

#endif
