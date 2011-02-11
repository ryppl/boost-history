
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

// Use recursion, default parameters, and bind by non-const reference.

//[ factorial_cpp
#include <boost/local/function.hpp>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>

int main () {
    std::ostringstream output;

    int // Result type (outside the `PARAMS` macro).

    // On all C++ preprocessors (including C99 preprocessors) the macro:
    //
    //  int BOOST_LOCAL_FUNCTION_PARAMS(
    //          (int n) (bool recursion)(default false) (bind& output) )
    //
    // Or, on C99 preprocessors only the macro:
    //
    //  int BOOST_LOCAL_FUNCTION_PARAMS(
    //          int n, bool recursion, default false, bind& output)
    //
    // Expands to code equivalent to the following.
    //
    // NOTE:
    // * Use line number __LINE__ (e.g., 29) to generate unique symbols.
    // * Parameter name not available separately from its type.
    // * Function name NOT available.
    
// In actual expansion, the following tokens are made available as macro
// parameters (and not as macro symbols) by the `PARAMS` macro:
#define PARAMS_arg_0            int n
#define PARAMS_arg_with_dflt_0  PARAMS_arg_0 // No default.
#define PARAMS_arg_1            bool recursion
#define PARAMS_arg_with_dflt_1  PARAMS_arg_1 = false
#define PARAMS_bind_0           &output
#define PARAMS_is_const_bind_0  0 // Not a constant bind.

    // Function traits.
    // NOTE: Following result type specified just before the `PARAMS` macro.
    // Default parameter values need to be separated from their parameter types
    // and names because they are not part of the function type so they cannot
    // be used by the following expressions and their number cannot be count
    // at compile-time using template metaprogramming.
    ERROR_missing_result_type_at_line_29(PARAMS_arg_0, PARAMS_arg_1);
    /** @todo This typeof requires registration of result, arg, etc type? */
    typedef BOOST_TYPEOF(ERROR_missing_result_type_at_line_29) function_type_29;
    typedef boost::function_traits<function_type_29>::result_type
            result_type_29;
    typedef boost::function_traits<function_type_29>::arg1_type arg_type_0_29;
    typedef boost::function_traits<function_type_29>::arg2_type arg_type_1_29;

    // Handle bound parameters as done by Boost.ScopeEixt, deducing their types
    // (using Boost.Typeof) and storing them by reference or by value.
    typedef void (*bind_deduce_type_0_29)(int PARAMS_bind_0);
    typedef BOOST_TYPEOF(boost::type_of::ensure_obj(
            boost::scope_exit::aux::wrap(boost::scope_exit::aux::deref(
                    PARAMS_bind_0, static_cast<bind_deduce_type_0_29>(0)))))
            bind_wrapped_type_0_29;
    typedef bind_wrapped_type_0_29::type capture_bind_type_0_29;
    // Hold bound parameter types and values.
    struct binds_29 {
        typedef capture_bind_type_0_29 bind_type_0_29;
        boost::scope_exit::aux::member<bind_type_0_29, bind_deduce_type_0_29>
                bind_value_0_29;
    } params_29 = {
        { boost::scope_exit::aux::deref(PARAMS_bind_0,
                static_cast<bind_deduce_type_0_29>(0)) }
    };
    // NOTE: The `args` variable is declared globally and not prefixed with
    // __LINE__ so it can be used by both the `PARAMS` and `NAME`. The special
    // template declaration type prevents this variable to be declared multiple
    // times within the same scope.
    boost::scope_exit::aux::declared<boost::scope_exit::aux::resolve<
            sizeof(boost_local_auxXargs)>::cmp1<0>::cmp2> boost_local_auxXargs;
    boost_local_auxXargs.value = &params_29;
    
    // Functor for local function.
    class functor_29:
            // Base used to assign local functions to `function_ref` which can
            // then be passed as template parameter.
            public ::boost::local::aux::function_base<function_type_29, 1> {
        typedef ::boost::local::function_ref<function_type_29, 1> function_type;
        binds_29* binds_; // Bound parameter values.
    public:
        // Public so it can be used later to deduce function type where
        // __LINE__ postfixed symbols `..._29` are no longer available.
        function_type* deduce_function_type;
        explicit functor_29(void* binds):
                binds_(static_cast<binds_29*>(binds)) {
            init_recursion();
        }
        result_type_29 operator()(arg_type_0_29 arg_0, arg_type_1_29 arg_1) { 
            assert(binds_);
            return body(
                binds_->bind_value_0_29.value 
                // Using general names `arg_i` because parameter types and
                // names are not separated by the preprocessor so the actual
                // argument name (e.g., `n`) is not available here.
                , arg_0, arg_1 
            ); 
        }
        // Overloading to support default parameters.
        result_type_29 operator()(arg_type_0_29 arg_0) { 
            assert(binds_);
            return body(
                  binds_->bind_value_0_29.value 
                , arg_0 
            ); 
        } 
    private: 
        // LIMITATION: Body cannot be static because it has to access the
        // member named after the function name for recursive calls (the
        // function name is not know to this macro). However, ideally the body
        // will be static so to prevent using `this` instead of `this_` by
        // mistake (in most cases this will still cause a compile-time error
        // because when functor has a different structure than the bound object
        // `this_` -- but that is not the case if `this` is mistakenly used
        // instead of `this` to do pointer operations). Programmers need to
        // inspect the local function body code by eye and make sure that
        // `this` is not used by the body code.
        result_type_29 body(
#if PARAMS_is_const_bind_0
              ::boost::add_const< // Handle constant binding.
#endif
                    binds_29::bind_type_0_29 
#if PARAMS_is_const_bind_0
              >::type
#endif
                    PARAMS_bind_0
            , PARAMS_arg_with_dflt_0
            , PARAMS_arg_with_dflt_1)
    
    // Local function body (programmed outside the macros).
    
    {
        int result = 0;
        if (n < 2 ) result = 1;
        else result = n * factorial(n - 1, true);

        if (!recursion) output << result << " ";
        return result;
    }

// All `..._29` and `PARAMS_...` symbols are only available for within `PARAMS`
// macro expansion for the code above.
#undef PARAMS_arg0
#undef PARAMS_dflt0
#undef PARAMS_arg1
#undef PARAMS_dflt1
#undef PARAMS_bind0

    // The macro:
    //
    //  BOOST_LOCAL_FUNCTION_NAME(factorial)
    //
    // Expands to code equivalent to the following. Note:
    //
    // * Use function name `factorial` to generate unique symbols.
    // * Function name `factorial` available.
    // * None of the `..._29` symbols are available (different __LINE__).

        // Member with function name for recursive calls. This cannot be
        // defined sooner because the function name is only available here.
        function_type factorial;
        // Cannot be programmed in the constructor because it sets the
        // `factorial` member with name only known in this macro expansion.
        void init_recursion() { factorial = *this; }
    } object_factorial(boost_local_auxXargs.value);
    BOOST_TYPEOF(*object_factorial.deduce_function_type) factorial(
            object_factorial);
    
    // Rest of the program.

    std::vector<int> v;
    v.resize(3);
    v[0] = 1; v[1] = 4; v[2] = 7;
    std::for_each(v.begin(), v.end(), factorial);

    std::cout << output.str() << std::endl;
    return 0;
}
//]

