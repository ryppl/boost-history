//-----------------------------------------------------------------------------
// boost mpl/wrapper/unary_predicate.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-01
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_WRAPPER_UNARY_PREDICATE_HPP
#define BOOST_MPL_WRAPPER_UNARY_PREDICATE_HPP

#include "boost/mpl/unary_function.hpp"
#include "boost/mpl/bool_t.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

template<typename Predicate>
struct unary_predicate
{
    template<typename U> struct apply
    {
        typedef mpl::unary_function<Predicate,U> pred_;
        typedef mpl::bool_t<pred_::value> type;
        BOOST_STATIC_CONSTANT(bool, value = pred_::value);
    };
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_WRAPPER_UNARY_PREDICATE_HPP
