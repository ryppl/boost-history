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

#ifndef BOOST_ACT_DETAIL_FUNCTION_PACKAGE_HPP
#define BOOST_ACT_DETAIL_FUNCTION_PACKAGE_HPP

#include "queued_function_caller.hpp"
#include "active_function_caller.hpp"
#include "params_to_stored_params.hpp"
#include "tuple_from_sequence.hpp"
#include <boost/mpl/empty.hpp>

namespace boost
{
namespace act
{
namespace detail
{

template< typename ResultType
        , typename ActModel
        , typename ParamSequence
        , typename FunctionType
        , typename TupleType
        >
class function_package
{
public:
  typedef ResultType result_type;
  typedef mpl::empty< ParamSequence > param_sequence_is_empty;
public:
  function_package( FunctionType const& function_init
                  , TupleType const& arguments_init
                  )
    : function_m( function_init )
    , arguments_m( arguments_init )
  {
  }
public:
  class stored_function_type
  {
  public:
    typedef ResultType result_type;
  private:
    typedef typename detail::params_to_stored_params
              < ActModel, ParamSequence >::type
                stored_params_sequence;

    typedef typename detail::tuple_from_sequence
              < stored_params_sequence >::type
                stored_params_type;
  public:
    stored_function_type( FunctionType const& function_init
                        , TupleType const& arguments_init
                        )
      : function_m( function_init )
      , arguments_m( arguments_init )
    {
    }
  public:
    result_type operator ()() const
    {
      return detail::active_function_caller< result_type >
               ( function_m, arguments_m );
    }

    template< typename TargetType >
    result_type operator ()( TargetType* const target ) const
    {
      return detail::queued_function_caller< result_type >
               ( target, function_m, arguments_m );
    }
  private:
    FunctionType function_m;
    stored_params_type arguments_m;
  };
public:
  stored_function_type store_function() const
  {
    return stored_function_type( function_m, arguments_m );
  }
private:
  FunctionType const& function_m;
  TupleType const& arguments_m;
};

template< typename ResultType
        , typename ActModel
        , typename ParamSequence
        , typename FunctionType
        , typename ArgumentsType
        >
function_package< ResultType, ActModel
                , ParamSequence, FunctionType, ArgumentsType
                >
make_function_package( FunctionType const& function
                     , ArgumentsType const& arguments
                     )
{
  return function_package< ResultType, ActModel
                         , ParamSequence, FunctionType, ArgumentsType
                         >( function, arguments );
}

}
}
}

#endif
