
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#ifndef PUSHABLE_HPP_
#define PUSHABLE_HPP_

//[ pushable_cpp

#include <contract.hpp>

template<typename T>
class pushable {

    CONTRACT_CLASS( (pushable) )

public:
    virtual ~pushable(void) {}

    // Contract for pure virtual function.
    CONTRACT_FUNCTION(
    (public) (virtual) (void) (push_back)( (const T&)(element) )
        (postcondition)( (back() == element) )
    (
        = 0; // Pure virtual body.
    ) ) 

    virtual const T& back(void) const = 0;
};

//]

#endif // #include guard


