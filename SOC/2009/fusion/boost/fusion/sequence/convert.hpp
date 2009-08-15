/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_CONVERT_HPP
#define BOOST_FUSION_SEQUENCE_CONVERT_HPP

#include <boost/config.hpp>
#include <boost/fusion/container/vector/convert.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>

#include <boost/mpl/always.hpp>
#include <boost/mpl/apply.hpp>

//TODO in /sequence/intrisic/ or /container/?
//TODO doc!!!
namespace boost { namespace fusion
{
    struct void_;

    namespace extension
    {
        template <typename Tag>
        struct convert_impl
        {
            template<typename SeqRef>
            struct apply
            {
                typedef typename result_of::as_vector<SeqRef>::type type;

                static type
                call(SeqRef seq)
                {
                    return fusion::as_vector(seq);
                }
            };
        };
    }

    namespace result_of
    {
        template <typename Tag, typename Seq>
        struct convert
        {
            //BOOST_FUSION_MPL_ASSERT((traits_is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));

            typedef typename extension::convert_impl<Tag> gen;

            typedef typename
                gen::template apply<typename detail::add_lref<Seq>::type>::type
            type;
        };
    }

    template <typename Tag, typename Seq>
    inline typename
        result_of::convert<Tag, BOOST_FUSION_R_ELSE_CLREF(Seq)>::type
    convert(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return result_of::convert<Tag, BOOST_FUSION_R_ELSE_CLREF(Seq)>::
            gen::call(seq);
    }
}}

#endif
