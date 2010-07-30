/*==============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_SINGLE_VIEW_DETAIL_SINGLE_VIEW_ITERATOR_HPP
#define BOOST_FUSION_VIEW_SINGLE_VIEW_DETAIL_SINGLE_VIEW_ITERATOR_HPP

#include <boost/fusion/support/iterator_base.hpp>
#include <boost/mpl/bool.hpp>

namespace boost { namespace fusion
{
    struct single_view_iterator_tag;

    template<typename Value, typename ValueRef, bool End>
    struct single_view_iterator
      : iterator_base<single_view_iterator<Value, ValueRef, End> >
    {
        typedef Value value_type;
        typedef ValueRef value_ref_type;
        typedef mpl::bool_<End> end;
        typedef typename
            detail::remove_reference<value_ref_type>::type*
        value_ptr_type;

        typedef single_view_iterator_tag fusion_tag;
        typedef random_access_traversal_tag category;

        single_view_iterator(value_ptr_type val, int)
          : val(val)
        {}

        template<typename OtherValueRef>
        single_view_iterator(
            single_view_iterator<Value, OtherValueRef, End> const it)
          : val(it.val)
        {
            BOOST_FUSION_MPL_ASSERT((
                is_convertible<OtherValueRef, value_ref_type>));
        }

        template<typename OtherValueRef>
        single_view_iterator&
        operator=(single_view_iterator<Value, OtherValueRef, End> const& it)
        {
            BOOST_FUSION_MPL_ASSERT((
                is_convertible<OtherValueRef, value_ref_type>));

            val=it.val;
            return *this;
        }

        value_ptr_type val;
    };
}}

#endif
