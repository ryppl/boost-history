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

#ifndef BOOST_ACT_ALGORITHM_ALGORITHM_FOR_EACH_IF_HPP
#define BOOST_ACT_ALGORITHM_ALGORITHM_FOR_EACH_IF_HPP

#include "../detail/make_algo.hpp"

#include "for_each_if/for_each_if_fwd.hpp"

#include <boost/fusion/sequence/intrinsic/front.hpp>
#include <boost/fusion/sequence/container/vector.hpp>
#include <boost/fusion/sequence/view/transform_view.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/type_traits/is_same.pp>
#include <boost/utility/enable_if.hpp>

#include <boost/act/detail/call_with_sequence.hpp>
#include <boost/act/config/num_ranges.hpp>

namespace boost
{
namespace act
{
namespace detail
{

struct deref_iterator
{
  template< typename Type >
  struct result
    : iterator_reference< Type >
  {
  };

  template< typename Type >
  typename result< Type >::type
  operator ()( Type const& iterator ) const
  {
    return *iterator;
  }
};

template< typename FunctionType, typename ReturnType = void >
class call_with_seq_fun
{
public:
  call_with_seq_fun( FunctionType& function_init )
    : function_m( function_init )
  {
  }
public:
  template< typename SequenceType >
  void operator ()( SequenceType const& arguments ) const
  {
    return call_with_sequence< ReturnType >( function_m, arguments );
  }
private:
  FunctionType& function_m;
};

template< typename FunctionType >
class call_with_seq_fun< FunctionType, void >
{
public:
  call_with_seq_fun( FunctionType& function_init )
    : function_m( function_init )
  {
  }
public:
  template< typename SequenceType >
  void operator ()( SequenceType const& arguments ) const
  {
    call_with_sequence( function_m, arguments );
  }
private:
  FunctionType& function_m;
};

template< typename FunctionType >
call_with_seq_fun< FunctionType >
make_call_with_seq_fun( FunctionType& function_init )
{
  return call_with_seq_fun< FunctionType >( function_init );
}

template< typename ReturnType, typename FunctionType >
call_with_seq_fun< FunctionType, ReturnType >
make_call_with_seq_fun( FunctionType& function_init )
{
  return call_with_seq_fun< FunctionType, ReturnType >( function_init );
}

template< typename IteratorSequence >
struct are_random_access_iterators
  : is_same
    <
      typename mpl::find_if
      <
        IteratorSequence
      , mpl::not_
        <
          is_same< iterator_category< mpl::_1 >
                 , typename ::std::random_access_iterator_tag
                 >
        >
      >
      ::type
    , typename mpl::end< IteratorSequence >::type
    >
{
};

// ToDo: Possibly store references for datamembers
template< typename SequenceType, typename Condition, typename Function >
class for_each_if_parallel_fun
{
private:
  typedef typename fusion::result_of::value_at_c< SequenceType, 0 >::type
            front_type;
public:
  for_each_if_parallel_fun( SequenceType const& begin_iterators_init
                          , Condition const& condition_init
                          , Function const& function_init
                          )
    : begin_iterators_m( begin_iterators_init )
    , condition_m( condition_init )
    , function_m( function_init )
  {
  }
public:
  template< typename SizeType >
  void operator ()( SizeType index, SizeType length ) const
  {
    SequenceType curr_iterators
      = fusion::transform( begin_iterators_m, _1 + index );

    using fusion::front;

    fusion::transform_view< SequenceType, detail::deref_iterator > const
      dereferenced_iterators( curr_iterators, detail::deref_iterator() );

    for( front_type const end = front( curr_iterators ) + length
       ; front( curr_iterators ) != end0
       ; fusion::for_each( seq, ++_1 )
       )
      if( condition_m( dereferenced_iterators ) )
        function_m( dereferenced_iterators );

    return function;
  }
private:
  SequenceType begin_iterators_m;
  Condition condition_m;
  Function function_m;
};

template< typename SequenceType, typename Condition, typename Function >
for_each_if_parallel_fun< SequenceType, Condition, Function >
make_for_each_if_parallel_fun( SequenceType const& begin_iterators_init
                             , Condition const& condition_init
                             , Function const& function_init
                             )
{
  return for_each_if_parallel_fun< SequenceType, Condition, Function >
           ( begin_iterators_init, condition_init, function_init );
}

}

BOOST_ACT_DETAIL_PREPARE_ALGO_IMPLEMENTATION
(
  for_each_if
, BOOST_ACT_MAX_RANGES
, BOOST_ACT_DETAIL_FOR_EACH_IF_OVERLOADS()
)

template< typename AlgoModel >
struct for_each_if_seq_impl
{
  template< typename ExtendedParamsType
          , typename SeqType
          , typename FunType
          , typename Condition
          >
  static
  typename disable_if
  <
    detail::are_random_access_iterators< typename SeqType::types >
  , FunType
  >
  ::type execute( ExtendedParamsType const& extended_params
                , SeqType& begin_iterators
                , typename mpl::front< typename SeqType::types >::type& end0
                , FunType& function
                , Condition& condition
                )
  {
    using fusion::front;

    fusion::transform_view< SequenceType, detail::deref_iterator > const
      dereferenced_iterators( begin_iterators, detail::deref_iterator() );

    for( ; front( begin_iterators ) != end0; fusion::for_each( seq, ++_1 ) )
      if( condition_m( dereferenced_iterators ) )
        function_m( dereferenced_iterators );

    return function;
  }

