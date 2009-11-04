/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_JOINT_VIEW_DETAIL_JOINT_VIEW_ITERATOR_HPP
#define BOOST_FUSION_VIEW_JOINT_VIEW_DETAIL_JOINT_VIEW_ITERATOR_HPP

#include <boost/fusion/support/iterator_base.hpp>

namespace boost { namespace fusion
{
    struct joint_view_iterator_tag;
    struct concat_iterator_tag;

    template <typename Category, typename Begin, typename End, typename Concat>
    struct joint_view_iterator
      : iterator_base<joint_view_iterator<Category, Begin, End, Concat> >
    {
        typedef Begin begin_type;
        typedef End end_type;
        typedef Concat concat_type;

        typedef joint_view_iterator_tag fusion_tag;
        typedef Category category;

        template<typename OtherIt>
        joint_view_iterator(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
          : first(BOOST_FUSION_FORWARD(OtherIt,it).first)
          , concat(BOOST_FUSION_FORWARD(OtherIt,it).concat)
        {
            BOOST_FUSION_TAG_CHECK(OtherIt,joint_view_iterator_tag);
        }

        joint_view_iterator(Begin const& first, Concat const& concat)
          : first(first)
          , concat(concat)
        {}

        template<typename OtherIt>
        joint_view_iterator&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
        {
            BOOST_FUSION_TAG_CHECK(OtherIt,joint_view_iterator_tag);

            first=BOOST_FUSION_FORWARD(OtherIt,it).first;
            concat=BOOST_FUSION_FORWARD(OtherIt,it).concat;
            return *this;
        }

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

        template<typename OtherIt>
        concat_iterator(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
          : first(BOOST_FUSION_FORWARD(OtherIt,it).first)
        {
            BOOST_FUSION_TAG_CHECK(OtherIt,concat_iterator_tag);
        }

        concat_iterator(Begin const& first,int)
          : first(first)
        {}

        template<typename OtherIt>
        concat_iterator&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
        {
            BOOST_FUSION_TAG_CHECK(OtherIt,concat_iterator_tag);

            first=BOOST_FUSION_FORWARD(OtherIt,it).first;
            return *this;
        }

        begin_type first;
    };
}}

#endif
