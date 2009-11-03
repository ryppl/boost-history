/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

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
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/internal/assert.hpp>
#include <boost/fusion/view/detail/strictest_traversal.hpp>
#include <boost/fusion/view/detail/view_storage.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/bool.hpp>
#ifdef BOOST_FUSION_ENABLE_STATIC_ASSERTS
#   include <boost/mpl/equal_to.hpp>
#endif
#include <boost/call_traits.hpp>

#include <boost/fusion/view/transform_view/transform_view_fwd.hpp>
#include <boost/fusion/view/transform_view/detail/at_impl.hpp>
#include <boost/fusion/view/transform_view/detail/value_at_impl.hpp>
#include <boost/fusion/view/transform_view/detail/transform_view_iterator.hpp>
#include <boost/fusion/view/transform_view/detail/begin_impl.hpp>
#include <boost/fusion/view/transform_view/detail/end_impl.hpp>
#include <boost/fusion/view/transform_view/detail/deref_impl.hpp>
#include <boost/fusion/view/transform_view/detail/deref_data_impl.hpp>
#include <boost/fusion/view/transform_view/detail/next_impl.hpp>
#include <boost/fusion/view/transform_view/detail/prior_impl.hpp>
#include <boost/fusion/view/transform_view/detail/value_of_impl.hpp>
#include <boost/fusion/view/transform_view/detail/value_of_data_impl.hpp>
#include <boost/fusion/view/transform_view/detail/key_of_impl.hpp>
#include <boost/fusion/view/transform_view/detail/advance_impl.hpp>
#include <boost/fusion/view/transform_view/detail/distance_impl.hpp>
#include <boost/fusion/view/transform_view/detail/equal_to_impl.hpp>

namespace boost { namespace fusion
{
    struct void_;
    struct transform_view_tag;
    struct transform_view2_tag;
    struct fusion_sequence_tag;

    //TODO IsAssociative

    // Binary Version
    template<typename Seq1, typename Seq2, typename F, typename IsAssociative>
    struct transform_view
      : sequence_base<transform_view<Seq1, Seq2, F,IsAssociative> >
    {
        BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq1>));
        BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq1>));
        BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq2>));
        BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq2>));
        BOOST_FUSION_MPL_ASSERT((
            mpl::equal_to<result_of::size<Seq1>,result_of::size<Seq2> >));

        typedef IsAssociative is_associative;
        typedef detail::view_storage<Seq1> storage1_type;
        typedef typename storage1_type::type seq1_type;
        typedef detail::view_storage<Seq2> storage2_type;
        typedef typename storage2_type::type seq2_type;
        typedef F transform_type;
        typedef typename
            detail::strictest_traversal<
#ifdef BOOST_NO_VARIADIC_TEMPLATES
                fusion::vector2<seq1_type, seq2_type>
#else
                fusion::vector<seq1_type, seq2_type>
#endif
            >::type
        strictest_traversal;

        typedef transform_view2_tag fusion_tag;
        typedef fusion_sequence_tag tag;
        typedef mpl::true_ is_view;
        typedef typename
            mpl::eval_if<
                is_associative
              , mpl::inherit2<strictest_traversal,associative_tag>
              , mpl::identity<strictest_traversal>
            >::type
        category;
        typedef typename result_of::size<seq1_type>::type size;

        template<typename OtherView>
        transform_view(BOOST_FUSION_R_ELSE_CLREF(OtherView) view)
          : seq1(BOOST_FUSION_FORWARD(OtherView,view).seq1)
          , seq2(BOOST_FUSION_FORWARD(OtherView,view).seq2)
          , f(BOOST_FUSION_FORWARD(OtherView,view).f)
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

        template<typename OtherView>
        OtherView&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherView) other_view)
        {
            BOOST_FUSION_TAG_CHECK(OtherView,transform_view2_tag);

            seq1=BOOST_FUSION_FORWARD(OtherView,other_view).seq1;
            seq2=BOOST_FUSION_FORWARD(OtherView,other_view).seq2;
            f=BOOST_FUSION_FORWARD(OtherView,other_view).f;
            return *this;
        }

        storage1_type seq1;
        storage2_type seq2;
        transform_type f;
    };

    // Unary Version implementation
    template<typename Seq, typename F, typename IsAssociative>
    struct unary_transform_view
      : sequence_base<unary_transform_view<Seq, F, void_> >
    {
        BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
        BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));

        typedef IsAssociative is_associative;
        typedef detail::view_storage<Seq> storage_type;
        typedef typename storage_type::type seq_type;
        typedef typename
            mpl::eval_if<
                traits::is_random_access<seq_type>
              , mpl::identity<random_access_traversal_tag>
              , mpl::if_<
                    traits::is_bidirectional<seq_type>
                  , bidirectional_traversal_tag
                  , forward_traversal_tag
                >
            >::type
        seq_category;
        typedef F transform_type;

        typedef typename
            mpl::eval_if<
                is_associative
              , mpl::inherit2<seq_category,associative_tag>
              , mpl::identity<seq_category>
            >::type
        category;
        typedef typename result_of::size<seq_type>::type size;
        typedef transform_view_tag fusion_tag;
        typedef fusion_sequence_tag tag;
        typedef mpl::true_ is_view;

        template<typename OtherView>
        unary_transform_view(BOOST_FUSION_R_ELSE_CLREF(OtherView) view)
          : seq(BOOST_FUSION_FORWARD(OtherView,view).seq)
          , f(BOOST_FUSION_FORWARD(OtherView,view).f)
        {}

