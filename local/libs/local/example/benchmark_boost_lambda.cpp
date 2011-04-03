
#include <boost/lambda/lambda.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

#define N 10000

int main() {
    using boost::lambda::_1;
    using boost::lambda::var;

    double sum = 0.0;
    int factor = 10;

    std::vector<double> v(N * 100);
    std::fill(v.begin(), v.end(), 10);

    for (size_t n = 0; n < N; ++n) {
        std::for_each(v.begin(), v.end(), (
            sum += factor * _1
        ));
    }

    std::cout << sum << std::endl;
    return 0;
}

