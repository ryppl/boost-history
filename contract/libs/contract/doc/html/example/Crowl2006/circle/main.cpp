
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#include <contract.hpp>
#include <iostream>

class shape {

    CONTRACT_CLASS( (shape) )

public:
    virtual ~shape(void)
    CONTRACT_DESTRUCTOR(
            (public) (virtual) (shape)( (void) )
    (body) ({
    }) )

    virtual int compute_area(void) const
    CONTRACT_FUNCTION(
            (public) (virtual) (int) (compute_area)( (void) ) (const)
    (postcondition) (result) ({
        CONTRACT_ASSERT( result > 0 );
    })
    (body) (
        = 0; // Contract for pure virtual function.
    ) )
};

class circle: public shape {
    
    CONTRACT_CLASS( (circle) (shape) ) // Subcontracting from `shape`.

public:
    // Hard-coded radius for simplicity in this example.
    int radius(void) const { return 2; }

    virtual int compute_area(void) const
    CONTRACT_FUNCTION( // Inherit `shape::copute_area()` contract.
            (public) (virtual) (int) (compute_area)( (void) ) (const)
    (postcondition) (result) ({
        CONTRACT_ASSERT( result == PI * radius() * radius() );
    })
    (body) ({
        return PI * radius() * radius();
    }) )

private:
    static const int PI = 3; // Truncated int of 3.14...
};

int main() {
    circle c;

    std::cout << std::endl << "compute_area()" << std::endl;
    std::cout << c.compute_area() << std::endl;
    
    return 0;
}

