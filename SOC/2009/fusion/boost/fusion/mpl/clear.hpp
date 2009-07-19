/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_MPL_CLEAR_HPP
#define BOOST_FUSION_MPL_CLEAR_HPP

#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/algorithm/transformation/clear.hpp>

namespace boost { namespace mpl
{
    template <typename Tag>
    struct clear_impl;

    template <>
    struct clear_impl<fusion::fusion_sequence_tag>
    {
        template <typename Seq>
        struct apply
          : fusion::result_of::clear<Seq>
        {};
    };
}}

#endif
