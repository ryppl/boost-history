/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_ZIP_VIEW_ZIP_VIEW_ITERATOR_HPP
#define BOOST_FUSION_VIEW_ZIP_VIEW_ZIP_VIEW_ITERATOR_HPP

#include <boost/fusion/container/vector/convert.hpp>
#include <boost/fusion/support/iterator_base.hpp>
#include <boost/fusion/support/ref.hpp>

#include <boost/fusion/view/zip_view/zip_view_iterator_fwd.hpp>
#include <boost/fusion/view/zip_view/detail/deref_impl.hpp>
#include <boost/fusion/view/zip_view/detail/next_impl.hpp>
#include <boost/fusion/view/zip_view/detail/prior_impl.hpp>
#include <boost/fusion/view/zip_view/detail/advance_impl.hpp>
#include <boost/fusion/view/zip_view/detail/distance_impl.hpp>
#include <boost/fusion/view/zip_view/detail/value_of_impl.hpp>
#include <boost/fusion/view/zip_view/detail/equal_to_impl.hpp>

namespace boost { namespace fusion {

    struct zip_view_iterator_tag;

    template<typename ItSeq, typename Traversal>
    struct zip_view_iterator
      : iterator_base<zip_view_iterator<ItSeq, Traversal> >
    {
        typedef zip_view_iterator_tag fusion_tag;
        typedef Traversal category;

        //TODO !!!
        zip_view_iterator(BOOST_FUSION_R_ELSE_CLREF(zip_view_iterator) it)
          : iterators_(std::move(it._iterators_))
        {
        }

        template<typename InitSeq>
        zip_view_iterator(BOOST_FUSION_R_ELSE_CLREF(InitSeq) iterator_seq)
          : iterators_(
                sequence_assign(BOOST_FUSION_FORWARD(InitSeq,iterator_seq)))
        {}

        typedef typename result_of::as_vector<ItSeq>::type iterators;
        iterators iterators_;
    };
}}

#endif
