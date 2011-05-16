
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

// Tricks to "pass" a local class as a template parameter.

//[tparam_trick_cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

// Casting functor trick.
struct casting_func {
    explicit casting_func(void* obj, void (*call)(void*, const double&)):
            obj_(obj), call_(call) {}
    // Unfortunately, function pointer call is not inlined.
    inline void operator()(const double& num) { call_(obj_, num); }
private:
    void* obj_;
    void (*call_)(void*, const double&);
};

// Virtual functor trick.
struct virtual_func {
    struct interface {
        // Unfortunately, virtual function call is not inlined.
        inline virtual void operator()(const double&) {}
    };
    explicit virtual_func(interface& func): func_(&func) {}
    inline void operator()(const double& num) { (*func_)(num); }
private:
    interface* func_;
};

// Local class for local function.
int main() {
    double sum = 0.0;
    int factor = 10;

    struct local_add : virtual_func::interface {
        explicit local_add(double& _sum, const int& _factor):
                sum_(_sum), factor_(_factor) {}
        inline void operator()(const double& num) {
            body(sum_, factor_, num);
        }
        inline static void call(
                void* obj, const double& num) {
            local_add* self = static_cast<local_add*>(obj);
            self->body(self->sum_, self->factor_, num);
        }
    private:
        double& sum_;
        const int& factor_;
        inline void body(double& sum, const int& factor, const double& num) {
            sum += factor * num;
        }
    } add_local(sum, factor);
    casting_func add_casting(&add_local, &local_add::call);
    virtual_func add_virtual(add_local);

    std::vector<double> v(10);
    std::fill(v.begin(), v.end(), 1.0);
    
//  std::for_each(v.begin(), v.end(), add_local); // Error but OK on C++0x.
    std::for_each(v.begin(), v.end(), add_casting); // OK.
    std::for_each(v.begin(), v.end(), add_virtual); // OK.

    std::cout << sum << std::endl;
    return 0;
}
//]

