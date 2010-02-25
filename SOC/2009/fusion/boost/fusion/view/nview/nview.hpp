/*=============================================================================
    Copyright (c) 2009 Hartmut Kaiser
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_NVIEW_NVIEW_HPP
#define BOOST_FUSION_VIEW_NVIEW_NVIEW_HPP

#include <boost/fusion/support/internal/assert.hpp>
#include <boost/fusion/view/detail/view_storage.hpp>
#include <boost/fusion/support/sequence_base.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/workaround.hpp>
#ifndef BOOST_NO_VARIADIC_TEMPLATES
#   include <boost/fusion/support/internal/variadic_templates/variadic_arguments_to_vector_c.hpp>
#else
#   include <boost/fusion/container/vector/limits.hpp>

#   include <boost/preprocessor/cat.hpp>
#   include <boost/preprocessor/iteration/iterate.hpp>
#   include <boost/preprocessor/repetition/enum_params.hpp>
#   include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#   include <boost/mpl/vector/vector10_c.hpp>
#   include <boost/mpl/vector/vector20_c.hpp>
#   include <boost/mpl/vector/vector30_c.hpp>
#   include <boost/mpl/vector/vector40_c.hpp>
#   include <boost/mpl/vector/vector50_c.hpp>
#endif
#include <boost/mpl/size.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/vector_c.hpp>

#include <boost/fusion/view/nview/detail/nview_fwd.hpp>
#include <boost/fusion/view/nview/detail/nview_iterator.hpp>
#include <boost/fusion/view/nview/detail/advance_impl.hpp>
#include <boost/fusion/view/nview/detail/at_impl.hpp>
#include <boost/fusion/view/nview/detail/begin_impl.hpp>
#include <boost/fusion/view/nview/detail/deref_impl.hpp>
#include <boost/fusion/view/nview/detail/distance_impl.hpp>
#include <boost/fusion/view/nview/detail/end_impl.hpp>
#include <boost/fusion/view/nview/detail/equal_to_impl.hpp>
#include <boost/fusion/view/nview/detail/next_impl.hpp>
#include <boost/fusion/view/nview/detail/prior_impl.hpp>
#include <boost/fusion/view/nview/detail/value_at_impl.hpp>
#include <boost/fusion/view/nview/detail/value_of_impl.hpp>

namespace boost { namespace fusion
{
    struct nview_tag;
    struct random_access_traversal_tag;
    struct fusion_sequence_tag;

    template<typename Seq, typename Indices>
    struct nview
      : sequence_base<nview<Seq, Indices> >
    {
        BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
        BOOST_FUSION_MPL_ASSERT((traits::is_random_access<Seq>));

        typedef Indices indices;
        typedef detail::view_storage<Seq> storage_type;
        typedef typename storage_type::type seq_type;

        typedef nview_tag fusion_tag;
        typedef fusion_sequence_tag tag;
        typedef random_access_traversal_tag category;
        typedef mpl::true_ is_view;
        typedef typename mpl::size<Indices>::type size;

#define BOOST_FUSION_NVIEW_CTOR(MODIFIER,_)\
        template<typename OtherSeq>\
        nview(nview<OtherSeq, Indices> MODIFIER view)\
          : seq(static_cast<nview<OtherSeq, Indices> MODIFIER>(view).seq)\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_NVIEW_CTOR,_)

#undef BOOST_FUSION_NVIEW_CTOR

#ifdef BOOST_NO_RVALUE_REFERENCES
        explicit
        nview(typename storage_type::call_param seq)
          : seq(seq)
        {}
#else
        template<typename OtherSeq>
        explicit
        nview(BOOST_FUSION_R_ELSE_CLREF(OtherSeq) seq)
          : seq(BOOST_FUSION_FORWARD(OtherSeq,seq))
        {}
#endif

        template<typename OtherView>
        nview&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherView) other_view)
        {
            BOOST_FUSION_TAG_CHECK(OtherView,nview_tag);

            seq=BOOST_FUSION_FORWARD(OtherView,other_view).seq;
            return *this;
        }

        storage_type seq;
    };

#ifndef BOOST_NO_VARIADIC_TEMPLATES
    namespace result_of
    {
        template<class Seq, int... Indices>
        struct as_nview
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_random_access<Seq>));

            typedef
                nview<
                    Seq
                  , typename detail::variadic_arguments_to_vector_c<
                        Indices...
                    >::type
                //, mpl::vector_c<int, Indices...>
                >
            type;
        };
    }

    template<int... Indices, class Seq>
    typename result_of::as_nview<
        BOOST_FUSION_R_ELSE_CLREF(Seq)
      , Indices...
    >::type
    as_nview(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return typename result_of::as_nview<
                BOOST_FUSION_R_ELSE_CLREF(Seq)
              , Indices...
            >::type(BOOST_FUSION_FORWARD(Seq,seq));
    }

#   ifdef BOOST_NO_RVALUE_REFERENCES
    template<int... Indices, class Seq>
    BOOST_FUSION_EXPLICIT_TEMPLATE_NON_CONST_ARG_OVERLOAD(
        result_of::as_nview,Seq<,&, Indices...>)
    as_nview(Seq& seq)
    {
        return typename result_of::as_nview<Seq&, Indices...>::type(seq);
    }
    BOOST_FUSION_EXPLICIT_TEMPLATE_NON_CONST_ARG_OVERLOAD
#   endif
#else
    namespace result_of
    {
        template<
            class Seq
          , BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(
                FUSION_MAX_VECTOR_SIZE, int I, -1)
        >
        struct as_nview;
    }

#   define BOOST_PP_FILENAME_1\
        <boost/fusion/view/nview/detail/pp/as_nview.hpp>
#   define BOOST_PP_ITERATION_LIMITS (0, FUSION_MAX_VECTOR_SIZE)
    #include BOOST_PP_ITERATE()
#endif
}}

#endif


