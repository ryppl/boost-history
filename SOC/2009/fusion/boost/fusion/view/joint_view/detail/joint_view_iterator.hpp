/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_JOINT_VIEW_DETAIL_JOINT_VIEW_ITERATOR_HPP
#define BOOST_FUSION_VIEW_JOINT_VIEW_DETAIL_JOINT_VIEW_ITERATOR_HPP

#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/support/iterator_base.hpp>

namespace boost { namespace fusion
{
    struct joint_view_iterator_tag;
    struct concat_iterator_tag;
    struct forward_traversal_tag;

    template <typename Category, typename Begin, typename End, typename Concat>
    struct joint_view_iterator
      : iterator_base<joint_view_iterator<Category, Begin, End, Concat> >
    {
        typedef joint_view_iterator_tag fusion_tag;
        typedef Category category;

        typedef Begin begin_type;
        typedef End end_type;
        typedef Concat concat_type;

        joint_view_iterator(Begin const& first, Concat const& concat)
          : first(first)
          , concat(concat)
        {}

        begin_type first;
        concat_type concat;
    };

    template <typename Category, typename Begin>
    struct concat_iterator
      : iterator_base<concat_iterator<Category, Begin> >
    {
        typedef Begin begin_type;

        typedef concat_iterator_tag fusion_tag;
        typedef Category category;

        explicit
        concat_iterator(Begin const& first)
          : first(first)
        {}

        begin_type first;
    };
}}

#endif
