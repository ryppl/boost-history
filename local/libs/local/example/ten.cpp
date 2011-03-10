
//[ten_cpp
#include <boost/local/function.hpp>
#include <iostream>

int main() {
    int BOOST_LOCAL_FUNCTION_PARAMS( (void) ) {
        return 10;
    } BOOST_LOCAL_FUNCTION_NAME(ten)

    std::cout << ten() << std::endl;
    return 0;
}
//]

