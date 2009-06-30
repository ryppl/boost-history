/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_ZIP_VIEW_DETAIL_ADVANCE_IMPL_HPP
#define BOOST_FUSION_VIEW_ZIP_VIEW_DETAIL_ADVANCE_IMPL_HPP

#include <boost/fusion/view/zip_view/zip_view_iterator_fwd.hpp>
#include <boost/fusion/iterator/advance.hpp>
#include <boost/fusion/algorithm/transformation/transform.hpp>

namespace boost { namespace fusion {

    struct zip_view_iterator_tag;

    namespace detail
    {
        template<typename N>
        struct poly_advance
        {
            template<typename Sig>
            struct result;

            template<typename N1, typename It>
            struct result<poly_advance<N1>(It)>
            {
                typedef typename result_of::advance<It,N>::type type;
            };

            template<typename It>
            typename result_of::advance<It const&,N>::type
            operator()(It const& it) const
            {
                return fusion::advance<N>(it);
            }
        };
    }

    namespace extension
    {
        template<typename Tag>
        struct advance_impl;

        template<>
        struct advance_impl<zip_view_iterator_tag>
        {
            template<typename ItRef, typename N>
            struct apply
            {
                typedef
                    zip_view_iterator<
                        typename result_of::transform<
                            typename detail::remove_reference<
                                ItRef
                            >::type::iterators
                          , detail::poly_advance<N>
                        >::type
                    >
                type;

                static type
                call(ItRef it)
                {
                    return type(fusion::transform(
                            it.iterators_, detail::poly_advance<N>()));
                }
            };
        };
    }
}}

#endif
