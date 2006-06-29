//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_TYPE_TRAITS_ADD_ACTIVE_NO_DEFAULT_HPP
#define BOOST_ACT_TYPE_TRAITS_ADD_ACTIVE_NO_DEFAULT_HPP

#include "../detail/give_cv_to.hpp"
#include "../detail/active_type.hpp"

#include <boost/type_traits/remove_cv.hpp>

#include "../act_fwd.hpp"

namespace boost
{
namespace act
{

namespace detail
{

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
