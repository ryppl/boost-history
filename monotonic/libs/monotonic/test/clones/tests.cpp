// (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// documentation at https://svn.boost.org/svn/boost/sandbox/monotonic/libs/monotonic/doc/index.html
// sandbox at https://svn.boost.org/svn/boost/sandbox/monotonic/

#include <string>
#include <boost/heterogenous/vector.hpp>
#include <boost/monotonic/allocator.hpp>

using namespace std;
using namespace boost;
using namespace heterogenous;

struct derived : base<derived>
{
	int num;
	derived() : num(0) { }
	explicit derived(int n) : num(n) { }
};

struct derived2 : base<derived2>
{
	std::string str;

	derived2() { }
	explicit derived2(std::string const &n) : str(n) { }
};

struct derived3 : base<derived3>
{
	float real;
	int num;
	std::string str;

	derived3() { }
	explicit derived3(float f, int n, std::string const &s) : real(f), num(n), str(s) { }
};

// naive way of allowing reuse in derived types: factor out the implementation
struct derived4_impl
{
};

struct derived4 : derived4_impl, base<derived4>
{
};

struct derived5 : derived4_impl, base<derived5>
{
};

int main()
{
	// there is a problem with static_move_ptr<>
	//typedef heterogenous::vector<std::allocator<int> > vec;
	typedef heterogenous::vector<monotonic::allocator<int> > vec;

	{
		vec bases;
		bases.push_back<derived>(42);
		bases.push_back<derived2>("foo");
		bases.push_back<derived3>(3.14f, -123, "spam");

		BOOST_ASSERT(bases.size() == 3);
		vec copy = bases;
		BOOST_ASSERT(copy.size() == 3);

		common_base &generic0 = copy[0];
		common_base &generic1 = copy[1];
		common_base &generic2 = copy[2];

		derived &p1 = copy.ref_at<derived>(0);
		derived2 *p2 = copy.ptr_at<derived2>(1);
		derived3 *p3 = copy.ptr_at<derived3>(2);
		
		BOOST_ASSERT(p2);
		BOOST_ASSERT(p3);
		
		BOOST_ASSERT(p1.num == 42);
		BOOST_ASSERT(p2->str == "foo");
		BOOST_ASSERT(p3->real == 3.14f);
		BOOST_ASSERT(p3->num == -123);
		BOOST_ASSERT(p3->str == "spam");

		bool caught = false;
		try
		{
			common_base &base = copy.ref_at<derived2>(0);
		}
		catch (std::bad_cast)
		{
			caught = true;
		}
		BOOST_ASSERT(caught);

	}
	monotonic::static_storage<>::release();

	return 0;
}

//EOF
