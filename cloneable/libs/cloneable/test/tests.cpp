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

#include <boost/bind.hpp>
#include <boost/any.hpp>
#include <boost/variant.hpp>
#include <boost/function.hpp>

#define BOOST_HETEROGENOUS
#include <boost/monotonic/allocator.hpp>
#include <boost/monotonic/local.hpp>

#include <boost/cloneable/clone.hpp>
#include <boost/cloneable/vector.hpp>
#include <boost/cloneable/list.hpp>
#include <boost/cloneable/map.hpp>
#include <boost/cloneable/set.hpp>
#include <boost/cloneable/heterogenous_map.hpp>

#include <boost/cloneable/adaptor.hpp>

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
		return dynamic_cast<T *>(dynamic_cast<const abstract_base<my_base> &>(base).clone());
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

	// cloning from a raw base type
	my_base *t0_base = new T0();
	T0 *t0_base_clone = cloneable::clone_as<T0>(*t0_base);
	BOOST_ASSERT(t0_base_clone != 0);

	// make a new object from an existing base
	my_base *t0_new = cloneable::create_new(*t0_base);
	BOOST_ASSERT(t0_new && typeid(*t0_new) == typeid(T0));

	delete t0_base;
	delete t0_base_clone;
	delete t0_new;

	// cloning from an abstract_base_type
	BOOST_STATIC_ASSERT((is_same<my_base, T1::base_type>::value));
	BOOST_STATIC_ASSERT((is_same<cloneable::abstract_base<my_base>, T1::abstract_base_type>::value));
	T1::abstract_base_type *t1_base = new T1();

	// query for supported clone types
	BOOST_ASSERT(t1_base->can_clone_as<T1>());
	BOOST_ASSERT(!t1_base->can_clone_as<T2>());

	T1 *t1_clone = t1_base->clone_as<T1>();
	BOOST_ASSERT(t1_clone != 0);

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
	
	struct W : base<W> { };

	struct Q2 : W, Q1, base<Q2>
	{
	};

	struct my_region { };
}

BOOST_AUTO_TEST_CASE(test_multiple_inheritance)
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

	// create new sub-objects
	Q2 *q2 = create<Q2>(alloc);
	Q0 *q2_0 = q2->create_as<Q0>(alloc);
	Q1 *q2_1 = q2->create_as<Q1>(alloc);
	W *q2_w = q2->create_as<W>(alloc);
	BOOST_ASSERT(q2_0 && q2_1 && q2_w);
}	

namespace mi_test_2
{
	struct Base { virtual ~Base() { } };
	struct T0 : base<T0, Base> { };
	struct T1 : T0, base<T1, Base> { };
	struct W : base<W, Base> { };
	struct T2 : W, T1, base<T2, Base> { };
}

BOOST_AUTO_TEST_CASE(test_mi_2)
{
	using namespace mi_test_2;
	using namespace cloneable;

    T2 *t2 = new T2();

	assert(t2->can_clone_as<T0>());
	assert(t2->can_clone_as<T1>());
	assert(t2->can_clone_as<T2>());
	assert(t2->can_clone_as<W>());
    
	assert(t2->can_create_as<T0>());
	assert(t2->can_create_as<T1>());
	assert(t2->can_create_as<T2>());
	assert(t2->can_create_as<W>());

	// these free-functions currently do not work...
   /* assert(can_clone_as<W>(*t2));
    assert(can_clone_as<T0>(*t2));
    assert(can_clone_as<T1>(*t2));
    assert(can_clone_as<T2>(*t2));
   */ 
    //assert(can_create_as<W>(*t2));
  //  assert(can_create_as<T0>(*t2));
//    assert(can_create_as<T1>(*t2));
    //assert(can_create_as<T2>(*t2));
    
    // clone sub-objects
    W *t2_w = t2->clone_as<W>();
    T0 *t2_t0 = t2->clone_as<T0>();
    T1 *t2_t1 = t2->clone_as<T1>();
    T1 *t2_t2 = t2->clone_as<T2>();
    
    // create sub-objects
    W *t2_w_new = t2->create_as<W>();
    T0 *t2_t0_new = t2->create_as<T0>();
    T1 *t2_t1_new = t2->create_as<T1>();
    T2 *t2_t2_new = t2->create_as<T2>();

	delete t2;
	delete t2_w;
	delete t2_t0;
	delete t2_t1;
	delete t2_t2;
	delete t2_w_new;
	delete t2_t0_new;
	delete t2_t1_new;
	delete t2_t2_new;
}


