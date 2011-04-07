
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

#define N 10000

struct base_function {
    virtual void operator()(const double&) {
        std::cout << "parent\n";
    }
};

struct function {
    function(void* obj, void (*call)(void*, const double&)):
            obj_(obj), call_(call) {}
    inline void operator()(const double& arg1) { 
        call_(obj_, arg1);
    }
private:
    void* obj_;
    void (*call_)(void*, const double&);
};

int main() {
    double sum = 0.0;
    int factor = 10;

    struct local_add: base_function {
        local_add(double& _sum, const int& _factor):
                sum_(_sum), factor_(_factor) {}
        void operator()(const double& num) { body(sum_, factor_, num); }
        static void call(void* obj, const double& num)
            { static_cast<local_add*>(obj)->operator()(num); }
    private:
        double& sum_;
        const int& factor_;
        void body(double& sum, const int& factor, const double& num) {
            sum += factor * num;
        }
    };
    local_add add_l(sum, factor);
    base_function& add_b = add_l;
    function add_f(&add_l, local_add::call);

    void* obj_ = &add_l;
    register void (*call_)(void*, const double&) = &local_add::call;

    std::vector<double> v(N * 100);
    std::fill(v.begin(), v.end(), 10);
    
    for (size_t n = 0; n < N; ++n) {
//        std::for_each(v.begin(), v.end(), add_l); // Error.
//        std::for_each(v.begin(), v.end(), add_b);
//        std::for_each(v.begin(), v.end(), add_f);
        for (size_t i = 0; i < v.size(); ++i) {
//            add_l(v[i]);
//            add_b(v[i]);
//            add_f(v[i]);
//            local_add::call(&add_l, v[i]);            
            call_(obj_, v[i]);
        }
    }

    std::cout << sum << std::endl;
    return 0;
}

