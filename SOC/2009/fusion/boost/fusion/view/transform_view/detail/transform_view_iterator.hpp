/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_TRANSFORM_VIEW_ITERATOR_HPP
#define BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_TRANSFORM_VIEW_ITERATOR_HPP

#include <boost/fusion/support/iterator_base.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/internal/assert.hpp>

namespace boost { namespace fusion
{
    // Unary Version
    struct transform_view_iterator_tag;

    template <typename It, typename FRef, typename IsAssociative>
    struct transform_view_iterator
      : iterator_base<transform_view_iterator<It, FRef, IsAssociative> >
    {
        typedef FRef transform_type;
        typedef It it_type;
        typedef typename
            mpl::eval_if<
                traits::is_random_access<it_type>
              , mpl::identity<random_access_traversal_tag>
              , mpl::if_<
                    traits::is_bidirectional<it_type>
                  , bidirectional_traversal_tag
                  , forward_traversal_tag
                >
            >::type
        it_category;
        typedef IsAssociative is_associative;

        typedef transform_view_iterator_tag fusion_tag;
        typedef typename
            mpl::eval_if<
                is_associative
              , mpl::inherit2<it_category,associative_sequence_tag>
              , mpl::identity<it_category>
            >::type
        category;

        template<typename OtherIt>
        transform_view_iterator(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
          : it(BOOST_FUSION_FORWARD(OtherIt,it).it)
          , f(BOOST_FUSION_FORWARD(OtherIt,it).f)
        {
            BOOST_FUSION_TAG_CHECK(OtherIt,transform_view_iterator_tag);
        }

        transform_view_iterator(It const& it, transform_type f)
          : it(it)
          , f(&f)
        {}

        template<typename OtherIt>
        transform_view_iterator&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it_)
        {
            BOOST_FUSION_TAG_CHECK(OtherIt,transform_view_iterator_tag);

            it=BOOST_FUSION_FORWARD(OtherIt,it_).it;
            f=BOOST_FUSION_FORWARD(OtherIt,it_).f;
            return *this;
        }

        it_type it;
        typename detail::remove_reference<transform_type>::type* f;
    };

    // Binary Version
    struct transform_view_iterator2_tag;

    template <typename It1, typename It2, typename FRef, typename IsAssociative>
    struct transform_view_iterator2
      : iterator_base<transform_view_iterator2<It1, It2, FRef, IsAssociative> >
    {
        typedef It1 it1_type;
        typedef It2 it2_type;
        typedef FRef transform_type;
        typedef typename
            detail::strictest_traversal<
#ifdef BOOST_NO_VARIADIC_TEMPLATES
                fusion::vector2<it1_type, it2_type>
#else
                fusion::vector<it1_type, it2_type>
#endif
            >::type
        strictest_traversal;
        typedef IsAssociative is_associative;

        typedef transform_view_iterator2_tag fusion_tag;
        typedef typename
            mpl::eval_if<
                is_associative
              , mpl::inherit2<strictest_traversal,associative_sequence_tag>
              , mpl::identity<strictest_traversal>
            >::type
        category;

        template<typename OtherIt>
        transform_view_iterator2(
                BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
          : it1(BOOST_FUSION_FORWARD(OtherIt,it).it1)
          , it2(BOOST_FUSION_FORWARD(OtherIt,it).it2)
          , f(BOOST_FUSION_FORWARD(OtherIt,it).f)
        {}

        transform_view_iterator2(It1 const& it1,It2 const& it2,transform_type f)
          : it1(it1)
          , it2(it2)
          , f(&f)
        {}

        template<typename OtherIt>
        transform_view_iterator2&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
        {
            BOOST_FUSION_TAG_CHECK(OtherIt,transform_view_iterator2_tag);

            it1=BOOST_FUSION_FORWARD(OtherIt,it).it1;
            it2=BOOST_FUSION_FORWARD(OtherIt,it).it2;
            f=BOOST_FUSION_FORWARD(OtherIt,it).f;
            return *this;
        }

        it1_type it1;
        it2_type it2;
        typename detail::remove_reference<transform_type>::type* f;
    };
}}

#endif
