/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_BOOST_TUPLE_BEGIN_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_BOOST_TUPLE_BEGIN_IMPL_HPP

#include <boost/fusion/iterator/basic_iterator.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct begin_impl;

    template <>
    struct begin_impl<boost_tuple_tag>
    {
        template <typename Seq>
        struct apply
        {
            typedef
                basic_iterator<
                    boost_tuple_iterator_tag
                  , random_access_traversal_tag
                  , typename detail::add_lref<Seq>::type
                  , 0
                >
            type;

            static type
            call(Seq seq)
            {
                return type(seq,0);
            }
        };
    };
}}}

#endif
