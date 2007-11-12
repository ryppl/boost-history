/* Copyright 2006-2007 Joaquín M López Muñoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/flyweight for library home page.
 */

#ifndef BOOST_FLYWEIGHT_DETAIL_PP_TEMPLATE_CTORS_DEF_HPP
#define BOOST_FLYWEIGHT_DETAIL_PP_TEMPLATE_CTORS_DEF_HPP

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#if BOOST_FLYWEIGHT_LIMIT_NUM_CTOR_ARGS>=1
#define BOOST_FLYWEIGHT_CTORS_1 \
template<typename T0> explicit flyweight(T0& t0):h(core::insert(T(t0))){}\
template<typename T0> explicit flyweight(const T0& t0):h(core::insert(T(t0))){}
#endif

#if BOOST_FLYWEIGHT_LIMIT_NUM_CTOR_ARGS>=2
#define BOOST_FLYWEIGHT_CTORS_2 \
template<typename T0,typename T1> explicit flyweight(T0& t0,T1& t1):h(core::insert(T(t0,t1))){}\
template<typename T0,typename T1> explicit flyweight(T0& t0,const T1& t1):h(core::insert(T(t0,t1))){}\
template<typename T0,typename T1> explicit flyweight(const T0& t0,T1& t1):h(core::insert(T(t0,t1))){}\
template<typename T0,typename T1> explicit flyweight(const T0& t0,const T1& t1):h(core::insert(T(t0,t1))){}
#endif

#if BOOST_FLYWEIGHT_LIMIT_NUM_CTOR_ARGS>=3
#define BOOST_FLYWEIGHT_CTORS_3 \
template<typename T0,typename T1,typename T2> explicit flyweight(T0& t0,T1& t1,T2& t2):h(core::insert(T(t0,t1,t2))){}\
template<typename T0,typename T1,typename T2> explicit flyweight(T0& t0,T1& t1,const T2& t2):h(core::insert(T(t0,t1,t2))){}\
template<typename T0,typename T1,typename T2> explicit flyweight(T0& t0,const T1& t1,T2& t2):h(core::insert(T(t0,t1,t2))){}\
template<typename T0,typename T1,typename T2> explicit flyweight(T0& t0,const T1& t1,const T2& t2):h(core::insert(T(t0,t1,t2))){}\
template<typename T0,typename T1,typename T2> explicit flyweight(const T0& t0,T1& t1,T2& t2):h(core::insert(T(t0,t1,t2))){}\
template<typename T0,typename T1,typename T2> explicit flyweight(const T0& t0,T1& t1,const T2& t2):h(core::insert(T(t0,t1,t2))){}\
template<typename T0,typename T1,typename T2> explicit flyweight(const T0& t0,const T1& t1,T2& t2):h(core::insert(T(t0,t1,t2))){}\
template<typename T0,typename T1,typename T2> explicit flyweight(const T0& t0,const T1& t1,const T2& t2):h(core::insert(T(t0,t1,t2))){}
#endif

#if BOOST_FLYWEIGHT_LIMIT_NUM_CTOR_ARGS>=4
#define BOOST_FLYWEIGHT_CTORS_4 \
template<typename T0,typename T1,typename T2,typename T3> explicit flyweight(T0& t0,T1& t1,T2& t2,T3& t3):h(core::insert(T(t0,t1,t2,t3))){}\
template<typename T0,typename T1,typename T2,typename T3> explicit flyweight(T0& t0,T1& t1,T2& t2,const T3& t3):h(core::insert(T(t0,t1,t2,t3))){}\
template<typename T0,typename T1,typename T2,typename T3> explicit flyweight(T0& t0,T1& t1,const T2& t2,T3& t3):h(core::insert(T(t0,t1,t2,t3))){}\
template<typename T0,typename T1,typename T2,typename T3> explicit flyweight(T0& t0,T1& t1,const T2& t2,const T3& t3):h(core::insert(T(t0,t1,t2,t3))){}\
template<typename T0,typename T1,typename T2,typename T3> explicit flyweight(T0& t0,const T1& t1,T2& t2,T3& t3):h(core::insert(T(t0,t1,t2,t3))){}\
template<typename T0,typename T1,typename T2,typename T3> explicit flyweight(T0& t0,const T1& t1,T2& t2,const T3& t3):h(core::insert(T(t0,t1,t2,t3))){}\
template<typename T0,typename T1,typename T2,typename T3> explicit flyweight(T0& t0,const T1& t1,const T2& t2,T3& t3):h(core::insert(T(t0,t1,t2,t3))){}\
template<typename T0,typename T1,typename T2,typename T3> explicit flyweight(T0& t0,const T1& t1,const T2& t2,const T3& t3):h(core::insert(T(t0,t1,t2,t3))){}\
template<typename T0,typename T1,typename T2,typename T3> explicit flyweight(const T0& t0,T1& t1,T2& t2,T3& t3):h(core::insert(T(t0,t1,t2,t3))){}\
template<typename T0,typename T1,typename T2,typename T3> explicit flyweight(const T0& t0,T1& t1,T2& t2,const T3& t3):h(core::insert(T(t0,t1,t2,t3))){}\
template<typename T0,typename T1,typename T2,typename T3> explicit flyweight(const T0& t0,T1& t1,const T2& t2,T3& t3):h(core::insert(T(t0,t1,t2,t3))){}\
template<typename T0,typename T1,typename T2,typename T3> explicit flyweight(const T0& t0,T1& t1,const T2& t2,const T3& t3):h(core::insert(T(t0,t1,t2,t3))){}\
template<typename T0,typename T1,typename T2,typename T3> explicit flyweight(const T0& t0,const T1& t1,T2& t2,T3& t3):h(core::insert(T(t0,t1,t2,t3))){}\
template<typename T0,typename T1,typename T2,typename T3> explicit flyweight(const T0& t0,const T1& t1,T2& t2,const T3& t3):h(core::insert(T(t0,t1,t2,t3))){}\
template<typename T0,typename T1,typename T2,typename T3> explicit flyweight(const T0& t0,const T1& t1,const T2& t2,T3& t3):h(core::insert(T(t0,t1,t2,t3))){}\
template<typename T0,typename T1,typename T2,typename T3> explicit flyweight(const T0& t0,const T1& t1,const T2& t2,const T3& t3):h(core::insert(T(t0,t1,t2,t3))){}
#endif

