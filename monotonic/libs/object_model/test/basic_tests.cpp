// (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// documentation at https://svn.boost.org/svn/boost/sandbox/monotonic/libs/object_model/doc/index.html
// sandbox at https://svn.boost.org/svn/boost/sandbox/monotonic/object_model/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#define BOOST_TEST_MODULE basic_test test
#include <boost/test/unit_test.hpp>

#include <string>
#include <boost/object_model/object.hpp>
#include <boost/object_model/registry.hpp>
#include <boost/object_model/type/builtins.hpp>
#include <boost/object_model/builder.hpp>
#include <boost/object_model/type/traits.hpp>

using namespace std;
using namespace boost;
namespace om = boost::object_model;

BOOST_AUTO_TEST_CASE(test_generic_object)
{
	om::registry<> reg;

	om::generic::object obj = reg.create<void>();
	BOOST_ASSERT(num.exists());

	om::generic::const_object cobj = obj;
//	om::generic::mutable_object mobj = obj;
}

BOOST_AUTO_TEST_CASE(test_object)
{
	om::registry<> reg;
	om::object<int> num = reg.create<int>();
	BOOST_ASSERT(num.exists());
	BOOST_ASSERT(num.is_type<int>());
	*num = 42;
	BOOST_ASSERT(deref<int>(num) == 42);
}

struct Foo
{
	int num;
	std::string str;

	void bar()
	{
	}
	int spam(int n)
	{
		return n*2;
	}
};

BOOST_OBJECT_MODEL_TRAITS_NUM(Foo, 666);

BOOST_AUTO_TEST_CASE(test_builder)
{
	om::registry<> reg;
	om::builder<Foo>(reg)
		.methods
			("bar", &Foo::bar)
			("spam", &Foo::spam)
		.fields
			("num", &Foo::num)
			("str", &Foo::str)
		;
	om::object<Foo> foo = reg.create<Foo>();
	BOOST_ASSERT(foo.exists());
	BOOST_ASSERT(foo.is_type<Foo>());

	BOOST_ASSERT(foo.get_class().has_method("bar"));
	BOOST_ASSERT(foo.get_class().has_method("spam"));
	BOOST_ASSERT(foo.get_class().has_field("num"));
	BOOST_ASSERT(foo.get_class().has_field("str"));
}

//EOF
