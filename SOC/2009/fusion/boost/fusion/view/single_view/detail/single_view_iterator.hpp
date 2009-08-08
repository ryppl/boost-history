/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_SINGLE_VIEW_DETAIL_SINGLE_VIEW_ITERATOR_HPP
#define BOOST_FUSION_VIEW_SINGLE_VIEW_DETAIL_SINGLE_VIEW_ITERATOR_HPP

#include <boost/fusion/support/iterator_base.hpp>

namespace boost { namespace fusion
{
    struct single_view_iterator_tag;
    struct forward_traversal_tag;

    struct void_;

    template <typename SingleView>
    struct single_view_iterator_end
      : iterator_base<single_view_iterator_end<SingleView> >
    {
        typedef void_ view_type;

        typedef single_view_iterator_tag fusion_tag;
        typedef forward_traversal_tag category;
    };

    template <typename SingleViewRef>
    struct single_view_iterator
      : iterator_base<single_view_iterator<SingleViewRef> >
    {
        typedef SingleViewRef view_type;
        typedef typename
            detail::forward_as<
                SingleViewRef,
                typename detail::remove_reference<
                    SingleViewRef
                >::type::value_type
            >::type
        value_type;

        typedef single_view_iterator_tag fusion_tag;
        typedef forward_traversal_tag category;

        template<typename OtherIt>
        single_view_iterator(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
          : view(it.view)
        {}

        single_view_iterator(SingleViewRef view, int)
          : view(&view)
        {}

        template<typename OtherIt>
        single_view_iterator&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
        {
            view=it.view;
            return *this;
        }

        typename detail::remove_reference<SingleViewRef>::type* view;
    };
}}

#endif
