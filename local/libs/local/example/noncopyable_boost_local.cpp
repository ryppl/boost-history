
//[noncopyable_boost_local_cpp
#include <boost/local/function.hpp>
#include <boost/noncopyable.hpp>
#include <iostream>

struct x_t: boost::noncopyable {
    int i;
    x_t(int _i): i(_i) {}
};

int main() {
    x_t x(-1);

    void BOOST_LOCAL_FUNCTION_PARAMS( (const bind& x) ) { // OK...
        std::cout << x.i << std::endl; // ... and constant.
    } BOOST_LOCAL_FUNCTION_NAME(l)
    l();

    return 0;
}
//]

