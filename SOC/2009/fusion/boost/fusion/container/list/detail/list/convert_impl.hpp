// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_CONTAINER_LIST_DETAIL_LIST_CONVERT_IMPL_HPP
#define BOOST_FUSION_CONTAINER_LIST_DETAIL_LIST_CONVERT_IMPL_HPP

#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   include <boost/fusion/container/list/detail/list/pp/as_list.hpp>
#   include <boost/fusion/sequence/intrinsic/begin.hpp>
#   include <boost/fusion/sequence/intrinsic/size.hpp>
#else
#   include <boost/fusion/container/list/detail/list/variadic_templates/as_list_impl.hpp>
#endif

namespace boost { namespace fusion { namespace extension
{
    template <typename T>
    struct convert_impl;

    template <>
    struct convert_impl<list_tag>
    {
        template <typename SeqRef>
        struct apply
        {
#ifdef BOOST_NO_VARIADIC_TEMPLATES
            typedef typename
                detail::as_list<result_of::size<Seq>::value>
            gen;
            typedef typename gen::template apply<
                typename result_of::begin<Seq>::type>::type
            type;

            static type call(SeqRef seq)
            {
                return gen::call(
                    fusion::begin(BOOST_FUSION_FORWARD(SeqRef,seq)));
            }
#else
            typedef typename detail::as_list_impl<SeqRef> gen;
            typedef typename gen::apply::type type;

            static type call(SeqRef seq)
            {
                return gen::call(seq);
            }
#endif
        };
    };
}}}

#endif
