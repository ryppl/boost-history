/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_VECTOR_DETAIL_VARIADIC_TEMPLATES_VECTOR_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_DETAIL_VARIADIC_TEMPLATES_VECTOR_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/iterator/advance_c.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/deref.hpp>
#ifdef BOOST_FUSION_PREFER_MPL
#   include <boost/fusion/support/internal/variadic_templates/variadic_arguments_to_vector.hpp>
#endif
#include <boost/fusion/support/internal/sequence_assign.hpp>
#include <boost/fusion/support/internal/assign_tags.hpp>
#include <boost/fusion/support/sequence_base.hpp>
#include <boost/fusion/support/internal/assert.hpp>

#include <boost/preprocessor/inc.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>
#ifdef BOOST_NO_RVALUE_REFERENCES
#   include <boost/call_traits.hpp>
#endif
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>

#include <utility>

namespace boost { namespace fusion
{
    struct fusion_sequence_tag;

    namespace detail
    {
        template<typename From, typename... Elements>
        struct is_convertible_to_head;

        template<typename From, typename Head, typename... Rest>
        struct is_convertible_to_head<From, Head, Rest...>
          : is_convertible<From, Head>
        {};

        template<int Index,typename... Elements>
        struct vector_impl;

        template<>
        struct vector_impl<0>
        {
            vector_impl()
            {}

            template<typename It>
            vector_impl(assign_by_deref,It const& it)
            {}

            vector_impl(assign_directly)
            {}

            template<typename It0>
            void
            assign(It0 const& it0)
            {}
        };

#define BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DEFAULT_INIT(Z,N,_)\
        BOOST_PP_CAT(_element,N)()

#define BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DEREF_ASSIGN_PARAMS(Z,N,_)\
        BOOST_PP_CAT(_element,N)(\
            fusion::deref(fusion::advance_c<N>(it)))

#define BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DEREF_ASSIGN(Z,N,_)\
        BOOST_PP_CAT(_element,N)=fusion::deref(BOOST_PP_CAT(it,N));\
        \
        typedef typename\
            result_of::next<BOOST_PP_CAT(It,N)>::type\
        BOOST_PP_CAT(It,BOOST_PP_INC(N));\
        BOOST_PP_CAT(It,BOOST_PP_INC(N)) BOOST_PP_CAT(it,BOOST_PP_INC(N))=\
            fusion::next(BOOST_PP_CAT(it,N));

#ifdef BOOST_NO_RVALUE_REFERENCES
#   define BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DIRECT_ASSIGN_TEMPLATE_ARGS(N)

#   define BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DIRECT_ASSIGN_PARAMS(N)\
        BOOST_PP_ENUM_BINARY_PARAMS(N, typename call_traits<H,>::type _)
#else
#   define BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DIRECT_ASSIGN_TEMPLATE_ARGS(N)\
        template<BOOST_PP_ENUM_PARAMS(N, typename OtherH)>

#   define BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DIRECT_ASSIGN_PARAMS(N)\
        BOOST_PP_ENUM_BINARY_PARAMS(N, OtherH,&& _)
#endif

#define BOOST_FUSION_UNROLLED_VECTOR_DIRECT_ASSIGN(Z,N,_)\
        BOOST_PP_CAT(_element,N)(BOOST_PP_CAT(_,N))

#ifdef BOOST_FUSION_PREFER_MPL
#   define BOOST_FUSION_META_VALUE_AT(N)
#else
#   define BOOST_FUSION_META_VALUE_AT(N)\
        typedef BOOST_PP_CAT(H,N) BOOST_PP_CAT(h,N);
#endif

#define BOOST_FUSION_UNROLLED_VECTOR_AT(Z,N,_)\
        BOOST_FUSION_META_VALUE_AT(N)\
        \
        typename detail::add_lref<BOOST_PP_CAT(H,N)>::type\
        at_impl(mpl::int_<Index+N>)\
        {\
            return BOOST_PP_CAT(_element,N) ;\
        }\
        \
        typename detail::add_lref<\
            typename add_const<BOOST_PP_CAT(H,N)>::type\
        >::type\
        at_impl(mpl::int_<Index+N>)const\
        {\
            return BOOST_PP_CAT(_element,N) ;\
        }\
        \
        BOOST_PP_CAT(H,N) BOOST_PP_CAT(_element,N);

#define BOOST_FUSION_UNROLLED_VECTOR_IMPL(Z, N, MAX)\
        template<int Index, BOOST_PP_ENUM_PARAMS(N, typename H)>\
        struct vector_impl<Index,BOOST_PP_ENUM_PARAMS(N, H)>\
        {\
            vector_impl()\
              : BOOST_PP_ENUM(\
                      N,\
                      BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DEFAULT_INIT,\
                      _)\
            {}\
            \
            template<typename It>\
            vector_impl(assign_by_deref,It const& it)\
              : BOOST_PP_ENUM(N,\
                    BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DEREF_ASSIGN_PARAMS,\
                    _)\
            {}\
            \
            template<typename It0>\
            void\
            assign(It0 const& it0)\
            {\
                BOOST_PP_REPEAT(N,\
                        BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DEREF_ASSIGN,\
                        _)\
            }\
            \
            BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DIRECT_ASSIGN_TEMPLATE_ARGS(N)\
            vector_impl(\
                assign_directly,\
                BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DIRECT_ASSIGN_PARAMS(N)\
            )\
              : BOOST_PP_ENUM(N, BOOST_FUSION_UNROLLED_VECTOR_DIRECT_ASSIGN, _)\
            {}\
            \
            BOOST_PP_REPEAT(N,BOOST_FUSION_UNROLLED_VECTOR_AT,_)\
        };

