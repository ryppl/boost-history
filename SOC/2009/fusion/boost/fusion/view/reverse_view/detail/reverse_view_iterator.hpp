/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_REVERSE_VIEW_DETAIL_REVERSE_VIEW_ITERATOR_HPP
#define BOOST_FUSION_VIEW_REVERSE_VIEW_DETAIL_REVERSE_VIEW_ITERATOR_HPP

#include <boost/fusion/support/iterator_base.hpp>
#include <boost/fusion/support/category_of.hpp>

namespace boost { namespace fusion
{
    struct reverse_view_iterator_tag;

    template <typename It>
    struct reverse_view_iterator
      : iterator_base<reverse_view_iterator<It> >
    {
        typedef It it_type;

        typedef reverse_view_iterator_tag fusion_tag;
        typedef typename traits::category_of<it_type>::type category;

        template<typename OtherIt>
        reverse_view_iterator(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
          : it(BOOST_FUSION_FORWARD(OtherIt,it).it)
        {
            BOOST_FUSION_TAG_CHECK(OtherIt,reverse_view_iterator_tag);
        }

        reverse_view_iterator(It const& it,int)
          : it(it)
        {}

        template<typename OtherIt>
        reverse_view_iterator&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it_)
        {
            BOOST_FUSION_TAG_CHECK(OtherIt,reverse_view_iterator_tag);

            it=BOOST_FUSION_FORWARD(OtherIt,it_).it;
            return *this;
        }

        it_type it;
    };
}}

#endif
