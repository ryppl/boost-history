/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_DETAIL_CONSUMER_ALLOCATOR_DETAIL_CONSUMER_FUN_FROM_TYPE_HPP
#define BOOST_ACT_DETAIL_CONSUMER_ALLOCATOR_DETAIL_CONSUMER_FUN_FROM_TYPE_HPP

#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/is_void.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>

#include <boost/function.hpp>

// ToDo: Rewrite to have fewer template instantiations

namespace boost
{
namespace act
{
namespace detail
{
/*
template< typename Type >
struct raw_make_consumer_fun_type
{
  typedef void type( typename add_reference< Type >::type );
};

template< typename Type >
struct raw_make_consumer_const_fun_type
{
  typedef void type( typename add_reference
                     <
                       typename add_const< Type >::type
                     >
                     ::type
                   );
};

template< typename ConsumerData >
struct consumer_fun_from_data
{
private:
  typedef typename mpl::eval_if
          <
            is_void< ConsumerData >
          , mpl::identity< void () >
          , detail::raw_make_consumer_fun_type< ConsumerData >
          >
          ::type raw_nonconst_fun;

  typedef typename mpl::eval_if
          <
            is_void< ConsumerData >
          , mpl::identity< void () >
          , detail::raw_make_consumer_const_fun_type< ConsumerData >
          >
          ::type raw_const_fun;
public:
  typedef function< raw_nonconst_fun > function_type;
  typedef function< raw_const_fun > const_function_type;
};
*/
}
}
}

#endif
