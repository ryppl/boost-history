
#include <boost/detail/preprocessor/keyword/is_auto.hpp>
#include <boost/detail/preprocessor/keyword/is_register.hpp>
#include <boost/detail/preprocessor/keyword/is_default.hpp>
#include <boost/detail/preprocessor/keyword/is_const.hpp>
#include <boost/local/aux_/preprocessor/keyword/is_bind.hpp>
#include <boost/preprocessor.hpp>

// GRAMMAR
//
// param_list:
//      [required_param] [optional_param] [bind_param]
//
// required_param:
//      ( [auto | register] param_type param_name )
//
// optional_param:
//      required_param param_default
//
// param_default:
//      ( default param_default_value )
//
// bind_param:
//      ( [const] bind [&] var_name )
//

void BOOST_LOCAL_FUNCTION_PARAMS(
        (double& x) (auto int y)(default -1) (register int z)(default -2)
        (bind a) (bind& b) (const bind c) (const bind& d) ) {
    y = x;
} BOOST_LOCAL_FUNCTION_NAME(f)

#define params \
        (double& x) (auto int y)(default -1) (register int z)(default -2) \
        (bind a) (bind& b) (const bind c) (const bind& d)

// STRIP //

#define BOOST_DETAIL_PP_KEYWORD_STRIP_(tokens, stripping_prefix) \
    BOOST_PP_CAT(stripping_prefix, tokens)

#define BOOST_DETAIL_PP_KEYWORD_STRIP(tokens, check_macro, stripping_prefix) \
    BOOST_PP_IIF(check_macro(tokens), \
        BOOST_DETAIL_PP_KEYWORD_STRIP_ \
    , \
        tokens BOOST_PP_TUPLE_EAT(2) \
    )(tokens, stripping_prefix)

// STRIP_AUTO //

#define BOOST_DETAIL_PP_KEYWORD_STRIP_AUTO_auto /* nothing */

#define BOOST_DETAIL_PP_KEYWORD_STRIP_AUTO(tokens) \
    BOOST_DETAIL_PP_KEYWORD_STRIP(tokens, BOOST_DETAIL_PP_KEYWORD_IS_AUTO, \
            BOOST_DETAIL_PP_KEYWORD_STRIP_AUTO_)

// STRIP_REGISTER //

#define BOOST_DETAIL_PP_KEYWORD_STRIP_REGISTER_register /* nothing */

#define BOOST_DETAIL_PP_KEYWORD_STRIP_REGISTER(tokens) \
    BOOST_DETAIL_PP_KEYWORD_STRIP(tokens, BOOST_DETAIL_PP_KEYWORD_IS_REGISTER, \
            BOOST_DETAIL_PP_KEYWORD_STRIP_REGISTER_)

// STRIP_DEFAULT //

#define BOOST_DETAIL_PP_KEYWORD_STRIP_DEFAULT_default /* nothing */

#define BOOST_DETAIL_PP_KEYWORD_STRIP_DEFAULT(tokens) \
    BOOST_DETAIL_PP_KEYWORD_STRIP(tokens, BOOST_DETAIL_PP_KEYWORD_IS_DEFAULT, \
            BOOST_DETAIL_PP_KEYWORD_STRIP_DEFAULT_)

// STRIP_CONST //

#define BOOST_DETAIL_PP_KEYWORD_STRIP_CONST_const /* nothing */

#define BOOST_DETAIL_PP_KEYWORD_STRIP_CONST(tokens) \
    BOOST_DETAIL_PP_KEYWORD_STRIP(tokens, BOOST_DETAIL_PP_KEYWORD_IS_CONST, \
            BOOST_DETAIL_PP_KEYWORD_STRIP_CONST_)

// PARAM_TYPE_NAME //

// param_decl is `[auto | register] param_type param_name`
#define BOOST_DETAIL_PP_SIGN_PARAM_TYPE_NAME(param_decl) \
    BOOST_DETAIL_PP_KEYWORD_STRIP_AUTO( \
            BOOST_DETAIL_PP_KEYWORD_STRIP_REGISTER(param_decl))

// PARAM_IS_BOUND //

// param_decl is `[const] bind [&] var_name` for bound params.
#define BOOST_DETAIL_PP_SIGN_PARAM_IS_BIND(param_decl) \
    BOOST_LOCAL_AUX_PP_KEYWORD_IS_BIND(param_decl)

#define BOOST_DETAIL_PP_SIGN_PARAM_IS_CONST_BIND(param_decl) \
    BOOST_PP_BITAND( \
          BOOST_DETAIL_PP_KEYWORD_IS_CONST(param_decl) \
        , BOOST_LOCAL_AUX_PP_KEYWORD_IS_BIND( \
                    BOOST_DETAIL_PP_KEYWORD_STRIP_CONST(param_decl)) \
    )

// TEST //

BOOST_DETAIL_PP_SIGN_PARAM_TYPE_NAME(auto int x)
BOOST_DETAIL_PP_SIGN_PARAM_TYPE_NAME(double& y)

BOOST_DETAIL_PP_KEYWORD_STRIP_DEFAULT(default -1)

BOOST_DETAIL_PP_SIGN_PARAM_IS_BIND(bind& a)
BOOST_DETAIL_PP_SIGN_PARAM_IS_CONST_BIND(const bind& a)

#define PRED(s, check_macro, param_decl) check_macro(param_decl)

BOOST_PP_SEQ_FILTER(PRED, BOOST_DETAIL_PP_SIGN_PARAM_IS_BIND, params)
BOOST_PP_SEQ_FILTER(PRED, BOOST_DETAIL_PP_SIGN_PARAM_IS_CONST_BIND, params)

