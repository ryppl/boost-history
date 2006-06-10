//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

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

#include "has_result_type.hpp"

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
       , typename ::boost::enable_if
         <
           ::boost::is_function< Type >
         >
         ::type
       >
{
  typedef typename ::boost::function_traits< Type >::result_type type;
};

template< typename Type
        , typename DefaultType
        >
struct result_type_impl
       <
         Type
       , DefaultType
       , typename::boost::mpl::apply
         <
           ::boost::mpl::always< void >
         , typename Type::result_type
         >
         ::type
       >
{
  typedef typename Type::result_type type;
};

template< typename Type
        , typename DefaultType = no_default_type
        >
struct result_type
  : ::boost::mpl::if_
    <
      ::boost::mpl::or_
      <
        has_result_type< Type >
      , ::boost::mpl::not_
        <
          ::boost::is_same< DefaultType, no_default_type >
        >
      >
    , result_type_impl< Type, DefaultType >
    , ::boost::mpl::empty_base
    >
    ::type
{
};

}

}
}

#endif
