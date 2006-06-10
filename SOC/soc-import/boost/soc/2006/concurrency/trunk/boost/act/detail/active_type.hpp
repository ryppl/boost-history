//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_ACTIVE_TYPE_HPP
#define BOOST_ACT_DETAIL_ACTIVE_TYPE_HPP

#include <boost/tuple/tuple.hpp>

#include "../active/active_interface.hpp"

#include "active_helper.hpp"

namespace boost
{
namespace act
{
namespace detail
{

// ToDo: Error if Type is qualified
template< typename UnqualifiedType
        , typename ActModel
        >
class active_type
  : ActModel::template active_impl< UnqualifiedType >
  , public active_interface< UnqualifiedType
                           , active_details
                             <
                               active_type< UnqualifiedType, ActModel >
                             , ActModel
                             , action_return_meta
                             >
                           >
  , public active_helper< active_type< UnqualifiedType, ActModel >
                 , UnqualifiedType
                 , ActModel
                 > // ToDo: Change to private
{
public:
  typedef UnqualifiedType represented_type;
  typedef ActModel act_model;
private:
  typedef typename ActModel::template active_impl< UnqualifiedType >
            active_impl_base_type;

  typedef active_helper< active_type< UnqualifiedType, ActModel >
                       , UnqualifiedType
                       , ActModel
                       >
                       active_helper_base;
public: // ToDo: Make private
  active_impl_base_type& impl()
  {
    return static_cast< active_impl_base_type& >( *this );
  }

  active_impl_base_type const& impl() const
  {
    return static_cast< active_impl_base_type const& >( *this );
  }
public:
  // ToDo: Make constructors
public:
  UnqualifiedType inactive_value() const
  {
    return active_impl_base_type::inactive_value();
  }
public:
  BOOST_ACT_DETAIL_PROMOTE_ACTIVE_MEMBERS( active_helper_base )
};

}
}
}

#endif
