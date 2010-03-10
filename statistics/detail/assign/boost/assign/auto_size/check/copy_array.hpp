//////////////////////////////////////////////////////////////////////////////
// assign::detail::auto_size::copy_array.hpp                                //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHECK_COPY_ARRAY_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHECK_COPY_ARRAY_ER_2010_HPP
#include <algorithm>
#include <boost/range.hpp>

#ifndef BOOST_ASSIGN_CHECK_EQUAL
#error
#endif

namespace boost{
namespace assign{
namespace detail{
namespace auto_size{

// Usage
// BOOST_AUTO(tmp1,ref_list_of(a)(b)(c));
// BOOST_AUTO(tmp2,ref_list_of(d)(e)(f));
// check_copy_array(tmp1,tmp2,a,b,c,d,e,f);

template<typename C,typename C1,typename T>
void check_copy_array(C& coll,C1& coll1,
          T& a,       T& b,       T& c,       
          T& d,       T& e,       T& f
){
        // Ensures that they are different
        const T a1 = 1, b1 = 5, c1 = 3, 
                d1 = 4, e1 = 2, f1 = 9; 
        a = a1;
        b = b1;
        c = c1;
        d = d1;
        e = e1;
        f = f1;
        coll.swap(coll1);
        BOOST_ASSIGN_CHECK_EQUAL( a , d1 );
        BOOST_ASSIGN_CHECK_EQUAL( b , e1 );
        BOOST_ASSIGN_CHECK_EQUAL( c , f1 );
        BOOST_ASSIGN_CHECK_EQUAL( d , a1 );
        BOOST_ASSIGN_CHECK_EQUAL( e , b1 );
        BOOST_ASSIGN_CHECK_EQUAL( f , c1 );
        coll.assign(0);
        BOOST_ASSIGN_CHECK_EQUAL( a , 0 );
        BOOST_ASSIGN_CHECK_EQUAL( b , 0 );
        BOOST_ASSIGN_CHECK_EQUAL( c , 0 );
}

}// auto_size
}// detail
}// assign
}// boost

#endif
