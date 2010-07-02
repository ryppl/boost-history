/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_TUPLE_CATEGORY_OF_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_TUPLE_CATEGORY_OF_IMPL_HPP

namespace boost { namespace fusion 
{ 
    struct random_access_traversal_tag;

    namespace extension
    {
        template<typename>
        struct category_of_impl;

        template<>
        struct category_of_impl<boost_tuple_tag>
        {
            template<typename Seq>
            struct apply
            {
                typedef random_access_traversal_tag type;
            };
        };

        template<>
        struct category_of_impl<std_tuple_tag>
          : category_of_impl<boost_tuple_tag>
        {};
    }
}}

#endif