/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ITERATOR_ITERATOR_FACADE_HPP
#define BOOST_FUSION_ITERATOR_ITERATOR_FACADE_HPP

#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/iterator/detail/advance.hpp>
#include <boost/fusion/support/iterator_base.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/assert.hpp>

#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion
{
    struct iterator_facade_tag;

    template <typename Derived, typename Category>
    struct iterator_facade
      : iterator_base<Derived>
    {
        typedef iterator_facade_tag fusion_tag;
        typedef Derived derived_type;
        typedef Category category;

        // default implementation
        template <typename It1Ref, typename It2Ref>
        struct equal_to // default implementation
          : is_same<
                typename detail::remove_reference<It1Ref>::type::derived_type
              , typename detail::remove_reference<It2Ref>::type::derived_type
            >
        {};

        // default implementation
        template <typename ItRef, typename N>
        struct advance
          : mpl::if_c<
                (N::value > 0)
              , advance_detail::forward<ItRef, N::value>
              , advance_detail::backward<ItRef, N::value>
            >::type
        {
     //             BOOST_FUSION_MPL_ASSERT_NOT(
     //                     traits::is_random_access<Iterator>,
     //       "default implementation not available for random access iterators");
        };
    };
}}

#endif
