
#include <boost/spirit/include/phoenix.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    using boost::phoenix::let;
    using boost::phoenix::local_names::_f;
    using boost::phoenix::cref;
    using boost::phoenix::ref;
    using boost::phoenix::arg_names::_1;
    using boost::phoenix::val;

    double sum = 0.0;
    int factor = 10;

    std::vector<double> v(1000000);
    std::fill(v.begin(), v.end(), 10);
    for (size_t n = 0; n < 10000; ++n) {
        std::for_each(v.begin(), v.end(), (
            ref(sum) += factor * _1
        ));
    }

    std::cout << sum << std::endl;
    return 0;
}

