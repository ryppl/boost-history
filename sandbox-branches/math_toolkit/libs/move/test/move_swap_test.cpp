//-----------------------------------------------------------------------------
// boost libs/move/move_swap_test.cpp source file
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

#include <boost/move.hpp>
#include <boost/noncopyable.hpp> // for boost::noncopyable
#include <boost/assert.hpp>

#include <vector>

namespace ns {

//////////////////////////////////////////////////////////////////////////
// class my_moveable
//
// Noncopyable, nonswappable but moveable.
//
class my_moveable
    : public boost::movable< my_moveable >
{
private: // represntation
    std::vector<char> v_;

public: // structors
    explicit my_moveable(char c)
        : v_(300, c)
    {
    }

    my_moveable(boost::move_from<my_moveable> source)
        : v_()
    {
        *this = source;  // invoke the move assignment operator
    }

public: // modifiers
    my_moveable& operator=(boost::move_from<my_moveable> source)
    {
        my_moveable& rhs = *source;

        v_.swap(rhs.v_);
        rhs.v_.clear(); // not sure why we're doing this
        return *this;
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
}  // namespace ns

#if !defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP) // || BOOST_WORKAROUND(BOOST_MSVC, == 1310)
namespace
ns
{
#endif

void swap(ns::my_swappable& lhs, ns::my_swappable& rhs)
{
    lhs.swap(rhs);
}

#if !defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP) // || BOOST_WORKAROUND(BOOST_MSVC, == 1310)
} // namespace ns
#endif 

//////////////////////////////////////////////////////////////////////////
// function template swap_test
//
// Tests the specified type for proper behavior under boost::move_swap.
//
template <typename T>
void swap_test()
{
    T x('x');
    T x_orig('x');

    T y('y');
    T y_orig('y');

    BOOST_ASSERT( x == x_orig );
    BOOST_ASSERT( y == y_orig );
    BOOST_ASSERT( !(x == y) );

#ifdef BOOST_MSVC
# pragma warning(disable: 4675) // resolved overload was found by argument-dependent lookup -- duh!
#endif
    
#if !defined(__EDG__) && BOOST_WORKAROUND(__GNUC__, == 2)           \
    || defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)                  \
    || defined(BOOST_FUNCTION_SCOPE_USING_DECLARATION_BREAKS_ADL)
# define swap boost::move_swap
#else
    using boost::swap;
#endif 
    swap(x, y);

    BOOST_ASSERT( x == y_orig );
    BOOST_ASSERT( y == x_orig );
}

//////////////////////////////////////////////////////////////////////////
// function test_main
//
int main()
{
    // test boost::move_swap on primitive, copyable type
    swap_test< char >();

    // test boost::move_swap on moveable, noncopyable type
    swap_test< ns::my_moveable >();

#if !defined(BOOST_NO_MOVE_SWAP_BY_OVERLOAD)

    // test boost::move_swap on swappable, noncopyable type
    swap_test< ns::my_swappable >();

#endif // !defined(BOOST_NO_MOVE_SWAP_BY_OVERLOAD)
    
    return 0;
}
