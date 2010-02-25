/*=============================================================================
    Copyright (c) 2009 Hartmut Kaiser
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_NVIEW_DETAIL_BEGIN_IMPL_HPP
#define BOOST_FUSION_VIEW_NVIEW_DETAIL_BEGIN_IMPL_HPP

#include <boost/mpl/begin.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct begin_impl;

    template<>
    struct begin_impl<nview_tag>
    {
        template<typename Seq>
        struct apply
        {
            typedef typename detail::remove_reference<Seq>::type seq;

            typedef
                nview_iterator<
                    typename detail::add_lref<
                        typename seq::storage_type::type
                    >::type
                  , typename mpl::begin<typename seq::indices>::type
                >
            type;

            static type
            call(Seq seq)
            {
                return type(seq.seq.get(),0);
            }
        };
    };
}}}

#endif

