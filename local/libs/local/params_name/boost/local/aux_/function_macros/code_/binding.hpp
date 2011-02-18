
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#ifndef BOOST_LOCAL_AUX_FUNCTION_CODE_BINDING_HPP_
#define BOOST_LOCAL_AUX_FUNCTION_CODE_BINDING_HPP_

#include "param.hpp"
#include "../../symbol.hpp"
#include "../../scope_exit/scope_exit.hpp" // Use this lib's ScopeExit impl.
#include "../../preprocessor/sign/params_any_bind.hpp"
#include <boost/detail/preprocessor/nilseq.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/control/expr_iif.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/cat.hpp>

// Undefine local function bound args global variable. Actual declaration of
// this variable is made using SFINAE mechanisms by each local function macro.
extern boost::scope_exit::aux::undeclared
        BOOST_LOCAL_AUX_SYMBOL_ARGS_VARIABLE_NAME;

// PRIVATE //

// Adapted from `BOOST_SCOPE_EXIT_AUX_IMPL()`.
#define BOOST_LOCAL_AUX_FUNCTION_CODE_BINDING_WITH_TAGS_SEQ_( \
        all_bind_nilseq, has_any_bind_this, id, typename_keyword) \
    /* binding tags */ \
    BOOST_PP_EXPR_IIF(has_any_bind_this, \
        BOOST_SCOPE_EXIT_TYPEDEF_TYPEOF_THIS() \
        BOOST_LOCAL_AUX_FUNCTION_CODE_PARAM_THIS_TYPE(id); \
    ) \
    BOOST_DETAIL_PP_NILSEQ_FOR_EACH_I( \
            BOOST_LOCAL_AUX_FUNCTION_CODE_PARAM_TAG_DECL, \
            id, all_bind_nilseq) \
    BOOST_DETAIL_PP_NILSEQ_FOR_EACH_I(BOOST_SCOPE_EXIT_AUX_CAPTURE_DECL, \
            (id, typename_keyword), all_bind_nilseq) \
    /* binding class */ \
    struct BOOST_SCOPE_EXIT_AUX_PARAMS_T(id) { \
        BOOST_PP_EXPR_IIF(has_any_bind_this, \
            BOOST_LOCAL_AUX_FUNCTION_CODE_PARAM_THIS_TYPE(id) \
            BOOST_LOCAL_AUX_FUNCTION_CODE_PARAM_THIS_NAME; \
        ) \
        BOOST_DETAIL_PP_NILSEQ_FOR_EACH_I(BOOST_SCOPE_EXIT_AUX_PARAM_DECL, \
                (id, typename_keyword), all_bind_nilseq) \
        BOOST_DETAIL_PP_NILSEQ_FOR_EACH_I(BOOST_SCOPE_EXIT_AUX_MEMBER, \
                id, all_bind_nilseq) \
    } BOOST_LOCAL_AUX_SYMBOL_PARAMS_LOCAL_VARIABLE_NAME(id) = { \
        /* initialize the struct with param values to bind */ \
        BOOST_PP_EXPR_IIF(has_any_bind_this, this) \
        BOOST_PP_COMMA_IF(BOOST_PP_BITAND(has_any_bind_this, BOOST_PP_NOT( \
                BOOST_DETAIL_PP_NILSEQ_IS_NIL(all_bind_nilseq)))) \
        BOOST_DETAIL_PP_NILSEQ_FOR_EACH_I( \
                BOOST_SCOPE_EXIT_AUX_PARAM_INIT, id, all_bind_nilseq) \
    };

// Assume has some bind param.
#define BOOST_LOCAL_AUX_FUNCTION_CODE_BINDING_( \
        sign_params, id, typename_keyword) \
    /* has some bind param then all bind names is never empty nil-seq */ \
    BOOST_LOCAL_AUX_FUNCTION_CODE_BINDING_WITH_TAGS_SEQ_( \
            BOOST_LOCAL_AUX_PP_SIGN_PARAMS_ALL_BIND(sign_params), \
            BOOST_LOCAL_AUX_PP_SIGN_PARAMS_HAVE_ANY_BIND_THIS(sign_params), \
            id, typename_keyword)

// PUBLIC //

#define BOOST_LOCAL_AUX_FUNCTION_CODE_BINDING( \
        sign_params, id, typename_keyword) \
    /* deduce bound type and stores their values/references */ \
    BOOST_PP_IIF(BOOST_PP_EXPAND( /* expand needed on MSVC */ \
            BOOST_LOCAL_AUX_PP_SIGN_PARAMS_HAVE_ANY_BIND(sign_params)), \
        BOOST_LOCAL_AUX_FUNCTION_CODE_BINDING_ \
    , \
        BOOST_PP_TUPLE_EAT(3) \
    )(sign_params, id, typename_keyword) \
    /* this `declared<>` must be present also when no bind param specified */ \
    /* because `...args.value` is ued by `NAME` macro (to constructor */ \
    /* functor) without knowing if binds were specified or not */ \
    boost::scope_exit::aux::declared< boost::scope_exit::aux::resolve< \
        /* cannot prefix with `::` as in `sizeof(:: ...` because the name */ \
        /* must refer to the local variable name to allow multiple local */ \
        /* functions (and exits) within the same scope (however this */ \
        /* does not allow for nesting because local variables cannot be */ \
        /* used in nested code blocks) */ \
        sizeof(BOOST_LOCAL_AUX_SYMBOL_ARGS_VARIABLE_NAME) \
    >::cmp1<0>::cmp2 > BOOST_LOCAL_AUX_SYMBOL_ARGS_VARIABLE_NAME; \
    /* stores bound types/values into `...args` variable (args variable */ \
    /* can be accessed by `NAME` macro because doesn't use __LINE__ id) */ \
    BOOST_PP_EXPR_IIF(BOOST_PP_EXPAND( /* expand needed on MSVC */ \
            BOOST_LOCAL_AUX_PP_SIGN_PARAMS_HAVE_ANY_BIND(sign_params)), \
        BOOST_LOCAL_AUX_SYMBOL_ARGS_VARIABLE_NAME.value = \
                &BOOST_LOCAL_AUX_SYMBOL_PARAMS_LOCAL_VARIABLE_NAME(id); \
    )

#endif // #include guard

