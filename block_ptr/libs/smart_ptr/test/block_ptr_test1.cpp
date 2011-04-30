/**
	@file
	block_ptr_test2.cpp

	@note
	Memory manager handling heap deallocations in constant time.
*/

#include <list>
#include <iostream>
#include <algorithm>

#include <boost/block_ptr.hpp>
#include <boost/current_function.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>


using namespace std;
using namespace boost;


struct A
{
	static int counter;
	static list<int> order;
	
	int i;
	block_ptr<A> p;
	
	A() : i(counter ++) 
	{
	}
	
	~A()
	{
		order.push_back(i);
	}
};

int A::counter = 0;
list<int> A::order;

BOOST_AUTO_TEST_CASE(block_ptr_test1)
{
	{
		block_ptr<A> p = make_block<A>(); // 0
		block_ptr<A> q = make_block<A>(); // 1
		block_ptr<A> r = make_block<A>(); // 2

		block_ptr<void> t = make_block<A>(); // 3

		p->p = p;
		q = r;

	}
	int order1[] = {1, 3, 2, 0};
	BOOST_CHECK( equal(A::order.begin(), A::order.end(), order1) );
	A::order.clear();

	// The following don't work with MSVC:
#if ! defined(_MSC_VER)
	{
		block_ptr<A[5]> s = make_block<A[5]>(); // 4, 5, 6, 7, 8
	}
	int order2[] = {8, 7, 6, 5, 4};
	BOOST_CHECK( equal(A::order.begin(), A::order.end(), order2) );
	A::order.clear();
#endif

	{
		block_ptr<A> v = make_block<A>(); // 9
		v->p = make_block<A>(); // 10
		v->p->p = make_block<A>(); // 11
		v->p->p->p = v;
	}
	int order3[] = {9, 10, 11};
	BOOST_CHECK( equal(A::order.begin(), A::order.end(), order3) );
	A::order.clear();
}
