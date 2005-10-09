// Copyright David Abrahams 2004. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef IS_INCREMENTABLE_DWA200415_HPP
# define IS_INCREMENTABLE_DWA200415_HPP

# include <boost/type_traits/detail/bool_trait_def.hpp>
# include <boost/type_traits/detail/template_arity_spec.hpp>
# include <boost/type_traits/remove_cv.hpp>
# include <boost/mpl/aux_/lambda_support.hpp>
# include <boost/mpl/bool.hpp>
# include <boost/config.hpp>  // BOOST_INTEL
# include <boost/detail/workaround.hpp>

// is_incrementable<T> metafunction
//
// Requires: Given x of type T&, if the expression ++x is well-formed
// it must have complete type; otherwise, it must neither be ambiguous
// nor violate access.

// This namespace ensures that ADL doesn't mess things up.
namespace is_incrementable_
{
  // a type returned from operator++ when no increment is found in the
  // type's own namespace
  struct tag {};
  
  // any soaks up implicit conversions and makes the following
  // operator++ less-preferred than any other such operator that
  // might be found via ADL.
  struct any { template <class T> any(T const&); };

# if defined(__GNUC_PATCHLEVEL__) && !defined(BOOST_INTEL)
#  define BOOST_IS_INCREMENTABLE_GCC \
     ((__GNUC__ * 100UL + __GNUC_MINOR__) * 100UL + __GNUC_PATCHLEVEL__) \
     /**/
# else
#  define BOOST_IS_INCREMENTABLE_GCC 0
# endif

# if BOOST_WORKAROUND(BOOST_IS_INCREMENTABLE_GCC, >= 40002)

  } // namespace is_incrementable_

  // This is a last-resort operator++ for when none other is found
  is_incrementable_::tag operator++(is_incrementable_::any const&);
  is_incrementable_::tag operator++(is_incrementable_::any const&,int);

  namespace is_incrementable_ {

# else 

  // This is a last-resort operator++ for when none other is found
  tag operator++(any const&);
  tag operator++(any const&,int);

# endif

# if BOOST_WORKAROUND(__MWERKS__, BOOST_TESTED_AT(0x3202)) \
    || BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
#  define BOOST_comma(a,b) (a)
# else 
  // In case an operator++ is found that returns void, we'll use ++x,0
  tag operator,(tag,int);  
#  define BOOST_comma(a,b) (a,b)
# endif 
  
  // two check overloads help us identify which operator++ was picked
  char (& check(tag) )[2];
  
  template <class T>
  char check(T const&);
  

  template <class T>
  struct impl
  {
      static typename boost::remove_cv<T>::type& x;

      BOOST_STATIC_CONSTANT(
          bool
        , value = sizeof(is_incrementable_::check(BOOST_comma(++x,0))) == 1
      );
  };

  template <class T>
  struct postfix_impl
  {
      static typename boost::remove_cv<T>::type& x;

      BOOST_STATIC_CONSTANT(
          bool
        , value = sizeof(is_incrementable_::check(BOOST_comma(x++,0))) == 1
      );
  };
} // namespace is_incrementable_

namespace boost { namespace detail {

# undef BOOST_comma

template<typename T> 
struct is_incrementable 
   BOOST_TT_AUX_BOOL_C_BASE(::is_incrementable_::impl<T>::value)
{ 
    BOOST_TT_AUX_BOOL_TRAIT_VALUE_DECL(::is_incrementable_::impl<T>::value)
    BOOST_MPL_AUX_LAMBDA_SUPPORT(1,is_incrementable,(T))
};

template<typename T> 
struct is_postfix_incrementable 
    BOOST_TT_AUX_BOOL_C_BASE(::is_incrementable_::impl<T>::value)
{ 
    BOOST_TT_AUX_BOOL_TRAIT_VALUE_DECL(::is_incrementable_::postfix_impl<T>::value)
    BOOST_MPL_AUX_LAMBDA_SUPPORT(1,is_postfix_incrementable,(T))
};

} // namespace detail

BOOST_TT_AUX_TEMPLATE_ARITY_SPEC(1, ::boost::detail::is_incrementable)
BOOST_TT_AUX_TEMPLATE_ARITY_SPEC(1, ::boost::detail::is_postfix_incrementable)

} // namespace boost


#endif // IS_INCREMENTABLE_DWA200415_HPP
