
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#include <contract.hpp>
#include <iostream>

//[ friend_cpp

class point {
public:
    point(const double& x, const double& y): x_(x), y_(y) {}

    // Declare friend the body and separate the definition.
    friend std::ostream& CONTRACT_BODY(operator(<<, out))(
            std::ostream& s, const point& o);

private:
    double x_;
    double y_;
};
    
std::ostream& operator<<(std::ostream& s, const point& o)
CONTRACT_FUNCTION( (std::ostream&) (operator(<<, out))(
        (std::ostream&)(s) (const point&)(o) )
(body) ({
    return s << "(" << o.x_ << ", " << o.y_ << ")";
}) )

//]

int main() {
    point pt(1.2, 3.4);
    std::cout << pt << std::endl;
    return 0;
}

