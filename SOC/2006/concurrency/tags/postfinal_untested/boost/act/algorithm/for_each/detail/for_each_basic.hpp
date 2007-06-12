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

#ifndef BOOST_ACT_ALGORITHM_FOR_EACH_DETAIL_FOR_EACH_BASIC_HPP
#define BOOST_ACT_ALGORITHM_FOR_EACH_DETAIL_FOR_EACH_BASIC_HPP

#include <boost/iterator/iterator_traits.hpp>

#include "../../../type_traits/is_algo_model.hpp"
#include "../../../type_traits/is_parallel_safe.hpp"
#include "../../../parallel_safe.hpp"
#include "../../basic_for.hpp"

namespace boost
{
namespace act
{
namespace detail
{

template< typename IteratorType, typename FunctionType >
class for_each_basic_applier
  : public parallel_safe_if< is_parallel_safe< FunctionType > >
{
public:
  for_each_basic_applier( IteratorType const& begin_init
                        , FunctionType const& function_init
                        )
    : begin_m( begin_init )
    , function_m( function_init )
  {
  }
private:
  typedef typename ::boost::iterator_difference< IteratorType >::type
            difference_type;
public:
  // ToDo: Possibly use a calculated size type
  void operator ()( difference_type index ) const
  {
    function_m( *( begin_m + index ) );
  }
public:
  operator FunctionType() const
  {
    return function_m;
  }
private:
  IteratorType begin_m;
  FunctionType function_m;
};

template< typename IteratorType, typename FunctionType >
for_each_basic_applier< IteratorType, FunctionType >
make_for_each_basic_applier( IteratorType const& begin
                           , FunctionType const& function
                           )
{
  return for_each_basic_applier< IteratorType, FunctionType >
           ( begin, function );
}

template< typename ExtendedParamsType
        , typename IteratorType
        , typename FunctionType
        >
FunctionType for_each_basic( ExtendedParamsType const& extended_params
                           , IteratorType begin
                           , IteratorType end
                           , FunctionType const& function
                           )
{
  typedef typename ::boost::iterator_difference< IteratorType >::type
            difference_type;

  return basic_for[ extended_params ]
         ( for_var = static_cast< difference_type >( 0 )
         , for_var < end - begin
         , ++for_var
         )
         [
           make_for_each_basic_applier( begin, function )
         ];
}

}
}
}

#endif
