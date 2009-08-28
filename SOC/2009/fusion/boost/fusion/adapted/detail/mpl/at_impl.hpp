/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_MPL_AT_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_MPL_AT_IMPL_HPP

#include <boost/mpl/at.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct at_impl;

    template <>
    struct at_impl<mpl_sequence_tag>
    {
        template <typename Seq, typename N>
        struct apply
        {
            typedef typename
                mpl::at<typename detail::identity<Seq>::type, N>::type
            type;

            static type
            call(Seq)
            {
                return type();
            }
        };
    };
}}}

#endif
