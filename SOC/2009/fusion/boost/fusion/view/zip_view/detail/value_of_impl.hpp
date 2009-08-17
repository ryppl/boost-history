/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_ZIP_VIEW_DETAIL_VALUE_OF_IMPL_HPP
#define BOOST_FUSION_VIEW_ZIP_VIEW_DETAIL_VALUE_OF_IMPL_HPP

#include <boost/fusion/container/vector/convert.hpp>
#include <boost/fusion/algorithm/transformation/transform.hpp>
#include <boost/fusion/iterator/value_of.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/fusion/support/unused.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        struct poly_value_of
        {
            template<typename T>
            struct result;

            template<typename Self, typename ItRef>
            struct result<Self(ItRef)>
              : mpl::eval_if<
                    is_same<ItRef, unused_type const&>
                  , mpl::identity<unused_type const&>
                  , result_of::value_of<ItRef>
                >
            {};
        };
    }

    namespace extension
    {
        template<typename Tag>
        struct value_of_impl;

        template<>
        struct value_of_impl<zip_view_iterator_tag>
        {
            template<typename ItRef>
            struct apply
            {
                typedef typename
                    result_of::transform<
                        typename detail::remove_reference<
                            ItRef
                        >::type::iterators
                      , detail::poly_value_of
                    >::type
                values;

                typedef typename result_of::as_vector<values>::type type;
            };
        };
    }
}}

#endif
