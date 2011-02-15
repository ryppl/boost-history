
#include "function_params.hpp"

/** @todo Add to docs that this can never be bound by ref (as in C++). */

/** @todo Add to docs that MSVC is not C99 because it does not support empty params (so `()` can't be used for empty params and `void` -- or `(void)` -- should be used. However, MSVC supports variadics you can use `..., ...` to separate params instead of parenthesis `(...)...`. */

/** @todo Add to docs that if `, default ...` syntax seems strange for C99, programmers can always #define and use the following macro (or somenthing similar). */
#define WITH_DEFAULT(x) , default x

// OK.

BOOST_LOCAL_FUNCTION_PARAMS( (int x) (double y)(default 1.23)
        (bind a) (bind& b) (const bind c) (const bind this) (const bind& d) )
BOOST_LOCAL_FUNCTION_PARAMS(int x, double y WITH_DEFAULT(1.23), // C99 (ok for MSVC)
        bind a, bind& b, const bind c, const bind this, const bind& d)

// Void.

BOOST_LOCAL_FUNCTION_PARAMS(void)
BOOST_LOCAL_FUNCTION_PARAMS( (void) )
//BOOST_LOCAL_FUNCTION_PARAMS() // C99 (not MSVC)

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

