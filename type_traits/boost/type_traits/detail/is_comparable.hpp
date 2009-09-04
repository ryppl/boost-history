// Copyright 2009 Robert Stewart, Steven Watanabe, Roman Perepelitsa & Frédéric Bron
//
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#include <boost/config.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/detail/yes_no_type.hpp>

// should be the last #include
#include <boost/type_traits/detail/bool_trait_def.hpp>

namespace boost {
namespace detail {

// This namespace ensures that ADL does not mess things up.
namespace BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl) {

// a type returned from comparison operator when no such operator is found in the
// type's own namespace
struct tag { } ;

// any soaks up implicit conversions and makes the following
// comparison operators less-preferred than any other such operators that
// might be found via ADL.
struct any { template <class T> any(T const&) ; } ;

// when operator< is not available, this one is used
tag operator BOOST_TT_TRAIT_OP (const any&, const any&) ;

::boost::type_traits::yes_type is_bool(bool) ; // this version is preferred for types convertible to bool
::boost::type_traits::no_type is_bool(...) ; // this version is used otherwise

template < typename T, typename U >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl) {
	static T make_T() ;
	static U make_U() ;
	static const bool value =
		sizeof(is_bool(make_T() BOOST_TT_TRAIT_OP make_U())) == sizeof(::boost::type_traits::yes_type) ;
} ;

} // namespace impl
} // namespace detail

BOOST_TT_AUX_BOOL_TRAIT_DEF2(BOOST_TT_TRAIT_NAME,T,U=T,(::boost::detail::BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl)::BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl)<T,U>::value))

} // namespace boost

#include <boost/type_traits/detail/bool_trait_undef.hpp>
