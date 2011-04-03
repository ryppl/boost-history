
#include <iostream>
#include <vector>
#include <algorithm>

#define N 10000

template<typename R, typename A0>
struct abstract_function {
    virtual R operator()(A0) = 0;
};

template<typename R, typename A0>
struct function {
    function(abstract_function<R, A0>& ref): ptr_(&ref) {}
    R operator()(A0 a0) { return (*ptr_)(a0); }
private:
    abstract_function<R, A0>* ptr_;
};

int main() {
    double sum = 0.0;
    int factor = 10;

    struct add_function: abstract_function<void, const double&> {
        add_function(double& _sum, const int& _factor):
                sum_(_sum), factor_(_factor) {}
        void operator()(const double& num) { return body(num, sum_, factor_); }
    private:
        double& sum_;
        const int& factor_;
        void body(const double& num, double& sum, const int& factor) {
            sum += factor * num;
        }
    };
    add_function functor_add(sum, factor);
    function<void, const double&> add(functor_add);

    std::vector<double> v(N * 100);
    std::fill(v.begin(), v.end(), 10);
    
    for (size_t n = 0; n < N; ++n) {
        for (size_t i = 0; i < v.size(); ++i) {
//            functor_add(v[i]); // (1)
            add(v[i]);  // (2)
        }
        // std::for_each(v.begin(), v.end(), add); // (3) OK add as tparam!
    }

    std::cout << sum << std::endl;
    return 0;
}

