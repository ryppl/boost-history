//-----------------------------------------------------------------------------
// boost dynamic_visitable.hpp header file
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

#ifndef BOOST_DYNAMIC_VISITABLE_HPP
#define BOOST_DYNAMIC_VISITABLE_HPP

#include "boost/visitor/bad_visit.hpp"
#include "boost/visitor/dynamic_visitor_base.hpp"
#include "boost/visitor/try_dynamic_visit.hpp"

// Include dynamic_visitable_base header as convenience:
#include "boost/visitor/dynamic_visitable_base.hpp"

namespace boost {

//////////////////////////////////////////////////////////////////////////
// class template dynamic_visitable<Deriving>
//
// Publicly inherited by Deriving to implement inheritance-based dynamic
// visitation.
//
template <typename Deriving, typename Base>
class dynamic_visitable
    : public Base
{
private:
    typedef dynamic_visitor_interface<Deriving>
        target_visitor_t;

    void apply_visitor(dynamic_visitor_base& visitor)
    {
        // Downcast *this...
        Deriving& operand = *static_cast< Deriving * >(this);

        // ...and attempt to apply the given visitor...
        if ( !try_dynamic_visit(visitor, operand) )
            // ...but throw upon failure to visit:
            throw bad_visit();
    }

protected:
    ~dynamic_visitable()
    {
    }
};

} // namespace boost

#endif // BOOST_DYNAMIC_VISITABLE_HPP
