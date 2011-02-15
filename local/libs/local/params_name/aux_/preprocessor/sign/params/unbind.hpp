
#ifndef BOOST_LOCAL_AUX_PP_SIGN_PARAMS_UNBIND_HPP_
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_UNBIND_HPP_

#include "../parsed_/params/index.hpp"
#include <boost/preprocessor/tuple/elem.hpp>

// Private API.

// Param 2-tuple `([auto | register] type name, default_value)`.
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_UNBIND_PARAM_INDEX_DECL_     0
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_UNBIND_PARAM_INDEX_DEFAULT_  1
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_UNBIND_PARAM_INDEX_MAX_      2

// Public API.

// Expand to param declaration: [auto | register] type name.
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_UNBIND_PARAM_DECL(param) \
    BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_SIGN_PARAMS_UNBIND_PARAM_INDEX_MAX_, \
            BOOST_LOCAL_AUX_SIGN_PARAMS_UNBIND_PARAM_INDEX_DECL_, param)

// Expand to param default value (empty if none).
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_UNBIND_PARAM_DEFAULT(param) \
    BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_SIGN_PARAMS_UNBIND_PARAM_INDEX_MAX_, \
            BOOST_LOCAL_AUX_SIGN_PARAMS_UNBIND_PARAM_INDEX_DEFAULT_, param)

// Expand to 1 if param has default value, 0 otherwise.
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_UNBIND_PARAM_HAS_DEFAULT(param) \
    BOOST_PP_IF_EMPTY(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_UNBIND_PARAM_DEFAULT( \
            param) (/* expand empty */) )

// Expand to nil-seq of ALL unbind params.
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_UNBIND(params) \
    BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
            BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_UNBIND_, params)

#endif // #include guard

