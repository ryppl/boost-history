
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


#define vaps \
    int x, double& y, const bind a, const bind& b, bind c, bind& d
    
#define ps \
    (int x) (double& y) (const bind a) (const bind& b) (bind c) (bind& d)

#define vap1 \
    int x
#define p1 \
    (int x)

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

HANDLE_PARAMS_VA(vaps)
HANDLE_PARAMS_VA(vap1)
HANDLE_PARAMS_VA(ps)
HANDLE_PARAMS_VA(p1)



