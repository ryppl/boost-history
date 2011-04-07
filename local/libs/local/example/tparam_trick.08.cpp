
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

#define N 1e4
#define S N * 1e2

struct func_if {
    inline virtual void operator()(const double&) = 0;
};

struct virtual_func {
    explicit virtual_func(func_if& ref): ptr_(&ref) {}
    inline void operator()(const double& num) { (*ptr_)(num); }
private:
    func_if* ptr_;
};

struct casting_func {
    explicit casting_func(void* obj, void (*call)(void*, const double&)):
            obj_(obj), call_(call) {}
    inline void operator()(const double& num) { call_(obj_, num); }
private:
    void* obj_;
    void (*call_)(void*, const double&);
};

int main() {
    double sum = 0.0;
    int factor = 10;

    struct local_add: func_if {
        explicit local_add(double& _sum, const int& _factor):
                sum_(_sum), factor_(_factor) {}
        inline void operator()(const double& num) { body(sum_, factor_, num); }
        inline static void call(void* obj, const double& num) {
            local_add* self = static_cast<local_add*>(obj);
            self->body(self->sum_, self->factor_, num);
        }
    private:
        double& sum_;
        const int& factor_;
        inline void body(double& sum, const int& factor, const double& num) {
            sum += factor * num;
        }
    };
    local_add add_local(sum, factor);

    void* obj = &add_local;
    void (*call)(void*, const double&) = &local_add::call;

    casting_func add_casting(&add_local, &local_add::call);
    virtual_func add_virtual(add_local);

    std::vector<double> v(S);
    std::fill(v.begin(), v.end(), 10);
    
    for (size_t n = 0; n < N; ++n) {
        for (size_t i = 0; i < v.size(); ++i) {
//            local_add::call(&add_local, v[i]); // (1) runs in 19s
//            call(obj, v[i]); // (2) runs in 20s 
//            add_casting(v[i]); // (3) runs in 23s (or 25s w/ func_if base)
            add_virtual(v[i]); // (4) runs in 21s
        }
    }

    std::cout << sum << std::endl;
    return 0;
}

