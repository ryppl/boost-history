
#include <boost/spirit/include/phoenix.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

#define N 10000

int f() { return -1; }

int main() {
    using boost::phoenix::let;
    using boost::phoenix::local_names::_f;
    using boost::phoenix::cref;
    using boost::phoenix::ref;
    using boost::phoenix::arg_names::_1;
    using boost::phoenix::val;

    double sum = 0.0;
    int factor = 10;

    struct l {
        double f(const double& x) { return x; }
    } ll;

    std::vector<double> v(N * 100);
    std::fill(v.begin(), v.end(), 10);

    for (size_t n = 0; n < N; ++n) {
        std::for_each(v.begin(), v.end(), (
            ref(sum) += factor * ll.f(1.0 * _1)
        ));
    }

    std::cout << sum << std::endl;
    return 0;
}

