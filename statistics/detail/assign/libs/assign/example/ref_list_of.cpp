//////////////////////////////////////////////////////////////////////////////
// example::ref_list_of.h                                                   //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/detail/workaround.hpp>
#include <libs/assign/test/ref_list_of.h>
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
//#include <boost/range/chain.hpp>//Uncomment when in release
#include <boost/assign/auto_size/ref_list_of.hpp>
#include <boost/assign/auto_size/ref_rebind_list_of.hpp>
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
        BOOST_ASSERT(*it == a); ++it;
        BOOST_ASSERT(*it == b); ++it;
        BOOST_ASSERT(*it == c); ++it;
        BOOST_ASSERT(*it == d); ++it;
        BOOST_ASSERT(*it == e); ++it;
        BOOST_ASSERT(*it == f); ++it;
        BOOST_ASSERT(*it == g); ++it;
        BOOST_ASSERT(*it == h); ++it;
        BOOST_ASSERT(it == boost::end(elems)); 
    }else{
        BOOST_ASSERT(*it == 0); ++it;
        BOOST_ASSERT(*it == 1); ++it;
        BOOST_ASSERT(*it == 2); ++it;
        BOOST_ASSERT(*it == 3); ++it;
        BOOST_ASSERT(*it == 4); ++it;
        BOOST_ASSERT(*it == 5); ++it;
        BOOST_ASSERT(*it == 7); ++it;
        BOOST_ASSERT(*it == 9); ++it;
        BOOST_ASSERT(it == boost::end(elems));
    }
}

void example_ref_list_of(std::ostream& os)
{
    os << "->example_ref_list_of : ";

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
            BOOST_ASSERT( max == f );
            max = 8;
            BOOST_ASSERT( f == 8 );
            const int& const_max = *max_element( 
                cref_list_of(1)(5)(3)(d)(e)(f)(g)(h) );
            BOOST_ASSERT( max == const_max );
        }
        {
            int a=a1, b=b1, c=c1, d=d1, e=e1, f=f1;
            BOOST_AUTO(tmp1,ref_rebind_list_of(a)(b)(c));
            BOOST_AUTO(tmp2,ref_rebind_list_of(d)(e)(f));
            tmp1.swap(tmp2);
            BOOST_ASSERT(tmp1[1]==e);
            BOOST_ASSERT(tmp2[1]==b);
            
        }
    }
    // ---- Copy semantics
	{   // As right operand (iterators + array)
    	int a=a1, d=d1, e=e1, f=f1, g=g1, h=h1;
        BOOST_AUTO(tmp,cref_list_of(1)(5)(3)(d)(e)(f)(g)(h));
        BOOST_AUTO(it,boost::begin(tmp));
        BOOST_ASSERT(*it == a); it = boost::next(it,7);
        BOOST_ASSERT(*it == h); ++it;
        BOOST_ASSERT(it == boost::end(tmp));
        BOOST_ASSERT( tmp.size() == 8 );
        BOOST_ASSERT( tmp.empty() == false );
        BOOST_ASSERT( tmp[0] == a );
        BOOST_ASSERT( tmp[7] == h );
        BOOST_ASSERT( tmp.front() == a );
        BOOST_ASSERT( tmp.back() == h );
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
        BOOST_ASSERT(a == a1);
        BOOST_ASSERT(b == b1);
        BOOST_ASSERT(c == c1);
        BOOST_ASSERT(d == d1);
        BOOST_ASSERT(e == e1);
        BOOST_ASSERT(f == f1);
        BOOST_ASSERT(g == g1);
        BOOST_ASSERT(h == h1);
        BOOST_ASSERT(i == i1);
    }
	{   // As left operand (array)  
    	int a=a1, b=b1, c=c1, 
            d=d1, e=e1, f=f1, 
            g=g1, h=h1, i=i1;
        BOOST_AUTO(tmp1,ref_list_of(a)(b)(c));
        BOOST_AUTO(tmp2,ref_list_of(d)(e)(f));
        BOOST_AUTO(tmp3,ref_list_of(g)(h)(i));
        tmp1.swap(tmp2);
        BOOST_ASSERT( a == d1 );
        BOOST_ASSERT( b == e1 );
        BOOST_ASSERT( c == f1 );
        BOOST_ASSERT( d == a1 );
        BOOST_ASSERT( e == b1 );
        BOOST_ASSERT( f == c1 );
        tmp3.assign(0);
        BOOST_ASSERT( g == 0 );
        BOOST_ASSERT( h == 0 );
        BOOST_ASSERT( i == 0 );
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
        BOOST_ASSERT( tmp1[0] == d );
        BOOST_ASSERT( tmp1[1] == e );
        BOOST_ASSERT( tmp1[2] == f );
        BOOST_ASSERT( tmp2[0] == a );
        BOOST_ASSERT( tmp2[1] == b );
        BOOST_ASSERT( tmp2[2] == c );
        BOOST_ASSERT( a == a1 );
        BOOST_ASSERT( b == b1 );
        BOOST_ASSERT( c == c1 );
        BOOST_ASSERT( d == d1 );
        BOOST_ASSERT( e == e1 );
        BOOST_ASSERT( f == f1 );
        tmp3.assign(d);
        BOOST_ASSERT( tmp3[0] == d );
        BOOST_ASSERT( tmp3[0] == d );
        BOOST_ASSERT( tmp3[0] == d );
        BOOST_ASSERT( g == g1 );
        BOOST_ASSERT( h == h1 );
        BOOST_ASSERT( i == i1 );
    }
    // ---- Concatenate ranges
    {
        boost::array<int,4> array; array.assign(-1);
    	int a=a1, b=b1, c=c1;

        BOOST_ASSERT(boost::size(cref_list_of(a)(b)(c).range(array))==7u);
        // Uncomment when range_ex is in the release
        //BOOST_ASSERT(
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
	        BOOST_ASSERT(elems.top() == h); elems.pop(); 
	        BOOST_ASSERT(elems.top() == g); elems.pop(); 
	        BOOST_ASSERT(elems.top() == f); elems.pop(); 
	        BOOST_ASSERT(elems.top() == e); elems.pop(); 
	        BOOST_ASSERT(elems.top() == d); elems.pop(); 
	        BOOST_ASSERT(elems.top() == c); elems.pop(); 
	        BOOST_ASSERT(elems.top() == b); elems.pop(); 
	        BOOST_ASSERT(elems.top() == a); elems.pop(); 
	        BOOST_ASSERT(elems.empty() == true);  
        }        
        {	
            std::queue<int> elems;
            int a=a1, b=b1, c=c1, d=d1, e=e1, f=f1, g=g1, h=h1;
            elems = cref_list_of(a)(b)(c)(d)(e)(f)(g)(h).to_adapter();
	        BOOST_ASSERT(elems.front() == a); elems.pop(); 
	        BOOST_ASSERT(elems.front() == b); elems.pop(); 
	        BOOST_ASSERT(elems.front() == c); elems.pop(); 
	        BOOST_ASSERT(elems.front() == d); elems.pop(); 
	        BOOST_ASSERT(elems.front() == e); elems.pop(); 
	        BOOST_ASSERT(elems.front() == f); elems.pop(); 
	        BOOST_ASSERT(elems.front() == g); elems.pop(); 
	        BOOST_ASSERT(elems.front() == h); elems.pop(); 
	        BOOST_ASSERT(elems.empty() == true);  
        }        
    }
    os << "<-" << std::endl;
}
