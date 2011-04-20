# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002) */
# /* Revised by Edward Diener (2011) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef BOOST_PREPROCESSOR_TUPLE_REVERSE_HPP
# define BOOST_PREPROCESSOR_TUPLE_REVERSE_HPP
#
# include <boost/preprocessor/config/config.hpp>
# include <boost/preprocessor/config/variadics.hpp>
#
# if BOOST_PP_VARIADICS
#
# include <boost/preprocessor/tuple/size.hpp>
# include <boost/preprocessor/tuple/detail/use_overload.hpp>
#
# /* BOOST_PP_TUPLE_REVERSE */
#
#    define BOOST_PP_TUPLE_REVERSE(...) \
         BOOST_PP_TUPLE_DETAIL_REVERSE_USE_OVERLOAD(BOOST_PP_TUPLE_DETAIL_REVERSE_, __VA_ARGS__) \
         /**/
#    define BOOST_PP_TUPLE_DETAIL_REVERSE_1(tuple) \
         BOOST_PP_TUPLE_DETAIL_REVERSE_2(BOOST_PP_TUPLE_SIZE(tuple),tuple) \
         /**/
#    define BOOST_PP_TUPLE_DETAIL_REVERSE_2(size, tuple) BOOST_PP_TUPLE_REVERSE_COMMON(size, tuple)
#
# else
#
# /* BOOST_PP_TUPLE_REVERSE */
#
#    define BOOST_PP_TUPLE_REVERSE(size, tuple) BOOST_PP_TUPLE_REVERSE_COMMON(size, tuple)
#
# endif /* BOOST_PP_VARIADICS */
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MWCC()
#    define BOOST_PP_TUPLE_REVERSE_COMMON(size, tuple) BOOST_PP_TUPLE_REVERSE_I(size, tuple)
#    if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC()
#        define BOOST_PP_TUPLE_REVERSE_I(s, t) BOOST_PP_TUPLE_REVERSE_ ## s t
#    else
#        define BOOST_PP_TUPLE_REVERSE_I(s, t) BOOST_PP_TUPLE_REVERSE_II(BOOST_PP_TUPLE_REVERSE_ ## s t)
#        define BOOST_PP_TUPLE_REVERSE_II(res) res
#    endif
# else
#    define BOOST_PP_TUPLE_REVERSE_COMMON(size, tuple) BOOST_PP_TUPLE_REVERSE_OO((size, tuple))
#    define BOOST_PP_TUPLE_REVERSE_OO(par) BOOST_PP_TUPLE_REVERSE_I ## par
#    define BOOST_PP_TUPLE_REVERSE_I(s, t) BOOST_PP_TUPLE_REVERSE_ ## s ## t
# endif
#
# define BOOST_PP_TUPLE_REVERSE_0() ()
# define BOOST_PP_TUPLE_REVERSE_1(a) (a)
# define BOOST_PP_TUPLE_REVERSE_2(a, b) (b, a)
# define BOOST_PP_TUPLE_REVERSE_3(a, b, c) (c, b, a)
# define BOOST_PP_TUPLE_REVERSE_4(a, b, c, d) (d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_5(a, b, c, d, e) (e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_6(a, b, c, d, e, f) (f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_7(a, b, c, d, e, f, g) (g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_8(a, b, c, d, e, f, g, h) (h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_9(a, b, c, d, e, f, g, h, i) (i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_10(a, b, c, d, e, f, g, h, i, j) (j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_11(a, b, c, d, e, f, g, h, i, j, k) (k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_12(a, b, c, d, e, f, g, h, i, j, k, l) (l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_13(a, b, c, d, e, f, g, h, i, j, k, l, m) (m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_14(a, b, c, d, e, f, g, h, i, j, k, l, m, n) (n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_15(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) (o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_16(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) (p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_17(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q) (q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_18(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r) (r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_19(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s) (s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_20(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t) (t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_21(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) (u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_22(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) (v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_23(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) (w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_24(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) (x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_25(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) (y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_26(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z) (z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_27(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa) (aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_28(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb) (bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_29(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc) (cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_30(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd) (dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_31(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee) (ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_32(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff) (ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_33(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg) (gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_34(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh) (hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_35(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii) (ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_36(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj) (jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_37(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk) (kk, jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_38(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll) (ll, kk, jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_39(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm) (mm, ll, kk, jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_40(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn) (nn, mm, ll, kk, jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_41(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo) (oo, nn, mm, ll, kk, jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_42(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp) (pp, oo, nn, mm, ll, kk, jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_43(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq) (qq, pp, oo, nn, mm, ll, kk, jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_44(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr) (rr, qq, pp, oo, nn, mm, ll, kk, jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_45(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss) (ss, rr, qq, pp, oo, nn, mm, ll, kk, jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_46(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt) (tt, ss, rr, qq, pp, oo, nn, mm, ll, kk, jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_47(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu) (uu, tt, ss, rr, qq, pp, oo, nn, mm, ll, kk, jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_48(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv) (vv, uu, tt, ss, rr, qq, pp, oo, nn, mm, ll, kk, jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_49(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww) (ww, vv, uu, tt, ss, rr, qq, pp, oo, nn, mm, ll, kk, jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_50(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx) (xx, ww, vv, uu, tt, ss, rr, qq, pp, oo, nn, mm, ll, kk, jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_51(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy) (yy, xx, ww, vv, uu, tt, ss, rr, qq, pp, oo, nn, mm, ll, kk, jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_52(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz) (zz, yy, xx, ww, vv, uu, tt, ss, rr, qq, pp, oo, nn, mm, ll, kk, jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_53(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab) (ab, zz, yy, xx, ww, vv, uu, tt, ss, rr, qq, pp, oo, nn, mm, ll, kk, jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_54(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd) (cd, ab, zz, yy, xx, ww, vv, uu, tt, ss, rr, qq, pp, oo, nn, mm, ll, kk, jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_55(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef) (ef, cd, ab, zz, yy, xx, ww, vv, uu, tt, ss, rr, qq, pp, oo, nn, mm, ll, kk, jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_56(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef, gh) (gh, ef, cd, ab, zz, yy, xx, ww, vv, uu, tt, ss, rr, qq, pp, oo, nn, mm, ll, kk, jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_57(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef, gh, ij) (ij, gh, ef, cd, ab, zz, yy, xx, ww, vv, uu, tt, ss, rr, qq, pp, oo, nn, mm, ll, kk, jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_58(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef, gh, ij, kl) (kl, ij, gh, ef, cd, ab, zz, yy, xx, ww, vv, uu, tt, ss, rr, qq, pp, oo, nn, mm, ll, kk, jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_59(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef, gh, ij, kl, mn) (mn, kl, ij, gh, ef, cd, ab, zz, yy, xx, ww, vv, uu, tt, ss, rr, qq, pp, oo, nn, mm, ll, kk, jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_60(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef, gh, ij, kl, mn, op) (op, mn, kl, ij, gh, ef, cd, ab, zz, yy, xx, ww, vv, uu, tt, ss, rr, qq, pp, oo, nn, mm, ll, kk, jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_61(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef, gh, ij, kl, mn, op, qr) (qr, op, mn, kl, ij, gh, ef, cd, ab, zz, yy, xx, ww, vv, uu, tt, ss, rr, qq, pp, oo, nn, mm, ll, kk, jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_62(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef, gh, ij, kl, mn, op, qr, st) (st, qr, op, mn, kl, ij, gh, ef, cd, ab, zz, yy, xx, ww, vv, uu, tt, ss, rr, qq, pp, oo, nn, mm, ll, kk, jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_63(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef, gh, ij, kl, mn, op, qr, st, uv) (uv, st, qr, op, mn, kl, ij, gh, ef, cd, ab, zz, yy, xx, ww, vv, uu, tt, ss, rr, qq, pp, oo, nn, mm, ll, kk, jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
# define BOOST_PP_TUPLE_REVERSE_64(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef, gh, ij, kl, mn, op, qr, st, uv, wx) (wx, uv, st, qr, op, mn, kl, ij, gh, ef, cd, ab, zz, yy, xx, ww, vv, uu, tt, ss, rr, qq, pp, oo, nn, mm, ll, kk, jj, ii, hh, gg, ff, ee, dd, cc, bb, aa, z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
#
# endif
