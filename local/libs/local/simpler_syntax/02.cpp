
#include <boost/preprocessor.hpp>
#include <boost/preprocessor/detail/is_unary.hpp>
#include <boost/preprocessor/facilities/is_empty.hpp>
#include <boost/detail/preprocessor/keyword/void.hpp>


/// VOID ///
    
#define BOOST_LOCAL_AUX_FUNCTION_PARAMS_IS_VOID_TOKEN_(token) \
    BOOST_PP_IIF(BOOST_PP_IS_EMPTY(token), \
        1 /* handles empty params `()` as no params (C99 only) */ \
        BOOST_PP_EMPTY \
    , \
        BOOST_DETAIL_PP_KEYWORD_IS_VOID_FRONT \
    )(token)

#define BOOST_LOCLAL_AUX_FUNCTION_PARAMS_IS_VOID_SEQ_(params_seq) \
    BOOST_LOCAL_AUX_FUNCTION_PARAMS_IS_VOID_TOKEN_( \
            BOOST_PP_SEQ_HEAD(params_seq))

// Expand to 1 iff params_seq is empty (C99 only), or `void` (supported also
// for C++ but similar to unparenthesized paramter syntax for C99), or `(void)`
// (parenthesized parameter syntax for C++).
#define BOOST_LOCAL_AUX_FUNCTION_PARAMS_IS_VOID(params_seq) \
    BOOST_PP_IIF(BOOST_PP_IS_UNARY(params_seq), \
        BOOST_LOCLAL_AUX_FUNCTION_PARAMS_IS_VOID_SEQ_ \
    , /* else, it's a sequence */ \
        BOOST_LOCAL_AUX_FUNCTION_PARAMS_IS_VOID_TOKEN_ \
    )(params_seq)

//#define BOOST_LOCAL_AUX_FUNCTION_PARAMS_IS_VOID(params_seq) 
//    params_seq --> BOOST_PP_IS_EMPTY(params_seq)


/// PARAMS ///

#define BOOST_LOCAL_AUX_FUNCTION_PARAMS(params_seq) \
    BOOST_LOCAL_AUX_FUNCTION_PARAMS_IS_VOID(params_seq)



BOOST_LOCAL_AUX_FUNCTION_PARAMS( (int x) (double y)(default 1.23)
        (bind a) (const bind b) (bind& c) (const bind& d) )

BOOST_LOCAL_AUX_FUNCTION_PARAMS(void)

BOOST_LOCAL_AUX_FUNCTION_PARAMS( (void) )

BOOST_LOCAL_AUX_FUNCTION_PARAMS()

