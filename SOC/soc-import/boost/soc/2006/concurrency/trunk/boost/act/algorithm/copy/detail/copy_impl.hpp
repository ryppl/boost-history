//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ALGORITHM_DETAIL_COPY_IMPL_HPP
#define BOOST_ACT_ALGORITHM_DETAIL_COPY_IMPL_HPP

#include <algorithm>

#include "../../../act_fwd.hpp"

#include "../../basic_for.hpp"

#include "../../../parallel_safe.hpp"

namespace boost
{
namespace act
{
namespace detail
{

template< typename SourceIteratorType, typename TargetIteratorType >
class safe_assign
  : public parallel_safe
{
public:
  safe_assign( SourceIteratorType source_begin_init
             , TargetIteratorType target_begin_init
             )
    : source_begin_m( source_begin_init )
    , target_begin_m( target_begin_init )
  {
  }
public:
  template< typename DifferenceType >
  void operator ()( DifferenceType curr_offset ) const
  {
    *( target_begin_m + curr_offset ) = *( source_begin_m + curr_offset );
  }
private:
  SourceIteratorType const source_begin_m;
  TargetIteratorType const target_begin_m;
};


template< typename SourceIteratorType, typename TargetIteratorType >
safe_assign< SourceIteratorType, TargetIteratorType >
make_safe_assign( SourceIteratorType source_begin
                , TargetIteratorType target_begin
                )
{
  return safe_assign< SourceIteratorType
                    , TargetIteratorType
                    >( source_begin, target_begin );
}

}

template< typename AlgoModel >
struct copy_safe_random_impl
{
  template< typename SourceIteratorType
          , typename TargetIteratorType
          >
  static TargetIteratorType execute( SourceIteratorType source_begin
                                   , SourceIteratorType source_end
                                   , TargetIteratorType target_begin
                                   )
  {
    typedef typename iterator_difference< SourceIteratorType >::type
              difference_type;

    difference_type const range_size = source_end - source_begin;

    basic_for[ AlgoModel() ]( for_var
                                = static_cast< difference_type >( 0 )
                            , for_var < range_size
                            , ++for_var
                            )
    [
      detail::make_safe_assign( source_begin, target_begin )
    ];

    return target_begin + range_size;
  }
};

template< typename AlgoModel >
struct copy_safe_forward_impl
{
  template< typename SourceIteratorType
          , typename TargetIteratorType
          >
  static TargetIteratorType execute( SourceIteratorType source_begin
                                   , SourceIteratorType source_end
                                   , TargetIteratorType target_begin
                                   )
  {
    // ToDo: Change to spawn active objects which queue functions
    return ::std::copy( source_begin, source_end, target_begin );
  }
};

template< typename AlgoModel >
struct copy_unsafe_impl
{
  template< typename SourceIteratorType
          , typename TargetIteratorType
          >
  static TargetIteratorType execute( SourceIteratorType source_begin
                                   , SourceIteratorType source_end
                                   , TargetIteratorType target_begin
                                   )
  {
    return ::std::copy( source_begin, source_end, target_begin );
  }
};

}
}


#endif
