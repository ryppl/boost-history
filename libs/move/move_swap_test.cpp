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

#include "boost/move.hpp"
#include "boost/test/minimal.hpp"

#include <vector>
#include "boost/utility.hpp" // for boost::noncopyable

namespace ns {

//////////////////////////////////////////////////////////////////////////
// class my_moveable
//
// Noncopyable, nonswappable but moveable.
//
class my_moveable
    : public boost::moveable< my_moveable >
    , boost::noncopyable
{
private: // represntation
    std::vector<char> v_;

public: // structors
    explicit my_moveable(char c)
        : v_(300, c)
    {
    }

    my_moveable(boost::move_source<my_moveable> source)
        : v_()
    {
        *this = source;
    }

public: // modifiers
    my_moveable& operator=(boost::move_source<my_moveable> source)
    {
        my_moveable& rhs = source.get();

        v_.swap(rhs.v_);
        rhs.v_.clear();
    }

public: // operators
    bool operator==(const my_moveable& rhs) const
    {
        return v_ == rhs.v_;
    }
};

//////////////////////////////////////////////////////////////////////////
// class my_swappable
//
// Noncopyable, nonmoveable but swappable.
//
// NOTE: move_swap only handles swap on compilers properly supporting 
// argument-dependent (Koenig) function lookup.
//

#if !defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)

class my_swappable
    : boost::noncopyable
{
private: // representation
    std::vector<char> v_;

public: // structors
    explicit my_swappable(char c)
        : v_(300, c)
    {
    }

public: // modifiers
    void swap(my_swappable& rhs)
    {
        v_.swap(rhs.v_);
    }

public: // operators
    bool operator==(const my_swappable& rhs) const
    {
        return v_ == rhs.v_;
    }
};

void swap(my_swappable& lhs, my_swappable& rhs)
{
    lhs.swap(rhs);
}

#endif // !defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

} // namespace ns

//////////////////////////////////////////////////////////////////////////
// function template swap_test
//
// Tests the specified type for proper behavior under boost::swap.
//
template <typename T>
void swap_test()
{
    T x('x');
    T x_orig('x');

    T y('y');
    T y_orig('y');

    BOOST_REQUIRE( x == x_orig );
    BOOST_REQUIRE( y == y_orig );
    BOOST_REQUIRE( !(x == y) );

    boost::move_swap(x, y);

    BOOST_CHECK( x == y_orig );
    BOOST_CHECK( y == x_orig );    
}

//////////////////////////////////////////////////////////////////////////
// function test_main
//
int test_main( int, char *[] )
{
    // test boost::swap on primitive, copyable type
    swap_test< char >();

    // test boost::swap on swappable, noncopyable type
    swap_test< ns::my_swappable >();

#if !defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)

    // test boost::swap on moveable, noncopyable type
    swap_test< ns::my_moveable >();

#endif // !defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)

    return boost::exit_success;
}
