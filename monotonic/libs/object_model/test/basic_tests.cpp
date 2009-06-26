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
#include <boost/object_model/dereference.hpp>
#include <boost/object_model/type/specifier.hpp>
#include <boost/object_model/string.hpp>

using namespace std;
using namespace boost;
namespace om = boost::object_model;

BOOST_AUTO_TEST_CASE(test_type_specifier)
{
	om::registry<> reg;
	reg.register_class<void>();
	reg.register_class<int>();

	//om::string<> text = type::make_specifier<int>().to_string(reg);
}

BOOST_AUTO_TEST_CASE(test_type_traits)
{
	BOOST_STATIC_ASSERT(om::type::traits<int>::type_number == om::type::number::Int);
	BOOST_STATIC_ASSERT(om::type::traits<void>::type_number == om::type::number::Void);
}

BOOST_AUTO_TEST_CASE(test_generic_object)
{
	om::registry<> reg;

	reg.register_class<void>();
	BOOST_ASSERT(reg.has_class<void>());

	om::generic::object obj = reg.create<void>();
	BOOST_ASSERT(obj.exists());

	om::generic::const_object const_obj = obj;
	BOOST_ASSERT(const_obj.exists());
	om::generic::mutable_object mobj = obj;

	BOOST_ASSERT(reg.num_classes() == 1);
	BOOST_ASSERT(reg.num_instances() == 1);

	BOOST_ASSERT(obj.exists());
	reg.destroy(obj);
	BOOST_ASSERT(!obj.exists());
	BOOST_ASSERT(reg.num_instances() == 0);
}

BOOST_AUTO_TEST_CASE(test_object)
{
	om::registry<> reg;
	reg.register_class<int>();
	BOOST_ASSERT(reg.has_class<int>());

	om::object<int> num = reg.create<int>();
	BOOST_ASSERT(num.exists());
	BOOST_ASSERT(num.is_type<int>());
	
	*num = 42;
	
	BOOST_ASSERT(om::deref<int>(num) == 42);
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
	om::class_builder<Foo>(reg)
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
