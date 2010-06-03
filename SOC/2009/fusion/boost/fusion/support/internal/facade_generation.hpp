/*=============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_INTERNAL_FACADE_GENERATION_HPP
#define BOOST_FUSION_SUPPORT_INTERNAL_FACADE_GENERATION_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/if.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/tuple/eat.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/always.hpp>

#define BOOST_FUSION_DEFINE_SEQUENCE_FACADE_INTRINSIC_FUNC_WRAPPER(R, TAG, FUNC)\
    template<typename>                                                          \
    struct BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(3, 0, FUNC),_impl);                 \
                                                                                \
    template<>                                                                  \
    struct BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(3, 0, FUNC),_impl)<TAG>             \
    {                                                                           \
        template<                                                               \
            typename T                                                          \
            BOOST_PP_COMMA_IF(BOOST_PP_TUPLE_ELEM(3, 1, FUNC))                  \
            BOOST_PP_ENUM_PARAMS(                                               \
                BOOST_PP_TUPLE_ELEM(3, 1, FUNC), typename T)                    \
        >                                                                       \
        struct apply                                                            \
          : detail::remove_reference<T>::type::                                 \
                template BOOST_PP_TUPLE_ELEM(3, 0, FUNC)<                       \
                T                                                               \
                BOOST_PP_COMMA_IF(BOOST_PP_TUPLE_ELEM(3, 1, FUNC))              \
                BOOST_PP_ENUM_PARAMS(BOOST_PP_TUPLE_ELEM(3, 1, FUNC), T)        \
            >                                                                   \
        {};                                                                     \
    };

#define BOOST_FUSION_FACADE_DEFINE_INTRINSIC_FUNCS_WRAPPER(TAG, FUNCS)          \
namespace boost { namespace fusion                                              \
{                                                                               \
    struct TAG;                                                                 \
    struct void_;                                                               \
                                                                                \
    namespace extension                                                         \
    {                                                                           \
        BOOST_PP_SEQ_FOR_EACH(                                                  \
            BOOST_FUSION_DEFINE_SEQUENCE_FACADE_INTRINSIC_FUNC_WRAPPER,         \
            TAG,                                                                \
            FUNCS)                                                              \
    }                                                                           \
}}


#define BOOST_FUSION_DEFINE_SEQUENCE_FACADE_INTRINSIC_FUNC_FORDER(R, _, FUNC)   \
    template<                                                                   \
        typename T                                                              \
        BOOST_PP_COMMA_IF(BOOST_PP_TUPLE_ELEM(3, 1, FUNC))                      \
        BOOST_PP_ENUM_PARAMS(                                                   \
            BOOST_PP_TUPLE_ELEM(3, 1, FUNC), typename T)                        \
    >                                                                           \
    struct BOOST_PP_TUPLE_ELEM(3, 0, FUNC)                                      \
      : extension::BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(3, 0, FUNC),_impl)<         \
            typename mpl::apply1<mpl::always<void_>,T>::type                    \
        >::template apply<                                                      \
            T                                                                   \
            BOOST_PP_COMMA_IF(BOOST_PP_TUPLE_ELEM(3, 1, FUNC))                  \
            BOOST_PP_ENUM_PARAMS(BOOST_PP_TUPLE_ELEM(3, 1, FUNC), T)            \
        >                                                                       \
    {};

#define BOOST_FUSION_FACADE_DEFINE_INTRINSIC_FUNCS_FORWARDER(FUNCS)             \
    BOOST_PP_SEQ_FOR_EACH(                                                      \
        BOOST_FUSION_DEFINE_SEQUENCE_FACADE_INTRINSIC_FUNC_FORDER,              \
        _,                                                                      \
        FUNCS)

#endif
