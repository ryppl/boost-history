/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_BOOST_TUPLE_END_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_BOOST_TUPLE_END_IMPL_HPP

#include <boost/fusion/iterator/basic_iterator.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename Tag>
    struct end_impl;

    template <>
    struct end_impl<boost_tuple_tag>
    {
        template <typename SeqRef>
        struct apply
        {
            typedef
                basic_iterator<
                    boost_tuple_iterator_tag
                  , random_access_traversal_tag
                  , SeqRef
                  , tuples::length<
                        typename detail::identity<SeqRef>::type
                    >::value
                >
            type;

            static type
            call(SeqRef seq)
            {
                return type(seq,0);
            }
        };
    };
}}}

#endif
