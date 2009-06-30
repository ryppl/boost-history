/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_FILTER_VIEW_FILTER_VIEW_HPP
#define BOOST_FUSION_VIEW_FILTER_VIEW_FILTER_VIEW_HPP

#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/support/sequence_base.hpp>
#include <boost/fusion/support/is_view.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/mpl/bool.hpp>

#include <boost/fusion/view/filter_view/filter_view_iterator.hpp>
#include <boost/fusion/view/filter_view/detail/begin_impl.hpp>
#include <boost/fusion/view/filter_view/detail/end_impl.hpp>
#include <boost/fusion/view/filter_view/detail/size_impl.hpp>

namespace boost { namespace fusion
{
    struct filter_view_tag;
    struct forward_traversal_tag;
    struct fusion_sequence_tag;

    template <typename Seq, typename Pred>
    struct filter_view
      : sequence_base<filter_view<Seq, Pred> >
    {
        typedef filter_view_tag fusion_tag;
        typedef fusion_sequence_tag tag; // this gets picked up by MPL
        typedef forward_traversal_tag category;
        typedef mpl::true_ is_view;

        typedef typename
            mpl::if_<
                traits::is_view<Seq>
              , typename detail::identity<Seq>::type
              , typename detail::add_lref<Seq>::type
            >::type
        seq_type;
        typedef Pred pred_type;

        template<typename OtherSeq>
        explicit filter_view(BOOST_FUSION_R_ELSE_LREF(OtherSeq) other_seq)
          : seq(BOOST_FUSION_FORWARD(OtherSeq,other_seq))
        {}

        seq_type seq;
    };
}}

#endif
