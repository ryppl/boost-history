/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_BOOST_TUPLE_VALUE_AT_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_BOOST_TUPLE_VALUE_AT_IMPL_HPP

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct value_at_impl;

    template <>
    struct value_at_impl<boost_tuple_tag>
    {
        template <typename Seq, typename N>
        struct apply
          : tuples::element<N::value, typename detail::identity<Seq>::type>
        {};
    };
}}}

#endif
