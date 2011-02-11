
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
    std::ostringstream factorials;

    // On all C++ preprocessors (including C99 preprocessors) the macro:
    //
    //  int BOOST_LOCAL_FUNCTION_PARAMS(
    //          (int x) (bool recursion)(default false) (bind& factorials) )
    //
    // Or, on C99 preprocessors only the macro:
    //
    //  int BOOST_LOCAL_FUNCTION_PARAMS(
    //          int x, bool recursion, default false, bind& factorials)
    //
    // Expand to code equivalent to the following:

    // From the macros that parse the parameters:
    #define ARG0 int x
    #define DFLT0 // empty
    #define ARG1 bool recursion
    #define DFLT1 false
    #define BIND0 &factorials

    typedef void (*boost_se_tag_0_at_line_29)(int &factorials );
    typedef BOOST_TYPEOF(boost::type_of::ensure_obj(
            boost::scope_exit::aux::wrap(boost::scope_exit::aux::deref(
                    &factorials, (boost_se_tag_0_at_line_29)0))))
            boost_se_wrapped_t_0_at_line_29;
    typedef boost_se_wrapped_t_0_at_line_29::type
            boost_se_capture_t_0_at_line_29;
    
    struct boost_se_params_t_at_line_29 {
        typedef boost_se_capture_t_0_at_line_29 boost_se_param_t_0_at_line_29;
        boost::scope_exit::aux::member<boost_se_param_t_0_at_line_29,
                boost_se_tag_0_at_line_29> boost_se_param_0_at_line_29;
    } boost_local_auxXparams_at_line_29 = {
        { boost::scope_exit::aux::deref(&factorials,
                (boost_se_tag_0_at_line_29)0) }
    }; 
    boost::scope_exit::aux::declared<boost::scope_exit::aux::resolve<
            sizeof(boost_local_auxXargs)>::cmp1<0>::cmp2> boost_local_auxXargs;
    boost_local_auxXargs.value = &boost_local_auxXparams_at_line_29;
    
    typedef ::boost::local::function_ref<int (int , bool) , 1>
            boost_local_auxXfactorialXref;
    struct boost_local_auxXfunctorXfactorial :
            ::boost::local::aux::function_base<int (int ,bool), 1> {
        explicit boost_local_auxXfunctorXfactorial(void* bind_params) : 
                boost_local_auxXparams(
                        (boost_se_params_t_at_line_29*) bind_params)
                {} 
        int operator()(int n , bool recursion) { 
            return boost_local_auxXbody( *this
                , boost_local_auxXparams->boost_se_param_0_at_line_29.value 
                , n , recursion 
            ); 
        } 
        int operator()(int n) { 
            return boost_local_auxXbody( *this 
                , boost_local_auxXparams->boost_se_param_0_at_line_29.value 
                , n 
            ); 
        } 
    private: 
        boost_se_params_t_at_line_29* boost_local_auxXparams; 
        static int boost_local_auxXbody(
              boost_local_auxXfunctorXfactorial& factorial 
            , boost_se_params_t_at_line_29::boost_se_param_t_0_at_line_29 
                    &factorials
            , int n
            , bool recursion = false)
    
    // Local function body (programmed outside the macros).
    
    {
        int result = 0;
        if (n < 2 ) result = 1;
        else result = n * factorial(n - 1, true);
        if (!recursion) factorials << result << " ";
        return result;
    }

    // The macro:
    //
    //  BOOST_LOCAL_FUNCTION_NAME(factorial)
    //
    // Expand to code equivalent to the following:

    #define NAME factorial
    
    };
    boost_local_auxXfunctorXfactorial boost_local_auxXobjectXfactorial(
            boost_local_auxXargs.value);
    boost_local_auxXfactorialXref factorial(boost_local_auxXobjectXfactorial);

    // Rest of the program.

    std::vector<int> v;
    v.resize(3);
    v[0] = 1; v[1] = 4; v[2] = 7;
    std::for_each(v.begin(), v.end(), factorial);

    std::cout << factorials.str() << std::endl;
    return 0;
}
//]

