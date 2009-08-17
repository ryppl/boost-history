/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_DETAIL_CONS_EQUAL_TO_IMPL_HPP
#define BOOST_FUSION_CONTAINER_LIST_DETAIL_CONS_EQUAL_TO_IMPL_HPP

namespace boost { namespace fusion { namespace extension
{
    template <typename Tag>
    struct equal_to_impl;

    template <>
    struct equal_to_impl<cons_iterator_tag>
    {
        template <typename It1Ref, typename It2Ref>
        struct apply
          : is_same<
                typename detail::identity<
                    typename detail::remove_reference<
                        It1Ref
                    >::type::cons_type
                >::type
              , typename detail::identity<
                    typename detail::remove_reference<
                        It2Ref
                    >::type::cons_type
                >::type
            >
        {
        };
    };
}}}

#endif
