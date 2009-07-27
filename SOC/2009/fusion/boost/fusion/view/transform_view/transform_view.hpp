/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_TRANSFORM_VIEW_TRANSFORM_VIEW_HPP
#define BOOST_FUSION_VIEW_TRANSFORM_VIEW_TRANSFORM_VIEW_HPP

#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   include <boost/fusion/container/vector/vector10.hpp>
#else
#   include <boost/fusion/container/vector/vector.hpp>
#endif
#include <boost/fusion/support/sequence_base.hpp>
#include <boost/fusion/support/assert.hpp>
#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/support/is_view.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/view/detail/strictest_traversal.hpp>
#include <boost/fusion/view/detail/view_storage.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/call_traits.hpp>

#include <boost/fusion/view/transform_view/transform_view_fwd.hpp>
#include <boost/fusion/view/transform_view/detail/at_impl.hpp>
#include <boost/fusion/view/transform_view/detail/value_at_impl.hpp>

#include <boost/fusion/view/transform_view/detail/transform_view_iterator.hpp>
#include <boost/fusion/view/transform_view/detail/begin_impl.hpp>
#include <boost/fusion/view/transform_view/detail/end_impl.hpp>
#include <boost/fusion/view/transform_view/detail/deref_impl.hpp>
#include <boost/fusion/view/transform_view/detail/next_impl.hpp>
#include <boost/fusion/view/transform_view/detail/prior_impl.hpp>
#include <boost/fusion/view/transform_view/detail/value_of_impl.hpp>
#include <boost/fusion/view/transform_view/detail/advance_impl.hpp>
#include <boost/fusion/view/transform_view/detail/distance_impl.hpp>
#include <boost/fusion/view/transform_view/detail/equal_to_impl.hpp>

namespace boost { namespace fusion
{
    struct void_;
    struct transform_view_tag;
    struct transform_view2_tag;
    struct fusion_sequence_tag;

    // Binary Version
    template <typename Seq1, typename Seq2, typename F>
    struct transform_view
      : sequence_base<transform_view<Seq1, Seq2, F> >
    {
        //BOOST_FUSION_MPL_ASSERT_RELATION(
        //       result_of::size<Sequence1>,==,result_of::size<Sequence2>,
        //       "both sequences must have the same length");

        typedef detail::view_storage<Seq1> storage1_type;
        typedef typename storage1_type::type seq1_type;
        typedef detail::view_storage<Seq2> storage2_type;
        typedef typename storage2_type::type seq2_type;
        typedef F transform_type;

        typedef transform_view2_tag fusion_tag;
        typedef fusion_sequence_tag tag; // this gets picked up by MPL
        typedef mpl::true_ is_view;
        typedef typename
            detail::strictest_traversal<
#ifdef BOOST_NO_VARIADIC_TEMPLATES
                fusion::vector2<seq1_type, seq2_type>
#else
                fusion::vector<seq1_type, seq2_type>
#endif
            >::type
        category;
        typedef typename result_of::size<seq1_type>::type size;

        template<typename OtherTransformView>
        transform_view(BOOST_FUSION_R_ELSE_CLREF(OtherTransformView) view)
          : seq1(BOOST_FUSION_FORWARD(OtherTransformView,view).seq1)
          , seq2(BOOST_FUSION_FORWARD(OtherTransformView,view).seq2)
          , f(BOOST_FUSION_FORWARD(OtherTransformView,view).f)
        {}

#ifdef BOOST_NO_RVALUE_REFERENCES
        transform_view(typename storage1_type::call_param seq1,
                typename storage2_type::call_param seq2,
                typename call_traits<F>::param_type f)
          : seq1(seq1)
          , seq2(seq2)
          , f(f)
        {}
#else
        template<typename OtherSeq1, typename OtherSeq2,typename OtherF>
        transform_view(BOOST_FUSION_R_ELSE_CLREF(OtherSeq1) seq1,
                BOOST_FUSION_R_ELSE_CLREF(OtherSeq2) seq2,
                BOOST_FUSION_R_ELSE_CLREF(OtherF) f)
          : seq1(BOOST_FUSION_FORWARD(OtherSeq1,seq1))
          , seq2(BOOST_FUSION_FORWARD(OtherSeq2,seq2))
          , f(BOOST_FUSION_FORWARD(OtherF,f))
        {}
#endif

        template<typename OtherTransformView>
        OtherTransformView&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherTransformView) other_view)
        {
            seq1=BOOST_FUSION_FORWARD(OtherTransformView,other_view).seq1;
            seq2=BOOST_FUSION_FORWARD(OtherTransformView,other_view).seq2;
            f=BOOST_FUSION_FORWARD(OtherTransformView,other_view).f;
            return *this;
        }

        storage1_type seq1;
        storage2_type seq2;
        transform_type f;
    };

    // Unary Version
    template <typename Seq, typename F>
#if defined(BOOST_NO_PARTIAL_SPECIALIZATION_IMPLICIT_DEFAULT_ARGS)
    struct transform_view<Seq, F, void_>
      : sequence_base<transform_view<Seq, F, void_> >
#else
    struct transform_view<Seq, F>
      : sequence_base<transform_view<Seq, F> >
#endif
    {
        typedef detail::view_storage<Seq> storage_type;
        typedef typename storage_type::type seq_type;
        typedef F transform_type;

        typedef typename traits::category_of<seq_type>::type category;
        typedef typename result_of::size<seq_type>::type size;
        typedef transform_view_tag fusion_tag;
        typedef fusion_sequence_tag tag; // this gets picked up by MPL
        typedef mpl::true_ is_view;

        template<typename OtherTransformView>
        transform_view(BOOST_FUSION_R_ELSE_CLREF(OtherTransformView) view)
          : seq(BOOST_FUSION_FORWARD(OtherTransformView,view).seq)
          , f(BOOST_FUSION_FORWARD(OtherTransformView,view).f)
        {}

#ifdef BOOST_NO_RVALUE_REFERENCES
        transform_view(
            typename storage_type::call_param seq,
            typename call_traits<F>::param_type f)
          : seq(seq)
          , f(f)
        {}
#else
        template<typename OtherSeq, typename OtherF>
        transform_view(OtherSeq&& seq,OtherF&& f)
          : seq(std::forward<OtherSeq>(seq))
          , f(std::forward<OtherF>(f))
        {}
#endif

        template<typename OtherTransformView>
        OtherTransformView&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherTransformView) view)
        {
            seq=BOOST_FUSION_FORWARD(OtherTransformView,view).seq;
            f=BOOST_FUSION_FORWARD(OtherTransformView,view).f;
            return *this;
        }

        storage_type seq;
        transform_type f;
    };
}}

#endif
