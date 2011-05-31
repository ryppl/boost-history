
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#ifndef PUSH_BUTTON_HPP_
#define PUSH_BUTTON_HPP_

#include <contract.hpp>

/** Basic button. */
class push_button {

    CONTRACT_CLASS( (push_button) )

public:
    // Creation //

    /** Create button. */
    CONTRACT_CONSTRUCTOR(
    (public) (push_button)( (void) ) (enabled_(true))
    ({
    }) )

    CONTRACT_DESTRUCTOR(
    (public) (virtual) (push_button)( (void) )
    ({
    }) )

    // Queries //
    
    /** If button enabled. */
    CONTRACT_FUNCTION(
    (public) (bool) (enabled)( (void) ) (const)
    ({
        return enabled_;
    }) )

    // Commands //

    /** Enable this button. */
    CONTRACT_FUNCTION(
    (public) (void) (enable)( (void) )
        (postcondition)( (enabled()) )
    ({
        enabled_ = true;
    }) )

    /** Disable this button. */
    CONTRACT_FUNCTION(
    (public) (void) (disable)( (void) )
        (postcondition)( (bool(!enabled())) )
    ({
        enabled_ = false;
    }) )

    /** Invoked externally when this button is clicked. */
    CONTRACT_FUNCTION(
    (public) (virtual) (void) (on_bn_clicked)( (void) )
    (
        = 0;
    ) )

private:
    bool enabled_;
};

#endif // #include guard

