#ifndef BOOST_PREPROCESSOR_REPEAT_HPP
#define BOOST_PREPROCESSOR_REPEAT_HPP

/* Copyright (C) 2001
 * Housemarque Oy
 * http://www.housemarque.com
 *
 * Permission to copy, use, modify, sell and distribute this software is
 * granted provided this copyright notice appears in all copies. This
 * software is provided "as is" without express or implied warranty, and
 * with no claim as to its suitability for any purpose.
 *
 * See http://www.boost.org for most recent version.
 */

#include <boost/preprocessor/detail/cat.hpp>
#include <boost/preprocessor/detail/auto_rec.hpp>
#include <boost/preprocessor/expand.hpp>

/** <p>Repeats the macro <code>MACRO(INDEX,DATA)</code> for <code>INDEX = [0,COUNT)</code>.</p>

<p>In other words, expands to the sequence:</p>

<pre>
MACRO(0,DATA) MACRO(1,DATA) ... MACRO(BOOST_PP_DEC(COUNT),DATA)
</pre>

<p>For example,</p>

<pre>
#define TEST(INDEX,DATA) DATA(INDEX);
BOOST_PP_REPEAT(3,TEST,X)
</pre>

<p>expands to:</p>

<pre>
X(0); X(1); X(2);
</pre>

<h3>2D and 3D repetition</h3>

<p>BOOST_PP_REPEAT() implements automatic recursion. 2D and 3D repetition
are directly supported.</p>

<h3>Example</h3>
<ul>
  <li><a href="../../example/duffs_device.c">duffs_device.c</a></li>
  <li><a href="../../example/repeat_2d.c">repeat_2d.c</a></li>
</ul>

<h3>See</h3>
<ul>
  <li>BOOST_PP_FOR()</li>
  <li>BOOST_PP_LIMIT_DIM</li>
  <li>BOOST_PP_LIMIT_MAG</li>
</ul>

<h3>Test</h3>
<ul>
  <li><a href="../../test/repeat_test.cpp">repeat_test.cpp</a></li>
  <li><a href="../../test/repeat_2nd_test.cpp">repeat_2nd_test.cpp</a></li>
</ul>
*/
#if 0
#  define BOOST_PP_REPEAT(COUNT,MACRO,DATA)
#endif

#if !defined(BOOST_NO_COMPILER_CONFIG) && defined(_MSC_VER)
/* This is a workaround for a MSVC++ PP bug. You'll need to add further
 * BOOST_PP_REPEAT_AUTO_REC3/4/etc. wrapping as the maximum level of 
 * nesting REPEATS increases
 */
#  define BOOST_PP_REPEAT\
     BOOST_PP_AUTO_REC_CAT1(BOOST_PP_REPEAT_,BOOST_PP_REPEAT_AUTO_REC2(\
       BOOST_PP_REPEAT_AUTO_REC1,(BOOST_PP_AUTO_REC_ID,(1))\
       ))
#else
#  define BOOST_PP_REPEAT\
    BOOST_PP_AUTO_REC_CAT1(BOOST_PP_REPEAT_,BOOST_PP_REPEAT_AUTO_REC1(BOOST_PP_AUTO_REC_ID,(1)))
#endif

#define BOOST_PP_REPEAT_BOOST_PP_REPEAT_AUTO_REC1(M,P)\
  BOOST_PP_AUTO_REC_CAT2(BOOST_PP_REPEAT_,BOOST_PP_REPEAT_AUTO_REC2(BOOST_PP_AUTO_REC_ID,(2)))
#define BOOST_PP_REPEAT_BOOST_PP_REPEAT_AUTO_REC2(M,P)\
  BOOST_PP_AUTO_REC_CAT3(BOOST_PP_REPEAT_,BOOST_PP_REPEAT_AUTO_REC3(BOOST_PP_AUTO_REC_ID,(3)))
#define BOOST_PP_REPEAT_BOOST_PP_REPEAT_AUTO_REC3(M,P)\
  (TOO MANY NESTED REPEATS!)

#define BOOST_PP_REPEAT_AUTO_REC1(M,P) BOOST_PP_EXPAND(M P)
#define BOOST_PP_REPEAT_AUTO_REC2(M,P) BOOST_PP_EXPAND(M P)
#define BOOST_PP_REPEAT_AUTO_REC3(M,P) BOOST_PP_EXPAND(M P)

#define BOOST_PP_REPEAT_1(C,M,D) BOOST_PP_REPEAT_AUTO_REC1(BOOST_PP_DETAIL_CAT2(BOOST_PP_R1_,C),(M,D))
#define BOOST_PP_REPEAT_2(C,M,D) BOOST_PP_REPEAT_AUTO_REC2(BOOST_PP_DETAIL_CAT2(BOOST_PP_R2_,C),(M,D))
#define BOOST_PP_REPEAT_3(C,M,D) BOOST_PP_REPEAT_AUTO_REC3(BOOST_PP_DETAIL_CAT2(BOOST_PP_R3_,C),(M,D))

#if defined __EDG__ // unrolled repeats for EDG front end
#include <boost/preprocessor/detail/repeat_edg.hpp>
#else
#error edg?

