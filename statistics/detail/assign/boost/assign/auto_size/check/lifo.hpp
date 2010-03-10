//////////////////////////////////////////////////////////////////////////////
// assign::detail::auto_size::check_lifo.hpp                                //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHECK_LIFO_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHECK_LIFO_ER_2010_HPP
#include <set>
#include <stdexcept>

#ifndef BOOST_ASSIGN_CHECK_EQUAL
#error
#endif

namespace boost{
namespace assign{
namespace detail{
namespace auto_size{

	template<typename C,typename C1,typename T>
    void check_lifo(C& elems,const C1& coll,
        const T& a,const T& b,const T& c,const T& d,const T& e,const T& f,
        const T& g,const T& h
    )
    {   
        elems = coll.to_adapter();
        BOOST_ASSIGN_CHECK_EQUAL(elems.top() , h); elems.pop(); 
        BOOST_ASSIGN_CHECK_EQUAL(elems.top() , g); elems.pop(); 
        BOOST_ASSIGN_CHECK_EQUAL(elems.top() , f); elems.pop(); 
        BOOST_ASSIGN_CHECK_EQUAL(elems.top() , e); elems.pop(); 
        BOOST_ASSIGN_CHECK_EQUAL(elems.top() , d); elems.pop(); 
        BOOST_ASSIGN_CHECK_EQUAL(elems.top() , c); elems.pop(); 
        BOOST_ASSIGN_CHECK_EQUAL(elems.top() , b); elems.pop(); 
        BOOST_ASSIGN_CHECK_EQUAL(elems.top() , a); elems.pop(); 
        BOOST_ASSIGN_CHECK_EQUAL(elems.empty() , true);  
    }

}// auto_size
}// detail
}// assign
}// boost

#endif
