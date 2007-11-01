/*=============================================================================
    Copyright (c) 2007 Marco Cecchetti

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef _BOOST_OVERLOAD_DETAIL_MEMBER_FUNCTION_FORM_HPP_
#define _BOOST_OVERLOAD_DETAIL_MEMBER_FUNCTION_FORM_HPP_

#include <boost/type_traits/function_traits.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_class.hpp>

#include "member_function_trait.hpp"

#define OVL_MEMB_FUNC_FORM(z, n, unused) \
template< typename ClassT,                                                     \
          typename R BOOST_PP_ENUM_TRAILING_PARAMS(n, typename A) >            \
struct memb_func_form<                                                         \
    R ( ClassT* BOOST_PP_ENUM_TRAILING_PARAMS(n, A) ), non_const, true>        \
{                                                                              \
  private:                                                                     \
    typedef memb_func_trait< R (ClassT::*) ( BOOST_PP_ENUM_PARAMS(n, A) ) >    \
            traits;                                                            \
  public:                                                                      \
    typedef typename traits::type          type;                               \
};                                                                             \
                                                                               \
template< typename ClassT,                                                     \
          typename R BOOST_PP_ENUM_TRAILING_PARAMS(n, typename A) >            \
struct memb_func_form<                                                         \
    R ( ClassT* BOOST_PP_ENUM_TRAILING_PARAMS(n, A) ), const_, true>           \
{                                                                              \
  private:                                                                     \
    typedef memb_func_trait< R (ClassT::*) ( BOOST_PP_ENUM_PARAMS(n, A) ) >    \
            traits;                                                            \
  public:                                                                      \
    typedef typename traits::const_type          type;                         \
};                                                                             \
                                                                               \
template< typename ClassT,                                                     \
          typename R BOOST_PP_ENUM_TRAILING_PARAMS(n, typename A) >            \
struct memb_func_form<                                                         \
    R ( const ClassT* BOOST_PP_ENUM_TRAILING_PARAMS(n, A) ), const_, true >    \
{                                                                              \
  private:                                                                     \
    typedef memb_func_trait< R (ClassT::*) ( BOOST_PP_ENUM_PARAMS(n, A) ) >    \
            traits;                                                            \
  public:                                                                      \
    typedef typename traits::const_type    type;                               \
};                                                                             \
                                                                               \
template< typename ClassT,                                                     \
          typename R BOOST_PP_ENUM_TRAILING_PARAMS(n, typename A) >            \
struct memb_func_form<                                                         \
    R ( const ClassT* BOOST_PP_ENUM_TRAILING_PARAMS(n, A) ), non_const, true > \
{                                                                              \
};                                                                             \

// end macro OVL_MEMB_FUNC_FORM

namespace boost{ namespace overloads{ namespace detail{

///////////////////////////////////////////////////////////////////////////////
// return true if the first parameter type of the function is a pointer or 
// a const pointer to a class

template< typename Signature, 
          unsigned int arity = boost::function_traits<Signature>::arity >
struct is_interface_func
{
  private:
    typedef typename boost::function_traits<Signature>::arg1_type arg_type;
    typedef typename boost::remove_pointer<arg_type>::type candidate_type;
  public:
      static const bool value 
          = boost::is_class<candidate_type>::value
          && boost::is_pointer<arg_type>::value;
};

template< typename Signature >
struct is_interface_func<Signature, 0>
{
    static const bool value = false;
};

///////////////////////////////////////////////////////////////////////////////
// provides the type pointer to function member whose signature and class 
// type owner are deduced from the signature of the equivalent free function

struct const_;
struct non_const;

template< typename Signature,
          typename const_qualifier = non_const,
          bool enable = is_interface_func<Signature>::value >
struct memb_func_form
{};

BOOST_PP_REPEAT(BOOST_OVERLOAD_LIMIT, OVL_MEMB_FUNC_FORM, unused)

} } } // end namespaces

#undef OVL_MEMB_FUNC_FORM

#endif // _BOOST_OVERLOAD_DETAIL_MEMBER_FUNCTION_FORM_HPP_

