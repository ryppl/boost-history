
#include <boost/local/function.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

#define N 30000

int main() {
    double sum = 0.0;
    int factor = 10;

    std::vector<double> v(N);
    std::fill(v.begin(), v.end(), 10);

    void BOOST_LOCAL_FUNCTION_PARAMS( (double num)
            (bind& sum) (const bind factor)
    ) {
        sum += factor * num;
    } BOOST_LOCAL_FUNCTION_NAME(add)

    for (size_t n = 0; n < N; ++n) {
        std::for_each(v.begin(), v.end(), add);
//        for (size_t i = 0; i < v.size(); ++i) {
//            sum += factor * v[i];//10;
//            add(v[i]);
//        }
    }

    std::cout << sum << std::endl;
    return 0;
}

