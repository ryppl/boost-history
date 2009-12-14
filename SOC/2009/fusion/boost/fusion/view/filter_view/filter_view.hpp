/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_FILTER_VIEW_FILTER_VIEW_HPP
#define BOOST_FUSION_VIEW_FILTER_VIEW_FILTER_VIEW_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/sequence_base.hpp>
#include <boost/fusion/support/internal/assert.hpp>
#include <boost/fusion/view/detail/view_storage.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/identity.hpp>
#ifdef BOOST_FUSION_ENABLE_STATIC_ASSERTS
#   include <boost/type_traits/is_same.hpp>
#endif

#include <boost/fusion/view/filter_view/detail/filter_view_fwd.hpp>
#include <boost/fusion/view/filter_view/detail/filter_view_iterator.hpp>
#include <boost/fusion/view/filter_view/detail/size_impl.hpp>
#include <boost/fusion/view/filter_view/detail/begin_impl.hpp>
#include <boost/fusion/view/filter_view/detail/end_impl.hpp>
#include <boost/fusion/view/filter_view/detail/deref_impl.hpp>
#include <boost/fusion/view/filter_view/detail/next_impl.hpp>
#include <boost/fusion/view/filter_view/detail/value_of_impl.hpp>
#include <boost/fusion/view/filter_view/detail/equal_to_impl.hpp>
#include <boost/fusion/view/filter_view/detail/deref_data_impl.hpp>
#include <boost/fusion/view/filter_view/detail/key_of_impl.hpp>
#include <boost/fusion/view/filter_view/detail/value_of_data_impl.hpp>

namespace boost { namespace fusion
{
    struct fusion_sequence_tag;

    template <typename Seq, typename Pred>
    struct filter_view
      : sequence_base<filter_view<Seq, Pred> >
    {
        BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
        BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));

        typedef detail::view_storage<Seq> storage_type;
        typedef typename storage_type::type seq_type;
        typedef Pred pred_type;

        typedef typename
            mpl::eval_if<
                traits::is_associative<seq_type>
              , mpl::inherit2<forward_traversal_tag,associative_tag>
              , mpl::identity<forward_traversal_tag>
            >::type
        category;
        typedef filter_view_tag fusion_tag;
        typedef fusion_sequence_tag tag; 
        typedef mpl::true_ is_view;

#define BOOST_FUSION_FILTER_VIEW_CTOR(MODIFIER,_)\
        template<typename OtherSeq>\
        filter_view(filter_view<OtherSeq, Pred> MODIFIER view)\
          : seq(static_cast<filter_view<OtherSeq, Pred> MODIFIER>(view).seq)\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_FILTER_VIEW_CTOR,_)

#undef BOOST_FUSION_FILTER_VIEW_CTOR

#ifdef BOOST_NO_RVALUE_REFERENCES
        explicit
        filter_view(typename storage_type::call_param seq)
          : seq(seq)
        {}
#else
        template<typename OtherSeq>
        explicit
        filter_view(BOOST_FUSION_R_ELSE_CLREF(OtherSeq) seq)
          : seq(BOOST_FUSION_FORWARD(OtherSeq,seq))
        {}
#endif

        template<typename OtherView>
        filter_view&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherView) other_view)
        {
            BOOST_FUSION_TAG_CHECK(OtherView,filter_view_tag);
            BOOST_FUSION_MPL_ASSERT((is_same<
                    pred_type
                  , detail::remove_reference<OtherView>::type::pred_type
                >));

            seq=BOOST_FUSION_FORWARD(OtherView,other_view).seq;
            return *this;
        }

        storage_type seq;
    };
}}

#endif
