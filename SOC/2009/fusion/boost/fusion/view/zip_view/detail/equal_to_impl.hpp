/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_ZIP_VIEW_DETAIL_EQUAL_TO_IMPL_HPP
#define BOOST_FUSION_VIEW_ZIP_VIEW_DETAIL_EQUAL_TO_IMPL_HPP

#include <boost/fusion/mpl.hpp>
#include <boost/fusion/iterator/equal_to.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/mpl/zip_view.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/mpl/unpack_args.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        template<typename It1, typename It2>
        struct zip_iterators_equal
        {
            typedef
                mpl::zip_view<
                    mpl::vector2<
                        typename It1::iterators
                      , typename It2::iterators
                    >
                >
            zipped;
            typedef
                mpl::transform_view<
                    zipped
                  , mpl::unpack_args<result_of::equal_to<mpl::_1,mpl::_1> >
                >
            transformed;
            typedef typename
                mpl::find_if<
                    transformed
                  , mpl::equal_to<mpl::_1, mpl::false_>
                >::type
            found;

            typedef typename
                is_same<typename mpl::end<transformed>::type, found>::type
            type;
        };
    }

    namespace extension
    {
        template<typename>
        struct equal_to_impl;

        template<>
        struct equal_to_impl<zip_view_iterator_tag>
        {
            template<typename It1, typename It2>
            struct apply
              : detail::zip_iterators_equal<
                    typename detail::remove_reference<It1>::type
                  , typename detail::remove_reference<It2>::type
                >::type
            {};
        };
    }
}}

#endif
