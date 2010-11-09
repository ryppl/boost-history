//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#define BOOST_MPL_LIMIT_STRING_SIZE 64

#include <boost/static_string/c_str.hpp>
#include <boost/detail/lightweight_test.hpp>

#include <boost/mpl/char.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/deque.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/list_c.hpp>
#include <boost/mpl/set_c.hpp>
#include <boost/mpl/string.hpp>

#include <boost/mpl/find.hpp>
#include <boost/mpl/erase.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/insert_range.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/push_front.hpp>

#include <boost/mpl/for_each.hpp>

#include <string>
#include <cstring>
#include <iostream>

#if !defined(BOOST_NO_STATIC_ASSERT)
#define NOTHING ""
#endif

using namespace boost::static_string;
using namespace boost;

// Accept a string as a template parameter!
template<char const *sz>
struct greeting
{
    std::string say_hello() const
    {
        return sz;
    }
};

struct push_char
{
    push_char(std::string &str)
      : str_(&str)
    {}

    void operator()(char ch) const
    {
        this->str_->push_back(ch);
    }

    std::string *str_;
};

int main()
{
    // mpl::string<>
    {
    BOOST_TEST(0 == std::strcmp(
        c_str<mpl::string<> >::value
                             , ""
    ));
    BOOST_TEST(0 == std::strcmp(
        c_str<mpl::string<'Hell','o wo','rld!'> >::value
                             , "Hell" "o wo" "rld!"
    ));

    BOOST_TEST(0 == std::strcmp(
        c_str<mpl::string<'aaaa','aaaa','aaaa','aaaa','aaaa','aaaa','aaaa','aaaX'> >::value
                             , "aaaa" "aaaa" "aaaa" "aaaa" "aaaa" "aaaa" "aaaa" "aaaX"
    ));

    // test using as a template parameter
    greeting<c_str<mpl::string<'Hell','o wo','rld!'> >::value> g;
    BOOST_TEST("Hello world!" == g.say_hello());

    std::string result;
    mpl::for_each<mpl::string<'Hell','o wo','rld!'> >(push_char(result));
    BOOST_TEST("Hello world!" == result);
   
    }
    // mpl::vector<>
    {
    typedef mpl::vector<
        mpl::char_<'H'>,
        mpl::char_<'e'>,
        mpl::char_<'l'>,
        mpl::char_<'l'>,
        mpl::char_<'o'>,
        mpl::char_<' '>,
        mpl::char_<'w'>,
        mpl::char_<'o'>,
        mpl::char_<'r'>,
        mpl::char_<'l'>,
        mpl::char_<'d'>,
        mpl::char_<'!'>
    > str_type;
    BOOST_TEST(0 == std::strcmp(
        c_str<str_type>::value, "Hell" "o wo" "rld!"
    ));

    // test using as a template parameter
    greeting<c_str<str_type>::value> g;
    BOOST_TEST("Hello world!" == g.say_hello());

    std::string result;
    mpl::for_each<str_type>(push_char(result));
    BOOST_TEST("Hello world!" == result);
   
    }
    // mpl::list<>
    {
    typedef mpl::list<
        mpl::char_<'H'>,
        mpl::char_<'e'>,
        mpl::char_<'l'>,
        mpl::char_<'l'>,
        mpl::char_<'o'>,
        mpl::char_<' '>,
        mpl::char_<'w'>,
        mpl::char_<'o'>,
        mpl::char_<'r'>,
        mpl::char_<'l'>,
        mpl::char_<'d'>,
        mpl::char_<'!'>
    > str_type;
    BOOST_TEST(0 == std::strcmp(
        c_str<str_type>::value, "Hell" "o wo" "rld!"
    ));

    // test using as a template parameter
    greeting<c_str<str_type>::value> g;
    BOOST_TEST("Hello world!" == g.say_hello());

    std::string result;
    mpl::for_each<str_type>(push_char(result));
    BOOST_TEST("Hello world!" == result);
   
    }
    
    // mpl::deque<>
    {
    typedef mpl::deque<
        mpl::char_<'H'>,
        mpl::char_<'e'>,
        mpl::char_<'l'>,
        mpl::char_<'l'>,
        mpl::char_<'o'>,
        mpl::char_<' '>,
        mpl::char_<'w'>,
        mpl::char_<'o'>,
        mpl::char_<'r'>,
        mpl::char_<'l'>,
        mpl::char_<'d'>,
        mpl::char_<'!'>
    > str_type;
    BOOST_TEST(0 == std::strcmp(
        c_str<str_type>::value, "Hell" "o wo" "rld!"
    ));

    // test using as a template parameter
    greeting<c_str<str_type>::value> g;
    BOOST_TEST("Hello world!" == g.say_hello());

    std::string result;
    mpl::for_each<str_type>(push_char(result));
    BOOST_TEST("Hello world!" == result);
   
    }
    // mpl::set<>
    {
    typedef mpl::set<
        mpl::char_<'a'>,
        mpl::char_<'b'>,
        mpl::char_<'c'>,
        mpl::char_<'d'>
    > str_type;
        
    std::cout << c_str<str_type>::value << std::endl;
    BOOST_TEST(0 == std::strcmp(
        c_str<str_type>::value, "dcba"
    ));

    // test using as a template parameter
    greeting<c_str<str_type>::value> g;
    std::cout << g.say_hello() << std::endl;
    BOOST_TEST("dcba" == g.say_hello());

    std::string result;
    mpl::for_each<str_type>(push_char(result));
    std::cout << result << std::endl;
    BOOST_TEST("dcba" == result);
   
    }
    // mpl::range_c<>
    {
    typedef mpl::range_c<char, 'a','d'> str_type;
    BOOST_TEST(0 == std::strcmp(
        c_str<str_type>::value, "abc"
    ));

    // test using as a template parameter
    greeting<c_str<str_type>::value> g;
    BOOST_TEST("abc" == g.say_hello());

    std::string result;
    mpl::for_each<str_type>(push_char(result));
    BOOST_TEST("abc" == result);
   
    }
    // mpl::vector_c<>
    {
    BOOST_TEST(0 == std::strcmp(
        c_str<mpl::vector_c<char> >::value
                             , ""
    ));
    typedef mpl::vector_c<char, 'H','e','l','l','o',' ','w','o','r','l','d','!'> str_type;
    BOOST_TEST(0 == std::strcmp(
        c_str<str_type>::value, "Hell" "o wo" "rld!"
    ));

    // test using as a template parameter
    greeting<c_str<str_type>::value> g;
    BOOST_TEST("Hello world!" == g.say_hello());

    std::string result;
    mpl::for_each<str_type>(push_char(result));
    BOOST_TEST("Hello world!" == result);
   
    }
    // mpl::list_c<>
    {
    BOOST_TEST(0 == std::strcmp(
        c_str<mpl::list_c<char> >::value
                             , ""
    ));
    typedef mpl::list_c<char, 'H','e','l','l','o',' ','w','o','r','l','d','!'> str_type;
    BOOST_TEST(0 == std::strcmp(
        c_str<str_type>::value, "Hell" "o wo" "rld!"
    ));

    // test using as a template parameter
    greeting<c_str<str_type>::value> g;
    BOOST_TEST("Hello world!" == g.say_hello());

    std::string result;
    mpl::for_each<str_type>(push_char(result));
    BOOST_TEST("Hello world!" == result);
   
    }
    // mpl::set_c<>
    {
    BOOST_TEST(0 == std::strcmp(
        c_str<mpl::set_c<char> >::value
                             , ""
    ));
//    typedef mpl::set_c<char, 'H','e','l','l','o',' ','w','o','r','l','d','!'> str_type;
//    typedef mpl::set_c<char, 'a'> str_type;
//    typedef mpl::set<mpl::char_<'a'> > str_type;
//    typedef mpl::begin<str_type>::type i0;
//    typedef mpl::next<i0>::type i1;
//    typedef mpl::set_c<char, 'a', 'b', 'c', 'd'> str_type;
//    std::cout << c_str<str_type>::value << std::endl;
//    BOOST_TEST(0 == std::strcmp(
//        c_str<str_type>::value, "dcba"
//    ));
//
//    // test using as a template parameter
//    greeting<c_str<str_type>::value> g;
//    std::cout << g.say_hello() << std::endl;
//    BOOST_TEST("dcba" == g.say_hello());
//
//    std::string result;
//    mpl::for_each<str_type>(push_char(result));
//    std::cout << result << std::endl;
//    BOOST_TEST("dcba" == result);
   
    }
    return boost::report_errors();
}

