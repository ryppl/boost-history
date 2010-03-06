/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ITERATOR_EQUAL_TO_HPP
#define BOOST_FUSION_ITERATOR_EQUAL_TO_HPP

#include <boost/config.hpp>
#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/support/is_iterator.hpp>
#include <boost/fusion/support/internal/assert.hpp>

#include <boost/mpl/and.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost { namespace fusion
{
    struct iterator_facade_tag;

    namespace extension
    {
        template <typename>
        struct equal_to_impl;

        template <>
        struct equal_to_impl<iterator_facade_tag>
        {
            template <typename It1, typename It2>
            struct apply
              : detail::remove_reference<It1>::type::template equal_to<It1, It2>
            {};
        };
    }

    namespace result_of
    {
        template <typename It1, typename It2>
        struct equal_to
          : extension::equal_to_impl<typename traits::tag_of<It1>::type>::
                template apply<It1, It2>::type
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_iterator<It1>));
            BOOST_FUSION_MPL_ASSERT((traits::is_iterator<It2>));
        };
    }

    namespace iterator_operators
    {
        template <typename It1, typename It2>
        inline
#ifndef BOOST_NO_CONSTEXPR
        constexpr
#endif
        typename enable_if<
            mpl::and_<traits::is_iterator<It1>, traits::is_iterator<It2> >
          , bool
        >::type
        operator==(It1 const&, It2 const&)
        {
            return result_of::equal_to<It1 const&, It1 const&>::value;
        }

        template <typename It1, typename It2>
        inline
#ifndef BOOST_NO_CONSTEXPR
        constexpr
#endif
        typename enable_if<
            mpl::and_<
                traits::is_iterator<It1>
              , traits::is_iterator<It2>
            >
          , bool
        >::type
        operator!=(It1 const&, It2 const&)
        {
            return !result_of::equal_to<It1 const&, It2 const&>::value;
        }
    }

    using iterator_operators::operator==;
    using iterator_operators::operator!=;
}}

#endif
