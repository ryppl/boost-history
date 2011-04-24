/**
	@file
	mm_ptr_test2.cpp

	@note
	Memory manager handling heap deallocations in constant time.
*/

#include <iostream>

#include <boost/mm_ptr.hpp>


using namespace std;
using namespace boost;


struct A
{
	int i;
	mm_ptr<A> p;
	
	A(int i = 0) : i(i) 
	{
		std::cout << __FUNCTION__ << ": " << i << std::endl;
	}
	
	~A()
	{
		std::cout << __FUNCTION__ << ": " << i << std::endl;
	}
};


int main()
{
	mm_ptr<A> p = new mm<A>(7);
	mm_ptr<A> q = new mm<A>(8);
	mm_ptr<A> r = new mm<A>(9);

	mm_ptr<void> t = new mm<A>(10);
	mm_ptr<int const volatile> v = new mm<int const volatile>(11);

	p->p = p;
	q = r;
	v = new mm<int const volatile>(12);

	cout << "p->i = " << p->i << endl;
	cout << "q->i = " << q->i << endl;
	cout << "r->i = " << r->i << endl;
	cout << "* v = " << * v << endl;

	// The following don't work with MSVC:
#if ! defined(_MSC_VER)
	mm_ptr<A[5]> s = new mm<A[5]>();
	mm_ptr<char[9]> u = new mm<char[9]>();

	u[4] = 'Z';

	cout << "u[4] = " << u[4] << endl;
#endif

	cout << "Done." << endl;
}
