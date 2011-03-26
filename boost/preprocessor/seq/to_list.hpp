# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2011.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef BOOST_PREPROCESSOR_SEQ_TO_LIST_HPP
# define BOOST_PREPROCESSOR_SEQ_TO_LIST_HPP
#
# include <boost/preprocessor/cat.hpp>
# include <boost/preprocessor/facilities/empty.hpp>
# include <boost/preprocessor/punctuation/comma.hpp>
# include <boost/preprocessor/punctuation/paren.hpp>
#
# /* BOOST_PP_SEQ_TO_LIST */
#
# define BOOST_PP_SEQ_TO_LIST(seq) \
    BOOST_PP_SEQ_TO_LIST_I( \
        BOOST_PP_CAT(BOOST_PP_SEQ_TO_LIST_A seq, 0) \
        BOOST_PP_CAT(BOOST_PP_SEQ_TO_LIST_C seq, 0) \
    ) \
    /**/
# define BOOST_PP_SEQ_TO_LIST_A(x) BOOST_PP_SEQ_TO_LIST_AB(x, B)
# define BOOST_PP_SEQ_TO_LIST_B(x) BOOST_PP_SEQ_TO_LIST_AB(x, A)
#
# define BOOST_PP_SEQ_TO_LIST_AB(x, suffix) \
    (BOOST_PP_LPAREN)(x BOOST_PP_EMPTY)(BOOST_PP_COMMA) BOOST_PP_SEQ_TO_LIST_ ## suffix \
    /**/
#
# define BOOST_PP_SEQ_TO_LIST_A0 (BOOST_PP_NIL BOOST_PP_EMPTY)
# define BOOST_PP_SEQ_TO_LIST_B0 (BOOST_PP_NIL BOOST_PP_EMPTY)
#
# define BOOST_PP_SEQ_TO_LIST_C(x) (BOOST_PP_RPAREN) BOOST_PP_SEQ_TO_LIST_D
# define BOOST_PP_SEQ_TO_LIST_D(x) (BOOST_PP_RPAREN) BOOST_PP_SEQ_TO_LIST_C
#
# define BOOST_PP_SEQ_TO_LIST_C0
# define BOOST_PP_SEQ_TO_LIST_D0
#
# define BOOST_PP_SEQ_TO_LIST_I(seq) \
    BOOST_PP_CAT(BOOST_PP_SEQ_TO_LIST_X seq, 0) \
    /**/
# define BOOST_PP_SEQ_TO_LIST_X(c) c() BOOST_PP_SEQ_TO_LIST_Y
# define BOOST_PP_SEQ_TO_LIST_Y(c) c() BOOST_PP_SEQ_TO_LIST_X
#
# define BOOST_PP_SEQ_TO_LIST_X0
# define BOOST_PP_SEQ_TO_LIST_Y0
#
# endif // BOOST_PREPROCESSOR_SEQ_TO_LIST_HPP
