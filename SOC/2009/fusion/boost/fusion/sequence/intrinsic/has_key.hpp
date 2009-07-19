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
    struct sequence_facade_tag;

    namespace extension
    {
        template <typename Tag>
        struct has_key_impl
        {
            template <typename SeqRef, typename Key>
            struct apply
              : mpl::not_<is_same<
                    typename detail::remove_reference<SeqRef>::type::
                        template meta_at_impl<Key>::type
                  , void_>
                >
            {};
        };

        template <>
        struct has_key_impl<sequence_facade_tag>
        {
            template <typename SeqRef, typename Key>
            struct apply
              : detail::remove_reference<SeqRef>::type::
                    template has_key<SeqRef, Key>
            {};
        };
    }

    namespace result_of
    {
        template <typename Seq, typename Key>
        struct has_key
            : extension::has_key_impl<typename traits::tag_of<Seq>::type>::
                template apply<typename detail::add_lref<Seq>::type, Key>
        {};
    }

    template <typename Key, typename Seq>
    inline typename result_of::has_key<Seq const&, Key>::type
    has_key(Seq const& seq)
    {
        typedef typename result_of::has_key<Seq const&, Key>::type result;
        return result();
    }
}}

#endif
