/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_INTRINSIC_AT_KEY_HPP
#define BOOST_FUSION_SEQUENCE_INTRINSIC_AT_KEY_HPP

#include <boost/fusion/iterator/deref_data.hpp>
#include <boost/fusion/algorithm/query/find_key.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/support/internal/assert.hpp>

namespace boost { namespace fusion
{
    struct sequence_facade_tag;

    namespace extension
    {
        template <typename>
        struct at_key_impl
        {
            template <typename Seq, typename Key>
            struct apply
            {
                typedef typename
                    result_of::deref_data<
                        typename result_of::find_key<Seq, Key>::type
                    >::type
                type;

                static type
                call(Seq seq)
                {
                    return fusion::deref_data(fusion::find_key<Key>(
                        BOOST_FUSION_FORWARD(Seq,seq)));
                }
            };
        };

        template <>
        struct at_key_impl<sequence_facade_tag>
        {
            template <typename Seq, typename Key>
            struct apply
              : detail::remove_reference<Seq>::type::
                    template at_key<Seq, Key>
            {};
        };
    }

    namespace result_of
    {
        template <typename Seq, typename Key>
        struct at_key
            : extension::at_key_impl<typename traits::tag_of<Seq>::type>::
                template apply<Seq, Key>
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_associative<Seq>));
            BOOST_FUSION_MPL_ASSERT((has_key<Seq,Key>));
        };
    }

    template <typename Key, typename Seq>
    inline typename
        result_of::at_key<
            BOOST_FUSION_R_ELSE_CLREF(Seq)
          , Key
        >::type
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
