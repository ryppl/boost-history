
#ifndef BOOST_LOCAL_AUX_PP_SIGN_PARAMS_ERROR_HPP_
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_ERROR_HPP_

#include "../parsed_/params/index.hpp"
#include <boost/preprocessor/tuple/elem.hpp>

// Expand to `EMPTY` if no error, to `ERROR_message_text EMPTY` otherwise.
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_ERROR(params) \
    BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_INDEX_MAX, \
            BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_INDEX_ERROR, \
            params)

#endif // #include guard

