
#include <boost/chrono.hpp>
#include <boost/lambda/lambda.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include "profile_helpers.hpp"

int main(int argc, char* argv[]) {
    unsigned long size = 0, trials = 0;
    begin(argc, argv, size, trials);

    double sum = 0.0;
    int factor = 1;

    std::vector<double> v(size);
    std::fill(v.begin(), v.end(), 1.0);

    boost::chrono::duration<double> trials_sec;
    for (unsigned long i = 0; i < trials; ++i) {
        boost::chrono::system_clock::time_point start =
                boost::chrono::system_clock::now();

        using boost::lambda::_1;
        std::for_each(v.begin(), v.end(), (
            sum += factor * _1
        ));

        trials_sec += boost::chrono::system_clock::now() - start;
    }

    end(size, trials, sum, trials_sec.count());
    return 0;
}

