
#include <boost/local/function.hpp>

#define WITH_DEFAULT(value) , default value

// OK.

char& BOOST_LOCAL_FUNCTION_PARAMS( (int x) (double y)(default 1.23)
        (bind a) (bind& b) (const bind c) (const bind this) (const bind& d) )
char& BOOST_LOCAL_FUNCTION_PARAMS(int x, double y WITH_DEFAULT(1.23), // C99 (ok for MSVC)
        bind a, bind& b, const bind c, const bind this, const bind& d)

// Void.

char& BOOST_LOCAL_FUNCTION_PARAMS(void)
char& BOOST_LOCAL_FUNCTION_PARAMS( (void) )
char& BOOST_LOCAL_FUNCTION_PARAMS() // C99 (or trick on MSVC...)

// Errors.
    
char& BOOST_LOCAL_FUNCTION_PARAMS( (default -1) (int x)
        (double y)(default 1.23)
        (bind a) (bind& b) (const bind c) (const bind this) (const bind& d) )
char& BOOST_LOCAL_FUNCTION_PARAMS(default -1, int x, 
        double y, default 1.23, // C99
        bind a, bind& b, const bind c, const bind this, const bind& d)

char& BOOST_LOCAL_FUNCTION_PARAMS( (int x) (double y)(default 1.23)
        (bind a) (bind& b) (default -1) (const bind this) (const bind& d) )
char& BOOST_LOCAL_FUNCTION_PARAMS(int x, double y, default 1.23,
        bind a, bind& b, default -1, const bind this, const bind& d)

char& BOOST_LOCAL_FUNCTION_PARAMS( (bind this) (int x) (bind this)
        (bind a) (const bind this) (const bind& d) )
char& BOOST_LOCAL_FUNCTION_PARAMS(bind this, int x, bind this,
        bind a, const bind this, const bind& d)


BOOST_LOCAL_FUNCTION_NAME(l)

