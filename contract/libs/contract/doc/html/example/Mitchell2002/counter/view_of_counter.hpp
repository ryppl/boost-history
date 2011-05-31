
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#ifndef VIEW_OF_COUNT_HPP_
#define VIEW_OF_COUNT_HPP_

#include "../observer/observer.hpp"
#include "counter.hpp"
#include <contract.hpp>
#include <iostream> // To view counter value on console.

/** Show current value of associated counter. */
class view_of_counter: private observer {

    CONTRACT_CLASS( (view_of_counter) (observer) )

public:
    // Creation //

    /** Create viewer associated with given counter. */
    CONTRACT_CONSTRUCTOR(
    (public) (explicit) (view_of_counter)( (counter&)(the_counter) )
            (counter_ref_(the_counter))
    ({
        counter_ref_.attach(this);
        std::cout << std::endl << ">> Counter started at "
                << counter_ref_.value() << std::endl << std::endl;
    }) )

private:
    CONTRACT_FUNCTION(
    (private) (bool) (up_to_date_with_subject)( (void) ) (const)
    ({
        return true; // For simplicity, always return true.
    }) )

    CONTRACT_FUNCTION(
    (private) (void) (update)( (void) )
    ({
        std::cout << std::endl << ">> Counter changed to "
                << counter_ref_.value() << std::endl << std::endl;
    }) )

    counter& counter_ref_;
};

#endif // #include guard

