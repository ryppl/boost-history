/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONSUMER_POLICY_IS_HPP
#define BOOST_ACT_CONSUMER_POLICY_IS_HPP

#include <boost/act/consumer_policy/is_fwd.hpp>
#include <boost/act/consumer_policy/default_normalization_info_fwd.hpp>
#include <boost/act/consumer_policy/consumer_arguments_fwd.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/eval_if.hpp>

#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/add_reference.hpp>

#include <boost/act/consumer_policy/consumer_arguments_fwd.hpp>

namespace boost { namespace act {

// ToDo: Declare this template somewhere
template< typename Type >
struct default_normalization_info< is< Type > >
{
  struct eludible;

  struct normalize
  {
    typedef Type result_type;

    template< typename ParamType >
    result_type operator ()( ParamType& argument ) const
    {
      return argument;
    }
  };

  struct denormalize
  {
    typedef void result_type;

    template< typename VisitorType, typename ParamType >
    void operator ()( VisitorType& visitor, ParamType& argument ) const
    {
      visitor( argument );
    }
  };
};

namespace detail {

template< typename Type, typename Arg >
struct is_impl_basic
  : mpl::and_< is_convertible< typename remove_cv< Type >::type*
                             , typename remove_cv< Arg >::type*
                             >
             , is_convertible< Arg, Type >
             > {};

// ToDo: Possibly rewrite to use unwrap_reference
template< typename Type, typename Arg >
struct is_impl_ref
  : is_convertible< typename add_pointer< Arg >::type
                  , typename add_pointer
                    <
                      typename remove_reference< Type >::type
                    >
                    ::type
                  > {};

}

// ToDo: Make variable argument list size toggleable
template< typename Type >
struct is
{
  typedef constraint< is< Type > > revised_type;

  template< typename Arg >
  struct apply
    : mpl::eval_if< is_reference< Type >
                  , detail::is_impl_ref< Type, Arg >
                  , detail::is_impl_basic< Type, Arg >
                  > {};
};

} }

#endif
