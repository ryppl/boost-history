//-----------------------------------------------------------------------------
// boost libs/move/auto_mover_test.cpp source file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002
// Eric Friedman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#include "boost/auto_mover.hpp"

#include <string>

#include "boost/test/minimal.hpp"
#include "boost/cstdlib.hpp"

//////////////////////////////////////////////////////////////////////////
// class boost::move_traits<std::string>
//
// Implements nothrow move for std::string.
//
namespace boost {

template <>
struct move_traits< std::string >
{
    static void move(void* dest, std::string& src)
    {
        std::string* p = new(dest) std::string; // nothrow
        p->swap(src); // nothrow
    }
};

} // namespace boost

//////////////////////////////////////////////////////////////////////////
// function source
// 
// Returns a std::string via an auto_mover.
//
boost::auto_mover<std::string> source()
{
    std::string str("abcd");
    str += "efg";

    return boost::auto_mover<std::string>(str);
}

//////////////////////////////////////////////////////////////////////////
// function test_main
//
int test_main( int, char *[] )
{
    boost::auto_mover<std::string> sink(source());
    BOOST_TEST(*sink == "abcdefg");

    return boost::exit_success;
}
