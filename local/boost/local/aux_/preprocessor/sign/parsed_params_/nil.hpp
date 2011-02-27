
#ifndef BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_NIL_HPP_
#define BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_NIL_HPP_

#include <boost/preprocessor/facilities/empty.hpp>

// See "index_.hpp".
#define BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_NIL \
    ( \
        /* unbound params: list of 2-tuples (classified_name, default) */ \
        BOOST_PP_NIL /* nil list */ \
    , \
        /* const-binds: nil-seq of 1-tuple `ref_name` */ \
        BOOST_PP_NIL /* nil list */ \
    , \
        0 /* number of const-bind `this` */ \
    , \
        /* binds: nil-seq of 1-tuple `ref_name` */ \
        BOOST_PP_NIL /* nil list */ \
    , \
        0 /* number of bind `this` */ \
    , \
        BOOST_PP_EMPTY /* no error */ \
    )

#endif // #include guard

