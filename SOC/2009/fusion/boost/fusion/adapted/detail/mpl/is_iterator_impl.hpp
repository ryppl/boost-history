/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_MPL_IS_ITERATOR_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_MPL_IS_ITERATOR_IMPL_HPP

#include <boost/mpl/bool.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct is_iterator_impl;

    template<>
    struct is_iterator_impl<mpl_iterator_tag>
    {
        template<typename Seq>
        struct apply
          : mpl::true_
        {};
    };
}}}

#endif
