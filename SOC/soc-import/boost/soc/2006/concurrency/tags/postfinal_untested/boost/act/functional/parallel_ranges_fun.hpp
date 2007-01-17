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

#ifndef BOOST_ACT_FUNCTIONAL_PARALLEL_RANGES_FUN_HPP
#define BOOST_ACT_FUNCTIONAL_PARALLEL_RANGES_FUN_HPP

#include <utility>
#include <algorithm>

#include <boost/mpl/size_t.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/fusion/sequence/container/vector.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/ref.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>

namespace boost
{
namespace act
{
namespace detail
{

#define BOOST_ACT_MAX_MAX_RANGES 10

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

template< typename NaryRangeFunType, typename SizeType >
void perform_nary_range_operation( NaryRangeFunType& target
                                 , SizeType index, SizeType num_sections
                                 )
{
  ::std::pair< SizeType, SizeType > const subrange_info
    = parallel_subrange_info( target.range_size_m, index, num_sections );

  SizeType const begin_index    = subrange_info.first,
                 section_length = subrange_info.second;

  if( section_length )
    perform_nary_range_fun_operation( target, begin_index, section_length );
}

#define BOOST_ACT_DETAIL_OFFSET_ITERATOR( z, curr_index, dummy )               \
at_c< curr_index >( target.begin_iterators_m ) + begin_index

#define BOOST_ACT_DETAIL_PERFORM_NARY_RANGE_FUN_OP(z,curr_num_iterators,dummy) \
template< typename NaryRangeFunType                                            \
        , typename SizeType                                                    \
        >                                                                      \
typename enable_if                                                             \
<                                                                              \
  mpl::equal_to                                                                \
  <                                                                            \
    typename fusion::result_of::size                                           \
      < typename NaryRangeFunType::iterator_sequence_type >::type              \
  , mpl::size_t< curr_num_iterators >                                          \
  >                                                                            \
>                                                                              \
::type                                                                         \
perform_nary_range_fun_operation( NaryRangeFunType& target                     \
                                , SizeType begin_index                         \
                                , SizeType section_length                      \
                                )                                              \
{                                                                              \
  function_m( BOOST_PP_CAT( BOOST_PP_ENUM_, z )                                \
                ( curr_num_iterators, BOOST_ACT_DETAIL_OFFSET_ITERATOR, dummy )\
            , section_length                                                   \
            );                                                                 \
}

BOOST_PP_REPEAT( BOOST_PP_INC( BOOST_ACT_MAX_MAX_RANGES )
               , BOOST_ACT_DETAIL_PERFORM_NARY_RANGE_FUN_OP
               , BOOST_PP_NIL
               )

}

// ToDo: Vary implementation depending on a scheduler policy
template< typename IteratorSequence
        , typename DifferenceType
        , typename FunctionType
        >
class nary_parallel_ranges_fun
{
  template< typename NaryRangeFunType, typename SizeType >
  friend
  void perform_nary_range_operation( NaryRangeFunType& target
                                   , SizeType index, SizeType num_sections
                                   );
public:
  typedef IteratorSequence iterator_sequence_type;
public:
  template< typename SourceIteratorSequenceType >
  nary_parallel_ranges_fun( SourceIteratorSequenceType const& begin_iterators_init
                          , DifferenceType range_size_init
                          , FunctionType const& function_init
                          )
    : begin_iterators_m( begin_iterators_init )
    , range_size_m( range_size_init )
    , function_m( function_init )
  {
  }
public:
  template< typename SizeType >
  void operator ()( SizeType index, SizeType num_sections )
  {
    detail::perform_nary_range_operation( *this, index, num_sections );
  }

  template< typename SizeType >
  void operator ()( SizeType index, SizeType num_sections ) const
  {
    detail::perform_nary_range_operation( *this, index, num_sections );
  }
private:
  IteratorSequence begin_iterators_m;
  DifferenceType range_size_m;
  FunctionType function_m;
};

template< typename IteratorSequence
        , typename DifferenceType
        , typename FunctionType
        >
typename enable_if
<
  fusion::traits::is_sequence< IteratorSequence >
, nary_parallel_ranges_fun< IteratorSequence, DifferenceType, FunctionType >
>
::type
make_parallel_ranges_fun( IteratorSequence const& source_sequence
                        , DifferenceType range_size_init
                        , FunctionType function_init
                        )
{
  return nary_range_fun< IteratorSequence, DifferenceType, FunctionType >
           ( source_sequence, range_size_init, function_init );
}

template< typename DifferenceType
        , typename FunctionType
        >
nary_parallel_ranges_fun< fusion::vector<>
                        , DifferenceType
                        , FunctionType
                        >
make_parallel_ranges_fun( DifferenceType range_size_init
                        , FunctionType function_init
                        )
{
  return nary_parallel_ranges_fun< fusion::vector<>
                                 , DifferenceType, FunctionType
                                 >
           ( fusion::vector<>()
           , range_size_init
           , function_init
           );
}

#define BOOST_ACT_DETAIL_REF_PARAM( z, curr_index, dummy )                     \
ref( BOOST_PP_CAT( iterator, curr_index ) )

#define BOOST_ACT_DETAIL_MAKE_NARY_RANGE_FUN( z, curr_num_params, dummy )      \
template< BOOST_PP_ENUM_PARAMS_Z( z, curr_num_params, typename IteratorType )  \
        , typename DifferenceType                                              \
        , typename FunctionType                                                \
        >                                                                      \
typename disable_if                                                            \
<                                                                              \
  fusion::traits::is_sequence< IteratorType0 >                                 \
, nary_parallel_ranges_fun< fusion::vector                                     \
                              < BOOST_PP_ENUM_PARAMS_Z( z                      \
                                                      , curr_num_params        \
                                                      , IteratorType           \
                                                      )                        \
                              >                                                \
                          , DifferenceType                                     \
                          , FunctionType                                       \
                          >                                                    \
>                                                                              \
::type                                                                         \
make_parallel_ranges_fun( BOOST_PP_ENUM_BINARY_PARAMS_Z( z, curr_num_params    \
                                                       , IteratorType          \
                                                       , const& iterator       \
                                                       )                       \
                        , DifferenceType range_size_init                       \
                        , FunctionType function_init                           \
                        )                                                      \
{                                                                              \
  typedef fusion::vector< BOOST_PP_ENUM_PARAMS_Z( z                            \
                                                , curr_num_params              \
                                                , IteratorType                 \
                                                )                              \
                        >                                                      \
          begin_iterators_type;                                                \
                                                                               \
  return nary_parallel_ranges_fun< begin_iterators_type, DifferenceType        \
                                 , FunctionType                                \
                                 >                                             \
           ( make_vector( BOOST_PP_CAT( BOOST_PP_ENUM_, z )                    \
                          ( curr_num_params                                    \
                          , BOOST_ACT_DETAIL_CREF_PARAM                        \
                          , BOOST_PP_NIL                                       \
                          )                                                    \
                        )                                                      \
           , range_size_init                                                   \
           , function_init                                                     \
           );                                                                  \
}

BOOST_PP_REPEAT_FROM_TO( 1, BOOST_PP_INC( BOOST_ACT_MAX_MAX_RANGES )
                       , BOOST_ACT_DETAIL_MAKE_NARY_RANGE_FUN
                       , BOOST_PP_NIL
                       )

}
}

#endif
