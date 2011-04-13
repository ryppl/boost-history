
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef BOOST_LOCAL_AUX_FUNCTION_NAME_HPP_
#define BOOST_LOCAL_AUX_FUNCTION_NAME_HPP_

#include "../symbol.hpp"
#include "../config.hpp"
// For BOOST_TYPEOF.
#include "../scope_exit/scope_exit.hpp" // Use this lib's ScopeExit impl.

// PRIVATE //

#define BOOST_LOCAL_AUX_FUNCTION_NAME_INIT_RECURSION_FUNC_ \
    BOOST_LOCAL_AUX_INTERNAL_SYMBOL(init_recursion)

#define BOOST_LOCAL_AUX_FUNCTION_NAME_END_LOCAL_FUNCTOR_( \
        local_function_name, local_functor_name, nonlocal_functor_name, id) \
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
        BOOST_LOCAL_AUX_SYMBOL_FUNCTOR_TYPE local_function_name; \
        /* run-time: the `init_recursion()` function cannot be called by */ \
        /* the constructor to allow for compiler optimization (inlining) */ \
        /* so it must be public */ \
        inline void BOOST_LOCAL_AUX_FUNCTION_NAME_INIT_RECURSION_FUNC_( \
                BOOST_LOCAL_AUX_SYMBOL_FUNCTOR_TYPE& functor) { \
            local_function_name = functor; \
        } \
    /* local functor can be passed as tparam only on C++03 (faster) */ \
    } local_functor_name(BOOST_LOCAL_AUX_SYMBOL_ARGS_VARIABLE_NAME.value); \
    /* non-local functor can always be passed as tparam (but slower) */ \
    BOOST_TYPEOF(local_functor_name.local_function_name) \
            nonlocal_functor_name; \
    /* run-time: the following order in which the functors are set is */ \
    /* important to allow for compiler optimization (changing this order */ \
    /* and/or moving some of these sets into the functor constructors might */ \
    /* prevent compiler optimizations) */ \
    local_functor_name.BOOST_LOCAL_AUX_FUNCTION_NAME_INIT_RECURSION_FUNC_( \
            nonlocal_functor_name); \
    local_functor_name.BOOST_LOCAL_AUX_SYMBOL_INIT_CALL_FUNCTION_NAME( \
            &local_functor_name, nonlocal_functor_name);

#define BOOST_LOCAL_AUX_FUNCTION_NAME_FUNCTOR_(local_function_name) \
    BOOST_LOCAL_AUX_INTERNAL_SYMBOL(local_function_name)

// PUBLIC //

// Limitation: This is faster (smaller run-time than `FUNCION_NAME`) but it
// cannot be passed as template parameter on ISO C++ but it can on C++03.
#define BOOST_LOCAL_AUX_FUNCTION_NAME_OPTIMIZED(local_function_name) \
    BOOST_LOCAL_AUX_FUNCTION_NAME_END_LOCAL_FUNCTOR_( \
            local_function_name, \
            local_function_name, \
            BOOST_LOCAL_AUX_FUNCTION_NAME_FUNCTOR_(local_function_name), \
            __LINE__)

// ISO C++ does not allow to pass local classes as template parameters. But
// if can use C++03 (no ISO C++) features and MSVC compiler (which allows to
// pass local classes as template parameters), than pass local class as
// template parameter without the extra global functor to reduce run-time.
#ifdef BOOST_LOCAL_AUX_CONFIG_LOCAL_CLASS_AS_TEMPLATE_PARAMETER

#define BOOST_LOCAL_AUX_FUNCTION_NAME(local_function_name) \
    BOOST_LOCAL_AUX_FUNCTION_NAME_OPTIMIZED(local_function_name)

#else

#define BOOST_LOCAL_AUX_FUNCTION_NAME(local_function_name) \
    BOOST_LOCAL_AUX_FUNCTION_NAME_END_LOCAL_FUNCTOR_( \
            local_function_name, \
            BOOST_LOCAL_AUX_FUNCTION_NAME_FUNCTOR_(local_function_name), \
            local_function_name, \
            __LINE__)

#endif

#endif // #include guard

