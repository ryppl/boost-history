//-----------------------------------------------------------------------------
// boost visitor_ptr.hpp header file
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

#ifndef BOOST_VISITOR_PTR_HPP
#define BOOST_VISITOR_PTR_HPP

namespace boost {

//////////////////////////////////////////////////////////////////////////
// function template visitor_ptr
//
// Adapts a function pointer for use as visitor capable of handling
// values of a single type.
//

template <typename T, typename R>
struct visitor_ptr_t
{
private: // representation
    typedef R (*visitor_t)(T);

    visitor_t visitor_;

public: // typedefs
    typedef void result_type;

public: // structors
    visitor_ptr_t(visitor_t& visitor)
      : visitor_(visitor)
    {
    }

public: // operators
    result_type operator()(T operand) const
    {
        visitor_(operand);
    }

    template <typename U>
    result_type operator()(const U& operand) const
    {
    }
};

template <typename R, typename T>
visitor_ptr_t<T, R> visitor_ptr(R (*visitor)(T))
{
    return visitor_ptr_t<T, R>(visitor);
}

} // namespace boost

#endif// BOOST_VISITOR_PTR_HPP
