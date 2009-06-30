// (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// documentation at https://svn.boost.org/svn/boost/sandbox/monotonic/libs/monotonic/doc/index.html
// sandbox at https://svn.boost.org/svn/boost/sandbox/monotonic/

#include <string>
#include <iostream>
#include <boost/heterogenous/vector.hpp>
#include <boost/heterogenous/map.hpp>
#include <boost/bind.hpp>
#include <boost/any.hpp>
#include <boost/variant.hpp>

using namespace std;
using namespace boost;
using namespace heterogenous;

struct my_base
{
	virtual ~my_base() { }
};

struct derived : base<derived, my_base>
{
	int num;
	derived() : num(0) { }
	derived(int n) : num(n) { }
};

struct derived2 : base<derived2, my_base>
{
	std::string str;

	derived2() { }
	derived2(std::string const &n) : str(n) { }
};

struct derived3 : base<derived3, my_base>
{
	float real;
	int num;
	std::string str;

	derived3() { }
	derived3(float f, int n, std::string const &s) : real(f), num(n), str(s) { }

	void print() const
	{
		cout << "derived3: " << real << ", " << num << ", " << str << endl;
	}
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

void test_any();
void test_variant();

void test_map();


	namespace test
	{
        using namespace heterogenous;
		struct my_base
		{
			virtual ~my_base() { }
		};

		struct T0 : base<T0, my_base> { };
		struct T1 : base<T1, my_base> { };

		void run()
		{
			typedef heterogenous::vector<my_base> vec;
			vec v0;
			v0.emplace_back<T0>();
			v0.emplace_back<T1>();
			vec v1 = v0;
			my_base &whatever = v1[0];
			assert(v1.ptr_at<T1>(1));
		}
	}

int main()
{
	test::run();

	test_any();
	test_variant();
	test_map();




	// a 'heterogenous' container of objects of any type that derives from common_base
	typedef heterogenous::vector<my_base> vec;

	{
		vec bases;

		// type of thing to insert must be passed explicitly, and must derive from common_base.
		// arguments to push_back are passed directly to ctor
		bases.emplace_back<derived>(42);						
		bases.emplace_back<derived2>("foo");
		bases.emplace_back<derived3>(3.14f, -123, "spam");

		// perform functor on each contained object of the given type
		bases.for_each<derived3>(boost::bind(&derived3::print, _1));

		BOOST_ASSERT(bases.size() == 3);

		// does a deep copy, preserving concrete types
		vec copy = bases;

		BOOST_ASSERT(copy.size() == 3);

		// each object in the container can be retrieved generically as a default_base_type
		my_base &generic0 = copy[0];
		my_base &generic1 = copy[1];
		my_base &generic2 = copy[2];

		// get a reference; will throw bad_cast on type mismatch
		derived &p1 = copy.ref_at<derived>(0);

		// get a pointer; returns null on type mismatch
		derived2 *p2 = copy.ptr_at<derived2>(1);
		derived3 *p3 = copy.ptr_at<derived3>(2);
		
		BOOST_ASSERT(p2);
		BOOST_ASSERT(p3);
		
		BOOST_ASSERT(p1.num == 42);
		BOOST_ASSERT(p2->str == "foo");
		BOOST_ASSERT(p3->real == 3.14f);BOOST_ASSERT(p3->num == -123);BOOST_ASSERT(p3->str == "spam");

		bool caught = false;
		try
		{
			my_base &base = copy.ref_at<derived2>(0);
		}
		catch (std::bad_cast)
		{
			caught = true;
		}
		BOOST_ASSERT(caught);

	}
	return 0;
}

void test_any()
{
	// this works, after changing boost::any<> to take an allocator type argument
	typedef any<monotonic::allocator<char> > any_type;
	typedef std::vector<any_type, monotonic::allocator<any_type> > vec;
	vec v;

	// an issue here is that instances are copy-constructed into the container.
	// another issue is that this is effectively typeless.
	// but, types added do not have to derive from anything in order for duplication to work.
	v.push_back(derived(42));
	v.push_back(derived2("foo"));

	vec v2 = v;

	BOOST_ASSERT(any_cast<derived2 &>(v2[1]).str == "foo");
}

void test_variant()
{
	// need to declare all the possible types that could be used at the point of declaration
	typedef variant<derived, derived2, derived3> var;
	typedef std::vector<var, monotonic::allocator<var> > vec;
	vec v0;
	v0.push_back(derived(42));
	v0.push_back(derived2("foo"));
	vec v1 = v0;
	BOOST_ASSERT(boost::get<derived2>(v1[1]).str == "foo");

}

struct my_base2
{
	int number;
	my_base2(int n = 0) : number(n) { }
	virtual ~my_base2() { }
};

struct T0 : heterogenous::base<T0, my_base2>
{
};

struct T1 : heterogenous::base<T1, my_base2>
{
};

struct T2 : heterogenous::base<T2, my_base2>
{
};

struct T3 : heterogenous::base<T3, my_base2>
{
};

struct my_less
{
	bool operator()(my_base2 const *left, my_base2 const *right) const
	{
		return left->number < right->number;
	}
};

void test_map()
{
	heterogenous::map<my_less, my_base2> map;

	map .key<T0>().value<T1>()
		.key<T2>().value<T3>()
		;

}

//EOF
