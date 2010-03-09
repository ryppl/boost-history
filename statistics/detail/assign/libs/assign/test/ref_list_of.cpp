//////////////////////////////////////////////////////////////////////////////
// assign::test::ref_list_of.cpp                                            //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/detail/workaround.hpp>

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
#  pragma warn -8091 // supress warning in Boost.Test
#  pragma warn -8057 // unused argument argc/argv in Boost.Test
#endif

#include <vector>
#include <set>
#include <list>
#include <stack>
#include <queue>
#include <algorithm>
#include <iostream>
#include <boost/typeof/typeof.hpp>
#include <boost/array.hpp>
#include <boost/range.hpp>
//#include <boost/range/chain.hpp>
#include <boost/assign/auto_size/ref_list_of.hpp>
#include <boost/assign/auto_size/ref_rebind_list_of.hpp>
#include <boost/test/test_tools.hpp>
#include <libs/assign/test/ref_list_of.h>

template< class Range >
void print( const Range& r )
{
    std::cout << "\n printing " << typeid(r).name() << " \n"; 
    std::cout << "\n";
    
    typedef typename boost::range_iterator<const Range>::type it_;
    for(it_ i = r.begin(), e = r.end(); 
    	i !=e; ++i )
    std::cout << " " << *i;
}

template< class Range >
typename Range::const_iterator max_element( const Range& r )
{
    return std::max_element( r.begin(), r.end() );
}


template<typename C>
void check_converter(C& elems,bool sorted = false){
	using namespace boost::assign;
    int a=1, b=5, c=3, d=4, e=2, f=9, g=0, h=7;
    BOOST_AUTO(tmp,cref_list_of(a)(b)(c)(d)(e)(f)(g)(h));
    elems = tmp;
    elems == tmp;
    elems >= tmp;
    tmp == elems;
    tmp <= elems;
    typedef typename boost::range_iterator<const C>::type it_;
    it_ it = boost::begin(elems);
    if(!sorted){
        BOOST_CHECK_EQUAL(*it == a); ++it;
        BOOST_CHECK_EQUAL(*it == b); ++it;
        BOOST_CHECK_EQUAL(*it == c); ++it;
        BOOST_CHECK_EQUAL(*it == d); ++it;
        BOOST_CHECK_EQUAL(*it == e); ++it;
        BOOST_CHECK_EQUAL(*it == f); ++it;
        BOOST_CHECK_EQUAL(*it == g); ++it;
        BOOST_CHECK_EQUAL(*it == h); ++it;
        BOOST_CHECK_EQUAL(it == boost::end(elems)); 
    }else{
        BOOST_CHECK_EQUAL(*it == 0); ++it;
        BOOST_CHECK_EQUAL(*it == 1); ++it;
        BOOST_CHECK_EQUAL(*it == 2); ++it;
        BOOST_CHECK_EQUAL(*it == 3); ++it;
        BOOST_CHECK_EQUAL(*it == 4); ++it;
        BOOST_CHECK_EQUAL(*it == 5); ++it;
        BOOST_CHECK_EQUAL(*it == 7); ++it;
        BOOST_CHECK_EQUAL(*it == 9); ++it;
        BOOST_CHECK_EQUAL(it == boost::end(elems));
    }
}

