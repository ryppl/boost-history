/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_SINGLE_VIEW_SINGLE_VIEW_ITERATOR_HPP
#define BOOST_FUSION_VIEW_SINGLE_VIEW_SINGLE_VIEW_ITERATOR_HPP

#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/support/iterator_base.hpp>

#include <boost/fusion/view/single_view/detail/deref_impl.hpp>
#include <boost/fusion/view/single_view/detail/next_impl.hpp>
#include <boost/fusion/view/single_view/detail/value_of_impl.hpp>

namespace boost { namespace fusion
{
    struct single_view_iterator_tag;
    struct forward_traversal_tag;

    template <typename SingleView>
    struct single_view_iterator_end
      : iterator_base<single_view_iterator_end<SingleView> >
    {
        typedef single_view_iterator_tag fusion_tag;
        typedef forward_traversal_tag category;
    };

    template <typename SingleView>
    struct single_view_iterator
      : iterator_base<single_view_iterator<SingleView> >
    {
        typedef single_view_iterator_tag fusion_tag;
        typedef forward_traversal_tag category;

        typedef SingleView view_type;
        typedef typename
            detail::result_of_forward_as<
                SingleView,
                typename detail::remove_reference<SingleView>::type::value_type
            >::type
        value_type;

        explicit single_view_iterator(SingleView view)
          : view(view)
        {}

        SingleView view;
    };
}}

#endif
