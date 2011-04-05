# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* Revised by Edward Diener (2011) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef BOOST_PREPROCESSOR_TUPLE_REM_HPP
# define BOOST_PREPROCESSOR_TUPLE_REM_HPP
#
# include <boost/preprocessor/config/config.hpp>
# include <boost/preprocessor/config/variadics.hpp>
#
# if BOOST_PP_VARIADICS
#
# include <boost/preprocessor/tuple/detail/use_overload.hpp>
#
# /* BOOST_PP_TUPLE_REM */
#
#define BOOST_PP_TUPLE_REM(size) BOOST_PP_DETAIL_TUPLE_REM
#define BOOST_PP_DETAIL_TUPLE_REM(...) __VA_ARGS__
#
# /* BOOST_PP_TUPLE_REM_CTOR */
#
#
#    define BOOST_PP_TUPLE_REM_CTOR(...) \
         BOOST_PP_TUPLE_DETAIL_REM_USE_OVERLOAD(BOOST_PP_TUPLE_DETAIL_REM_CTOR_, __VA_ARGS__) \
         /**/
#    define BOOST_PP_TUPLE_DETAIL_REM_CTOR_1(tuple) BOOST_PP_DETAIL_TUPLE_REM tuple
#    define BOOST_PP_TUPLE_DETAIL_REM_CTOR_2(size, tuple) BOOST_PP_TUPLE_DETAIL_REM_CTOR_1(tuple)
#
# else
#
# /* BOOST_PP_TUPLE_REM */
#
#    define BOOST_PP_TUPLE_REM(size) BOOST_PP_TUPLE_REM_COMMON(size)
#
# /* BOOST_PP_TUPLE_REM_CTOR */
#
#    define BOOST_PP_TUPLE_REM_CTOR(size, tuple) BOOST_PP_TUPLE_REM_CTOR_COMMON(size, tuple)
#
# endif /* BOOST_PP_VARIADICS */
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MWCC()
#    define BOOST_PP_TUPLE_REM_COMMON(size) BOOST_PP_TUPLE_REM_I(size)
# else
#    define BOOST_PP_TUPLE_REM_COMMON(size) BOOST_PP_TUPLE_REM_OO((size))
#    define BOOST_PP_TUPLE_REM_OO(par) BOOST_PP_TUPLE_REM_I ## par
# endif
#
# define BOOST_PP_TUPLE_REM_I(size) BOOST_PP_TUPLE_REM_ ## size
#
# define BOOST_PP_TUPLE_REM_0()
# define BOOST_PP_TUPLE_REM_1(a) a
# define BOOST_PP_TUPLE_REM_2(a, b) a, b
# define BOOST_PP_TUPLE_REM_3(a, b, c) a, b, c
# define BOOST_PP_TUPLE_REM_4(a, b, c, d) a, b, c, d
# define BOOST_PP_TUPLE_REM_5(a, b, c, d, e) a, b, c, d, e
# define BOOST_PP_TUPLE_REM_6(a, b, c, d, e, f) a, b, c, d, e, f
# define BOOST_PP_TUPLE_REM_7(a, b, c, d, e, f, g) a, b, c, d, e, f, g
# define BOOST_PP_TUPLE_REM_8(a, b, c, d, e, f, g, h) a, b, c, d, e, f, g, h
# define BOOST_PP_TUPLE_REM_9(a, b, c, d, e, f, g, h, i) a, b, c, d, e, f, g, h, i
# define BOOST_PP_TUPLE_REM_10(a, b, c, d, e, f, g, h, i, j) a, b, c, d, e, f, g, h, i, j
# define BOOST_PP_TUPLE_REM_11(a, b, c, d, e, f, g, h, i, j, k) a, b, c, d, e, f, g, h, i, j, k
# define BOOST_PP_TUPLE_REM_12(a, b, c, d, e, f, g, h, i, j, k, l) a, b, c, d, e, f, g, h, i, j, k, l
# define BOOST_PP_TUPLE_REM_13(a, b, c, d, e, f, g, h, i, j, k, l, m) a, b, c, d, e, f, g, h, i, j, k, l, m
# define BOOST_PP_TUPLE_REM_14(a, b, c, d, e, f, g, h, i, j, k, l, m, n) a, b, c, d, e, f, g, h, i, j, k, l, m, n
# define BOOST_PP_TUPLE_REM_15(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) a, b, c, d, e, f, g, h, i, j, k, l, m, n, o
# define BOOST_PP_TUPLE_REM_16(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p
# define BOOST_PP_TUPLE_REM_17(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q) a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q
# define BOOST_PP_TUPLE_REM_18(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r) a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r
# define BOOST_PP_TUPLE_REM_19(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s) a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s
# define BOOST_PP_TUPLE_REM_20(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t) a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t
# define BOOST_PP_TUPLE_REM_21(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u
# define BOOST_PP_TUPLE_REM_22(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v
# define BOOST_PP_TUPLE_REM_23(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w
# define BOOST_PP_TUPLE_REM_24(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x
# define BOOST_PP_TUPLE_REM_25(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_EDG()
#    define BOOST_PP_TUPLE_REM_CTOR_COMMON(size, tuple) BOOST_PP_TUPLE_REM_CTOR_I(BOOST_PP_TUPLE_REM(size), tuple)
# else
#    define BOOST_PP_TUPLE_REM_CTOR_COMMON(size, tuple) BOOST_PP_TUPLE_REM_CTOR_D(size, tuple)
#    define BOOST_PP_TUPLE_REM_CTOR_D(size, tuple) BOOST_PP_TUPLE_REM_CTOR_I(BOOST_PP_TUPLE_REM(size), tuple)
# endif
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MWCC()
#    define BOOST_PP_TUPLE_REM_CTOR_I(ext, tuple) ext tuple
# else
#    define BOOST_PP_TUPLE_REM_CTOR_I(ext, tuple) BOOST_PP_TUPLE_REM_CTOR_OO((ext, tuple))
#    define BOOST_PP_TUPLE_REM_CTOR_OO(par) BOOST_PP_TUPLE_REM_CTOR_II ## par
#    define BOOST_PP_TUPLE_REM_CTOR_II(ext, tuple) ext ## tuple
# endif
#
# endif
