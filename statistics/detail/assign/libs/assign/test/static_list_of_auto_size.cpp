// Boost.Assign library
//
//  Copyright Thorsten Ottosen 2003-2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/assign/
//


// This file is simply an override by ER of that by the same name without the
// auto-size suffix name.

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
#include <boost/assign/auto_size/ref_list_of.hpp>
#include <boost/assign/auto_size/ref_rebind_list_of.hpp>
#include <boost/test/test_tools.hpp>

template< class Range >
void print( const Range& r )
{
    std::cout << "\n printing " << typeid(r).name() << " \n"; 
    std::cout << "\n";
    
	// ER_2010    
    typedef typename boost::range_iterator<const Range>::type it_;
    
	// Previously : 
    // for( typename Range::iterator i = r.begin(), e = r.end();

	// ER_2010
    for(it_ i = r.begin(), e = r.end(); 
    	i !=e; ++i )
    std::cout << " " << *i;
}

template< class Range >
void sort( const Range& r )
{
    std::cout << "\n sorting " << typeid(r).name() << " \n"; 
    std::sort( r.begin(), r.end() );
    print( r );
}

template< class Range, class Pred >
void sort( const Range& r, Pred pred )
{
    std::cout << "\n sorting " << typeid(r).name() << " \n"; 
    std::sort( r.begin(), r.end(), pred );
    print( r );
}

template< class Range >
typename Range::const_iterator max_element( const Range& r )
{
    return std::max_element( r.begin(), r.end() );
}


template<typename C>
void check_converter(C& elems){
	using namespace boost::assign;
    int a=1, b=5, c=3, d=4, e=2, f=9, g=0, h=7;
    BOOST_AUTO(tmp,cref_list_of(a)(b)(c)(d)(e)(f)(g)(h));
    elems = tmp;
    elems == tmp;
    typedef typename boost::range_iterator<const C>::type it_;
    it_ it = boost::begin(elems);
    BOOST_CHECK_EQUAL(*it,a); ++it;
    BOOST_CHECK_EQUAL(*it,b); ++it;
    BOOST_CHECK_EQUAL(*it,c); ++it;
    BOOST_CHECK_EQUAL(*it,d); ++it;
    BOOST_CHECK_EQUAL(*it,e); ++it;
    BOOST_CHECK_EQUAL(*it,f); ++it;
    BOOST_CHECK_EQUAL(*it,g); ++it;
    BOOST_CHECK_EQUAL(*it,h); ++it;
}

