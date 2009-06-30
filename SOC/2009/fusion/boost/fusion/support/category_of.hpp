/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_CATEGORY_OF_HPP
#define BOOST_FUSION_SUPPORT_CATEGORY_OF_HPP

#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/support/tag_of.hpp>

#include <boost/type_traits/is_base_of.hpp>

namespace boost { namespace fusion
{
    struct incrementable_traversal_tag
    {};

    struct single_pass_traversal_tag
      : incrementable_traversal_tag
    {};

    struct forward_traversal_tag
      : single_pass_traversal_tag
    {};

    struct bidirectional_traversal_tag
      : forward_traversal_tag
    {};

    struct random_access_traversal_tag
      : bidirectional_traversal_tag
    {};

    struct associative_sequence_tag
    {};

    namespace extension
    {
        template<typename>
        struct category_of_impl
        {
            template<typename T>
            struct apply
            {
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
            >::template apply<typename detail::add_lref<T>::type>
        {};

        template <typename T>
        struct is_associative
          : is_base_of<
                associative_sequence_tag
              , typename category_of<T>::type
            >
        {};

        template <typename T>
        struct is_incrementable
          : is_base_of<
                incrementable_traversal_tag
              , typename category_of<T>::type
            >
        {};

        template <typename T>
        struct is_single_pass
          : is_base_of<
                single_pass_traversal_tag
              , typename category_of<T>::type
            >
        {};

        template <typename T>
        struct is_forward
          : is_base_of<
                forward_traversal_tag
              , typename category_of<T>::type
            >
        {};

        template <typename T>
        struct is_bidirectional
          : is_base_of<
                bidirectional_traversal_tag
              , typename category_of<T>::type
            >
        {};

        template <typename T>
        struct is_random_access
          : is_base_of<
                random_access_traversal_tag
              , typename category_of<T>::type
            >
        {};
    }
}}

#endif