#define BOOST_PP_R1_0(M,D)
#define BOOST_PP_R1_1(M,D) M(0,D)
#define BOOST_PP_R1_2(M,D) M(0,D) M(1,D)
#define BOOST_PP_R1_3(M,D) M(0,D) M(1,D) M(2,D)
#define BOOST_PP_R1_4(M,D) M(0,D) M(1,D) M(2,D) M(3,D)
#define BOOST_PP_R1_5(M,D) BOOST_PP_R1_4(M,D) M(4,D)
#define BOOST_PP_R1_6(M,D) BOOST_PP_R1_5(M,D) M(5,D)
#define BOOST_PP_R1_7(M,D) BOOST_PP_R1_6(M,D) M(6,D)
#define BOOST_PP_R1_8(M,D) BOOST_PP_R1_7(M,D) M(7,D)
#define BOOST_PP_R1_9(M,D) BOOST_PP_R1_8(M,D) M(8,D)
#define BOOST_PP_R1_10(M,D) BOOST_PP_R1_9(M,D) M(9,D)
#define BOOST_PP_R1_11(M,D) BOOST_PP_R1_10(M,D) M(10,D)
#define BOOST_PP_R1_12(M,D) BOOST_PP_R1_11(M,D) M(11,D)
#define BOOST_PP_R1_13(M,D) BOOST_PP_R1_12(M,D) M(12,D)
#define BOOST_PP_R1_14(M,D) BOOST_PP_R1_13(M,D) M(13,D)
#define BOOST_PP_R1_15(M,D) BOOST_PP_R1_14(M,D) M(14,D)
#define BOOST_PP_R1_16(M,D) BOOST_PP_R1_15(M,D) M(15,D)
#define BOOST_PP_R1_17(M,D) BOOST_PP_R1_16(M,D) M(16,D)
#define BOOST_PP_R1_18(M,D) BOOST_PP_R1_17(M,D) M(17,D)
#define BOOST_PP_R1_19(M,D) BOOST_PP_R1_18(M,D) M(18,D)
#define BOOST_PP_R1_20(M,D) BOOST_PP_R1_19(M,D) M(19,D)
#define BOOST_PP_R1_21(M,D) BOOST_PP_R1_20(M,D) M(20,D)
#define BOOST_PP_R1_22(M,D) BOOST_PP_R1_21(M,D) M(21,D)
#define BOOST_PP_R1_23(M,D) BOOST_PP_R1_22(M,D) M(22,D)
#define BOOST_PP_R1_24(M,D) BOOST_PP_R1_23(M,D) M(23,D)
#define BOOST_PP_R1_25(M,D) BOOST_PP_R1_24(M,D) M(24,D)
#define BOOST_PP_R1_26(M,D) BOOST_PP_R1_25(M,D) M(25,D)
#define BOOST_PP_R1_27(M,D) BOOST_PP_R1_26(M,D) M(26,D)
#define BOOST_PP_R1_28(M,D) BOOST_PP_R1_27(M,D) M(27,D)
#define BOOST_PP_R1_29(M,D) BOOST_PP_R1_28(M,D) M(28,D)
#define BOOST_PP_R1_30(M,D) BOOST_PP_R1_29(M,D) M(29,D)
#define BOOST_PP_R1_31(M,D) BOOST_PP_R1_30(M,D) M(30,D)
#define BOOST_PP_R1_32(M,D) BOOST_PP_R1_31(M,D) M(31,D)
#define BOOST_PP_R1_33(M,D) BOOST_PP_R1_32(M,D) M(32,D)
#define BOOST_PP_R1_34(M,D) BOOST_PP_R1_33(M,D) M(33,D)
#define BOOST_PP_R1_35(M,D) BOOST_PP_R1_34(M,D) M(34,D)
#define BOOST_PP_R1_36(M,D) BOOST_PP_R1_35(M,D) M(35,D)
#define BOOST_PP_R1_37(M,D) BOOST_PP_R1_36(M,D) M(36,D)
#define BOOST_PP_R1_38(M,D) BOOST_PP_R1_37(M,D) M(37,D)
#define BOOST_PP_R1_39(M,D) BOOST_PP_R1_38(M,D) M(38,D)
#define BOOST_PP_R1_40(M,D) BOOST_PP_R1_39(M,D) M(39,D)
#define BOOST_PP_R1_41(M,D) BOOST_PP_R1_40(M,D) M(40,D)
#define BOOST_PP_R1_42(M,D) BOOST_PP_R1_41(M,D) M(41,D)
#define BOOST_PP_R1_43(M,D) BOOST_PP_R1_42(M,D) M(42,D)
#define BOOST_PP_R1_44(M,D) BOOST_PP_R1_43(M,D) M(43,D)
#define BOOST_PP_R1_45(M,D) BOOST_PP_R1_44(M,D) M(44,D)
#define BOOST_PP_R1_46(M,D) BOOST_PP_R1_45(M,D) M(45,D)
#define BOOST_PP_R1_47(M,D) BOOST_PP_R1_46(M,D) M(46,D)
#define BOOST_PP_R1_48(M,D) BOOST_PP_R1_47(M,D) M(47,D)
#define BOOST_PP_R1_49(M,D) BOOST_PP_R1_48(M,D) M(48,D)
#define BOOST_PP_R1_50(M,D) BOOST_PP_R1_49(M,D) M(49,D)
#define BOOST_PP_R1_51(M,D) BOOST_PP_R1_50(M,D) M(50,D)
#define BOOST_PP_R1_52(M,D) BOOST_PP_R1_51(M,D) M(51,D)
#define BOOST_PP_R1_53(M,D) BOOST_PP_R1_52(M,D) M(52,D)
#define BOOST_PP_R1_54(M,D) BOOST_PP_R1_53(M,D) M(53,D)
#define BOOST_PP_R1_55(M,D) BOOST_PP_R1_54(M,D) M(54,D)
#define BOOST_PP_R1_56(M,D) BOOST_PP_R1_55(M,D) M(55,D)
#define BOOST_PP_R1_57(M,D) BOOST_PP_R1_56(M,D) M(56,D)
#define BOOST_PP_R1_58(M,D) BOOST_PP_R1_57(M,D) M(57,D)
#define BOOST_PP_R1_59(M,D) BOOST_PP_R1_58(M,D) M(58,D)
#define BOOST_PP_R1_60(M,D) BOOST_PP_R1_59(M,D) M(59,D)
#define BOOST_PP_R1_61(M,D) BOOST_PP_R1_60(M,D) M(60,D)
#define BOOST_PP_R1_62(M,D) BOOST_PP_R1_61(M,D) M(61,D)
#define BOOST_PP_R1_63(M,D) BOOST_PP_R1_62(M,D) M(62,D)
#define BOOST_PP_R1_64(M,D) BOOST_PP_R1_63(M,D) M(63,D)
#define BOOST_PP_R1_65(M,D) BOOST_PP_R1_64(M,D) M(64,D)
#define BOOST_PP_R1_66(M,D) BOOST_PP_R1_65(M,D) M(65,D)
#define BOOST_PP_R1_67(M,D) BOOST_PP_R1_66(M,D) M(66,D)
#define BOOST_PP_R1_68(M,D) BOOST_PP_R1_67(M,D) M(67,D)
#define BOOST_PP_R1_69(M,D) BOOST_PP_R1_68(M,D) M(68,D)
#define BOOST_PP_R1_70(M,D) BOOST_PP_R1_69(M,D) M(69,D)
#define BOOST_PP_R1_71(M,D) BOOST_PP_R1_70(M,D) M(70,D)
#define BOOST_PP_R1_72(M,D) BOOST_PP_R1_71(M,D) M(71,D)
#define BOOST_PP_R1_73(M,D) BOOST_PP_R1_72(M,D) M(72,D)
#define BOOST_PP_R1_74(M,D) BOOST_PP_R1_73(M,D) M(73,D)
#define BOOST_PP_R1_75(M,D) BOOST_PP_R1_74(M,D) M(74,D)
#define BOOST_PP_R1_76(M,D) BOOST_PP_R1_75(M,D) M(75,D)
#define BOOST_PP_R1_77(M,D) BOOST_PP_R1_76(M,D) M(76,D)
#define BOOST_PP_R1_78(M,D) BOOST_PP_R1_77(M,D) M(77,D)
#define BOOST_PP_R1_79(M,D) BOOST_PP_R1_78(M,D) M(78,D)
#define BOOST_PP_R1_80(M,D) BOOST_PP_R1_79(M,D) M(79,D)
#define BOOST_PP_R1_81(M,D) BOOST_PP_R1_80(M,D) M(80,D)
#define BOOST_PP_R1_82(M,D) BOOST_PP_R1_81(M,D) M(81,D)
#define BOOST_PP_R1_83(M,D) BOOST_PP_R1_82(M,D) M(82,D)
#define BOOST_PP_R1_84(M,D) BOOST_PP_R1_83(M,D) M(83,D)
#define BOOST_PP_R1_85(M,D) BOOST_PP_R1_84(M,D) M(84,D)
#define BOOST_PP_R1_86(M,D) BOOST_PP_R1_85(M,D) M(85,D)
#define BOOST_PP_R1_87(M,D) BOOST_PP_R1_86(M,D) M(86,D)
#define BOOST_PP_R1_88(M,D) BOOST_PP_R1_87(M,D) M(87,D)
#define BOOST_PP_R1_89(M,D) BOOST_PP_R1_88(M,D) M(88,D)
#define BOOST_PP_R1_90(M,D) BOOST_PP_R1_89(M,D) M(89,D)
#define BOOST_PP_R1_91(M,D) BOOST_PP_R1_90(M,D) M(90,D)
#define BOOST_PP_R1_92(M,D) BOOST_PP_R1_91(M,D) M(91,D)
#define BOOST_PP_R1_93(M,D) BOOST_PP_R1_92(M,D) M(92,D)
#define BOOST_PP_R1_94(M,D) BOOST_PP_R1_93(M,D) M(93,D)
#define BOOST_PP_R1_95(M,D) BOOST_PP_R1_94(M,D) M(94,D)
#define BOOST_PP_R1_96(M,D) BOOST_PP_R1_95(M,D) M(95,D)
#define BOOST_PP_R1_97(M,D) BOOST_PP_R1_96(M,D) M(96,D)
#define BOOST_PP_R1_98(M,D) BOOST_PP_R1_97(M,D) M(97,D)
#define BOOST_PP_R1_99(M,D) BOOST_PP_R1_98(M,D) M(98,D)
#define BOOST_PP_R1_100(M,D) BOOST_PP_R1_99(M,D) M(99,D)
#define BOOST_PP_R1_101(M,D) BOOST_PP_R1_100(M,D) M(100,D)
#define BOOST_PP_R1_102(M,D) BOOST_PP_R1_101(M,D) M(101,D)
#define BOOST_PP_R1_103(M,D) BOOST_PP_R1_102(M,D) M(102,D)
#define BOOST_PP_R1_104(M,D) BOOST_PP_R1_103(M,D) M(103,D)
#define BOOST_PP_R1_105(M,D) BOOST_PP_R1_104(M,D) M(104,D)
#define BOOST_PP_R1_106(M,D) BOOST_PP_R1_105(M,D) M(105,D)
#define BOOST_PP_R1_107(M,D) BOOST_PP_R1_106(M,D) M(106,D)
#define BOOST_PP_R1_108(M,D) BOOST_PP_R1_107(M,D) M(107,D)
#define BOOST_PP_R1_109(M,D) BOOST_PP_R1_108(M,D) M(108,D)
#define BOOST_PP_R1_110(M,D) BOOST_PP_R1_109(M,D) M(109,D)
#define BOOST_PP_R1_111(M,D) BOOST_PP_R1_110(M,D) M(110,D)
#define BOOST_PP_R1_112(M,D) BOOST_PP_R1_111(M,D) M(111,D)
#define BOOST_PP_R1_113(M,D) BOOST_PP_R1_112(M,D) M(112,D)
#define BOOST_PP_R1_114(M,D) BOOST_PP_R1_113(M,D) M(113,D)
#define BOOST_PP_R1_115(M,D) BOOST_PP_R1_114(M,D) M(114,D)
#define BOOST_PP_R1_116(M,D) BOOST_PP_R1_115(M,D) M(115,D)
#define BOOST_PP_R1_117(M,D) BOOST_PP_R1_116(M,D) M(116,D)
#define BOOST_PP_R1_118(M,D) BOOST_PP_R1_117(M,D) M(117,D)
#define BOOST_PP_R1_119(M,D) BOOST_PP_R1_118(M,D) M(118,D)
#define BOOST_PP_R1_120(M,D) BOOST_PP_R1_119(M,D) M(119,D)
#define BOOST_PP_R1_121(M,D) BOOST_PP_R1_120(M,D) M(120,D)
#define BOOST_PP_R1_122(M,D) BOOST_PP_R1_121(M,D) M(121,D)
#define BOOST_PP_R1_123(M,D) BOOST_PP_R1_122(M,D) M(122,D)
#define BOOST_PP_R1_124(M,D) BOOST_PP_R1_123(M,D) M(123,D)
#define BOOST_PP_R1_125(M,D) BOOST_PP_R1_124(M,D) M(124,D)
#define BOOST_PP_R1_126(M,D) BOOST_PP_R1_125(M,D) M(125,D)
#define BOOST_PP_R1_127(M,D) BOOST_PP_R1_126(M,D) M(126,D)
#define BOOST_PP_R1_128(M,D) BOOST_PP_R1_127(M,D) M(127,D)

