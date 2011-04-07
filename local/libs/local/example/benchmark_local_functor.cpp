
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

#define N 1e4
#define S N * 1e2

int main() {
    double sum = 0.0;
    int factor = 1;

    std::vector<double> v(S);
    std::fill(v.begin(), v.end(), 1.0);

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

    for (size_t n = 0; n < N; ++n) {
        for (size_t i = 0; i < v.size(); ++i) add(v[i]); // Can't use for_each.
    }

    std::cout << sum << std::endl;
    assert(sum == N * S);
    return 0;
}