void check_ref_list_of()
{
    using namespace boost::assign;
	{    
    	int a=1, b=5, c=3, d=4, e=2, f=9, g=0, h=7;
        BOOST_AUTO(tmp,cref_list_of(1)(5)(3)(d)(e)(f)(g)(h));
        BOOST_AUTO(it,boost::begin(tmp));
        BOOST_CHECK_EQUAL(*it,a); it = boost::next(it,7);
        BOOST_CHECK_EQUAL(*it,h); ++it;
        BOOST_CHECK_EQUAL(it,boost::end(tmp));
        BOOST_CHECK_EQUAL( tmp.size(), 8 );
        BOOST_CHECK_EQUAL( tmp.empty(), false );
        BOOST_CHECK_EQUAL( tmp[0], a );
        BOOST_CHECK_EQUAL( tmp[7], h );
        BOOST_CHECK_EQUAL( tmp.front(), a );
        BOOST_CHECK_EQUAL( tmp.back(), h );
    }
	{    
    	int a, b, c, d, e, f, g, h, i;
		{
        	BOOST_AUTO(tmp,ref_list_of(a)(b)(c)(d)(e)(f)(g));
            //ref_list_of(a)(b)(c)(d)(e)(f)(g) = cref_list_of(1)(5)(3)(4)(2)(9)(0)(7);
            BOOST_CHECK_EQUAL(a,1);
            BOOST_CHECK_EQUAL(h,7);
        }
		{
            BOOST_AUTO(tmp,ref_list_of(a)(b)(c));
            tmp.front() = d;
            tmp[1] = e;
            tmp.back() = f;
            BOOST_CHECK_EQUAL( a, d );
            BOOST_CHECK_EQUAL( b, e );
            BOOST_CHECK_EQUAL( c, f );
        }
        {
            BOOST_AUTO(tmp1,ref_list_of(a)(b)(c));
            BOOST_AUTO(tmp2,ref_list_of(g)(h)(i));
            tmp1.assign(0);
            BOOST_CHECK_EQUAL( a, 0 );
            BOOST_CHECK_EQUAL( c, 0 );
            BOOST_CHECK_EQUAL( c, 0 );
            // TODO. Currently ambiguous
            //BOOST_CHECK_EQUAL( 
            //    (ref_list_of(a)(b)(c) == ref_list_of(g)(h)(i)) ,false);
            //BOOST_CHECK_EQUAL( 
            //    ref_list_of(a)(b)(c) == cref_list_of(0)(0)(0) ,true);
            //BOOST_CHECK_EQUAL( 
            //    ref_list_of(a)(b)(c) < ref_list_of(g)(h)(i) ,false);
            //BOOST_CHECK_EQUAL( 
            //    ref_list_of(a)(b)(c) > ref_list_of(g)(h)(i) ,true);
	    }
    }
	{   // ref_rebind_list_of 
    	int a=1, b=5, c=3, d=4, e=2, f=9, g=0, h=7, i=9;
        BOOST_AUTO(tmp,ref_rebind_list_of(a)(b)(c));
        tmp[0] = d;
        tmp[1] = e;
        tmp[2] = f;
        BOOST_CHECK_EQUAL( a, d );
        BOOST_CHECK_EQUAL( b, e );
        BOOST_CHECK_EQUAL( c, f );
		{
            BOOST_AUTO(tmp,ref_rebind_list_of(d)(e)(f));
            tmp[0] = g;
            tmp[1] = h;
            tmp[2] = i;
            BOOST_CHECK_EQUAL( a, g );
            BOOST_CHECK_EQUAL( b, h );
            BOOST_CHECK_EQUAL( c, i );
        }
        // TODO swap
    }
    {
        {
            std::list<int> elems;
            check_converter(elems);
		}
        {
            std::set<int> elems;
            check_converter(elems);
		}
        {
            std::vector<int> elems;
            check_converter(elems);
		}
        {
            boost::array<int,8> elems;
            check_converter(elems);
		}
        {
            boost::array<int,1> elems;
            check_converter(elems);
		}
        {	
            std::stack<int> elems;
            bool fifo = false;
            int a=1, b=5, c=3, d=4, e=2, f=9, g=0, h=7;
            elems = cref_list_of(a)(b)(c)(d)(e)(f)(g)(h).to_adapter();
            int bench = fifo?a:h;
	        BOOST_CHECK_EQUAL(elems.top(),bench); elems.pop(); bench = fifo?b:g;
	        BOOST_CHECK_EQUAL(elems.top(),bench); elems.pop(); bench = fifo?c:f;
	        BOOST_CHECK_EQUAL(elems.top(),bench); elems.pop(); bench = fifo?d:e;
	        BOOST_CHECK_EQUAL(elems.top(),bench); elems.pop(); bench = fifo?e:d;
	        BOOST_CHECK_EQUAL(elems.top(),bench); elems.pop(); bench = fifo?f:c;
	        BOOST_CHECK_EQUAL(elems.top(),bench); elems.pop(); bench = fifo?f:b;
	        BOOST_CHECK_EQUAL(elems.top(),bench); elems.pop(); bench = fifo?h:a;
        }        
        {	
            std::queue<int> elems;
            bool fifo = true;
            int a=1, b=5, c=3, d=4, e=2, f=9, g=0, h=7;
            elems = cref_list_of(a)(b)(c)(d)(e)(f)(g)(h).to_adapter();
            int bench = fifo?a:h;
	        BOOST_CHECK_EQUAL(elems.front(),bench); elems.pop(); bench = fifo?b:g;
	        BOOST_CHECK_EQUAL(elems.front(),bench); elems.pop(); bench = fifo?c:f;
	        BOOST_CHECK_EQUAL(elems.front(),bench); elems.pop(); bench = fifo?d:e;
	        BOOST_CHECK_EQUAL(elems.front(),bench); elems.pop(); bench = fifo?e:d;
	        BOOST_CHECK_EQUAL(elems.front(),bench); elems.pop(); bench = fifo?f:c;
	        BOOST_CHECK_EQUAL(elems.front(),bench); elems.pop(); bench = fifo?f:b;
	        BOOST_CHECK_EQUAL(elems.front(),bench); elems.pop(); bench = fifo?h:a;
        }        
    
    }
}
/*
#include <boost/test/unit_test.hpp>
using boost::unit_test::test_suite;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test = BOOST_TEST_SUITE( "List Test Suite" );

    test->add( BOOST_TEST_CASE( &check_static_list_of_auto_size ) );

    return test;
}
*/