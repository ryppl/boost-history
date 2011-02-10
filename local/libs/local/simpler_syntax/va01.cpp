
#include <boost/variadic_macro_data/VariadicMacroData.hpp>
#include <boost/preprocessor.hpp>

// I would rename the library to Boost.Preprocessor/Variadics, use the
// prefix `BOOST_PP_VA` (from the language abbreviation "VA" of `__VA_ARGS__`),
// and put it into "boost/preprocessor/va/" (with one separate header for each
// `VA_XXX` macro and a general header "boost/preprocessor/va.hpp" as
// Boost.Preprocessor does).
// I would add the followings:
#define PP_VA_IDENTITY(...) __VA_ARGS__ BOOST_PP_EMPTY
// I find this usefull but PP does not have for not variadics.
#define PP_VA_SAME(...) __VA_ARGS__
#define PP_VA_EAT(...)
// Using variadics macros, it should be possible to support sequences of
// variable length tuples `(a) (b1, b2) (c1, c2, c3)` (PP only supports
// sequences of 1-tuple `(a) (b) (c)`). Is this the same of PP_LIST, etc?
// So I would provide a family of macros similar to PP_SEQ but that work with
// variable length tuples:
// PP_VA_SEQ_SIZE, PP_VA_SEQ_ELEM, PP_VA_SEQ_FOR_EACH, etc.


// PARENTHESIZED SYNTAX ENHANCMENTS FOR C99
//
// C99 VS. ISO C++ PREPROCESSOR
// On C99 preprocessor, the same macros simply accept multiple syntaxes. The
// parenthesized syntax as well as the variable length tuple syntax. For
// example, on C99 the same macros will accept and parse both [1] and [2]
// below, however on non-C99 preprocessor [2] will generate (nasty)
// preprocessor errors and only [1] will compile.
//
// FUNCTION PARAMETERS
// Function parameter can be specified as (but no type/name separation :(( ):
//     (void) (f)( (int x) (double& y) (const bind a) ) // [1]
// and also as (C99 only):
//     (void) (f)(int x, double& y, const bind a) // [2] Usual C++ syntax :))
//
// DEFAULT PARAMETER VALUES
// Empty parameters can be specified as:
//     (void) (f)( (void) ) // [3]
// and also as (C99 only):
//     (void) (f)() // [4] Usual C++ syntax :))
//
// Default parameter values can be specified as:
//     (void) (f)( (int x)(default 0)   (double y)(default 1.23) ) // [5]
// and also as (C99 only):
//     (void) (f)( (int x, default 0) , (double y, default 1.23) ) // [6]
// and also mix/matching all of the above (C99 only):
//     (void) (f)( (int x)(default 0) , (double y)(default 1.23) ) // [7]
//     (void) (f)( (int x, default 0)   (double y, default 1.23) ) // [8]
//     (void) (f)(  int x, default 0  ,  double y, default 1.23  ) // [9]
// This is kind of confusing... too many options. I can't decide which one is
// the best because they are all different from usual C++ syntax so I just
// prefer the parenthesized syntax without C99 features...
// Boosters, what do you think?
//




#define vaf \
    (void) (f)(int x, double& y, const bind a, const bind& b, bind c, bind& d)
    
#define f \
    (void) (f)( (int x) (double& y) (const bind a) (const bind& b) (bind c) (bind& d) )

#define vaf1 \
    (void) (f)(int x)
#define f1 \
    (void) (f)(int x)

/// ARE_PARAMS_VA ///

#define ARE_PARAMS_VA_UNARY_(...) BOOST_PP_NOT(BOOST_PP_IS_UNARY(__VA_ARGS__))

#define ARE_PARAMS_VA(...) \
    BOOST_PP_IIF(BOOST_PP_GREATER(VMD_DATA_SIZE(__VA_ARGS__), 1), \
        1 PP_VA_EAT \
    , \
        ARE_PARAMS_VA_UNARY_ \
    )(__VA_ARGS__)

/////////////////////


//>> ARE_PARAMS_VA(vap1) <<
//>> ARE_PARAMS_VA(vaps) <<
//>> ARE_PARAMS_VA(p1) <<
//>> ARE_PARAMS_VA(ps) <<


#define HANDLE_PARAMS_VA(...) \
    BOOST_PP_IIF(ARE_PARAMS_VA(__VA_ARGS__), \
        VMD_DATA_TO_PP_SEQ \
    , \
        PP_VA_SAME \
    )(__VA_ARGS__)

//HANDLE_PARAMS_VA(vaps)
//HANDLE_PARAMS_VA(vap1)
//HANDLE_PARAMS_VA(ps)
//HANDLE_PARAMS_VA(p1)

BOOST_PP_EXPAND(HANDLE_PARAMS BOOST_PP_SEQ_TAIL(BOOST_PP_SEQ_TAIL(vaf)) )
BOOST_PP_SEQ_TAIL(BOOST_PP_SEQ_TAIL(vaf1))
BOOST_PP_SEQ_TAIL(BOOST_PP_SEQ_TAIL(f))
BOOST_PP_SEQ_TAIL(BOOST_PP_SEQ_TAIL(f1))



