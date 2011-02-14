
#include <boost/preprocessor.hpp>
#include <boost/preprocessor/detail/is_unary.hpp>

#define IS_AUTO_auto (1) /* must expand to unary */
#define IS_AUTO(tokens) BOOST_PP_IS_UNARY(BOOST_PP_CAT(IS_AUTO_, tokens))

#define REMOVE_AUTO_STRIP_auto /* must expand to nothing */
#define REMOVE_AUTO_(tokens) BOOST_PP_CAT(REMOVE_AUTO_STRIP_, tokens)
#define REMOVE_AUTO(tokens) \
    BOOST_PP_IIF(IS_AUTO(tokens), \
        REMOVE_AUTO_ \
    , \
        tokens BOOST_PP_TUPLE_EAT(1) \
    )(tokens)

#define ADD_AUTO(tokens) \
    BOOST_PP_EXPR_IIF(BOOST_PP_NOT(IS_AUTO(tokens)), auto) tokens


IS_AUTO(auto int x) // 1
IS_AUTO(int x) // 0

REMOVE_AUTO(auto int x) // int x
REMOVE_AUTO(int x) // int x

ADD_AUTO(auto int x) // auto int x
ADD_AUTO(int x) // auto int x


