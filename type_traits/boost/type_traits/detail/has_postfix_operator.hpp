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
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_reference.hpp>

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
tag operator BOOST_TT_TRAIT_OP (const any&, int);

namespace {
   template <typename T>
   typename ::boost::remove_cv<T>::type &make();
}

template < typename LHS >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_returns_void) {
   struct returns_void_t {
      template <typename T> friend int operator,(T const &, returns_void_t);
   };
   static ::boost::type_traits::yes_type returns_void(returns_void_t);
   static ::boost::type_traits::no_type returns_void(int);
   BOOST_STATIC_CONSTANT(bool, value = (sizeof(::boost::type_traits::yes_type)==sizeof(returns_void((make<LHS>() BOOST_TT_TRAIT_OP,returns_void_t())))));
};

template < typename LHS, typename RET, typename RETURNS_VOID >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl2);

// BOOST_TT_TRAIT_OP LHS does not return void, checks if it is convertible to RET
template < typename LHS, typename RET >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl2)< LHS, RET, ::boost::false_type > {
   static ::boost::type_traits::yes_type is_convertible_to_RET(RET); // this version is preferred for types convertible to RET
   static ::boost::type_traits::no_type is_convertible_to_RET(...); // this version is used otherwise

   BOOST_STATIC_CONSTANT(bool, value = (sizeof(is_convertible_to_RET(make<LHS>() BOOST_TT_TRAIT_OP))==sizeof(::boost::type_traits::yes_type)));
};

// BOOST_TT_TRAIT_OP LHS returns void!=RET
template < typename LHS, typename RET >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl2)< LHS, RET, ::boost::true_type > {
   BOOST_STATIC_CONSTANT(bool, value = false);
};

template < typename LHS, typename RET, bool forbidden_if >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl1);

template < typename LHS, typename RET >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl1) < LHS, RET, true > {
   BOOST_STATIC_CONSTANT(bool, value = false);
};

// checks for return type if 2nd template parameter RET is non void
template < typename LHS, typename RET >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl1) < LHS, RET, false > {
   BOOST_STATIC_CONSTANT(bool, value = (BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl2)< LHS, RET, typename ::boost::integral_constant< bool, BOOST_JOIN(BOOST_TT_TRAIT_NAME,_returns_void)< LHS >::value > >::value));
};

// in case we do not want to check for return type
tag operator,(tag, int);

// do not check for return type if 2nd template parameter RET is void
template < typename LHS >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl1)< LHS, void, false > {
   static ::boost::type_traits::yes_type check(int); // this version is preferred for types convertible to RET
   static ::boost::type_traits::no_type check(tag); // this version is used otherwise

   BOOST_STATIC_CONSTANT(bool, value = (sizeof(check(((make<LHS>() BOOST_TT_TRAIT_OP),0)))==sizeof(::boost::type_traits::yes_type)));
};

template < typename RET >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl1)< void, RET, false > {
   BOOST_STATIC_CONSTANT(bool, value = false);
};

template <>
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl1)< void, void, false > {
   BOOST_STATIC_CONSTANT(bool, value = false);
};

template < typename LHS, typename RET >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl) : public BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl1) < LHS, RET, BOOST_TT_FORBIDDEN_IF > { };

} // namespace impl
} // namespace detail

BOOST_TT_AUX_BOOL_TRAIT_DEF2(BOOST_TT_TRAIT_NAME,LHS,RET=BOOST_TT_DEFAULT_RET,(::boost::detail::BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl)::BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl)<LHS,RET>::value))

} // namespace boost

#include <boost/type_traits/detail/bool_trait_undef.hpp>
