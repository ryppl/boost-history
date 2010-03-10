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
#include <set>
#include <stdexcept>
#include <boost/range.hpp>

namespace boost{
namespace assign{
namespace detail{
namespace auto_size{

// Usage:
//  check_converter(elems,list_of(a)(b)(c)(d)(e)(f)(g)(h),a,b,c,d,e,f,g,h)
// These must not be a sorted sequence

#ifndef BOOST_ASSIGN_CHECK_EQUAL
#error
#endif

// fwd declare
template<typename C,typename C2,typename T>
void check_converter(
	C& elems, const C2& coll, 
    const T& a, const T& b, const T& c, const T& d, const T& e, const T& f,
    const T& g, const T& h, bool sorted = false
);

// Overload #1

template<typename C2,typename T>
void check_converter(
	std::set<T>& elems, const C2& coll, 
    const T& a, const T& b, const T& c, const T& d, const T& e, const T& f,
    const T& g, const T& h
){
    check_converter(elems,coll,a,b,c,d,e,f,g,h,true);
}

// Generic implementation

template<typename C,typename C2,typename T>
void check_converter(
	C& elems, const C2& coll, 
    const T& a, const T& b, const T& c, const T& d, const T& e, const T& f,
    const T& g, const T& h, bool sorted = false
){

    if( (a<=b) && (b<=c) && (c<=d) && (d<=e) && (e<=f) && (f<=g) && (g<=h) ){
    	throw std::runtime_error(
           "check_converter expects a non-sorterd sequence"
    	);
    }

	using namespace boost::assign;
    elems = coll;
    typedef typename boost::range_iterator<const C>::type it_;
    it_ it = boost::begin(elems);
    if(!sorted){
        BOOST_ASSIGN_CHECK_EQUAL(*it , a); ++it;
        BOOST_ASSIGN_CHECK_EQUAL(*it , b); ++it;
        BOOST_ASSIGN_CHECK_EQUAL(*it , c); ++it;
        BOOST_ASSIGN_CHECK_EQUAL(*it , d); ++it;
        BOOST_ASSIGN_CHECK_EQUAL(*it , e); ++it;
        BOOST_ASSIGN_CHECK_EQUAL(*it , f); ++it;
        BOOST_ASSIGN_CHECK_EQUAL(*it , g); ++it;
        BOOST_ASSIGN_CHECK_EQUAL(*it , h); ++it;
        BOOST_ASSIGN_CHECK_EQUAL(it==it_(boost::end(elems)) ,true ); 
       // Comparison operators
       BOOST_ASSIGN_CHECK_EQUAL(elems == coll,true);
       BOOST_ASSIGN_CHECK_EQUAL(elems >= coll,true);
       BOOST_ASSIGN_CHECK_EQUAL(coll == elems,true);
       BOOST_ASSIGN_CHECK_EQUAL(coll <= elems,true);
    }else{
        BOOST_ASSIGN_CHECK_EQUAL(*it , 0); ++it;
        BOOST_ASSIGN_CHECK_EQUAL(*it , 1); ++it;
        BOOST_ASSIGN_CHECK_EQUAL(*it , 2); ++it;
        BOOST_ASSIGN_CHECK_EQUAL(*it , 3); ++it;
        BOOST_ASSIGN_CHECK_EQUAL(*it , 4); ++it;
        BOOST_ASSIGN_CHECK_EQUAL(*it , 5); ++it;
        BOOST_ASSIGN_CHECK_EQUAL(*it , 7); ++it;
        BOOST_ASSIGN_CHECK_EQUAL(*it , 9); ++it;
        BOOST_ASSIGN_CHECK_EQUAL(it==it_(boost::end(elems)) ,true ); 
       // Comparison operators
       BOOST_ASSIGN_CHECK_EQUAL(elems != coll,true);
       BOOST_ASSIGN_CHECK_EQUAL(elems < coll, true);
       BOOST_ASSIGN_CHECK_EQUAL(coll != elems,true);
       BOOST_ASSIGN_CHECK_EQUAL(coll > elems, true);
    }

}

}// auto_size
}// detail
}// assign
}// boost

#endif
