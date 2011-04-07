
#include <boost/local/function.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    double sum = 0.0;
    int factor = 10;

    void BOOST_LOCAL_FUNCTION_PARAMS( (double num)
            (bind& sum) (const bind factor) ) {
        sum += factor * num;
    } BOOST_LOCAL_FUNCTION_NAME(add)

    std::vector<double> v(100000);
    std::fill(v.begin(), v.end(), 10);

    for (size_t i = 0; i < 10000; ++i) {
        std::for_each(v.begin(), v.end(), add);
    }

    std::cout << sum << std::endl;
    return 0;
}

