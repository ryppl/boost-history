//-----------------------------------------------------------------------------
// boost visitor.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2003
// Eric Friedman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_VISITOR_HPP
#define BOOST_VISITOR_HPP

// dynamic visitation support
#include "boost/visitor/bad_visit.hpp"
#include "boost/visitor/dynamic_visitable.hpp"
#include "boost/visitor/dynamic_visitable_base.hpp"
#include "boost/visitor/dynamic_visitor.hpp"
#include "boost/visitor/dynamic_visitor_base.hpp"
#include "boost/visitor/dynamic_visitor_interface.hpp"
#include "boost/visitor/dynamic_visitor_return_base.hpp"

// static visitation support
#include "boost/visitor/static_visitable.hpp"
#include "boost/visitor/static_visitor.hpp"

// applications
#include "boost/visitor/apply_visitor.hpp"
#include "boost/visitor/try_dynamic_visit.hpp"
#include "boost/visitor/visitor_ptr.hpp"

#endif // BOOST_VISITOR_HPP
