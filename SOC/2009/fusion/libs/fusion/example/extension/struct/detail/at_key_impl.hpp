/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef EXTENSION_STRUCT_DETAIL_AT_KEY_IMPL_HPP
#define EXTENSION_STRUCT_DETAIL_AT_KEY_IMPL_HPP

#include "at_impl.hpp"
#include <boost/mpl/int.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename Tag>
    struct at_key_impl;

    template<>
    struct at_key_impl<example::example_sequence_tag>
    {
        template<typename SeqRef, typename Key>
        struct apply;

        template<typename SeqRef>
        struct apply<SeqRef, fields::name>
          : at_impl<example::example_sequence_tag>::apply<SeqRef, mpl::int_<0> >
        {};

        template<typename SeqRef>
        struct apply<SeqRef, fields::age>
          : at_impl<example::example_sequence_tag>::apply<SeqRef, mpl::int_<1> >
        {};
    };
}}}

#endif
