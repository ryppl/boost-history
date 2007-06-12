/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_DETAIL_RESULT_TYPE_HPP
#define BOOST_ACT_DETAIL_RESULT_TYPE_HPP

#include <boost/type_traits/function_traits.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_function.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/empty_base.hpp>

#include <boost/act/detail/has_single_result_type.hpp>

namespace boost
{
namespace act
{
namespace detail
{

struct no_default_type;

template< typename Type
        , typename DefaultType
        , typename Enabler = void
        >
struct result_type_impl
{
  typedef DefaultType type;
};

template< typename Type
        , typename DefaultType
        >
struct result_type_impl
       <
         Type
       , DefaultType
       , typename enable_if< is_function< Type > >::type
       >
{
  typedef typename function_traits< Type >::result_type type;
};

template< typename Type
        , typename DefaultType
        >
struct result_type_impl
       <
         Type
       , DefaultType
       , typename mpl::apply
         <
           mpl::always< void >
         , typename Type::result_type
         >
         ::type
       >
{
  typedef typename Type::result_type type;
};

// ToDo: Add support for polymorphic function object returns

template< typename Type
        , typename DefaultType = no_default_type
        >
struct result_type
  : mpl::if_
    <
      mpl::or_
      <
        has_single_result_type< Type >
      , mpl::not_< is_same< DefaultType, no_default_type > >
      >
    , result_type_impl< Type, DefaultType >
    , mpl::empty_base
    >
    ::type
{
};

}

}
}

#endif
