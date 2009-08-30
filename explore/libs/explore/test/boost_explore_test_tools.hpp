#ifndef BOOST_EXPLORE_TEST_TOOLS_H
#define BOOST_EXPLORE_TEST_TOOLS_H

// Boost.Explore library

// Copyright Jeffrey Faust 2009. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

// some testing tools to help test both char and wchar_t streams

#include <boost/mpl/list.hpp>

#include <sstream>
#include <string>
#include <list>

typedef boost::mpl::list<char, wchar_t> test_types;

template<typename C>
struct test_traits;

template<>
struct test_traits<char>
{
    typedef std::stringstream stream_type;
    typedef std::string string_type;
};

template<>
struct test_traits<wchar_t>
{
    typedef std::wstringstream stream_type;
    typedef std::wstring string_type;
};

void reset(std::stringstream& s)
{
    s.str("");
}

void reset(std::wstringstream& s)
{
    s.str(L"");
}

std::string output(const std::stringstream& s)
{
    return s.str();
}

std::string output(const std::wstringstream& s)
{
    std::wstring ws = s.str();
    return std::string(ws.begin(), ws.end());
}

template<typename C>
const C* str_to(const char* in);

template<>
const char* str_to<char>(const char* in)
{
    return in;
}

template<>
const wchar_t* str_to<wchar_t>(const char* in)
{
    // we never free this memory.  OK for tests.
    static std::list<std::wstring> storage;
    std::string instr(in);
    storage.push_back(std::wstring(instr.begin(), instr.end()));
    std::wstring& out = storage.back();
    return out.c_str();
}

#endif
