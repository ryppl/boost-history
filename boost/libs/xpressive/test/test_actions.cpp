///////////////////////////////////////////////////////////////////////////////
// test_actions.cpp
//
//  Copyright 2004 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//#define BOOST_XPRESSIVE_BETTER_ERRORS

#include <map>
#include <list>
#include <stack>
#include <numeric>
#include <boost/xpressive/xpressive_static.hpp>
#include <boost/xpressive/regex_actions.hpp>
#include <boost/test/unit_test.hpp>

///////////////////////////////////////////////////////////////////////////////
// test1
//  simple action which builds a string
void test1()
{
    using namespace boost::xpressive;

    std::string result;
    std::string str("foo bar baz foo bar baz");
    sregex rx = (+_w)[ ref(result) += _ ] >> *(' ' >> (+_w)[ ref(result) += ',' + _ ]);

    if(!regex_match(str, rx))
    {
        BOOST_ERROR("oops");
    }
    else
    {
        BOOST_CHECK_EQUAL(result, "foo,bar,baz,foo,bar,baz");
    }
}

///////////////////////////////////////////////////////////////////////////////
// test2
//  test backtracking over actions
void test2()
{
    using namespace boost::xpressive;

    std::string result;
    std::string str("foo bar baz foo bar baz");
    sregex rx = (+_w)[ ref(result) += _ ] >> *(' ' >> (+_w)[ ref(result) += ',' + _ ]) >> repeat<4>(_);

    if(!regex_match(str, rx))
    {
        BOOST_ERROR("oops");
    }
    else
    {
        BOOST_CHECK_EQUAL(result, "foo,bar,baz,foo,bar");
    }
}

///////////////////////////////////////////////////////////////////////////////
// test3
//  cast string to int, push back into list, use alternate ->* syntax
void test3()
{
    using namespace boost::xpressive;

    std::list<int> result;
    std::string str("1 23 456 7890");
    sregex rx = (+_d)[ ref(result)->*push_back( as<int>(_) ) ] 
        >> *(' ' >> (+_d)[ ref(result)->*push_back( as<int>(_) ) ]);

    if(!regex_match(str, rx))
    {
        BOOST_ERROR("oops");
    }
    else
    {
        BOOST_REQUIRE_EQUAL(result.size(), 4u);
        BOOST_CHECK_EQUAL(*result.begin(), 1);
        BOOST_CHECK_EQUAL(*++result.begin(), 23);
        BOOST_CHECK_EQUAL(*++++result.begin(), 456);
        BOOST_CHECK_EQUAL(*++++++result.begin(), 7890);
    }
}

///////////////////////////////////////////////////////////////////////////////
// test4
//  build a map of strings to integers
void test4()
{
    using namespace boost::xpressive;

    std::map<std::string, int> result;
    std::string str("aaa=>1 bbb=>23 ccc=>456");
    sregex pair = ( (s1= +_w) >> "=>" >> (s2= +_d) )[ ref(result)[s1] = as<int>(s2) ];
    sregex rx = pair >> *(+_s >> pair);

    if(!regex_match(str, rx))
    {
        BOOST_ERROR("oops");
    }
    else
    {
        BOOST_REQUIRE_EQUAL(result.size(), 3u);
        BOOST_CHECK_EQUAL(result["aaa"], 1);
        BOOST_CHECK_EQUAL(result["bbb"], 23);
        BOOST_CHECK_EQUAL(result["ccc"], 456);
    }
}

///////////////////////////////////////////////////////////////////////////////
// test4_aux
//  build a map of strings to integers, with a late-bound action argument.
void test4_aux()
{
    using namespace boost::xpressive;
    arg< std::map<std::string, int> > map;

    sregex pair = ( (s1= +_w) >> "=>" >> (s2= +_d) )[ map[s1] = as<int>(s2) ];
    sregex rx = pair >> *(+_s >> pair);

    std::string str("aaa=>1 bbb=>23 ccc=>456");
    smatch what;
    std::map<std::string, int> result;
    what.let(map = result); // bind the argument!

    if(!regex_match(str, what, rx))
    {
        BOOST_ERROR("oops");
    }
    else
    {
        BOOST_REQUIRE_EQUAL(result.size(), 3u);
        BOOST_CHECK_EQUAL(result["aaa"], 1);
        BOOST_CHECK_EQUAL(result["bbb"], 23);
        BOOST_CHECK_EQUAL(result["ccc"], 456);
    }
}

///////////////////////////////////////////////////////////////////////////////
// test5
//  calculator that calculates. This is just silly, but hey.
void test5()
{
    using namespace boost::xpressive;

    // test for "local" variables.
    local<int> left, right;

    // test for reference<> to an existing variable
    std::stack<int> stack_;
    reference<std::stack<int> > stack(stack_);

    std::string str("4+5*(3-1)");

    sregex group, factor, term, expression;

    group       = '(' >> by_ref(expression) >> ')';
    factor      = (+_d)[ push(stack, as<int>(_)) ] | group;
    term        = factor >> *(
                                ('*' >> factor)
                                    [ right = top(stack)
                                    , pop(stack)
                                    , left = top(stack)
                                    , pop(stack)
                                    , push(stack, left * right)
                                    ]
                              | ('/' >> factor)
                                    [ right = top(stack)
                                    , pop(stack)
                                    , left = top(stack)
                                    , pop(stack)
                                    , push(stack, left / right)
                                    ]
                             );
    expression  = term >> *(
                                ('+' >> term)
                                    [ right = top(stack)
                                    , pop(stack)
                                    , left = top(stack)
                                    , pop(stack)
                                    , push(stack, left + right)
                                    ]
                              | ('-' >> term)
                                    [ right = top(stack)
                                    , pop(stack)
                                    , left = top(stack)
                                    , pop(stack)
                                    , push(stack, left - right)
                                    ]
                             );

    if(!regex_match(str, expression))
    {
        BOOST_ERROR("oops");
    }
    else
    {
        BOOST_REQUIRE_EQUAL(stack_.size(), 1u);
        BOOST_CHECK_EQUAL(stack_.top(), 14);

        BOOST_REQUIRE_EQUAL(stack.get().size(), 1u);
        BOOST_CHECK_EQUAL(stack.get().top(), 14);
    }
}

using namespace boost::unit_test;

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("test_actions");
    test->add(BOOST_TEST_CASE(&test1));
    test->add(BOOST_TEST_CASE(&test2));
    test->add(BOOST_TEST_CASE(&test3));
    test->add(BOOST_TEST_CASE(&test4));
    test->add(BOOST_TEST_CASE(&test4_aux));
    test->add(BOOST_TEST_CASE(&test5));
    return test;
}