#define BOOST_PP_R2_0(M,D)
#define BOOST_PP_R2_1(M,D) M(0,D)
#define BOOST_PP_R2_2(M,D) M(0,D) M(1,D)
#define BOOST_PP_R2_3(M,D) M(0,D) M(1,D) M(2,D)
#define BOOST_PP_R2_4(M,D) M(0,D) M(1,D) M(2,D) M(3,D)
#define BOOST_PP_R2_5(M,D) BOOST_PP_R2_4(M,D) M(4,D)
#define BOOST_PP_R2_6(M,D) BOOST_PP_R2_5(M,D) M(5,D)
#define BOOST_PP_R2_7(M,D) BOOST_PP_R2_6(M,D) M(6,D)
#define BOOST_PP_R2_8(M,D) BOOST_PP_R2_7(M,D) M(7,D)
#define BOOST_PP_R2_9(M,D) BOOST_PP_R2_8(M,D) M(8,D)
#define BOOST_PP_R2_10(M,D) BOOST_PP_R2_9(M,D) M(9,D)
#define BOOST_PP_R2_11(M,D) BOOST_PP_R2_10(M,D) M(10,D)
#define BOOST_PP_R2_12(M,D) BOOST_PP_R2_11(M,D) M(11,D)
#define BOOST_PP_R2_13(M,D) BOOST_PP_R2_12(M,D) M(12,D)
#define BOOST_PP_R2_14(M,D) BOOST_PP_R2_13(M,D) M(13,D)
#define BOOST_PP_R2_15(M,D) BOOST_PP_R2_14(M,D) M(14,D)
#define BOOST_PP_R2_16(M,D) BOOST_PP_R2_15(M,D) M(15,D)
#define BOOST_PP_R2_17(M,D) BOOST_PP_R2_16(M,D) M(16,D)
#define BOOST_PP_R2_18(M,D) BOOST_PP_R2_17(M,D) M(17,D)
#define BOOST_PP_R2_19(M,D) BOOST_PP_R2_18(M,D) M(18,D)
#define BOOST_PP_R2_20(M,D) BOOST_PP_R2_19(M,D) M(19,D)
#define BOOST_PP_R2_21(M,D) BOOST_PP_R2_20(M,D) M(20,D)
#define BOOST_PP_R2_22(M,D) BOOST_PP_R2_21(M,D) M(21,D)
#define BOOST_PP_R2_23(M,D) BOOST_PP_R2_22(M,D) M(22,D)
#define BOOST_PP_R2_24(M,D) BOOST_PP_R2_23(M,D) M(23,D)
#define BOOST_PP_R2_25(M,D) BOOST_PP_R2_24(M,D) M(24,D)
#define BOOST_PP_R2_26(M,D) BOOST_PP_R2_25(M,D) M(25,D)
#define BOOST_PP_R2_27(M,D) BOOST_PP_R2_26(M,D) M(26,D)
#define BOOST_PP_R2_28(M,D) BOOST_PP_R2_27(M,D) M(27,D)
#define BOOST_PP_R2_29(M,D) BOOST_PP_R2_28(M,D) M(28,D)
#define BOOST_PP_R2_30(M,D) BOOST_PP_R2_29(M,D) M(29,D)
#define BOOST_PP_R2_31(M,D) BOOST_PP_R2_30(M,D) M(30,D)
#define BOOST_PP_R2_32(M,D) BOOST_PP_R2_31(M,D) M(31,D)
#define BOOST_PP_R2_33(M,D) BOOST_PP_R2_32(M,D) M(32,D)
#define BOOST_PP_R2_34(M,D) BOOST_PP_R2_33(M,D) M(33,D)
#define BOOST_PP_R2_35(M,D) BOOST_PP_R2_34(M,D) M(34,D)
#define BOOST_PP_R2_36(M,D) BOOST_PP_R2_35(M,D) M(35,D)
#define BOOST_PP_R2_37(M,D) BOOST_PP_R2_36(M,D) M(36,D)
#define BOOST_PP_R2_38(M,D) BOOST_PP_R2_37(M,D) M(37,D)
#define BOOST_PP_R2_39(M,D) BOOST_PP_R2_38(M,D) M(38,D)
#define BOOST_PP_R2_40(M,D) BOOST_PP_R2_39(M,D) M(39,D)
#define BOOST_PP_R2_41(M,D) BOOST_PP_R2_40(M,D) M(40,D)
#define BOOST_PP_R2_42(M,D) BOOST_PP_R2_41(M,D) M(41,D)
#define BOOST_PP_R2_43(M,D) BOOST_PP_R2_42(M,D) M(42,D)
#define BOOST_PP_R2_44(M,D) BOOST_PP_R2_43(M,D) M(43,D)
#define BOOST_PP_R2_45(M,D) BOOST_PP_R2_44(M,D) M(44,D)
#define BOOST_PP_R2_46(M,D) BOOST_PP_R2_45(M,D) M(45,D)
#define BOOST_PP_R2_47(M,D) BOOST_PP_R2_46(M,D) M(46,D)
#define BOOST_PP_R2_48(M,D) BOOST_PP_R2_47(M,D) M(47,D)
#define BOOST_PP_R2_49(M,D) BOOST_PP_R2_48(M,D) M(48,D)
#define BOOST_PP_R2_50(M,D) BOOST_PP_R2_49(M,D) M(49,D)
#define BOOST_PP_R2_51(M,D) BOOST_PP_R2_50(M,D) M(50,D)
#define BOOST_PP_R2_52(M,D) BOOST_PP_R2_51(M,D) M(51,D)
#define BOOST_PP_R2_53(M,D) BOOST_PP_R2_52(M,D) M(52,D)
#define BOOST_PP_R2_54(M,D) BOOST_PP_R2_53(M,D) M(53,D)
#define BOOST_PP_R2_55(M,D) BOOST_PP_R2_54(M,D) M(54,D)
#define BOOST_PP_R2_56(M,D) BOOST_PP_R2_55(M,D) M(55,D)
#define BOOST_PP_R2_57(M,D) BOOST_PP_R2_56(M,D) M(56,D)
#define BOOST_PP_R2_58(M,D) BOOST_PP_R2_57(M,D) M(57,D)
#define BOOST_PP_R2_59(M,D) BOOST_PP_R2_58(M,D) M(58,D)
#define BOOST_PP_R2_60(M,D) BOOST_PP_R2_59(M,D) M(59,D)
#define BOOST_PP_R2_61(M,D) BOOST_PP_R2_60(M,D) M(60,D)
#define BOOST_PP_R2_62(M,D) BOOST_PP_R2_61(M,D) M(61,D)
#define BOOST_PP_R2_63(M,D) BOOST_PP_R2_62(M,D) M(62,D)
#define BOOST_PP_R2_64(M,D) BOOST_PP_R2_63(M,D) M(63,D)
#define BOOST_PP_R2_65(M,D) BOOST_PP_R2_64(M,D) M(64,D)
#define BOOST_PP_R2_66(M,D) BOOST_PP_R2_65(M,D) M(65,D)
#define BOOST_PP_R2_67(M,D) BOOST_PP_R2_66(M,D) M(66,D)
#define BOOST_PP_R2_68(M,D) BOOST_PP_R2_67(M,D) M(67,D)
#define BOOST_PP_R2_69(M,D) BOOST_PP_R2_68(M,D) M(68,D)
#define BOOST_PP_R2_70(M,D) BOOST_PP_R2_69(M,D) M(69,D)
#define BOOST_PP_R2_71(M,D) BOOST_PP_R2_70(M,D) M(70,D)
#define BOOST_PP_R2_72(M,D) BOOST_PP_R2_71(M,D) M(71,D)
#define BOOST_PP_R2_73(M,D) BOOST_PP_R2_72(M,D) M(72,D)
#define BOOST_PP_R2_74(M,D) BOOST_PP_R2_73(M,D) M(73,D)
#define BOOST_PP_R2_75(M,D) BOOST_PP_R2_74(M,D) M(74,D)
#define BOOST_PP_R2_76(M,D) BOOST_PP_R2_75(M,D) M(75,D)
#define BOOST_PP_R2_77(M,D) BOOST_PP_R2_76(M,D) M(76,D)
#define BOOST_PP_R2_78(M,D) BOOST_PP_R2_77(M,D) M(77,D)
#define BOOST_PP_R2_79(M,D) BOOST_PP_R2_78(M,D) M(78,D)
#define BOOST_PP_R2_80(M,D) BOOST_PP_R2_79(M,D) M(79,D)
#define BOOST_PP_R2_81(M,D) BOOST_PP_R2_80(M,D) M(80,D)
#define BOOST_PP_R2_82(M,D) BOOST_PP_R2_81(M,D) M(81,D)
#define BOOST_PP_R2_83(M,D) BOOST_PP_R2_82(M,D) M(82,D)
#define BOOST_PP_R2_84(M,D) BOOST_PP_R2_83(M,D) M(83,D)
#define BOOST_PP_R2_85(M,D) BOOST_PP_R2_84(M,D) M(84,D)
#define BOOST_PP_R2_86(M,D) BOOST_PP_R2_85(M,D) M(85,D)
#define BOOST_PP_R2_87(M,D) BOOST_PP_R2_86(M,D) M(86,D)
#define BOOST_PP_R2_88(M,D) BOOST_PP_R2_87(M,D) M(87,D)
#define BOOST_PP_R2_89(M,D) BOOST_PP_R2_88(M,D) M(88,D)
#define BOOST_PP_R2_90(M,D) BOOST_PP_R2_89(M,D) M(89,D)
#define BOOST_PP_R2_91(M,D) BOOST_PP_R2_90(M,D) M(90,D)
#define BOOST_PP_R2_92(M,D) BOOST_PP_R2_91(M,D) M(91,D)
#define BOOST_PP_R2_93(M,D) BOOST_PP_R2_92(M,D) M(92,D)
#define BOOST_PP_R2_94(M,D) BOOST_PP_R2_93(M,D) M(93,D)
#define BOOST_PP_R2_95(M,D) BOOST_PP_R2_94(M,D) M(94,D)
#define BOOST_PP_R2_96(M,D) BOOST_PP_R2_95(M,D) M(95,D)
#define BOOST_PP_R2_97(M,D) BOOST_PP_R2_96(M,D) M(96,D)
#define BOOST_PP_R2_98(M,D) BOOST_PP_R2_97(M,D) M(97,D)
#define BOOST_PP_R2_99(M,D) BOOST_PP_R2_98(M,D) M(98,D)
#define BOOST_PP_R2_100(M,D) BOOST_PP_R2_99(M,D) M(99,D)
#define BOOST_PP_R2_101(M,D) BOOST_PP_R2_100(M,D) M(100,D)
#define BOOST_PP_R2_102(M,D) BOOST_PP_R2_101(M,D) M(101,D)
#define BOOST_PP_R2_103(M,D) BOOST_PP_R2_102(M,D) M(102,D)
#define BOOST_PP_R2_104(M,D) BOOST_PP_R2_103(M,D) M(103,D)
#define BOOST_PP_R2_105(M,D) BOOST_PP_R2_104(M,D) M(104,D)
#define BOOST_PP_R2_106(M,D) BOOST_PP_R2_105(M,D) M(105,D)
#define BOOST_PP_R2_107(M,D) BOOST_PP_R2_106(M,D) M(106,D)
#define BOOST_PP_R2_108(M,D) BOOST_PP_R2_107(M,D) M(107,D)
#define BOOST_PP_R2_109(M,D) BOOST_PP_R2_108(M,D) M(108,D)
#define BOOST_PP_R2_110(M,D) BOOST_PP_R2_109(M,D) M(109,D)
#define BOOST_PP_R2_111(M,D) BOOST_PP_R2_110(M,D) M(110,D)
#define BOOST_PP_R2_112(M,D) BOOST_PP_R2_111(M,D) M(111,D)
#define BOOST_PP_R2_113(M,D) BOOST_PP_R2_112(M,D) M(112,D)
#define BOOST_PP_R2_114(M,D) BOOST_PP_R2_113(M,D) M(113,D)
#define BOOST_PP_R2_115(M,D) BOOST_PP_R2_114(M,D) M(114,D)
#define BOOST_PP_R2_116(M,D) BOOST_PP_R2_115(M,D) M(115,D)
#define BOOST_PP_R2_117(M,D) BOOST_PP_R2_116(M,D) M(116,D)
#define BOOST_PP_R2_118(M,D) BOOST_PP_R2_117(M,D) M(117,D)
#define BOOST_PP_R2_119(M,D) BOOST_PP_R2_118(M,D) M(118,D)
#define BOOST_PP_R2_120(M,D) BOOST_PP_R2_119(M,D) M(119,D)
#define BOOST_PP_R2_121(M,D) BOOST_PP_R2_120(M,D) M(120,D)
#define BOOST_PP_R2_122(M,D) BOOST_PP_R2_121(M,D) M(121,D)
#define BOOST_PP_R2_123(M,D) BOOST_PP_R2_122(M,D) M(122,D)
#define BOOST_PP_R2_124(M,D) BOOST_PP_R2_123(M,D) M(123,D)
#define BOOST_PP_R2_125(M,D) BOOST_PP_R2_124(M,D) M(124,D)
#define BOOST_PP_R2_126(M,D) BOOST_PP_R2_125(M,D) M(125,D)
#define BOOST_PP_R2_127(M,D) BOOST_PP_R2_126(M,D) M(126,D)
#define BOOST_PP_R2_128(M,D) BOOST_PP_R2_127(M,D) M(127,D)

