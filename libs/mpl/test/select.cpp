// (C) Copyright Jonathan Turkanis 2004.
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies. This
// software is provided "as is" without express or implied warranty, and
// with no claim as to its suitability for any purpose.

// Test program for <boost/utility/select_by_size.hpp>

#include <boost/mpl/assert_is_same.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/select.hpp>

struct lazy_char { typedef char type; };
struct lazy_long { typedef long type; };

int main()
{
    using namespace boost;
    using mpl::true_;
    using mpl::false_;
    using mpl::lazy;

    //----------Test select_c with no lazy evaluation-------------------------//

    typedef mpl::select_c<
                true, char, false, long, false, long, false, long
            >::type s1;
    typedef mpl::select_c<
                false, long, true, char, false, long, false, long
            >::type s2;
    typedef mpl::select_c<
                false, long, false, long, true, char, false, long
            >::type s3;
    typedef mpl::select_c<
                false, long, false, long, false, long, true, char
            >::type s4;
    typedef mpl::select_c<
                false, long, false, long, false, long, false, long,
                true, char, false, long, false, long, false, long
            >::type s5;
    typedef mpl::select_c<
                false, long, false, long, false, long, false, long,
                false, long, true, char, false, long, false, long
            >::type s6;
    typedef mpl::select_c<
                false, long, false, long, false, long, false, long,
                false, long, false, long, true, char, false, long
            >::type s7;
    typedef mpl::select_c<
                false, long, false, long, false, long, false, long,
                false, long, false, long, false, long, true, char
            >::type s8;
    typedef mpl::select_c<
                false, long, false, long, false, long, false, long,
                false, long, false, long, false, long, false, long,
                true, char, false, long, false, long, false, long
            >::type s9;
    typedef mpl::select_c<
                false, long, false, long, false, long, false, long,
                false, long, false, long, false, long, false, long,
                false, long, true, char, false, long, false, long
            >::type s10;
    typedef mpl::select_c<
                false, long, false, long, false, long, false, long,
                false, long, false, long, false, long, false, long,
                false, long, false, long, true, char, false, long
            >::type s11;
    typedef mpl::select_c<
                false, long, false, long, false, long, false, long,
                false, long, false, long, false, long, false, long,
                false, long, false, long, false, long, true, char
            >::type s12;
    typedef mpl::select_c<
                false, long, false, long, false, long, false, long,
                false, long, false, long, false, long, false, long,
                true, char, false, long, false, long, false, long
            >::type s13;
    typedef mpl::select_c<
                false, long, false, long, false, long, false, long,
                false, long, false, long, false, long, false, long,
                false, long, false, long, false, long, false, long,
                false, long, true, char, false, long, false, long
            >::type s14;
    typedef mpl::select_c<
                false, long, false, long, false, long, false, long,
                false, long, false, long, false, long, false, long,
                false, long, false, long, false, long, false, long,
                false, long, false, long, true, char, false, long
            >::type s15;
    typedef mpl::select_c<
                false, long, false, long, false, long, false, long,
                false, long, false, long, false, long, false, long,
                false, long, false, long, false, long, false, long,
                false, long, false, long, false, long, true, char
            >::type s16;
    BOOST_MPL_ASSERT_IS_SAME(s1, char);
    BOOST_MPL_ASSERT_IS_SAME(s2, char);
    BOOST_MPL_ASSERT_IS_SAME(s3, char);
    BOOST_MPL_ASSERT_IS_SAME(s4, char);
    BOOST_MPL_ASSERT_IS_SAME(s5, char);
    BOOST_MPL_ASSERT_IS_SAME(s6, char);
    BOOST_MPL_ASSERT_IS_SAME(s7, char);
    BOOST_MPL_ASSERT_IS_SAME(s8, char);
    BOOST_MPL_ASSERT_IS_SAME(s9, char);
    BOOST_MPL_ASSERT_IS_SAME(s10, char);
    BOOST_MPL_ASSERT_IS_SAME(s11, char);
    BOOST_MPL_ASSERT_IS_SAME(s12, char);
    BOOST_MPL_ASSERT_IS_SAME(s13, char);
    BOOST_MPL_ASSERT_IS_SAME(s14, char);
    BOOST_MPL_ASSERT_IS_SAME(s15, char);
    BOOST_MPL_ASSERT_IS_SAME(s16, char);

    //----------Test select with lazy no evaluation---------------------------//

    typedef mpl::select<
                true_, char, false_, long, false_, long, false_, long
            >::type t1;
    typedef mpl::select<
                false_, long, true_, char, false_, long, false_, long
            >::type t2;
    typedef mpl::select<
                false_, long, false_, long, true_, char, false_, long
            >::type t3;
    typedef mpl::select<
                false_, long, false_, long, false_, long, true_, char
            >::type t4;
    typedef mpl::select<
                false_, long, false_, long, false_, long, false_, long,
                true_, char, false_, long, false_, long, false_, long
            >::type t5;
    typedef mpl::select<
                false_, long, false_, long, false_, long, false_, long,
                false_, long, true_, char, false_, long, false_, long
            >::type t6;
    typedef mpl::select<
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, true_, char, false_, long
            >::type t7;
    typedef mpl::select<
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, false_, long, true_, char
            >::type t8;
    typedef mpl::select<
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, false_, long, false_, long,
                true_, char, false_, long, false_, long, false_, long
            >::type t9;
    typedef mpl::select<
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, false_, long, false_, long,
                false_, long, true_, char, false_, long, false_, long
            >::type t10;
    typedef mpl::select<
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, true_, char, false_, long
            >::type t11;
    typedef mpl::select<
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, false_, long, true_, char
            >::type t12;
    typedef mpl::select<
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, false_, long, false_, long,
                true_, char, false_, long, false_, long, false_, long
            >::type t13;
    typedef mpl::select<
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, false_, long, false_, long,
                false_, long, true_, char, false_, long, false_, long
            >::type t14;
    typedef mpl::select<
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, true_, char, false_, long
            >::type t15;
    typedef mpl::select<
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, false_, long, true_, char
            >::type t16;
    BOOST_MPL_ASSERT_IS_SAME(t1, char);
    BOOST_MPL_ASSERT_IS_SAME(t2, char);
    BOOST_MPL_ASSERT_IS_SAME(t3, char);
    BOOST_MPL_ASSERT_IS_SAME(t4, char);
    BOOST_MPL_ASSERT_IS_SAME(t5, char);
    BOOST_MPL_ASSERT_IS_SAME(t6, char);
    BOOST_MPL_ASSERT_IS_SAME(t7, char);
    BOOST_MPL_ASSERT_IS_SAME(t8, char);
    BOOST_MPL_ASSERT_IS_SAME(t9, char);
    BOOST_MPL_ASSERT_IS_SAME(t10, char);
    BOOST_MPL_ASSERT_IS_SAME(t11, char);
    BOOST_MPL_ASSERT_IS_SAME(t12, char);
    BOOST_MPL_ASSERT_IS_SAME(t13, char);
    BOOST_MPL_ASSERT_IS_SAME(t14, char);
    BOOST_MPL_ASSERT_IS_SAME(t15, char);
    BOOST_MPL_ASSERT_IS_SAME(t16, char);

    //----------Test select_c with lazy evaluation----------------------------//

    typedef mpl::select_c<
                true, lazy<lazy_char>, false, long, false, long, false, long
            >::type u1;
    typedef mpl::select_c<
                false, long, true, lazy<lazy_char>, false, long, false, long
            >::type u2;
    typedef mpl::select_c<
                false, long, false, long, true, lazy<lazy_char>, false, long
            >::type u3;
    typedef mpl::select_c<
                false, long, false, long, false, long, true, lazy<lazy_char>
            >::type u4;
    typedef mpl::select_c<
                false, long, false, long, false, long, false, long,
                true, lazy<lazy_char>, false, long, false, long, false, long
            >::type u5;
    typedef mpl::select_c<
                false, long, false, long, false, long, false, long,
                false, long, true, lazy<lazy_char>, false, long, false, long
            >::type u6;
    typedef mpl::select_c<
                false, long, false, long, false, long, false, long,
                false, long, false, long, true, lazy<lazy_char>, false, long
            >::type u7;
    typedef mpl::select_c<
                false, long, false, long, false, long, false, long,
                false, long, false, long, false, long, true, lazy<lazy_char>
            >::type u8;
    typedef mpl::select_c<
                false, long, false, long, false, long, false, long,
                false, long, false, long, false, long, false, long,
                true, lazy<lazy_char>, false, long, false, long, false, long
            >::type u9;
    typedef mpl::select_c<
                false, long, false, long, false, long, false, long,
                false, long, false, long, false, long, false, long,
                false, long, true, lazy<lazy_char>, false, long, false, long
            >::type u10;
    typedef mpl::select_c<
                false, long, false, long, false, long, false, long,
                false, long, false, long, false, long, false, long,
                false, long, false, long, true, lazy<lazy_char>, false, long
            >::type u11;
    typedef mpl::select_c<
                false, long, false, long, false, long, false, long,
                false, long, false, long, false, long, false, long,
                false, long, false, long, false, long, true, lazy<lazy_char>
            >::type u12;
    typedef mpl::select_c<
                false, long, false, long, false, long, false, long,
                false, long, false, long, false, long, false, long,
                true, lazy<lazy_char>, false, long, false, long, false, long
            >::type u13;
    typedef mpl::select_c<
                false, long, false, long, false, long, false, long,
                false, long, false, long, false, long, false, long,
                false, long, false, long, false, long, false, long,
                false, long, true, lazy<lazy_char>, false, long, false, long
            >::type u14;
    typedef mpl::select_c<
                false, long, false, long, false, long, false, long,
                false, long, false, long, false, long, false, long,
                false, long, false, long, false, long, false, long,
                false, long, false, long, true, lazy<lazy_char>, false, long
            >::type u15;
    typedef mpl::select_c<
                false, long, false, long, false, long, false, long,
                false, long, false, long, false, long, false, long,
                false, long, false, long, false, long, false, long,
                false, long, false, long, false, long, true, lazy<lazy_char>
            >::type u16;
    BOOST_MPL_ASSERT_IS_SAME(u1, char);
    BOOST_MPL_ASSERT_IS_SAME(u2, char);
    BOOST_MPL_ASSERT_IS_SAME(u3, char);
    BOOST_MPL_ASSERT_IS_SAME(u4, char);
    BOOST_MPL_ASSERT_IS_SAME(u5, char);
    BOOST_MPL_ASSERT_IS_SAME(u6, char);
    BOOST_MPL_ASSERT_IS_SAME(u7, char);
    BOOST_MPL_ASSERT_IS_SAME(u8, char);
    BOOST_MPL_ASSERT_IS_SAME(u9, char);
    BOOST_MPL_ASSERT_IS_SAME(u10, char);
    BOOST_MPL_ASSERT_IS_SAME(u11, char);
    BOOST_MPL_ASSERT_IS_SAME(u12, char);
    BOOST_MPL_ASSERT_IS_SAME(u13, char);
    BOOST_MPL_ASSERT_IS_SAME(u14, char);
    BOOST_MPL_ASSERT_IS_SAME(u15, char);
    BOOST_MPL_ASSERT_IS_SAME(u16, char);

    //----------Test select with lazy evaluation------------------------------//

    typedef mpl::select<
                true_, lazy<lazy_char>, false_, long, false_, long, false_, long
            >::type v1;
    typedef mpl::select<
                false_, long, true_, lazy<lazy_char>, false_, long, false_, long
            >::type v2;
    typedef mpl::select<
                false_, long, false_, long, true_, lazy<lazy_char>, false_, long
            >::type v3;
    typedef mpl::select<
                false_, long, false_, long, false_, long, true_, lazy<lazy_char>
            >::type v4;
    typedef mpl::select<
                false_, long, false_, long, false_, long, false_, long,
                true_, lazy<lazy_char>, false_, long, false_, long, false_, long
            >::type v5;
    typedef mpl::select<
                false_, long, false_, long, false_, long, false_, long,
                false_, long, true_, lazy<lazy_char>, false_, long, false_, long
            >::type v6;
    typedef mpl::select<
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, true_, lazy<lazy_char>, false_, long
            >::type v7;
    typedef mpl::select<
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, false_, long, true_, lazy<lazy_char>
            >::type v8;
    typedef mpl::select<
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, false_, long, false_, long,
                true_, lazy<lazy_char>, false_, long, false_, long, false_, long
            >::type v9;
    typedef mpl::select<
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, false_, long, false_, long,
                false_, long, true_, lazy<lazy_char>, false_, long, false_, long
            >::type v10;
    typedef mpl::select<
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, true_, lazy<lazy_char>, false_, long
            >::type v11;
    typedef mpl::select<
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, false_, long, true_, lazy<lazy_char>
            >::type v12;
    typedef mpl::select<
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, false_, long, false_, long,
                true_, lazy<lazy_char>, false_, long, false_, long, false_, long
            >::type v13;
    typedef mpl::select<
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, false_, long, false_, long,
                false_, long, true_, lazy<lazy_char>, false_, long, false_, long
            >::type v14;
    typedef mpl::select<
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, true_, lazy<lazy_char>, false_, long
            >::type v15;
    typedef mpl::select<
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, false_, long, false_, long,
                false_, long, false_, long, false_, long, true_, lazy<lazy_char>
            >::type v16;
    BOOST_MPL_ASSERT_IS_SAME(v1, char);
    BOOST_MPL_ASSERT_IS_SAME(v2, char);
    BOOST_MPL_ASSERT_IS_SAME(v3, char);
    BOOST_MPL_ASSERT_IS_SAME(v4, char);
    BOOST_MPL_ASSERT_IS_SAME(v5, char);
    BOOST_MPL_ASSERT_IS_SAME(v6, char);
    BOOST_MPL_ASSERT_IS_SAME(v7, char);
    BOOST_MPL_ASSERT_IS_SAME(v8, char);
    BOOST_MPL_ASSERT_IS_SAME(v9, char);
    BOOST_MPL_ASSERT_IS_SAME(v10, char);
    BOOST_MPL_ASSERT_IS_SAME(v11, char);
    BOOST_MPL_ASSERT_IS_SAME(v12, char);
    BOOST_MPL_ASSERT_IS_SAME(v13, char);
    BOOST_MPL_ASSERT_IS_SAME(v14, char);
    BOOST_MPL_ASSERT_IS_SAME(v15, char);
    BOOST_MPL_ASSERT_IS_SAME(v16, char);

    //----------Test lazy_select_c--------------------------------------------//

    typedef mpl::lazy_select_c<
                true, lazy_char, false, lazy_long, false, lazy_long, false, lazy_long
            >::type w1;
    typedef mpl::lazy_select_c<
                false, lazy_long, true, lazy_char, false, lazy_long, false, lazy_long
            >::type w2;
    typedef mpl::lazy_select_c<
                false, lazy_long, false, lazy_long, true, lazy_char, false, lazy_long
            >::type w3;
    typedef mpl::lazy_select_c<
                false, lazy_long, false, lazy_long, false, lazy_long, true, lazy_char
            >::type w4;
    typedef mpl::lazy_select_c<
                false, lazy_long, false, lazy_long, false, lazy_long, false, lazy_long,
                true, lazy_char, false, lazy_long, false, lazy_long, false, lazy_long
            >::type w5;
    typedef mpl::lazy_select_c<
                false, lazy_long, false, lazy_long, false, lazy_long, false, lazy_long,
                false, lazy_long, true, lazy_char, false, lazy_long, false, lazy_long
            >::type w6;
    typedef mpl::lazy_select_c<
                false, lazy_long, false, lazy_long, false, lazy_long, false, lazy_long,
                false, lazy_long, false, lazy_long, true, lazy_char, false, lazy_long
            >::type w7;
    typedef mpl::lazy_select_c<
                false, lazy_long, false, lazy_long, false, lazy_long, false, lazy_long,
                false, lazy_long, false, lazy_long, false, lazy_long, true, lazy_char
            >::type w8;
    typedef mpl::lazy_select_c<
                false, lazy_long, false, lazy_long, false, lazy_long, false, lazy_long,
                false, lazy_long, false, lazy_long, false, lazy_long, false, lazy_long,
                true, lazy_char, false, lazy_long, false, lazy_long, false, lazy_long
            >::type w9;
    typedef mpl::lazy_select_c<
                false, lazy_long, false, lazy_long, false, lazy_long, false, lazy_long,
                false, lazy_long, false, lazy_long, false, lazy_long, false, lazy_long,
                false, lazy_long, true, lazy_char, false, lazy_long, false, lazy_long
            >::type w10;
    typedef mpl::lazy_select_c<
                false, lazy_long, false, lazy_long, false, lazy_long, false, lazy_long,
                false, lazy_long, false, lazy_long, false, lazy_long, false, lazy_long,
                false, lazy_long, false, lazy_long, true, lazy_char, false, lazy_long
            >::type w11;
    typedef mpl::lazy_select_c<
                false, lazy_long, false, lazy_long, false, lazy_long, false, lazy_long,
                false, lazy_long, false, lazy_long, false, lazy_long, false, lazy_long,
                false, lazy_long, false, lazy_long, false, lazy_long, true, lazy_char
            >::type w12;
    typedef mpl::lazy_select_c<
                false, lazy_long, false, lazy_long, false, lazy_long, false, lazy_long,
                false, lazy_long, false, lazy_long, false, lazy_long, false, lazy_long,
                true, lazy_char, false, lazy_long, false, lazy_long, false, lazy_long
            >::type w13;
    typedef mpl::lazy_select_c<
                false, lazy_long, false, lazy_long, false, lazy_long, false, lazy_long,
                false, lazy_long, false, lazy_long, false, lazy_long, false, lazy_long,
                false, lazy_long, false, lazy_long, false, lazy_long, false, lazy_long,
                false, lazy_long, true, lazy_char, false, lazy_long, false, lazy_long
            >::type w14;
    typedef mpl::lazy_select_c<
                false, lazy_long, false, lazy_long, false, lazy_long, false, lazy_long,
                false, lazy_long, false, lazy_long, false, lazy_long, false, lazy_long,
                false, lazy_long, false, lazy_long, false, lazy_long, false, lazy_long,
                false, lazy_long, false, lazy_long, true, lazy_char, false, lazy_long
            >::type w15;
    typedef mpl::lazy_select_c<
                false, lazy_long, false, lazy_long, false, lazy_long, false, lazy_long,
                false, lazy_long, false, lazy_long, false, lazy_long, false, lazy_long,
                false, lazy_long, false, lazy_long, false, lazy_long, false, lazy_long,
                false, lazy_long, false, lazy_long, false, lazy_long, true, lazy_char
            >::type w16;
    BOOST_MPL_ASSERT_IS_SAME(w1, char);
    BOOST_MPL_ASSERT_IS_SAME(w2, char);
    BOOST_MPL_ASSERT_IS_SAME(w3, char);
    BOOST_MPL_ASSERT_IS_SAME(w4, char);
    BOOST_MPL_ASSERT_IS_SAME(w5, char);
    BOOST_MPL_ASSERT_IS_SAME(w6, char);
    BOOST_MPL_ASSERT_IS_SAME(w7, char);
    BOOST_MPL_ASSERT_IS_SAME(w8, char);
    BOOST_MPL_ASSERT_IS_SAME(w9, char);
    BOOST_MPL_ASSERT_IS_SAME(w10, char);
    BOOST_MPL_ASSERT_IS_SAME(w11, char);
    BOOST_MPL_ASSERT_IS_SAME(w12, char);
    BOOST_MPL_ASSERT_IS_SAME(w13, char);
    BOOST_MPL_ASSERT_IS_SAME(w14, char);
    BOOST_MPL_ASSERT_IS_SAME(w15, char);
    BOOST_MPL_ASSERT_IS_SAME(w16, char);

    //----------Test lazy_select----------------------------------------------//

    typedef mpl::lazy_select<
                true_, lazy_char, false_, lazy_long, false_, lazy_long, false_, lazy_long
            >::type x1;
    typedef mpl::lazy_select<
                false_, lazy_long, true_, lazy_char, false_, lazy_long, false_, lazy_long
            >::type x2;
    typedef mpl::lazy_select<
                false_, lazy_long, false_, lazy_long, true_, lazy_char, false_, lazy_long
            >::type x3;
    typedef mpl::lazy_select<
                false_, lazy_long, false_, lazy_long, false_, lazy_long, true_, lazy_char
            >::type x4;
    typedef mpl::lazy_select<
                false_, lazy_long, false_, lazy_long, false_, lazy_long, false_, lazy_long,
                true_, lazy_char, false_, lazy_long, false_, lazy_long, false_, lazy_long
            >::type x5;
    typedef mpl::lazy_select<
                false_, lazy_long, false_, lazy_long, false_, lazy_long, false_, lazy_long,
                false_, lazy_long, true_, lazy_char, false_, lazy_long, false_, lazy_long
            >::type x6;
    typedef mpl::lazy_select<
                false_, lazy_long, false_, lazy_long, false_, lazy_long, false_, lazy_long,
                false_, lazy_long, false_, lazy_long, true_, lazy_char, false_, lazy_long
            >::type x7;
    typedef mpl::lazy_select<
                false_, lazy_long, false_, lazy_long, false_, lazy_long, false_, lazy_long,
                false_, lazy_long, false_, lazy_long, false_, lazy_long, true_, lazy_char
            >::type x8;
    typedef mpl::lazy_select<
                false_, lazy_long, false_, lazy_long, false_, lazy_long, false_, lazy_long,
                false_, lazy_long, false_, lazy_long, false_, lazy_long, false_, lazy_long,
                true_, lazy_char, false_, lazy_long, false_, lazy_long, false_, lazy_long
            >::type x9;
    typedef mpl::lazy_select<
                false_, lazy_long, false_, lazy_long, false_, lazy_long, false_, lazy_long,
                false_, lazy_long, false_, lazy_long, false_, lazy_long, false_, lazy_long,
                false_, lazy_long, true_, lazy_char, false_, lazy_long, false_, lazy_long
            >::type x10;
    typedef mpl::lazy_select<
                false_, lazy_long, false_, lazy_long, false_, lazy_long, false_, lazy_long,
                false_, lazy_long, false_, lazy_long, false_, lazy_long, false_, lazy_long,
                false_, lazy_long, false_, lazy_long, true_, lazy_char, false_, lazy_long
            >::type x11;
    typedef mpl::lazy_select<
                false_, lazy_long, false_, lazy_long, false_, lazy_long, false_, lazy_long,
                false_, lazy_long, false_, lazy_long, false_, lazy_long, false_, lazy_long,
                false_, lazy_long, false_, lazy_long, false_, lazy_long, true_, lazy_char
            >::type x12;
    typedef mpl::lazy_select<
                false_, lazy_long, false_, lazy_long, false_, lazy_long, false_, lazy_long,
                false_, lazy_long, false_, lazy_long, false_, lazy_long, false_, lazy_long,
                true_, lazy_char, false_, lazy_long, false_, lazy_long, false_, lazy_long
            >::type x13;
    typedef mpl::lazy_select<
                false_, lazy_long, false_, lazy_long, false_, lazy_long, false_, lazy_long,
                false_, lazy_long, false_, lazy_long, false_, lazy_long, false_, lazy_long,
                false_, lazy_long, false_, lazy_long, false_, lazy_long, false_, lazy_long,
                false_, lazy_long, true_, lazy_char, false_, lazy_long, false_, lazy_long
            >::type x14;
    typedef mpl::lazy_select<
                false_, lazy_long, false_, lazy_long, false_, lazy_long, false_, lazy_long,
                false_, lazy_long, false_, lazy_long, false_, lazy_long, false_, lazy_long,
                false_, lazy_long, false_, lazy_long, false_, lazy_long, false_, lazy_long,
                false_, lazy_long, false_, lazy_long, true_, lazy_char, false_, lazy_long
            >::type x15;
    typedef mpl::lazy_select<
                false_, lazy_long, false_, lazy_long, false_, lazy_long, false_, lazy_long,
                false_, lazy_long, false_, lazy_long, false_, lazy_long, false_, lazy_long,
                false_, lazy_long, false_, lazy_long, false_, lazy_long, false_, lazy_long,
                false_, lazy_long, false_, lazy_long, false_, lazy_long, true_, lazy_char
            >::type x16;
    BOOST_MPL_ASSERT_IS_SAME(x1, char);
    BOOST_MPL_ASSERT_IS_SAME(x2, char);
    BOOST_MPL_ASSERT_IS_SAME(x3, char);
    BOOST_MPL_ASSERT_IS_SAME(x4, char);
    BOOST_MPL_ASSERT_IS_SAME(x5, char);
    BOOST_MPL_ASSERT_IS_SAME(x6, char);
    BOOST_MPL_ASSERT_IS_SAME(x7, char);
    BOOST_MPL_ASSERT_IS_SAME(x8, char);
    BOOST_MPL_ASSERT_IS_SAME(x9, char);
    BOOST_MPL_ASSERT_IS_SAME(x10, char);
    BOOST_MPL_ASSERT_IS_SAME(x11, char);
    BOOST_MPL_ASSERT_IS_SAME(x12, char);
    BOOST_MPL_ASSERT_IS_SAME(x13, char);
    BOOST_MPL_ASSERT_IS_SAME(x14, char);
    BOOST_MPL_ASSERT_IS_SAME(x15, char);
    BOOST_MPL_ASSERT_IS_SAME(x16, char);

    return 0;
}
