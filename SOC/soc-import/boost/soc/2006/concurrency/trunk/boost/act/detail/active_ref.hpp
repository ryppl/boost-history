//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_ACTIVE_REF_HPP
#define BOOST_ACT_DETAIL_ACTIVE_REF_HPP

#include <boost/noncopyable.hpp>

#include "active_helper.hpp"

#include "../active/active_interface.hpp"

#include "action_return_meta.hpp"

namespace boost
{
namespace act
{
namespace detail
{

template< typename ChildType, typename RepresentedType, typename ActModel >
class active_ref
  : public active_interface< RepresentedType
                           , active_details< ChildType
                                           , ActModel
                                           , action_return_meta
                                           >
                           >
  , public active_helper< active_ref< ChildType, RepresentedType, ActModel >
                 , RepresentedType
                 , ActModel
                 > // ToDo: Change to private
  , ::boost::noncopyable
{
private:
  typedef active_helper< active_ref< ChildType, RepresentedType, ActModel >
                       , RepresentedType
                       , ActModel
                       > 
                       active_helper_base;

  typedef typename ActModel::template action_impl< RepresentedType > impl_type;
public:
  impl_type impl()
  {
    return static_cast< ChildType& >( *this ).impl();
  }
public:
  BOOST_ACT_DETAIL_PROMOTE_ACTIVE_MEMBERS( active_helper_base )
};

}
}
}

#endif
