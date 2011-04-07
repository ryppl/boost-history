
#include <iostream>
#include <vector>
#include <algorithm>

#define N 10000

struct abstract_function {
    virtual void operator()() = 0;
};

struct function {
    function(abstract_function& ref): ptr_(&ref) {}
    void operator()() { return ptr_->operator()(); }
private:
    abstract_function* ptr_;
};

int main() {
    double sum = 0.0;
    int factor = 10;

    struct add_function: abstract_function {
        add_function(double& _sum, const int& _factor):
                sum_(_sum), factor_(_factor) {}
        void operator()() { return body(sum_, factor_); }
    private:
        double& sum_;
        const int& factor_;
        void body(double& sum, const int& factor) {
            sum += factor * 10;
        }
    };
    add_function functor_add(sum, factor);
    function add(functor_add);

    std::vector<double> v(N * 100);
    std::fill(v.begin(), v.end(), 10);
    
    for (size_t n = 0; n < N; ++n) {
        for (size_t i = 0; i < v.size(); ++i) {
//            functor_add(v[i]); // (1)
            add();  // (2)
        }
        // std::for_each(v.begin(), v.end(), add); // (3) OK add as tparam!
    }

    std::cout << sum << std::endl;
    return 0;
}

