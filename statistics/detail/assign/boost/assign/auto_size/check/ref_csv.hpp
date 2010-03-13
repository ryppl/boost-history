//////////////////////////////////////////////////////////////////////////////
// assign::detail::auto_size::check_ref_csv.hpp                             //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHECK_REF_CSV_OF_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHECK_REF_CSV_OF_ER_2010_HPP
#include <vector>
#include <set>
#include <list>
#include <stack>
#include <queue>
#include <boost/array.hpp>
#include <boost/assign/auto_size/ref_csv.hpp>
#include <boost/assign/auto_size/ref_rebind_csv.hpp>

#include <boost/assign/auto_size/check/array.hpp>
#include <boost/assign/auto_size/check/adapter.hpp>
#include <boost/assign/auto_size/check/converter.hpp>
#include <boost/assign/auto_size/check/copy_array.hpp>
#include <boost/assign/auto_size/check/copy_iterator.hpp>
#include <boost/assign/auto_size/check/fifo.hpp>
#include <boost/assign/auto_size/check/iterator.hpp>
#include <boost/assign/auto_size/check/lifo.hpp>
#include <boost/assign/auto_size/check/rebind_array.hpp>

// Until range_ex avail.
#include <boost/assign/auto_size/check/max_element.hpp>

namespace boost{
namespace assign{
namespace detail{
namespace auto_size{

template<typename T>
void check_ref_csv()
{
	typedef T val_;
    using namespace boost::assign;
    const val_ 
            a1 = 1, b1 = 5, c1 = 3, 
            d1 = 4, e1 = 2, f1 = 9, 
            g1 = 0, h1 = 7;

    {
        // ---- Examples in the documentation
        val_ a=a1, b=b1, c=c1, d=d1, e=e1, f=f1, g=g1, h=h1;
            val_& max = *max_element( ref_csv(a,b,c,d,e,f,g,h) );
            BOOST_ASSERT( max == f );
            max = 8;
            BOOST_ASSERT( f == 8 );
            const val_& const_max = *max_element( 
                cref_csv(1,5,3,d,e,f,g,h) );
            BOOST_ASSERT( max == const_max );
    }
	{   
        check_iterator(
        	cref_csv(a1,b1,c1,d1,e1,f1,g1,h1),
            a1,b1,c1,d1,e1,f1,g1,h1);
    }
	{   
        check_array(
            cref_csv(a1,b1,c1,d1,e1,f1,g1,h1),
            a1,b1,c1,d1,e1,f1,g1,h1);
    }
    {    
        val_ a, b, c, d, e, f, g, h;
        BOOST_AUTO(tmp,ref_csv(a,b,c,d,e,f,g,h));
        check_copy_iterator(
            tmp,
            a,b,c,d,e,f,g,h);
    }
    {
        val_ a, b, c, d, e, f;
        BOOST_AUTO(tmp1,ref_csv(a,b,c));
        BOOST_AUTO(tmp2,ref_csv(d,e,f));
        check_copy_array(
            tmp1,tmp2,
            a,b,c,d,e,f);
    }
    {
         val_ a, b, c, d, e, f;
        BOOST_AUTO(tmp1,ref_rebind_csv(a,b,c));
        BOOST_AUTO(tmp2,ref_rebind_csv(d,e,f));
        check_rebind_array(
             tmp1,
             tmp2,
             a,b,c,d,e,f);
    }
    {
        std::list<val_> elems;
        check_converter(
           elems,
           ref_csv(a1,b1,c1,d1,e1,f1,g1,h1),
           a1,b1,c1,d1,e1,f1,g1,h1);
    }
    {
        std::vector<val_> elems;
        check_converter(
           elems,
           ref_csv(a1,b1,c1,d1,e1,f1,g1,h1),
           a1,b1,c1,d1,e1,f1,g1,h1);
    }
    {
        boost::array<val_,8> elems;
        check_converter(
           elems,
           ref_csv(a1,b1,c1,d1,e1,f1,g1,h1),
           a1,b1,c1,d1,e1,f1,g1,h1);
    }
    {
        std::set<val_> elems;
        check_converter(
           elems,
           ref_csv(a1,b1,c1,d1,e1,f1,g1,h1),
           a1,b1,c1,d1,e1,f1,g1,h1);
    }
    {
        std::stack<val_> elems;
        check_adapter(
           elems,
           ref_csv(a1,b1,c1,d1,e1,f1,g1,h1),
           a1,b1,c1,d1,e1,f1,g1,h1);
    }
    {	
        std::queue<val_> elems;
        check_adapter(
           elems,
           ref_csv(a1,b1,c1,d1,e1,f1,g1,h1),
           a1,b1,c1,d1,e1,f1,g1,h1);
    }

    // TODO add check_chain
}


}// auto_size
}// detail
}// assign
}// boost

#endif


