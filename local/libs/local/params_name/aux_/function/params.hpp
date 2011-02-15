
#ifndef BOOST_LOCAL_AUX_FUNCTION_PARAMS_HPP_
#define BOOST_LOCAL_AUX_FUNCTION_PARAMS_HPP_

#include "../preprocessor/sign/parse_params.hpp"
#include "../preprocessor/sign/params_error.hpp"
#include <boost/mpl/assert.hpp>
#include <boost/preprocessor/control/iif.hpp>

// Private API.

#define BOOST_LOCAL_AUX_FUNCTION_PARAMS_OK_(sign_params) \
    CODE(sign_params)

#define BOOST_LOCAL_AUX_FUNCTION_PARAMS_ERROR_(sign_params) \
    BOOST_MPL_ASSERT_MSG(0, BOOST_LOCAL_AUX_PP_SIGN_PARAMS_ERROR_MSG( \
            sign_params), ())

// sign_params: parsed parenthesized params.
#define BOOST_LOCAL_AUX_FUNCTION_PARAMS_(sign_params) \
    BOOST_PP_IIF(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_HAVE_ERROR(sign_params), \
        BOOST_LOCAL_AUX_FUNCTION_PARAMS_ERROR_ \
    , \
        BOOST_LOCAL_AUX_FUNCTION_PARAMS_OK_ \
    )(sign_params)

// Public API.

#define BOOST_LOCAL_AUX_FUNCTION_PARAMS(parenthesized_params) \
    BOOST_LOCAL_AUX_FUNCTION_PARAMS_(BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS( \
            parenthesized_params))

#endif // #include guard

