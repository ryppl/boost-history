//  Copyright (c) 2006, Matthew Calabrese
//
//  This code may be used under either of the following two licences:
//
//    Permission is hereby granted, free of charge, to any person
//    obtaining a copy of this software and associated documentation
//    files (the "Software"), to deal in the Software without
//    restriction, including without limitation the rights to use,
//    copy, modify, merge, publish, distribute, sublicense, and/or
//    sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following
//    conditions:
//
//    The above copyright notice and this permission notice shall be
//    included in all copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//    OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)

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
