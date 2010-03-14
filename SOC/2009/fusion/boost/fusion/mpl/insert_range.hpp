/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_MPL_INSERT_RANGE_HPP
#define BOOST_FUSION_MPL_INSERT_RANGE_HPP

#include <boost/fusion/algorithm/transformation/insert_range.hpp>

namespace boost { namespace mpl
{
    template <typename>
    struct insert_range_impl;

    template <>
    struct insert_range_impl<fusion::fusion_sequence_tag>
    {
        template <typename Seq, typename Pos, typename Range>
        struct apply
          : fusion::result_of::insert_range<Seq, Pos, Range>
        {};
    };
}}

#endif