#if BOOST_FLYWEIGHT_LIMIT_NUM_CTOR_ARGS>=5
#define BOOST_FLYWEIGHT_CTORS_5 \
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(T0& t0,T1& t1,T2& t2,T3& t3,T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(T0& t0,T1& t1,T2& t2,T3& t3,const T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(T0& t0,T1& t1,T2& t2,const T3& t3,T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(T0& t0,T1& t1,T2& t2,const T3& t3,const T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(T0& t0,T1& t1,const T2& t2,T3& t3,T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(T0& t0,T1& t1,const T2& t2,T3& t3,const T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(T0& t0,T1& t1,const T2& t2,const T3& t3,T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(T0& t0,T1& t1,const T2& t2,const T3& t3,const T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(T0& t0,const T1& t1,T2& t2,T3& t3,T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(T0& t0,const T1& t1,T2& t2,T3& t3,const T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(T0& t0,const T1& t1,T2& t2,const T3& t3,T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(T0& t0,const T1& t1,T2& t2,const T3& t3,const T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(T0& t0,const T1& t1,const T2& t2,T3& t3,T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(T0& t0,const T1& t1,const T2& t2,T3& t3,const T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(T0& t0,const T1& t1,const T2& t2,const T3& t3,T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(T0& t0,const T1& t1,const T2& t2,const T3& t3,const T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(const T0& t0,T1& t1,T2& t2,T3& t3,T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(const T0& t0,T1& t1,T2& t2,T3& t3,const T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(const T0& t0,T1& t1,T2& t2,const T3& t3,T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(const T0& t0,T1& t1,T2& t2,const T3& t3,const T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(const T0& t0,T1& t1,const T2& t2,T3& t3,T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(const T0& t0,T1& t1,const T2& t2,T3& t3,const T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(const T0& t0,T1& t1,const T2& t2,const T3& t3,T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(const T0& t0,T1& t1,const T2& t2,const T3& t3,const T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(const T0& t0,const T1& t1,T2& t2,T3& t3,T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(const T0& t0,const T1& t1,T2& t2,T3& t3,const T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(const T0& t0,const T1& t1,T2& t2,const T3& t3,T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(const T0& t0,const T1& t1,T2& t2,const T3& t3,const T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(const T0& t0,const T1& t1,const T2& t2,T3& t3,T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(const T0& t0,const T1& t1,const T2& t2,T3& t3,const T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(const T0& t0,const T1& t1,const T2& t2,const T3& t3,T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}\
template<typename T0,typename T1,typename T2,typename T3,typename T4> explicit flyweight(const T0& t0,const T1& t1,const T2& t2,const T3& t3,const T4& t4):h(core::insert(T(t0,t1,t2,t3,t4))){}
#endif

#if   BOOST_FLYWEIGHT_LIMIT_NUM_CTOR_ARGS==0
#define BOOST_FLYWEIGHT_CTORS
#elif BOOST_FLYWEIGHT_LIMIT_NUM_CTOR_ARGS==1
#define BOOST_FLYWEIGHT_CTORS \
BOOST_FLYWEIGHT_CTORS_1
#elif BOOST_FLYWEIGHT_LIMIT_NUM_CTOR_ARGS==2
#define BOOST_FLYWEIGHT_CTORS \
BOOST_FLYWEIGHT_CTORS_1       \
BOOST_FLYWEIGHT_CTORS_2
#elif BOOST_FLYWEIGHT_LIMIT_NUM_CTOR_ARGS==3
#define BOOST_FLYWEIGHT_CTORS \
BOOST_FLYWEIGHT_CTORS_1       \
BOOST_FLYWEIGHT_CTORS_2       \
BOOST_FLYWEIGHT_CTORS_3
#elif BOOST_FLYWEIGHT_LIMIT_NUM_CTOR_ARGS==4
#define BOOST_FLYWEIGHT_CTORS \
BOOST_FLYWEIGHT_CTORS_1       \
BOOST_FLYWEIGHT_CTORS_2       \
BOOST_FLYWEIGHT_CTORS_3       \
BOOST_FLYWEIGHT_CTORS_4
#else /* BOOST_FLYWEIGHT_LIMIT_NUM_CTOR_ARGS==5 */
#define BOOST_FLYWEIGHT_CTORS \
BOOST_FLYWEIGHT_CTORS_1       \
BOOST_FLYWEIGHT_CTORS_2       \
BOOST_FLYWEIGHT_CTORS_3       \
BOOST_FLYWEIGHT_CTORS_4       \
BOOST_FLYWEIGHT_CTORS_5
#endif

#endif
