/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_SET_CONVERT_HPP
#define BOOST_FUSION_CONTAINER_SET_CONVERT_HPP

#include <boost/fusion/container/set/set.hpp>
#include <boost/fusion/container/set/detail/convert_impl.hpp>
#include <boost/fusion/support/ref.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Sequence>
        struct as_set
        {
            typedef typename
                extension::convert_impl<set_tag>::
                    template apply<typename detail::add_lref<Sequence>::type>
            gen;

            typedef typename gen::apply::type type;
        };
    }

    template <typename Sequence>
    inline typename result_of::as_set<BOOST_FUSION_R_ELSE_CLREF(Sequence)>::type
    as_set(BOOST_FUSION_R_ELSE_CLREF(Sequence) seq)
    {
        typedef typename
            result_of::as_set<BOOST_FUSION_R_ELSE_CLREF(Sequence)>::gen
        gen;

        return gen::call(BOOST_FUSION_FORWARD(Sequence,seq));
    }

}}

#endif
