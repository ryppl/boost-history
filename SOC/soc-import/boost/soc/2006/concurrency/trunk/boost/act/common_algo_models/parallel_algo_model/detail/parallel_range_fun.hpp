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

#ifndef BOOST_ACT_COMMON_ALGO_MODELS_PARALLEL_ALGO_MODEL_PARALLEL_RANGE_FUN_HPP
#define BOOST_ACT_COMMON_ALGO_MODELS_PARALLEL_ALGO_MODEL_PARALLEL_RANGE_FUN_HPP

#include <utility>

namespace boost
{
namespace act
{
namespace detail
{

template< typename DifferenceType, typename SizeType >
::std::pair< SizeType, SizeType >
parallel_subrange_info( DifferenceType range_length
                      , SizeType index, SizeType num_sections
                      )
{
  // Note: Current scheduling policy is even sections, heavy towards beginning

  if( index >= range_length )
    return ::std::pair< SizeType, SizeType >( 0, 0 );

  SizeType const num_used_sections = ::std::min( range_length, num_sections );

  DifferenceType const min_section_length = range_length / num_used_sections,
                       remaining_elements = range_length % num_used_sections;

  if( remaining_elements == 0 )
  {
    DifferenceType const section_length = min_section_length,
                         begin_index    = section_length * index;

    return ::std::make_pair( begin_index, section_length );
  }
  else
  {
    DifferenceType const max_section_length = min_section_length + 1;

    bool const is_large_section = index < remaining_elements;

    if( is_large_section )
    {
      DifferenceType const section_length = max_section_length,
                           begin_index    = max_section_length * index;

      return ::std::make_pair( begin_index, section_length );
    }
    else
    {
      DifferenceType const
        section_length = min_section_length,
        begin_index    =   max_section_length * remaining_elements
                         + min_section_length
                             * ( index - remaining_elements );

      return ::std::make_pair( begin_index, section_length );
    }
  }
}

// ToDo: Vary depending on the scheduler
template< typename LeftIteratorType, typename DifferenceType
        , typename FunctionType
        >
class parallel_range_fun
{
public:
  parallel_range_fun( LeftIteratorType const& left_it_init
                    , DifferenceType range_size_init
                    , FunctionType const& function_init
                    )
    : left_it_m( left_it_init )
    , range_size_m( range_size_init )
    , function_m( function_init )
  {
  }
public:
  template< typename SizeType >
  void operator ()( SizeType index, SizeType num_sections )
  {
    ::std::pair< SizeType, SizeType > const subrange_info
      = parallel_subrange_info( range_size_m, index, num_sections );

    SizeType const begin_index    = subrange_info.first,
                   section_length = subrange_info.second;

    if( section_length )
      perform_operation( begin_index, section_length );
  }
private:
  template< typename SizeType >
  void perform_operation( SizeType begin_index, SizeType section_length )
  {
    function_m( left_it_m + begin_index, section_length );
  }
private:
  LeftIteratorType left_it_m;
  DifferenceType range_size_m;
  FunctionType function_m;
};

template< typename LeftIteratorType, typename DifferenceType
        , typename FunctionType
        >
parallel_range_fun< LeftIteratorType, DifferenceType, FunctionType >
make_parallel_range_fun( LeftIteratorType const& left_it_init
                       , DifferenceType range_size_init
                       , FunctionType const& function_init
                       )
{
  return parallel_range_fun< LeftIteratorType, DifferenceType, FunctionType >
           ( left_it_init, range_size_init, function_init );
}

// ToDo: Vary depending on the scheduler
template< typename LeftIteratorType, typename DifferenceType
        , typename RightIteratorType, typename FunctionType
        >
class binary_parallel_range_fun
{
public:
  binary_parallel_range_fun( LeftIteratorType const& left_it_init
                           , DifferenceType range_size_init
                           , RightIteratorType const& right_it_init
                           , FunctionType const& function_init
                           )
    : left_it_m( left_it_init )
    , range_size_m( range_size_init )
    , right_it_m( right_it_init )
    , function_m( function_init )
  {
  }
public:
  template< typename SizeType >
  void operator ()( SizeType index, SizeType num_sections )
  {
    ::std::pair< SizeType, SizeType > const subrange_info
      = parallel_subrange_info( range_size_m, index, num_sections );

    SizeType const begin_index    = subrange_info.first,
                   section_length = subrange_info.second;

    if( section_length )
      perform_operation( begin_index, section_length );
  }
private:
  template< typename SizeType >
  void perform_operation( SizeType begin_index, SizeType section_length )
  {
    function_m( left_it_m + begin_index
              , section_length
              , right_it_m + begin_index
              );
  }
private:
  LeftIteratorType left_it_m;
  DifferenceType range_size_m;
  RightIteratorType right_it_m;
  FunctionType function_m;
};

template< typename LeftIteratorType, typename DifferenceType
        , typename RightIteratorType, typename FunctionType
        >
binary_parallel_range_fun< LeftIteratorType, DifferenceType
                         , RightIteratorType, FunctionType
                         >
make_binary_parallel_range_fun( LeftIteratorType const& left_it_init
                              , DifferenceType range_size_init
                              , RightIteratorType const& right_it_init
                              , FunctionType const& function_init
                              )
{
  return binary_parallel_range_fun< LeftIteratorType, DifferenceType
                                  , RightIteratorType, FunctionType
                                  >
           ( left_it_init, range_size_init, right_it_init, function_init );
}

}
}
}

#endif
