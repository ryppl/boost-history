/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_REVERSE_VIEW_REVERSE_VIEW_HPP
#define BOOST_FUSION_VIEW_REVERSE_VIEW_REVERSE_VIEW_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/support/is_view.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/sequence_base.hpp>
#include <boost/fusion/support/assert.hpp>

#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/bool.hpp>

#include <boost/fusion/view/reverse_view/reverse_view_iterator.hpp>
#include <boost/fusion/view/reverse_view/detail/begin_impl.hpp>
#include <boost/fusion/view/reverse_view/detail/end_impl.hpp>

namespace boost { namespace fusion
{
    struct reverse_view_tag;
    struct fusion_sequence_tag;

    template <typename Seq>
    struct reverse_view
      : sequence_base<reverse_view<Seq> >
    {
        typedef reverse_view_tag fusion_tag;
        typedef fusion_sequence_tag tag; // this gets picked up by MPL
        typedef mpl::true_ is_view;

        typedef typename
            mpl::if_<
                traits::is_view<Seq>
              , typename detail::remove_reference<Seq>::type
              , typename detail::add_lref<Seq>::type
            >::type
        seq_type;
        typedef typename traits::category_of<seq_type>::type category;
        typedef typename result_of::size<seq_type>::type size;

        //BOOST_FUSION_STATIC_ASSERT(
        //    (is_base_of<bidirectional_traversal_tag,
        //            typename traits::category_of<first_type>::type>::value),
        //    "underlying iterator must be bidirectional");

        template<typename OtherSeq>
        explicit reverse_view(BOOST_FUSION_R_ELSE_LREF(OtherSeq) other_seq)
          : seq(BOOST_FUSION_FORWARD(OtherSeq,other_seq))
        {}

        seq_type seq;
    };
}}

#endif
