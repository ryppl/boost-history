/**
	@file
	set_ptr_test2.cpp

	@note
	Memory manager handling heap deallocations in constant time.
*/

#include <iostream>

#include <boost/set_ptr.hpp>


using namespace std;
using namespace boost;


struct A
{
	int i;
	set_ptr<A> p;
	
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
	set_ptr<A> p = new set<A>(7);
	set_ptr<A> q = new set<A>(8);
	set_ptr<A> r = new set<A>(9);

	set_ptr<void> t = new set<A>(10);
	set_ptr<int const volatile> v = new set<int const volatile>(11);

	p->p = p;
	q = r;
	v = new set<int const volatile>(12);

	cout << "p->i = " << p->i << endl;
	cout << "q->i = " << q->i << endl;
	cout << "r->i = " << r->i << endl;
	cout << "* v = " << * v << endl;

	// The following don't work with MSVC:
#if ! defined(_MSC_VER)
	set_ptr<A[5]> s = new set<A[5]>();
	set_ptr<char[9]> u = new set<char[9]>();

	u[4] = 'Z';

	cout << "u[4] = " << u[4] << endl;
#endif

	cout << "Done." << endl;
}