  template< typename ExtendedParamsType
          , typename SeqType
          , typename FunType
          , typename Condition
          >
  static
  typename disable_if
  <
    detail::are_random_access_iterators< typename SeqType::types >
  , FunType
  >
  ::type execute( ExtendedParamsType const& extended_params
                , SeqType& begin_iterators
                , typename mpl::front< typename SeqType::types >::type& end0
                , FunType& function
                , Condition& condition
                )
  {
    parallelize< AlgoModel >
      ::execute( extended_params
               , detail::make_for_each_if_parallel_fun( begin_iterators
                                                      , condition
                                                      , function
                                                      )
               );

    return function;
  }
};

#define BOOST_ACT_DETAIL_FOR_EACH_IF_IMPL( z, curr_num_ranges, dummy )         \
BOOST_ACT_DETAIL_IMPLEMENT_ALGO_OVER                                           \
(                                                                              \
  BOOST_PP_CAT(BOOST_PP_REPEAT_,z)                                             \
    (curr_num_ranges,BOOST_ACT_DETAIL_FOR_EACH_IT,BOOST_PP_NIL)                \
  ((typename),FunctionType)((typename),Condition)                              \
, (FunctionType)                                                               \
, for_each_if                                                                  \
, ((IteratorType0),begin0)                                                     \
  ((IteratorType0),end0)                                                       \
  BOOST_PP_REPEAT_FROM_TO_Z( z, 1, curr_num_ranges                             \
                           , BOOST_ACT_DETAIL_FOR_EACH_IT, BOOST_PP_NIL        \
                           )                                                   \
  ((FunctionType),function)((Condition),condition)                             \
)                                                                              \
{                                                                              \
  typedef fusion::vector< BOOST_PP_ENUM_PARAMS_Z( z, curr_num_ranges           \
                                                , IteratorType                 \
                                                )                              \
                        >                                                      \
          sequence_type;                                                       \
                                                                               \
  sequence_type begin_iterators( BOOST_PP_ENUM_PARAMS_Z( z, curr_num_ranges    \
                                                       , first                 \
                                                       )                       \
                               );                                              \
                                                                               \
  detail::call_with_seq_fun< FunctionType > fun( function );                   \
                                                                               \
  detail::call_with_seq_fun< Condition, bool > cond( condition );              \
                                                                               \
  return for_each_if_seq_impl< AlgoModel >                                     \
           ::execute( begin_iterators, end0, fun, cond );                      \
}

BOOST_PP_REPEAT( BOOST_ACT_MAX_RANGES
               , BOOST_ACT_DETAIL_FOR_EACH_IF_IMPL
               , BOOST_PP_NIL
               )

}
}

#endif
