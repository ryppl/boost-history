
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#ifndef COUNTER_HPP_
#define COUNTER_HPP_

#include "../observer/subject.hpp"
#include <contract.hpp>

/** Positive integer counter. */
class counter: public subject {

    CONTRACT_CLASS( (counter) (subject) )

public:
    // Creation //

    /** Construct counter. */
    CONTRACT_CONSTRUCTOR(
    (public) (explicit) (counter)( (const int&) (value) (default)(10) )
            (value_(value))
    ({
    }) )

    /** Destroy counter. */
    CONTRACT_DESTRUCTOR(
    (public) (virtual) (counter)( (void) )
    ({
    }) )

    // Queries //

    /** Counter current value. */
    CONTRACT_FUNCTION(
    (public) (int) (value)( (void) ) (const)
    ({
        return value_;
    }) )

    // Commands //

    /** Decrement counter value. */
    CONTRACT_FUNCTION(
    (public) (void) (decrement)( (void) ) (copyable)
        (postcondition)(
            (value() ==
                (CONTRACT_OLDOF(this)->value() - 1))
        )
    ({
        --value_;
        notify(); // Notifies all attached observers.
    }) )

private:
    int value_;
};

#endif // #include guard

