//-----------------------------------------------------------------------------
// boost visitor/visitor_ptr.hpp header file
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

#ifndef BOOST_VISITOR_VISITOR_PTR_HPP
#define BOOST_VISITOR_VISITOR_PTR_HPP

#include "boost/visitor/bad_visit.hpp"
#include "boost/visitor/static_visitor.hpp"
#include "boost/visitor/dynamic_visitor_base.hpp"
#include "boost/visitor/dynamic_visitor_return_base.hpp"
#include "boost/visitor/dynamic_visitor_interface.hpp"

#include "boost/mpl/bool.hpp"
#include "boost/mpl/if.hpp"
#include "boost/type_traits/add_reference.hpp"
#include "boost/type_traits/is_void.hpp"

namespace boost {

//////////////////////////////////////////////////////////////////////////
// function template visitor_ptr
//
// Adapts a function pointer for use as visitor capable of handling
// values of a single type. Throws bad_visit if inappropriately applied.
//
template <typename T, typename R>
class visitor_ptr_t
    : public static_visitor<R>
    , public mpl::if_<
          is_void<R>
        , dynamic_visitor_base
        , dynamic_visitor_return_base<R>
        >::type
    , public dynamic_visitor_interface<T>
{
private: // representation

    typedef R (*visitor_t)(T);

    visitor_t visitor_;

public: // typedefs

    typedef R result_type;

private: // private typedefs

    typedef typename add_reference<T>::type
        argument_fwd_type;

public: // structors

    explicit visitor_ptr_t(visitor_t visitor)
      : visitor_(visitor)
    {
    }

public: // static visitor interfaces

    result_type operator()(argument_fwd_type operand) const
    {
        return visitor_(operand);
    }

    template <typename U>
    result_type operator()(const U& operand) const
    {
        throw bad_visit();
    }

private: // helpers, for dynamic visitor interfaces (below)

    void visit_impl(
          argument_fwd_type operand
        , mpl::true_// is_void_return
        )
    {
        (*this)(operand);
    }

    void visit_impl(
          argument_fwd_type operand
        , mpl::false_// is_void_return
        )
    {
        result_value.reset( (*this)(operand) );
    }

private: // dynamic visitor interfaces

    virtual void visit(argument_fwd_type operand)
    {
        visit_impl(operand, typename is_void<result_type>::type());
    }

};

template <typename R, typename T>
inline visitor_ptr_t<T,R> visitor_ptr(R (*visitor)(T))
{
    return visitor_ptr_t<T,R>(visitor);
}

} // namespace boost

#endif// BOOST_VISITOR_VISITOR_PTR_HPP
