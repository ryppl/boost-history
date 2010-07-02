//  Copyright (c) 2001-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/config/warning_disable.hpp>
#include <boost/detail/lightweight_test.hpp>

#include <boost/spirit/include/karma_char.hpp>
#include <boost/spirit/include/karma_string.hpp>
#include <boost/spirit/include/karma_generate.hpp>
#include <boost/spirit/include/karma_action.hpp>
#include <boost/spirit/include/support_argument.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include "test.hpp"

using namespace spirit_test;

///////////////////////////////////////////////////////////////////////////////
int
main()
{
    using namespace boost::spirit;

    {
        using namespace boost::spirit::ascii;

        BOOST_TEST(test("a", lit('a')));
        BOOST_TEST(!test("a", lit('b')));

        BOOST_TEST(test("abc", "abc"));
        BOOST_TEST(!test("abcd", "abc"));

        BOOST_TEST(test("abc", lit("abc")));
        BOOST_TEST(!test("abcd", lit("abc")));

        BOOST_TEST(test("abc", string, "abc"));
        BOOST_TEST(!test("abcd", string, "abc"));

        BOOST_TEST(test("abc", string("abc")));
        BOOST_TEST(!test("abcd", string("abc")));
    }

    {
        using namespace boost::spirit::ascii;

        std::string str("abc");
        BOOST_TEST(test("abc", lit(str)));
        BOOST_TEST(!test("abcd", lit(str)));

        BOOST_TEST(test("abc", string(str)));
        BOOST_TEST(!test("abcd", string(str)));

        BOOST_TEST(test("abc", string, str));
        BOOST_TEST(!test("abcd", string, str));

        BOOST_TEST(test("abc", str));
        BOOST_TEST(!test("abcd", str));

        std::basic_string<wchar_t> wstr(L"abc");
        BOOST_TEST(test(L"abc", lit(wstr)));
        BOOST_TEST(!test(L"abcd", lit(wstr)));

        BOOST_TEST(test(L"abc", string, wstr));
        BOOST_TEST(!test(L"abcd", string, wstr));

        BOOST_TEST(test(L"abc", wstr));
        BOOST_TEST(!test(L"abcd", wstr));
    }

    {
        using namespace boost::spirit::ascii;

        BOOST_TEST(test(L"a", lit(L'a')));
        BOOST_TEST(!test(L"a", lit(L'b')));

        BOOST_TEST(test(L"abc", L"abc"));
        BOOST_TEST(test(L"abc", "abc"));
        BOOST_TEST(!test(L"abcd", L"abc"));

        BOOST_TEST(test(L"abc", lit(L"abc")));
        BOOST_TEST(!test(L"abcd", lit(L"abc")));

        BOOST_TEST(test(L"abc", string(L"abc")));
        BOOST_TEST(!test(L"abcd", string(L"abc")));

        BOOST_TEST(test(L"abc", string, L"abc"));
        BOOST_TEST(!test(L"abcd", string, L"abc"));

        BOOST_TEST(test(L"abc", string, "abc"));
        BOOST_TEST(!test(L"abcd", string, "abc"));
    }

    {
        using namespace boost::spirit::ascii;

        BOOST_TEST(test_delimited("a ", lit('a'), ' '));
        BOOST_TEST(!test_delimited("a ", lit('b'), ' '));

        BOOST_TEST(test_delimited("abc ", "abc", ' '));
        BOOST_TEST(!test_delimited("abcd ", "abc", ' '));

        BOOST_TEST(test_delimited("abc ", lit("abc"), ' '));
        BOOST_TEST(!test_delimited("abcd ", lit("abc"), ' '));

        BOOST_TEST(test_delimited("abc ", string, "abc", ' '));
        BOOST_TEST(!test_delimited("abcd ", string, "abc", ' '));

        BOOST_TEST(test_delimited("abc ", string("abc"), ' '));
        BOOST_TEST(!test_delimited("abcd ", string("abc"), ' '));
    }

    {
        using namespace boost::spirit::ascii;

        BOOST_TEST(test_delimited(L"a ", lit(L'a'), ' '));
        BOOST_TEST(!test_delimited(L"a ", lit(L'b'), ' '));

        BOOST_TEST(test_delimited(L"abc ", L"abc", ' '));
        BOOST_TEST(!test_delimited(L"abcd ", L"abc", ' '));

        BOOST_TEST(test_delimited(L"abc ", lit(L"abc"), ' '));
        BOOST_TEST(!test_delimited(L"abcd ", lit(L"abc"), ' '));

        BOOST_TEST(test_delimited(L"abc ", string, L"abc", ' '));
        BOOST_TEST(!test_delimited(L"abcd ", string, L"abc", ' '));

        BOOST_TEST(test_delimited(L"abc ", string(L"abc"), ' '));
        BOOST_TEST(!test_delimited(L"abcd ", string(L"abc"), ' '));
    }

    {   // test action
        using namespace boost::phoenix;
        using namespace boost::spirit::ascii;

        std::string str("abc");
        BOOST_TEST(test("abc", string[_1 = ref(str)]));
        BOOST_TEST(test_delimited("abc ", string[_1 = ref(str)], space));
    }

    {   // lazy strings
        using namespace boost::phoenix;
        using namespace boost::spirit::ascii;

        std::basic_string<char> s("abc");
        BOOST_TEST((test("abc", lit(val(s)))));
        BOOST_TEST((test("abc", string(val(s)))));

        std::basic_string<wchar_t> ws(L"abc");
        BOOST_TEST((test(L"abc", lit(ref(ws)))));
        BOOST_TEST((test(L"abc", string(ref(ws)))));
    }

    return boost::report_errors();
}