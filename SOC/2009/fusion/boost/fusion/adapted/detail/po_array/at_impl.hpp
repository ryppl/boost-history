/*=============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_PO_ARRAY_AT_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_PO_ARRAY_AT_IMPL_HPP

#include <boost/type_traits/remove_extent.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct at_impl;

    template<>
    struct at_impl<po_array_tag>
    {
        template<typename Seq, typename N>
        struct apply
        {
            typedef typename
                detail::add_lref<
                    typename remove_extent<
                        typename detail::remove_reference<Seq>::type
                    >::type
                >::type
            type;

            static type
            call(Seq seq)
            {
                return seq[N::value];
            }
        };
    };
}}}

#endif
