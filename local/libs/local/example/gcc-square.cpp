
#include <boost/local/function.hpp>

void add_square(double a, double b) {
    double BOOST_LOCAL_FUNCTION(double z) {
        return z * z;
    } BOOST_LOCAL_FUNCTION_NAME(suare)

    return square(a) + square(b);
}



