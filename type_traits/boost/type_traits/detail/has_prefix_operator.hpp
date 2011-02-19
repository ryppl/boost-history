//  (C) Copyright 2009-2011 Frédéric Bron, Robert Stewart, Steven Watanabe & Roman Perepelitsa
//
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#include <boost/config.hpp>
#include <boost/type_traits/ice.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_fundamental.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_reference.hpp>

// should be the last #include
#include <boost/type_traits/detail/bool_trait_def.hpp>

// cannot include this header without getting warnings of the kind:
// gcc:
//    warning: value computed is not used
//    warning: comparison between signed and unsigned integer expressions
// msvc:
//    warning C4146: unary minus operator applied to unsigned type, result still unsigned
//    warning C4804: '-' : unsafe use of type 'bool' in operation
// cannot find another implementation -> declared as system header to suppress these warnings.
#if defined(__GNUC__) && ((__GNUC__==3 && __GNUC_MINOR__>=1) || (__GNUC__>3))
#   pragma GCC system_header
#elif BOOST_MSVC
#   pragma warning ( push )
#   pragma warning ( disable : 4146 4804 )
#endif

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
tag operator BOOST_TT_TRAIT_OP (const any&);

namespace {
   template <typename T>
   T &make();
}

struct returns_void_t {};
template <typename T> int operator,(T const &, returns_void_t);

template < typename RHS >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_returns_void) {
   static ::boost::type_traits::yes_type returns_void(returns_void_t);
   static ::boost::type_traits::no_type returns_void(int);
   BOOST_STATIC_CONSTANT(bool, value = (sizeof(::boost::type_traits::yes_type)==sizeof(returns_void((BOOST_TT_TRAIT_OP make<RHS>(),returns_void_t())))));
};

template < typename RHS, typename RET, typename RETURNS_VOID >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl2);

// BOOST_TT_TRAIT_OP RHS does not return void, checks if it is convertible to RET
template < typename RHS, typename RET >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl2)< RHS, RET, ::boost::false_type > {
   static ::boost::type_traits::yes_type is_convertible_to_RET(RET); // this version is preferred for types convertible to RET
   static ::boost::type_traits::no_type is_convertible_to_RET(...); // this version is used otherwise

   BOOST_STATIC_CONSTANT(bool, value = (sizeof(is_convertible_to_RET(BOOST_TT_TRAIT_OP make<RHS>()))==sizeof(::boost::type_traits::yes_type)));
};

// BOOST_TT_TRAIT_OP RHS returns void!=RET
template < typename RHS, typename RET >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl2)< RHS, RET, ::boost::true_type > {
   BOOST_STATIC_CONSTANT(bool, value = false);
};

template < typename RHS, typename RET, bool forbidden_if >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl1);

template < typename RHS, typename RET >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl1) < RHS, RET, true > {
   BOOST_STATIC_CONSTANT(bool, value = false);
};

// checks for return type if 2nd template parameter RET is non void
template < typename RHS, typename RET >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl1) < RHS, RET, false > {
   BOOST_STATIC_CONSTANT(bool, value = (BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl2)< RHS, RET, typename ::boost::integral_constant< bool, BOOST_JOIN(BOOST_TT_TRAIT_NAME,_returns_void)< RHS >::value > >::value));
};

// in case we do not want to check for return type
tag operator,(tag, int);

// do not check for return type if 2nd template parameter RET is void
template < typename RHS >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl1)< RHS, void, false > {
   static ::boost::type_traits::yes_type check(int); // this version is preferred when operator exists
   static ::boost::type_traits::no_type check(tag); // this version is used otherwise

   BOOST_STATIC_CONSTANT(bool, value = (sizeof(check(((BOOST_TT_TRAIT_OP make<RHS>()),0)))==sizeof(::boost::type_traits::yes_type)));
};

template < typename RET >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl1)< void, RET, false > {
   BOOST_STATIC_CONSTANT(bool, value = false);
};

template <>
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl1)< void, void, false > {
   BOOST_STATIC_CONSTANT(bool, value = false);
};

template < typename RHS, typename RET >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl) {
   typedef typename ::boost::remove_reference<RHS>::type rhs_noref;
   typedef typename ::boost::remove_cv<rhs_noref>::type rhs_nocv;
   BOOST_STATIC_CONSTANT(bool, value = (BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl1) < rhs_noref, RET, BOOST_TT_FORBIDDEN_IF >::value));
};

} // namespace impl
} // namespace detail

BOOST_TT_AUX_BOOL_TRAIT_DEF2(BOOST_TT_TRAIT_NAME,RHS,RET=BOOST_TT_DEFAULT_RET,(::boost::detail::BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl)::BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl)<RHS,RET>::value))

} // namespace boost

#ifdef BOOST_MSVC
#   pragma warning ( pop )
#endif

#include <boost/type_traits/detail/bool_trait_undef.hpp>
