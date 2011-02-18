
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#ifndef BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_HPP_
#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_HPP_

#include "param.hpp"
#include "../../abstract_function.hpp"
#include "../../symbol.hpp"
#include "../../preprocessor/sign/params_unbind.hpp"
#include "../../preprocessor/sign/params_const_bind.hpp"
#include "../../preprocessor/sign/params_bind.hpp"
#include "../../scope_exit/scope_exit.hpp" // Use this lib's ScopeExit impl.
#include "../../../config.hpp"
#include "../../../function.hpp"
#include <boost/type_traits.hpp>
#include <boost/detail/preprocessor/nilseq.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/logical/bitand.hpp>
#include <boost/preprocessor/logical/and.hpp>
#include <boost/preprocessor/logical/bitor.hpp>
#include <boost/preprocessor/logical/or.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/tuple/eat.hpp>

// PRIVATE //

// Expand to the function type `R (A1, ...)`.
#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_F_(sign_params, id) \
    BOOST_LOCAL_AUX_SYMBOL_RESULT_TYPE(id) \
    ( \
        BOOST_DETAIL_PP_NILSEQ_FOR_EACH_I( \
                BOOST_LOCAL_AUX_FUNCTION_CODE_PARAM_UNBIND_DECL, ~, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_UNBIND(sign_params)) \
    )

#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_CALL_(z, \
        sign_params, \
        unbind_params, \
        const_bind_names, has_const_bind_this, \
        bind_names, has_bind_this, \
        id) \
    BOOST_LOCAL_AUX_SYMBOL_RESULT_TYPE(id) \
    operator()( \
            BOOST_DETAIL_PP_NILSEQ_FOR_EACH_I( \
                BOOST_LOCAL_AUX_FUNCTION_CODE_PARAM_UNBIND_ARG_DECL, \
                ~, unbind_params) \
            ) { \
        /* just forward call to member function with local func name */ \
        return BOOST_LOCAL_AUX_SYMBOL_BODY_FUNCTION_NAME( \
            BOOST_DETAIL_PP_NILSEQ_FOR_EACH_I( \
                    BOOST_LOCAL_AUX_FUNCTION_CODE_PARAM_MAYBECONST_BIND, \
                    (id, 0 /* no offset */), \
                    const_bind_names) \
            /* pass plain binds */ \
            BOOST_PP_COMMA_IF(BOOST_PP_NOT( \
                    BOOST_DETAIL_PP_NILSEQ_IS_NIL(bind_names))) \
            BOOST_DETAIL_PP_NILSEQ_FOR_EACH_I( \
                    BOOST_LOCAL_AUX_FUNCTION_CODE_PARAM_MAYBECONST_BIND, \
                    (id \
                    /* offset param index of # of preceeding */ \
                    /* const-bind params (could be 0)*/ \
                    , BOOST_DETAIL_PP_NILSEQ_SIZE(const_bind_names) \
                    ), \
                    bind_names) \
            /* pass bind `this` */ \
            BOOST_PP_COMMA_IF(BOOST_PP_BITOR( \
                    has_const_bind_this, has_bind_this)) \
            BOOST_PP_EXPR_IIF( \
                    BOOST_PP_BITOR(has_const_bind_this, has_bind_this), \
                BOOST_LOCAL_AUX_SYMBOL_BINDS_VARIABLE_NAME-> \
                BOOST_LOCAL_AUX_FUNCTION_CODE_PARAM_THIS_NAME \
            ) \
            /* pass unbind params */ \
            BOOST_PP_COMMA_IF(BOOST_DETAIL_PP_NILSEQ_SIZE(unbind_params)) \
            BOOST_DETAIL_PP_NILSEQ_FOR_EACH_I( \
                    BOOST_LOCAL_AUX_FUNCTION_CODE_PARAM_UNBIND_ARG_NAME, \
                    ~, unbind_params) \
        ); \
    }

// Return unbind params but without last (default) params specified by count.
#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_UNBIND_REMOVE_LAST_N_(n, \
        unbind_params) \
    /* must use PP_SEQ instead of PP_NILSEQ so resulting seq is still nil */ \
    BOOST_PP_SEQ_FIRST_N(BOOST_PP_SUB(BOOST_PP_SEQ_SIZE( \
            unbind_params), n), unbind_params)

