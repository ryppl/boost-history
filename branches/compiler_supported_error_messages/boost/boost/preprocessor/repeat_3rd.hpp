#ifndef BOOST_PREPROCESSOR_REPEAT_3RD_HPP
#define BOOST_PREPROCESSOR_REPEAT_3RD_HPP

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

/** <p>Same as BOOST_PP_REPEAT(), but implemented independently.</p> */
#define BOOST_PP_REPEAT_3RD(N,M,P) BOOST_PP_REPEAT_3RD_DELAY(N,M,P)

#define BOOST_PP_REPEAT_3RD_DELAY(N,M,P) BOOST_PP_REPEAT_3RD##N(M,P)
#define BOOST_PP_REPEAT_3RD0(M,P)
#define BOOST_PP_REPEAT_3RD1(M,P) M(0,P)
#define BOOST_PP_REPEAT_3RD2(M,P) M(0,P) M(1,P)
#define BOOST_PP_REPEAT_3RD3(M,P) M(0,P) M(1,P) M(2,P)
#define BOOST_PP_REPEAT_3RD4(M,P) M(0,P) M(1,P) M(2,P) M(3,P)
#define BOOST_PP_REPEAT_3RD5(M,P) BOOST_PP_REPEAT_3RD4(M,P) M(4,P)
#define BOOST_PP_REPEAT_3RD6(M,P) BOOST_PP_REPEAT_3RD5(M,P) M(5,P)
#define BOOST_PP_REPEAT_3RD7(M,P) BOOST_PP_REPEAT_3RD6(M,P) M(6,P)
#define BOOST_PP_REPEAT_3RD8(M,P) BOOST_PP_REPEAT_3RD7(M,P) M(7,P)
#define BOOST_PP_REPEAT_3RD9(M,P) BOOST_PP_REPEAT_3RD8(M,P) M(8,P)
#define BOOST_PP_REPEAT_3RD10(M,P) BOOST_PP_REPEAT_3RD9(M,P) M(9,P)
#define BOOST_PP_REPEAT_3RD11(M,P) BOOST_PP_REPEAT_3RD10(M,P) M(10,P)
#define BOOST_PP_REPEAT_3RD12(M,P) BOOST_PP_REPEAT_3RD11(M,P) M(11,P)
#define BOOST_PP_REPEAT_3RD13(M,P) BOOST_PP_REPEAT_3RD12(M,P) M(12,P)
#define BOOST_PP_REPEAT_3RD14(M,P) BOOST_PP_REPEAT_3RD13(M,P) M(13,P)
#define BOOST_PP_REPEAT_3RD15(M,P) BOOST_PP_REPEAT_3RD14(M,P) M(14,P)
#define BOOST_PP_REPEAT_3RD16(M,P) BOOST_PP_REPEAT_3RD15(M,P) M(15,P)
#define BOOST_PP_REPEAT_3RD17(M,P) BOOST_PP_REPEAT_3RD16(M,P) M(16,P)
#define BOOST_PP_REPEAT_3RD18(M,P) BOOST_PP_REPEAT_3RD17(M,P) M(17,P)
#define BOOST_PP_REPEAT_3RD19(M,P) BOOST_PP_REPEAT_3RD18(M,P) M(18,P)
#define BOOST_PP_REPEAT_3RD20(M,P) BOOST_PP_REPEAT_3RD19(M,P) M(19,P)
#define BOOST_PP_REPEAT_3RD21(M,P) BOOST_PP_REPEAT_3RD20(M,P) M(20,P)
#define BOOST_PP_REPEAT_3RD22(M,P) BOOST_PP_REPEAT_3RD21(M,P) M(21,P)
#define BOOST_PP_REPEAT_3RD23(M,P) BOOST_PP_REPEAT_3RD22(M,P) M(22,P)
#define BOOST_PP_REPEAT_3RD24(M,P) BOOST_PP_REPEAT_3RD23(M,P) M(23,P)
#define BOOST_PP_REPEAT_3RD25(M,P) BOOST_PP_REPEAT_3RD24(M,P) M(24,P)
#define BOOST_PP_REPEAT_3RD26(M,P) BOOST_PP_REPEAT_3RD25(M,P) M(25,P)
#define BOOST_PP_REPEAT_3RD27(M,P) BOOST_PP_REPEAT_3RD26(M,P) M(26,P)
#define BOOST_PP_REPEAT_3RD28(M,P) BOOST_PP_REPEAT_3RD27(M,P) M(27,P)
#define BOOST_PP_REPEAT_3RD29(M,P) BOOST_PP_REPEAT_3RD28(M,P) M(28,P)
#define BOOST_PP_REPEAT_3RD30(M,P) BOOST_PP_REPEAT_3RD29(M,P) M(29,P)
#define BOOST_PP_REPEAT_3RD31(M,P) BOOST_PP_REPEAT_3RD30(M,P) M(30,P)
#define BOOST_PP_REPEAT_3RD32(M,P) BOOST_PP_REPEAT_3RD31(M,P) M(31,P)
#define BOOST_PP_REPEAT_3RD33(M,P) BOOST_PP_REPEAT_3RD32(M,P) M(32,P)
#define BOOST_PP_REPEAT_3RD34(M,P) BOOST_PP_REPEAT_3RD33(M,P) M(33,P)
#define BOOST_PP_REPEAT_3RD35(M,P) BOOST_PP_REPEAT_3RD34(M,P) M(34,P)
#define BOOST_PP_REPEAT_3RD36(M,P) BOOST_PP_REPEAT_3RD35(M,P) M(35,P)
#define BOOST_PP_REPEAT_3RD37(M,P) BOOST_PP_REPEAT_3RD36(M,P) M(36,P)
#define BOOST_PP_REPEAT_3RD38(M,P) BOOST_PP_REPEAT_3RD37(M,P) M(37,P)
#define BOOST_PP_REPEAT_3RD39(M,P) BOOST_PP_REPEAT_3RD38(M,P) M(38,P)
#define BOOST_PP_REPEAT_3RD40(M,P) BOOST_PP_REPEAT_3RD39(M,P) M(39,P)
#define BOOST_PP_REPEAT_3RD41(M,P) BOOST_PP_REPEAT_3RD40(M,P) M(40,P)
#define BOOST_PP_REPEAT_3RD42(M,P) BOOST_PP_REPEAT_3RD41(M,P) M(41,P)
#define BOOST_PP_REPEAT_3RD43(M,P) BOOST_PP_REPEAT_3RD42(M,P) M(42,P)
#define BOOST_PP_REPEAT_3RD44(M,P) BOOST_PP_REPEAT_3RD43(M,P) M(43,P)
#define BOOST_PP_REPEAT_3RD45(M,P) BOOST_PP_REPEAT_3RD44(M,P) M(44,P)
#define BOOST_PP_REPEAT_3RD46(M,P) BOOST_PP_REPEAT_3RD45(M,P) M(45,P)
#define BOOST_PP_REPEAT_3RD47(M,P) BOOST_PP_REPEAT_3RD46(M,P) M(46,P)
#define BOOST_PP_REPEAT_3RD48(M,P) BOOST_PP_REPEAT_3RD47(M,P) M(47,P)
#define BOOST_PP_REPEAT_3RD49(M,P) BOOST_PP_REPEAT_3RD48(M,P) M(48,P)
#define BOOST_PP_REPEAT_3RD50(M,P) BOOST_PP_REPEAT_3RD49(M,P) M(49,P)
#define BOOST_PP_REPEAT_3RD51(M,P) BOOST_PP_REPEAT_3RD50(M,P) M(50,P)
#define BOOST_PP_REPEAT_3RD52(M,P) BOOST_PP_REPEAT_3RD51(M,P) M(51,P)
#define BOOST_PP_REPEAT_3RD53(M,P) BOOST_PP_REPEAT_3RD52(M,P) M(52,P)
#define BOOST_PP_REPEAT_3RD54(M,P) BOOST_PP_REPEAT_3RD53(M,P) M(53,P)
#define BOOST_PP_REPEAT_3RD55(M,P) BOOST_PP_REPEAT_3RD54(M,P) M(54,P)
#define BOOST_PP_REPEAT_3RD56(M,P) BOOST_PP_REPEAT_3RD55(M,P) M(55,P)
#define BOOST_PP_REPEAT_3RD57(M,P) BOOST_PP_REPEAT_3RD56(M,P) M(56,P)
#define BOOST_PP_REPEAT_3RD58(M,P) BOOST_PP_REPEAT_3RD57(M,P) M(57,P)
#define BOOST_PP_REPEAT_3RD59(M,P) BOOST_PP_REPEAT_3RD58(M,P) M(58,P)
#define BOOST_PP_REPEAT_3RD60(M,P) BOOST_PP_REPEAT_3RD59(M,P) M(59,P)
#define BOOST_PP_REPEAT_3RD61(M,P) BOOST_PP_REPEAT_3RD60(M,P) M(60,P)
#define BOOST_PP_REPEAT_3RD62(M,P) BOOST_PP_REPEAT_3RD61(M,P) M(61,P)
#define BOOST_PP_REPEAT_3RD63(M,P) BOOST_PP_REPEAT_3RD62(M,P) M(62,P)
#define BOOST_PP_REPEAT_3RD64(M,P) BOOST_PP_REPEAT_3RD63(M,P) M(63,P)
#define BOOST_PP_REPEAT_3RD65(M,P) BOOST_PP_REPEAT_3RD64(M,P) M(64,P)
#define BOOST_PP_REPEAT_3RD66(M,P) BOOST_PP_REPEAT_3RD65(M,P) M(65,P)
#define BOOST_PP_REPEAT_3RD67(M,P) BOOST_PP_REPEAT_3RD66(M,P) M(66,P)
#define BOOST_PP_REPEAT_3RD68(M,P) BOOST_PP_REPEAT_3RD67(M,P) M(67,P)
#define BOOST_PP_REPEAT_3RD69(M,P) BOOST_PP_REPEAT_3RD68(M,P) M(68,P)
#define BOOST_PP_REPEAT_3RD70(M,P) BOOST_PP_REPEAT_3RD69(M,P) M(69,P)
#define BOOST_PP_REPEAT_3RD71(M,P) BOOST_PP_REPEAT_3RD70(M,P) M(70,P)
#define BOOST_PP_REPEAT_3RD72(M,P) BOOST_PP_REPEAT_3RD71(M,P) M(71,P)
#define BOOST_PP_REPEAT_3RD73(M,P) BOOST_PP_REPEAT_3RD72(M,P) M(72,P)
#define BOOST_PP_REPEAT_3RD74(M,P) BOOST_PP_REPEAT_3RD73(M,P) M(73,P)
#define BOOST_PP_REPEAT_3RD75(M,P) BOOST_PP_REPEAT_3RD74(M,P) M(74,P)
#define BOOST_PP_REPEAT_3RD76(M,P) BOOST_PP_REPEAT_3RD75(M,P) M(75,P)
#define BOOST_PP_REPEAT_3RD77(M,P) BOOST_PP_REPEAT_3RD76(M,P) M(76,P)
#define BOOST_PP_REPEAT_3RD78(M,P) BOOST_PP_REPEAT_3RD77(M,P) M(77,P)
#define BOOST_PP_REPEAT_3RD79(M,P) BOOST_PP_REPEAT_3RD78(M,P) M(78,P)
#define BOOST_PP_REPEAT_3RD80(M,P) BOOST_PP_REPEAT_3RD79(M,P) M(79,P)
#define BOOST_PP_REPEAT_3RD81(M,P) BOOST_PP_REPEAT_3RD80(M,P) M(80,P)
#define BOOST_PP_REPEAT_3RD82(M,P) BOOST_PP_REPEAT_3RD81(M,P) M(81,P)
#define BOOST_PP_REPEAT_3RD83(M,P) BOOST_PP_REPEAT_3RD82(M,P) M(82,P)
#define BOOST_PP_REPEAT_3RD84(M,P) BOOST_PP_REPEAT_3RD83(M,P) M(83,P)
#define BOOST_PP_REPEAT_3RD85(M,P) BOOST_PP_REPEAT_3RD84(M,P) M(84,P)
#define BOOST_PP_REPEAT_3RD86(M,P) BOOST_PP_REPEAT_3RD85(M,P) M(85,P)
#define BOOST_PP_REPEAT_3RD87(M,P) BOOST_PP_REPEAT_3RD86(M,P) M(86,P)
#define BOOST_PP_REPEAT_3RD88(M,P) BOOST_PP_REPEAT_3RD87(M,P) M(87,P)
#define BOOST_PP_REPEAT_3RD89(M,P) BOOST_PP_REPEAT_3RD88(M,P) M(88,P)
#define BOOST_PP_REPEAT_3RD90(M,P) BOOST_PP_REPEAT_3RD89(M,P) M(89,P)
#define BOOST_PP_REPEAT_3RD91(M,P) BOOST_PP_REPEAT_3RD90(M,P) M(90,P)
#define BOOST_PP_REPEAT_3RD92(M,P) BOOST_PP_REPEAT_3RD91(M,P) M(91,P)
#define BOOST_PP_REPEAT_3RD93(M,P) BOOST_PP_REPEAT_3RD92(M,P) M(92,P)
#define BOOST_PP_REPEAT_3RD94(M,P) BOOST_PP_REPEAT_3RD93(M,P) M(93,P)
#define BOOST_PP_REPEAT_3RD95(M,P) BOOST_PP_REPEAT_3RD94(M,P) M(94,P)
#define BOOST_PP_REPEAT_3RD96(M,P) BOOST_PP_REPEAT_3RD95(M,P) M(95,P)
#define BOOST_PP_REPEAT_3RD97(M,P) BOOST_PP_REPEAT_3RD96(M,P) M(96,P)
#define BOOST_PP_REPEAT_3RD98(M,P) BOOST_PP_REPEAT_3RD97(M,P) M(97,P)
#define BOOST_PP_REPEAT_3RD99(M,P) BOOST_PP_REPEAT_3RD98(M,P) M(98,P)
#define BOOST_PP_REPEAT_3RD100(M,P) BOOST_PP_REPEAT_3RD99(M,P) M(99,P)
#define BOOST_PP_REPEAT_3RD101(M,P) BOOST_PP_REPEAT_3RD100(M,P) M(100,P)
#define BOOST_PP_REPEAT_3RD102(M,P) BOOST_PP_REPEAT_3RD101(M,P) M(101,P)
#define BOOST_PP_REPEAT_3RD103(M,P) BOOST_PP_REPEAT_3RD102(M,P) M(102,P)
#define BOOST_PP_REPEAT_3RD104(M,P) BOOST_PP_REPEAT_3RD103(M,P) M(103,P)
#define BOOST_PP_REPEAT_3RD105(M,P) BOOST_PP_REPEAT_3RD104(M,P) M(104,P)
#define BOOST_PP_REPEAT_3RD106(M,P) BOOST_PP_REPEAT_3RD105(M,P) M(105,P)
#define BOOST_PP_REPEAT_3RD107(M,P) BOOST_PP_REPEAT_3RD106(M,P) M(106,P)
#define BOOST_PP_REPEAT_3RD108(M,P) BOOST_PP_REPEAT_3RD107(M,P) M(107,P)
#define BOOST_PP_REPEAT_3RD109(M,P) BOOST_PP_REPEAT_3RD108(M,P) M(108,P)
#define BOOST_PP_REPEAT_3RD110(M,P) BOOST_PP_REPEAT_3RD109(M,P) M(109,P)
#define BOOST_PP_REPEAT_3RD111(M,P) BOOST_PP_REPEAT_3RD110(M,P) M(110,P)
#define BOOST_PP_REPEAT_3RD112(M,P) BOOST_PP_REPEAT_3RD111(M,P) M(111,P)
#define BOOST_PP_REPEAT_3RD113(M,P) BOOST_PP_REPEAT_3RD112(M,P) M(112,P)
#define BOOST_PP_REPEAT_3RD114(M,P) BOOST_PP_REPEAT_3RD113(M,P) M(113,P)
#define BOOST_PP_REPEAT_3RD115(M,P) BOOST_PP_REPEAT_3RD114(M,P) M(114,P)
#define BOOST_PP_REPEAT_3RD116(M,P) BOOST_PP_REPEAT_3RD115(M,P) M(115,P)
#define BOOST_PP_REPEAT_3RD117(M,P) BOOST_PP_REPEAT_3RD116(M,P) M(116,P)
#define BOOST_PP_REPEAT_3RD118(M,P) BOOST_PP_REPEAT_3RD117(M,P) M(117,P)
#define BOOST_PP_REPEAT_3RD119(M,P) BOOST_PP_REPEAT_3RD118(M,P) M(118,P)
#define BOOST_PP_REPEAT_3RD120(M,P) BOOST_PP_REPEAT_3RD119(M,P) M(119,P)
#define BOOST_PP_REPEAT_3RD121(M,P) BOOST_PP_REPEAT_3RD120(M,P) M(120,P)
#define BOOST_PP_REPEAT_3RD122(M,P) BOOST_PP_REPEAT_3RD121(M,P) M(121,P)
#define BOOST_PP_REPEAT_3RD123(M,P) BOOST_PP_REPEAT_3RD122(M,P) M(122,P)
#define BOOST_PP_REPEAT_3RD124(M,P) BOOST_PP_REPEAT_3RD123(M,P) M(123,P)
#define BOOST_PP_REPEAT_3RD125(M,P) BOOST_PP_REPEAT_3RD124(M,P) M(124,P)
#define BOOST_PP_REPEAT_3RD126(M,P) BOOST_PP_REPEAT_3RD125(M,P) M(125,P)
#define BOOST_PP_REPEAT_3RD127(M,P) BOOST_PP_REPEAT_3RD126(M,P) M(126,P)
#define BOOST_PP_REPEAT_3RD128(M,P) BOOST_PP_REPEAT_3RD127(M,P) M(127,P)

/** <p>Obsolete. Use BOOST_PP_REPEAT_3RD().</p> */
#define BOOST_PREPROCESSOR_REPEAT_3RD(N,M,P) BOOST_PP_REPEAT_3RD(N,M,P)
#endif
