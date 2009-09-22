/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_ZIP_VIEW_DETAIL_ZIP_VIEW_ITERATOR_HPP
#define BOOST_FUSION_VIEW_ZIP_VIEW_DETAIL_ZIP_VIEW_ITERATOR_HPP

#include <boost/fusion/container/vector/convert.hpp>
#include <boost/fusion/support/iterator_base.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>

namespace boost { namespace fusion {

    struct zip_view_iterator_tag;

    template<
        typename ItSeq
      , typename Traversal=typename detail::strictest_traversal<ItSeq>::type
    >
    struct zip_view_iterator
      : iterator_base<zip_view_iterator<ItSeq, Traversal> >
    {
        typedef zip_view_iterator_tag fusion_tag;
        typedef Traversal category;

        template<typename OtherIt>
        zip_view_iterator(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
          : iterators_(BOOST_FUSION_FORWARD(OtherIt,it).iterators_)
        {
            BOOST_FUSION_TAG_CHECK(OtherIt,zip_view_iterator_tag);
        }

        template<typename Seq>
        zip_view_iterator(BOOST_FUSION_R_ELSE_CLREF(Seq) seq,int)
          : iterators_(BOOST_FUSION_FORWARD(Seq,seq))
        {}

        template<typename OtherIt>
        zip_view_iterator&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
        {
            BOOST_FUSION_TAG_CHECK(OtherIt,zip_view_iterator_tag);

            iterators_=BOOST_FUSION_FORWARD(OtherIt,it).iterators_;
            return *this;
        }

        typedef typename result_of::as_vector<ItSeq>::type iterators;
        iterators iterators_;
    };
}}

#endif
