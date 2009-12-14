/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_DETAIL_FILTER_KEY_VIEW_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_DETAIL_FILTER_KEY_VIEW_HPP

#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/view/detail/view_storage.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>

#include <boost/mpl/bool.hpp>

#include <boost/fusion/algorithm/transformation/detail/filter_key_view/filter_key_view_fwd.hpp>
#include <boost/fusion/algorithm/transformation/detail/filter_key_view/has_key_impl.hpp>
#include <boost/fusion/algorithm/transformation/detail/filter_key_view/at_key_impl.hpp>
#include <boost/fusion/algorithm/transformation/detail/filter_key_view/value_at_key_impl.hpp>

namespace boost { namespace fusion
{
    struct fusion_sequence_tag;

    namespace detail
    {
        template <typename Seq, typename Pred>
        struct filter_key_view
          : sequence_base<filter_key_view<Seq, Pred> >
        {
            typedef view_storage<Seq> storage_type;
            typedef typename storage_type::type seq_type;
            typedef Pred pred_type;

            typedef associative_tag category;
            typedef filter_key_view_tag fusion_tag;
            typedef fusion_sequence_tag tag;
            typedef mpl::true_ is_view;

#define BOOST_FUSION_FILTER_KEY_VIEW_CTOR(MODIFIER,_)\
            template<typename OtherSeq>\
            filter_key_view(filter_key_view<OtherSeq,Pred> MODIFIER view)\
              : seq(static_cast<filter_key_view<OtherSeq,Pred>MODIFIER>(\
                    view).seq)\
            {\
                BOOST_FUSION_TAG_CHECK(OtherSeq,filter_key_view);\
            }

            BOOST_FUSION_ALL_CTOR_COMBINATIONS(
                    BOOST_FUSION_FILTER_KEY_VIEW_CTOR,_)

#undef BOOST_FUSION_FILTER_KEY_VIEW_CTOR

#ifdef BOOST_NO_RVALUE_REFERENCES
            explicit
            filter_key_view(typename storage_type::call_param seq)
              : seq(seq)
            {}
#else
            template<typename OtherSeq>
            explicit
            filter_key_view(OtherSeq&& seq)
              : seq(std::forward<OtherSeq>(seq))
            {}
#endif

            template<typename OtherView>
            filter_key_view&
            operator=(BOOST_FUSION_R_ELSE_CLREF(OtherView) other_view)
            {
                BOOST_FUSION_TAG_CHECK(OtherView, filter_key_view_tag);

                seq=BOOST_FUSION_FORWARD(OtherView,other_view).seq;
                return *this;
            }

            detail::view_storage<Seq> seq;
        };
    }
}}

#endif