void check_ref_list_of()
{
    using namespace boost::assign;
    const int 
            a1 = 1, b1 = 5, c1 = 3, 
            d1 = 4, e1 = 2, f1 = 9, 
            g1 = 0, h1 = 7, i1 = 8;
    // ---- Examples in the documentation
    {
        int a=a1, b=b1, c=c1, d=d1, e=e1, f=f1, g=g1, h=h1;
        {
            int& max = *max_element( ref_list_of(a)(b)(c)(d)(e)(f)(g)(h) );
            BOOST_CHECK_EQUAL( max == f );
            max = 8;
            BOOST_CHECK_EQUAL( f == 8 );
            const int& const_max = *max_element( 
                cref_list_of(1)(5)(3)(d)(e)(f)(g)(h) );
            BOOST_CHECK_EQUAL( max == const_max );
        }
    }
    // ---- Copy semantics
	{   // As right operand (iterators + array)
    	int a=a1, d=d1, e=e1, f=f1, g=g1, h=h1;
        BOOST_AUTO(tmp,cref_list_of(1)(5)(3)(d)(e)(f)(g)(h));
        BOOST_AUTO(it,boost::begin(tmp));
        BOOST_CHECK_EQUAL(*it == a); it = boost::next(it,7);
        BOOST_CHECK_EQUAL(*it == h); ++it;
        BOOST_CHECK_EQUAL(it == boost::end(tmp));
        BOOST_CHECK_EQUAL( tmp.size() == 8 );
        BOOST_CHECK_EQUAL( tmp.empty() == false );
        BOOST_CHECK_EQUAL( tmp[0] == a );
        BOOST_CHECK_EQUAL( tmp[7] == h );
        BOOST_CHECK_EQUAL( tmp.front() == a );
        BOOST_CHECK_EQUAL( tmp.back() == h );
    }
	{   // As left operand (iterators)
    	int a, b, c, 
            d, e, f, 
            g, h, i;
        BOOST_AUTO(tmp1,cref_list_of
            (a1)(b1)(c1)
            (d1)(e1)(f1)
            (g1)(h1)(i1));
        BOOST_AUTO(tmp2,ref_list_of
            (a)(b)(c)
            (d)(e)(f)
            (g)(h)(i));
        std::copy(boost::begin(tmp1),boost::end(tmp1),boost::begin(tmp2));
        BOOST_CHECK_EQUAL(a == a1);
        BOOST_CHECK_EQUAL(b == b1);
        BOOST_CHECK_EQUAL(c == c1);
        BOOST_CHECK_EQUAL(d == d1);
        BOOST_CHECK_EQUAL(e == e1);
        BOOST_CHECK_EQUAL(f == f1);
        BOOST_CHECK_EQUAL(g == g1);
        BOOST_CHECK_EQUAL(h == h1);
        BOOST_CHECK_EQUAL(i == i1);
    }
	{   // As left operand (array)  
    	int a=a1, b=b1, c=c1, 
            d=d1, e=e1, f=f1, 
            g=g1, h=h1, i=i1;
        BOOST_AUTO(tmp1,ref_list_of(a)(b)(c));
        BOOST_AUTO(tmp2,ref_list_of(d)(e)(f));
        BOOST_AUTO(tmp3,ref_list_of(g)(h)(i));
        tmp1.swap(tmp2);
        BOOST_CHECK_EQUAL( a == d1 );
        BOOST_CHECK_EQUAL( b == e1 );
        BOOST_CHECK_EQUAL( c == f1 );
        BOOST_CHECK_EQUAL( d == a1 );
        BOOST_CHECK_EQUAL( e == b1 );
        BOOST_CHECK_EQUAL( f == c1 );
        tmp3.assign(0);
        BOOST_CHECK_EQUAL( g == 0 );
        BOOST_CHECK_EQUAL( h == 0 );
        BOOST_CHECK_EQUAL( i == 0 );
    }
    // ---- Rebind semantics
	{   // As left operand 
    	int a=a1, b=b1, c=c1, 
            d=d1, e=e1, f=f1, 
            g=g1, h=h1, i=i1;
        BOOST_AUTO(tmp1,ref_rebind_list_of(a)(b)(c));
        BOOST_AUTO(tmp2,ref_rebind_list_of(d)(e)(f));
        BOOST_AUTO(tmp3,cref_rebind_list_of(g)(h)(i));
		tmp1.swap(tmp2);
        BOOST_CHECK_EQUAL( tmp1[0] == d );
        BOOST_CHECK_EQUAL( tmp1[1] == e );
        BOOST_CHECK_EQUAL( tmp1[2] == f );
        BOOST_CHECK_EQUAL( tmp2[0] == a );
        BOOST_CHECK_EQUAL( tmp2[1] == b );
        BOOST_CHECK_EQUAL( tmp2[2] == c );
        BOOST_CHECK_EQUAL( a == a1 );
        BOOST_CHECK_EQUAL( b == b1 );
        BOOST_CHECK_EQUAL( c == c1 );
        BOOST_CHECK_EQUAL( d == d1 );
        BOOST_CHECK_EQUAL( e == e1 );
        BOOST_CHECK_EQUAL( f == f1 );
        tmp3.assign(d);
        BOOST_CHECK_EQUAL( tmp3[0] == d );
        BOOST_CHECK_EQUAL( tmp3[0] == d );
        BOOST_CHECK_EQUAL( tmp3[0] == d );
        BOOST_CHECK_EQUAL( g == g1 );
        BOOST_CHECK_EQUAL( h == h1 );
        BOOST_CHECK_EQUAL( i == i1 );
    }
    // ---- Concatenate ranges
    {
        boost::array<int,4> array; array.assign(-1);
    	int a=a1, b=b1, c=c1;

        BOOST_CHECK_EQUAL(boost::size(cref_list_of(a)(b)(c).range(array))==7u);
        // TODO:
        //BOOST_CHECK_EQUAL(
        //    boost::size(
        //	    boost::chain(
        //            cref_list_of(a)(b)(c),
        //            array
        //        )
        //    )== 7u
        //);
    }
    // ---- Range conversion
    {
        {
            std::list<int> elems;
            check_converter(elems);
		}
        {
            std::set<int> elems;
            check_converter(elems,true);
		}
        {
            std::vector<int> elems;
            check_converter(elems);
		}
        {
            boost::array<int,8> elems;
            check_converter(elems);
		}
    }
    // ---- Adapters
    {
        {	
            std::stack<int> elems;
            int a=a1, b=b1, c=c1, d=d1, e=e1, f=f1, g=g1, h=h1;
            elems = cref_list_of(a)(b)(c)(d)(e)(f)(g)(h).to_adapter();
	        BOOST_CHECK_EQUAL(elems.top() == h); elems.pop(); 
	        BOOST_CHECK_EQUAL(elems.top() == g); elems.pop(); 
	        BOOST_CHECK_EQUAL(elems.top() == f); elems.pop(); 
	        BOOST_CHECK_EQUAL(elems.top() == e); elems.pop(); 
	        BOOST_CHECK_EQUAL(elems.top() == d); elems.pop(); 
	        BOOST_CHECK_EQUAL(elems.top() == c); elems.pop(); 
	        BOOST_CHECK_EQUAL(elems.top() == b); elems.pop(); 
	        BOOST_CHECK_EQUAL(elems.top() == a); elems.pop(); 
	        BOOST_CHECK_EQUAL(elems.empty() == true);  
        }        
        {	
            std::queue<int> elems;
            int a=a1, b=b1, c=c1, d=d1, e=e1, f=f1, g=g1, h=h1;
            elems = cref_list_of(a)(b)(c)(d)(e)(f)(g)(h).to_adapter();
	        BOOST_CHECK_EQUAL(elems.front() == a); elems.pop(); 
	        BOOST_CHECK_EQUAL(elems.front() == b); elems.pop(); 
	        BOOST_CHECK_EQUAL(elems.front() == c); elems.pop(); 
	        BOOST_CHECK_EQUAL(elems.front() == d); elems.pop(); 
	        BOOST_CHECK_EQUAL(elems.front() == e); elems.pop(); 
	        BOOST_CHECK_EQUAL(elems.front() == f); elems.pop(); 
	        BOOST_CHECK_EQUAL(elems.front() == g); elems.pop(); 
	        BOOST_CHECK_EQUAL(elems.front() == h); elems.pop(); 
	        BOOST_CHECK_EQUAL(elems.empty() == true);  
        }        
    }
}
#include <boost/test/unit_test.hpp>
using boost::unit_test::test_suite;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test = BOOST_TEST_SUITE( "List Test Suite" );

    test->add( BOOST_TEST_CASE( &check_ref_list_of ) );

    return test;
}
