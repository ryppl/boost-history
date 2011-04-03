
#include <boost/local/function.hpp>
#include <vector>
#include <algorithm>
#include <iostream>

#define N 10000

int main() {
    double sum = 0.0;
    int factor = 10;

    void BOOST_LOCAL_FUNCTION_PARAMS( (const double& num)
            (bind& sum) (const bind& factor) ) {
        sum += factor * num;
    } BOOST_LOCAL_FUNCTION_NAME(add)

    std::vector<double> v(N * 100);
    std::fill(v.begin(), v.end(), 10);
    for (size_t n = 0; n < N; ++n) {
        std::for_each(v.begin(), v.end(), add);
    }

    std::cout << sum << std::endl;
    return 0;
}