BOOST_AUTO_TEST_CASE(test_multiple_inheritance_vector)
{
	using namespace mulitple_inheritance_test;
	typedef cloneable::vector<> vec;
	
	vec v;
	v.push_back<Q0>(42);
	v.push_back<Q1>("foo");

	Q0 &q0 = v.as<Q0>(0);
	Q1 &q1 = v.as<Q1>(1);

	// ensure duplication of types with multiple cloneable sub-objects works correctly
	vec v2 = v;
	
	BOOST_ASSERT(v2.as<Q0>(0).num == 42);
	BOOST_ASSERT(v2.as<Q1>(1).str == "foo");
}

namespace custom_clone_test
{
	struct T0 : base<T0>
	{
		bool custom_cloned;
		T0() : custom_cloned(false) { }

		/// override the means to create a copy of this
		T0 *make_copy(abstract_allocator &alloc) const
		{
			T0 *ptr = cloneable::create<T0>(alloc);
			ptr->custom_cloned = true;
			return ptr;
		}
	};
}

BOOST_AUTO_TEST_CASE(test_custom_clone)
{
	using namespace custom_clone_test;
	abstract_base<> *p = new T0();
	T0 *q = dynamic_cast<T0 *>(p->clone());
	BOOST_ASSERT(q && q->custom_cloned);
	delete p;
	delete q;
}

namespace no_default_ctor_test
{
	struct T0 : base<T0, base_type, no_default_construction_tag>
	{
		T0(int) { }
	};
	struct T1 : T0, base<T1>
	{
		T1() : T0(4) { }
	};
}

BOOST_AUTO_TEST_CASE(test_no_default_ctor)
{
	using namespace no_default_ctor_test;
	T0 *p = new T0(10);

	// ensure we can clone, even without a default ctor
	T0 *q = p->clone_as<T0>();
	BOOST_ASSERT(q && typeid(*q) == typeid(T0));

	// ensure we fail to make a default-constructed new object
	delete q;
	bool caught = false;
	try
	{
		base_type *r = p->create();
	}
	catch (no_default_construction)
	{
		caught = true;
	}
	BOOST_ASSERT(caught);

	T1 *p1 = new T1();
	// ensure we can clone a type that is derived from a type that is not
	// default constructable
	T1 *p1_clone = p1->clone_as<T1>();
	BOOST_ASSERT(p1_clone);

	// ensure that if we try to make a new sub-object that is not 
	// default-constructable, we throw
	caught = false;
	try
	{
		p1_clone->create_as<T0>();
	}
	catch (no_default_construction)
	{
		caught = true;
	}
	BOOST_ASSERT(caught);

	delete p;
	delete p1;
	delete p1_clone;
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

namespace traits_test
{
	struct T0 : base<T0> { };
	struct T1 : base<T1, base_type, no_default_construction_tag> { };
	struct T2 { };
}

BOOST_AUTO_TEST_CASE(test_traits)
{
	using namespace traits_test;
	typedef traits<T0> t0_traits;

	BOOST_STATIC_ASSERT(is_cloneable<T0>::value);
	BOOST_STATIC_ASSERT(is_default_constructable<T0>::value);

	BOOST_STATIC_ASSERT(is_cloneable<T1>::value);
	BOOST_STATIC_ASSERT(!is_default_constructable<T1>::value);
	
	BOOST_STATIC_ASSERT(!is_cloneable<T2>::value);
	BOOST_STATIC_ASSERT(!is_default_constructable<T2>::value);
}

namespace free_clone_test
{
	struct T0 { };
	struct T1 : base<T1> 
	{ 
		bool copy_constructed;
		T1() : copy_constructed(false) { }
		T1(T1 const &) : copy_constructed(true) { }
	};
}

BOOST_AUTO_TEST_CASE(test_free_clone)
{
	using namespace free_clone_test;
	T0 *t0 = new T0();
	T1 *t1 = new T1();

	// test cloning on a type that does not derive from cloneable::base<>
	// this will use new T0(orig);
	T0 *t0_clone = clone(*t0);

	// clone using cloneable system
	T1 *t1_clone = clone(*t1);

	BOOST_ASSERT(t0_clone);
	BOOST_ASSERT(t1_clone);
	BOOST_ASSERT(t1_clone->copy_constructed);

	delete t0;
	delete t1;
	delete t0_clone;
	delete t1_clone;
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
		bases.push_back<T0>(42);						
		bases.push_back<T1>("foo");
		bases.push_back<T2>(3.14f, -123, "spam");
		bases.push_back<cloneable_external_type>("external");

		// perform functor on each contained object of the given type
		bases.for_each<T2>(boost::bind(&T2::print, _1));

		// does a deep copy, preserving concrete types
		vec copy = bases;

		// each object in the container can be retrieved generically as a base_type
		my_base &generic0 = copy[0];
		my_base &generic1 = copy[1];
		my_base &generic2 = copy[2];

		// get a reference; will throw bad_cast on type mismatch
		T0 &p1 = copy.as<T0>(0);

		// get a pointer; returns null on type mismatch
		T1 *p2 = copy.pointer<T1>(1);
		T2 *p3 = copy.pointer<T2>(2);
		
		BOOST_ASSERT(p2);
		BOOST_ASSERT(p3);
		
		BOOST_ASSERT(p1.num == 42);
		BOOST_ASSERT(p2->str == "foo");
		BOOST_ASSERT(p3->real == 3.14f);
		BOOST_ASSERT(p3->num == -123);
		BOOST_ASSERT(p3->str == "spam");
		BOOST_ASSERT(copy.as<cloneable_external_type>(3).text == "external");

		bool caught = false;
		try
		{
			my_base &base = copy.as<T1>(0);
		}
		catch (std::bad_cast)
		{
			caught = true;
		}
		BOOST_ASSERT(caught);

	}
}

