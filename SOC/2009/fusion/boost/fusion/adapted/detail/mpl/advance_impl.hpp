/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_MPL_ADVANCE_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_MPL_ADVANCE_IMPL_HPP

#include <boost/mpl/advance.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename Tag>
    struct advance_impl;

    template <>
    struct advance_impl<mpl_iterator_tag>
    {
        template <typename ItRef, typename N>
        struct apply
        {
            typedef
                mpl_iterator<
                    typename mpl::advance<
                        typename detail::identity<ItRef>::type
                      , N
                    >::type
                >
            type;

            template<typename ItRef2>
            static type
            call(ItRef2)
            {
                return type();
            }
        };
    };
}}}

#endif
