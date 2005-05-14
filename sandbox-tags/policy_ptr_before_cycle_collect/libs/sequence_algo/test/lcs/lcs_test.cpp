// (C) Copyright Craig Henderson 2002, 2003
//               cdm.henderson@virgin.net
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appears in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  The author makes no representations
// about the suitability of this software for any purpose.  It is
// provided "as is" without express or implied warranty.

#include <algorithm>
#include <sstream>
#include <iostream>
#include <time.h>
#include <list>
#include <vector>
#include <deque>
#include <typeinfo>              // typeid

#include "boost/config.hpp"
#define BOOST_DEBUG 1
#include "boost/assert.hpp"      // BOOST_ASSERT
#include "boost/config.hpp"
#include "boost/crc.hpp"
#include "boost/array.hpp"
#include "boost/sequence_algo/longest_common_subsequence.hpp"
#include "crc_string.hpp"


inline bool boost_error(char const * expr, char const * func, char const * file, long line)
{
    std::stringstream str;
    str << std::endl
        << std::endl
        << "Assertion Failed in " << func << std::endl
        << file << "(" << line << ")" << std::endl
        << "Expression: " << expr << std::endl << std::endl;
    std::cerr << str.str().c_str();

    return false;
}



// test function to calculate the lcs of two character strings
static void test_character_seq(const char *str1, const char *str2)
{
    std::cout << "Comparing: \"" << str1 << "\"" << std::endl
              << "         & \"" << str2 << "\"" << std::endl;

// VC6 chokes on using a std::string for the subsequence:
// the Dunkumware STL does not implement a push_back method
// on std::basic_string
#if defined(BOOST_MSVC)  &&  (_MSC_VER <= 1200)
    typedef
    std::vector<char>
    subsequence_t;
#else
    typedef
    std::string
    subsequence_t;
#endif

    signed short  llcs;
    subsequence_t subsequence;
    llcs = boost::longest_common_subsequence<signed short>(str1, str1+strlen(str1),
                                                           str2, str2+strlen(str2),
                                                           std::back_inserter<>(subsequence));

    std::cout << "Longest Common Subsequence: \"";
#if defined(BOOST_MSVC)  &&  (_MSC_VER <= 1200)
{
    subsequence_t::const_iterator it  = subsequence.begin();
    subsequence_t::const_iterator ite = subsequence.end();
    for (; it!=ite; ++it)
        std::cout << *it;
}
#else
    std::cout << subsequence;
#endif
    std::cout << "\"" << std::endl;
    std::cout << "Length is " << llcs << std::endl;

    std::allocator<signed short> alloc;
              
    BOOST_ASSERT(llcs == static_cast<signed short>(subsequence.size()));
    BOOST_ASSERT(llcs == boost::longest_common_subsequence_length<signed short>(str1, str1+strlen(str1),
                                                                                str2, str2+strlen(str2)));
    BOOST_ASSERT(llcs == boost::longest_common_subsequence_length<signed short>(str1, str1+strlen(str1),
                                                                                str2, str2+strlen(str2),
                                                                                alloc));

    subsequence_t::const_iterator       it  = subsequence.begin();
    const subsequence_t::const_iterator ite = subsequence.end();
    while (it != ite)
    {
        if (*str1 == *str2)
        {
            std::cout << "Keep     '" << *str1 << "'" << std::endl;
            ++str1;
            ++str2;
            ++it;
        }
        else if (*it != *str1)
            std::cout << "Deleted  '" << *str1++ << "' from first" << std::endl;
        else if (*it != *str2)
            std::cout << "Inserted '" << *str2++ << "' into second" << std::endl;
    }
    
    while (*str1 != 0)
        std::cout << "Deleted  '" << *str1++ << "' from first" << std::endl;

    while (*str2 != 0)
        std::cout << "Inserted '" << *str2++ << "' into second" << std::endl;
}


// test lcs of two arrays of strings
void test_string_array_seq(void)
{
    typedef cdmh::crc_string<char> string_type;
    typedef std::vector<string_type> array_t;
    array_t data1;
    data1.push_back("one");
    data1.push_back("three");
    data1.push_back("five");
    data1.push_back("seven");
    data1.push_back("nine");
    data1.push_back("eleven");
    data1.push_back("thirteen");
    data1.push_back("fifteen");

    array_t data2;
    data2.push_back("two");
    data2.push_back("three");
    data2.push_back("five");
    data2.push_back("eight");
    data2.push_back("nine");
    data2.push_back("eleven");
    data2.push_back("thirteen");
    data2.push_back("fifteen");

    std::cout << "Comparing string array" << std::endl;
    array_t::const_iterator it;
    for (it=data1.begin(); it !=data1.end(); ++it)
        std::cout << *it << "  ";
    std::cout << std::endl;
    for (it=data2.begin(); it !=data2.end(); ++it)
        std::cout << *it << "  ";
    std::cout << std::endl;

    signed short llcs;
    std::vector<string_type> subsequence;
    llcs = boost::longest_common_subsequence<signed short>(data1.begin(), data1.end(),
                                                           data2.begin(), data2.end(),
                                                           std::back_inserter<>(subsequence));

    std::cout << "Longest Common Subsequence Length is " << llcs << std::endl;
              
    BOOST_ASSERT(llcs == static_cast<signed short>(subsequence.size()));
    BOOST_ASSERT(llcs == boost::longest_common_subsequence_length<signed short>(data1.begin(), data1.end(),
                                                                                data2.begin(), data2.end()));
}


