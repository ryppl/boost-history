/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_MPL_POP_FRONT_HPP
#define BOOST_FUSION_MPL_POP_FRONT_HPP

#include <boost/fusion/algorithm/transformation/pop_front.hpp>

namespace boost { namespace mpl
{
    template <typename>
    struct pop_front_impl;

    template <>
    struct pop_front_impl<fusion::fusion_sequence_tag>
    {
        template <typename Seq>
        struct apply
          : fusion::result_of::pop_front<Seq>
        {};
    };
}}

#endif
