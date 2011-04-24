
#include <boost/chrono.hpp>
#include <boost/local/function.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include "benchmark_helpers.hpp"

int main(int argc, char* argv[]) {
    boost::chrono::duration<double> sec;
    unsigned long loop; unsigned long size; bool verbose; double n;
    begin(argc, argv, loop, size, verbose, n);

    double sum = 0.0;
    int factor = 1;

    boost::chrono::system_clock::time_point start =
            boost::chrono::system_clock::now();
    void BOOST_LOCAL_FUNCTION_PARAMS( (const double& num)
            (bind& sum) (const bind& factor) ) {
        sum += factor * num;
    } BOOST_LOCAL_FUNCTION_NAME(add)
    sec += boost::chrono::system_clock::now() - start;
    if (verbose) {
        std::clog << "declaration run-time = " << sec.count() << "s" <<
                std::endl;
    }

    std::vector<double> v(size);
    std::fill(v.begin(), v.end(), 1.0);

    for (unsigned long i = 0; i < loop; ++i) {
        boost::chrono::system_clock::time_point start =
                boost::chrono::system_clock::now();
        std::for_each(v.begin(), v.end(), add);
        sec += boost::chrono::system_clock::now() - start;
    }

    end(verbose, n, sum, sec);
    return 0;
}

