# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.  Permission to copy, use,        *
#  *     modify, sell, and distribute this software is granted provided       *
#  *     this copyright notice appears in all copies.  This software is       *
#  *     provided "as is" without express or implied warranty, and with       *
#  *     no claim at to its suitability for any purpose.                      *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef BOOST_PREPROCESSOR_TUPLE_TO_SET_HPP
# define BOOST_PREPROCESSOR_TUPLE_TO_SET_HPP
#
# include <boost/preprocessor/config/config.hpp>
#
# /* BOOST_PP_TUPLE_TO_SET */
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MWCC()
#    define BOOST_PP_TUPLE_TO_SET(size, tuple) BOOST_PP_TUPLE_TO_SET_I(size, tuple)
#    if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC()
#        define BOOST_PP_TUPLE_TO_SET_I(s, t) BOOST_PP_TUPLE_TO_SET_ ## s t
#    else
#        define BOOST_PP_TUPLE_TO_SET_I(s, t) BOOST_PP_TUPLE_TO_SET_II(BOOST_PP_TUPLE_TO_SET_ ## s t)
#        define BOOST_PP_TUPLE_TO_SET_II(res) res
#    endif
# else
#    define BOOST_PP_TUPLE_TO_SET(size, tuple) BOOST_PP_TUPLE_TO_SET_OO((size, tuple))
#    define BOOST_PP_TUPLE_TO_SET_OO(par) BOOST_PP_TUPLE_TO_SET_I ## par
#    define BOOST_PP_TUPLE_TO_SET_I(s, t) BOOST_PP_TUPLE_TO_SET_ ## s ## t
# endif
#
# define BOOST_PP_TUPLE_TO_SET_0()
# define BOOST_PP_TUPLE_TO_SET_1(a) (a)
# define BOOST_PP_TUPLE_TO_SET_2(a, b) (a)(b)
# define BOOST_PP_TUPLE_TO_SET_3(a, b, c) (a)(b)(c)
# define BOOST_PP_TUPLE_TO_SET_4(a, b, c, d) (a)(b)(c)(d)
# define BOOST_PP_TUPLE_TO_SET_5(a, b, c, d, e) (a)(b)(c)(d)(e)
# define BOOST_PP_TUPLE_TO_SET_6(a, b, c, d, e, f) (a)(b)(c)(d)(e)(f)
# define BOOST_PP_TUPLE_TO_SET_7(a, b, c, d, e, f, g) (a)(b)(c)(d)(e)(f)(g)
# define BOOST_PP_TUPLE_TO_SET_8(a, b, c, d, e, f, g, h) (a)(b)(c)(d)(e)(f)(g)(h)
# define BOOST_PP_TUPLE_TO_SET_9(a, b, c, d, e, f, g, h, i) (a)(b)(c)(d)(e)(f)(g)(h)(i)
# define BOOST_PP_TUPLE_TO_SET_10(a, b, c, d, e, f, g, h, i, j) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)
# define BOOST_PP_TUPLE_TO_SET_11(a, b, c, d, e, f, g, h, i, j, k) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)
# define BOOST_PP_TUPLE_TO_SET_12(a, b, c, d, e, f, g, h, i, j, k, l) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)
# define BOOST_PP_TUPLE_TO_SET_13(a, b, c, d, e, f, g, h, i, j, k, l, m) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)
# define BOOST_PP_TUPLE_TO_SET_14(a, b, c, d, e, f, g, h, i, j, k, l, m, n) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)
# define BOOST_PP_TUPLE_TO_SET_15(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)
# define BOOST_PP_TUPLE_TO_SET_16(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)
# define BOOST_PP_TUPLE_TO_SET_17(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)
# define BOOST_PP_TUPLE_TO_SET_18(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)
# define BOOST_PP_TUPLE_TO_SET_19(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)
# define BOOST_PP_TUPLE_TO_SET_20(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)
# define BOOST_PP_TUPLE_TO_SET_21(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)
# define BOOST_PP_TUPLE_TO_SET_22(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)
# define BOOST_PP_TUPLE_TO_SET_23(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)
# define BOOST_PP_TUPLE_TO_SET_24(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)
# define BOOST_PP_TUPLE_TO_SET_25(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)
#
# endif
