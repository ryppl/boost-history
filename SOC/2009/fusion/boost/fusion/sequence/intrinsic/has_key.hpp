/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_INTRINSIC_HAS_KEY_HPP
#define BOOST_FUSION_SEQUENCE_INTRINSIC_HAS_KEY_HPP

#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/support/tag_of.hpp>

#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion
{
    struct void_;

    // Special tags:
    struct sequence_facade_tag;

    namespace extension
    {
        template <typename Tag>
        struct has_key_impl
        {
            template <typename Sequence, typename Key>
            struct apply
                : mpl::not_<is_same<
                    typename detail::remove_reference<Sequence>::type::
                        template meta_at_impl<Key>::type
                  , void_> >
            {};
        };

        template <>
        struct has_key_impl<sequence_facade_tag>
        {
            template <typename Sequence, typename Key>
            struct apply
                : detail::remove_reference<Sequence>::type::
                    template has_key<Sequence, Key>
            {};
        };
    }

    namespace result_of
    {
        template <typename Sequence, typename Key>
        struct has_key
            : extension::has_key_impl<typename traits::tag_of<Sequence>::type>::
                template apply<typename detail::add_lref<Sequence>::type, Key>
        {};
    }

    template <typename Key, typename Sequence>
    inline typename result_of::has_key<Sequence, Key>::type
    has_key(Sequence const& seq)
    {
        typedef typename result_of::has_key<Sequence const&, Key>::type result;
        return result();
    }
}}

#endif
