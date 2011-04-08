
#include <boost/spirit/include/phoenix.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

#define N 1e4
#define S N * 1e2

template<typename T>
struct number_interface { 
    virtual T complement(T x) = 0; 
};

template<typename T>
struct number: number_interface<T> {
    T complement(T x) { return -x; } 
};

const double& identity(const double& x) { return x; }

int main() {
    using boost::phoenix::let;
    using boost::phoenix::ref;
    using boost::phoenix::cref;
    using boost::phoenix::arg_names::_1;
    using boost::phoenix::local_names::_f;
    using boost::phoenix::local_names::_x;
    using boost::phoenix::bind;

    double sum = 0.0;
    int factor = 1;
    const double& (*p)(const double&) = &identity;
    number<double> d;

    std::vector<double> v(S);
    std::fill(v.begin(), v.end(), 1.0);

    for (size_t n = 0; n < N; ++n) {
        std::for_each(v.begin(), v.end(), let(_f = cref(factor), _x = 0.0)[
            _x = _f * bind(&identity, _1),
            _x = -bind(&number<double>::complement, d, _x),
            ref(sum) += bind(p, _x)
        ]);
    }

    std::cout << sum << std::endl;
    assert(sum == N * S);
    return 0;
}

