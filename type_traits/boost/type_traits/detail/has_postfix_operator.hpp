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

// 1. a function to have an instance of type T without requiring T to be default
// constructible
namespace {
   template <typename T> T &make();
}

// 2. we provide our operator definition for types that do not have one already

// a type returned from operator BOOST_TT_TRAIT_OP when no such operator is
// found in the type's own namespace so that we have a means to know that our
// operator was used
struct tag { };

// this class allows implicit conversions and makes the following operator
// definition less-preferred than any other such operators that might be found
// via argument-dependent name lookup
struct any { template <class T> any(T const&); };

// when operator BOOST_TT_TRAIT_OP is not available, this one is used
tag operator BOOST_TT_TRAIT_OP (const any&, int);

// 3. if the return type has to be checked, we need to determine if
// operator BOOST_TT_TRAIT_OP returns void or not

// we first redefine "operator," so that we have no compilation error if
// operator BOOST_TT_TRAIT_OP returns void and we can use the return type of
// (lhs BOOST_TT_TRAIT_OP, returns_void_t()) to deduce if
// operator BOOST_TT_TRAIT_OP returns void or not:
// - operator BOOST_TT_TRAIT_OP returns void   -> (lhs BOOST_TT_TRAIT_OP, returns_void_t()) returns returns_void_t
// - operator BOOST_TT_TRAIT_OP returns !=void -> (lhs BOOST_TT_TRAIT_OP, returns_void_t()) returns int
struct returns_void_t {};
template <typename T> int operator,(T const &, returns_void_t);

// this intermediate trait has member value of type bool:
// - value==true -> operator BOOST_TT_TRAIT_OP returns void
// - value==false -> operator BOOST_TT_TRAIT_OP does not return void
template < typename LHS >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_returns_void) {
   // overloads of function returns_void make the difference
   // yes_type and no_type have different size by construction
   static ::boost::type_traits::yes_type returns_void(returns_void_t);
   static ::boost::type_traits::no_type returns_void(int);
   BOOST_STATIC_CONSTANT(bool, value = (sizeof(::boost::type_traits::yes_type)==sizeof(returns_void((make<LHS>() BOOST_TT_TRAIT_OP,returns_void_t())))));
};

// 4. check for return type when RET is non void

// this class is used only when RET is non void, therefore it is specialized to
// have its value==false when the operator returns void
template < typename LHS, typename RET, typename RETURNS_VOID >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl2);

// operator BOOST_TT_TRAIT_OP does not return void, checks if it is convertible to
// RET using the sizeof trick based on overload resolution
template < typename LHS, typename RET >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl2)< LHS, RET, ::boost::false_type > {
   static ::boost::type_traits::yes_type is_convertible_to_RET(RET); // this version is preferred for types convertible to RET
   static ::boost::type_traits::no_type is_convertible_to_RET(...); // this version is used otherwise

   BOOST_STATIC_CONSTANT(bool, value = (sizeof(is_convertible_to_RET(make<LHS>() BOOST_TT_TRAIT_OP))==sizeof(::boost::type_traits::yes_type)));
};

// operator BOOST_TT_TRAIT_OP returns void!=RET -> specialization to return
// value==false
template < typename LHS, typename RET >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl2)< LHS, RET, ::boost::true_type > {
   BOOST_STATIC_CONSTANT(bool, value = false);
};

// 5. main struct: to avoid any compilation error, this struct behaves
// differently when:
// - RET is void or non void
// - operator BOOST_TT_TRAIT_OP(LHS) is forbidden by the standard
// forbidden_if is a bool that is:
// - true when the operator BOOST_TT_TRAIT_OP(LHS) is forbidden by the standard
//   (would yield compilation error if used)
// - false otherwise
template < typename LHS, typename RET, bool forbidden_if >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl1);

template < typename LHS, typename RET >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl1) < LHS, RET, true > {
   BOOST_STATIC_CONSTANT(bool, value = false);
};

// checks for return type if template parameter RET is non void: use previously
// defined struct
template < typename LHS, typename RET >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl1) < LHS, RET, false > {
   BOOST_STATIC_CONSTANT(bool, value = (BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl2)< LHS, RET, typename ::boost::integral_constant< bool, BOOST_JOIN(BOOST_TT_TRAIT_NAME,_returns_void)< LHS >::value > >::value));
};

// in case we do not want to check for return type, we still need to know if our
// definition of operator BOOST_TT_TRAIT_OP is used or an other existing one;
// this is done with redefinition of "operator," that returns tag or int
tag operator,(tag, int);

// do not check for return type if template parameter RET is void
template < typename LHS >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl1)< LHS, void, false > {
   static ::boost::type_traits::yes_type check(int); // this version is preferred when operator exists
   static ::boost::type_traits::no_type check(tag); // this version is used otherwise

   BOOST_STATIC_CONSTANT(bool, value = (sizeof(check(((make<LHS>() BOOST_TT_TRAIT_OP),0)))==sizeof(::boost::type_traits::yes_type)));
};

// some specializations needs to be declared for the special void case
template < typename RET >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl1)< void, RET, false > {
   BOOST_STATIC_CONSTANT(bool, value = false);
};

template <>
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl1)< void, void, false > {
   BOOST_STATIC_CONSTANT(bool, value = false);
};

// defines some typedef for convenience
template < typename LHS, typename RET >
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl) {
   typedef typename ::boost::remove_reference<LHS>::type lhs_noref;
   typedef typename ::boost::remove_cv<lhs_noref>::type lhs_nocv;
   BOOST_STATIC_CONSTANT(bool, value = (BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl1) < lhs_noref, RET, BOOST_TT_FORBIDDEN_IF >::value));
};

} // namespace impl
} // namespace detail

// this is the accessible definition of the trait to end user
BOOST_TT_AUX_BOOL_TRAIT_DEF2(BOOST_TT_TRAIT_NAME, LHS, RET=BOOST_TT_DEFAULT_RET, (::boost::detail::BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl)::BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl)< LHS, RET >::value))

} // namespace boost

#include <boost/type_traits/detail/bool_trait_undef.hpp>
