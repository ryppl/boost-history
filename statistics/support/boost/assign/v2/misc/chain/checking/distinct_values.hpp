//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CHAIN_CHECKING_DISTINCT_VALUES_ER_2010_HPP
#define BOOST_ASSIGN_V2_CHAIN_CHECKING_DISTINCT_VALUES_ER_2010_HPP
#include <vector>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/assign/v2/detail/checking/constants.hpp>
#include <boost/assign/v2/detail/checking/container.hpp>
#include <boost/assign/v2/misc/chain/pipe.hpp>

namespace boost{
namespace assign{ 
namespace v2{
namespace checking{
namespace chain{
namespace distinct_values{

// If the value types of the two ranges differ, their references must be 
// converted to value_type. In particular, lvalue cannot be preserved.

#define MACRO(T1,T2)\
    template<typename T>\
    void do_check(typename boost::enable_if<boost::is_same<T,T1> >::type* = 0)\
    {\
        typedef T1 t1_;\
        typedef T2 t2_;\
        typedef std::vector<T1> vec1_;\
        typedef std::vector<T2> vec2_;\
        vec1_ vec1, vec3;\
        vec2_ vec2;\
        {\
            namespace ns = v2::checking::constants;\
            vec1.push_back( ns::a );\
            vec1.push_back( ns::b );\
            vec2.push_back( ns::c );\
            vec2.push_back( ns::d );\
            vec2.push_back( ns::e );\
            vec3.push_back( ns::f );\
            vec3.push_back( ns::g );\
            vec3.push_back( ns::h );\
        }\
        namespace chk = checking::container;\
        {\
    		chk::do_check( vec1 | _chain( vec2 ) | _chain( vec3 ) );\
        }\
    }\
/**/


MACRO(short  , int)
MACRO(int    , long)
MACRO(float  , double)
MACRO(double , long double)

#undef MACRO

}// distinct_values
}// chain
}// checking
}// v2
}// assign
}// boost

#endif
