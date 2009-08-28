/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef 
#define 

#include <boost/mpl/equal_to.hpp>

namespace example
{
    struct example_struct_iterator_tag;
}

namespace boost { namespace fusion {

    namespace extension
    {
        template<typename>
        struct equal_to_impl;

        template<>
        struct equal_to_impl<example::example_struct_iterator_tag>
        {
            template<typename It1, typename It2>
            struct apply
                : mpl::equal_to<
                typename It1::index,
                typename It2::index>
            {};
        };
    }
}}

#endif
