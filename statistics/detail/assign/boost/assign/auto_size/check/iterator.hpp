//////////////////////////////////////////////////////////////////////////////
// assign::detail::auto_size::check_iterator.hpp                            //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHECK_ITERATOR_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHECK_ITERATOR_ER_2010_HPP
#include <boost/typeof/typeof.hpp>
#include <boost/next_prior.hpp>
#include <boost/range.hpp>


#ifndef BOOST_ASSIGN_CHECK_EQUAL
#error
#endif

namespace boost{
namespace assign{
namespace detail{
namespace auto_size{

// Usage
// check_iterator(list_of(a)(b)(c)(d)(e)(f)(g)(h),a,b,c,d,e,f,g,h);

template<typename C1,typename T>
void check_iterator(const C1& coll,
    const T& a,const T& b,const T& c, const T& d,const T& e,const T& f, 
    const T& g,const T& h
){
        BOOST_AUTO(it,boost::begin(coll));
        BOOST_ASSIGN_CHECK_EQUAL(*boost::next(it,0), a); 
        BOOST_ASSIGN_CHECK_EQUAL(*boost::next(it,1), b); 
        BOOST_ASSIGN_CHECK_EQUAL(*boost::next(it,2), c); 
        BOOST_ASSIGN_CHECK_EQUAL(*boost::next(it,3), d); 
        BOOST_ASSIGN_CHECK_EQUAL(*boost::next(it,4), e); 
        BOOST_ASSIGN_CHECK_EQUAL(*boost::next(it,5), f); 
        BOOST_ASSIGN_CHECK_EQUAL(*boost::next(it,6), g); 
        BOOST_ASSIGN_CHECK_EQUAL(*boost::next(it,7), h); 
        BOOST_ASSIGN_CHECK_EQUAL(boost::next(it,8), boost::end(coll)); 
}


}// auto_size
}// detail
}// assign
}// boost

#endif
