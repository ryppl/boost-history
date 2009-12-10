//#include <iostream>
#include <boost/cstdint.hpp>

#ifdef INTMAX_C
#define BOOST_INTMAX_C(a) INTMAX_C(a)
#else
#define BOOST_INTMAX_C(a) a##LL
#endif

boost::intmax_t i = BOOST_INTMAX_C(1000000000);
int main() {
    //std::cout << i << std::endl;
    return (i);
}
