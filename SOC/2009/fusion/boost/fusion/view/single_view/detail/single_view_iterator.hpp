/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

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

    //cschmidt: Due to a nasty bug in Proto (ticket #3583,
    //https://svn.boost.org/trac/boost/ticket/3583) store value_type and
    //value_ref_type directly, rather than a reference to the view itself.
    template<typename Value, typename ValueRef, bool End>
    struct single_view_iterator
      : iterator_base<single_view_iterator<Value, ValueRef, End> >
    {
        typedef Value value_type;
        typedef ValueRef value_ref_type;
        typedef mpl::bool_<End> end;

        typedef single_view_iterator_tag fusion_tag;
        typedef random_access_traversal_tag category;

        template<typename OtherIt>
        single_view_iterator(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
          : val(BOOST_FUSION_FORWARD(OtherIt,it).val)
        {
            BOOST_FUSION_TAG_CHECK(OtherIt,single_view_iterator_tag);
        }

        explicit
        single_view_iterator(value_ref_type val)
          : val(&val)
        {}

        template<typename OtherIt>
        single_view_iterator&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
        {
            BOOST_FUSION_TAG_CHECK(OtherIt,single_view_iterator_tag);

            val=BOOST_FUSION_FORWARD(OtherIt,it).val;
            return *this;
        }

        typename detail::remove_reference<value_ref_type>::type* val;
    };
}}

#endif
