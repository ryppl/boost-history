//-----------------------------------------------------------------------------
// boost visitor/dynamic_visitable_base.hpp header file
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

#ifndef BOOST_VISITOR_DYNAMIC_VISITABLE_BASE_HPP
#define BOOST_VISITOR_DYNAMIC_VISITABLE_BASE_HPP

#include "boost/config.hpp"
#include "boost/visitor/dynamic_visitor_base.hpp"
#include "boost/type_traits/is_base_and_derived.hpp"

#include "boost/mpl/aux_/lambda_support.hpp" // used by is_dynamic_visitable

namespace boost {

//////////////////////////////////////////////////////////////////////////
// class dynamic_visitable_base
//
// Serves as an abstract base to all types supporting dynamic visitation.
//
struct dynamic_visitable_base
{
    virtual void apply_visitor(dynamic_visitor_base&) = 0;

    virtual ~dynamic_visitable_base()
    {
    }
};


//////////////////////////////////////////////////////////////////////////
// metafunction is_dynamic_visitable
//
// Value metafunction indicates whether the specified type is visitable
// by a dynamic visitor.
// 
// NOTE: This template never needs to be specialized!
//
template <typename T>
struct is_dynamic_visitable
{
    typedef typename is_base_and_derived<
          dynamic_visitable_base
        , T
        >::type type;

    BOOST_STATIC_CONSTANT(bool, value = type::value);

    BOOST_MPL_AUX_LAMBDA_SUPPORT(1,is_dynamic_visitable,(T))
};

} // namespace boost

#endif // BOOST_VISITOR_DYNAMIC_VISITABLE_BASE_HPP
