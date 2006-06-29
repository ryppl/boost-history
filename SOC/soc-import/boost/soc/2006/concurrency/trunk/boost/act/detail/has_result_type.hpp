//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_HAS_RESULT_TYPE_HPP
#define BOOST_ACT_DETAIL_HAS_RESULT_TYPE_HPP

#include <boost/mpl/bool.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_function.hpp>
#include <boost/mpl/apply.hpp>

#include <boost/mpl/always.hpp>

namespace boost
{
namespace act
{
namespace detail
{

template< typename Type
        , typename Enabler = void
        >
struct has_result_type_impl
  : ::boost::mpl::false_
{
};

template< typename Type >
struct has_result_type_impl
       <
         Type
       , typename ::boost::enable_if
         <
           ::boost::is_function< Type >
         >
         ::type
       >
  : ::boost::mpl::true_
{
};

template< typename Type >
struct has_result_type_impl
       <
         Type
       , typename ::boost::mpl::apply
         <
           ::boost::mpl::always< void >
         , typename Type::result_type
         >
         ::type
       >
  : ::boost::mpl::true_
{
};

template< typename Type >
struct has_result_type
  : has_result_type_impl< Type >
{
};

}

}
}

#endif
