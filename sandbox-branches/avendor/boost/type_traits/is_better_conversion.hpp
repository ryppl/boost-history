//-----------------------------------------------------------------------------
// boost type_traits/is_better_conversion.hpp header file
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

#ifndef BOOST_TYPE_TRAITS_IS_BETTER_CONVERSION_HPP
#define BOOST_TYPE_TRAITS_IS_BETTER_CONVERSION_HPP

#include "boost/type_traits/config.hpp"
#include "boost/type_traits/detail/yes_no_type.hpp"

#include "boost/mpl/bool_c.hpp"
#include "boost/mpl/logical.hpp"
#include "boost/type_traits/is_same.hpp"

namespace boost {

namespace detail {

#if defined(__GNUC__)

struct is_better_conversion_any_t
{
    template <typename T> is_better_conversion_any_t(const T&);
    template <typename T> is_better_conversion_any_t(T&);
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
    static From _m_from;
    static bool const value =
		sizeof( is_better_conversion_checker<To1,To2>::_m_check(_m_from, 0) ) 
        == sizeof(::boost::type_traits::yes_type);

	typedef ::boost::mpl::bool_c<value> type;
};

#else// !defined(__GNUC__)

template <typename From, typename To1, typename To2>
struct is_better_conversion_impl
{
private:
    // Alexander Nasonov suggested (comp.lang.c++.moderated, 4 Jul 2002)
    // the following fix for the ambiguous conversion case:
    template <typename Int>
    static ::boost::type_traits::no_type BOOST_TT_DECL check_(To2, Int);
    static ::boost::type_traits::no_type BOOST_TT_DECL check_(...);

    static ::boost::type_traits::yes_type BOOST_TT_DECL check_(To1, int);

    static From from_;

public:
	typedef ::boost::mpl::bool_c<
          ( sizeof( check_(from_,0) ) == sizeof( ::boost::type_traits::yes_type ) )
        > type;

    void foo(); // avoid warning about all members being private
};

#endif // __GNUC__ workaround

} // namespace detail

template <typename From, typename To1, typename To2>
struct is_better_conversion
{
    typedef typename mpl::logical_and< // !is_same< 1,2 > && is_better< 1,2 > && !is_better< 2,1 >
		  mpl::logical_not< is_same<To1, To2> >
        , detail::is_better_conversion_impl<From, To1, To2>
        , mpl::logical_not<
              detail::is_better_conversion_impl<From, To2, To1>
            >
        >::type type;

    BOOST_STATIC_CONSTANT(bool, value = type::value);
};

} // namespace boost

#endif // BOOST_TYPE_TRAITS_IS_BETTER_CONVERSION_HPP
