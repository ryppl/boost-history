//////////////////////////////////////////////////////////////////////////////
// assign::detail::auto_size::check_adapter.hpp                             //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHECK_CHAIN_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHECK_CHAIN_ER_2010_HPP
#include <boost/array.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/assign/auto_size/chain/chain_convert.hpp>
#include <boost/assign/auto_size/check/iterator.hpp>
#include <boost/assign/auto_size/check/constants.hpp>
#include <boost/assign/auto_size/check/copy_iterator.hpp>
#include <boost/assign/auto_size/reference_wrapper/conversion_traits.hpp> // MUST be included

#ifndef BOOST_ASSIGN_CHECK_EQUAL
#error
#endif

#define BOOST_ASSIGN_AS_CHECK_chain                                            \
{                                                                              \
    typedef T val_;                                                            \
    using namespace check_constants;                                           \
    BOOST_AUTO(tmp1,BOOST_ASSIGN_AS_CHECK_ref3(a,b,c));                        \
    BOOST_AUTO(tmp2,BOOST_ASSIGN_AS_CHECK_ref3(d,e,f));                        \
    BOOST_AUTO(tmp3,BOOST_ASSIGN_AS_CHECK_ref2(g,h));                          \
    typedef boost::array<T,2> ar2_;                                            \
    typedef boost::array<T,3> ar3_;                                            \
    ar3_ ar1, ar2;                                                             \
    ar2_ ar3;                                                                  \
    ar1[0] = a; ar1[1] = b; ar1[2] = c;                                        \
    ar2[0] = d; ar2[1] = e; ar2[2] = f;                                        \
    ar3[0] = g; ar3[1] = h;                                                    \
    BOOST_ASSIGN_AS_CHECK_iterator_f(chain_convert_r(ar1)(ar2)(ar3))           \
    BOOST_ASSIGN_AS_CHECK_iterator_f(chain_convert_r(ar1)(ar2)(tmp3))          \
    BOOST_ASSIGN_AS_CHECK_iterator_f(chain_convert_r(ar1)(tmp2)(ar3))          \
    BOOST_ASSIGN_AS_CHECK_iterator_f(chain_convert_r(ar1)(tmp2)(tmp3))         \
    BOOST_ASSIGN_AS_CHECK_iterator_f(chain_convert_r(tmp1)(ar2)(ar3))          \
    BOOST_ASSIGN_AS_CHECK_iterator_f(chain_convert_r(tmp1)(ar2)(tmp3))         \
    BOOST_ASSIGN_AS_CHECK_iterator_f(chain_convert_r(tmp1)(tmp2)(ar3))         \
    BOOST_ASSIGN_AS_CHECK_iterator_f(chain_convert_r(tmp1)(tmp2)(tmp3))        \
}                                                                              \
/**/

//    BOOST_ASSIGN_AS_CHECK_copy_iterator_f(chain_convert_l(ar1)(ar2)(ar3))      \
//    check_copy_iterator(tmp1,a,b,c,d,e,f,g,h);                                 \


#endif
