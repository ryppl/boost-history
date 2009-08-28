/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_MPL_PRIOR_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_MPL_PRIOR_IMPL_HPP

#include <boost/mpl/prior.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct prior_impl;

    template <>
    struct prior_impl<mpl_iterator_tag>
    {
        template <typename It>
        struct apply
        {
            typedef
                mpl_iterator<
                    typename mpl::prior<
                        typename detail::identity<It>::type
                    >::type
                >
            type;

            template<typename It2>
            static type
            call(It2)
            {
                return type();
            }
        };
    };
}}}

#endif