namespace list_test
{
	struct list_test_base
	{
		int num;
		list_test_base(int n = 0) : num(n) { }
		friend bool operator<(list_test::list_test_base const &left, list_test::list_test_base const &right)
		{
			return left.num < right.num;
		}
		friend bool operator==(list_test::list_test_base const &left, list_test::list_test_base const &right)
		{
			return left.num == right.num;
		}
	};

	struct L0 : base<L0, list_test_base, no_default_construction_tag>
	{
		L0(int n) : list_test_base(n) { }
	};
	struct L1 : base<L1, list_test_base, no_default_construction_tag>
	{
		L1(string s, int n) : list_test_base(n) { }
	};
	struct L2 : base<L2, list_test_base, no_default_construction_tag>
	{
		L2(float f, int n, string s) : list_test_base(n) { }
	};
}


BOOST_AUTO_TEST_CASE(test_list)
{
	using namespace list_test;
	typedef cloneable::list<list_test_base> list;
	list l0;
	l0.push_back<L0>(42);						
	l0.push_back<L1>("foo", 123);
	l0.push_back<L2>(3.14f, -123, "spam");

	list l1 = l0;
	bool b = l0.front() == l0.back();
	BOOST_ASSERT(l0 == l1);
	BOOST_ASSERT(!(l0 < l1));
	BOOST_ASSERT(!(l1 < l0));
	list::reference e = l1.front();
	e.num = 41;
	BOOST_ASSERT(l1 < l0);

	list::iterator iter = l1.begin();
	BOOST_ASSERT(typeid(*iter++) == typeid(L0));
	BOOST_ASSERT(typeid(*iter++) == typeid(L1));
	BOOST_ASSERT(typeid(*iter++) == typeid(L2));
}

namespace map_test
{
	struct my_base
	{
		int number;
		my_base(int n = 0) : number(n) { }
		virtual ~my_base() { }
		bool operator==(my_base const &other) const
		{
			return number == other.number;
		}
		bool operator!=(my_base const &other) const
		{
			return number != other.number;
		}
		bool operator<(my_base const &other) const
		{
			return number < other.number;
		}
	};

