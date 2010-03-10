//////////////////////////////////////////////////////////////////////////////
// assign::detail::auto_size::chec_ref_list_of.hpp                          //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHECK_REF_LIST_OF_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHECK_REF_LIST_OF_ER_2010_HPP
#include <vector>
#include <set>
#include <list>
#include <stack>
#include <queue>
#include <boost/array.hpp>
#include <boost/assign/auto_size/ref_list_of.hpp>
#include <boost/assign/auto_size/ref_rebind_list_of.hpp>
#include <boost/assign/auto_size/check/include.hpp>

namespace boost{
namespace assign{
namespace detail{
namespace auto_size{

template<typename T>
void check_ref_list_of(std::ostream& os)
{
	typedef T val_;
    using namespace boost::assign;
    const val_ 
            a1 = 1, b1 = 5, c1 = 3, 
            d1 = 4, e1 = 2, f1 = 9, 
            g1 = 0, h1 = 7, i1 = 8;
    {
        // ---- Examples in the documentation
        val_ a=a1, b=b1, c=c1, d=d1, e=e1, f=f1, g=g1, h=h1;
            val_& max = *max_element( ref_list_of(a)(b)(c)(d)(e)(f)(g)(h) );
            BOOST_ASSERT( max == f );
            max = 8;
            BOOST_ASSERT( f == 8 );
            const val_& const_max = *max_element( 
                cref_list_of(1)(5)(3)(d)(e)(f)(g)(h) );
            BOOST_ASSERT( max == const_max );
    }
	{   
        check_iterator<val_>(cref_list_of(a1)(b1)(c1)(d1)(e1)(f1)(g1)(h1),
            a1,b1,c1,d1,e1,f1,g1,h1);
    }
	{   
        check_array<val_>(cref_list_of(a1)(b1)(c1)(d1)(e1)(f1)(g1)(h1),
            a1,b1,c1,d1,e1,f1,g1,h1);
    }
    {    
        val_ a, b, c, d, e, f;
        check_copy_iterator<val_>(
            ref_list_of(a)(b)(c),
            ref_list_of(d)(e)(f),
            a,b,c,d,e,f)
        check_copy_array<val_>(
            ref_list_of(a)(b)(c),
            ref_list_of(d)(e)(f),
            a,b,c,d,e,f)
    }
    {
         val_ a, b, c, d, e, f;
         check_rebind_array(
             ref_rebind_list_of(a)(b)(c),
             ref_rebind_list_of(d)(e)(f),
             a,b,c,d,e,f);
    }
    {
        std::list<val_> elems;
        check_converter(
           elems,
           list_of(a1)(b1)(c1)(d1)(e1)(f1)(g1)(h1),
           a1,b1,c1,d1,e1,f1,g1,h1);
    }
    {
        std::set<val_> elems;
        check_converter(
           elems,
           list_of(a1)(b1)(c1)(d1)(e1)(f1)(g1)(h1),
           a1,b1,c1,d1,e1,f1,g1,h1);
    }
    {
        std::vector<val_> elems;
        check_converter(
           elems,
           list_of(a1)(b1)(c1)(d1)(e1)(f1)(g1)(h1),
           a1,b1,c1,d1,e1,f1,g1,h1);
    }
    {
        boost::array<val_,8> elems;
        check_converter(
           elems,
           list_of(a1)(b1)(c1)(d1)(e1)(f1)(g1)(h1),
           a1,b1,c1,d1,e1,f1,g1,h1);
    }
    {
        std::stack<val_> elems;
        check_adapter(
           elems,
           list_of(a1)(b1)(c1)(d1)(e1)(f1)(g1)(h1),
           a1,b1,c1,d1,e1,f1,g1,h1);
    }
    {	
        std::queue<val_> elems;
        check_adapter(
           elems,
           list_of(a1)(b1)(c1)(d1)(e1)(f1)(g1)(h1),
           a1,b1,c1,d1,e1,f1,g1,h1);
    }
}

#endif


