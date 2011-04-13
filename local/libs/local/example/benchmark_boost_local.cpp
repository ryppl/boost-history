
#include <boost/local/function.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>

#define N 1e4
#define S N * 1e2

int main() {
    double sum = 0.0;
    int factor = 1;

    void BOOST_LOCAL_FUNCTION_PARAMS( (const double& num)
            (bind& sum) (const bind& factor) ) {
        sum += factor * num;
    } BOOST_LOCAL_FUNCTION_NAME(add)

    std::vector<double> v(S);
    std::fill(v.begin(), v.end(), 1.0);

    for (size_t n = 0; n < N; ++n) {
        std::for_each(v.begin(), v.end(), add);
    }

    std::cout << sum << std::endl;
    assert(sum == N * S);
    return 0;
}

