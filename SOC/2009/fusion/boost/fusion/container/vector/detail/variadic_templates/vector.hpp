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

#ifndef BOOST_FUSION_TAGGED_VECTOR
#   include <boost/preprocessor/inc.hpp>
#   include <boost/preprocessor/cat.hpp>
#   include <boost/preprocessor/repetition/repeat.hpp>
#   include <boost/preprocessor/repetition/repeat_from_to.hpp>
#   include <boost/preprocessor/repetition/enum.hpp>
#   include <boost/preprocessor/repetition/enum_params.hpp>
#   include <boost/preprocessor/repetition/enum_binary_params.hpp>
#endif
#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>
#ifdef BOOST_NO_RVALUE_REFERENCES
#   include <boost/call_traits.hpp>
#endif
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/add_const.hpp>
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

#ifdef BOOST_FUSION_TAGGED_VECTOR
        template<int...>
        struct package
        {};

        template<int Max,int... Args>
        struct make_package
          : make_package<Max-1,Max-1,Args...>
        {};

        template<int... Args>
        struct make_package<0,Args...>
        {
            typedef package<Args...> type;
        };

        template<int I, typename Element>
        struct element
        {
            Element element;

            element()
              : element()
            {}

            template<typename It>
            element(assign_by_deref,It const& it)
              : element(fusion::deref(fusion::advance_c<I>(it)))
            {}

#ifndef BOOST_NO_RVALUE_REFERENCES
            template<typename OtherArgument>
#endif
            element(
                assign_directly,
#ifdef BOOST_NO_RVALUE_REFERENCES
                typename call_traits<Element>::param_type argument
#else
                OtherArgument&& argument
#endif
                )
              : element(BOOST_FUSION_FORWARD(OtherArgument,argument))
            {}
        };

        template<typename Keys, typename... Elements>
        struct vector_impl;

        template<int... Indices, typename... Elements>
        struct vector_impl<package<Indices...>, Elements...>
          : element<Indices, Elements>...
        {
            vector_impl()
            {}

            template<typename It>
            vector_impl(assign_by_deref,It const& it)
              : element<Indices, Elements>(assign_by_deref(),it)...
            {}

#ifndef BOOST_NO_RVALUE_REFERENCES
            template<typename... OtherArguments>
#endif
            vector_impl(
                assign_directly,
#ifdef BOOST_NO_RVALUE_REFERENCES
                typename call_traits<Elements>::param_type...  arguments
#else
                OtherArguments&&... arguments
#endif
                )
              : element<Indices, Elements>(
                    assign_directly(),
                    BOOST_FUSION_FORWARD(OtherArguments,arguments))...
            {}

            template<typename It,int FirstIndex,int... OtherIndices>
            void
            assign(package<FirstIndex,OtherIndices...>,It const& it)
            {
                detail::at_helper<FirstIndex>(*this)=
                    fusion::deref(fusion::advance_c<FirstIndex>(it));

                assign(package<OtherIndices...>(),it);
            }

            template<typename It>
            void
            assign(package<>,It const& it)
            {}

            template<typename It>
            void
            assign(It const& it)
            {
                assign(package<Indices...>(),it);
            }
        };
    }
#else
        template<int I,typename... Elements>
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

#   define BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DEFAULT_INIT(Z,N,_)\
        BOOST_PP_CAT(element,N)()

#   define BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DEREF_ASSIGN_PARAMS(Z,N,_)\
        BOOST_PP_CAT(element,N)(\
            fusion::deref(fusion::advance_c<N>(it)))

#   define BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DEREF_ASSIGN(Z,N,_)\
        BOOST_PP_CAT(element,N)=fusion::deref(BOOST_PP_CAT(it,N));\
        \
        typedef typename\
            result_of::next<BOOST_PP_CAT(It,N)>::type\
        BOOST_PP_CAT(It,BOOST_PP_INC(N));\
        BOOST_PP_CAT(It,BOOST_PP_INC(N)) BOOST_PP_CAT(it,BOOST_PP_INC(N))=\
            fusion::next(BOOST_PP_CAT(it,N));

#   ifdef BOOST_NO_RVALUE_REFERENCES
#       define BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DIRECT_ASSIGN_TEMPLATE_ARGS(N)

#       define BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DIRECT_ASSIGN_PARAMS(N)\
        BOOST_PP_ENUM_BINARY_PARAMS(N, typename call_traits<H,>::type _)
#   else
#       define BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DIRECT_ASSIGN_TEMPLATE_ARGS(N)\
        template<BOOST_PP_ENUM_PARAMS(N, typename OtherH)>

#       define BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DIRECT_ASSIGN_PARAMS(N)\
        BOOST_PP_ENUM_BINARY_PARAMS(N, OtherH,&& _)
#   endif

#   define BOOST_FUSION_UNROLLED_VECTOR_DIRECT_ASSIGN(Z,N,_)\
        BOOST_PP_CAT(element,N)(BOOST_PP_CAT(_,N))

#   ifdef BOOST_FUSION_PREFER_MPL
#       define BOOST_FUSION_META_VALUE_AT(N)
#   else
#       define BOOST_FUSION_META_VALUE_AT(N)\
        typedef BOOST_PP_CAT(H,N) BOOST_PP_CAT(h,N);
#   endif

