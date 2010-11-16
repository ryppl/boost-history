// Copyright 2010 Robert Stewart, Steven Watanabe, Roman Perepelitsa & Frédéric Bron
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
#include <boost/type_traits/is_void.hpp>
#include <boost/typeof/typeof.hpp>

// should be the last #include
#include <boost/type_traits/detail/bool_trait_def.hpp>

namespace boost {
namespace detail {

// This namespace ensures that ADL does not mess things up.
namespace BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl) {

// a type returned from comparison operator when no such operator is found in the
// type's own namespace
struct tag { };

// any soaks up implicit conversions and makes the following
// comparison operators less-preferred than any other such operators that
// might be found via ADL.
struct any { template <class T> any(T const&); };

// when operator BOOST_TT_TRAIT_OP is not available, this one is used
tag operator BOOST_TT_TRAIT_OP (const any&, const any&);

namespace {
	template <typename T>
	typename ::boost::remove_cv<T>::type make();
}

template < typename LHS, typename RHS >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_returns_void) {
	struct returns_void_t {
		template <typename T> friend int operator,(T const &, returns_void_t);
	};
	static ::boost::type_traits::yes_type returns_void(returns_void_t);
	static ::boost::type_traits::no_type returns_void(int);
	static bool const value=
		sizeof(::boost::type_traits::yes_type)==sizeof(returns_void((make<LHS>() BOOST_TT_TRAIT_OP make<RHS>(),returns_void_t())));
};

template < typename LHS, typename RHS, typename RET, typename RETURNS_VOID >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl1);

// LHS BOOST_TT_TRAIT_OP RHS does not return void, checks if it is convertible to RET
template < typename LHS, typename RHS, typename RET >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl1)< LHS, RHS, RET, ::boost::false_type > {
	static ::boost::type_traits::yes_type is_convertible_to_RET(RET); // this version is preferred for types convertible to RET
	static ::boost::type_traits::no_type is_convertible_to_RET(...); // this version is used otherwise

	static const bool value=
			sizeof(is_convertible_to_RET(make<LHS>() BOOST_TT_TRAIT_OP make<RHS>()))==sizeof(::boost::type_traits::yes_type);
};

// LHS BOOST_TT_TRAIT_OP RHS returns void!=RET
template < typename LHS, typename RHS, typename RET >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl1)< LHS, RHS, RET, ::boost::true_type > {
	static const bool value=false;
};

// checks for return type if 3rd template parameter RET is non void
template < typename LHS, typename RHS, typename RET >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl) {
	static const bool value=
			BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl1)< LHS, RHS, RET, typename ::boost::integral_constant< bool, BOOST_JOIN(BOOST_TT_TRAIT_NAME,_returns_void)< LHS, RHS >::value > >::value;
};

// in case we do not want to check for return type
tag operator,(tag, int);

// do not check for return type if 3rd template parameter RET is void
template < typename LHS, typename RHS >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl)< LHS, RHS, void > {
	static ::boost::type_traits::yes_type check(int); // this version is preferred for types convertible to RET
	static ::boost::type_traits::no_type check(tag); // this version is used otherwise

	static const bool value=
		sizeof(check(((make<LHS>() BOOST_TT_TRAIT_OP make<RHS>()),0)))==sizeof(::boost::type_traits::yes_type);
};

template < typename RHS, typename RET >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl)< void, RHS, RET > {
	static const bool value=false;
};

template < typename LHS, typename RET >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl)< LHS, void, RET > {
	static const bool value=false;
};

template < typename LHS >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl)< LHS, void, void > {
	static const bool value=false;
};

template < typename RHS >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl)< void, RHS, void > {
	static const bool value=false;
};

template < typename RET >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl)< void, void, RET > {
	static const bool value=false;
};

template <>
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl)< void, void, void > {
	static const bool value=false;
};

} // namespace impl
} // namespace detail

BOOST_TT_AUX_BOOL_TRAIT_DEF3(BOOST_TT_TRAIT_NAME,LHS,RHS=LHS,RET=BOOST_TT_DEFAULT_RET,(::boost::detail::BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl)::BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl)<LHS,RHS,RET>::value))

} // namespace boost

#include <boost/type_traits/detail/bool_trait_undef.hpp>
