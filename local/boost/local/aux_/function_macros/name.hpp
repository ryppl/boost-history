
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef BOOST_LOCAL_AUX_FUNCTION_NAME_HPP_
#define BOOST_LOCAL_AUX_FUNCTION_NAME_HPP_

#include "../symbol.hpp"
// For BOOST_TYPEOF.
#include "../scope_exit/scope_exit.hpp" // Use this lib's ScopeExit impl.
#include <boost/preprocessor/control/iif.hpp>

// PRIVATE //

#define BOOST_LOCAL_AUX_FUNCTION_NAME_END_LOCAL_FUNCTOR_( \
        local_functor_name, local_function_name, \
        is_local_function_member_public, id) \
    /* `PARAMS() { ... }` expandsion here -- still within functor class */ \
    /* class functor ## __LINE__ { ... */ \
    BOOST_PP_IIF(is_local_function_member_public, \
        /* member var with function name for recursive calls; must be */ \
        /* `public` because is it also used by this macro but outside */ \
        /* the functor class to deduce the functor type; it cannot be */ \
        /* `const` because it is init after construction (because */ \
        /* constructor doesn't know local function name) */ \
        public: \
    , \
        /* if optimized, then no need to access this for typeof */ \
        private: \
    ) \
        BOOST_LOCAL_AUX_SYMBOL_FUNCTOR_TYPE local_function_name; \
    private: \
        /* called by the constructor to init member variable for recursion */ \
        void BOOST_LOCAL_AUX_SYMBOL_INIT_RECURSION_FUNCTION_NAME() { \
            local_function_name = *this; \
        } \
    /* declares object for local class functor (but this functor cannot be */ \
    /* passed as template parameter); cannot be `const` `abstract_function` */ \
    /* is not passed as const to `function` to make the function call */ \
    } local_functor_name(BOOST_LOCAL_AUX_SYMBOL_ARGS_VARIABLE_NAME.value);

#define BOOST_LOCAL_AUX_FUNCTION_NAME_DECL_GLOBAL_FUNCTOR_( \
        local_functor_name, local_function_name, id) \
    /* declares actual functor for the local function (of type */ \
    /* local::function which can be passed as template parameter); this */ \
    /* is correctly `const` because it is directly visible to programmers */ \
    /* which cannot re-assign the local functor (they can only call it) */ \
    BOOST_TYPEOF(local_functor_name.local_function_name) const \
            local_function_name(local_functor_name);

// PUBLIC //

// Limitation: This is faster (smaller run-time than `FUNCION_NAME`) but it
// cannot be passed as template parameter on ISO C++.
#define BOOST_LOCAL_AUX_FUNCTION_NAME_OPTIMIZED(local_function_name) \
    BOOST_LOCAL_AUX_FUNCTION_NAME_END_LOCAL_FUNCTOR_(local_function_name, \
            local_function_name, 0 /* local func mem not public */, __LINE__)

// ISO C++ does not allow to pass local classes as template parameters. But
// if can use no ISO C++ standard features and MSVC compiler (which allows to
// pass local classes as template parameters), than pass local class as
// template parameter without the extra global functor to reduce run-time.
#if !defined(BOOST_LOCAL_CONFIG_COMPLIANT) && defined(_MSC_VER) 
    #define BOOST_LOCAL_AUX_FUNCTION_NAME(local_function_name) \
        BOOST_LOCAL_AUX_FUNCTION_NAME_OPTIMIZED(local_function_name)
#else
    #define BOOST_LOCAL_AUX_FUNCTION_NAME(local_function_name) \
        BOOST_LOCAL_AUX_FUNCTION_NAME_END_LOCAL_FUNCTOR_( \
                BOOST_LOCAL_AUX_SYMBOL_FUNCTOR_OBJECT_NAME( \
                        local_function_name), \
                local_function_name, 1 /* local func mem public */, __LINE__) \
        /* use the global functor increases run-time (because its uses one */ \
        /* indirected function call that cannot be optimized awa) */ \
        BOOST_LOCAL_AUX_FUNCTION_NAME_DECL_GLOBAL_FUNCTOR_( \
                BOOST_LOCAL_AUX_SYMBOL_FUNCTOR_OBJECT_NAME( \
                        local_function_name), local_function_name, __LINE__)
#endif

#endif // #include guard

