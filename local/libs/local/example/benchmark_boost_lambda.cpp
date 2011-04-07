
#include <boost/lambda/lambda.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

#define N 1e4
#define S N * 1e2

int main() {
    using boost::lambda::_1;

    double sum = 0.0;
    int factor = 1;

    std::vector<double> v(S);
    std::fill(v.begin(), v.end(), 1.0);

    for (size_t n = 0; n < N; ++n) {
        std::for_each(v.begin(), v.end(), (
            sum += factor * _1
        ));
    }

    std::cout << sum << std::endl;
    assert(sum == N * S);
    return 0;
}

