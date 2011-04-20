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
# ifndef BOOST_PREPROCESSOR_TUPLE_EAT_HPP
# define BOOST_PREPROCESSOR_TUPLE_EAT_HPP
#
# include <boost/preprocessor/config/config.hpp>
# include <boost/preprocessor/config/variadics.hpp>
#
# if BOOST_PP_VARIADICS
#
# /* BOOST_PP_TUPLE_EAT */
#
#define BOOST_PP_TUPLE_EAT(size) BOOST_PP_TUPLE_EAT_DETAIL
#define BOOST_PP_TUPLE_EAT_DETAIL(...)
#
# else
#
# /* BOOST_PP_TUPLE_EAT */
#
#    define BOOST_PP_TUPLE_EAT(size) BOOST_PP_TUPLE_EAT_COMMON(size)
#
# endif /* BOOST_PP_VARIADICS */
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MWCC()
#    define BOOST_PP_TUPLE_EAT_COMMON(size) BOOST_PP_TUPLE_EAT_I(size)
# else
#    define BOOST_PP_TUPLE_EAT_COMMON(size) BOOST_PP_TUPLE_EAT_OO((size))
#    define BOOST_PP_TUPLE_EAT_OO(par) BOOST_PP_TUPLE_EAT_I ## par
# endif
#
# define BOOST_PP_TUPLE_EAT_I(size) BOOST_PP_TUPLE_EAT_ ## size
#
# define BOOST_PP_TUPLE_EAT_0()
# define BOOST_PP_TUPLE_EAT_1(a)
# define BOOST_PP_TUPLE_EAT_2(a, b)
# define BOOST_PP_TUPLE_EAT_3(a, b, c)
# define BOOST_PP_TUPLE_EAT_4(a, b, c, d)
# define BOOST_PP_TUPLE_EAT_5(a, b, c, d, e)
# define BOOST_PP_TUPLE_EAT_6(a, b, c, d, e, f)
# define BOOST_PP_TUPLE_EAT_7(a, b, c, d, e, f, g)
# define BOOST_PP_TUPLE_EAT_8(a, b, c, d, e, f, g, h)
# define BOOST_PP_TUPLE_EAT_9(a, b, c, d, e, f, g, h, i)
# define BOOST_PP_TUPLE_EAT_10(a, b, c, d, e, f, g, h, i, j)
# define BOOST_PP_TUPLE_EAT_11(a, b, c, d, e, f, g, h, i, j, k)
# define BOOST_PP_TUPLE_EAT_12(a, b, c, d, e, f, g, h, i, j, k, l)
# define BOOST_PP_TUPLE_EAT_13(a, b, c, d, e, f, g, h, i, j, k, l, m)
# define BOOST_PP_TUPLE_EAT_14(a, b, c, d, e, f, g, h, i, j, k, l, m, n)
# define BOOST_PP_TUPLE_EAT_15(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o)
# define BOOST_PP_TUPLE_EAT_16(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p)
# define BOOST_PP_TUPLE_EAT_17(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q)
# define BOOST_PP_TUPLE_EAT_18(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r)
# define BOOST_PP_TUPLE_EAT_19(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s)
# define BOOST_PP_TUPLE_EAT_20(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t)
# define BOOST_PP_TUPLE_EAT_21(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u)
# define BOOST_PP_TUPLE_EAT_22(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v)
# define BOOST_PP_TUPLE_EAT_23(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w)
# define BOOST_PP_TUPLE_EAT_24(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x)
# define BOOST_PP_TUPLE_EAT_25(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y)
# define BOOST_PP_TUPLE_EAT_26(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z)
# define BOOST_PP_TUPLE_EAT_27(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa)
# define BOOST_PP_TUPLE_EAT_28(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb)
# define BOOST_PP_TUPLE_EAT_29(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc)
# define BOOST_PP_TUPLE_EAT_30(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd)
# define BOOST_PP_TUPLE_EAT_31(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee)
# define BOOST_PP_TUPLE_EAT_32(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff)
# define BOOST_PP_TUPLE_EAT_33(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg)
# define BOOST_PP_TUPLE_EAT_34(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh)
# define BOOST_PP_TUPLE_EAT_35(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii)
# define BOOST_PP_TUPLE_EAT_36(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj)
# define BOOST_PP_TUPLE_EAT_37(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk)
# define BOOST_PP_TUPLE_EAT_38(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll)
# define BOOST_PP_TUPLE_EAT_39(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm)
# define BOOST_PP_TUPLE_EAT_40(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn)
# define BOOST_PP_TUPLE_EAT_41(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo)
# define BOOST_PP_TUPLE_EAT_42(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp)
# define BOOST_PP_TUPLE_EAT_43(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq)
# define BOOST_PP_TUPLE_EAT_44(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr)
# define BOOST_PP_TUPLE_EAT_45(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss)
# define BOOST_PP_TUPLE_EAT_46(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt)
# define BOOST_PP_TUPLE_EAT_47(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu)
# define BOOST_PP_TUPLE_EAT_48(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv)
# define BOOST_PP_TUPLE_EAT_49(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww)
# define BOOST_PP_TUPLE_EAT_50(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx)
# define BOOST_PP_TUPLE_EAT_51(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy)
# define BOOST_PP_TUPLE_EAT_52(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz)
# define BOOST_PP_TUPLE_EAT_53(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab)
# define BOOST_PP_TUPLE_EAT_54(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd)
# define BOOST_PP_TUPLE_EAT_55(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef)
# define BOOST_PP_TUPLE_EAT_56(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef, gh)
# define BOOST_PP_TUPLE_EAT_57(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef, gh, ij)
# define BOOST_PP_TUPLE_EAT_58(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef, gh, ij, kl)
# define BOOST_PP_TUPLE_EAT_59(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef, gh, ij, kl, mn)
# define BOOST_PP_TUPLE_EAT_60(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef, gh, ij, kl, mn, op)
# define BOOST_PP_TUPLE_EAT_61(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef, gh, ij, kl, mn, op, qr)
# define BOOST_PP_TUPLE_EAT_62(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef, gh, ij, kl, mn, op, qr, st)
# define BOOST_PP_TUPLE_EAT_63(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef, gh, ij, kl, mn, op, qr, st, uv)
# define BOOST_PP_TUPLE_EAT_64(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, jj, kk, ll, mm, nn, oo, pp, qq, rr, ss, tt, uu, vv, ww, xx, yy, zz, ab, cd, ef, gh, ij, kl, mn, op, qr, st, uv, wx)
#
# endif
