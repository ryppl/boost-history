/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_MPL_ERASE_HPP
#define BOOST_FUSION_MPL_ERASE_HPP

#include <boost/fusion/algorithm/transformation/erase.hpp>

namespace boost { namespace mpl
{
    template <typename>
    struct erase_impl;

    template <>
    struct erase_impl<fusion::fusion_sequence_tag>
    {
        template <typename Seq, typename Begin, typename End>
        struct apply
          : fusion::result_of::erase<Seq, Begin, End>
        {};
    };
}}

#endif
