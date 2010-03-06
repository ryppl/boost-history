/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef EXTENSION_STRUCT_DETAIL_EQUAL_TO_IMPL_HPP
#define EXTENSION_STRUCT_DETAIL_EQUAL_TO_IMPL_HPP

#include <boost/mpl/equal_to.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename Tag>
    struct equal_to_impl;

    template<>
    struct equal_to_impl<example::example_struct_iterator_tag>
    {
        template<typename It1Ref, typename It2Ref>
        struct apply
          : mpl::equal_to<
                typename detail::remove_reference<It1Ref>::type::index
              , typename detail::remove_reference<It2Ref>::type::index
            >
        {};
    };
}}}

#endif
