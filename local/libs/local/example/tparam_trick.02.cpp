
//#include <boost/spirit/include/phoenix.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

#define N 10000

struct abstract_function {
    virtual void operator()(const double&) = 0;
};

struct function {
    function(void* obj, void (*call)(void*, const double&)):
            obj_(obj), call_(call) {}
    inline void operator()(const double& arg1) { call_(obj_, arg1); }
private:
    void* obj_;
    void (*call_)(void*, const double&);
};

void call(void* obj, const double& num) {}

int main() {
    double sum = 0.0;
    int factor = 10;

    struct add_function: abstract_function {
        add_function(double& _sum, const int& _factor):
                sum_(_sum), factor_(_factor) {}
        void operator()(const double& num) { body(sum_, factor_, num); }
        static void call(void* obj, const double& num) {
            static_cast<add_function*>(obj)->operator()(num);
//            add_function* self = static_cast<add_function*>(obj);
//            return self->body(self->sum_, self->factor_, num);
        }
    private:
        double& sum_;
        const int& factor_;
        void body(double& sum, const int& factor, const double& num) {
            sum += factor * num;
        }
    };
    add_function adder(sum, factor);
    function add(&adder, &add_function::call);

    std::vector<double> v(N * 100);
    std::fill(v.begin(), v.end(), 10);
    
    for (size_t n = 0; n < N; ++n) {
        for (size_t i = 0; i < v.size(); ++i) {
//            adder(v[i]); // (1)
//            add_function::call(&adder, v[i]);
//            add(v[i]);
            boost::bind(add_function::call, (void*)&adder, _1)(v[i]);
//            v[i];
//            add();  // (2)
//            add_function::call(&adder);
//            add(v[i]);
        }
//        std::for_each(v.begin(), v.end(), add); // (3) OK add as tparam!
//        std::for_each(v.begin(), v.end(),
//            boost::phoenix::val(adder.f(10))
//        );
    }

    std::cout << sum << std::endl;
    return 0;
}

