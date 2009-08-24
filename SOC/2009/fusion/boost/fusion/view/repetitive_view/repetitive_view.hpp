/*=============================================================================
    Copyright (c) 2007 Tobias Schwinger
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_REPETITIVE_VIEW_REPETITIVE_VIEW_HPP
#define BOOST_FUSION_VIEW_REPETITIVE_VIEW_REPETITIVE_VIEW_HPP

//TODO doc/testcase/zip_view testcase

#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/sequence_base.hpp>
#include <boost/fusion/support/internal/assert.hpp>
#include <boost/fusion/view/detail/view_storage.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/long.hpp>
#include <boost/integer_traits.hpp>

#include <boost/fusion/view/repetitive_view/detail/repetitive_view_fwd.hpp>
#include <boost/fusion/view/repetitive_view/detail/repetitive_view_iterator.hpp>
#include <boost/fusion/view/repetitive_view/detail/begin_impl.hpp>
#include <boost/fusion/view/repetitive_view/detail/end_impl.hpp>
#include <boost/fusion/view/repetitive_view/detail/deref_impl.hpp>
#include <boost/fusion/view/repetitive_view/detail/next_impl.hpp>
#include <boost/fusion/view/repetitive_view/detail/prior_impl.hpp>
#include <boost/fusion/view/repetitive_view/detail/advance_impl.hpp>
#include <boost/fusion/view/repetitive_view/detail/distance_impl.hpp>
#include <boost/fusion/view/repetitive_view/detail/value_of_impl.hpp>
#include <boost/fusion/view/repetitive_view/detail/equal_to_impl.hpp>
#include <boost/fusion/view/repetitive_view/detail/deref_data_impl.hpp>
#include <boost/fusion/view/repetitive_view/detail/key_of_impl.hpp>
#include <boost/fusion/view/repetitive_view/detail/value_of_data_impl.hpp>

namespace boost { namespace fusion
{
    struct fusion_sequence_tag;

    template<typename Seq>
    struct repetitive_view
      : sequence_base<repetitive_view<Seq> >
    {
        BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));

        typedef detail::view_storage<Seq> storage_type;
        typedef typename storage_type::type seq_type;

        typedef typename traits::category_of<seq_type>::type category;
        typedef mpl::int_<integer_traits<int>::const_max-1> size;
        typedef repetitive_view_tag fusion_tag;
        typedef fusion_sequence_tag tag;
        typedef mpl::true_ is_view;

#define BOOST_FUSION_REPETITIVE_VIEW_CTOR(COMBINATION,_)\
        template<typename OtherSeq>\
        repetitive_view(repetitive_view<OtherSeq> COMBINATION view)\
          : seq(BOOST_FUSION_FORWARD(\
                repetitive_view<OtherSeq> COMBINATION,view).seq)\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_REPETITIVE_VIEW_CTOR,_)

#undef BOOST_FUSION_REPETITIVE_VIEW_CTOR

#ifdef BOOST_NO_RVALUE_REFERENCES
        explicit
        repetitive_view(typename storage_type::call_param seq)
          : seq(seq)
        {}
#else
        template<typename OtherSeq>
        explicit
        repetitive_view(OtherSeq&& seq)
          : seq(std::forward<OtherSeq>(seq))
        {}
#endif

        template<typename OtherView>
        repetitive_view&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherView) other_view)
        {
            BOOST_FUSION_TAG_CHECK(OtherView,reverse_view_tag);

            seq=BOOST_FUSION_FORWARD(OtherView,other_view).seq;
            return *this;
        }

        detail::view_storage<Seq> seq;
    };

}}

#endif
