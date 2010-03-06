/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef EXTENSION_STRUCT_DETAIL_PRIOR_IMPL_HPP
#define EXTENSION_STRUCT_DETAIL_PRIOR_IMPL_HPP

#include "advance_impl.hpp"
#include <boost/mpl/int.hpp>

namespace boost { namespace fusion {

    namespace extension
    {
        template<typename Tag>
        struct prior_impl;

        template<>
        struct prior_impl<example::example_struct_iterator_tag>
        {
            template<typename ItRef>
            struct apply
              : advance_impl<example::example_struct_iterator_tag>::
                    apply<ItRef, mpl::int_<-1> >
            {};
        };
    }
}}

#endif
