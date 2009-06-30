/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_INTRINSIC_VALUE_AT_KEY_HPP
#define BOOST_FUSION_SEQUENCE_INTRINSIC_VALUE_AT_KEY_HPP

#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/support/ref.hpp>

#include <boost/mpl/int.hpp>

namespace boost { namespace fusion
{
    // Special tags:
    struct sequence_facade_tag;

    namespace extension
    {
        template <typename Tag>
        struct value_at_key_impl
        {
            template <typename Sequence, typename Key>
            struct apply;
        };

        template <>
        struct value_at_key_impl<sequence_facade_tag>
        {
            template <typename Sequence, typename Key>
            struct apply :
                detail::remove_reference<Sequence>::type::
                    template value_at_key<Sequence, Key>
            {};
        };
    }

    namespace result_of
    {
        template <typename Sequence, typename N>
        struct value_at_key
            : extension::value_at_key_impl<
                    typename traits::tag_of<Sequence>::type>::
                template apply<typename detail::add_lref<Sequence>::type, N>
        {};
    }
}}

#endif
