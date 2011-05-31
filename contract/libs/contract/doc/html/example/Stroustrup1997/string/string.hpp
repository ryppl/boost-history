
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#include <contract.hpp>

// Adapted from an example presented in [Stroustrup1997] to illustrate 
// importance of invariants. Simple preconditions were added where it
// made sense. This should be compiled with postconditions checking
// turned off because postconditions are deliberately not used.
// See [Stroustrup1997] for a discussion on the importance of
// invariants, and on pros and cons of using pre and post conditions.
class String {

    CONTRACT_CLASS( (String)
    (invariant) (
        // It would be better to assert invariants conditions separately
        // so to generate more informative error in case thay are broken.
        (p && sz >= 0 && TOO_LARGE >= sz && !p[sz] ? true : throw Invariant())
    ) )

    int sz;
    char* p;

public:
    // Broken contracts throw user defined exceptions.
    class Range {};                 // exception classes
    class Invariant {};
    class Null {};
    class Too_large {};

    enum { TOO_LARGE = 16000 };     // length limit

    CONTRACT_CONSTRUCTOR(
    (public) (String)( (const char*)(q) )
        (precondition)(
            (q ? true : throw Null())
            (strlen(q) <= TOO_LARGE ? true : throw Too_large())
        )
    (
        ;
    ) )

    CONTRACT_CONSTRUCTOR(
    (public) (String)( (const String&)(s) )
    (
        ;
    ) )

    CONTRACT_DESTRUCTOR(
    (public) (String)( (void) )
    (
        ;
    ) )

    CONTRACT_FUNCTION(
    (public) (char&) (operator([], at))( (int)(i) )
        (precondition)(
            (i >= 0 && sz > i ? true : throw Range())
        )
    (
        ;
    ) )

    CONTRACT_FUNCTION(
    (public) (int) (size)( (void) )
    (
        ;
    ) )

private:
    CONTRACT_FUNCTION( // not public so it does not check invariants
    (private) (void) (init)( (const char*)(q) )
    (
        ;
    ) )
};

