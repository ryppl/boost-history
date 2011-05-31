
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#ifndef DECREMENT_BUTTON_HPP_
#define DECREMENT_BUTTON_HPP_

#include "push_button.hpp"
#include "counter.hpp"
#include "../observer/observer.hpp"
#include <contract.hpp>
#include <boost/utility.hpp>

class decrement_button: public push_button, protected observer,
        // This class is non-copyable but postconditions can still
        // access its old values (see below).
        boost::noncopyable {

    CONTRACT_CLASS( (decrement_button) (push_button) (observer) )

public:
    // Creation //

    /** Create button associated with given counter. */
    CONTRACT_CONSTRUCTOR(
    (public) (explicit) (decrement_button)( (counter&)(the_counter) )
            (counter_ref_(the_counter))
        (postcondition)( (enabled() == (the_counter.value() > 0)) )
    ({
        counter_ref_.attach(this);
    }) )

    /** Destroy button. */
    CONTRACT_DESTRUCTOR(
    (public) (virtual) (decrement_button)( (void) )
    ({
    }) )

    // Commands //

    CONTRACT_FUNCTION(
    (public) (void) (on_bn_clicked)( (void) ) (copyable)
        (precondition)( (enabled()) )
        (postcondition)(
            (counter_ref_.value() ==
                    (CONTRACT_OLDOF(this)->counter_ref_.value() - 1))
        )
    ({
        counter_ref_.decrement();
    }) )
    
private:
    decrement_button& operator=(const decrement_button&); // Hide.

    CONTRACT_FUNCTION(
    (private) (bool) (up_to_date_with_subject)( (void) ) (const)
    ({
        return true; // For simplicity, always return true.
    }) )

    CONTRACT_FUNCTION(
    (private) (void) (update)( (void) )
        (postcondition) (
            (enabled() == (counter_ref_.value() > 0))
        )
    ({
        if (0 == counter_ref_.value()) disable();
        else enable();
    }) )

    counter& counter_ref_;
    
    // Allow access to contracts to implement special "copy".
    friend class contract::copy<decrement_button>;
};

// WARNING: Specialize `contract::copy` with care and only if really
// necessary (as a wrongly implemented specialization could brake
// contract constant-correctness guarantees).
//
// The decrement_button class has a reference member. The referenced
// counter (and not the counter reference!) are copied here to support
// object old value semantics in contract preconditions. This special
// "copy" is for contracts only -- in fact, decrement_button is
// `boost::noncopyable` and cannot be copied for general purposes.
//
// Similar techniques can be used to handle copy of objects with
// member pointers, in which case the `contract::copy` constructor
// allocates and copies the pointed objects (and not the pointers!)
// and the destructor deallocates them.
namespace contract {

template<>
class copy<decrement_button> {
private:
    // Not a reference type so it is copied.
    counter counter_; // Non-const but local to this class.
public:
    const decrement_button value;

    copy(const decrement_button& source):
            // Copy the referenced counter (not the reference).
            counter_(source.counter_ref_),
            // "Copy" decrement_button constructing one from
            // the copied counter.
            value(counter_) {}
};

} // namespace

#endif // #include guard

