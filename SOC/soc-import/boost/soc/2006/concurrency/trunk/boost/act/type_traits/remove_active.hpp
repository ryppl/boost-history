//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_TYPE_TRAITS_REMOVE_ACTIVE_HPP
#define BOOST_ACT_TYPE_TRAITS_REMOVE_ACTIVE_HPP

#include "../detail/give_cv_to.hpp"
#include <boost/type_traits/remove_cv.hpp>

namespace boost
{
namespace act
{
namespace detail
{

template< typename OriginalType
        , typename UnqualifiedType
        >
struct remove_active_impl
{
  typedef OriginalType type;
};

template< typename UnqualifiedType, typename ActModel >
class active_type;

// ToDo: Possibly change "RepresentedType" parameter name
template< typename OriginalType
        , typename UnqualifiedType
        , typename ActModel
        >
struct remove_active_impl< OriginalType
                         , active_type< UnqualifiedType, ActModel >
                         >
  : give_cv_to
    <
      OriginalType
    , UnqualifiedType
    >
{
};

}

template< typename Type >
struct remove_active
  : detail::remove_active_impl< Type
                              , typename ::boost::remove_cv< Type >::type
                              >
{
};

}
}

#endif
