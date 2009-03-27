/**
 * \file test/visitors_01.cpp
 *
 *  This file is part of the Mirror library testsuite.
 *
 *  Testing traversals
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <map>

#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/meta_classes/std/pair.hpp>
#include <boost/mirror/meta_classes/boost/tuples/tuple.hpp>
#include <boost/mirror/meta_classes/boost/fusion/vector.hpp>
#include <boost/mirror/traversal.hpp>

#include "./namespaces.hpp"
#include "./simple_classes.hpp"
#include "./test.hpp"

typedef ::std::map< ::std::string, int > visitors_log;

struct visitor_1
{
	typedef ::boost::mpl::false_ works_on_instances;

	visitors_log& log;	

	visitor_1(visitors_log& _log)
	 : log(_log)
	{ }

        template <class MetaNamespace, class Context>
        void enter_namespace(MetaNamespace, Context)
        {
		++log["item"];	
		++log["namespace"];	
	}

        template <class MetaObjectSequence, class Context>
        void enter_namespace_members(MetaObjectSequence, Context)
        {
		++log["item"];	
		++log["namespace_members"];	
	}

        template <class MetaClass, class Context>
        void enter_type(MetaClass, Context)
	{
		++log["item"];	
		++log["type"];
	}

        template <class MetaClass, class BaseClasses, class Context>
        void enter_base_classes(MetaClass, BaseClasses, Context)
	{
		++log["item"];	
		++log["base_classes"];
	}

        template <class MetaInheritance, class Context>
        void enter_base_class(MetaInheritance, Context)
	{
		++log["item"];	
		++log["base_class"];
	}

        template <class MetaClass, class MetaAttributes, class Context>
        void enter_attributes(MetaClass, MetaAttributes, Context)
	{
		++log["item"];	
		++log["attributes"];
	}

        template <class MetaAttribute, class Context>
        void enter_attribute(MetaAttribute, Context)
	{
		++log["item"];	
		++log["attribute"];
	}
};


struct visitor_2
{
	typedef ::boost::mpl::false_ works_on_instances;

	visitors_log& log;	

	visitor_2(visitors_log& _log)
	 : log(_log)
	{ }

        template <class MetaNamespace, class Context>
        void leave_namespace(MetaNamespace, Context)
        {
		++log["item"];	
		++log["namespace"];	
	}

        template <class MetaObjectSequence, class Context>
        void leave_namespace_members(MetaObjectSequence, Context)
        {
		++log["item"];	
		++log["namespace_members"];	
	}

        template <class MetaClass, class Context>
        void leave_type(MetaClass, Context)
	{
		++log["item"];	
		++log["type"];
	}

        template <class MetaClass, class BaseClasses, class Context>
        void leave_base_classes(MetaClass, BaseClasses, Context)
	{
		++log["item"];	
		++log["base_classes"];
	}

        template <class MetaInheritance, class Context>
        void leave_base_class(MetaInheritance, Context)
	{
		++log["item"];	
		++log["base_class"];
	}

        template <class MetaClass, class MetaAttributes, class Context>
        void leave_attributes(MetaClass, MetaAttributes, Context)
	{
		++log["item"];	
		++log["attributes"];
	}

        template <class MetaAttribute, class Context>
        void leave_attribute(MetaAttribute, Context)
	{
		++log["item"];	
		++log["attribute"];
	}
};

template <class MetaClass>
void test_traversals(void)
{
	using namespace ::boost::mirror;
	//
	visitors_log log1, log2;
	//
	deep_traversal_of<MetaClass>::accept(visitor_1(log1));
	deep_traversal_of<MetaClass>::accept(visitor_2(log2));
	//
	BOOST_CHECK(!log1.empty());
	BOOST_CHECK(!log2.empty());
	BOOST_CHECK(log1["item"] > 0);
	BOOST_CHECK(log2["item"] > 0);
	BOOST_CHECK(log1 == log2);
	//
	log1.clear();
	log2.clear();
	//
	flat_traversal_of<MetaClass>::accept(visitor_1(log1));
	flat_traversal_of<MetaClass>::accept(visitor_2(log2));
	//
	BOOST_CHECK(!log1.empty());
	BOOST_CHECK(!log2.empty());
	BOOST_CHECK(log1["item"] > 0);
	BOOST_CHECK(log2["item"] > 0);
	BOOST_CHECK(log1 == log2);
}


void test_main()
{
	typedef ::std::pair< ::std::string, ::std::wstring> P;
	typedef ::boost::tuple<bool, char, short, int, long, float, double> T;
	typedef ::boost::fusion::vector<bool, char, short, int, long, float> V;

	test_traversals<BOOST_MIRRORED_CLASS(::test::H)>();
	test_traversals<BOOST_MIRRORED_CLASS(P)>();
	test_traversals<BOOST_MIRRORED_CLASS(T)>();
	test_traversals<BOOST_MIRRORED_CLASS(V)>();
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: traversals/visitors run-time test 01");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}


