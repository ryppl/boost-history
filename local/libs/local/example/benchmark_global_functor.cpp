
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

#define N 1e4
#define S N * 1e2

struct global_add {
    global_add(double& _sum, int _factor): sum(_sum), factor(_factor) {}
    void operator()(const double& num) {
        sum += factor * num;
    }
private:
    double& sum;
    const int factor;
};

int main() {
    double sum = 0.0;
    int factor = 1;

    std::vector<double> v(S);
    std::fill(v.begin(), v.end(), 1.0);

    global_add add(sum, factor);

    for (size_t n = 0; n < N; ++n) {
        std::for_each(v.begin(), v.end(), add);
    }

    std::cout << sum << std::endl;
    assert(sum == N * S);
    return 0;
}

