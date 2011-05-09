
#include <boost/local/aux_/preprocessor/keyword/bind.hpp>
#include <boost/preprocessor.hpp>
#include <boost/preprocessor/detail/is_unary.hpp>

#define IS_BIND_TYPED_(tokens) \
    BOOST_PP_IS_UNARY(BOOST_LOCAL_AUX_PP_KEYWORD_BIND_REMOVE_FRONT(tokens))

#define IS_BIND_TYPED(tokens) \
    BOOST_PP_IIF(BOOST_LOCAL_AUX_PP_KEYWORD_IS_BIND_FRONT(tokens), \
        IS_BIND_TYPED_ \
    , \
        0 BOOST_PP_TUPLE_EAT(1) \
    )(tokens)

#define BIND_TYPED_EXPR_bind(type) \
    type

#define BIND_TYPED_EXPR_(tokens) \
    BOOST_PP_CAT(BIND_TYPED_EXPR_, tokens)

#define BIND_TYPED_EXPR(tokens) \
    BOOST_PP_IIF(IS_BIND_TYPED(tokens), \
        BIND_TYPED_EXPR_ \
    , \
        tokens BOOST_PP_TUPLE_EAT(1) \
    )(tokens)

#define BIND_TYPED_REMOVE_FRONT_bind(tokens) /* must expand to nothing */

#define BIND_TYPED_REMOVE_FRONT_(tokens) \
    BOOST_PP_CAT(BIND_TYPED_REMOVE_FRONT_, tokens)

#define BIND_TYPED_REMOVE_FRONT(tokens) \
    BOOST_PP_IIF(IS_BIND_TYPED(tokens), \
        BIND_TYPED_REMOVE_FRONT_ \
    , \
        tokens BOOST_PP_TUPLE_EAT(1) \
    )(tokens)

-----
IS_BIND_TYPED( int& x )
IS_BIND_TYPED( bind& x )
IS_BIND_TYPED( bind(double)& x )
-----
BIND_TYPED_EXPR( int& x )
BIND_TYPED_EXPR( bind& x )
BIND_TYPED_EXPR( bind(double)& x )
-----
BIND_TYPED_REMOVE_FRONT( int& x )
BIND_TYPED_REMOVE_FRONT( bind& x )
BIND_TYPED_REMOVE_FRONT( bind(double)& x )
-----

