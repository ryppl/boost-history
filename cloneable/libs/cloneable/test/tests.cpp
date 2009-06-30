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

#define BOOST_HETEROGENOUS
#include <boost/monotonic/allocator.hpp>
#include <boost/monotonic/local.hpp>

#include <boost/cloneable/vector.hpp>
#include <boost/cloneable/map.hpp>
#include <boost/cloneable/adaptor.hpp>
#include <boost/cloneable/allocator.hpp>
#include <boost/bind.hpp>
#include <boost/any.hpp>
#include <boost/variant.hpp>

#define BOOST_TEST_MODULE basic_test test
#include <boost/test/unit_test.hpp>
#include <boost/timer.hpp>

using namespace std;
using namespace boost;
using namespace cloneable;

namespace basic_test
{
	struct my_base { virtual ~my_base() { } };
	struct T0 : cloneable::base<T0, my_base> {};
	struct T1 : cloneable::base<T1, my_base> {};
	struct T2 : cloneable::base<T2, my_base> {};
	struct T3 : cloneable::base<T3, my_base> {};

	template <class T>
	T *clone(const my_base &base)
	{
		return dynamic_cast<const abstract_base<my_base> &>(base).clone_as<T>();
	}
}

template <class T, class Base>
T *clone(const cloneable::abstract_base<Base> &base)
{
	return base.clone_as<T>();
}

BOOST_AUTO_TEST_CASE(test_basic)
{
	using namespace basic_test;

	// basic cloning
	T0 *t0 = new T0();
	T0 *t0_clone = dynamic_cast<T0 *>(t0->clone());
	BOOST_ASSERT(typeid(*t0_clone) == typeid(T0));

	// cloning from an abstract_base_type, and also using clone_as
	T1::abstract_base_type *t1_base = new T1();
	T1 *t1_clone = t1_base->clone_as<T1>();
	BOOST_ASSERT(typeid(*t1_clone) == typeid(T1));

	// use a free-function from a generalised abstract_base
	T2 *t2 = new T2;
	T2 *t2_clone = clone<T2>(*t2);
	BOOST_ASSERT(typeid(*t2_clone) == typeid(T2));

	// use a free-function from a specific base
	my_base *t3 = new T3;
	T3 *t3_clone = basic_test::clone<T3>(*t3);
	BOOST_ASSERT(typeid(*t3_clone) == typeid(T3));

	delete t0;
	delete t0_clone;
	delete t1_base;
	delete t1_clone;
	delete t2;
	delete t2_clone;
	delete t3;
	delete t3_clone;
}

namespace mulitple_inheritance_test
{
	struct Q0 : base<Q0>
	{
		int num;
		Q0(int n = 0) : num(n) { }
	};

	/// derive from Q0, which is also cloneable<>
	struct Q1 : Q0, base<Q1>
	{
		string str;
		Q1() { }
		Q1(const char * t) : str(t) { }
	};
	
	struct my_region { };
}

BOOST_AUTO_TEST_CASE(test_clones)
{
	using namespace mulitple_inheritance_test;

	// just for spice, use a regionalised monotonic allocator for creating
	// originals, and producing clones.
	// at least this way we don't have to free anything...
	monotonic::local<my_region> local;
	monotonic::allocator<int,my_region> alloc = local.make_allocator<int>();

	Q0 *q0 = create<Q0>(alloc);
	BOOST_ASSERT(typeid(*q0) == typeid(Q0));

	Q0 *q0_c = q0->clone_as<Q0>(alloc);
	BOOST_ASSERT(typeid(*q0_c) == typeid(Q0));

	Q1 *q1 = create<Q1>(alloc);
	BOOST_ASSERT(typeid(*q1) == typeid(Q1));

	Q0 *q1_c0  = q1->clone_as<Q0>(alloc);
	BOOST_ASSERT(typeid(*q1_c0) == typeid(Q0));

	Q1 *q1_c1  = q1->clone_as<Q1>(alloc);
	BOOST_ASSERT(typeid(*q1_c1) == typeid(Q1));
}	

namespace custom_clone_test
{
	struct T0 : base<T0>
	{
		bool custom_cloned;
		T0() : custom_cloned(false) { }

		/// override the means to create a copy from abstract_base<>
		T0 *make_copy(abstract_allocator &alloc) const
		{
			T0 *ptr = create<T0>(alloc);
			ptr->custom_cloned = true;
			return ptr;
		}
	};
}

BOOST_AUTO_TEST_CASE(test_custom_clone)
{
	using namespace custom_clone_test;
	T0 *p = new T0;
	T0 *q = p->clone_as<T0>();
	BOOST_ASSERT(q && q->custom_cloned);
	delete p;
	delete q;
}

struct my_base
{
	virtual ~my_base() { }
};

