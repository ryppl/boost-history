/**
 * \file test/visitors_02.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 *  Testing traversals
 *
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/meta_classes/std/pair.hpp>
#include <boost/mirror/meta_classes/boost/tuples/tuple.hpp>
#include <boost/mirror/traversal.hpp>

#include "./namespaces.hpp"
#include "./simple_classes.hpp"
#include "./test.hpp"


struct visitor_1
{
	typedef ::boost::mpl::true_ works_on_instances;

	size_t& size;

	visitor_1(size_t& _size)
	 : size(_size)
	{ }

        template <class MetaClass, class Context, typename InstanceType>
        void visit_instance(MetaClass, Context ctx, InstanceType* ptr_to_inst)
        {
		size += sizeof(*ptr_to_inst);
	}
};

struct visitor_2
{
	typedef ::boost::mpl::true_ works_on_instances;

	size_t& size;

	visitor_2(size_t& _size)
	 : size(_size)
	{ }

        template <class MetaClass, class Context, typename InstanceType>
        void visit_instance(MetaClass, Context ctx, InstanceType* ptr_to_inst)
        {
		size -= sizeof(*ptr_to_inst);
	}
};

template <class Class>
void test_traversals(const Class& instance)
{
	using namespace ::boost::mirror;
	typedef BOOST_MIRRORED_CLASS(Class) meta_Class;

	size_t size = 0;

	deep_traversal_of<meta_Class>::accept(visitor_1(size), &instance);
	BOOST_CHECK(size > 0);
	deep_traversal_of<meta_Class>::accept(visitor_2(size), &instance);
	BOOST_CHECK(size == 0);

	flat_traversal_of<meta_Class>::accept(visitor_1(size), &instance);
	BOOST_CHECK(size > 0);
	flat_traversal_of<meta_Class>::accept(visitor_2(size), &instance);
	BOOST_CHECK(size == 0);

}


void test_main()
{
	using ::std::pair;
	using ::std::string;
	using ::std::wstring;
	using ::boost::make_tuple;
	test_traversals(make_tuple(0,1,2,3,4,5,6,7,8,9));
	test_traversals(pair<string, wstring>("ABC",L"DEF"));
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: traversals/visitors run-time test 02");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


