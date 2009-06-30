/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_MPL_DETAIL_VALUE_OF_IMPL_HPP
#define BOOST_FUSION_ADAPTED_MPL_DETAIL_VALUE_OF_IMPL_HPP

#include <boost/mpl/deref.hpp>

namespace boost { namespace fusion
{
    struct mpl_iterator_tag;

    namespace extension
    {
        template <typename Tag>
        struct value_of_impl;

        template <>
        struct value_of_impl<mpl_iterator_tag>
        {
            template <typename ItRef>
            struct apply
              : mpl::deref<typename detail::identity<ItRef>::type>
            {};
        };
    }
}}

#endif
