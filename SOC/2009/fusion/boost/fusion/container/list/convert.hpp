/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_CONVERT_HPP
#define BOOST_FUSION_CONTAINER_LIST_CONVERT_HPP

#include <boost/fusion/container/list/list.hpp>
#include <boost/fusion/container/list/detail/list/convert_impl.hpp>
#include <boost/fusion/support/ref.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Sequence>
        struct as_list
        {
            typedef typename
                extension::convert_impl<list_tag>::
                    template apply<typename detail::add_lref<Sequence>::type>
            gen;

            typedef typename gen::type type;
        };
    }

    template <typename Sequence>
    inline typename
        result_of::as_list<BOOST_FUSION_R_ELSE_CLREF(Sequence)>::type
    as_list(BOOST_FUSION_R_ELSE_CLREF(Sequence) seq)
    {
        typedef typename
            result_of::as_list<BOOST_FUSION_R_ELSE_CLREF(Sequence)>::gen
        gen;
        return gen::call(seq);
    }

}}


#endif
