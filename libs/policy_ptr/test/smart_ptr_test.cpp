//////////////////////////////////////////////////////////////////////////////
//
//  smart_ptr_test.cpp
//
//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) David Held 2002.
//
//  Permission to copy, use, modify, sell and distribute this software is
//  granted provided this  copyright notice  appears in all copies.  This
//  software is provided "as is" without express or implied warranty, and
//  with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org/ for most recent version, including documentation.
//
//////////////////////////////////////////////////////////////////////////////
//
//  This is the main test program for the policy-based smart pointer library.
//  All tests should pass.  The ordering_test.cpp tests the ordering comparison
//  operators (in a separate file, because they are enabled via a macro).  See
//  libs/policy_ptr/doc/index.html
//
//////////////////////////////////////////////////////////////////////////////
#define BOOST_SMART_POINTER_BASIC_INTERFACE

#include <string>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include "../../../boost/policy_ptr/smart_ptr.hpp"
//#include "../../../boost/policy_ptr/std_ptrs.hpp"

namespace test = boost::unit_test_framework;
using boost::mpl::_;

class object
{
public:
    object(bool& dead) : dead_(dead), dummy_(3.14159) { }
    virtual ~object(void) { dead_ = true; }

    int foo(void) const { return 42; }
    double bar(void) const { return dummy_; }
private:
    bool& dead_;
    double dummy_;
};

class child : public object
{
public:
    child(bool& dead, int age) : object(dead), age_(age) { }

    int age(void) const { return age_; }
private:
    int age_;
};
//#if 0
void default_test(void)
{
    boost::smart_ptr<object> p;
    object* q = 0;
    BOOST_CHECK_EQUAL(sizeof(p), sizeof(object*) + sizeof(int));
    BOOST_CHECK(!p);
    BOOST_CHECK(p == q);
    BOOST_CHECK(!boost::get_impl(p));
}

void init_const_test(boost::smart_ptr<object> const& p)
{
    BOOST_CHECK_EQUAL(p->foo(), 42);
    BOOST_CHECK_EQUAL((*p).bar(), 3.14159);
}

void init_test(void)
{
    bool dead(false);
    {
        boost::smart_ptr<object> p(new object(dead));
        BOOST_CHECK_EQUAL(p->foo(), 42);
        BOOST_CHECK_EQUAL((*p).bar(), 3.14159);
        init_const_test(p);
        boost::smart_ptr<object> q;
        BOOST_CHECK(p != q);
        BOOST_CHECK(p ? true : false);
    }
    BOOST_CHECK(dead);
}
//#endif
void copy_test(void)
{
    bool dead(false);
    {
        boost::smart_ptr<object, boost::ref_counted<_>, boost::assert_check<_> > p(new object(dead));
        BOOST_CHECK_EQUAL(boost::use_count(p), unsigned(1));
        bool dead_child(false);
        {
            boost::smart_ptr<object, boost::scalar_storage<_> > q(p);
            BOOST_CHECK(p == q);
            BOOST_CHECK_EQUAL(q->foo(), 42);
            BOOST_CHECK_EQUAL(boost::use_count(p), 2u );
            BOOST_CHECK_EQUAL(boost::use_count(q), 2u );

            boost::reset(q, new child(dead_child, 3));
            boost::smart_ptr<child> c(
                boost::smart_static_cast<child>(q)
            );
            boost::smart_ptr<child> d(q, boost::detail::static_cast_tag());
//            BOOST_CHECK_EQUAL(boost::use_count(c), 2u );
        }
        BOOST_CHECK(dead_child);
        BOOST_CHECK_EQUAL(boost::use_count(p), 1u );
    }
    BOOST_CHECK(dead);
}

test::test_suite* init_unit_test_suite(int argc, char* argv[])
{
    test::test_suite* test = BOOST_TEST_SUITE("Basic smart_ptr framework tests");

    test->add(BOOST_TEST_CASE(&default_test));
    test->add(BOOST_TEST_CASE(&init_test));
    test->add(BOOST_TEST_CASE(&copy_test));

    return test;
}

