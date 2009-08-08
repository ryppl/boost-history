/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_VECTOR_DETAIL_END_IMPL_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_DETAIL_END_IMPL_HPP

namespace boost { namespace fusion { namespace extension
{
    template <typename Tag>
    struct end_impl;

    template <>
    struct end_impl<vector_tag>
    {
        template <typename SeqRef>
        struct apply
        {
            typedef
                basic_iterator<
                    vector_iterator_tag
                  , random_access_traversal_tag
                  , SeqRef
                  , detail::remove_reference<SeqRef>::type::size::value
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
