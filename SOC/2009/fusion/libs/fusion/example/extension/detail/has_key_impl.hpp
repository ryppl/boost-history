/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef 
#define 

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/or.hpp>

namespace fields
{
    struct name;
    struct age;
}

namespace example
{
    struct example_sequence_tag;
}

namespace boost { namespace fusion {

    namespace extension
    {
        template<typename>
        struct has_key_impl;

        template<>
        struct has_key_impl<example::example_sequence_tag>
        {
            template<typename Sequence, typename Key>
            struct apply
                : mpl::or_<
                is_same<Key, fields::name>,
                is_same<Key, fields::age> >
            {};
        };
    }
}}

#endif
