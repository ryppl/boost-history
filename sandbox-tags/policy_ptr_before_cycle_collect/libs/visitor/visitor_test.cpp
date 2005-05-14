//-----------------------------------------------------------------------------
// boost libs/visitor/visitor_test.cpp source file
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

// Headers whose facilities are to be tested:
#include "boost/dynamic_visitor.hpp"
#include "boost/dynamic_visitable.hpp"
#include "boost/apply_visitor.hpp"

#include "boost/mpl/list.hpp"

#include "boost/test/minimal.hpp"

//////////////////////////////////////////////////////////////////////////

class base
    : public boost::dynamic_visitable_base
{
};

class derivedA
    : public boost::dynamic_visitable<
          derivedA
        , base
        >
{
};

class derivedB
    : public boost::dynamic_visitable<
          derivedB
        , base
        >
{
};

class derivedB2
    : public boost::dynamic_visitable<
          derivedB2
        , derivedB
        >
{
};

//////////////////////////////////////////////////////////////////////////

struct visitor
    : boost::dynamic_visitor<
          visitor
        , boost::mpl::list<derivedA, derivedB, derivedB2>
        , int
        >
{
    int operator()(const derivedA& operand)
    {
        return 1;
    }

    int operator()(const derivedB& operand)
    {
        return 2;
    }

    int operator()(const derivedB2& operand)
    {
        return 3;
    }
};

//////////////////////////////////////////////////////////////////////////
// function test_main
//
int test_main( int, char *[] )
{
    visitor v;

    try
    {
        derivedB2 d;
        base& b = d;

        BOOST_CHECK(( 
              boost::apply_visitor(v, b) == 3
            ));
    }
    catch (boost::bad_visit& e)
    {
        BOOST_ERROR("Visit failed unexpectedly.");
    }

    return boost::exit_success;
}
