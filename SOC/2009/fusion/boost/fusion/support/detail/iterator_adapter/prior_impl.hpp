// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_SUPPORT_ITERATOR_ADAPTER_PRIOR_IMPL_HPP
#define BOOST_FUSION_SUPPORT_ITERATOR_ADAPTER_PRIOR_IMPL_HPP

#include <boost/fusion/iterator/prior.hpp>

namespace boost { namespace fusion
{
    template<typename It, typename NewCategory>
    struct iterator_adapter;

    struct iterator_adapter_tag;

    namespace extension
    {
        template <typename Tag>
        struct prior_impl;

        template <>
        struct prior_impl<iterator_adapter_tag>
        {
            template <typename ItRef>
            struct apply
            {
                typedef typename
                    detail::remove_reference<ItRef>::type
                it;

                typedef
                    iterator_adapter<
                        typename result_of::prior<
                            typename it::iterator_type
                        >::type
                      , typename it::category
                    >
                type;

                static type
                call(ItRef it)
                {
                    return type(fusion::prior(it.it),0);
                }
            };
        };
    }
}}

#endif
