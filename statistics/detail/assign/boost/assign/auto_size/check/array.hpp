//////////////////////////////////////////////////////////////////////////////
// assign::detail::auto_size::check_array.hpp                               //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHECK_ARRAY_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHECK_ARRAY_ER_2010_HPP
#include <boost/typeof/typeof.hpp>
#include <boost/next_prior.hpp>
#include <boost/range.hpp>
#include <boost/assign/auto_size/check/constants.hpp>

#ifndef BOOST_ASSIGN_CHECK_EQUAL
#error
#endif

#define BOOST_ASSIGN_AS_CHECK_array                                            \
{                                                                              \
    typedef T val_;                                                            \
    using namespace check_constants;                                           \
                                                                               \
    BOOST_AUTO(tmp,BOOST_ASSIGN_AS_CHECK_cref8(a,b,c,d,e,f,g,h));              \
                                                                               \
    BOOST_ASSIGN_CHECK_EQUAL( tmp.empty() , false );                           \
    BOOST_ASSIGN_CHECK_EQUAL( tmp[0] , a );                                    \
    BOOST_ASSIGN_CHECK_EQUAL( tmp[1] , b );                                    \
    BOOST_ASSIGN_CHECK_EQUAL( tmp[2] , c );                                    \
    BOOST_ASSIGN_CHECK_EQUAL( tmp[3] , d );                                    \
    BOOST_ASSIGN_CHECK_EQUAL( tmp[4] , e );                                    \
    BOOST_ASSIGN_CHECK_EQUAL( tmp[5] , f );                                    \
    BOOST_ASSIGN_CHECK_EQUAL( tmp[6] , g );                                    \
    BOOST_ASSIGN_CHECK_EQUAL( tmp[7] , h );                                    \
    BOOST_ASSIGN_CHECK_EQUAL( tmp.front() , a );                               \
    BOOST_ASSIGN_CHECK_EQUAL( tmp.back() , h );                                \
}                                                                              \
/**/

#endif