#ifdef BOOST_NO_RVALUE_REFERENCES
        unary_transform_view(
            typename storage_type::call_param seq,
            typename call_traits<transform_type>::param_type f)
          : seq(seq)
          , f(f)
        {}
#else
        template<typename OtherSeq, typename OtherF>
        unary_transform_view(OtherSeq&& seq,OtherF&& f)
          : seq(std::forward<OtherSeq>(seq))
          , f(std::forward<OtherF>(f))
        {}
#endif

        template<typename OtherView>
        unary_transform_view&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherView) view)
        {
            BOOST_FUSION_TAG_CHECK(OtherView,transform_view_tag);

            seq=BOOST_FUSION_FORWARD(OtherView,view).seq;
            f=BOOST_FUSION_FORWARD(OtherView,view).f;
            return *this;
        }

        storage_type seq;
        transform_type f;
    };

    //TODO cschmidt: template typedef
    template<typename Seq, typename F>
    struct transform_view<Seq, F, mpl::true_,mpl::false_>
      : unary_transform_view<Seq, F, mpl::true_>
    {
        typedef unary_transform_view<Seq, F, mpl::true_> base;

        template<typename OtherView>
        transform_view(BOOST_FUSION_R_ELSE_CLREF(OtherView) view)
          : base(BOOST_FUSION_FORWARD(OtherView,view))
        {}

#ifdef BOOST_NO_RVALUE_REFERENCES
        transform_view(
            typename base::storage_type::call_param seq,
            typename call_traits<typename base::transform_type>::param_type f)
          : base(seq,f)
        {}
#else
        template<typename OtherSeq, typename OtherF>
        transform_view(OtherSeq&& seq,OtherF&& f)
          : base(std::forward<OtherSeq>(seq),std::forward<OtherF>(f))
        {}
#endif

        template<typename OtherView>
        transform_view&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherView) view)
        {
            *static_cast<base*>(this)=BOOST_FUSION_FORWARD(OtherView,view);
            return *this;
        }
    };

    template<typename Seq, typename F>
    struct transform_view<Seq, F, mpl::false_,mpl::false_>
      : unary_transform_view<Seq, F, mpl::false_>
    {
        typedef unary_transform_view<Seq, F, mpl::false_> base;

        template<typename OtherView>
        transform_view(BOOST_FUSION_R_ELSE_CLREF(OtherView) view)
          : base(BOOST_FUSION_FORWARD(OtherView,view))
        {}

#ifdef BOOST_NO_RVALUE_REFERENCES
        transform_view(
            typename base::storage_type::call_param seq,
            typename call_traits<typename base::transform_type>::param_type f)
          : base(seq,f)
        {}
#else
        template<typename OtherSeq, typename OtherF>
        transform_view(OtherSeq&& seq,OtherF&& f)
          : base(std::forward<OtherSeq>(seq),std::forward<OtherF>(f))
        {}
#endif

        template<typename OtherView>
        transform_view&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherView) view)
        {
            *static_cast<base*>(this)=BOOST_FUSION_FORWARD(OtherView,view);
            return *this;
        }
    };
}}

#endif
