/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_ARRAY_VALUE_AT_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_ARRAY_VALUE_AT_IMPL_HPP

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct value_at_impl;

    template <>
    struct value_at_impl<array_tag>
    {
        template <typename Seq, typename N>
        struct apply
        {
            typedef typename
                detail::remove_reference<Seq>::type::value_type
            type;
        };
    };
}}}

#endif
