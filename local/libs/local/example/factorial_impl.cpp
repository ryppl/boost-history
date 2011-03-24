
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

//[ factorial_impl_cpp
#include <boost/local/function.hpp>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <string>

struct calculator {
    std::ostringstream output;

    void factorials(const std::vector<int>& nums,
            const std::string& separator = " ") {

        int // The local function result type (just before the `BOOST_LOCAL_FUNCTION_PARAMS` macro).

        // *BEGIN* The macro
        //  BOOST_LOCAL_FUNCTION_PARAMS( (int n) (bool recursion)(default false)
        //          (const bind& separator) (bind this) )
        // expands (at line 21) to:

        // DEDUCE RESULT TYPE
        // Long name `ERROR_...` gives meaningful compiler-error message if programmers forget result type before the macro.
        (*ERROR_missing_result_type_before_the_local_function_parameter_macro_id21)();
        // Use Boost.ScopeExit type deduction technique (tagging, wrapping, struct, etc) so to work on all compilers (specifically, some GCC version give internal errors otherwise).
        typedef void (*boost_local_auxXdeduce_result_tag21)(int ERROR_missing_result_type_before_the_local_function_parameter_macro_id21); 
        typedef BOOST_TYPEOF(boost::scope_exit::aux::wrap(boost::scope_exit::aux::deref(ERROR_missing_result_type_before_the_local_function_parameter_macro_id21, (boost_local_auxXdeduce_result_tag21)0))) boost_local_auxXdeduce_result_wrap21;
        typedef boost_local_auxXdeduce_result_wrap21::type boost_local_auxXdeduce_result_capture21; 
        struct boost_local_auxXdeduce_result_params21 { 
            typedef boost_local_auxXdeduce_result_capture21 function_ptr_type;
        }; 
        typedef boost::remove_pointer< boost_local_auxXdeduce_result_params21::function_ptr_type >::type boost_local_auxXdeduce_result_function_type21; 
        typedef boost::function_traits< boost_local_auxXdeduce_result_function_type21>::result_type boost_local_auxXresult_type21;
        
        // HANDLE BOUND PARAMETERS
        // Deduce object `this` type (using technique from Boost.ScopeExit patch to support `this`).
#if BOOST_WORKAROUND(BOOST_MSVC,>=1300)
        enum { boost_se_thistype_index_21 = sizeof(*boost::scope_exit::msvc_typeof_this::encode_start(this)) }; 
        typedef boost::scope_exit::msvc_typeof_this::msvc_typeid_wrapper<boost_se_thistype_index_21>::type se_this_type21;
#else
        typedef BOOST_TYPEOF(this) se_this_type21;
#endif
        // Deduce other `& separator` type (using Boost.ScopeExit technique).
        typedef void (*boost_se_tag_0_21)(int & separator); 
        typedef BOOST_TYPEOF(boost::scope_exit::aux::wrap(boost::scope_exit::aux::deref(& separator, (boost_se_tag_0_21)0))) boost_se_wrapped_t_0_21; 
        typedef boost_se_wrapped_t_0_21::type boost_se_capture_t_0_21;
        // Store bound parameter values and references.
        struct boost_se_params_t_21 {
            se_this_type21 se_this;
            typedef boost_se_capture_t_0_21 boost_se_param_t_0_21; 
            boost::scope_exit::aux::member<boost_se_param_t_0_21, boost_se_tag_0_21> boost_se_param_0_21;
        } boost_local_auxXparams21 = { 
            this, 
#ifdef BOOST_SCOPE_EXIT_AUX_TPL_WORKAROUND
            {
#endif
                boost::scope_exit::aux::deref(& separator, (boost_se_tag_0_21)0) 
#ifdef BOOST_SCOPE_EXIT_AUX_TPL_WORKAROUND
            } 
#endif
        };
        // Use variable name `...args` without line number to hold bound parameter values (so the same variable name can be used by the `NAME` macro even if it expands on a different line number).
        // The `declared<>` template makes sure that the variable `...args` is not declared multiple times in the same context (if the variable is already declared, this template expand to a no-operation).
        boost::scope_exit::aux::declared<boost::scope_exit::aux::resolve<sizeof(boost_local_auxXargs)>::cmp1<0>::cmp2> boost_local_auxXargs; 
        boost_local_auxXargs.value = &boost_local_auxXparams21;
        
        // LOCAL FUNCTOR
        class boost_local_auxXfunctor21:
                // Base abstract class is used to later assign this local functor to `boost::local::function` so the local function can be passed as a template parameter (polymorphism ensures that the correct `operator()` as implemented by this class is called when this object is used as an `abstract_function` by `boost::local::function`).
                public ::boost::local::aux::abstract_function<boost_local_auxXresult_type21 (int n, bool recursion), 1> {
            typedef boost_local_auxXresult_type21 (boost_local_auxXfunction_type)(int n , bool recursion);
        public:
            // Take a generic pointer so it can be invoked by the `NAME` macro even if it expands on a different line number.
            explicit boost_local_auxXfunctor21(void* binding_data):
                    boost_local_auxXbinds(static_cast<boost_se_params_t_21*>(binding_data)) {
                boost_local_auxXinit_recursion();
            }
        
            // Implement `operator()` for all parameters and any combination of default parameter.
            boost_local_auxXresult_type21 operator()(
                    ::boost::function_traits<boost_local_auxXfunction_type>::arg1_type arg1, 
                    ::boost::function_traits<boost_local_auxXfunction_type>::arg2_type arg2) {
                // Actual parameter names are not known to the macro syntax separately from their types so generic names `arg...` must be used (and therefore the parameter types are extracted from the function type).
                return boost_local_auxXbody(boost_local_auxXbinds->boost_se_param_0_21.value, boost_local_auxXbinds->se_this, arg1, arg2);
            }
            boost_local_auxXresult_type21 operator()(
                    ::boost::function_traits<boost_local_auxXfunction_type>::arg1_type arg1) {
                return boost_local_auxXbody(boost_local_auxXbinds->boost_se_param_0_21.value, boost_local_auxXbinds->se_this, arg1);
            } 
        
        private:
            // Local function functor type (for a later declaration).
            typedef ::boost::local::function<boost_local_auxXfunction_type, 1> boost_local_auxXfunctor_type;
            // Define the deduced types for using `BOOST_LOCAL_TYPEOF` from within the local function body (the `&` is kept because the bound parameter name is not known to the macro syntax separately from the reference qualifier).
            typedef ::boost::add_const< boost_se_params_t_21:: boost_se_param_t_0_21 >::type & separatorXboost_local_auxXtypeof_type;
            typedef se_this_type21 thisXboost_local_auxXtypeof_type; 
            // Hold bound parameters values and references.
            boost_se_params_t_21* boost_local_auxXbinds;
            // This same declaration is first made at global scope in one of the library header files. This declaration needs to be repeated here so it is also visible from within the local function body to allow for nesting local function into one another.
            boost::scope_exit::aux::undeclared boost_local_auxXargs;
            
            // The local function body. The body function cannot be static because is needs to access the member variable with the local function name to support recursion (and local classes cannot have static member variable) but unfortunately this implies that `this` can be misused within the body instead of `this_` without necessarily generating a compile-time error.
            boost_local_auxXresult_type21 boost_local_auxXbody(
                    // Use special name `this_` instead of `this` here.
                    ::boost::add_const<boost_se_params_t_21::boost_se_param_t_0_21>::type & separator, se_this_type21 this_,
                    // Specify default parameter values here.
                    int n, bool recursion = false) const 
        
        // *END* The expansion of `BOOST_LOCAL_FUNCTION_PARAMS` ends here.

        // *BEGIN* The local function body code `{ ... }` as specified by the programmers:
 
        {
            int result = 0;

            if (n < 2 ) result = 1;
            else result = n * factorial(n - 1, true); // Recursive call.

            if (!recursion) this_->output << result << separator;
            return result;
        } 
        
        // *END* The local function body `{ ... }` ends here.
    
        // *BEGIN* The macro `BOOST_LOCAL_FUNCTION_NAME(factorial)` expands to:

        public:
            // Member variable named after the local function so the body can recursively call the local function (this must be defined here because the local function name was not known before). This must be public because it is also used to deduce the local function functor type later when when declaring the actual local function object.
            boost_local_auxXfunctor_type factorial;
        private:
            // Initializes the local functor member variable (this cannot be done directly within the constructor because the local function name is not known when the constructor code is expanded by the `PARAMS` macro).
            void boost_local_auxXinit_recursion() {
                factorial = *this;
            }
        // Declare the local class object -- which cannot be passed as template parameter (because it is a local class). It uses the variable with the generic `...args` name to pass the bound parameter values and references (so this variable name must not contain the line number because it is used by different macros expanding on different lines).
        } boost_local_auxXfunctorXfactorial(boost_local_auxXargs.value);
        // Declare the actual local function object with the local function name -- which can be passed as template parameter (because it is a `boost::local::function` functor and not a local class).
        BOOST_TYPEOF(boost_local_auxXfunctorXfactorial.factorial) const factorial(boost_local_auxXfunctorXfactorial);

        // *END* The expansion of `BOOST_LOCAL_FUNCTION_NAME` ends here.
 
        std::for_each(nums.begin(), nums.end(), factorial);
    }
};

int main() {
    std::vector<int> v(3);
    v[0] = 1; v[1] = 4; v[2] = 7;

    calculator calc;
    calc.factorials(v);
    std::cout << calc.output.str() << std::endl;

    return 0;
}
//]

