// (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// documentation at https://svn.boost.org/svn/boost/sandbox/monotonic/libs/monotonic/doc/index.html
// sandbox at https://svn.boost.org/svn/boost/sandbox/monotonic/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <string>
#include <iostream>
#include <boost/monotonic/local.hpp>
#include <boost/heterogenous/vector.hpp>
#include <boost/heterogenous/map.hpp>
#include <boost/heterogenous/adaptor.hpp>
#include <boost/bind.hpp>
#include <boost/any.hpp>
#include <boost/variant.hpp>

#define BOOST_TEST_MODULE basic_test test
#include <boost/test/unit_test.hpp>
#include <boost/timer.hpp>

using namespace std;
using namespace boost;
using namespace heterogenous;

struct my_base
{
	virtual ~my_base() { }
};

struct T0 : cloneable<T0, my_base>
{
	int num;
	T0() : num(0) { }
	T0(int n) : num(n) { }
};

struct T1 : cloneable<T1, my_base>
{
	std::string str;

	T1() { }
	T1(std::string const &n) : str(n) { }
};

struct T2 : cloneable<T2, my_base>
{
	float real;
	int num;
	std::string str;

	T2() { }
	T2(float f, int n, std::string const &s) : real(f), num(n), str(s) { }

	void print() const
	{
		cout << "derived3: " << real << ", " << num << ", " << str << endl;
	}
};

namespace mi_test
{

	struct Q0 : cloneable<Q0>
	{
		int num;
		Q0(int n = 0) : num(n) { }
	};

	/// derive from Q1, which is also cloneable<>
	struct Q1 : Q0, cloneable<Q1>
	{
		string s;
		Q1() { }
		Q1(string t) : s(t) { }
	};
	
	struct my_region { };

}


BOOST_AUTO_TEST_CASE(test_clones)
{
	using namespace mi_test;
	
	monotonic::local<my_region> local;
	monotonic::allocator<int,my_region> alloc = local.make_allocator<int>();

	Q0 *q0 = create<Q0>(alloc);
	BOOST_ASSERT(typeid(*q0) == typeid(Q0));

	Q0 *q0_c = dynamic_cast<Q0 *>(q0->clone(alloc));
	BOOST_ASSERT(typeid(*q0_c) == typeid(Q0));

	Q1 *q1 = create<Q1>(alloc);
	BOOST_ASSERT(typeid(*q1) == typeid(Q1));

	Q0 *q1_c0  = dynamic_cast<Q0 *>(q1->clone_as<Q0>(alloc));
	BOOST_ASSERT(typeid(*q1_c0) == typeid(Q0));

	Q1 *q1_c1  = dynamic_cast<Q1 *>(q1->clone_as<Q1>(alloc));
	BOOST_ASSERT(typeid(*q1_c1) == typeid(Q1));

}	

BOOST_AUTO_TEST_CASE(test_multiple_inheritance)
{
	using namespace mi_test;
	typedef heterogenous::vector<> vec;
	vec v;

	v.emplace_back<Q0>(42);
	v.emplace_back<Q1>("foo");

	vec v2 = v;
	BOOST_ASSERT(v2.ref_at<Q1>(1).s == "foo");

}

namespace test
{
    using namespace heterogenous;
	struct my_base
	{
		virtual ~my_base() { }
	};

	struct T0 : cloneable<T0, my_base> { };
	struct T1 : cloneable<T1, my_base> { };

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

/// some external type that we cannot change
struct ExternalType
{
	std::string text;
	ExternalType() { }
	ExternalType(const char *T) : text(T) { }
};

/// make an adaptor type, which makes `ExternalType` heterogenous
typedef heterogenous::adaptor<ExternalType, my_base> ExternalType_;

BOOST_AUTO_TEST_CASE(test_vector)
{
	// a 'heterogenous' container of objects of any type that derives from common_base
	typedef heterogenous::vector<my_base> vec;

	{
		vec bases;

		// type of thing to insert must be passed explicitly, and must derive from common_base.
		// arguments to push_back are passed directly to ctor
		bases.emplace_back<T0>(42);						
		bases.emplace_back<T1>("foo");
		bases.emplace_back<T2>(3.14f, -123, "spam");
		bases.emplace_back<ExternalType_>("external");

		// perform functor on each contained object of the given type
		bases.for_each<T2>(boost::bind(&T2::print, _1));

		// does a deep copy, preserving concrete types
		vec copy = bases;

		// each object in the container can be retrieved generically as a default_base_type
		my_base &generic0 = copy[0];
		my_base &generic1 = copy[1];
		my_base &generic2 = copy[2];

		// get a reference; will throw bad_cast on type mismatch
		T0 &p1 = copy.ref_at<T0>(0);

		// get a pointer; returns null on type mismatch
		T1 *p2 = copy.ptr_at<T1>(1);
		T2 *p3 = copy.ptr_at<T2>(2);
		
		BOOST_ASSERT(p2);
		BOOST_ASSERT(p3);
		
		BOOST_ASSERT(p1.num == 42);
		BOOST_ASSERT(p2->str == "foo");
		BOOST_ASSERT(p3->real == 3.14f);BOOST_ASSERT(p3->num == -123);BOOST_ASSERT(p3->str == "spam");
		BOOST_ASSERT(copy.ref_at<ExternalType_>(3).text == "external");

		bool caught = false;
		try
		{
			my_base &base = copy.ref_at<T1>(0);
		}
		catch (std::bad_cast)
		{
			caught = true;
		}
		BOOST_ASSERT(caught);

	}
}

BOOST_AUTO_TEST_CASE(test_any)
{
	// this works, after changing boost::any<> to take an allocator type argument
	typedef any<monotonic::allocator<char> > any_type;
	typedef std::vector<any_type, monotonic::allocator<any_type> > vec;
	vec v;

	// an issue here is that instances are copy-constructed into the container.
	// another issue is that this is effectively typeless.
	// but, types added do not have to derive from anything in order for duplication to work.
	v.push_back(T0(42));
	v.push_back(T1("foo"));

	vec v2 = v;

	BOOST_ASSERT(any_cast<T1 &>(v2[1]).str == "foo");
}

BOOST_AUTO_TEST_CASE(test_variant)
{
	// need to declare all the possible types that could be used at the point of declaration
	typedef variant<T0, T1, T2> var;
	typedef std::vector<var, monotonic::allocator<var> > vec;
	vec v0;
	v0.push_back(T0(42));
	v0.push_back(T1("foo"));
	vec v1 = v0;
	BOOST_ASSERT(boost::get<T1>(v1[1]).str == "foo");
}

struct my_base2
{
	int number;
	my_base2(int n = 0) : number(n) { }
	virtual ~my_base2() { }
};

struct M0 : heterogenous::cloneable<M0, my_base2>
{
};

struct M1 : heterogenous::cloneable<M1, my_base2>
{
};

struct M2 : heterogenous::cloneable<M2, my_base2>
{
};

struct M3 : heterogenous::cloneable<M3, my_base2>
{
};

struct my_less
{
	bool operator()(my_base2 const *left, my_base2 const *right) const
	{
		return left->number < right->number;
	}
};

BOOST_AUTO_TEST_CASE(test_map)
{
	heterogenous::map<my_base2,my_less> map;

	map .key<M0>().value<M1>()
		.key<M2>().value<M3>()
		;

}

//EOF
