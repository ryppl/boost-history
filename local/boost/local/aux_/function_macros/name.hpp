
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef BOOST_LOCAL_AUX_FUNCTION_NAME_HPP_
#define BOOST_LOCAL_AUX_FUNCTION_NAME_HPP_

#include "../preprocessor/keyword/recursive.hpp"
#include "../symbol.hpp"
#include "../config.hpp"
// For BOOST_TYPEOF.
#include "../scope_exit/scope_exit.hpp" // Use this lib's ScopeExit impl.
#include <boost/detail/preprocessor/keyword/inline.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/control/expr_iif.hpp>
#include <boost/preprocessor/logical/bitor.hpp>

// PRIVATE //

#define BOOST_LOCAL_AUX_FUNCTION_NAME_INIT_RECURSION_FUNC_ \
    BOOST_LOCAL_AUX_INTERNAL_SYMBOL(init_recursion)

#define BOOST_LOCAL_AUX_FUNCTION_NAME_RECURSIVE_FUNC_( \
        is_recursive, local_function_name) \
    BOOST_PP_IIF(is_recursive, \
        local_function_name \
    , \
        BOOST_LOCAL_AUX_INTERNAL_SYMBOL(nonrecursive_local_function_name) \
    )

#define BOOST_LOCAL_AUX_FUNCTION_NAME_END_LOCAL_FUNCTOR_(id, \
        local_function_name, is_recursive, \
        local_functor_name, nonlocal_functor_name) \
    /* `PARAMS() { ... }` expandsion here -- still within functor class */ \
    /* class functor ## __LINE__ { ... */ \
    public: \
        /* member var with function name for recursive calls; must be */ \
        /* `public` because is it also used by this macro but outside */ \
        /* the functor class to deduce the functor type; it cannot be */ \
        /* `const` because it is init after construction (because */ \
        /* constructor doesn't know local function name) */ \
        /* run-time: even when optimizing, recursive calls cannot be */ \
        /* optimized (i.e., they must be via the non-local functor) */ \
        /* because this cannot be a mem ref because its name is not known */ \
        /* by the constructor so it cannot be set by the mem init list */ \
        BOOST_LOCAL_AUX_SYMBOL_FUNCTOR_TYPE \
                BOOST_LOCAL_AUX_FUNCTION_NAME_RECURSIVE_FUNC_(is_recursive, \
                        local_function_name); \
        BOOST_PP_EXPR_IIF(is_recursive, \
            /* run-time: the `init_recursion()` function cannot be called */ \
            /* by the constructor to allow for compiler optimization */ \
            /* (inlining) so it must be public */ \
            inline void BOOST_LOCAL_AUX_FUNCTION_NAME_INIT_RECURSION_FUNC_( \
                    BOOST_LOCAL_AUX_SYMBOL_FUNCTOR_TYPE& functor) { \
                local_function_name = functor; \
            } \
        ) \
    /* local functor can be passed as tparam only on C++03 (faster) */ \
    } local_functor_name(BOOST_LOCAL_AUX_SYMBOL_ARGS_VARIABLE_NAME.value); \
    /* non-local functor can always be passed as tparam (but slower) */ \
    BOOST_TYPEOF(local_functor_name. \
            BOOST_LOCAL_AUX_FUNCTION_NAME_RECURSIVE_FUNC_(is_recursive, \
                    local_function_name)) \
            nonlocal_functor_name; \
    /* the order of the following 2 function calls cannot be changed */ \
    /* because init_recursion uses the local_functor so the local_functor */ \
    /* must be init first */ \
    local_functor_name.BOOST_LOCAL_AUX_SYMBOL_INIT_CALL_FUNCTION_NAME( \
            &local_functor_name, nonlocal_functor_name); \
    BOOST_PP_EXPR_IIF(is_recursive, \
        /* init recursion causes MSVC to not optimize local function not */ \
        /* even when local functor is used as template parameter so no */ \
        /* recursion unless all inlining optimizations are specified off */ \
        local_functor_name.BOOST_LOCAL_AUX_FUNCTION_NAME_INIT_RECURSION_FUNC_( \
                nonlocal_functor_name); \
    )

#define BOOST_LOCAL_AUX_FUNCTION_NAME_FUNCTOR_(local_function_name) \
    BOOST_LOCAL_AUX_INTERNAL_SYMBOL(local_function_name)

