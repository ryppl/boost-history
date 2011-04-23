/**
	@file
	pack_ptr_test2.cpp

	@note
	Memory manager handling heap deallocations in constant time.
*/

#include <iostream>

#include <boost/pack_ptr.hpp>


using namespace std;
using namespace boost;


struct A
{
	int i;
	pack_ptr<A> p;
	
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
	pack_ptr<A> p = new pack<A>(7);
	pack_ptr<A> q = new pack<A>(8);
	pack_ptr<A> r = new pack<A>(9);

	pack_ptr<void> t = new pack<A>(10);
	pack_ptr<int const volatile> v = new pack<int const volatile>(11);

	p->p = p;
	q = r;
	v = new pack<int const volatile>(12);

	cout << "p->i = " << p->i << endl;
	cout << "q->i = " << q->i << endl;
	cout << "r->i = " << r->i << endl;
	cout << "* v = " << * v << endl;

	// The following don't work with MSVC:
#if ! defined(_MSC_VER)
	pack_ptr<A[5]> s = new pack<A[5]>();
	pack_ptr<char[9]> u = new pack<char[9]>();

	u[4] = 'Z';

	cout << "u[4] = " << u[4] << endl;
#endif

	cout << "Done." << endl;
}
