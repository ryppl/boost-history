
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef BOOST_LOCAL_AUX_FUNCTION_NAME_HPP_
#define BOOST_LOCAL_AUX_FUNCTION_NAME_HPP_

#include "../symbol.hpp"
// For BOOST_TYPEOF.
#include "../scope_exit/scope_exit.hpp" // Use this lib's ScopeExit impl.

// PRIVATE //

#define BOOST_LOCAL_AUX_FUNCTION_NAME_(local_function_name, id) \
    /* `PARAMS() { ... }` expandsion here -- still within functor class */ \
    /* class functor ## __LINE__ { ... */ \
    public: \
        /* member var with function name for recursive calls; must be */ \
        /* `public` because is it also used by this macro but outside */ \
        /* the functor class to deduce the functor type; it cannot be */ \
        /* `const` because it is init after construction (because */ \
        /* constructor doesn't know local function name) */ \
        BOOST_LOCAL_AUX_SYMBOL_FUNCTOR_TYPE local_function_name; \
    private: \
        /* called by the constructor to init member variable for recursion */ \
        void BOOST_LOCAL_AUX_SYMBOL_INIT_RECURSION_FUNCTION_NAME() { \
            local_function_name = *this; \
        } \
    /* declares object for local class functor (but this functor cannot be */ \
    /* passed as template parameter); cannot be `const` `abstract_function` */ \
    /* is not passed as const to `function` to make the function call */ \
    } BOOST_LOCAL_AUX_SYMBOL_FUNCTOR_OBJECT_NAME(local_function_name)( \
            BOOST_LOCAL_AUX_SYMBOL_ARGS_VARIABLE_NAME.value); \
    /* declares actual functor for the local function (of type */ \
    /* local::function which can be passed as template parameter); this */ \
    /* is correctly `const` because it is directly visible to programmers */ \
    /* which cannot re-assign the local functor (they can only call it) */ \
    BOOST_TYPEOF(BOOST_LOCAL_AUX_SYMBOL_FUNCTOR_OBJECT_NAME( \
            local_function_name).local_function_name) const \
            local_function_name(BOOST_LOCAL_AUX_SYMBOL_FUNCTOR_OBJECT_NAME( \
                    local_function_name));

// PUBLIC //

#define BOOST_LOCAL_AUX_FUNCTION_NAME(local_function_name) \
    BOOST_LOCAL_AUX_FUNCTION_NAME_(local_function_name, __LINE__) \

#endif // #include guard


