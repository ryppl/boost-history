/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef EXTENSION_STRUCT_DETAIL_TAG_OF_HPP
#define EXTENSION_STRUCT_DETAIL_TAG_OF_HPP

#include <boost/config.hpp>
#include <boost/fusion/support/tag_of_fwd.hpp>
#include <string>

namespace fields
{
    struct name;
    struct age;
}

namespace example
{
    struct example_struct;

    template<typename Struct, int Pos>
    struct example_struct_iterator;

    struct example_sequence_tag;
    struct example_struct_iterator_tag;
}

namespace boost { namespace fusion { namespace traits
{
    template<>
    struct tag_of<example::example_struct>
    {
        typedef example::example_sequence_tag type;
    };

    template<>
    struct tag_of<example::example_struct const>
    {
        typedef example::example_sequence_tag type;
    };

    template<>
    struct tag_of<example::example_struct&>
    {
        typedef example::example_sequence_tag type;
    };

    template<>
    struct tag_of<example::example_struct const&>
    {
        typedef example::example_sequence_tag type;
    };

#ifndef BOOST_NO_RVALUE_REFERENCES
    template<>
    struct tag_of<example::example_struct&&>
    {
        typedef example::example_sequence_tag type;
    };

    template<>
    struct tag_of<example::example_struct const&&>
    {
        typedef example::example_sequence_tag type;
    };
#endif
}}}

#endif
