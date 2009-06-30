/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_ARRAY_DETAIL_BEGIN_IMPL_HPP
#define BOOST_FUSION_ADAPTED_ARRAY_DETAIL_BEGIN_IMPL_HPP

#include <boost/fusion/adapted/array/array_iterator.hpp>

namespace boost { namespace fusion {

    struct array_tag;

    namespace extension
    {
        template<typename T>
        struct begin_impl;

        template <>
        struct begin_impl<array_tag>
        {
            template <typename Sequence>
            struct apply 
            {
                typedef array_iterator<Sequence, 0> type;
    
                static type
                call(Sequence& v)
                {
                    return type(v);
                }
            };
        };
    }
}}

#endif
