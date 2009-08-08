/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_JOINT_VIEW_DETAIL_JOINT_VIEW_ITERATOR_HPP
#define BOOST_FUSION_VIEW_JOINT_VIEW_DETAIL_JOINT_VIEW_ITERATOR_HPP

#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/support/iterator_base.hpp>
#include <boost/fusion/support/assert.hpp>

namespace boost { namespace fusion
{
    struct joint_view_iterator_tag;
    struct concat_iterator_tag;
    struct forward_traversal_tag;

    template <typename First, typename Last, typename Concat>
    struct joint_view_iterator
      : iterator_base<joint_view_iterator<First, Last, Concat> >
    {
        typedef joint_view_iterator_tag fusion_tag;
        typedef forward_traversal_tag category;

        typedef First first_type;
        typedef Last last_type;
        typedef Concat concat_type;

        joint_view_iterator(First const& first, Concat const& concat)
          : first(first)
          , concat(concat)
        {}

        first_type first;
        concat_type concat;
    };

    template <typename First>
    struct concat_iterator
      : iterator_base<concat_iterator<First> >
    {
        typedef First first_type;

        typedef concat_iterator_tag fusion_tag;
        typedef forward_traversal_tag category;

        explicit
        concat_iterator(First const& first)
          : first(first)
        {}

        first_type first;
    };
}}

#endif
