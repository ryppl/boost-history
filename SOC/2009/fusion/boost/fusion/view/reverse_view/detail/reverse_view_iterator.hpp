/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_REVERSE_VIEW_DETAIL_REVERSE_VIEW_ITERATOR_HPP
#define BOOST_FUSION_VIEW_REVERSE_VIEW_DETAIL_REVERSE_VIEW_ITERATOR_HPP

#include <boost/fusion/support/iterator_base.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/assert.hpp>

namespace boost { namespace fusion
{
    struct reverse_view_iterator_tag;

    template <typename First>
    struct reverse_view_iterator
      : iterator_base<reverse_view_iterator<First> >
    {
        typedef First first_type;

        typedef reverse_view_iterator_tag fusion_tag;
        typedef typename traits::category_of<first_type>::type category;

        //BOOST_FUSION_STATIC_ASSERT(
        //    (is_base_of<bidirectional_traversal_tag, category>::value),
        //    "underlying iterator must be bidirectional");

        reverse_view_iterator(First const& first)
          : first(first)
        {}

        first_type first;
    };
}}

#endif
