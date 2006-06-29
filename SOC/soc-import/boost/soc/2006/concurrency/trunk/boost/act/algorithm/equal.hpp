//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ALGORITHM_EQUAL_HPP
#define BOOST_ACT_ALGORITHM_EQUAL_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <iterator>

#include "../actable_function.hpp"
#include "../type_traits/is_act_model.hpp"
#include "../type_traits/is_actable_function.hpp"
#include "../type_traits/add_action.hpp"
#include "../active.hpp"
#include "../action.hpp"

#include "../detail/is_of_iterator_category.hpp"

#include "algorithm_fwd.hpp"

// ToDo: Rewrite to minimize iterator and function copies

namespace boost
{
namespace act
{
namespace detail
{

template< typename IteratorType, typename FunctionType, typename ActModel >
struct equal_logarithmic;

template< typename IteratorType, typename FunctionType, typename ActModel >
struct equal_linear;

}
/*
template< typename ActModel, typename IteratorType, typename FunctionType >
typename ::boost::enable_if
<
  ::boost::mpl::and_
  <
    is_asynchronous_act_model< ActModel >
  , is_actable_function< FunctionType >
  , detail::is_of_iterator_category< IteratorType
                                   , ::std::random_access_iterator_tag
                                   >
  >
, action< void, ActModel >
>
::type
for_each( IteratorType const& curr, IteratorType IteratorType const& end
        , FunctionType IteratorType const& function
        )
{
  return active_call< ActModel >( detail::for_each_logarithmic
                                  <
                                    IteratorType
                                  , FunctionType
                                  , ActModel
                                  >
                                  ()
                                , curr, end, function
                                );
}

template< typename ActModel, typename IteratorType, typename FunctionType >
typename ::boost::enable_if
<
  ::boost::mpl::and_
  <
    is_act_model< ActModel >
  , is_actable_function< FunctionType >
  , ::boost::mpl::or_
    <
      is_synchronous_act_model< ActModel >
    , ::boost::mpl::not_
      <
        detail::is_of_iterator_category< IteratorType
                                       , ::std::random_access_iterator_tag
                                       >
      >
    >
  >
, action< void, ActModel >
>
::type
for_each( IteratorType const& curr, IteratorType IteratorType const& end
        , FunctionType IteratorType const& function
        )
{
  return active_call< ActModel >( detail::for_each_linear
                                  <
                                    IteratorType
                                  , FunctionType
                                  , ActModel
                                  >
                                  ()
                                , curr, end, function
                                );
}

// ToDo: Put remove_cv into a metafunction call
template< typename IteratorType, typename FunctionType >
typename ::boost::enable_if
<
  ::boost::mpl::and_
  <
    ::boost::mpl::not_< is_act_model< IteratorType > >
  , is_actable_function< FunctionType >
  , detail::is_of_iterator_category< IteratorType
                                   , ::std::random_access_iterator_tag
                                   >
  >
, action< void >
>
::type
for_each( IteratorType const& curr, IteratorType IteratorType const& end
        , FunctionType IteratorType const& function
        )
{
  return for_each< default_act_model >( curr, end, function );
}
*/
namespace detail
{

// ToDo: Internally use an atomic state object for all branches
//       if an atomic type can be made portable
template< typename LeftIteratorType, typename RightIteratorType
        , typename FunctionType, typename ActModel
        >
struct equal_logarithmic
    <
      bool ( LeftIteratorType, LeftIteratorType
           , RightIteratorType, FunctionType
           )
    >
{
  template< typename PassedLeftIteratorType, typename PassedRightIteratorType
          , typename PassedFunctionType
          >
  bool operator ()( PassedLeftIteratorType const& left_curr
                  , PassedLeftIteratorType const& left_end
                  , PassedRightIteratorType const& right_curr
                  , PassedFunctionType const& function
                  )
                  const
  {
    if( curr != end )
    {
      typedef typename ::boost::iterator_difference< IteratorType >
                ::type difference_type;

      difference_type const range_size = end - curr;

      if( range_size == 1 )
      {
        FunctionType function_copy = function;

        // ToDo: Possibly unwrap if is an active function
        return function_copy( *left_curr, *right_curr );
      }
      else
      {
        difference_type const half_range = range_size / 2;

        PassedLeftIteratorType const left_middle = left_curr + half_range;

        action< bool, ActModel > const left_action
          = active_call( equal_logarithmic()
                       , left_curr, left_middle, right_curr, function
                       );

        PassedRightIteratorType const right_middle = right_curr + half_range;

        return    (*this)( left_curr, left_middle, right_curr, function )
               || left_action->inactive_copy();
      }
    }
  }
};

template< typename LeftIteratorType, typename RightIteratorType
        , typename FunctionType, typename ActModel
        >
struct equal_linear
  : actable_function
    <
      bool ( LeftIteratorType, LeftIteratorType
           , RightIteratorType, FunctionType
           )
    >
{
  template< typename PassedLeftIteratorType, typename PassedRightIteratorType
          , typename PassedFunctionType
          >
  bool operator ()( PassedLeftIteratorType const& left_curr
                  , PassedLeftIteratorType const& left_end
                  , PassedRightIteratorType const& right_curr
                  , PassedFunctionType const& function
                  )
                  const
  {
    // ToDo: Convert function to active function
    return ::std::equal( left_curr, left_end, right_curr, function );
  }
};

}
}
}

#endif
