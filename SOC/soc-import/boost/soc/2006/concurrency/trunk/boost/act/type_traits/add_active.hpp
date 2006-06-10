//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_TYPE_TRAITS_ADD_ACTIVE_HPP
#define BOOST_ACT_TYPE_TRAITS_ADD_ACTIVE_HPP

#include "../detail/give_cv_to.hpp"
#include "../detail/active_type.hpp"
#include <boost/type_traits/remove_cv.hpp>

#include "../config/default_act_model.hpp"

namespace boost
{
namespace act
{

namespace detail
{

/*
template< typename ChildType, typename Type = void >
class abstract_active
{
  // ToDo: Put call functions here
};

template< typename ChildType
        , typename UnqualifiedType
        , typename ActiveImpl
        , typename Enabler = void
        >
struct active_interface
{
};

namespace detail
{
  template< typename ActiveImpl >
  class active_interface_arithmetic_base;

  template< typename ActiveImpl >
  class active_interface_pointer_base;

  template< typename ArithmeticType
          , typename ActiveImpl
          >
  class active_interface_arithmetic_operations;
}

// ToDo: Use usual arithmetic conversions to determine return values
// ToDo: Make different base for comparison operators
template< typename ChildType
        , typename UnqualifiedType
        , typename ActiveImpl
        >
struct active_interface< ChildType
                       , UnqualifiedType
                       , ActiveImpl
                       , typename ::boost::enable_if
                         <
                           ::boost::mpl::or_
                           <
                             ::boost::is_arithmetic< UnqualifiedType >
                           , ::boost::is_pointer< UnqualifiedType >
                           >
                         >
                         ::type
                       >
  : ::boost::mpl::if_< ::boost::is_arithmetic< UnqualifiedType >
                     , detail::active_interface_arithmetic_operations
                       < UnqualifiedType, ActiveImpl >
                     , ::boost::mpl::empty_base
                     >
                     ::type
//      , active_base_helper< ChildType >
{
};
*/


// ToDo: Make sure to handle references, arrays, and qualified types
//       Remove action before adding active
template< typename Type
        , typename ActModel
        >
struct qualified_active_type
  : give_cv_to
    <
      Type
    , active_type< typename ::boost::remove_cv< Type >::type, ActModel >
    >
{ 
};

}

template< typename Type, typename ActModel = default_act_model >
struct add_active
  : detail::qualified_active_type< Type, ActModel >
{
};

}
}
#endif
