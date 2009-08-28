/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_REVERSE_VIEW_DETAIL_ADVANCE_IMPL_HPP
#define BOOST_FUSION_VIEW_REVERSE_VIEW_DETAIL_ADVANCE_IMPL_HPP

#include <boost/fusion/iterator/advance.hpp>
#include <boost/mpl/negate.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct advance_impl;

    template<>
    struct advance_impl<reverse_view_iterator_tag>
    {
        template<typename It, typename N>
        struct apply
        {
            typedef typename mpl::negate<N>::type neg_dist;

            typedef
                reverse_view_iterator<
                    typename result_of::advance<
                        typename detail::remove_reference<
                            It
                        >::type::it_type
                      , neg_dist
                    >::type
                >
            type;

            static type
            call(It it)
            {
                return type(fusion::advance<neg_dist>(it.it));
            }
        };
    };
}}}

#endif
