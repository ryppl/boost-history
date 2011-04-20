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
# ifndef BOOST_PREPROCESSOR_TUPLE_TO_SEQ_HPP
# define BOOST_PREPROCESSOR_TUPLE_TO_SEQ_HPP
#
# include <boost/preprocessor/config/config.hpp>
# include <boost/preprocessor/config/variadics.hpp>
#
# if BOOST_PP_VARIADICS
#
# include <boost/preprocessor/tuple/size.hpp>
# include <boost/preprocessor/tuple/detail/use_overload.hpp>
#
# /* BOOST_PP_TUPLE_TO_SEQ */
#
# define BOOST_PP_TUPLE_TO_SEQ(...) \
  BOOST_PP_TUPLE_DETAIL_TO_SEQ_USE_OVERLOAD(BOOST_PP_TUPLE_DETAIL_TO_SEQ_, __VA_ARGS__) \
  /**/
# define BOOST_PP_TUPLE_DETAIL_TO_SEQ_1(tuple) \
  BOOST_PP_TUPLE_DETAIL_TO_SEQ_2(BOOST_PP_TUPLE_SIZE(tuple),tuple) \
  /**/
# define BOOST_PP_TUPLE_DETAIL_TO_SEQ_2(size, tuple) BOOST_PP_TUPLE_TO_SEQ_COMMON(size, tuple)
#
# else
#
# /* BOOST_PP_TUPLE_TO_SEQ */
#
#    define BOOST_PP_TUPLE_TO_SEQ(size, tuple) BOOST_PP_TUPLE_TO_SEQ_COMMON(size, tuple)
#
# endif /* BOOST_PP_VARIADICS */
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MWCC()
#    define BOOST_PP_TUPLE_TO_SEQ_COMMON(size, tuple) BOOST_PP_TUPLE_TO_SEQ_I(size, tuple)
#    if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC()
#        define BOOST_PP_TUPLE_TO_SEQ_I(s, t) BOOST_PP_TUPLE_TO_SEQ_ ## s t
#    else
#        define BOOST_PP_TUPLE_TO_SEQ_I(s, t) BOOST_PP_TUPLE_TO_SEQ_II(BOOST_PP_TUPLE_TO_SEQ_ ## s t)
#        define BOOST_PP_TUPLE_TO_SEQ_II(res) res
#    endif
# else
#    define BOOST_PP_TUPLE_TO_SEQ_COMMON(size, tuple) BOOST_PP_TUPLE_TO_SEQ_OO((size, tuple))
#    define BOOST_PP_TUPLE_TO_SEQ_OO(par) BOOST_PP_TUPLE_TO_SEQ_I ## par
#    define BOOST_PP_TUPLE_TO_SEQ_I(s, t) BOOST_PP_TUPLE_TO_SEQ_ ## s ## t
# endif
#
# define BOOST_PP_TUPLE_TO_SEQ_0()
# define BOOST_PP_TUPLE_TO_SEQ_1(a) (a)
# define BOOST_PP_TUPLE_TO_SEQ_2(a, b) (a)(b)
# define BOOST_PP_TUPLE_TO_SEQ_3(a, b, c) (a)(b)(c)
# define BOOST_PP_TUPLE_TO_SEQ_4(a, b, c, d) (a)(b)(c)(d)
# define BOOST_PP_TUPLE_TO_SEQ_5(a, b, c, d, e) (a)(b)(c)(d)(e)
# define BOOST_PP_TUPLE_TO_SEQ_6(a, b, c, d, e, f) (a)(b)(c)(d)(e)(f)
# define BOOST_PP_TUPLE_TO_SEQ_7(a, b, c, d, e, f, g) (a)(b)(c)(d)(e)(f)(g)
# define BOOST_PP_TUPLE_TO_SEQ_8(a, b, c, d, e, f, g, h) (a)(b)(c)(d)(e)(f)(g)(h)
# define BOOST_PP_TUPLE_TO_SEQ_9(a, b, c, d, e, f, g, h, i) (a)(b)(c)(d)(e)(f)(g)(h)(i)
# define BOOST_PP_TUPLE_TO_SEQ_10(a, b, c, d, e, f, g, h, i, j) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)
# define BOOST_PP_TUPLE_TO_SEQ_11(a, b, c, d, e, f, g, h, i, j, k) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)
# define BOOST_PP_TUPLE_TO_SEQ_12(a, b, c, d, e, f, g, h, i, j, k, l) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)
# define BOOST_PP_TUPLE_TO_SEQ_13(a, b, c, d, e, f, g, h, i, j, k, l, m) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)
# define BOOST_PP_TUPLE_TO_SEQ_14(a, b, c, d, e, f, g, h, i, j, k, l, m, n) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)
# define BOOST_PP_TUPLE_TO_SEQ_15(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)
# define BOOST_PP_TUPLE_TO_SEQ_16(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)
# define BOOST_PP_TUPLE_TO_SEQ_17(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)
# define BOOST_PP_TUPLE_TO_SEQ_18(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)
# define BOOST_PP_TUPLE_TO_SEQ_19(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)
# define BOOST_PP_TUPLE_TO_SEQ_20(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)
# define BOOST_PP_TUPLE_TO_SEQ_21(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)
# define BOOST_PP_TUPLE_TO_SEQ_22(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)
# define BOOST_PP_TUPLE_TO_SEQ_23(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)
# define BOOST_PP_TUPLE_TO_SEQ_24(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)
# define BOOST_PP_TUPLE_TO_SEQ_25(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)
# define BOOST_PP_TUPLE_TO_SEQ_26(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)
# define BOOST_PP_TUPLE_TO_SEQ_27(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)
# define BOOST_PP_TUPLE_TO_SEQ_28(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)
# define BOOST_PP_TUPLE_TO_SEQ_29(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)
# define BOOST_PP_TUPLE_TO_SEQ_30(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)
# define BOOST_PP_TUPLE_TO_SEQ_31(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)
# define BOOST_PP_TUPLE_TO_SEQ_32(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)
# define BOOST_PP_TUPLE_TO_SEQ_33(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)
# define BOOST_PP_TUPLE_TO_SEQ_34(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)
# define BOOST_PP_TUPLE_TO_SEQ_35(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)
# define BOOST_PP_TUPLE_TO_SEQ_36(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)
# define BOOST_PP_TUPLE_TO_SEQ_37(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)(kk)
# define BOOST_PP_TUPLE_TO_SEQ_38(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)(kk)(ll)
# define BOOST_PP_TUPLE_TO_SEQ_39(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)(kk)(ll)(mm)
# define BOOST_PP_TUPLE_TO_SEQ_40(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)(kk)(ll)(mm)(nn)
# define BOOST_PP_TUPLE_TO_SEQ_41(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)(kk)(ll)(mm)(nn)(oo)
# define BOOST_PP_TUPLE_TO_SEQ_42(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)(kk)(ll)(mm)(nn)(oo)(pp)
# define BOOST_PP_TUPLE_TO_SEQ_43(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)(kk)(ll)(mm)(nn)(oo)(pp)(qq)
# define BOOST_PP_TUPLE_TO_SEQ_44(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)(kk)(ll)(mm)(nn)(oo)(pp)(qq)(rr)
# define BOOST_PP_TUPLE_TO_SEQ_45(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)(kk)(ll)(mm)(nn)(oo)(pp)(qq)(rr)(ss)
# define BOOST_PP_TUPLE_TO_SEQ_46(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)(kk)(ll)(mm)(nn)(oo)(pp)(qq)(rr)(ss)(tt)
# define BOOST_PP_TUPLE_TO_SEQ_47(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)(kk)(ll)(mm)(nn)(oo)(pp)(qq)(rr)(ss)(tt)(uu)
# define BOOST_PP_TUPLE_TO_SEQ_48(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)(kk)(ll)(mm)(nn)(oo)(pp)(qq)(rr)(ss)(tt)(uu)(vv)
# define BOOST_PP_TUPLE_TO_SEQ_49(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)(kk)(ll)(mm)(nn)(oo)(pp)(qq)(rr)(ss)(tt)(uu)(vv)(ww)
# define BOOST_PP_TUPLE_TO_SEQ_50(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)(kk)(ll)(mm)(nn)(oo)(pp)(qq)(rr)(ss)(tt)(uu)(vv)(ww)(xx)
# define BOOST_PP_TUPLE_TO_SEQ_51(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)(kk)(ll)(mm)(nn)(oo)(pp)(qq)(rr)(ss)(tt)(uu)(vv)(ww)(xx)(yy)
# define BOOST_PP_TUPLE_TO_SEQ_52(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)(kk)(ll)(mm)(nn)(oo)(pp)(qq)(rr)(ss)(tt)(uu)(vv)(ww)(xx)(yy)(zz)
# define BOOST_PP_TUPLE_TO_SEQ_53(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)(kk)(ll)(mm)(nn)(oo)(pp)(qq)(rr)(ss)(tt)(uu)(vv)(ww)(xx)(yy)(zz)(ab)
# define BOOST_PP_TUPLE_TO_SEQ_54(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)(kk)(ll)(mm)(nn)(oo)(pp)(qq)(rr)(ss)(tt)(uu)(vv)(ww)(xx)(yy)(zz)(ab)(cd)
# define BOOST_PP_TUPLE_TO_SEQ_55(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)(kk)(ll)(mm)(nn)(oo)(pp)(qq)(rr)(ss)(tt)(uu)(vv)(ww)(xx)(yy)(zz)(ab)(cd)(ef)
# define BOOST_PP_TUPLE_TO_SEQ_56(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef, gh) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)(kk)(ll)(mm)(nn)(oo)(pp)(qq)(rr)(ss)(tt)(uu)(vv)(ww)(xx)(yy)(zz)(ab)(cd)(ef)(gh)
# define BOOST_PP_TUPLE_TO_SEQ_57(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef, gh, ij) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)(kk)(ll)(mm)(nn)(oo)(pp)(qq)(rr)(ss)(tt)(uu)(vv)(ww)(xx)(yy)(zz)(ab)(cd)(ef)(gh)(ij)
# define BOOST_PP_TUPLE_TO_SEQ_58(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef, gh, ij, kl) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)(kk)(ll)(mm)(nn)(oo)(pp)(qq)(rr)(ss)(tt)(uu)(vv)(ww)(xx)(yy)(zz)(ab)(cd)(ef)(gh)(ij)(kl)
# define BOOST_PP_TUPLE_TO_SEQ_59(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef, gh, ij, kl, mn) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)(kk)(ll)(mm)(nn)(oo)(pp)(qq)(rr)(ss)(tt)(uu)(vv)(ww)(xx)(yy)(zz)(ab)(cd)(ef)(gh)(ij)(kl)(mn)
# define BOOST_PP_TUPLE_TO_SEQ_60(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef, gh, ij, kl, mn, op) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)(kk)(ll)(mm)(nn)(oo)(pp)(qq)(rr)(ss)(tt)(uu)(vv)(ww)(xx)(yy)(zz)(ab)(cd)(ef)(gh)(ij)(kl)(mn)(op)
# define BOOST_PP_TUPLE_TO_SEQ_61(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef, gh, ij, kl, mn, op, qr) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)(kk)(ll)(mm)(nn)(oo)(pp)(qq)(rr)(ss)(tt)(uu)(vv)(ww)(xx)(yy)(zz)(ab)(cd)(ef)(gh)(ij)(kl)(mn)(op)(qr)
# define BOOST_PP_TUPLE_TO_SEQ_62(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef, gh, ij, kl, mn, op, qr, st) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)(kk)(ll)(mm)(nn)(oo)(pp)(qq)(rr)(ss)(tt)(uu)(vv)(ww)(xx)(yy)(zz)(ab)(cd)(ef)(gh)(ij)(kl)(mn)(op)(qr)(st)
# define BOOST_PP_TUPLE_TO_SEQ_63(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef, gh, ij, kl, mn, op, qr, st, uv) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)(kk)(ll)(mm)(nn)(oo)(pp)(qq)(rr)(ss)(tt)(uu)(vv)(ww)(xx)(yy)(zz)(ab)(cd)(ef)(gh)(ij)(kl)(mn)(op)(qr)(st)(uv)
# define BOOST_PP_TUPLE_TO_SEQ_64(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef, gh, ij, kl, mn, op, qr, st, uv, wx) (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)(aa)(bb)(cc)(dd)(ee)(ff)(gg)(hh)(ii)(jj)(kk)(ll)(mm)(nn)(oo)(pp)(qq)(rr)(ss)(tt)(uu)(vv)(ww)(xx)(yy)(zz)(ab)(cd)(ef)(gh)(ij)(kl)(mn)(op)(qr)(st)(uv)(wx)
#
# endif