#define BOOST_PP_R3_0(M,D)
#define BOOST_PP_R3_1(M,D) M(0,D)
#define BOOST_PP_R3_2(M,D) M(0,D) M(1,D)
#define BOOST_PP_R3_3(M,D) M(0,D) M(1,D) M(2,D)
#define BOOST_PP_R3_4(M,D) M(0,D) M(1,D) M(2,D) M(3,D)
#define BOOST_PP_R3_5(M,D) BOOST_PP_R3_4(M,D) M(4,D)
#define BOOST_PP_R3_6(M,D) BOOST_PP_R3_5(M,D) M(5,D)
#define BOOST_PP_R3_7(M,D) BOOST_PP_R3_6(M,D) M(6,D)
#define BOOST_PP_R3_8(M,D) BOOST_PP_R3_7(M,D) M(7,D)
#define BOOST_PP_R3_9(M,D) BOOST_PP_R3_8(M,D) M(8,D)
#define BOOST_PP_R3_10(M,D) BOOST_PP_R3_9(M,D) M(9,D)
#define BOOST_PP_R3_11(M,D) BOOST_PP_R3_10(M,D) M(10,D)
#define BOOST_PP_R3_12(M,D) BOOST_PP_R3_11(M,D) M(11,D)
#define BOOST_PP_R3_13(M,D) BOOST_PP_R3_12(M,D) M(12,D)
#define BOOST_PP_R3_14(M,D) BOOST_PP_R3_13(M,D) M(13,D)
#define BOOST_PP_R3_15(M,D) BOOST_PP_R3_14(M,D) M(14,D)
#define BOOST_PP_R3_16(M,D) BOOST_PP_R3_15(M,D) M(15,D)
#define BOOST_PP_R3_17(M,D) BOOST_PP_R3_16(M,D) M(16,D)
#define BOOST_PP_R3_18(M,D) BOOST_PP_R3_17(M,D) M(17,D)
#define BOOST_PP_R3_19(M,D) BOOST_PP_R3_18(M,D) M(18,D)
#define BOOST_PP_R3_20(M,D) BOOST_PP_R3_19(M,D) M(19,D)
#define BOOST_PP_R3_21(M,D) BOOST_PP_R3_20(M,D) M(20,D)
#define BOOST_PP_R3_22(M,D) BOOST_PP_R3_21(M,D) M(21,D)
#define BOOST_PP_R3_23(M,D) BOOST_PP_R3_22(M,D) M(22,D)
#define BOOST_PP_R3_24(M,D) BOOST_PP_R3_23(M,D) M(23,D)
#define BOOST_PP_R3_25(M,D) BOOST_PP_R3_24(M,D) M(24,D)
#define BOOST_PP_R3_26(M,D) BOOST_PP_R3_25(M,D) M(25,D)
#define BOOST_PP_R3_27(M,D) BOOST_PP_R3_26(M,D) M(26,D)
#define BOOST_PP_R3_28(M,D) BOOST_PP_R3_27(M,D) M(27,D)
#define BOOST_PP_R3_29(M,D) BOOST_PP_R3_28(M,D) M(28,D)
#define BOOST_PP_R3_30(M,D) BOOST_PP_R3_29(M,D) M(29,D)
#define BOOST_PP_R3_31(M,D) BOOST_PP_R3_30(M,D) M(30,D)
#define BOOST_PP_R3_32(M,D) BOOST_PP_R3_31(M,D) M(31,D)
#define BOOST_PP_R3_33(M,D) BOOST_PP_R3_32(M,D) M(32,D)
#define BOOST_PP_R3_34(M,D) BOOST_PP_R3_33(M,D) M(33,D)
#define BOOST_PP_R3_35(M,D) BOOST_PP_R3_34(M,D) M(34,D)
#define BOOST_PP_R3_36(M,D) BOOST_PP_R3_35(M,D) M(35,D)
#define BOOST_PP_R3_37(M,D) BOOST_PP_R3_36(M,D) M(36,D)
#define BOOST_PP_R3_38(M,D) BOOST_PP_R3_37(M,D) M(37,D)
#define BOOST_PP_R3_39(M,D) BOOST_PP_R3_38(M,D) M(38,D)
#define BOOST_PP_R3_40(M,D) BOOST_PP_R3_39(M,D) M(39,D)
#define BOOST_PP_R3_41(M,D) BOOST_PP_R3_40(M,D) M(40,D)
#define BOOST_PP_R3_42(M,D) BOOST_PP_R3_41(M,D) M(41,D)
#define BOOST_PP_R3_43(M,D) BOOST_PP_R3_42(M,D) M(42,D)
#define BOOST_PP_R3_44(M,D) BOOST_PP_R3_43(M,D) M(43,D)
#define BOOST_PP_R3_45(M,D) BOOST_PP_R3_44(M,D) M(44,D)
#define BOOST_PP_R3_46(M,D) BOOST_PP_R3_45(M,D) M(45,D)
#define BOOST_PP_R3_47(M,D) BOOST_PP_R3_46(M,D) M(46,D)
#define BOOST_PP_R3_48(M,D) BOOST_PP_R3_47(M,D) M(47,D)
#define BOOST_PP_R3_49(M,D) BOOST_PP_R3_48(M,D) M(48,D)
#define BOOST_PP_R3_50(M,D) BOOST_PP_R3_49(M,D) M(49,D)
#define BOOST_PP_R3_51(M,D) BOOST_PP_R3_50(M,D) M(50,D)
#define BOOST_PP_R3_52(M,D) BOOST_PP_R3_51(M,D) M(51,D)
#define BOOST_PP_R3_53(M,D) BOOST_PP_R3_52(M,D) M(52,D)
#define BOOST_PP_R3_54(M,D) BOOST_PP_R3_53(M,D) M(53,D)
#define BOOST_PP_R3_55(M,D) BOOST_PP_R3_54(M,D) M(54,D)
#define BOOST_PP_R3_56(M,D) BOOST_PP_R3_55(M,D) M(55,D)
#define BOOST_PP_R3_57(M,D) BOOST_PP_R3_56(M,D) M(56,D)
#define BOOST_PP_R3_58(M,D) BOOST_PP_R3_57(M,D) M(57,D)
#define BOOST_PP_R3_59(M,D) BOOST_PP_R3_58(M,D) M(58,D)
#define BOOST_PP_R3_60(M,D) BOOST_PP_R3_59(M,D) M(59,D)
#define BOOST_PP_R3_61(M,D) BOOST_PP_R3_60(M,D) M(60,D)
#define BOOST_PP_R3_62(M,D) BOOST_PP_R3_61(M,D) M(61,D)
#define BOOST_PP_R3_63(M,D) BOOST_PP_R3_62(M,D) M(62,D)
#define BOOST_PP_R3_64(M,D) BOOST_PP_R3_63(M,D) M(63,D)
#define BOOST_PP_R3_65(M,D) BOOST_PP_R3_64(M,D) M(64,D)
#define BOOST_PP_R3_66(M,D) BOOST_PP_R3_65(M,D) M(65,D)
#define BOOST_PP_R3_67(M,D) BOOST_PP_R3_66(M,D) M(66,D)
#define BOOST_PP_R3_68(M,D) BOOST_PP_R3_67(M,D) M(67,D)
#define BOOST_PP_R3_69(M,D) BOOST_PP_R3_68(M,D) M(68,D)
#define BOOST_PP_R3_70(M,D) BOOST_PP_R3_69(M,D) M(69,D)
#define BOOST_PP_R3_71(M,D) BOOST_PP_R3_70(M,D) M(70,D)
#define BOOST_PP_R3_72(M,D) BOOST_PP_R3_71(M,D) M(71,D)
#define BOOST_PP_R3_73(M,D) BOOST_PP_R3_72(M,D) M(72,D)
#define BOOST_PP_R3_74(M,D) BOOST_PP_R3_73(M,D) M(73,D)
#define BOOST_PP_R3_75(M,D) BOOST_PP_R3_74(M,D) M(74,D)
#define BOOST_PP_R3_76(M,D) BOOST_PP_R3_75(M,D) M(75,D)
#define BOOST_PP_R3_77(M,D) BOOST_PP_R3_76(M,D) M(76,D)
#define BOOST_PP_R3_78(M,D) BOOST_PP_R3_77(M,D) M(77,D)
#define BOOST_PP_R3_79(M,D) BOOST_PP_R3_78(M,D) M(78,D)
#define BOOST_PP_R3_80(M,D) BOOST_PP_R3_79(M,D) M(79,D)
#define BOOST_PP_R3_81(M,D) BOOST_PP_R3_80(M,D) M(80,D)
#define BOOST_PP_R3_82(M,D) BOOST_PP_R3_81(M,D) M(81,D)
#define BOOST_PP_R3_83(M,D) BOOST_PP_R3_82(M,D) M(82,D)
#define BOOST_PP_R3_84(M,D) BOOST_PP_R3_83(M,D) M(83,D)
#define BOOST_PP_R3_85(M,D) BOOST_PP_R3_84(M,D) M(84,D)
#define BOOST_PP_R3_86(M,D) BOOST_PP_R3_85(M,D) M(85,D)
#define BOOST_PP_R3_87(M,D) BOOST_PP_R3_86(M,D) M(86,D)
#define BOOST_PP_R3_88(M,D) BOOST_PP_R3_87(M,D) M(87,D)
#define BOOST_PP_R3_89(M,D) BOOST_PP_R3_88(M,D) M(88,D)
#define BOOST_PP_R3_90(M,D) BOOST_PP_R3_89(M,D) M(89,D)
#define BOOST_PP_R3_91(M,D) BOOST_PP_R3_90(M,D) M(90,D)
#define BOOST_PP_R3_92(M,D) BOOST_PP_R3_91(M,D) M(91,D)
#define BOOST_PP_R3_93(M,D) BOOST_PP_R3_92(M,D) M(92,D)
#define BOOST_PP_R3_94(M,D) BOOST_PP_R3_93(M,D) M(93,D)
#define BOOST_PP_R3_95(M,D) BOOST_PP_R3_94(M,D) M(94,D)
#define BOOST_PP_R3_96(M,D) BOOST_PP_R3_95(M,D) M(95,D)
#define BOOST_PP_R3_97(M,D) BOOST_PP_R3_96(M,D) M(96,D)
#define BOOST_PP_R3_98(M,D) BOOST_PP_R3_97(M,D) M(97,D)
#define BOOST_PP_R3_99(M,D) BOOST_PP_R3_98(M,D) M(98,D)
#define BOOST_PP_R3_100(M,D) BOOST_PP_R3_99(M,D) M(99,D)
#define BOOST_PP_R3_101(M,D) BOOST_PP_R3_100(M,D) M(100,D)
#define BOOST_PP_R3_102(M,D) BOOST_PP_R3_101(M,D) M(101,D)
#define BOOST_PP_R3_103(M,D) BOOST_PP_R3_102(M,D) M(102,D)
#define BOOST_PP_R3_104(M,D) BOOST_PP_R3_103(M,D) M(103,D)
#define BOOST_PP_R3_105(M,D) BOOST_PP_R3_104(M,D) M(104,D)
#define BOOST_PP_R3_106(M,D) BOOST_PP_R3_105(M,D) M(105,D)
#define BOOST_PP_R3_107(M,D) BOOST_PP_R3_106(M,D) M(106,D)
#define BOOST_PP_R3_108(M,D) BOOST_PP_R3_107(M,D) M(107,D)
#define BOOST_PP_R3_109(M,D) BOOST_PP_R3_108(M,D) M(108,D)
#define BOOST_PP_R3_110(M,D) BOOST_PP_R3_109(M,D) M(109,D)
#define BOOST_PP_R3_111(M,D) BOOST_PP_R3_110(M,D) M(110,D)
#define BOOST_PP_R3_112(M,D) BOOST_PP_R3_111(M,D) M(111,D)
#define BOOST_PP_R3_113(M,D) BOOST_PP_R3_112(M,D) M(112,D)
#define BOOST_PP_R3_114(M,D) BOOST_PP_R3_113(M,D) M(113,D)
#define BOOST_PP_R3_115(M,D) BOOST_PP_R3_114(M,D) M(114,D)
#define BOOST_PP_R3_116(M,D) BOOST_PP_R3_115(M,D) M(115,D)
#define BOOST_PP_R3_117(M,D) BOOST_PP_R3_116(M,D) M(116,D)
#define BOOST_PP_R3_118(M,D) BOOST_PP_R3_117(M,D) M(117,D)
#define BOOST_PP_R3_119(M,D) BOOST_PP_R3_118(M,D) M(118,D)
#define BOOST_PP_R3_120(M,D) BOOST_PP_R3_119(M,D) M(119,D)
#define BOOST_PP_R3_121(M,D) BOOST_PP_R3_120(M,D) M(120,D)
#define BOOST_PP_R3_122(M,D) BOOST_PP_R3_121(M,D) M(121,D)
#define BOOST_PP_R3_123(M,D) BOOST_PP_R3_122(M,D) M(122,D)
#define BOOST_PP_R3_124(M,D) BOOST_PP_R3_123(M,D) M(123,D)
#define BOOST_PP_R3_125(M,D) BOOST_PP_R3_124(M,D) M(124,D)
#define BOOST_PP_R3_126(M,D) BOOST_PP_R3_125(M,D) M(125,D)
#define BOOST_PP_R3_127(M,D) BOOST_PP_R3_126(M,D) M(126,D)
#define BOOST_PP_R3_128(M,D) BOOST_PP_R3_127(M,D) M(127,D)

#endif // !__EDG__

/** <p>Obsolete, just use BOOST_PP_REPEAT().</p> */
#define BOOST_PP_REPEAT_2ND BOOST_PP_REPEAT
/** <p>Obsolete, just use BOOST_PP_REPEAT().</p> */
#define BOOST_PP_REPEAT_3RD BOOST_PP_REPEAT
#endif
