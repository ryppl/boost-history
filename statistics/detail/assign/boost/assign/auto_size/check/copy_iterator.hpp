//////////////////////////////////////////////////////////////////////////////
// assign::detail::auto_size::copy_iterator.hpp                             //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHECK_COPY_ITERATOR_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHECK_COPY_ITERATOR_ER_2010_HPP
#include <algorithm>
#include <boost/range.hpp>
#include <boost/array.hpp>
#include <boost/assign/auto_size/check/constants.hpp>

#ifndef BOOST_ASSIGN_CHECK_EQUAL
#error
#endif

#define BOOST_ASSIGN_AS_CHECK_copy_iterator_f(arg)                             \
{                                                                              \
    typedef T val_;                                                            \
    using namespace check_constants;                                           \
    boost::array<val_,8> ar;                                                   \
    ar[0] = a;                                                                 \
    ar[1] = b;                                                                 \
    ar[2] = c;                                                                 \
    ar[3] = d;                                                                 \
    ar[4] = e;                                                                 \
    ar[5] = f;                                                                 \
    ar[6] = g;                                                                 \
    ar[7] = h;                                                                 \
    val_ a1, b1, c1, d1, e1, f1, g1, h1;                                       \
    BOOST_AUTO(tmp,arg);                                                       \
    std::copy(boost::begin(ar),boost::end(ar),boost::begin(tmp));              \
    BOOST_ASSIGN_CHECK_EQUAL(a , a1);                                          \
    BOOST_ASSIGN_CHECK_EQUAL(b , b1);                                          \
    BOOST_ASSIGN_CHECK_EQUAL(c , c1);                                          \
    BOOST_ASSIGN_CHECK_EQUAL(d , d1);                                          \
    BOOST_ASSIGN_CHECK_EQUAL(e , e1);                                          \
    BOOST_ASSIGN_CHECK_EQUAL(f , f1);                                          \
    BOOST_ASSIGN_CHECK_EQUAL(g , g1);                                          \
    BOOST_ASSIGN_CHECK_EQUAL(h , h1);                                          \
}                                                                              \
/**/

#define BOOST_ASSIGN_AS_CHECK_copy_iterator                                    \
BOOST_ASSIGN_AS_CHECK_copy_iterator_f(BOOST_ASSIGN_AS_CHECK_ref8(a1,b1,c1,d1,e1,f1,g1,h1))\
/**/

#endif
