//-----------------------------------------------------------------------------
// boost type_traits/is_better_conversion.hpp header file
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

#ifndef BOOST_TYPE_TRAITS_IS_BETTER_CONVERSION_HPP
#define BOOST_TYPE_TRAITS_IS_BETTER_CONVERSION_HPP

#include "boost/detail/workaround.hpp"
#include "boost/type_traits/config.hpp"
#include "boost/type_traits/detail/yes_no_type.hpp"

#include "boost/mpl/aux_/value_wknd.hpp"
#include "boost/mpl/and.hpp"
#include "boost/mpl/bool.hpp"
#include "boost/mpl/not.hpp"

namespace boost {

namespace detail {

#if !BOOST_WORKAROUND(__GNUC__, BOOST_TESTED_AT(3)) \
 && !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x0551))

template <typename From, typename To1, typename To2>
struct is_better_conversion_impl
{
private:

    static ::boost::type_traits::no_type BOOST_TT_DECL check_(...);
    static ::boost::type_traits::yes_type BOOST_TT_DECL check_(To1, int);

    // Alexander Nasonov suggested (comp.lang.c++.moderated, 4 Jul 2002)
    // the following fix for the ambiguous conversion case.
    //
    // [MSVC6: Following must come *after* above overloads.]
    //
    template <typename Int>
    static ::boost::type_traits::no_type BOOST_TT_DECL check_(To2, Int);

    static From from_;

public:

    BOOST_STATIC_CONSTANT(bool, value = (
          sizeof(check_(from_,0)) == sizeof(::boost::type_traits::yes_type )
        ));

    typedef ::boost::mpl::bool_< value > type;

};

#else// g++, borland workaround

struct is_better_conversion_any_t
{
    template <typename T> is_better_conversion_any_t(const T&);
};

template <typename To1, typename To2> struct is_better_conversion_checker
{
    template <typename Int>
    static boost::type_traits::no_type _m_check(To2, Int);
    static boost::type_traits::no_type _m_check(is_better_conversion_any_t, ...);

    static boost::type_traits::yes_type _m_check(To1, int);
};

template <typename From, typename To1, typename To2>
struct is_better_conversion_impl
{
private:

    typedef is_better_conversion_impl self_t;

    static From _m_from;

public:

    BOOST_STATIC_CONSTANT(bool, value = (
          sizeof(is_better_conversion_checker<To1,To2>::_m_check(_m_from, 0))
          == sizeof(::boost::type_traits::yes_type)
        ));

    typedef ::boost::mpl::bool_<
          BOOST_MPL_AUX_VALUE_WKND(self_t)::value
        > type;

}; 

#endif // g++, borland workaround

} // namespace detail

template <typename From, typename To1, typename To2>
struct is_ambiguous_conversion
    : mpl::and_< // is_better_impl< 1,2 > && is_better_impl< 2,1 >
          detail::is_better_conversion_impl< From, To1,To2 >
        , detail::is_better_conversion_impl< From, To2,To1 >
        >
{
};

template <typename From, typename To1, typename To2>
struct is_better_conversion
    : mpl::and_< // is_better_impl< 1,2 > && !is_ambiguous< 1,2 >
          detail::is_better_conversion_impl< From, To1,To2 >
        , mpl::not_< detail::is_better_conversion_impl< From, To2,To1 > >
        >
{
};

} // namespace boost

#endif // BOOST_TYPE_TRAITS_IS_BETTER_CONVERSION_HPP
