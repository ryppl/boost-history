/*==============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_STRUCT_DEFINE_STRUCT_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_STRUCT_DEFINE_STRUCT_HPP

#include <boost/config.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/advance_c.hpp>
#include <boost/fusion/adapted/detail/struct/adapt_base.hpp>
#include <boost/fusion/adapted/detail/struct/namespace.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>
#include <boost/fusion/support/internal/sequence_assign.hpp>
#include <boost/fusion/support/internal/is_explicitly_convertible.hpp>
#include <boost/fusion/support/internal/constexpr.hpp>
#include <boost/preprocessor/inc.hpp>
#include <boost/preprocessor/if.hpp>
#include <boost/preprocessor/expr_if.hpp>
#include <boost/preprocessor/dec.hpp>
#include <boost/preprocessor/logical/not.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/tuple/eat.hpp>
#ifndef BOOST_NO_RVALUE_REFERENCES
#   include <boost/preprocessor/repetition/enum_params.hpp>
#   include <boost/preprocessor/repetition/enum_binary_params.hpp>
#else
#   include <boost/call_traits.hpp>
#endif
#include <boost/utility/enable_if.hpp>

#define BOOST_FUSION_DEFINE_STRUCT_COPY_MOVE_CTOR_FILLER_I(                     \
    R, DATA, I, ATTRIBUTE)                                                      \
                                                                                \
    BOOST_PP_COMMA_IF(I)                                                        \
    BOOST_PP_TUPLE_ELEM(BOOST_PP_TUPLE_ELEM(2,1,DATA),1,ATTRIBUTE)(             \
        static_cast<self_type BOOST_PP_TUPLE_ELEM(2,0,DATA)>(other_self).       \
            BOOST_PP_TUPLE_ELEM(BOOST_PP_TUPLE_ELEM(2,1,DATA),1,ATTRIBUTE))

#define BOOST_FUSION_DEFINE_STRUCT_COPY_MOVE_CTOR(MODIFIER,DATA)                \
    BOOST_PP_TUPLE_ELEM(3,0,DATA)(self_type MODIFIER other_self)                \
      : BOOST_PP_SEQ_FOR_EACH_I_R(                                              \
            1,                                                                  \
            BOOST_FUSION_DEFINE_STRUCT_COPY_MOVE_CTOR_FILLER_I,                 \
            (                                                                   \
                MODIFIER,                                                       \
                BOOST_PP_TUPLE_ELEM(3,2,DATA)                                   \
            ),                                                                  \
            BOOST_PP_TUPLE_ELEM(3,1,DATA))                                      \
    {}

#ifdef BOOST_NO_AUTO_DECLARATIONS
#   define BOOST_FUSION_DEFINE_STRUCT_ASSIGN_FILLER_I(                          \
        R, ATTRIBUTE_TUPEL_SIZE, I_, ATTRIBUTE)                                 \
                                                                                \
    BOOST_PP_EXPR_IF(                                                           \
        I_,                                                                     \
        typedef typename                                                        \
            boost::fusion::result_of::next<                                     \
                BOOST_PP_CAT(I,BOOST_PP_DEC(I_))&                               \
            >::type                                                             \
        BOOST_PP_CAT(I,I_);                                                     \
        BOOST_PP_CAT(I,I_) const BOOST_PP_CAT(i,I_)=                            \
            boost::fusion::next(BOOST_PP_CAT(i,BOOST_PP_DEC(I_)));)             \
                                                                                \
    BOOST_PP_TUPLE_ELEM(ATTRIBUTE_TUPEL_SIZE,1,ATTRIBUTE)=                      \
        boost::fusion::deref(BOOST_PP_CAT(i,I_));
#else
#   define BOOST_FUSION_DEFINE_STRUCT_ASSIGN_FILLER_I(                          \
        R, ATTRIBUTE_TUPEL_SIZE, I_, ATTRIBUTE)                                 \
                                                                                \
    BOOST_PP_EXPR_IF(                                                           \
        I_,                                                                     \
        auto const BOOST_PP_CAT(i,I_)=                                          \
            boost::fusion::next(BOOST_PP_CAT(i,BOOST_PP_DEC(I_)));)             \
                                                                                \
    BOOST_PP_TUPLE_ELEM(ATTRIBUTE_TUPEL_SIZE,1,ATTRIBUTE)=                      \
        boost::fusion::deref(BOOST_PP_CAT(i,I_));
#endif

#define BOOST_FUSION_DEFINE_STRUCT_ASSIGN_OP(                                   \
    ATTRIBUTES_SEQ, ATTRIBUTE_TUPEL_SIZE)                                       \
                                                                                \
    template<typename Seq>                                                      \
    self_type&                                                                  \
    operator=(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)                               \
    {                                                                           \
        BOOST_FUSION_STATIC_ASSERT((BOOST_PP_SEQ_SIZE(ATTRIBUTES_SEQ)==         \
            boost::fusion::result_of::size<Seq>::value));                       \
                                                                                \
        typedef typename                                                        \
            boost::fusion::result_of::begin<                                    \
                BOOST_FUSION_R_ELSE_CLREF(Seq)                                  \
            >::type                                                             \
        I0;                                                                     \
        I0 i0=boost::fusion::begin(BOOST_FUSION_FORWARD(Seq,seq));              \
                                                                                \
        BOOST_PP_SEQ_FOR_EACH_I_R(                                              \
            1,                                                                  \
            BOOST_FUSION_DEFINE_STRUCT_ASSIGN_FILLER_I,                         \
            ATTRIBUTE_TUPEL_SIZE,                                               \
            ATTRIBUTES_SEQ)                                                     \
                                                                                \
        return *this;                                                           \
    }

#define BOOST_FUSION_DEFINE_STRUCT_ATTR_I(R, ATTRIBUTE_TUPEL_SIZE, ATTRIBUTE)   \
                                                                                \
    BOOST_PP_TUPLE_ELEM(ATTRIBUTE_TUPEL_SIZE,0,ATTRIBUTE)                       \
        BOOST_PP_TUPLE_ELEM(ATTRIBUTE_TUPEL_SIZE,1,ATTRIBUTE);

#define BOOST_FUSION_DEFINE_STRUCT_SEQ_CTOR_FILLER_I(                           \
    R, DATA, I, ATTRIBUTE)                                                      \
                                                                                \
    BOOST_PP_COMMA_IF(I)                                                        \
    BOOST_PP_TUPLE_ELEM(BOOST_PP_TUPLE_ELEM(2,1,DATA),1,ATTRIBUTE)(             \
        boost::fusion::deref(boost::fusion::advance_c<I>(boost::fusion::begin(  \
            BOOST_PP_TUPLE_ELEM(2,0,DATA)))))

#define BOOST_FUSION_DEFINE_STRUCT_SEQ_ASSIGN_CTOR(MODIFIER, DATA)              \
                                                                                \
    template<typename SeqRef>                                                   \
    BOOST_PP_TUPLE_ELEM(3, 0, DATA)(                                            \
        boost::fusion::detail::sequence_assign_type<SeqRef> MODIFIER seq_assign)\
      : BOOST_PP_SEQ_FOR_EACH_I_R(                                              \
            1,                                                                  \
            BOOST_FUSION_DEFINE_STRUCT_SEQ_CTOR_FILLER_I,                       \
            (                                                                   \
                seq_assign.get(),                                               \
                BOOST_PP_TUPLE_ELEM(3, 2, DATA)                                 \
            ),                                                                  \
            BOOST_PP_TUPLE_ELEM(3, 1, DATA))                                    \
    {                                                                           \
        BOOST_FUSION_STATIC_ASSERT((                                            \
            BOOST_PP_SEQ_SIZE(BOOST_PP_TUPLE_ELEM(3, 1, DATA))==                \
                boost::fusion::result_of::size<SeqRef>::value));\
    }

#define BOOST_FUSION_DEFINE_STRUCT_SEQ_CTOR_DISABLER(                           \
    ATTRIBUTES_SEQ, ATTRIBUTE_TUPEL_SIZE)                                       \
                                                                                \
  , typename boost::disable_if<                                                 \
        boost::fusion::detail::is_explicitly_convertible<                       \
            BOOST_FUSION_R_ELSE_CLREF(Seq)                                      \
          , BOOST_PP_TUPLE_ELEM(                                                \
                ATTRIBUTE_TUPEL_SIZE,                                           \
                0,                                                              \
                BOOST_PP_SEQ_HEAD(ATTRIBUTES_SEQ))                              \
        >                                                                       \
    >::type* =0

#define BOOST_FUSION_DEFINE_STRUCT_SEQ_DEFAULT_CTOR_FILLER_I(                   \
    R, ATTRIBUTE_TUPEL_SIZE, I, ATTRIBUTE)                                      \
                                                                                \
    BOOST_PP_COMMA_IF(I)                                                        \
    BOOST_PP_TUPLE_ELEM(ATTRIBUTE_TUPEL_SIZE,1,ATTRIBUTE)()

#define BOOST_FUSION_DEFINE_STRUCT_IMPL_IMPL(                                   \
    NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPEL_SIZE)                                 \
                                                                                \
    BOOST_PP_SEQ_FOR_EACH_R(                                                    \
        1,                                                                      \
        BOOST_FUSION_DEFINE_STRUCT_ATTR_I,                                      \
        ATTRIBUTE_TUPEL_SIZE,                                                   \
        ATTRIBUTES_SEQ)                                                         \
                                                                                \
    BOOST_FUSION_CONSTEXPR                                                      \
    NAME()                                                                      \
      : BOOST_PP_SEQ_FOR_EACH_I_R(                                              \
            1,                                                                  \
            BOOST_FUSION_DEFINE_STRUCT_SEQ_DEFAULT_CTOR_FILLER_I,               \
            ATTRIBUTE_TUPEL_SIZE,                                               \
            ATTRIBUTES_SEQ)                                                     \
    {}                                                                          \
                                                                                \
    BOOST_FUSION_ALL_CTOR_COMBINATIONS(                                         \
        BOOST_FUSION_DEFINE_STRUCT_COPY_MOVE_CTOR,                              \
        (NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPEL_SIZE))                           \
                                                                                \
    template<typename Seq>                                                      \
    NAME(BOOST_FUSION_R_ELSE_CLREF(Seq) seq                                     \
        BOOST_PP_IF(                                                            \
            BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(ATTRIBUTES_SEQ)),                    \
            BOOST_PP_TUPLE_EAT(2),                                              \
            BOOST_FUSION_DEFINE_STRUCT_SEQ_CTOR_DISABLER)(                      \
                ATTRIBUTES_SEQ, ATTRIBUTE_TUPEL_SIZE)                           \
        )                                                                       \
      : BOOST_PP_SEQ_FOR_EACH_I_R(                                              \
            1,                                                                  \
            BOOST_FUSION_DEFINE_STRUCT_SEQ_CTOR_FILLER_I,                       \
            (                                                                   \
                BOOST_FUSION_FORWARD(Seq,seq),                                  \
                ATTRIBUTE_TUPEL_SIZE                                            \
            ),                                                                  \
            ATTRIBUTES_SEQ)                                                     \
    {                                                                           \
        BOOST_FUSION_STATIC_ASSERT((BOOST_PP_SEQ_SIZE(ATTRIBUTES_SEQ)==         \
            boost::fusion::result_of::size<Seq>::value));                       \
    }                                                                           \
                                                                                \
    BOOST_FUSION_ALL_CTOR_COMBINATIONS(                                         \
        BOOST_FUSION_DEFINE_STRUCT_SEQ_ASSIGN_CTOR,                             \
        (NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPEL_SIZE))                           \
                                                                                \
    BOOST_FUSION_DEFINE_STRUCT_ASSIGN_OP(ATTRIBUTES_SEQ, ATTRIBUTE_TUPEL_SIZE)

#ifndef BOOST_NO_VARIADIC_TEMPLATES
#   define BOOST_FUSION_DEFINE_STRUCT_CTOR_1(                                   \
        NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPEL_SIZE)                             \
                                                                                \
        template<typename Arg>                                                  \
        explicit                                                                \
        NAME(BOOST_FUSION_R_ELSE_CLREF(Arg) arg                                 \
          , typename boost::enable_if<                                          \
                boost::fusion::detail::is_explicitly_convertible<               \
                    BOOST_FUSION_R_ELSE_CLREF(Arg)                              \
                  , BOOST_PP_TUPLE_ELEM(                                        \
                      ATTRIBUTE_TUPEL_SIZE,0,BOOST_PP_SEQ_HEAD(ATTRIBUTES_SEQ)) \
                >                                                               \
            >::type* =0)                                                        \
          : BOOST_PP_TUPLE_ELEM(                                                \
                ATTRIBUTE_TUPEL_SIZE,1,BOOST_PP_SEQ_HEAD(ATTRIBUTES_SEQ))(      \
                    BOOST_FUSION_FORWARD(Arg,arg))                              \
        {}                                                                      \
                                                                                \
        template<typename Arg1, typename Arg2, typename... Args>                \
        NAME(BOOST_FUSION_R_ELSE_CLREF(Arg1) arg1                               \
          , BOOST_FUSION_R_ELSE_CLREF(Arg2) arg2                                \
          , BOOST_FUSION_R_ELSE_CLREF(Args)... args)                            \
          : BOOST_PP_TUPLE_ELEM(                                                \
                ATTRIBUTE_TUPEL_SIZE,1,BOOST_PP_SEQ_HEAD(ATTRIBUTES_SEQ))(      \
                    BOOST_FUSION_FORWARD(Arg1,arg1),                            \
                    BOOST_FUSION_FORWARD(Arg2,arg2),                            \
                    BOOST_FUSION_FORWARD(Args,args)...)                         \
        {}

#   define BOOST_FUSION_DEFINE_TPL_STRUCT_CTOR_1(                               \
        TEMPLATE_PARAMS_SEQ, NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPEL_SIZE)        \
                                                                                \
        BOOST_FUSION_DEFINE_STRUCT_CTOR_1(                                      \
            NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPEL_SIZE)
#elif defined(BOOST_NO_RVALUE_REFERENCES)
#   define BOOST_FUSION_DEFINE_STRUCT_CTOR_1(                                   \
        NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPEL_SIZE)                             \
                                                                                \
        explicit                                                                \
        NAME(boost::call_traits<                                                \
                BOOST_PP_TUPLE_ELEM(                                            \
                    ATTRIBUTE_TUPEL_SIZE,0,BOOST_PP_SEQ_HEAD(ATTRIBUTES_SEQ))   \
            >::param_type arg)                                                  \
          : BOOST_PP_TUPLE_ELEM(                                                \
                ATTRIBUTE_TUPEL_SIZE,1,BOOST_PP_SEQ_HEAD(ATTRIBUTES_SEQ))(arg)  \
        {}

#   define BOOST_FUSION_DEFINE_TPL_STRUCT_CTOR_1(                               \
        TEMPLATE_PARAMS_SEQ, NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPEL_SIZE)        \
                                                                                \
        explicit                                                                \
        NAME(typename boost::call_traits<                                       \
                typename boost::fusion::detail::get_first_arg<                  \
                    BOOST_PP_TUPLE_ELEM(                                        \
                        ATTRIBUTE_TUPEL_SIZE,                                   \
                        0,                                                      \
                        BOOST_PP_SEQ_HEAD(ATTRIBUTES_SEQ))                      \
                  , BOOST_PP_SEQ_HEAD(TEMPLATE_PARAMS_SEQ)                      \
                >::type                                                         \
            >::param_type arg)                                                  \
          : BOOST_PP_TUPLE_ELEM(                                                \
                ATTRIBUTE_TUPEL_SIZE,1,BOOST_PP_SEQ_HEAD(ATTRIBUTES_SEQ))(arg)  \
        {}
#else
#   define BOOST_FUSION_DEFINE_STRUCT_CTOR_1(                                   \
        NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPEL_SIZE)                             \
                                                                                \
        template<typename Arg>                                                  \
        explicit                                                                \
        NAME(Arg&& arg                                                          \
          , typename boost::enable_if<                                          \
                boost::fusion::detail::is_explicitly_convertible<               \
                    Arg&&                                                       \
                  , BOOST_PP_TUPLE_ELEM(                                        \
                      ATTRIBUTE_TUPEL_SIZE,0,BOOST_PP_SEQ_HEAD(ATTRIBUTES_SEQ)) \
                >                                                               \
            >::type* =0)                                                        \
          : BOOST_PP_TUPLE_ELEM(                                                \
                ATTRIBUTE_TUPEL_SIZE,1,BOOST_PP_SEQ_HEAD(ATTRIBUTES_SEQ))(      \
                    std::forward<Arg>(arg))                                     \
        {}

#   define BOOST_FUSION_DEFINE_TPL_STRUCT_CTOR_1(                               \
        TEMPLATE_PARAMS_SEQ, NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPEL_SIZE)        \
                                                                                \
        BOOST_FUSION_DEFINE_STRUCT_CTOR_1(                                      \
            NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPEL_SIZE)
#endif

#define BOOST_FUSION_DEFINE_STRUCT_CTOR_FILLER_I(                               \
    R, ATTRIBUTE_TUPEL_SIZE, I, ATTRIBUTE)                                      \
                                                                                \
    BOOST_PP_COMMA_IF(I)                                                        \
    BOOST_PP_TUPLE_ELEM(ATTRIBUTE_TUPEL_SIZE,1,ATTRIBUTE)(                      \
        BOOST_FUSION_FORWARD(BOOST_PP_CAT(A,I),BOOST_PP_CAT(_,I)))

#ifdef BOOST_NO_RVALUE_REFERENCES
#   define BOOST_FUSION_DEFINE_TPL_STRUCT_CTOR_ARG_I(R, DATA, I, ATTRIBUTE)     \
                                                                                \
        BOOST_PP_COMMA_IF(I)                                                    \
        typename boost::call_traits<                                            \
            typename boost::fusion::detail::get_first_arg<                      \
                BOOST_PP_TUPLE_ELEM(                                            \
                    BOOST_PP_TUPLE_ELEM(3,0,DATA),                              \
                    0,                                                          \
                    ATTRIBUTE)                                                  \
              , BOOST_PP_TUPLE_ELEM(3,2,DATA)                                   \
            >::type                                                             \
        >::param_type BOOST_PP_CAT(_,I)

#   define BOOST_FUSION_DEFINE_TPL_STRUCT_CTOR_N(                               \
        TEMPLATE_PARAMS_SEQ, NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPEL_SIZE)        \
                                                                                \
        NAME(BOOST_PP_SEQ_FOR_EACH_I_R(                                         \
                1,                                                              \
                BOOST_FUSION_DEFINE_TPL_STRUCT_CTOR_ARG_I,                      \
                (                                                               \
                    ATTRIBUTE_TUPEL_SIZE,                                       \
                    BOOST_PP_SEQ_SIZE(ATTRIBUTES_SEQ),                          \
                    BOOST_PP_SEQ_HEAD(TEMPLATE_PARAMS_SEQ)                      \
                ),                                                              \
                ATTRIBUTES_SEQ))                                                \
          : BOOST_PP_SEQ_FOR_EACH_I_R(                                          \
                1,                                                              \
                BOOST_FUSION_DEFINE_STRUCT_CTOR_FILLER_I,                       \
                ATTRIBUTE_TUPEL_SIZE,                                           \
                ATTRIBUTES_SEQ)                                                 \
        {}

#   define BOOST_FUSION_DEFINE_STRUCT_CTOR_ARG_I(                               \
        R, ATTRIBUTE_TUPEL_SIZE, I, ATTRIBUTE)                                  \
                                                                                \
        BOOST_PP_COMMA_IF(I)                                                    \
        boost::call_traits<                                                     \
            BOOST_PP_TUPLE_ELEM(ATTRIBUTE_TUPEL_SIZE,0,ATTRIBUTE)               \
        >::param_type BOOST_PP_CAT(_,I)

#   define BOOST_FUSION_DEFINE_STRUCT_CTOR_N(                                   \
        NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPEL_SIZE)                             \
                                                                                \
        NAME(BOOST_PP_SEQ_FOR_EACH_I_R(                                         \
                1,                                                              \
                BOOST_FUSION_DEFINE_STRUCT_CTOR_ARG_I,                          \
                ATTRIBUTE_TUPEL_SIZE,                                           \
                ATTRIBUTES_SEQ))                                                \
          : BOOST_PP_SEQ_FOR_EACH_I_R(                                          \
                1,                                                              \
                BOOST_FUSION_DEFINE_STRUCT_CTOR_FILLER_I,                       \
                ATTRIBUTE_TUPEL_SIZE,                                           \
                ATTRIBUTES_SEQ)                                                 \
        {}
#else
#   define BOOST_FUSION_DEFINE_STRUCT_CTOR_N(                                   \
        NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPEL_SIZE)                             \
                                                                                \
        template<                                                               \
            BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(ATTRIBUTES_SEQ), typename A) \
        >                                                                       \
        NAME(BOOST_PP_ENUM_BINARY_PARAMS(                                       \
                BOOST_PP_SEQ_SIZE(ATTRIBUTES_SEQ), A,&& _))                     \
          : BOOST_PP_SEQ_FOR_EACH_I_R(                                          \
                1,                                                              \
                BOOST_FUSION_DEFINE_STRUCT_CTOR_FILLER_I,                       \
                ATTRIBUTE_TUPEL_SIZE,                                           \
                ATTRIBUTES_SEQ)                                                 \
        {}

#   define BOOST_FUSION_DEFINE_TPL_STRUCT_CTOR_N(                               \
        TEMPLATE_PARAMS_SEQ, NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPEL_SIZE)        \
                                                                                \
        BOOST_FUSION_DEFINE_STRUCT_CTOR_N(                                      \
            NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPEL_SIZE)
#endif

#define BOOST_FUSION_DEFINE_STRUCT_CTOR(ATTRIBUTES_SEQ)                         \
    BOOST_PP_IF(BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(ATTRIBUTES_SEQ)),                \
        BOOST_FUSION_DEFINE_STRUCT_CTOR_N,                                      \
        BOOST_FUSION_DEFINE_STRUCT_CTOR_1)

#define BOOST_FUSION_DEFINE_TPL_STRUCT_CTOR(ATTRIBUTES_SEQ)                     \
    BOOST_PP_IF(BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(ATTRIBUTES_SEQ)),                \
        BOOST_FUSION_DEFINE_TPL_STRUCT_CTOR_N,                                  \
        BOOST_FUSION_DEFINE_TPL_STRUCT_CTOR_1)

#define BOOST_FUSION_DEFINE_STRUCT_IMPL(                                        \
    NAMESPACE_SEQ, NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPEL_SIZE)                  \
                                                                                \
    BOOST_FUSION_ADAPT_STRUCT_NAMESPACE_DEFINITION_BEGIN(NAMESPACE_SEQ)         \
                                                                                \
    struct NAME                                                                 \
    {                                                                           \
        typedef NAME self_type;                                                 \
                                                                                \
        BOOST_FUSION_DEFINE_STRUCT_IMPL_IMPL(                                   \
            NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPEL_SIZE)                         \
                                                                                \
        BOOST_FUSION_DEFINE_STRUCT_CTOR(ATTRIBUTES_SEQ)(                        \
            NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPEL_SIZE)                         \
    };                                                                          \
                                                                                \
    BOOST_FUSION_ADAPT_STRUCT_NAMESPACE_DEFINITION_END(NAMESPACE_SEQ)

#define BOOST_FUSION_DEFINE_TPL_STRUCT_IMPL(                                    \
    TEMPLATE_PARAMS_SEQ,                                                        \
    NAMESPACE_SEQ,                                                              \
    NAME,                                                                       \
    ATTRIBUTES_SEQ,                                                             \
    ATTRIBUTE_TUPEL_SIZE)                                                       \
                                                                                \
    BOOST_FUSION_ADAPT_STRUCT_NAMESPACE_DEFINITION_BEGIN(NAMESPACE_SEQ)         \
                                                                                \
    template<                                                                   \
        BOOST_FUSION_ADAPT_STRUCT_UNPACK_TEMPLATE_PARAMS_IMPL(                  \
            (0)TEMPLATE_PARAMS_SEQ)                                             \
    >                                                                           \
    struct NAME                                                                 \
    {                                                                           \
        typedef NAME self_type;                                                 \
                                                                                \
        BOOST_FUSION_DEFINE_STRUCT_IMPL_IMPL(                                   \
            NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPEL_SIZE)                         \
                                                                                \
        BOOST_FUSION_DEFINE_TPL_STRUCT_CTOR(ATTRIBUTES_SEQ)(                    \
            TEMPLATE_PARAMS_SEQ, NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPEL_SIZE)    \
    };                                                                          \
                                                                                \
    BOOST_FUSION_ADAPT_STRUCT_NAMESPACE_DEFINITION_END(NAMESPACE_SEQ)

namespace boost { namespace fusion { namespace detail
{
    template<typename A1, typename A2>
    struct get_first_arg
    {
        typedef A1 type;
    };
}}}

#endif
