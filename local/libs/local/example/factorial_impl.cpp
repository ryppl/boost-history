
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

//[factorial_impl_cpp
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
        typedef void (*boost_local_auxXdeduce_result_tag21)( int ERROR_missing_result_type_before_the_local_function_parameter_macro_id21);
        typedef BOOST_TYPEOF(boost::scope_exit::aux::wrap( boost::scope_exit::aux::deref( ERROR_missing_result_type_before_the_local_function_parameter_macro_id21, (boost_local_auxXdeduce_result_tag21)0))) boost_local_auxXdeduce_result_wrap21;
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
        // Deduce other `& separator` type (using Boost.ScopeExit techniques).
        typedef void (*boost_se_tag_0_21)(int & separator);
        typedef BOOST_TYPEOF(boost::scope_exit::aux::wrap( boost::scope_exit::aux::deref(& separator, (boost_se_tag_0_21)0))) boost_se_wrapped_t_0_21;
        typedef boost_se_wrapped_t_0_21::type boost_se_capture_t_0_21;
        // Store bound parameter values and references.
        struct boost_se_params_t_21 {
            se_this_type21 se_this;
            typedef boost_se_capture_t_0_21 boost_se_param_t_0_21;
            boost::scope_exit::aux::member< boost_se_param_t_0_21, boost_se_tag_0_21 > boost_se_param_0_21;
        } boost_local_auxXparams21 = { 
            this , 
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
        boost::scope_exit::aux::declared< boost::scope_exit::aux::resolve< sizeof(boost_local_auxXargs) >::cmp1<0>::cmp2 > boost_local_auxXargs;
        boost_local_auxXargs.value = &boost_local_auxXparams21;
        
        // LOCAL FUNCTOR
        class boost_local_auxXfunctor21 {
            typedef boost_local_auxXresult_type21 (boost_local_auxXfunction_type) ( int n , bool recursion ); 
            typedef ::boost::local::aux::function< boost_local_auxXfunction_type, 1 > boost_local_auxXfunctor_type; 
            typedef ::boost::add_const< boost_se_params_t_21:: boost_se_param_t_0_21 >::type & separatorXboost_local_auxXtypeof_type ; 
            typedef se_this_type21 thisXboost_local_auxXtypeof_type ; 
        public: 
            // Take a generic pointer so it can be invoked by the `NAME` macro even if it expands on a different line number.            
            inline explicit boost_local_auxXfunctor21( void* bindings) :
                      boost_local_auxXbind_this( static_cast< boost_se_params_t_21* >( bindings)-> se_this ) 
                    , boost_local_auxXbind0 ( static_cast< boost_se_params_t_21* >( bindings)-> boost_se_param_0_21.value ) 
            { } 

            // Implement `operator()` for all parameters and any combination of default parameter.
            inline boost_local_auxXresult_type21 operator()(
                      ::boost::call_traits< ::boost::function_traits< boost_local_auxXfunction_type>::arg1_type>::param_type arg1 
                    , ::boost::call_traits< ::boost::function_traits< boost_local_auxXfunction_type>::arg2_type>::param_type arg2 
            ) const {
                return boost_local_auxXbody( boost_local_auxXbind0 , boost_local_auxXbind_this , arg1 , arg2 ); 
            } 
            inline boost_local_auxXresult_type21 operator()( 
                    ::boost::call_traits< ::boost::function_traits< boost_local_auxXfunction_type>::arg1_type>::param_type arg1 
            ) const { 
                return boost_local_auxXbody( boost_local_auxXbind0 , boost_local_auxXbind_this , arg1 ); 
            } 
        
            // "Casting functor trick" to pass this local class as template parameter.
            inline static boost_local_auxXresult_type21 boost_local_auxXcall0( 
                      void* object 
                    , ::boost::call_traits< ::boost::function_traits< boost_local_auxXfunction_type>::arg1_type>::param_type arg1 
                    , ::boost::call_traits< ::boost::function_traits< boost_local_auxXfunction_type>::arg2_type>::param_type arg2 
            ) { 
                return static_cast< boost_local_auxXfunctor21* >( object)->operator()( arg1 , arg2 ); 
            } 
            inline static boost_local_auxXresult_type21 boost_local_auxXcall1( 
                      void* object 
                    , ::boost::call_traits< ::boost::function_traits< boost_local_auxXfunction_type>::arg1_type>::param_type arg1 
            ) { 
                return static_cast< boost_local_auxXfunctor21* >( object)->operator()( arg1 ); 
            } 
            inline static void boost_local_auxXinit_call( void* object, boost_local_auxXfunctor_type& functor) { 
                functor.boost_local_auxXinit_call(object, &boost_local_auxXcall0 , &boost_local_auxXcall1 ); 
            } 
        
        private: 
            // Hold bound variables (by value and reference).
            se_this_type21 & boost_local_auxXbind_this ; 
            ::boost::add_const< boost_se_params_t_21:: boost_se_param_t_0_21 >::type & boost_local_auxXbind0 ; 
            // This same declaration is first made at global scope in one of the library header files. This declaration needs to be repeated here so it is also visible from within the local function body to allow for nesting local function into one another.
            boost::scope_exit::aux::undeclared boost_local_auxXargs; 
            
            // The local function body.
            inline boost_local_auxXresult_type21 boost_local_auxXbody( 
                      ::boost::add_const< boost_se_params_t_21:: boost_se_param_t_0_21 >::type & separator 
                    , se_this_type21 const this_  // Use special name `this_` instead of `this` here.
                    // Specify default parameter values here.
                    , int n , bool recursion = false 
            ) const

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
    
        // *BEGIN* The macro `BOOST_LOCAL_FUNCTION_NAME(recursive factorial)` expands to:

        public: 
            // Member variable named after the local function so the body can recursively call the local function (this must be defined here because the local function name was not known before). This must be public because it is also used to deduce the local function functor type later when when declaring the actual local function object.
            boost_local_auxXfunctor_type factorial; 
            // Initializes the local functor member variable (this cannot be done directly within the constructor because the local function name is not known when the constructor code is expanded by the `PARAMS` macro).
            inline void boost_local_auxXinit_recursion( boost_local_auxXfunctor_type& functor) { 
                factorial = functor; 
            } 
        // Declare the local class object -- which cannot be passed as template parameter (because it is a local class). It uses the variable with the generic `...args` name to pass the bound parameter values and references (so this variable name must not contain the line number because it is used by different macros expanding on different lines).
        } boost_local_auxXfactorial(boost_local_auxXargs.value); 
        // Declare the actual local function object with the local function name -- which can be passed as template parameter (because it is a `boost::local::function` functor and not a local class).
        BOOST_TYPEOF(boost_local_auxXfactorial. factorial) factorial; 
        // Initializes casting functor to pass local function as template parameter.
        boost_local_auxXfactorial.boost_local_auxXinit_call( &boost_local_auxXfactorial, factorial); 
        // Initializes functor for recursion.
        boost_local_auxXfactorial.boost_local_auxXinit_recursion( factorial);

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

