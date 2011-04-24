
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>

int main(int argc, char* argv[]) {
    assert(argc == 2);
    unsigned long n = sqrt(double(atol(argv[1])));

    double sum = 0.0;
    int factor = 1;

    struct local_add {
        local_add(double& _sum, int _factor): sum(_sum), factor(_factor) {}
        void operator()(const double& num) {
            sum += factor * num;
        }
    private:
        double& sum;
        const int factor;
    };
    local_add add(sum, factor);

    std::vector<double> v(n);
    std::fill(v.begin(), v.end(), 1.0);
    for (unsigned long i = 0; i < n; ++i) {
//        for (size_t j = 0; j < v.size(); ++j) add(v[j]); // Can't use for_each.
    }

    std::cout << sum << std::endl;
//    assert(sum == n * n);
    return 0;
}

