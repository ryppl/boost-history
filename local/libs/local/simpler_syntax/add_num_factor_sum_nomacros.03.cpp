
#include <boost/local/function.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/mpl/at.hpp>
#include <iostream>

int main() {
    typedef void (func_type)(double num = -1.23);
    
    typedef boost::function_types::parameter_types<func_type>::type param_types;
    typedef boost::mpl::at_c<param_types, 0>::type arg_type0;

    return 0;
}

