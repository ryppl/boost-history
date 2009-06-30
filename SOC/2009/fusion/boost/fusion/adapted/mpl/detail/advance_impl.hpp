/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_MPL_DETAIL_ADVANCE_IMPL_HPP
#define BOOST_FUSION_ADAPTED_MPL_DETAIL_ADVANCE_IMPL_HPP

#include <boost/mpl/advance.hpp>

namespace boost { namespace fusion
{
    struct mpl_iterator_tag;

    namespace extension
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

                static type
                call(ItRef i)
                {
                    return type();
                }
            };
        };
    }
}}

#endif
