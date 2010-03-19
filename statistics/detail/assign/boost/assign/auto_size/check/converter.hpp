//////////////////////////////////////////////////////////////////////////////
// assign::detail::auto_size::check_converter.hpp                           //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHECK_CONVERTER_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHECK_CONVERTER_ER_2010_HPP
#include <boost/array.hpp>
#include <boost/range.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <boost/assign/auto_size/check/constants.hpp>

#ifndef BOOST_ASSIGN_CHECK_EQUAL
#error
#endif

#define BOOST_ASSIGN_AS_CHECK_converter(C)                                     \
{                                                                              \
    typedef T val_;                                                            \
                                                                               \
	using namespace boost::assign;                                             \
    using namespace check_constants;                                           \
    C elems = BOOST_ASSIGN_AS_CHECK_cref8(a,b,c,d,e,f,g,h);                    \
    typedef typename boost::range_iterator<const C>::type it_;                 \
    it_ it = boost::begin(elems);                                              \
    BOOST_ASSIGN_CHECK_EQUAL(*it , a); ++it;                                   \
    BOOST_ASSIGN_CHECK_EQUAL(*it , b); ++it;                                   \
    BOOST_ASSIGN_CHECK_EQUAL(*it , c); ++it;                                   \
    BOOST_ASSIGN_CHECK_EQUAL(*it , d); ++it;                                   \
    BOOST_ASSIGN_CHECK_EQUAL(*it , e); ++it;                                   \
    BOOST_ASSIGN_CHECK_EQUAL(*it , f); ++it;                                   \
    BOOST_ASSIGN_CHECK_EQUAL(*it , g); ++it;                                   \
    BOOST_ASSIGN_CHECK_EQUAL(*it , h); ++it;                                   \
    BOOST_ASSIGN_CHECK_EQUAL(it==it_(boost::end(elems)) ,true );               \
}                                                                              \
/**/

#define BOOST_ASSIGN_AS_CHECK_converter_sorted(C)                              \
{                                                                              \
    typedef T val_;                                                            \
                                                                               \
	using namespace boost::assign;                                             \
    using namespace check_constants;                                           \
    C elems = BOOST_ASSIGN_AS_CHECK_cref8(a,b,c,d,e,f,g,h);                    \
    typedef boost::array<T,8> ar_;                                             \
    ar_ ar;                                                                    \
    ar[0] = a;                                                                 \
    ar[1] = b;                                                                 \
    ar[2] = c;                                                                 \
    ar[3] = d;                                                                 \
    ar[4] = e;                                                                 \
    ar[5] = f;                                                                 \
    ar[6] = g;                                                                 \
    ar[7] = h;                                                                 \
    boost::sort(ar);                                                           \
    typedef typename boost::range_iterator<const C>::type it_;                 \
    it_ it = boost::begin(elems);                                              \
    BOOST_ASSIGN_CHECK_EQUAL(*it , ar[0]); ++it;                               \
    BOOST_ASSIGN_CHECK_EQUAL(*it , ar[1]); ++it;                               \
    BOOST_ASSIGN_CHECK_EQUAL(*it , ar[2]); ++it;                               \
    BOOST_ASSIGN_CHECK_EQUAL(*it , ar[3]); ++it;                               \
    BOOST_ASSIGN_CHECK_EQUAL(*it , ar[4]); ++it;                               \
    BOOST_ASSIGN_CHECK_EQUAL(*it , ar[5]); ++it;                               \
    BOOST_ASSIGN_CHECK_EQUAL(*it , ar[6]); ++it;                               \
    BOOST_ASSIGN_CHECK_EQUAL(*it , ar[7]); ++it;                               \
    BOOST_ASSIGN_CHECK_EQUAL(it==it_(boost::end(elems)) ,true );               \
}                                                                              \
/**/
#endif
