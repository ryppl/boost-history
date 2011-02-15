
#include "function_params.hpp"

/** @todo Add to doc that this can never be bound by ref (as in C++) */

// OK.

// If `, default ...` syntax seems strange on C99, programmers can always do:
#define WITH_DEFAULT(x) , default x

BOOST_LOCAL_FUNCTION_PARAMS( (int x) (double y)(default 1.23)
        (bind a) (bind& b) (const bind c) (const bind this) (const bind& d) )
BOOST_LOCAL_FUNCTION_PARAMS(int x, double y WITH_DEFAULT(1.23), // C99
        bind a, bind& b, const bind c, const bind this, const bind& d)

// Void.

BOOST_LOCAL_FUNCTION_PARAMS(void)
BOOST_LOCAL_FUNCTION_PARAMS( (void) )
BOOST_LOCAL_FUNCTION_PARAMS() // C99

// Errors.
    
BOOST_LOCAL_FUNCTION_PARAMS( (default -1) (int x) (double y)(default 1.23)
        (bind a) (bind& b) (const bind c) (const bind this) (const bind& d) )
BOOST_LOCAL_FUNCTION_PARAMS(default -1, int x, double y, default 1.23, // C99
        bind a, bind& b, const bind c, const bind this,const bind& d)

BOOST_LOCAL_FUNCTION_PARAMS( (int x) (double y)(default 1.23)
        (bind a) (bind& b) (default -1) (const bind this) (const bind& d) )
BOOST_LOCAL_FUNCTION_PARAMS(int x, double y, default 1.23,
        bind a, bind& b, default -1, const bind this, const bind& d)

BOOST_LOCAL_FUNCTION_PARAMS( (bind this) (int x) (bind this)
        (bind a) (const bind this) (const bind& d) )
BOOST_LOCAL_FUNCTION_PARAMS(bind this, int x, bind this,
        bind a, const bind this, const bind& d)