#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_CALL_FOR_DEFAULTS_(z, \
        n, params_unbind_constbind_hasconstthis_bind_hasthis_id) \
    BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_CALL_(z \
        , BOOST_PP_TUPLE_ELEM(7, 0, \
                params_unbind_constbind_hasconstthis_bind_hasthis_id) \
        /* remove last n default params */ \
        , BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_UNBIND_REMOVE_LAST_N_( \
                n, \
                BOOST_PP_TUPLE_ELEM(7, 1, \
                        params_unbind_constbind_hasconstthis_bind_hasthis_id)) \
        , BOOST_PP_TUPLE_ELEM(7, 2, \
                params_unbind_constbind_hasconstthis_bind_hasthis_id) \
        , BOOST_PP_TUPLE_ELEM(7, 3, \
                params_unbind_constbind_hasconstthis_bind_hasthis_id) \
        , BOOST_PP_TUPLE_ELEM(7, 4, \
                params_unbind_constbind_hasconstthis_bind_hasthis_id) \
        , BOOST_PP_TUPLE_ELEM(7, 5, \
                params_unbind_constbind_hasconstthis_bind_hasthis_id) \
        , BOOST_PP_TUPLE_ELEM(7, 6, \
                params_unbind_constbind_hasconstthis_bind_hasthis_id) \
    )