        BOOST_PP_REPEAT_FROM_TO(1, 5, BOOST_FUSION_UNROLLED_VECTOR_IMPL, _)

        template<
            int Index
          , BOOST_PP_ENUM_PARAMS(4, typename H)
          , typename... Others
        >
        struct vector_impl<Index,BOOST_PP_ENUM_PARAMS(4, H),Others...>
          : vector_impl<Index+4,Others...>
        {
            typedef vector_impl<Index+4,Others...> base_type;
            typedef base_type chain_upper;

            using base_type::at_impl;

            vector_impl()
              : BOOST_PP_ENUM(4,
                      BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DEFAULT_INIT,
                      _)
            {}

            template<typename It>
            vector_impl(assign_by_deref,It const& it)
              : base_type(assign_by_deref(),fusion::advance_c<4>(it))
              , BOOST_PP_ENUM(4,
                    BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DEREF_ASSIGN_PARAMS,
                    _)
            {}

            template<typename It0>
            void
            assign(It0 const& it0)
            {
                BOOST_PP_REPEAT(4,
                        BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DEREF_ASSIGN,
                        _)

                static_cast<base_type*>(this)->assign(fusion::next(it4));
            }

#ifndef BOOST_NO_RVALUE_REFERENCES
            template<
                BOOST_PP_ENUM_PARAMS(4, typename OtherH)
              , typename... OtherElements
            >
#endif
            vector_impl(
                assign_directly,
                BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DIRECT_ASSIGN_PARAMS(4),
#ifdef BOOST_NO_RVALUE_REFERENCES
                typename call_traits<Others>::param_type...  elements
#else
                OtherElements&&... elements
#endif
            )
              : base_type(assign_directly(),
                        BOOST_FUSION_FORWARD(OtherElements,elements)...)
              , BOOST_PP_ENUM(4, BOOST_FUSION_UNROLLED_VECTOR_DIRECT_ASSIGN, _)
            {}

            BOOST_PP_REPEAT(4,BOOST_FUSION_UNROLLED_VECTOR_AT,_)
        };

#undef BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DEFAULT_INIT
#undef BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DEREF_ASSIGN_PARAMS
#undef BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DEREF_ASSIGN
#undef BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DIRECT_ASSIGN_TEMPLATE_ARGS
#undef BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DIRECT_ASSIGN_PARAMS
#undef BOOST_FUSION_UNROLLED_VECTOR_DIRECT_ASSIGN
#undef BOOST_FUSION_META_VALUE_AT
#undef BOOST_FUSION_UNROLLED_VECTOR_AT
#undef BOOST_FUSION_UNROLLED_VECTOR_IMPL
    }

#ifndef BOOST_FUSION_PREFER_MPL
    template<typename Vector, int Index>
    struct vector_meta_value_at
      : vector_meta_value_at<typename Vector::chain_upper, Index-4>
    {};

#   define BOOST_FUSION_META_VALUE_AT_SPECIALIZATION(Z,N,_)\
    template<typename Vector>\
    struct vector_meta_value_at<Vector,N>\
    {\
        typedef typename Vector::BOOST_PP_CAT(h,N) type;\
    };

