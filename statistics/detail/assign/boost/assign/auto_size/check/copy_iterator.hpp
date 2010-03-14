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

#ifndef BOOST_ASSIGN_CHECK_EQUAL
#error
#endif

namespace boost{
namespace assign{
namespace detail{
namespace auto_size{

// Usage 
// BOOST_AUTO(tmp,ref_list_of(a)(b)(c)(d)(e)(f)(g)(h))
// check_iterator(tmp,a,b,c,d,e,f,g,h)

template<typename C,typename T>
void check_copy_iterator(C& coll,
    // TODO: I guess these can be const
    T& a,       T& b,       T& c,        T& d,       T& e,       T& f, 
    T& g,       T& h
){
        const T   
            a1 = 1, b1 = 5, c1 = 3, 
            d1 = 4, e1 = 2, f1 = 9, 
            g1 = 0, h1 = 7;
        boost::array<T,8> ar;
        ar[0] = a1;
        ar[1] = b1;
        ar[2] = c1;
        ar[3] = d1;
        ar[4] = e1;
        ar[5] = f1;
        ar[6] = g1;
        ar[7] = h1;
        std::copy(boost::begin(ar),boost::end(ar),boost::begin(coll));
        BOOST_ASSIGN_CHECK_EQUAL(a , a1);
        BOOST_ASSIGN_CHECK_EQUAL(b , b1);
        BOOST_ASSIGN_CHECK_EQUAL(c , c1);
        BOOST_ASSIGN_CHECK_EQUAL(d , d1);
        BOOST_ASSIGN_CHECK_EQUAL(e , e1);
        BOOST_ASSIGN_CHECK_EQUAL(f , f1);
        BOOST_ASSIGN_CHECK_EQUAL(g , g1);
        BOOST_ASSIGN_CHECK_EQUAL(h , h1);
}

}// auto_size
}// detail
}// assign
}// boost

#endif
