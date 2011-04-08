
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_HPP_
#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_HPP_

#include "bind_this.hpp"
#include "../../abstract_function.hpp"
#include "../../symbol.hpp"
#include "../../preprocessor/sign/params_unbind.hpp"
#include "../../preprocessor/sign/params_const_bind.hpp"
#include "../../preprocessor/sign/params_bind.hpp"
#include "../../scope_exit/scope_exit.hpp" // Use this lib's ScopeExit impl.
#include "../../../config.hpp"
#include "../../../function.hpp"
#include <boost/detail/preprocessor/keyword/auto.hpp>
#include <boost/detail/preprocessor/keyword/register.hpp>
#include <boost/type_traits.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/logical/bitand.hpp>
#include <boost/preprocessor/logical/bitand.hpp>
#include <boost/preprocessor/logical/bitor.hpp>
#include <boost/preprocessor/logical/or.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/control/expr_iif.hpp>
#include <boost/preprocessor/tuple/eat.hpp>
#include <boost/preprocessor/list/adt.hpp> // For `IS_CONS`.
#include <boost/preprocessor/list/for_each_i.hpp>

// PRIVATE //

// Unbind parameters.

// i: 1 for 1st param, 2 for 2nd, ... (start from 1 not 0).
#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_UNBIND_ARG_NAME_(i) \
    /* this must be a generic parameter name because unbind type and name */ \
    /* are not separate tokens in the macro syntax so name is not available */ \
    /* separately from its type */ \
    BOOST_PP_CAT(arg, i)

// i: 1 for 1st param, 2 for 2nd, ... (start from 1 not 0).
#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_UNBIND_ARG_TYPE_(i) \
    /* the parameter type must be accessed using function traits from */ \
    /* function type because it is not available to the macro syntax */ \
    /* separately from the parameter name */ \
    BOOST_PP_CAT(BOOST_PP_CAT(::boost::function_traits< \
            BOOST_LOCAL_AUX_SYMBOL_FUNCTION_TYPE>::arg, i), _type) \

#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_UNBIND_ARG_DECL_( \
        r, typename_keyword, i, param) \
    BOOST_PP_COMMA_IF(i) \
    typename_keyword \
    ::boost::call_traits< \
            BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_UNBIND_ARG_TYPE_( \
            BOOST_PP_INC(i))>::param_type \
    BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_UNBIND_ARG_NAME_(BOOST_PP_INC(i))

#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_UNBIND_ARG_NAME_ENUM_( \
        r, unused, i, param) \
    BOOST_PP_COMMA_IF(i) /* enumeration commas */ \
    BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_UNBIND_ARG_NAME_(BOOST_PP_INC(i))

#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_UNBIND_DECL_( \
        r, unused, i, param) \
    BOOST_PP_COMMA_IF(i) \
    BOOST_DETAIL_PP_KEYWORD_AUTO_REMOVE_BACK( \
        BOOST_DETAIL_PP_KEYWORD_REGISTER_REMOVE_BACK( \
            BOOST_LOCAL_AUX_PP_SIGN_PARAMS_UNBIND_PARAM_DECL(param) \
        ) \
    )

#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_UNBIND_DECL_WITH_DEFAULT_( \
        r, unused, i, param) \
    BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_UNBIND_DECL_(z, unused, i, param) \
    BOOST_PP_IIF(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_UNBIND_PARAM_HAS_DEFAULT( \
            param), \
        = BOOST_LOCAL_AUX_PP_SIGN_PARAMS_UNBIND_PARAM_DEFAULT \
    , \
        BOOST_PP_TUPLE_EAT(1) \
    )(param)

// Bound parameters.

#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_MAYBECONST_BIND_TYPE_( \
        id_typename_offset, i, var) \
    BOOST_SCOPE_EXIT_AUX_PARAMS_T( \
            BOOST_PP_TUPLE_ELEM(3, 0, id_typename_offset)):: \
    BOOST_SCOPE_EXIT_AUX_PARAM_T( \
            BOOST_PP_TUPLE_ELEM(3, 0, id_typename_offset), \
            BOOST_PP_ADD(i, BOOST_PP_TUPLE_ELEM(3, 2, id_typename_offset)),\
            var)