    BOOST_PP_REPEAT(4, BOOST_FUSION_META_VALUE_AT_SPECIALIZATION, _)

#   undef BOOST_FUSION_META_VALUE_AT_SPECIALIZATION
#endif

    template<typename... Elements>
    struct vector
      : sequence_base<vector<Elements...> >
      , detail::vector_impl<0,Elements...>
    {
        typedef detail::vector_impl<0,Elements...> base_type;

#ifdef BOOST_FUSION_PREFER_MPL
        typedef typename
            detail::variadic_arguments_to_vector<Elements...>::type
        types;
#endif

        typedef vector_tag fusion_tag;
        typedef fusion_sequence_tag tag;
        typedef mpl::false_ is_view;
        typedef random_access_traversal_tag category;
        typedef mpl::int_<sizeof...(Elements)> size;

        vector()
        {}

#define VECTOR_CTOR(COMBINATION,_)\
        vector(vector COMBINATION vec)\
          : base_type(detail::assign_by_deref(),\
                fusion::begin(BOOST_FUSION_FORWARD(vector COMBINATION,vec)))\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(VECTOR_CTOR,_)

#undef VECTOR_CTOR

#ifndef BOOST_NO_RVALUE_REFERENCES
        template<typename... OtherArguments>
#endif
        explicit
        vector(
#ifdef BOOST_NO_RVALUE_REFERENCES
            typename call_traits<Elements>::param_type...  arguments
#else
            OtherArguments&&... arguments
#endif
            )
          : base_type(detail::assign_directly(),
                 BOOST_FUSION_FORWARD(OtherArguments,arguments)...)
        {
            BOOST_FUSION_STATIC_ASSERT((
                    sizeof...(Elements)==sizeof...(OtherArguments)));
        }

#define BOOST_FUSION_VECTOR_ASSIGN_CTOR(COMBINATION,_)\
        template<typename SeqRef>\
        vector(detail::sequence_assign_type<SeqRef> COMBINATION seq_assign)\
          : base_type(detail::assign_by_deref(),fusion::begin(seq_assign.get()))\
        {\
            BOOST_FUSION_STATIC_ASSERT((\
                sizeof...(Elements)==result_of::size<SeqRef>::value));\
        }

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_VECTOR_ASSIGN_CTOR,_);

#undef BOOST_FUSION_VECTOR_ASSIGN_CTOR

        template<typename Seq>
        vector(BOOST_FUSION_R_ELSE_CLREF(Seq) seq,
            typename disable_if<
                mpl::and_<
                    mpl::bool_<sizeof...(Elements)==1>
                  , detail::is_convertible_to_head<
                        BOOST_FUSION_R_ELSE_CLREF(Seq)
                      , Elements...
                    >
                >
            >::type* =0)
          : base_type(detail::assign_by_deref(),
                 fusion::begin(BOOST_FUSION_FORWARD(Seq,seq)))
        {
            BOOST_FUSION_STATIC_ASSERT((
                sizeof...(Elements)==result_of::size<Seq>::value));
        }

        template<typename Seq>
        vector&
        operator=(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
        {
            BOOST_FUSION_STATIC_ASSERT((
                sizeof...(Elements)==result_of::size<Seq>::value));

            static_cast<base_type*>(this)->assign(
                fusion::begin(BOOST_FUSION_FORWARD(Seq,seq)));
            return *this;
        }
    };

}}
#endif
