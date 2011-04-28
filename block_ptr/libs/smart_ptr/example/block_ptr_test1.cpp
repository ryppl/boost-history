/**
	@file
	block_ptr_test2.cpp

	@note
	Memory manager handling heap deallocations in constant time.
*/

#include <iostream>

#include <boost/block_ptr.hpp>


using namespace std;
using namespace boost;


struct A
{
	int i;
	block_ptr<A> p;
	
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
		block_ptr<A> p = new block<A>(7);
		block_ptr<A> q = new block<A>(8);
		block_ptr<A> r = new block<A>(9);

		block_ptr<void> t = new block<A>(10);
		block_ptr<int const volatile> v = new block<int const volatile>(11);

		p->p = p;
		q = r;
		v = new block<int const volatile>(12);

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
		block_ptr<A[5]> s = new block<A[5]>();
		block_ptr<char[9]> u = new block<char[9]>();

		u[4] = 'Z';

		cout << "u[4] = " << u[4] << endl;
	}
	cout << endl;
#endif

	cout << "Order of destruction:" << endl;
	{
		block_ptr<A> v = new block<A>(0);
		v->p = new block<A>(1);
		v->p->p = new block<A>(2);
		v->p->p->p = v;
	}
	cout << endl;
}