#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_MAYBECONST_BIND_VALUE_( \
        id_offset, i, var) \
    BOOST_LOCAL_AUX_SYMBOL_BINDS_VARIABLE_NAME-> \
            BOOST_SCOPE_EXIT_AUX_PARAM( \
                    BOOST_PP_TUPLE_ELEM(2, 0, id_offset), \
                    BOOST_PP_ADD(i, BOOST_PP_TUPLE_ELEM(2, 1, id_offset)), \
                    var).value

// Adapted from `BOOST_SCOPE_EXIT_AUX_ARG_DECL()`.
#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_MAYBECONST_BIND_DECL_( \
        r, id_typename_offset, i, var, is_const) \
    BOOST_PP_TUPLE_ELEM(3, 1, id_typename_offset) /* eventual typename */ \
    BOOST_PP_EXPR_IF(is_const, \
        BOOST_PP_IIF( \
                BOOST_DETAIL_PP_KEYWORD_IS_THIS_BACK(var), \
            ::boost::local::aux::add_pointed_const< /* pointed obj const */ \
        , \
            ::boost::add_const< /* outer type const */ \
        ) \
        BOOST_PP_TUPLE_ELEM(3, 1, id_typename_offset) /* eventual typename */ \
    ) \
    BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_MAYBECONST_BIND_TYPE_( \
            id_typename_offset, i, var) \
    BOOST_PP_EXPR_IF(is_const, >::type) \
    BOOST_LOCAL_AUX_FUNCTION_CODE_BIND_THIS_RENAME(var)

// Adapted from `BOOST_SCOPE_EXIT_AUX_ARG_DECL()`.
#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_MAYBECONST_BIND_DECL_ENUM_( \
        r, id_typename_offset, i, var, is_const) \
    BOOST_PP_COMMA_IF(i) /* enumeration commas */ \
    BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_MAYBECONST_BIND_DECL_( \
            r, id_typename_offset, i, var, is_const)
    
// Adapted from `BOOST_SCOPE_EXIT_AUX_ARG()`.
#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_MAYBECONST_BIND_( \
        r, id_offset, i, var) \
    BOOST_PP_COMMA_IF(i) \
    BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_MAYBECONST_BIND_VALUE_( \
            id_offset, i, var)

#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_BIND_DECL_( \
        r, id_typename_offset, i, var) \
    BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_MAYBECONST_BIND_DECL_ENUM_( \
            r, id_typename_offset, i, var, 0 /* do not force const */)

#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_CONST_BIND_DECL_( \
        r, id_typename_offset, i, var) \
    BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_MAYBECONST_BIND_DECL_ENUM_( \
            r, id_typename_offset, i, var, 1 /* force const */)

// Typeof type-definitions.

#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_MAYBECONST_TYPEDEF_( \
        r, id_typename_offset, i, var, is_const) \
    typedef \
    /* the type with the special typeof name */ \
    BOOST_LOCAL_AUX_SYMBOL_TYPEOF_TYPE( \
        BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_MAYBECONST_BIND_DECL_( \
                r, id_typename_offset, i, var, is_const) \
    ) ; /* end typedef */

#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_TYPEDEF_( \
        r, id_typename_offset, i, var) \
    BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_MAYBECONST_TYPEDEF_( \
            r, id_typename_offset, i, var, 0 /* do not add const */)

#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_CONST_TYPEDEF_( \
        r, id_typename_offset, i, var) \
    BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_MAYBECONST_TYPEDEF_( \
            r, id_typename_offset, i, var, 1 /* add const */)

// Template parameter function type `F`.

// Expand to the function type `R (A1, ...)`.
#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_F_( \
        sign_params, id, has_type, type_name) \
    BOOST_LOCAL_AUX_SYMBOL_RESULT_TYPE(id) \
    BOOST_PP_EXPR_IIF(has_type, (type_name) ) \
    ( \
        BOOST_PP_LIST_FOR_EACH_I( \
                BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_UNBIND_DECL_, ~, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_UNBIND(sign_params)) \
    )

