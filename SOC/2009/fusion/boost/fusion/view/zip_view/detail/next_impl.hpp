/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_ZIP_VIEW_DETAIL_NEXT_IMPL_HPP
#define BOOST_FUSION_VIEW_ZIP_VIEW_DETAIL_NEXT_IMPL_HPP

#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/algorithm/transformation/transform.hpp>
#include <boost/fusion/support/unused.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        struct poly_next
        {
            template<typename Sig>
            struct result;

            template<typename Self, typename It>
            struct result<Self(It)>
              : mpl::eval_if<
                    is_same<It, unused_type const&>
                  , mpl::identity<unused_type const&>
                  , result_of::next<It>
                >
            {};

            template<typename It>
            typename result<poly_next(It const&)>::type
            operator()(It const& it) const
            {
                return fusion::next(it);
            }

            unused_type const&
            operator()(unused_type const& unused) const
            {
                return unused;
            }
        };
    }

    namespace extension
    {
        template<typename>
        struct next_impl;

        template<>
        struct next_impl<zip_view_iterator_tag>
        {
            template<typename It>
            struct apply
            {
                typedef typename detail::remove_reference<It>::type it;
                typedef
                    fusion::zip_view_iterator<
                        typename result_of::transform<
                            typename it::iterators
                          , detail::poly_next
                        >::type
                      , typename it::category
                    >
                type;

                static type
                call(It it)
                {
                    return type(
                        fusion::transform(it.iterators_, detail::poly_next())
                      , 0);
                }
            };
        };
    }
}}

#endif
