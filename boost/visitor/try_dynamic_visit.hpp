//-----------------------------------------------------------------------------
// boost visitor/try_dynamic_visit.hpp header file
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

#ifndef BOOST_VISITOR_TRY_DYNAMIC_VISIT_HPP
#define BOOST_VISITOR_TRY_DYNAMIC_VISIT_HPP

#include "boost/visitor/dynamic_visitor_base.hpp"
#include "boost/visitor/dynamic_visitor_interface.hpp"

namespace boost {

//////////////////////////////////////////////////////////////////////////
// function template try_dynamic_visitor
//
// Attempts to provide the given dynamic visitor with the given
// visitation operand. Returns false if the visitor does not support
// operand's of the given type; returns true is the operation succeeds.
//
// NOTE: Exceptions may propagate from invocation of given visitor.
//

template <typename T>
inline bool try_dynamic_visit(dynamic_visitor_base& visitor, T& operand)
{
    typedef dynamic_visitor_interface<T>
        visitor_interface;

    // Attempt to downcast the given visitor:
    visitor_interface* target =
        dynamic_cast< visitor_interface* >(&visitor);

    // Return possible failure indication:
    if (!target)
        return false;

    // Otherwise, apply the visitor:
    target->visit( operand );
    return true;
}

} // namespace boost

#endif // BOOST_VISITOR_TRY_DYNAMIC_VISIT_HPP
