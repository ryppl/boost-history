
#ifndef BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_VALIDATE_HPP_
#define BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_VALIDATE_HPP_

#include "../params/error.hpp"
#include "../params/const_bind.hpp"
#include "../params/bind.hpp"
#include "../parsed_/params/error.hpp"
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/facilities/is_empty.hpp>
#include <boost/preprocessor/tuple/eat.hpp>
#include <boost/preprocessor/arithmetic/greater.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>

// Private API.

#define BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_VALIDATE_THIS_COUNT_(params) \
    BOOST_PP_IIF(BOOST_PP_GREATER(BOOST_PP_ADD( \
            BOOST_LOCAL_AUX_PP_SIGN_PARAMS_CONST_BIND_THIS_COUNT(params), \
            BOOST_LOCAL_AUX_PP_SIGN_PARAMS_BIND_THIS_COUNT(params)), 1), \
        BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_SET_ERROR \
    , \
        params BOOST_PP_TUPLE_EAT(2) \
    )(params, ERROR_object_this_cannot_be_bound_multiple_times)

// Public API.

#define BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_VALIDATE(params) \
    BOOST_PP_IIF(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_HAVE_ERROR(params), \
        BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_VALIDATE_THIS_COUNT_ \
    , \
        params BOOST_PP_TUPLE_EAT(1) /* fwd existing error */ \
    )(params)

#endif // #include guard

