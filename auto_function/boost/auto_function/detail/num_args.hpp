/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_AUTO_FUNCTION_DETAIL_NUM_ARGS_HPP
#define BOOST_AUTO_FUNCTION_DETAIL_NUM_ARGS_HPP

// Implementation based on macros by Laurent Deniau

// Yields the number of arguments passed to a variadic macro
#define BOOST_AUTO_FUNCTION_DETAIL_NUM_ARGS( ... )                             \
BOOST_AUTO_FUNCTION_DETAIL_NUM_ARGS_IMPL                                       \
( __VA_ARGS__                                                                  \
, BOOST_AUTO_FUNCTION_DETAIL_DECENDING_VALUES()                                \
)

#define BOOST_AUTO_FUNCTION_DETAIL_NUM_WRAPPED_ARGS( args )                    \
BOOST_AUTO_FUNCTION_DETAIL_NUM_ARGS args

#define BOOST_AUTO_FUNCTION_DETAIL_NUM_ARGS_AFTER_RESCAN( ... )                \
BOOST_AUTO_FUNCTION_DETAIL_NUM_ARGS( __VA_ARGS__ )

#define BOOST_AUTO_FUNCTION_DETAIL_NUM_ARGS_IMPL( ... )                        \
BOOST_AUTO_FUNCTION_DETAIL_NUM_ARGS_IMPL_N( __VA_ARGS__ )

#define BOOST_AUTO_FUNCTION_DETAIL_NUM_ARGS_IMPL_N(                            \
 _00,_01,_02,_03,_04,_05,_06,_07,_08,_09,_0a,_0b,_0c,_0d,_0e,_0f               \
,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_1a,_1b,_1c,_1d,_1e,_1f               \
,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_2a,_2b,_2c,_2d,_2e,_2f               \
,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_3a,_3b,_3c,_3d,_3e,_3f               \
,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_4a,_4b,_4c,_4d,_4e,_4f               \
,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_5a,_5b,_5c,_5d,_5e,_5f               \
,_60,_61,_62,_63,_64,_65,_66,_67,_68,_69,_6a,_6b,_6c,_6d,_6e,_6f               \
,_70,_71,_72,_73,_74,_75,_76,_77,_78,_79,_7a,_7b,_7c,_7d,_7e,_7f               \
,_80,_81,_82,_83,_84,_85,_86,_87,_88,_89,_8a,_8b,_8c,_8d,_8e,_8f               \
,_90,_91,_92,_93,_94,_95,_96,_97,_98,_99,_9a,_9b,_9c,_9d,_9e,_9f               \
,_a0,_a1,_a2,_a3,_a4,_a5,_a6,_a7,_a8,_a9,_aa,_ab,_ac,_ad,_ae,_af               \
,_b0,_b1,_b2,_b3,_b4,_b5,_b6,_b7,_b8,_b9,_ba,_bb,_bc,_bd,_be,_bf               \
,_c0,_c1,_c2,_c3,_c4,_c5,_c6,_c7,_c8,_c9,_ca,_cb,_cc,_cd,_ce,_cf               \
,_d0,_d1,_d2,_d3,_d4,_d5,_d6,_d7,_d8,_d9,_da,_db,_dc,_dd,_de,_df               \
,_e0,_e1,_e2,_e3,_e4,_e5,_e6,_e7,_e8,_e9,_ea,_eb,_ec,_ed,_ee,_ef               \
,_f0,_f1,_f2,_f3,_f4,_f5,_f6,_f7,_f8,_f9,_fa,_fb,_fc,_fd,_fe,_ff               \
,N,...) N

#define BOOST_AUTO_FUNCTION_DETAIL_DECENDING_VALUES()                          \
 256,255,254,253,252,251,250,249,248,247,246,245,244,243,242,241,240,239,238   \
,237,236,235,234,233,232,231,230,229,228,227,226,225,224,223,222,221,220,219   \
,218,217,216,215,214,213,212,211,210,209,208,207,206,205,204,203,202,201,200   \
,199,198,197,196,195,194,193,192,191,190,189,188,187,186,185,184,183,182,181   \
,180,179,178,177,176,175,174,173,172,171,170,169,168,167,166,165,164,163,162   \
,161,160,159,158,157,156,155,154,153,152,151,150,149,148,147,146,145,144,143   \
,142,141,140,139,138,137,136,135,134,133,132,131,130,129,128,127,126,125,124   \
,123,122,121,120,119,118,117,116,115,114,113,112,111,110,109,108,107,106,105   \
,104,103,102,101,100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86   \
, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67   \
, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48   \
, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29   \
, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10   \
,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0

#endif // BOOST_AUTO_FUNCTION_DETAIL_NUM_ARGS_HPP
