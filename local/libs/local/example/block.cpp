
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

#include <boost/local/block.hpp>
#include <cassert>

struct c {
    c(): x_(0) {}

    void f(int& x) { // Non-const member function so `this` is not const.
        // Non-const `this` but const `x`.
        BOOST_LOCAL_BLOCK( (const bind& x) (bind this) ) {
            this_->x_ = x; // Non-const `this` cant be modified.
            assert(x == this_->x_); // Compiler error if `=` instead of `==`.
            
            return; // Exit local block (not enclosing function).
            assert(false); // Never executed.
        } BOOST_LOCAL_BLOCK_END

        x = x_; // Non-const `x` changed outside block.
    }

private:
    int x_;
};

int main() {
    c cc;
    int x = 1;
    cc.f(x);
    return 0;
}

