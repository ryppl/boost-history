//-----------------------------------------------------------------------------
// boost libs/incomplete/incomplete_test.cpp source file
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

#include "pimpl_test.h"

#include "boost/test/minimal.hpp"

//////////////////////////////////////////////////////////////////////////
// function test_main
//
int test_main( int, char *[] )
{
    static const int value = 42;
    pimpl_test test(value);

    BOOST_CHECK(test.value() == value);

    return boost::exit_success;
}
