// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_SUPPORT_ITERATOR_ADVANCE_IMPL_HPP
#define BOOST_FUSION_SUPPORT_ITERATOR_ADVANCE_IMPL_HPP

#include <boost/fusion/iterator/advance.hpp>

namespace boost { namespace fusion
{
    template<typename It, typename NewCategory>
    struct iterator_adapter;

    struct iterator_adapter_tag;

    namespace extension
    {
        template <typename Tag>
        struct advance_impl;

        template <>
        struct advance_impl<iterator_adapter_tag>
        {
            template <typename ItRef, typename N>
            struct apply
            {
                //TODO cschmidt: category assert
                typedef typename detail::remove_reference<ItRef>::type it;

                typedef
                    iterator_adapter<
                        typename result_of::advance<
                            typename it::iterator_type
                          , N
                        >::type
                      , typename it::category
                    >
                type;

                static type
                call(ItRef it)
                {
                    return type(fusion::advance<N>(it.it),0);
                }
            };
        };
    }
}}

#endif
