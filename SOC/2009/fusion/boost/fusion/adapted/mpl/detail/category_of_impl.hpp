/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_MPL_DETAIL_CATEGORY_OF_IMPL_HPP
#define BOOST_FUSION_ADAPTED_MPL_DETAIL_CATEGORY_OF_IMPL_HPP

#include <boost/fusion/support/detail/mpl_iterator_category.hpp>

#include <boost/mpl/begin.hpp>
#include <boost/mpl/is_sequence.hpp>

namespace boost { namespace fusion {

    struct mpl_sequence_tag;

    namespace extension
    {
        template<typename Tag>
        struct category_of_impl;

        template<>
        struct category_of_impl<mpl_sequence_tag>
        {
            template<typename SeqRef>
            struct apply
            {
                //TODO get_category?
                typedef typename
                    detail::mpl_iterator_category<
                        typename mpl::begin<
                            typename detail::identity<SeqRef>::type
                        >::type::category
                    >::type
                type;
            };
        };
    }
}}

#endif
