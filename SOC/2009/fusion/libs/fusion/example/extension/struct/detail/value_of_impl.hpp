/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef EXTENSION_STRUCT_DETAIL_VALUE_OF_IMPL_HPP
#define EXTENSION_STRUCT_DETAIL_VALUE_OF_IMPL_HPP

#include <boost/mpl/if.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename Tag>
    struct value_of_impl;

    template<>
    struct value_of_impl<example::example_struct_iterator_tag>
    {
        template<typename ItRef>
        struct apply
          : mpl::if_c<
                !detail::remove_reference<ItRef>::type::index::value
              , std::string
              , int
            >
        {};
    };
}}}

#endif