// Functor `operator()`.

#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_CALL_(z, \
        sign_params, \
        unbinds, \
        const_binds, has_const_bind_this, \
        binds, has_bind_this, \
        id, typename_keyword) \
    BOOST_LOCAL_AUX_SYMBOL_RESULT_TYPE(id) \
    operator()( \
            BOOST_PP_LIST_FOR_EACH_I( \
                    BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_UNBIND_ARG_DECL_, \
                    typename_keyword, unbinds) \
            ) { \
        /* just forward call to member function with local func name */ \
        return BOOST_LOCAL_AUX_SYMBOL_BODY_FUNCTION_NAME( \
            BOOST_PP_LIST_FOR_EACH_I( \
                    BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_MAYBECONST_BIND_, \
                    (id, 0 /* no offset */), \
                    const_binds) \
            /* pass plain binds */ \
            BOOST_PP_COMMA_IF( \
                BOOST_PP_BITAND( \
                      BOOST_PP_LIST_IS_CONS(const_binds) \
                    , BOOST_PP_LIST_IS_CONS(binds) \
                ) \
            ) \
            BOOST_PP_LIST_FOR_EACH_I( \
                    BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_MAYBECONST_BIND_, \
                    (id \
                    /* offset param index of # of preceeding */ \
                    /* const-bind params (could be 0)*/ \
                    , BOOST_PP_LIST_SIZE(const_binds) \
                    ), \
                    binds) \
            /* pass bind `this` */ \
            BOOST_PP_COMMA_IF( \
                BOOST_PP_BITAND( \
                      BOOST_PP_BITOR( \
                          BOOST_PP_LIST_IS_CONS(const_binds) \
                        , BOOST_PP_LIST_IS_CONS(binds) \
                      ) \
                    , BOOST_PP_BITOR(has_const_bind_this, has_bind_this) \
                ) \
            ) \
            BOOST_PP_EXPR_IIF( \
                    BOOST_PP_BITOR(has_const_bind_this, has_bind_this), \
                BOOST_LOCAL_AUX_SYMBOL_BINDS_VARIABLE_NAME-> \
                BOOST_LOCAL_AUX_FUNCTION_CODE_BIND_THIS_NAME \
            ) \
            /* pass unbind params */ \
            BOOST_PP_COMMA_IF( \
                BOOST_PP_BITAND( \
                      BOOST_PP_BITOR( \
                          BOOST_PP_BITOR( \
                              BOOST_PP_LIST_IS_CONS(const_binds) \
                            , BOOST_PP_LIST_IS_CONS(binds) \
                          ) \
                        , BOOST_PP_BITOR(has_const_bind_this, has_bind_this) \
                      ) \
                    , BOOST_PP_LIST_IS_CONS(unbinds) \
                ) \
            ) \
            BOOST_PP_LIST_FOR_EACH_I( \
                    BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_UNBIND_ARG_NAME_ENUM_, \
                    ~, unbinds) \
        ); \
    }

// Return unbind params but without last (default) params specified by count.
#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_UNBIND_REMOVE_LAST_N_(n, \
        unbinds) \
    BOOST_PP_LIST_FIRST_N(BOOST_PP_SUB(BOOST_PP_LIST_SIZE(unbinds), n), \
            unbinds)