	struct M0 : base<M0, my_base>
	{
		M0(int n = 0) : my_base(n) {}
	};

	struct M1 : base<M1, my_base>
	{
		string str;
		M1(int n = 1) : my_base(n) { }
		M1(const char *s, int n = 1) : my_base(n), str(s) { }
	};

	struct M2 : base<M2, my_base>
	{
		M2(int n = 2) : my_base(n) { }
	};

	struct M3 : base<M3, my_base>
	{
		M3(int n = 3) : my_base(n) { }
	};
}

BOOST_AUTO_TEST_CASE(test_map)
{
	using namespace map_test;
	typedef cloneable::map<int, map_test::my_base> Map;
	Map map;
	map.insert<M0>(1);
	map.insert<M1>(2, "foo");
	map.insert<M3>(3, 42);

	// the preacher - prior to bill hicks? 

	// deep copy
	Map copy = map;
	BOOST_ASSERT(copy == map);

	Map::iterator i3 = copy.find(3);
	BOOST_ASSERT(i3 != copy.end());
	BOOST_ASSERT(typeid(*i3->second) == typeid(M3));
	M3 *m3_clone = i3->second->clone_as<M3>();
	BOOST_ASSERT(m3_clone != 0);

	delete m3_clone;
}

BOOST_AUTO_TEST_CASE(test_heterogenous_map)
{
	//using namespace map_test;
	//typedef cloneable::heterogenous_map<map_test::my_base> Map;
	//Map map;
	//map.key<M2>().value<M3>();
	//BOOST_ASSERT(map.size() == 1);
	//Map::iterator a = map.find<M2>();
	//BOOST_ASSERT(a != map.end());

	//map.key<M0>(42).value<M1>("foo");
	//BOOST_ASSERT(map.size() == 2);

	//Map::iterator iter = map.find<M0>(42);
	//BOOST_ASSERT(iter!= map.end());

	//map.key<M2>().value<M3>();

//	Map copy = map;
}

BOOST_AUTO_TEST_CASE(test_hash)
{
	using namespace map_test;
	M0 a, b;
	//BOOST_ASSERT(a.hash() != b.hash());
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

namespace set_test
{
	struct set_base
	{
		int number;
		set_base(int n = 0) : number(n) { }
		friend bool operator==(const set_base &left, const set_base &right)
		{
			return left.number == right.number;
		}
		friend bool operator<(const set_base &left, const set_base &right)
		{
			return left.number < right.number;
		}
	};
	struct S0 : base<S0, set_base> 
	{ 
		S0(int n = 0) : set_base(n) { }
	};
	struct S1 : base<S1, set_base>
	{ 
		S1(int n = 0) : set_base(n) { }
	};

	struct S2 : base<S2, set_base>
	{ 
		S2(int n = 0) : set_base(n) { }
	};
}

BOOST_AUTO_TEST_CASE(test_set)
{
	using namespace set_test;
	typedef cloneable::set<set_base> Set;
	Set set;
	const S0 *s0 = set.emplace<S0>(1).value.to_derived();
	set.emplace<S1>(2);
	set.emplace<S2>(3);
	set.emplace<S2>(4);

	Set::const_iterator A = set.begin(), B = set.end();
	for (; A != B; ++A)
	{
		const set_base *base = &*A;
		BOOST_ASSERT(base);
	}

	BOOST_ASSERT(set.size() == 4);
	BOOST_ASSERT(set.find<S0>(1) != set.end());
	BOOST_ASSERT(&*set.find<S0>(1) == s0);
	BOOST_ASSERT(set.find<set_base>(2) != set.end());
	BOOST_ASSERT(set.find<S1>(1) == set.end());
	BOOST_ASSERT(set.find<set_base>(1) != set.end());

	Set copy = set;
	BOOST_ASSERT(copy == set);

	BOOST_ASSERT(copy.size() == 4);
	BOOST_ASSERT(copy.find<S0>(1) != copy.end());
	BOOST_ASSERT(copy.find<set_base>(2) != copy.end());

	Set::iterator found;
	found = set.find<set_base>(1);
	BOOST_ASSERT(found != set.end());
	BOOST_ASSERT(&*found == s0);
}

//EOF
