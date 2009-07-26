/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_INTRINSIC_AT_KEY_HPP
#define BOOST_FUSION_SEQUENCE_INTRINSIC_AT_KEY_HPP

#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/support/tag_of.hpp>

#include <boost/type_traits/is_const.hpp>
namespace boost { namespace fusion
{
    struct sequence_facade_tag;

    namespace extension
    {
        template <typename Tag>
        struct at_key_impl;

        template <>
        struct at_key_impl<sequence_facade_tag>
        {
            template <typename SeqRef, typename Key>
            struct apply
              : detail::remove_reference<SeqRef>::type::
                    template at_key<SeqRef, Key>
            {};
        };
    }

    namespace result_of
    {
        template <typename Seq, typename Key>
        struct at_key
            : extension::at_key_impl<typename traits::tag_of<Seq>::type>::
                template apply<typename detail::add_lref<Seq>::type, Key>
        {};
    }

    template <typename Key, typename Seq>
    inline typename result_of::at_key<
        BOOST_FUSION_R_ELSE_CLREF(Seq)
      , Key>::type
    at_key(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return
            result_of::at_key<
                BOOST_FUSION_R_ELSE_CLREF(Seq)
              , Key
            >::call(seq);
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename Key, typename Seq>
    inline typename result_of::at_key<Seq&, Key>::type
    at_key(Seq& seq)
    {
        return result_of::at_key<Seq&, Key>::call(seq);
    }
#endif

}}

#endif
