/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_CATEGORY_OF_HPP
#define BOOST_FUSION_SUPPORT_CATEGORY_OF_HPP

#include <boost/config.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/support/internal/assert.hpp>

#ifdef BOOST_FUSION_ENABLE_STATIC_ASSERTS
#   include <boost/mpl/or.hpp>
#endif
#include <boost/mpl/iterator_tags.hpp>
#include <boost/type_traits/is_base_of.hpp>

namespace boost { namespace fusion
{
    struct incrementable_traversal_tag
    {};

    struct single_pass_traversal_tag
      : incrementable_traversal_tag
    {};

    //cschmidt: to ensure full compatibility between fusion and mpl iterators,
    //the tags should inheritate the corresponding mpl tags.
    //Fusion tags may be used as template arguments for iterator base classes
    //(e.g. fusion::iterator_facade), therefore emulate the mpl tags
    //(3.4.2/2 !).
    struct forward_traversal_tag
      : single_pass_traversal_tag
    {
        typedef mpl::forward_iterator_tag::tag tag;
        typedef mpl::forward_iterator_tag::value_type value_type;
        typedef forward_traversal_tag type;
        BOOST_STATIC_CONSTANT(
                value_type,
                value=mpl::forward_iterator_tag::value);

        operator value_type() const
        {
            return value;
        }
    };

    struct bidirectional_traversal_tag
      : forward_traversal_tag
    {
        typedef mpl::bidirectional_iterator_tag::tag tag;
        typedef mpl::bidirectional_iterator_tag::value_type value_type;
        typedef bidirectional_traversal_tag type;
        BOOST_STATIC_CONSTANT(
                value_type,
                value=mpl::bidirectional_iterator_tag::value);

        operator value_type() const
        {
            return value;
        }
    };

    struct random_access_traversal_tag
      : bidirectional_traversal_tag
    {
        typedef mpl::random_access_iterator_tag::tag tag;
        typedef mpl::random_access_iterator_tag::value_type value_type;
        typedef bidirectional_traversal_tag type;
        BOOST_STATIC_CONSTANT(
                  value_type,
                  value=mpl::random_access_iterator_tag::value);

        operator value_type() const
        {
            return value;
        }
    };

    struct associative_tag
    {};

    namespace extension
    {
        template<typename>
        struct category_of_impl
        {
            template<typename T>
            struct apply
            {
                BOOST_FUSION_MPL_ASSERT((
                    mpl::or_<traits::is_sequence<T>, traits::is_iterator<T> >));

                typedef typename
                    detail::remove_reference<T>::type::category
                type;
            };
        };
    }

    namespace traits
    {
        template <typename T>
        struct category_of
          : extension::category_of_impl<
                typename fusion::traits::tag_of<T>::type
            >::template apply<T>
        {
            BOOST_FUSION_MPL_ASSERT((
                mpl::or_<traits::is_sequence<T>, traits::is_iterator<T> >));
        };

        template <typename T>
        struct is_associative
          : is_base_of<
                associative_tag
              , typename category_of<T>::type
            >
        {
            BOOST_FUSION_MPL_ASSERT((
                mpl::or_<traits::is_sequence<T>, traits::is_iterator<T> >));
        };

        template <typename T>
        struct is_incrementable
          : is_base_of<
                incrementable_traversal_tag
              , typename category_of<T>::type
            >
        {
            BOOST_FUSION_MPL_ASSERT((
                mpl::or_<traits::is_sequence<T>, traits::is_iterator<T> >));
        };

        template <typename T>
        struct is_single_pass
          : is_base_of<
                single_pass_traversal_tag
              , typename category_of<T>::type
            >
        {
            BOOST_FUSION_MPL_ASSERT((
                mpl::or_<traits::is_sequence<T>, traits::is_iterator<T> >));
        };

        template <typename T>
        struct is_forward
          : is_base_of<
                forward_traversal_tag
              , typename category_of<T>::type
            >
        {
            BOOST_FUSION_MPL_ASSERT((
                mpl::or_<traits::is_sequence<T>, traits::is_iterator<T> >));
        };

        template <typename T>
        struct is_bidirectional
          : is_base_of<
                bidirectional_traversal_tag
              , typename category_of<T>::type
            >
        {
            BOOST_FUSION_MPL_ASSERT((
                mpl::or_<traits::is_sequence<T>, traits::is_iterator<T> >));
        };

        template <typename T>
        struct is_random_access
          : is_base_of<
                random_access_traversal_tag
              , typename category_of<T>::type
            >
        {
            BOOST_FUSION_MPL_ASSERT((
                mpl::or_<traits::is_sequence<T>, traits::is_iterator<T> >));
        };
    }
}}

#endif
