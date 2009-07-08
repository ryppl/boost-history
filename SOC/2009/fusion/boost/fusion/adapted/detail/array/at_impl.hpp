/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_ARRAY_AT_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_ARRAY_AT_IMPL_HPP

namespace boost { namespace fusion {
    
    struct array_tag;

    namespace extension
    {
        template<typename T>
        struct at_impl;

        template<>
        struct at_impl<array_tag>
        {
            template<typename SeqRef, typename N>
            struct apply
            {
                typedef typename
                    detail::result_of_forward_as<
                        SeqRef
                      , typename detail::remove_reference<
                            SeqRef
                        >::type::value_type
                    >::type
                type;

                static type
                call(SeqRef seq)
                {
                    return seq[N::value];
                }
            };
        };
    }
}}

#endif