#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_CALL_FOR_DEFAULTS_(z, \
        n, params_unbinds_constbinds_hasconstthis_binds_hasthis_id_typename) \
    BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_CALL_(z \
        , BOOST_PP_TUPLE_ELEM(8, 0, \
            params_unbinds_constbinds_hasconstthis_binds_hasthis_id_typename) \
        /* remove last n default params */ \
        , BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_UNBIND_REMOVE_LAST_N_(n, \
            BOOST_PP_TUPLE_ELEM(8, 1, \
            params_unbinds_constbinds_hasconstthis_binds_hasthis_id_typename) \
        ) \
        , BOOST_PP_TUPLE_ELEM(8, 2, \
            params_unbinds_constbinds_hasconstthis_binds_hasthis_id_typename) \
        , BOOST_PP_TUPLE_ELEM(8, 3, \
            params_unbinds_constbinds_hasconstthis_binds_hasthis_id_typename) \
        , BOOST_PP_TUPLE_ELEM(8, 4, \
            params_unbinds_constbinds_hasconstthis_binds_hasthis_id_typename) \
        , BOOST_PP_TUPLE_ELEM(8, 5, \
            params_unbinds_constbinds_hasconstthis_binds_hasthis_id_typename) \
        , BOOST_PP_TUPLE_ELEM(8, 6, \
            params_unbinds_constbinds_hasconstthis_binds_hasthis_id_typename) \
        , BOOST_PP_TUPLE_ELEM(8, 7, \
            params_unbinds_constbinds_hasconstthis_binds_hasthis_id_typename) \
    )

// Functor class.

