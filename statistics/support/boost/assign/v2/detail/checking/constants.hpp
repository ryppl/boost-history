//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CHECKING_CONSTANTS_ER_2010_HPP
#define BOOST_ASSIGN_V2_CHECKING_CONSTANTS_ER_2010_HPP
#include <boost/next_prior.hpp>
#include <boost/range.hpp>
#include <boost/array.hpp>
#include <boost/numeric/conversion/converter.hpp>
#include <boost/assign/v2/detail/checking/check.hpp>

namespace boost{                                                               
namespace assign{ 
namespace v2{
namespace checking{ 
namespace constants{                                               

namespace {
    // value            // rank
    const int a = 1;    // 1
    const int b = 5;    // 5
    const int c = 3;    // 3
    const int d = 4;    // 4
    const int e = 2;    // 2
    const int f = 9;    // 7
    const int g = 0;    // 0
    const int h = 7;    // 6
}
    // array    
    template<typename T>
    boost::array<T,8> make_array()
    {                                              
        typedef boost::array<T,8> ar_;
        ar_ ar;
        ar[0] = a; ar[1] = b; ar[2] = c;                                           
        ar[3] = d; ar[4] = e; ar[5] = f;                                           
        ar[6] = g; ar[7] = h;                                                      
        return ar;
    }    

    namespace{
       const boost::array<int,8> array = make_array<int>();
    }
    
    // values 
    template<typename T>
    void do_check(
        const T& a1,const T& b1,const T& c1,const T& d1,
        const T& e1,const T& f1,const T& g1,const T& h1
    )
    {
        BOOST_ASSIGN_V2_CHECK( a == a1 );
        BOOST_ASSIGN_V2_CHECK( b == b1 );
        BOOST_ASSIGN_V2_CHECK( c == c1 );
        BOOST_ASSIGN_V2_CHECK( d == d1 ); 
        BOOST_ASSIGN_V2_CHECK( e == e1 );
        BOOST_ASSIGN_V2_CHECK( f == f1 ); 
        BOOST_ASSIGN_V2_CHECK( g == g1 );
        BOOST_ASSIGN_V2_CHECK( h == h1 );                                          
    }                                    

namespace sorted{

    template<typename T>
    void do_check(
        const T& a1,const T& b1,const T& c1,const T& d1,
        const T& e1,const T& f1,const T& g1,const T& h1
    )
    {
        namespace ns = checking::constants;
        BOOST_ASSIGN_V2_CHECK( a1 == ns::g );
        BOOST_ASSIGN_V2_CHECK( b1 == ns::a );
        BOOST_ASSIGN_V2_CHECK( c1 == ns::e );
        BOOST_ASSIGN_V2_CHECK( d1 == ns::c );
        BOOST_ASSIGN_V2_CHECK( e1 == ns::d );
        BOOST_ASSIGN_V2_CHECK( f1 == ns::b );
        BOOST_ASSIGN_V2_CHECK( g1 == ns::h );
        BOOST_ASSIGN_V2_CHECK( h1 == ns::f );
    }                                                                  

}// sorted
}// constants
}// checking
}// v2
}// assign
}// boost    
    
#endif
