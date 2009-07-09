/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_ARRAY_BEGIN_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_ARRAY_BEGIN_IMPL_HPP

namespace boost { namespace fusion {

    struct array_tag;

    namespace extension
    {
        template<typename Tag>
        struct begin_impl;

        template <>
        struct begin_impl<array_tag>
        {
            template <typename SeqRef>
            struct apply 
            {
                typedef array_iterator<SeqRef, 0> type;
    
                static type
                call(SeqRef seq)
                {
                    return type(seq,0);
                }
            };
        };
    }
}}

#endif
