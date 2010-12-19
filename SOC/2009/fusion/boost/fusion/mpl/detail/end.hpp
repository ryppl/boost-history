/*==============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_MPL_DETAIL_END_HPP
#define BOOST_FUSION_MPL_DETAIL_END_HPP

#include <boost/fusion/container/vector/convert.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/mpl/detail/fusion_iterator.hpp>

namespace boost { namespace mpl
{
    template<typename>
    struct end_impl;

    template<>
    struct end_impl<fusion::fusion_sequence_tag>
    {
        template<typename Seq>
        struct apply
        {
            typedef
                fusion_iterator<
                    typename fusion::result_of::end<Seq>::type
                >
            type;
        };
    };
}}

#endif
