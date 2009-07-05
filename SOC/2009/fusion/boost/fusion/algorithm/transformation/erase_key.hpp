/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_ERASE_KEY_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_ERASE_KEY_HPP

#include <boost/fusion/algorithm/transformation/erase.hpp>
#include <boost/fusion/algorithm/query/detail/assoc_find.hpp>
#include <boost/fusion/support/ref.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq, typename Key>
        struct erase_key
        {
            typedef
                detail::assoc_find<
                    typename detail::add_lref<Seq>::type
                  , Key
                >
            gen;

            typedef typename
                erase<
                    Seq
                  , typename gen::type
                >::type
            type;
        };
    }

    template <typename Key, typename Seq>
    inline typename
        result_of::erase_key<BOOST_FUSION_R_ELSE_LREF(Seq), Key>::type
    erase_key(BOOST_FUSION_R_ELSE_LREF(Seq) seq)
    {
        typedef result_of::erase_key<BOOST_FUSION_R_ELSE_LREF(Seq), Key> result;
        return erase(BOOST_FUSION_FORWARD(Seq,seq), result::gen::call(seq));
    }
}}

#endif
