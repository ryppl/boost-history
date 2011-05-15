
#include <boost/config.hpp>
#if defined(BOOST_NO_VARIADIC_MACROS) || defined(BOOST_LOCAL_CONFIG_COMPLIANT)
#include <iostream>
int main() {
    std::cerr << "Error: This program requires variadic macros" << std::endl;
    return 0;
}
#else

//[expensive_copy_boost_local_va_cpp
#include <boost/local/function.hpp>
#include <boost/noncopyable.hpp>
#include <iostream>

struct x_t {
    int i;
    x_t(int _i): i(_i) {}
    x_t(x_t const& o): i(o.i) { // Some time consuming copy.
        for (unsigned long i = 0; i < 10000000000; ++i); 
    }
};

int main() {
    x_t x(-1);

    void BOOST_LOCAL_FUNCTION_PARAMS(const bind& x) { // No copy...
        std::cout << x.i << std::endl; // ... and constant.
    } BOOST_LOCAL_FUNCTION_NAME(l)
    l();

    return 0;
}
//]

#endif

