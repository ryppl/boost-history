
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

int main () {
    std::ostringstream output;

    int // The local function result type (just before the `BOOST_LOCAL_FUNCTION_PARAMS` macro).

    // *BEGIN* The macro
    //  BOOST_LOCAL_FUNCTION_PARAMS( (int n) (bool recursion)(default false)
    //          (bind& output) )
    // expands (at line 18) to:

    (*ERROR_missing_result_type_before_the_local_function_parameter_macro_id18)(); 
    typedef void (*boost_local_auxXdeduce_result_tag18)(int ERROR_missing_result_type_before_the_local_function_parameter_macro_id18); 
    typedef BOOST_TYPEOF(boost::scope_exit::aux::wrap( boost::scope_exit::aux::deref( ERROR_missing_result_type_before_the_local_function_parameter_macro_id18, (boost_local_auxXdeduce_result_tag18)0))) boost_local_auxXdeduce_result_wrap18; 
    typedef boost_local_auxXdeduce_result_wrap18::type boost_local_auxXdeduce_result_capture18; 
    struct boost_local_auxXdeduce_result_params18 {
        typedef boost_local_auxXdeduce_result_capture18 function_ptr_type;
    }; 

    typedef boost::remove_pointer< boost_local_auxXdeduce_result_params18::function_ptr_type >::type boost_local_auxXdeduce_result_function_type18;
    typedef boost::function_traits< boost_local_auxXdeduce_result_function_type18>::result_type boost_local_auxXresult_type18; 
    typedef void (*boost_se_tag_0_18)(int & output ); 
    typedef BOOST_TYPEOF(boost::scope_exit::aux::wrap( boost::scope_exit::aux::deref(& output, (boost_se_tag_0_18)0))) boost_se_wrapped_t_0_18;
    typedef boost_se_wrapped_t_0_18::type boost_se_capture_t_0_18;
    struct boost_se_params_t_18 {
        typedef boost_se_capture_t_0_18 boost_se_param_t_0_18;
        boost::scope_exit::aux::member< boost_se_param_t_0_18, boost_se_tag_0_18 > boost_se_param_0_18;
    } boost_local_auxXparams18 = {
        { boost::scope_exit::aux::deref(& output, (boost_se_tag_0_18)0) } // This is for GCC but no extra curly parenthesis on MSVC.
    }; 
    
    boost::scope_exit::aux::declared< boost::scope_exit::aux::resolve< sizeof(boost_local_auxXargs) >::cmp1<0>::cmp2 > boost_local_auxXargs;
    boost_local_auxXargs.value = &boost_local_auxXparams18;
    
    class boost_local_auxXfunctor18 : public ::boost::local::aux::abstract_function< boost_local_auxXresult_type18 ( int n , bool recursion ), 1 > {
        typedef boost_local_auxXresult_type18 (boost_local_auxXfunction_type) ( int n , bool recursion );
    public:
        explicit boost_local_auxXfunctor18( void* binding_data) :
                boost_local_auxXbinds(static_cast< boost_se_params_t_18*>(binding_data)) {
            boost_local_auxXinit_recursion();
        }
        
        boost_local_auxXresult_type18 operator()(
                ::boost::function_traits< boost_local_auxXfunction_type>::arg1_type arg1 , 
                ::boost::function_traits< boost_local_auxXfunction_type>::arg2_type arg2 ) { 
            return boost_local_auxXbody( boost_local_auxXbinds-> boost_se_param_0_18.value , arg1 , arg2 );
        }
        boost_local_auxXresult_type18 operator()(
                ::boost::function_traits< boost_local_auxXfunction_type>::arg1_type arg1 ) {
            return boost_local_auxXbody( boost_local_auxXbinds-> boost_se_param_0_18.value , arg1 );
        } 
    
    private:
        boost::scope_exit::aux::undeclared boost_local_auxXargs;
        typedef ::boost::local::function<boost_local_auxXfunction_type, 1 > boost_local_auxXfunctor_type;
        typedef boost_se_params_t_18:: boost_se_param_t_0_18 & outputXboost_local_auxXtypeof_type ;
        boost_se_params_t_18* boost_local_auxXbinds;

        boost_local_auxXresult_type18 boost_local_auxXbody( boost_se_params_t_18:: boost_se_param_t_0_18 & output , int n , bool recursion = false ) const

    // *END* The expansion of `BOOST_LOCAL_FUNCTION_PARAMS` ends here.

    // *BEGIN* The local function body code `{ ... }` as specified by the programmers:

    {
        int result = 0;

        if (n < 2 ) result = 1;
        else result = n * factorial(n - 1, true); // Recursive call.

        if (!recursion) output << result << " ";
        return result;
    }

    // *END* The local function body `{ ... }` ends here.
    
    // *BEGIN* The macro `BOOST_LOCAL_FUNCTION_NAME(factorial)` expands to:

    public:
        boost_local_auxXfunctor_type factorial;

    private: 
        void boost_local_auxXinit_recursion() { 
            factorial = *this; 
        } 
    } boost_local_auxXfunctorXfactorial(boost_local_auxXargs.value);
    BOOST_TYPEOF(boost_local_auxXfunctorXfactorial.factorial) const factorial(boost_local_auxXfunctorXfactorial);

    // *END* The expansion of `BOOST_LOCAL_FUNCTION_NAME` ends here.

    std::vector<int> v(3);
    v[0] = 1; v[1] = 4; v[2] = 7;
    std::for_each(v.begin(), v.end(), factorial);

    std::cout << output.str() << std::endl;
    return 0;
}
//]

