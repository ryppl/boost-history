/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_TRANSFORM_VIEW_ITERATOR_HPP
#define BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_TRANSFORM_VIEW_ITERATOR_HPP

#include <boost/fusion/support/iterator_base.hpp>
#include <boost/fusion/support/category_of.hpp>

namespace boost { namespace fusion
{
    // Unary Version
    struct transform_view_iterator_tag;

    template <typename First, typename FRef>
    struct transform_view_iterator
        : iterator_base<transform_view_iterator<First, FRef> >
    {
        //TODO !!!
        typedef FRef transform_type;
        typedef First first_type;

        typedef transform_view_iterator_tag fusion_tag;
        typedef typename traits::category_of<first_type>::type category;

        template<typename OtherIt>
        transform_view_iterator(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
          : first(BOOST_FUSION_FORWARD(OtherIt,it).first)
          , f(BOOST_FUSION_FORWARD(OtherIt,it).f)
        {}

        transform_view_iterator(First const& first, transform_type f)
          : first(first)
          , f(&f)
        {}

        template<typename OtherIt>
        transform_view_iterator&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
        {
            first=BOOST_FUSION_FORWARD(OtherIt,it).first;
            f=BOOST_FUSION_FORWARD(OtherIt,it).f;
            return *this;
        }

        first_type first;
        typename detail::remove_reference<transform_type>::type* f;
    };

    // Binary Version
    struct transform_view_iterator2_tag;

    template <typename First1, typename First2, typename FRef>
    struct transform_view_iterator2
        : iterator_base<transform_view_iterator2<First1, First2, FRef> >
    {
        typedef First1 first1_type;
        typedef First2 first2_type;
        typedef FRef transform_type;

        typedef transform_view_iterator2_tag fusion_tag;
        typedef typename traits::category_of<first1_type>::type category;

        template<typename OtherIt>
        transform_view_iterator2(
                BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
          : first1(BOOST_FUSION_FORWARD(OtherIt,it).first1)
          , first2(BOOST_FUSION_FORWARD(OtherIt,it).first2)
          , f(BOOST_FUSION_FORWARD(OtherIt,it).f)
        {}

        transform_view_iterator2(First1 const& first1,
                First2 const& first2,
                FRef f)
          : first1(first1)
          , first2(first2)
          , f(&f)
        {}

        template<typename OtherIt>
        transform_view_iterator2&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
        {
            first1=BOOST_FUSION_FORWARD(OtherIt,it).first1;
            first2=BOOST_FUSION_FORWARD(OtherIt,it).first2;
            f=BOOST_FUSION_FORWARD(OtherIt,it).f;
            return *this;
        }

        first1_type first1;
        first2_type first2;
        typename detail::remove_reference<transform_type>::type* f;
    };
}}

#endif
