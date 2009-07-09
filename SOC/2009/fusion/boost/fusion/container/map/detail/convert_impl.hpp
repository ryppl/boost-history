// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_CONTAINER_MAP_DETAIL_CONVERT_IMPL_HPP
#define BOOST_FUSION_CONTAINER_MAP_DETAIL_CONVERT_IMPL_HPP

#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   include <boost/fusion/sequence/intrinsic/begin.hpp>
#   include <boost/fusion/sequence/intrinsic/size.hpp>
#   include <boost/fusion/container/map/detail/pp/as_set.hpp>
#else
#   include <boost/fusion/container/map/detail/variadic_templates/as_map_impl.hpp>
#endif

namespace boost { namespace fusion
{
    struct map_tag;

    namespace extension
    {
        template <typename T>
        struct convert_impl;

        template <>
        struct convert_impl<map_tag>
        {
            template <typename SeqRef>
            struct apply
            {
#ifdef BOOST_NO_VARIADIC_TEMPLATES
                typedef typename
                    detail::as_map<result_of::size<SeqRef>::value>
                gen;
                typedef typename
                    gen::template apply<
                        typename result_of::begin<SeqRef>::type
                    >::type
                type;

                static type call(SeqRef seq)
                {
                    return gen::call(
                            fusion::begin(BOOST_FUSION_FORWARD(Sequence,seq)));
                }
#else
                typedef typename detail::as_map_impl<SeqRef> gen;
                typedef typename gen::apply::type type;

                static type call(SeqRef seq)
                {
                    return gen::call(seq);
                }
#endif
            };
        };
    }
}}

#endif
