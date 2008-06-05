/**
	@file
	shifted_ptr_test2.cpp

	@note
	Memory manager handling heap deallocations in constant time.
*/

#include <iostream>

#include <boost/shifted_ptr.hpp>


using namespace std;
using namespace boost;


struct A
{
	int i;
	shifted_ptr<A> p;
	
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
	shifted_ptr<A> p = new shifted<A>(7);
	shifted_ptr<A> q = new shifted<A>(8);
	shifted_ptr<A> r = new shifted<A>(9);

	shifted_ptr<void> t = new shifted<A>(10);
	shifted_ptr<int const volatile> v = new shifted<int const volatile>(11);

	p->p = p;
	q = r;
	v = new shifted<int const volatile>(12);

	cout << "p->i = " << p->i << endl;
	cout << "q->i = " << q->i << endl;
	cout << "r->i = " << r->i << endl;
	cout << "* v = " << * v << endl;

	// The following don't work with MSVC:
#if ! defined(_MSC_VER)
	shifted_ptr<A[5]> s = new shifted<A[5]>();
	shifted_ptr<char[9]> u = new shifted<char[9]>();

	u[4] = 'Z';

	cout << "u[4] = " << u[4] << endl;
#endif

	cout << "Done." << endl;
}
