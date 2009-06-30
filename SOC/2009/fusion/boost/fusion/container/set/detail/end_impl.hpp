// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_CONTAINER_SET_END_IMPL_HPP
#define BOOST_FUSION_CONTAINER_SET_END_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/end.hpp>

namespace boost { namespace fusion
{
    struct set_tag;

    namespace extension
    {
        template <typename Tag>
        struct end_impl;

        template <>
        struct end_impl<set_tag>
        {
            template <typename Sequence>
            struct apply
            {
                typedef typename
                    detail::result_of_forward_as<Sequence
                      , typename detail::remove_reference<Sequence>::type::storage_type
                    >::type
                storage_type;

                typedef typename
                    result_of::end<storage_type>::type
                type;

                static type
                call(Sequence s)
                {
                    return fusion::end(
                            static_cast<storage_type>(s.get_data()));
                }
            };
        };
    }
}}

#endif
