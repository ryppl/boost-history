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

#ifndef BOOST_ACT_ALGORITHM_GENERATE_HPP
#define BOOST_ACT_ALGORITHM_GENERATE_HPP

#include "detail/make_algo.hpp"

#include "algorithm_fwd.hpp"

#include "../parallel_safe.hpp"

#include "for_each.hpp"

#include "generate/generate_fwd.hpp"

namespace boost
{
namespace act
{
namespace detail
{

template< typename FunctionType >
class safe_generator
  : public parallel_safe_if< is_parallel_safe< FunctionType > >
{
public:
  safe_generator( FunctionType function_init )
    : function_m( function_init )
  {
  }
public:
  template< typename TargetType >
  void operator ()( TargetType& target ) const
  {
    target = function_m();
  }
private:
  mutable FunctionType function_m;
};

template< typename FunctionType >
safe_generator< FunctionType >
make_safe_generator( FunctionType function_init )
{
  return safe_generator< FunctionType >( function_init );
}

}

BOOST_ACT_DETAIL_IMPLEMENT_ALGO( ((typename),IteratorType)
                                 ((typename),FunctionType)
                               , (void)
                               , generate
                               , ((IteratorType),begin)
                                 ((IteratorType),end)
                                 ((FunctionType),function)
                               )
{
  for_each[ AlgoModel() ]( begin, end
                         , detail::make_safe_generator( function )
                         );
}

}
}

#endif
