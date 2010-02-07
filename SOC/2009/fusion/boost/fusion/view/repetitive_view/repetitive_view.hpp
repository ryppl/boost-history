/*=============================================================================
    Copyright (c) 2007 Tobias Schwinger
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_REPETITIVE_VIEW_REPETITIVE_VIEW_HPP
#define BOOST_FUSION_VIEW_REPETITIVE_VIEW_REPETITIVE_VIEW_HPP

//TODO doc/testcase/zip_view testcase

#include <boost/fusion/support/deduce.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/sequence_base.hpp>
#include <boost/fusion/support/internal/workaround.hpp>
#include <boost/fusion/support/internal/assert.hpp>
#include <boost/fusion/view/detail/view_storage.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/int.hpp>
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

    template<
        typename Seq
      , typename Size=mpl::int_<integer_traits<int>::const_max-1>
    >
    struct repetitive_view
      : sequence_base<repetitive_view<Seq> >
    {
        BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
        BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));
        BOOST_FUSION_MPL_ASSERT((mpl::or_<
                            mpl:not_<result_of::empty<Seq> >
                          , mpl::not_<Size>
                        >));

        typedef detail::view_storage<Seq> storage_type;
        typedef typename storage_type::type seq_type;

        typedef typename traits::category_of<seq_type>::type category;
        typedef Size size;
        typedef repetitive_view_tag fusion_tag;
        typedef fusion_sequence_tag tag;
        typedef mpl::true_ is_view;

#define BOOST_FUSION_REPETITIVE_VIEW_CTOR(MODIFIER,_)\
        template<typename OtherSeq>\
        repetitive_view(repetitive_view<OtherSeq,Size> MODIFIER view)\
          : seq(static_cast<repetitive_view<OtherSeq,Size> MODIFIER>(\
                view).seq)\
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
            BOOST_FUSION_TAG_CHECK(OtherView,repetitive_view_tag);
            BOOST_FUSION_MPL_ASSERT((mpl::equal_to<
                    size
                  , detail::remove_reference<OtherView>::type::size
                >));

            seq=BOOST_FUSION_FORWARD(OtherView,other_view).seq;
            return *this;
        }

        detail::view_storage<Seq> seq;
    };


    namespace result_of
    {
        template<
            typename Seq
          , typename Size=mpl::int_<integer_traits<int>::const_max-1>
        >
        struct repeat
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));
            BOOST_FUSION_MPL_ASSERT((mpl::or_<
                                mpl:not_<result_of::empty<Seq> >
                              , mpl::not_<Size>
                            >));

            typedef
                repetitive_view<typename traits::deduce<Seq>::type, Size>
            type;
        };
    };

    template<typename Size, typename Seq>
    inline typename
        result_of::repeat<BOOST_FUSION_R_ELSE_CLREF(Seq),Size>::type
    repeat(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return typename result_of::repeat<
            BOOST_FUSION_R_ELSE_CLREF(Seq)
          , Size
        >::type(BOOST_FUSION_FORWARD(Seq,seq));
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template<typename Size, typename Seq>
    inline BOOST_FUSION_EXPLICIT_TEMPLATE_NON_CONST_ARG_OVERLOAD(
            result_of::repeat<,Seq,&, Size>)
    repeat(Seq& seq)
    {
        return typename result_of::repeat<Seq&,Size>::type(seq);
    }
#endif

    template<typename Seq>
    inline typename result_of::repeat<BOOST_FUSION_R_ELSE_CLREF(Seq)>::type
    repeat(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return typename result_of::repeat<BOOST_FUSION_R_ELSE_CLREF(Seq)>::type(
            BOOST_FUSION_FORWARD(Seq,seq));
    }

    template<typename Seq>
    //cschmidt: see https://svn.boost.org/trac/boost/ticket/3305
#   if defined(BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS) || BOOST_WORKAROUND(__GNUC__,<4)
    typename lazy_disable_if<
        is_const<Seq>
      , result_of::repeat<Seq&>
    >::type
#   else
    typename result_of::repeat<Seq&>::type
#   endif
    repeat(Seq& seq)
    {
        return typename result_of::repeat<Seq&>::type(seq);
    }
}}

#endif
