/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_MAP_CONVERT_HPP
#define BOOST_FUSION_CONTAINER_MAP_CONVERT_HPP

#include <boost/fusion/container/map/map.hpp>
#include <boost/fusion/support/ref.hpp>

#include <boost/fusion/container/map/detail/convert_impl.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq>
        struct as_map
        {
            typedef typename
                extension::convert_impl<map_tag>::
                    template apply<typename detail::add_lref<Seq>::type>
            gen;

            typedef typename gen::apply::type type;
        };
    }

    template <typename Seq>
    inline typename result_of::as_map<BOOST_FUSION_R_ELSE_CLREF(Seq)>::type
    as_map(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        typedef typename
            result_of::as_map<BOOST_FUSION_R_ELSE_CLREF(Seq)>::gen
        gen;

        return gen::call(seq);
    }

}}

#endif
