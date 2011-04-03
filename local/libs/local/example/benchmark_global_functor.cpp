
#include <iostream>
#include <vector>
#include <algorithm>

#define N 10000

struct global_add {
    global_add(double& _sum, int _factor): sum(_sum), factor(_factor) {}
    void operator()(double num) {
        sum += factor * num;
    }
private:
    double& sum;
    const int factor;
};

int main() {
    double sum = 0.0;
    int factor = 10;

    std::vector<double> v(N * 100);
    std::fill(v.begin(), v.end(), 10);

    global_add add(sum, factor);

    for (size_t n = 0; n < N; ++n) {
        std::for_each(v.begin(), v.end(), add);
    }

    std::cout << sum << std::endl;
    return 0;
}