#   define BOOST_FUSION_UNROLLED_VECTOR_AT(Z,N,_)\
        BOOST_FUSION_META_VALUE_AT(N)\
        \
        typename add_lref<BOOST_PP_CAT(H,N)>::type\
        at_impl(mpl::int_<I+N>)\
        {\
            return BOOST_PP_CAT(element,N) ;\
        }\
        \
        typename add_lref<\
            typename add_const<BOOST_PP_CAT(H,N)>::type\
        >::type\
        at_impl(mpl::int_<I+N>)const\
        {\
            return BOOST_PP_CAT(element,N) ;\
        }\
        \
        BOOST_PP_CAT(H,N) BOOST_PP_CAT(element,N);

#   define BOOST_FUSION_UNROLLED_VECTOR_IMPL(Z, N, MAX)\
        template<int I, BOOST_PP_ENUM_PARAMS(N, typename H)>\
        struct vector_impl<I,BOOST_PP_ENUM_PARAMS(N, H)>\
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
            BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DIRECT_ASSIGN_TEMPLATE_ARGS(N)\
            vector_impl(\
                assign_directly,\
                BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DIRECT_ASSIGN_PARAMS(N)\
            )\
              : BOOST_PP_ENUM(N, BOOST_FUSION_UNROLLED_VECTOR_DIRECT_ASSIGN, _)\
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
            BOOST_PP_REPEAT(N,BOOST_FUSION_UNROLLED_VECTOR_AT,_)\
        };

        BOOST_PP_REPEAT_FROM_TO(1, 5, BOOST_FUSION_UNROLLED_VECTOR_IMPL, _)

        template<int I, BOOST_PP_ENUM_PARAMS(4, typename H), typename... Others>
        struct vector_impl<I,BOOST_PP_ENUM_PARAMS(4, H),Others...>
          : vector_impl<I+4,Others...>
        {
            typedef vector_impl<I+4,Others...> base_type;
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

#   ifndef BOOST_NO_RVALUE_REFERENCES
            template<
                BOOST_PP_ENUM_PARAMS(4, typename OtherH)
              , typename... OtherElements
            >
#   endif
            vector_impl(
                assign_directly,
                BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DIRECT_ASSIGN_PARAMS(4),
#   ifdef BOOST_NO_RVALUE_REFERENCES
                typename call_traits<Others>::param_type...  elements
#   else
                OtherElements&&... elements
#   endif
            )
              : base_type(assign_directly(),
                        BOOST_FUSION_FORWARD(OtherElements,elements)...)
              , BOOST_PP_ENUM(4, BOOST_FUSION_UNROLLED_VECTOR_DIRECT_ASSIGN, _)
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

            BOOST_PP_REPEAT(4,BOOST_FUSION_UNROLLED_VECTOR_AT,_)
        };

#   undef BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DEFAULT_INIT
#   undef BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DEREF_ASSIGN_PARAMS
#   undef BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DEREF_ASSIGN
#   undef BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DIRECT_ASSIGN_TEMPLATE_ARGS
#   undef BOOST_FUSION_UNROLLED_VECTOR_MEMBER_DIRECT_ASSIGN_PARAMS
#   undef BOOST_FUSION_UNROLLED_VECTOR_DIRECT_ASSIGN
#   undef BOOST_FUSION_META_VALUE_AT
#   undef BOOST_FUSION_UNROLLED_VECTOR_AT
#   undef BOOST_FUSION_UNROLLED_VECTOR_IMPL
    }

#   ifndef BOOST_FUSION_PREFER_MPL
    template<typename Vector, int I>
    struct vector_meta_value_at
      : vector_meta_value_at<typename Vector::chain_upper, I-4>
    {};

#       define BOOST_FUSION_META_VALUE_AT_SPECIALIZATION(Z,N,_)\
    template<typename Vector>\
    struct vector_meta_value_at<Vector,N>\
    {\
        typedef typename Vector::BOOST_PP_CAT(h,N) type;\
    };

    BOOST_PP_REPEAT(4, BOOST_FUSION_META_VALUE_AT_SPECIALIZATION, _)

#       undef BOOST_FUSION_META_VALUE_AT_SPECIALIZATION
#   endif
#endif

    template<typename... Elements>
    struct vector
      : sequence_base<vector<Elements...> >
#ifdef BOOST_FUSION_TAGGED_VECTOR
      , detail::vector_impl<
            typename detail::make_package<sizeof...(Elements)>::type
          , Elements...
        >
#else
      , detail::vector_impl<0,Elements...>
#endif
    {
#ifdef BOOST_FUSION_TAGGED_VECTOR
        typedef
            detail::vector_impl<
                typename detail::make_package<sizeof...(Elements)>::type
              , Elements...
            >
        base_type;
#else
        typedef detail::vector_impl<0,Elements...> base_type;

#   ifdef BOOST_FUSION_PREFER_MPL
        typedef typename
            detail::variadic_arguments_to_vector<Elements...>::type
        types;
#   endif
#endif

        typedef vector_tag fusion_tag;
        typedef fusion_sequence_tag tag;
        typedef mpl::false_ is_view;
        typedef random_access_traversal_tag category;
        typedef mpl::int_<sizeof...(Elements)> size;

        vector()
        {}

#define VECTOR_CTOR(MODIFIER,_)\
        vector(vector MODIFIER vec)\
          : base_type(detail::assign_by_deref(),\
                fusion::begin(static_cast<vector MODIFIER>(vec)))\
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

#define BOOST_FUSION_VECTOR_ASSIGN_CTOR(MODIFIER,_)\
        template<typename SeqRef>\
        vector(detail::sequence_assign_type<SeqRef> MODIFIER seq_assign)\
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