// Adapted from `BOOST_SCOPE_EXIT_AUX_IMPL()`.
#define BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_(sign_params, \
        unbinds, defaults_count, \
        const_binds, has_const_bind_this, \
        binds, has_bind_this, \
        id, typename_keyword) \
    class BOOST_LOCAL_AUX_SYMBOL_FUNCTOR_CLASS_NAME(id) :  \
            public ::boost::local::aux::abstract_function< \
                    BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_F_( \
                            sign_params, id, 0 /* no type */, ~), \
                    defaults_count> { \
        typedef BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_F_(sign_params, id, \
                1 /* has type */, BOOST_LOCAL_AUX_SYMBOL_FUNCTION_TYPE); \
    public: \
        /* constructor */ \
        explicit BOOST_LOCAL_AUX_SYMBOL_FUNCTOR_CLASS_NAME(id)( \
                void* binding_data) \
            BOOST_PP_EXPR_IIF(BOOST_PP_EXPAND( /* expand for MSVC */ \
                    BOOST_LOCAL_AUX_PP_SIGN_PARAMS_HAVE_ANY_BIND(sign_params)),\
                /* member init (not a macro call) */ \
                : BOOST_LOCAL_AUX_SYMBOL_BINDS_VARIABLE_NAME(static_cast< \
                        BOOST_SCOPE_EXIT_AUX_PARAMS_T(id)*>(binding_data)) \
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
                (sign_params, unbinds, const_binds, has_const_bind_this, \
                 binds, has_bind_this, id, typename_keyword) ) \
    private: \
        /* this type symbol cannot have ID postfix because it is used */ \
        /* the `NAME` macro (because this symbol is within functor class */ \
        /* it doesn't have to have ID postfix). */ \
        typedef ::boost::local::function<BOOST_LOCAL_AUX_SYMBOL_FUNCTION_TYPE, \
                defaults_count> BOOST_LOCAL_AUX_SYMBOL_FUNCTOR_TYPE; \
        /* these types are qualified with extra eventual const and/or & if */ \
        /* their variables are bound by const and/or & (this is because */ \
        /* it is difficult strip the eventual & given that the var name is */ \
        /* always attached to the & symbol plus programmers can always */ \
        /* remove const& using type traits) */ \
        /* const bind typeof types */ \
        BOOST_PP_LIST_FOR_EACH_I( \
                BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_CONST_TYPEDEF_, \
                (id, typename_keyword, 0 /* no offset */), \
                const_binds) \
        /* bind typeof types */ \
        BOOST_PP_LIST_FOR_EACH_I( \
                BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_TYPEDEF_, \
                ( id, typename_keyword, \
                  /* offset param index of # of preceeding */ \
                  /* const-bindparams (could be 0)*/ \
                  BOOST_PP_LIST_SIZE(const_binds)),\
                binds) \
        /* this (const or not) bind type */ \
        BOOST_PP_EXPR_IIF(has_const_bind_this, \
            typedef \
            BOOST_LOCAL_AUX_SYMBOL_TYPEOF_TYPE( \
                typename_keyword ::boost::local::aux::add_pointed_const< \
                    BOOST_LOCAL_AUX_FUNCTION_CODE_BIND_THIS_TYPE(id) \
                >::type this /* must not use `this_` for TYPEOF_TYPE */ \
            ) ; /* end typedef */ \
        ) \
        BOOST_PP_EXPR_IIF(has_bind_this, \
            typedef \
            BOOST_LOCAL_AUX_SYMBOL_TYPEOF_TYPE( \
                BOOST_LOCAL_AUX_FUNCTION_CODE_BIND_THIS_TYPE(id) \
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
        /* this allows for nesting (local functions, blocks, and exits) as */ \
        /* it makes the args variable visible within the body code (which */ \
        /* cannot be static); this is for compilation only as the args */ \
        /* variable is actually declared by the 1st enclosing local func */ \
        boost::scope_exit::aux::undeclared \
                BOOST_LOCAL_AUX_SYMBOL_ARGS_VARIABLE_NAME; \
        /* body function (unfortunately, cannot be static to allow access */ \
        /* to member var with local function name for recursion but doing */ \
        /* so also allows the body to misuse `this` instead of `this_`) */ \
        BOOST_LOCAL_AUX_SYMBOL_RESULT_TYPE(id) \
        BOOST_LOCAL_AUX_SYMBOL_BODY_FUNCTION_NAME( \
                /* const binds */ \
                BOOST_PP_LIST_FOR_EACH_I( \
                        BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_CONST_BIND_DECL_,\
                        (id, typename_keyword, 0 /* no offset */), \
                        const_binds) \
                /* plain binds */ \
                BOOST_PP_COMMA_IF( \
                    BOOST_PP_BITAND( \
                          BOOST_PP_LIST_IS_CONS(const_binds) \
                        , BOOST_PP_LIST_IS_CONS(binds) \
                    ) \
                ) \
                BOOST_PP_LIST_FOR_EACH_I( \
                        BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_BIND_DECL_, \
                        ( id, typename_keyword, \
                          /* offset param index of # of preceeding */ \
                          /* const-bindparams (could be 0)*/ \
                          BOOST_PP_LIST_SIZE(const_binds) ),\
                        binds) \
                /* `this` bind */ \
                BOOST_PP_COMMA_IF( \
                    BOOST_PP_BITAND( \
                          BOOST_PP_BITOR( \
                              BOOST_PP_LIST_IS_CONS(const_binds) \
                            , BOOST_PP_LIST_IS_CONS(binds) \
                          ) \
                        , BOOST_PP_BITOR(has_const_bind_this, has_bind_this) \
                    ) \
                ) \
                BOOST_PP_EXPR_IIF(has_const_bind_this, \
                    typename_keyword ::boost::local::aux::add_pointed_const< \
                        BOOST_LOCAL_AUX_FUNCTION_CODE_BIND_THIS_TYPE(id) \
                    >::type BOOST_LOCAL_CONFIG_THIS_PARAM_NAME \
                ) \
                BOOST_PP_EXPR_IIF(has_bind_this, \
                    BOOST_LOCAL_AUX_FUNCTION_CODE_BIND_THIS_TYPE(id) \
                    BOOST_LOCAL_CONFIG_THIS_PARAM_NAME \
                ) \
                /* unbind params (last because they can have defaults) */ \
                BOOST_PP_COMMA_IF( \
                    BOOST_PP_BITAND( \
                          BOOST_PP_BITOR( \
                              BOOST_PP_BITOR( \
                                  BOOST_PP_LIST_IS_CONS(const_binds) \
                                , BOOST_PP_LIST_IS_CONS(binds) \
                              ) \
                            , BOOST_PP_BITOR(has_const_bind_this, \
                                    has_bind_this) \
                          ) \
                        , BOOST_PP_LIST_IS_CONS(unbinds) \
                    ) \
                ) \
                BOOST_PP_LIST_FOR_EACH_I( \
                        BOOST_LOCAL_AUX_FUNCTION_CODE_FUNCTOR_UNBIND_DECL_WITH_DEFAULT_, \
                        ~, unbinds) \
            ) /* end body function params */ \
            /* const member func so it cannot change obj (reassign member */ \
            /* var with local function name, etc) */ \
            const \
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

