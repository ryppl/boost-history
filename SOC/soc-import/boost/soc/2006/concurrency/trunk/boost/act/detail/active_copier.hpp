//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_ACTIVE_COPIER_HPP
#define BOOST_ACT_DETAIL_ACTIVE_COPIER_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_void.hpp>

namespace boost
{
namespace act
{
namespace detail
{

template< typename ResultType, typename Enabler = void >
struct active_copier
{
  template< typename SourceType >
  ResultType operator ()( SourceType const& source ) const
  {
    return source;
  }

  template< typename SourceType >
  ResultType operator ()( SourceType& source ) const
  {
    return source;
  }
};

template< typename ResultType >
struct active_copier< ResultType
                    , typename ::boost::enable_if
                      <
                        ::boost::is_void< ResultType >
                      >
                      ::type
                    >
{
  template< typename SourceType >
  ResultType operator ()( SourceType const& source ) const
  {
  }

  template< typename SourceType >
  ResultType operator ()( SourceType& source ) const
  {
  }

  ResultType operator ()() const
  {
  }
};

}
}
}

#endif
