//-----------------------------------------------------------------------------
// boost detail/variant_workaround.hpp header file
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

#ifndef BOOST_DETAIL_VARIANT_WORKAROUND_HPP
#define BOOST_DETAIL_VARIANT_WORKAROUND_HPP

#include "boost/config.hpp"

namespace boost {
namespace detail {

// [Leave the base undefined on conforming compilers:]
struct variant_workaround_base;

} // namespace detail
} // namespace boost

#if defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

#include "boost/mpl/bool_c.hpp"
#include "boost/type_traits/is_convertible.hpp"

namespace boost {
namespace detail {

struct variant_workaround_base
{
};

} // namespace detail

template <typename T>
struct is_variant
{
    typedef mpl::bool_c<
          is_convertible<T, const volatile detail::variant_workaround_base>::value
        > type;
    BOOST_STATIC_CONSTANT(bool, value = type::value);
};

} // namespace boost

#endif // defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

#endif // BOOST_DETAIL_VARIANT_WORKAROUND_HPP