// Adapted from `BOOST_SCOPE_EXIT_AUX_IMPL()`.
#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_( \
        sign_params, \
        unbind_params, defaults_count, \
        const_bind_names, has_const_bind_this, \
        bind_names, has_bind_this, \
        id, typename_keyword) \
    class BOOST_LOCAL_AUX_SYMBOL_FUNCTOR_CLASS_NAME(id) :  \
            public ::boost::local::aux::abstract_function< \
                    BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_F_(sign_params, id), \
                    defaults_count> { \
        typedef BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_F_(sign_params, id) \
                BOOST_LOCAL_AUX_SYMBOL_FUNCTION_TYPE; \
    public: \
        /* constructor */ \
        explicit BOOST_LOCAL_AUX_SYMBOL_FUNCTOR_CLASS_NAME(id)(void* binds) \
            BOOST_PP_EXPR_IF(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_HAVE_ANY_BIND( \
                    sign_params), \
                /* member init (not a macro call) */ \
                : BOOST_LOCAL_AUX_SYMBOL_BINDS_VARIABLE_NAME(static_cast< \
                        BOOST_SCOPE_EXIT_AUX_PARAMS_T(id)*>(binds)) \
            ) \
        { \
            /* init needs func name so programmed later by `NAME` macro */ \
            BOOST_LOCAL_AUX_SYMBOL_INIT_RECURSION_FUNCTION_NAME(); \
        } \
        /* implement base functor `operator()` (and for all default params) */ \
        BOOST_PP_REPEAT( \
                /* PP_INC to handle no dflt (EXPAND for MVSC) */ \
                BOOST_PP_EXPAND(BOOST_PP_INC(defaults_count)), \
                BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_CALL_FOR_DEFAULTS_,\
                (sign_params, unbind_params, const_bind_names, \
                        has_const_bind_this, bind_names, has_bind_this, id)) \
    private: \
        /* this type symbol cannot have ID postfix because it is used */ \
        /* the `NAME` macro (because this symbol is within functor class */ \
        /* it doesn't have to have ID postfix). */ \
        typedef ::boost::local::function<BOOST_LOCAL_AUX_SYMBOL_FUNCTION_TYPE, \
                defaults_count> BOOST_LOCAL_AUX_SYMBOL_FUNCTOR_TYPE; \
        /* these types are qualified with extra eventual const and/or & if */ \
        /* their variables are bound by const and/or & (this is because */ \
        /* it is difficult strip the eventual & given that the var name is */ \
        /* alwasy attached to the & symbol plus programmers can always */ \
        /* remove const& using type traits) */ \
        /* const bind typeof types */ \
        BOOST_DETAIL_PP_NILSEQ_FOR_EACH_I( \
                BOOST_LOCAL_AUX_FUNCTION_CODE_PARAM_CONST_TYPEDEF, \
                (id, typename_keyword, 0 /* no offset */), \
                const_bind_names) \
        /* bind typeof types */ \
        BOOST_DETAIL_PP_NILSEQ_FOR_EACH_I( \
                BOOST_LOCAL_AUX_FUNCTION_CODE_PARAM_TYPEDEF, \
                ( id, typename_keyword, \
                  /* offset param index of # of preceeding */ \
                  /* const-bindparams (could be 0)*/ \
                  BOOST_DETAIL_PP_NILSEQ_SIZE(const_bind_names)),\
                bind_names) \
        /* this (const or not) bind type */ \
        BOOST_PP_EXPR_IIF(has_const_bind_this, \
            typeof \
            BOOST_LOCAL_AUX_SYMBOL_TYPEOF_TYPE( \
                typename_keyword ::contract::detail::add_pointed_const< \
                    BOOST_LOCAL_AUX_FUNCTION_CODE_PARAM_THIS_TYPE(id) \
                >::type this /* must not use `this_` for TYPEOF_TYPE */ \
            ) ; /* end typedef */ \
        ) \
        BOOST_PP_EXPR_IIF(has_bind_this, \
            typeof \
            BOOST_LOCAL_AUX_SYMBOL_TYPEOF_TYPE( \
                BOOST_LOCAL_AUX_FUNCTION_CODE_PARAM_THIS_TYPE(id) \
                this /* must not use `this_` for TYPEOF_TYPE */ \
            ) ; /* end typedef */ \
        ) \
        /* bind params member variable (note -- signle bind param cannot */ \
        /* be represented as single member variables because their names */ \
        /* might be prefixed by `&` so they are not know to pp) */ \
        BOOST_PP_EXPR_IIF(BOOST_PP_EXPAND( /* EXPAND for MSVC */ \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_HAVE_ANY_BIND(sign_params)), \
            BOOST_SCOPE_EXIT_AUX_PARAMS_T(id)* \
                    BOOST_LOCAL_AUX_SYMBOL_BINDS_VARIABLE_NAME; \
        ) \
        /* body function */ \
        BOOST_LOCAL_AUX_SYMBOL_RESULT_TYPE(id) \
        BOOST_LOCAL_AUX_SYMBOL_BODY_FUNCTION_NAME( \
                /* const binds */ \
                BOOST_DETAIL_PP_NILSEQ_FOR_EACH_I( \
                        BOOST_LOCAL_AUX_FUNCTION_CODE_PARAM_CONST_BIND_DECL, \
                        (id, typename_keyword, 0 /* no offset */), \
                        const_bind_names) \
                /* plain binds */ \
                BOOST_PP_COMMA_IF(BOOST_PP_NOT( \
                        BOOST_DETAIL_PP_NILSEQ_IS_NIL(bind_names))) \
                BOOST_DETAIL_PP_NILSEQ_FOR_EACH_I( \
                        BOOST_LOCAL_AUX_FUNCTION_CODE_PARAM_BIND_DECL, \
                        ( id, typename_keyword, \
                          /* offset param index of # of preceeding */ \
                          /* const-bindparams (could be 0)*/ \
                          BOOST_DETAIL_PP_NILSEQ_SIZE(const_bind_names) ),\
                        bind_names) \
                /* `this` bind */ \
                BOOST_PP_COMMA_IF(BOOST_PP_BITOR( \
                        has_const_bind_this, has_bind_this)) \
                BOOST_PP_EXPR_IIF(has_const_bind_this, \
                    typename_keyword ::contract::detail::add_pointed_const< \
                        BOOST_LOCAL_AUX_FUNCTION_CODE_PARAM_THIS_TYPE(id) \
                    >::type BOOST_LOCAL_CONFIG_THIS_PARAM_NAME \
                ) \
                BOOST_PP_EXPR_IIF(has_bind_this, \
                    BOOST_LOCAL_AUX_FUNCTION_CODE_PARAM_THIS_TYPE(id) \
                    BOOST_LOCAL_CONFIG_THIS_PARAM_NAME \
                ) \
                /* unbind params (last because they can have defaults) */ \
                BOOST_PP_COMMA_IF(BOOST_DETAIL_PP_NILSEQ_SIZE(unbind_params)) \
                BOOST_DETAIL_PP_NILSEQ_FOR_EACH_I( \
                        BOOST_LOCAL_AUX_FUNCTION_CODE_PARAM_UNBIND_DECL_WITH_DEFAULT, \
                        ~, unbind_params) \
            ) /* end body function params */ \
            /* user local function definition `{ ... }` will follow here */ \
    /* `NAME` macro will close function class decl `};` here */ 

// PUBLIC //

#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR( \
        sign_params, id, typename_keyword) \
    BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_(sign_params, \
            /* unbind params (might have defaults) */ \
            BOOST_LOCAL_AUX_PP_SIGN_PARAMS_UNBIND(sign_params), \
            BOOST_LOCAL_AUX_PP_SIGN_PARAMS_UNBIND_COUNT_DEFAULTS(sign_params), \
            /* const bind vars (without `this`) */ \
            BOOST_LOCAL_AUX_PP_SIGN_PARAMS_CONST_BIND(sign_params), \
            /* eventual const bind `this` */ \
            BOOST_LOCAL_AUX_PP_SIGN_PARAMS_HAVE_CONST_BIND_THIS(sign_params), \
            /* bind (not const) vars (without `this`) */ \
            BOOST_LOCAL_AUX_PP_SIGN_PARAMS_BIND(sign_params), \
            /* eventual bind (not const) `this` */ \
            BOOST_LOCAL_AUX_PP_SIGN_PARAMS_HAVE_BIND_THIS(sign_params), \
            /* etc */ \
            id, typename_keyword)

#endif // #include guard

