# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Permission to copy, use, modify, sell and distribute this software is
#  * granted provided this copyright notice appears in all copies. This
#  * software is provided "as is" without express or implied warranty, and
#  * with no claim as to its suitability for any purpose.
#  *
#  * See http://www.boost.org for most recent version.
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# ifndef BOOST_PREPROCESSOR_REPETITION_ENUM_HPP
# define BOOST_PREPROCESSOR_REPETITION_ENUM_HPP
#
# include <boost/preprocessor/cat.hpp>
# include <boost/preprocessor/config/config.hpp>
# include <boost/preprocessor/detail/auto_rec.hpp>
# include <boost/preprocessor/punctuation/comma_if.hpp>
# include <boost/preprocessor/repetition/repeat.hpp>
# include <boost/preprocessor/tuple/elem.hpp>
# include <boost/preprocessor/tuple/rem.hpp>
#
# /* BOOST_PP_ENUM */
#
# if 0
#    define BOOST_PP_ENUM(count, macro, data)
# endif
#
# define BOOST_PP_ENUM BOOST_PP_CAT(BOOST_PP_ENUM_, BOOST_PP_AUTO_REC(BOOST_PP_REPEAT_P, 4))
#
# if ~BOOST_PP_CONFIG_FLAGS & BOOST_PP_CONFIG_EDG
#    define BOOST_PP_ENUM_1(c, m, d) BOOST_PP_REPEAT_1(c, BOOST_PP_ENUM_M_1, (m, d))
#    define BOOST_PP_ENUM_2(c, m, d) BOOST_PP_REPEAT_2(c, BOOST_PP_ENUM_M_2, (m, d))
#    define BOOST_PP_ENUM_3(c, m, d) BOOST_PP_REPEAT_3(c, BOOST_PP_ENUM_M_3, (m, d))
# else
#    define BOOST_PP_ENUM_1(c, m, d) BOOST_PP_ENUM_1_D(c, m, d)
#    define BOOST_PP_ENUM_2(c, m, d) BOOST_PP_ENUM_2_D(c, m, d)
#    define BOOST_PP_ENUM_3(c, m, d) BOOST_PP_ENUM_3_D(c, m, d)
#    define BOOST_PP_ENUM_1_D(c, m, d) BOOST_PP_REPEAT_1(c, BOOST_PP_ENUM_M_1, (m, d))
#    define BOOST_PP_ENUM_2_D(c, m, d) BOOST_PP_REPEAT_2(c, BOOST_PP_ENUM_M_2, (m, d))
#    define BOOST_PP_ENUM_3_D(c, m, d) BOOST_PP_REPEAT_3(c, BOOST_PP_ENUM_M_3, (m, d))
# endif
#
# define BOOST_PP_ENUM_4(c, m, d) BOOST_PP_ERROR(0x0003)
#
# if ~BOOST_PP_CONFIG_FLAGS & BOOST_PP_CONFIG_MWCW
#    if BOOST_PP_CONFIG_FLAGS & BOOST_PP_CONFIG_STRICT
#        define BOOST_PP_ENUM_M_1(z, n, md) BOOST_PP_ENUM_M_1_A(z, n, BOOST_PP_TUPLE_REM_2 md)
#        define BOOST_PP_ENUM_M_1_A(z, m, im) BOOST_PP_ENUM_M_1_B(z, m, im)
#        define BOOST_PP_ENUM_M_2(z, n, md) BOOST_PP_ENUM_M_2_A(z, n, BOOST_PP_TUPLE_REM_2 md)
#        define BOOST_PP_ENUM_M_2_A(z, m, im) BOOST_PP_ENUM_M_2_B(z, m, im)
#        define BOOST_PP_ENUM_M_3(z, n, md) BOOST_PP_ENUM_M_3_A(z, n, BOOST_PP_TUPLE_REM_2 md)
#        define BOOST_PP_ENUM_M_3_A(z, m, im) BOOST_PP_ENUM_M_3_B(z, m, im)
#    else
#        define BOOST_PP_ENUM_M_1(z, n, md) BOOST_PP_COMMA_IF(n) BOOST_PP_TUPLE_ELEM(2, 0, md)(z, n, BOOST_PP_TUPLE_ELEM(2, 1, md))
#        define BOOST_PP_ENUM_M_2(z, n, md) BOOST_PP_COMMA_IF(n) BOOST_PP_TUPLE_ELEM(2, 0, md)(z, n, BOOST_PP_TUPLE_ELEM(2, 1, md))
#        define BOOST_PP_ENUM_M_3(z, n, md) BOOST_PP_COMMA_IF(n) BOOST_PP_TUPLE_ELEM(2, 0, md)(z, n, BOOST_PP_TUPLE_ELEM(2, 1, md))
#    endif
# else
#    define BOOST_PP_ENUM_M_1(z, n, md) BOOST_PP_ENUM_M_1_A(z, n, BOOST_PP_TUPLE_ELEM(2, 0, md), BOOST_PP_TUPLE_ELEM(2, 1, md))
#    define BOOST_PP_ENUM_M_1_A(z, n, m, d) BOOST_PP_ENUM_M_1_B(z, n, m, d)
#    define BOOST_PP_ENUM_M_2(z, n, md) BOOST_PP_ENUM_M_2_A(z, n, BOOST_PP_TUPLE_ELEM(2, 0, md), BOOST_PP_TUPLE_ELEM(2, 1, md))
#    define BOOST_PP_ENUM_M_2_A(z, n, m, d) BOOST_PP_ENUM_M_2_B(z, n, m, d)
#    define BOOST_PP_ENUM_M_3(z, n, md) BOOST_PP_ENUM_M_3_A(z, n, BOOST_PP_TUPLE_ELEM(2, 0, md), BOOST_PP_TUPLE_ELEM(2, 1, md))
#    define BOOST_PP_ENUM_M_3_A(z, n, m, d) BOOST_PP_ENUM_M_3_B(z, n, m, d)
# endif
#
# define BOOST_PP_ENUM_M_1_B(z, n, m, d) BOOST_PP_COMMA_IF(n) m(z, n, d)
# define BOOST_PP_ENUM_M_2_B(z, n, m, d) BOOST_PP_COMMA_IF(n) m(z, n, d)
# define BOOST_PP_ENUM_M_3_B(z, n, m, d) BOOST_PP_COMMA_IF(n) m(z, n, d)
#
# endif
