/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_FILTER_VIEW_DETAIL_DEREF_IMPL_HPP
#define BOOST_FUSION_VIEW_FILTER_VIEW_DETAIL_DEREF_IMPL_HPP

#include <boost/fusion/iterator/deref.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct deref_impl;

    template <>
    struct deref_impl<filter_view_iterator_tag>
    {
        template<typename It>
        struct apply
        {
            typedef typename
                result_of::deref<
                    typename detail::remove_reference<
                        It
                    >::type::begin_type
                >::type
            type;

            static type call(It it)
            {
                return fusion::deref(it.first);
            }
        };
    };
}}}

#endif
