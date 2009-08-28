/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_DETAIL_LIST_END_IMPL_HPP
#define BOOST_FUSION_CONTAINER_LIST_DETAIL_LIST_END_IMPL_HPP

#include <boost/fusion/iterator/basic_iterator.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct end_impl;

    template <>
    struct end_impl<list_tag>
    {
        template <typename Seq>
        struct apply
        {
            typedef
                basic_iterator<
                    list_iterator_tag
                  , bidirectional_traversal_tag
                  , typename detail::add_lref<Seq>::type
                  , result_of::size<Seq>::type::value
                >
            type;

            static type
            call(Seq seq)
            {
                return type(seq,0);
            }
        };
    };
}}}

#endif
