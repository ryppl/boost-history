
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

#define N 10000

//struct func {
//    func(void* obj, void (*call)(void*, const double&)):
//            obj_(obj), call_(call) {}
//    inline void operator()(const double& num) { call_(obj_, num); }
//private:
//    void* obj_;
//    void (*call_)(void*, const double&);
};

int main() {
    double sum = 0.0;
    int factor = 10;

    struct local_add {
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
    void* obj_ = &add_l;
    void (*call_)(void*, const double&) = &local_add::call;
//    func add_f(&add_l, &local_add::call);

    std::vector<double> v(N);
    std::fill(v.begin(), v.end(), 10);
    
    for (size_t n = 0; n < N; ++n) {
        for (size_t i = 0; i < v.size(); ++i) {
//            local_add::call(&add_l, v[i]); // (1) runs in 16s
            call_(obj_, v[i]); // (2) runs in 16s
        }
    }

    std::cout << sum << std::endl;
    return 0;
}