/// some external type that we cannot change
struct external_type
{
	std::string text;
	external_type() { }
	external_type(const char *T) : text(T) { }
};

/// make an adaptor type, which makes `external_type` cloneable
typedef cloneable::adaptor<external_type, my_base> cloneable_external_type;

BOOST_AUTO_TEST_CASE(test_external_types)
{
	//  for spice, use stack for storage
	monotonic::storage<> storage;
	monotonic::allocator<int> alloc(storage);

	cloneable_external_type *ext = create<cloneable_external_type>(alloc);
	BOOST_ASSERT(typeid(*ext) == typeid(cloneable_external_type));

	cloneable_external_type *clone = ext->clone_as<cloneable_external_type>(alloc);
	BOOST_ASSERT(typeid(*clone) == typeid(cloneable_external_type));
}

//struct custom_external_cloneable : adaptor<external_type, my_base>
//{
//	custom_external_cloneable *clone(abstract_allocator &alloc) const
//	{
//		custom_external_cloneable *ptr = create<custom_external_cloneable>(alloc);
//		ptr->text = "custom_cloned";
//		return ptr;
//	}
//};
//
//BOOST_AUTO_TEST_CASE(test_custom_external_clone)
//{
//	custom_external_cloneable *p = new custom_external_cloneable;
//	custom_external_cloneable *q = p->clone_as<custom_external_cloneable>();
//	BOOST_ASSERT(typeid(*q) == typeid(custom_external_cloneable));
//}

BOOST_AUTO_TEST_CASE(test_multiple_inheritance_vector)
{
	using namespace mulitple_inheritance_test;
	typedef cloneable::vector<> vec;
	
	vec v;
	v.emplace_back<Q0>(42);
	v.emplace_back<Q1>("foo");

	// ensure duplication of types with multiple cloneable sub-objects works correctly
	vec v2 = v;
	
	BOOST_ASSERT(v2.ref_at<Q0>(0).num == 42);
	BOOST_ASSERT(v2.ref_at<Q1>(1).str == "foo");
}

struct T0 : base<T0, my_base>
{
	int num;
	T0() : num(0) { }
	T0(int n) : num(n) { }
};

struct T1 : base<T1, my_base>
{
	std::string str;

	T1() { }
	T1(std::string const &n) : str(n) { }
};

struct T2 : base<T2, my_base>
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

BOOST_AUTO_TEST_CASE(test_vector)
{
	// this uses the base type for the contained elements as a region tag for a monotonic allocator.
	// totally unnecessary, could just use a std::allocator, but this way gives more control
	typedef cloneable::vector<my_base, monotonic::allocator<my_base, my_base> > vec;

	// use a local scoped object to automatically release resources used by the my_base monotonic 
	// storage region on function exit.
	monotonic::local<my_base> local;
	{
		vec bases;

		// type of thing to insert must be passed explicitly, and must derive from common_base.
		// arguments to push_back are passed directly to ctor
		bases.emplace_back<T0>(42);						
		bases.emplace_back<T1>("foo");
		bases.emplace_back<T2>(3.14f, -123, "spam");
		bases.emplace_back<cloneable_external_type>("external");

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
		BOOST_ASSERT(p3->real == 3.14f);
		BOOST_ASSERT(p3->num == -123);
		BOOST_ASSERT(p3->str == "spam");
		BOOST_ASSERT(copy.ref_at<cloneable_external_type>(3).text == "external");

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

namespace map_test
{
	struct my_base
	{
		int number;
		my_base(int n = 0) : number(n) { }
		virtual ~my_base() { }
	};

	struct M0 : base<M0, my_base>
	{
		M0(int n = 0) : my_base(n) {}
	};

	struct M1 : base<M1, my_base>
	{
		string str;
		M1() { }
		M1(const char *s) : str(s) { }
	};

	struct M2 : base<M2, my_base>
	{
	};

	struct M3 : base<M3, my_base>
	{
	};

	struct my_less
	{
		bool operator()(my_base const *left, my_base const *right) const
		{
			return left->number < right->number;
		}
	};
}

BOOST_AUTO_TEST_CASE(test_map)
{
	using namespace map_test;
	typedef cloneable::map<map_test::my_base,my_less> map_type;
	map_type map;
	map .key<M0>(42).value<M1>("foo")
		.key<M2>().value<M3>()
		;
	M0 *m0 = create<M0>(map.get_allocator(), 42);
	map_type::iterator iter = map.find(m0);
	BOOST_ASSERT(iter!= map.end());
	M1 *m1 = dynamic_cast<M1 *>(iter->second);
	BOOST_ASSERT(m1 != 0);
	BOOST_ASSERT(m1->str == "foo");
}

BOOST_AUTO_TEST_CASE(test_hash)
{
	using namespace map_test;
	M0 a, b;
	BOOST_ASSERT(a.hash() != b.hash());
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

//EOF