// This can always be passed as a template parameters (on all compilers).
// However, it is slower because it cannot be inlined.
// Passed at tparam: Yes (on all C++). Inlineable: No. Recursive: No.
#define BOOST_LOCAL_AUX_FUNCTION_NAME_(local_function_name) \
    BOOST_LOCAL_AUX_FUNCTION_NAME_END_LOCAL_FUNCTOR_(__LINE__, \
            local_function_name, \
            /* local function is not recursive (because recursion and its */ \
            /* initialization cannot be inlined even on C++03, */ \
            /* so this allows optimization at least on C++03) */ \
            0 /* not recursive */ , \
            /* local functor */ \
            BOOST_LOCAL_AUX_FUNCTION_NAME_FUNCTOR_(local_function_name), \
            /* local function declared as non-local functor -- but it can */ \
            /* be inlined only by C++03 and it cannot be recursive */ \
            local_function_name)

// This is faster on some compilers but not all (e.g., it is faster on GCC
// because its optimization inlines it but not on MSVC). However, it cannot be
// passed as a template parameter on non C++03 compilers.
// Passed at tparam: Only on C++03. Inlineable: Yes. Recursive: No.
#define BOOST_LOCAL_AUX_FUNCTION_NAME_INLINE_(local_function_name) \
    BOOST_LOCAL_AUX_FUNCTION_NAME_END_LOCAL_FUNCTOR_(__LINE__, \
            local_function_name, \
            /* inlined local function is never recursive (because recursion */ \
            /* and its initialization cannot be inlined)*/ \
            0 /* not recursive */ , \
            /* inlined local function declared as local functor (maybe */ \
            /* inlined even by non C++03 -- but it can be passed as */ \
            /* template parameter only on C++03 */ \
            local_function_name, \
            /* non-local functor */ \
            BOOST_LOCAL_AUX_FUNCTION_NAME_FUNCTOR_(local_function_name))

// This is slower on all compilers (C++03 and non) because recursion and its
// initialization can never be inlined.
// Passed at tparam: Yes. Inlineable: No. Recursive: Yes.
#define BOOST_LOCAL_AUX_FUNCTION_NAME_RECURSIVE_(local_function_name) \
    BOOST_LOCAL_AUX_FUNCTION_NAME_END_LOCAL_FUNCTOR_(__LINE__, \
            local_function_name, \
            /* recursive local function -- but it cannot be inlined */ \
            1 /* recursive */ , \
            /* local functor */ \
            BOOST_LOCAL_AUX_FUNCTION_NAME_FUNCTOR_(local_function_name), \
            /* local function declared as non-local functor -- but it can */ \
            /* be inlined only by C++03 */ \
            local_function_name)

// Inlined local functions are specified by `..._NAME(inline name)`.
// They have more chances to be inlined for faster run-times by some compilers
// (for example by GCC but not by MSVC). C++03 compilers can always inline
// local functions even if they are not explicitly specified inline.
#define BOOST_LOCAL_AUX_FUNCTION_NAME_PARSE_INLINE_(qualified_name) \
    BOOST_PP_IIF(BOOST_PP_BITOR( \
            BOOST_LOCAL_AUX_CONFIG_LOCAL_TYPES_AS_TPARAMS_01, \
            BOOST_DETAIL_PP_KEYWORD_IS_INLINE_FRONT(qualified_name)), \
        /* on C++03 always use inlining because compilers might optimize */ \
        /* it to be faster and it can also be passed as tparam */ \
        BOOST_LOCAL_AUX_FUNCTION_NAME_INLINE_ \
    , \
        /* on non C++03 don't use liniling unless explicitly specified by */ \
        /* programmers `inline name` the inlined local function cannot be */ \
        /* passed as tparam */ \
        BOOST_LOCAL_AUX_FUNCTION_NAME_ \
    )(BOOST_DETAIL_PP_KEYWORD_INLINE_REMOVE_FRONT(qualified_name))

// Recursive local function are specified by `..._NAME(recursive name)`. 
// They can never be inlined for faster run-time (not even by C++03 compilers).
#define BOOST_LOCAL_AUX_FUNCTION_NAME_PARSE_RECURSIVE_(qualified_name) \
    BOOST_PP_IIF(BOOST_LOCAL_AUX_PP_KEYWORD_IS_RECURSIVE_FRONT( \
            qualified_name), \
        /* recursion can never be inlined (not even on C++03) */ \
        BOOST_LOCAL_AUX_FUNCTION_NAME_RECURSIVE_ \
    , \
        BOOST_LOCAL_AUX_FUNCTION_NAME_PARSE_INLINE_ \
    )(BOOST_LOCAL_AUX_PP_KEYWORD_RECURSIVE_REMOVE_FRONT(qualified_name))

// PUBLIC //

#define BOOST_LOCAL_AUX_FUNCTION_NAME(qualified_name) \
    BOOST_LOCAL_AUX_FUNCTION_NAME_PARSE_RECURSIVE_(qualified_name)

#endif // #include guard

