/*=============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef EXTENSION_STRUCT_DETAIL_KEY_OF_IMPL_HPP
#define EXTENSION_STRUCT_DETAIL_KEY_OF_IMPL_HPP

#include <boost/mpl/if.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename Tag>
    struct key_of_impl;

    template<>
    struct key_of_impl<example::example_struct_iterator_tag>
    {
        template<typename ItRef>
        struct apply
          : mpl::if_c<
                !detail::remove_reference<ItRef>::type::index::value
              , fields::name
              , fields::age
            >
        {};
    };
}}}

#endif
