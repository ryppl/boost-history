//  Copyright (c) 2001-2008 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/detail/lightweight_test.hpp>

// include this first to make rules have placeholders (e.g. _r1)
#include <boost/spirit/include/support_argument.hpp>

#include <boost/spirit/include/karma_operator.hpp>
#include <boost/spirit/include/karma_char.hpp>
#include <boost/spirit/include/karma_auxiliary.hpp>
#include <boost/spirit/include/karma_string.hpp>
#include <boost/spirit/include/karma_numeric.hpp>
#include <boost/spirit/include/karma_nonterminal.hpp>
#include <boost/spirit/include/karma_action.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>

#include "test.hpp"

using namespace spirit_test;

///////////////////////////////////////////////////////////////////////////////
int main()
{
    using namespace boost;
    using namespace boost::spirit;
    using namespace boost::spirit::arg_names;
    using namespace boost::spirit::karma;
    using namespace boost::spirit::ascii;

    typedef spirit_test::output_iterator<char>::type outiter_type;

    // basic tests
    {
        rule<outiter_type> start;

        start = char_[_1 = 'a'] << int_[_1 = 10] << double_[_1 = 12.4];
        BOOST_TEST(test("a1012.4", start));

        start = (char_ << int_ << double_)[_1 = 'a', _2 = 10, _3 = 12.4];
        BOOST_TEST(test("a1012.4", start));

        rule<outiter_type> a, b, c;
        a = char_[_1 = 'a'];
        b = int_[_1 = 10];
        c = double_[_1 = 12.4];

        start = a << b << c;
        BOOST_TEST(test("a1012.4", start));
    }

    // basic tests involving a direct parameter
    {
        typedef variant<char, int, double> var_type;
        fusion::vector<unused_type, var_type> v (unused, 'a');

        rule<outiter_type, void(var_type)> start;

        start = (char_ | int_ | double_)[_1 = _r1];
        BOOST_TEST(test("a", start, v));

        v = fusion::vector<unused_type, var_type>(unused, 10);
        BOOST_TEST(test("10", start, v));
        v = fusion::vector<unused_type, var_type>(unused, 12.4);
        BOOST_TEST(test("12.4", start, v));
    }

    {
        rule<outiter_type, void(char, int, double)> start;
        fusion::vector<unused_type, char, int, double> vec(unused, 'a', 10, 12.4);

        start = char_[_1 = _r1] << int_[_1 = _r2] << double_[_1 = _r3];
        BOOST_TEST(test("a1012.4", start, vec));
        BOOST_TEST(test("a1012.4", start('a', 10, 12.4)));

        start = (char_ << int_ << double_)[_1 = _r1, _2 = _r2, _3 = _r3];
        BOOST_TEST(test("a1012.4", start, vec));
        BOOST_TEST(test("a1012.4", start('a', 10, 12.4)));

        rule<outiter_type, void(char)> a;
        rule<outiter_type, void(int)> b;
        rule<outiter_type, void(double)> c;

        a = char_[_1 = _r1];
        b = int_[_1 = _r1];
        c = double_[_1 = _r1];
        start = a(_r1) << b(_r2) << c(_r3);
        BOOST_TEST(test("a1012.4", start, vec));
        BOOST_TEST(test("a1012.4", start('a', 10, 12.4)));
    }

    // test rule parameter propagation
    {
        rule<outiter_type, void(char, int, double)> start;
        fusion::vector<unused_type, char, int, double> vec(unused, 'a', 10, 12.4);

        start %= char_ << int_ << double_;
        BOOST_TEST(test("a1012.4", start, vec));
        BOOST_TEST(test("a1012.4", start('a', 10, 12.4)));

        rule<outiter_type, void(char)> a;
        rule<outiter_type, void(int)> b;
        rule<outiter_type, void(double)> c;

        a %= char_ << eps;
        b %= int_;
        c %= double_;
        start = a(_r1) << b(_r2) << c(_r3);
        BOOST_TEST(test("a1012.4", start, vec));
        BOOST_TEST(test("a1012.4", start('a', 10, 12.4)));
    }

    // basic tests with delimiter
    {
        rule<outiter_type, space_type> start;

        start = char_[_1 = 'a'] << int_[_1 = 10] << double_[_1 = 12.4];
        BOOST_TEST(test_delimited("a 10 12.4 ", start, space));

        start = (char_ << int_ << double_)[_1 = 'a', _2 = 10, _3 = 12.4];
        BOOST_TEST(test_delimited("a 10 12.4 ", start, space));

        rule<outiter_type, space_type> a, b, c;
        a = char_[_1 = 'a'];
        b = int_[_1 = 10];
        c = double_[_1 = 12.4];

        start = a << b << c;
        BOOST_TEST(test_delimited("a 10 12.4 ", start, space));
    }

    // locals test
    {
        rule<outiter_type, locals<std::string> > start;

        start = lit[_1 = "abc", _a = _1] << int_[_1 = 10] << lit[_1 = _a];
        BOOST_TEST(test("abc10abc", start));
    }

     //~ { // alias tests
         //~ typedef variant<char, int, double> var_type;
         //~ fusion::vector<unused_type, var_type> v (unused, 'a');

         //~ rule<outiter_type, void(var_type)> d, start;

         //~ d = start.alias();   // d will always track start

         //~ start = (char_ | int_ | double_)[_1 = _r1];
         //~ BOOST_TEST(test("a", d, v));

         //~ v = fusion::vector<unused_type, var_type>(unused, 10);
         //~ BOOST_TEST(test("10", d, v));
         //~ v = fusion::vector<unused_type, var_type>(unused, 12.4);
         //~ BOOST_TEST(test("12.4", d, v));
     //~ }

     //~ { // copy tests

         //~ rule<char const*> a, b, c, start;

         //~ a = 'a';
         //~ b = 'b';
         //~ c = 'c';

         //~ // The FF is the dynamic equivalent of start = *(a | b | c);
         //~ start = a;
         //~ start = start.copy() | b;
         //~ start = start.copy() | c;
         //~ start = *(start.copy());

         //~ BOOST_TEST(test("abcabcacb", start));

         //~ // The FF is the dynamic equivalent of start = (a | b) >> (start | b);
         //~ start = b;
         //~ start = a | copy(start);
         //~ start = start.copy() >> (start | b);

         //~ BOOST_TEST(test("aaaabababaaabbb", start));
         //~ BOOST_TEST(test("aaaabababaaabba", start, false));
     //~ }

    return boost::report_errors();
}

