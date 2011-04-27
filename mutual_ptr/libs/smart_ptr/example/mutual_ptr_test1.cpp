/**
	@file
	mutual_ptr_test2.cpp

	@note
	Memory manager handling heap deallocations in constant time.
*/

#include <iostream>

#include <boost/mutual_ptr.hpp>


using namespace std;
using namespace boost;


struct A
{
	int i;
	mutual_ptr<A> p;
	
	A(int i = 0) : i(i) 
	{
		cout << __FUNCTION__ << ": " << i << endl;
	}
	
	~A()
	{
		cout << __FUNCTION__ << ": " << i << endl;
	}
};


int main()
{
	cout << "Cyclicism:" << endl;
	{
		mutual_ptr<A> p = new mutual<A>(7);
		mutual_ptr<A> q = new mutual<A>(8);
		mutual_ptr<A> r = new mutual<A>(9);

		mutual_ptr<void> t = new mutual<A>(10);
		mutual_ptr<int const volatile> v = new mutual<int const volatile>(11);

		p->p = p;
		q = r;
		v = new mutual<int const volatile>(12);

		cout << "p->i = " << p->i << endl;
		cout << "q->i = " << q->i << endl;
		cout << "r->i = " << r->i << endl;
		cout << "* v = " << * v << endl;
	}
	cout << endl;

	// The following don't work with MSVC:
#if ! defined(_MSC_VER)
	cout << "Array access:" << endl;
	{
		mutual_ptr<A[5]> s = new mutual<A[5]>();
		mutual_ptr<char[9]> u = new mutual<char[9]>();

		u[4] = 'Z';

		cout << "u[4] = " << u[4] << endl;
	}
	cout << endl;
#endif

	cout << "Order of destruction:" << endl;
	{
		mutual_ptr<A> v = new mutual<A>(0);
		v->p = new mutual<A>(1);
		v->p->p = new mutual<A>(2);
		v->p->p->p = v;
	}
	cout << endl;
}
