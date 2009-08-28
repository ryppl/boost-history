/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_DETAIL_CONS_CONVERT_IMPL_HPP
#define BOOST_FUSION_CONTAINER_LIST_DETAIL_CONS_CONVERT_IMPL_HPP

#include <boost/fusion/container/list/detail/cons/build_cons.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename T>
    struct convert_impl;

    template <>
    struct convert_impl<cons_tag>
    {
        template <typename Seq>
        struct apply
        {
            typedef
                detail::build_cons<
                    typename result_of::begin<Seq>::type
                  , typename result_of::end<Seq>::type
                >
            build_cons;

            typedef typename build_cons::type type;

            static type
            call(Seq seq)
            {
                return build_cons::call(
                        fusion::begin(seq),
                        fusion::end(seq));
            }
        };
    };
}}}

#endif
