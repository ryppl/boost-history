//-----------------------------------------------------------------------------
// boost visitor_ptr.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002-2003
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

#include "boost/static_visitor.hpp"
#include "boost/type_traits/add_reference.hpp"

namespace boost {

//////////////////////////////////////////////////////////////////////////
// function template visitor_ptr
//
// Adapts a function pointer for use as visitor capable of handling
// values of a single type.
//

template <typename T, typename R>
class visitor_ptr_t
    : public static_visitor<>
    , public dynamic_visitor_base
    , public dynamic_visitor_interface<T>
{
private: // representation

    typedef R (*visitor_t)(T);

    visitor_t visitor_;

public: // typedefs

    typedef void result_type;

private: // private typedefs

    typedef typename add_reference<T>::type
        argument_fwd_type;

public: // structors

    visitor_ptr_t(visitor_t& visitor)
      : visitor_(visitor)
    {
    }

public: // static visitor interfaces

    void operator()(argument_fwd_type operand) const
    {
        visitor_(operand);
    }

    template <typename U>
    void operator()(const U& operand) const
    {
    }

private: // dynamic visitor interfaces

    virtual void visit(argument_fwd_type operand)
    {
        (*this)(operand);
    }

};

template <typename R, typename T>
visitor_ptr_t<T,R> visitor_ptr(R (*visitor)(T))
{
    return visitor_ptr_t<T,R>(visitor);
}

} // namespace boost

#endif// BOOST_VISITOR_PTR_HPP
