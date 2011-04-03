
#include <iostream>
#include <vector>
#include <algorithm>

#define N 10000

int main() {
    double sum = 0.0;
    int factor = 10;

    struct local_add {
        local_add(double& _sum, int _factor): sum(_sum), factor(_factor) {}
        void operator()(double num) {
            sum += factor * num;
        }
    private:
        double& sum;
        const int factor;
    };
    local_add add(sum, factor);

    std::vector<double> v(N * 100);
    std::fill(v.begin(), v.end(), 10);

    for (size_t n = 0; n < N; ++n) {
        for (size_t i = 0; i < v.size(); ++i) {
            add(v[i]);
        }
    }

    std::cout << sum << std::endl;
    return 0;
}

