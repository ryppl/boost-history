//////////////////////////////////////////////////////////////////////////////
// assign::detail::auto_size::rebind_array.hpp                              //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHECK_REBIND_ARRAY_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHECK_REBIND_ARRAY_ER_2010_HPP

#ifndef BOOST_ASSIGN_CHECK_EQUAL
#error
#endif

namespace boost{
namespace assign{
namespace detail{
namespace auto_size{

// Usage
// BOOST_AUTO(tmp1,ref_rebind_list_of(a)(b)(c));
// BOOST_AUTO(tmp2,ref_rebind_list_of(d)(e)(f));
// check_rebind_array(tmp1,tmp2,a,b,c,d,e,f);

template<typename C,typename C1,typename T>
void check_rebind_array(C& coll,C1& coll1,
    T& a,       T& b,       T& c,       
    T& d,       T& e,       T& f
){
        const T a1 = 1, b1 = 5, c1 = 3, 
                d1 = 4, e1 = 2, f1 = 9;
        a = a1;
        b = b1;
        c = c1;
        d = d1;
        e = e1;
        f = f1;
        BOOST_ASSIGN_CHECK_EQUAL( coll[0] , a1 );
        BOOST_ASSIGN_CHECK_EQUAL( coll[1] , b1 );
        BOOST_ASSIGN_CHECK_EQUAL( coll[2] , c1 );
        BOOST_ASSIGN_CHECK_EQUAL( coll1[0] , d1 );
        BOOST_ASSIGN_CHECK_EQUAL( coll1[1] , e1 );
        BOOST_ASSIGN_CHECK_EQUAL( coll1[2] , f1 );
		coll.swap(coll1);
        BOOST_ASSIGN_CHECK_EQUAL( coll[0] , d1 );
        BOOST_ASSIGN_CHECK_EQUAL( coll[1] , e1 );
        BOOST_ASSIGN_CHECK_EQUAL( coll[2] , f1 );
        BOOST_ASSIGN_CHECK_EQUAL( coll1[0] , a1 );
        BOOST_ASSIGN_CHECK_EQUAL( coll1[1] , b1 );
        BOOST_ASSIGN_CHECK_EQUAL( coll1[2] , c1 );
        BOOST_ASSIGN_CHECK_EQUAL( a , a1 );
        BOOST_ASSIGN_CHECK_EQUAL( b , b1 );
        BOOST_ASSIGN_CHECK_EQUAL( c , c1 );
        BOOST_ASSIGN_CHECK_EQUAL( d , d1 );
        BOOST_ASSIGN_CHECK_EQUAL( e , e1 );
        BOOST_ASSIGN_CHECK_EQUAL( f , f1 );
}

}// auto_size
}// detail
}// assign
}// boost

#endif
