
#include <boost/local/function.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/mpl/at.hpp>
#include <boost/function.hpp>
#include <iostream>

int main() {
    typedef void (func_type)(double num = -1.23);
    typedef int x;
    {
        typedef int y;
    }
    {
        typedef double y;
    }
    
    typedef boost::function_types::parameter_types<func_type>::type param_types;
    typedef boost::mpl::at_c<param_types, 0>::type arg_type0;

    boost::function<void ()> v;
    v();

    return 0;
}

