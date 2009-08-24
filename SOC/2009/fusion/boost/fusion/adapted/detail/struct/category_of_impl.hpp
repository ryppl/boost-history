/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_STRUCT_CATEGORY_OF_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_STRUCT_CATEGORY_OF_IMPL_HPP

#include <boost/fusion/support/category_of.hpp>

namespace boost { namespace fusion
{
    namespace extension
    {
        template<typename Tag>
        struct category_of_impl;

        template<>
        struct category_of_impl<struct_tag>
        {
            template<typename SeqRef>
            struct apply
            {
                typedef random_access_traversal_tag type;
            };
        };

        template<>
        struct category_of_impl<assoc_struct_tag>
        {
            template<typename SeqRef>
            struct apply
            {
                struct type
                  : random_access_traversal_tag, associative_sequence_tag
                {};
            };
        };
    }
}}

#endif