// test lcs of two numeric arrays, using boost::array
void test_number_array_seq(void)
{
#ifdef BOOST_ARRAY_HPP
    typedef boost::array<int, 8> array_t;

    array_t data1 = { 1, 3, 5, 7, 9, 11, 13, 15 };
    array_t data2 = { 2, 3, 5, 8, 9, 11, 13, 15 };

    array_t::const_iterator it;
    std::cout << "Comparing numeric array" << std::endl;
    for (it=data1.begin(); it !=data1.end(); ++it)
        std::cout << *it << "  ";
    std::cout << std::endl;
    for (it=data2.begin(); it !=data2.end(); ++it)
        std::cout << *it << "  ";
    std::cout << std::endl;

    signed short llcs;
    std::vector<array_t::value_type> subsequence;
    llcs = boost::longest_common_subsequence<signed short>(data1.begin(), data1.end(),
                                                           data2.begin(), data2.end(),
                                                           std::back_inserter<>(subsequence));
    std::cout << "Longest Common Subsequence length is " << llcs << std::endl;
    BOOST_ASSERT(llcs == static_cast<signed short>(subsequence.size()));
    BOOST_ASSERT(llcs == boost::longest_common_subsequence_length<signed short>(data1.begin(), data1.end(),
                                                                                data2.begin(), data2.end()));
#endif
}



template<typename T>
void test_container_type(int size)
{
#ifdef DBG_SCOPE
    DBG_SCOPE("Container Test: %s", typeid(T).name());
#endif

    std::cout << "Container Test " << typeid(T).name() << " - " << size << " elements..." << std::endl;
    T data1, data2;
    std::generate_n<>(std::back_inserter<>(data1), size, rand);
    std::generate_n<>(std::back_inserter<>(data2), size, rand);

    signed short llcs;
#if  defined(BOOST_MSVC)  &&  (_MSC_VER <= 1200)
    std::vector<T::value_type> subsequence;
#else
    std::vector<typename T::value_type> subsequence;
#endif
    llcs = boost::longest_common_subsequence<signed short>(data1.begin(), data1.end(),
                                                           data2.begin(), data2.end(),
                                                           std::back_inserter<>(subsequence));
    std::cout << "Longest Common Subsequence length is " << llcs << std::endl;
    BOOST_ASSERT(llcs == static_cast<signed short>(subsequence.size()));
    BOOST_ASSERT(llcs == boost::longest_common_subsequence_length<signed short>(data1.begin(), data1.end(),
                                                                                data2.begin(), data2.end()));
}



int main(int, char **)
{
    srand(time(NULL));
    std::cout << "LCS Test Program" << std::endl << std::endl;

    try
    {
        // bug reported by graydon hoare <graydon@redhat.com>
        test_character_seq("ab ccc", "a  b ccc");
        test_character_seq("aaaa aaaa", "a  a a  a");

        // this is the test documented by David Eppstein at
        // http://www1.ics.uci.edu/~eppstein/161/960229.html
        test_character_seq("nematode knowledge", "empty bottle");
        test_character_seq("empty bottle", "nematode knowledge");

        // character array tests
        test_character_seq("little jack horner.", "sat in a corner");
        test_character_seq("jack and jill went up the hill", "to fetch a pale of water");
        test_character_seq("to fetch a pale of water", "jack and jill went up the hill");
/*
        // test for string array difference
        test_string_array_seq();

        // test for numeric array difference
        test_number_array_seq();

        test_container_type< std::list<int> >(500);
        test_container_type< std::deque<int> >(5000);
        for (int loop=1; loop<25; ++loop)
            test_container_type< std::vector<int> >(1000);
*/
    }
    catch(const std::exception &e)
    {
        std::cerr << "Caught exception";
        if (e.what())
            std::cerr << ": " << e.what();
        std::cerr << std::endl;
    }
    std::cout << std::endl;

    return 0;
}
