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

#ifndef BOOST_ACTIVE_FUNCTION_HPP
#define BOOST_ACTIVE_FUNCTION_HPP

namespace boost
{
  namespace detail
  {
    class dummy_active_function_param;

    template< typename ChildType
            , typename ReturnType
              BOOST_PP_ENUM_TRAILING_BINARY_PARAMS
              (
                BOOST_ACTIVE_FUNCTION_NUM_PARAMS
              , Param
              , = dummy_active_function_param BOOST_PP_INTERCEPT
              )
            >
    class active_function_base;

    template< typename ChildType
            , typename ReturnType
              BOOST_PP_ENUM_TRAILING_PARAMS_Z
              (
                z
              , curr_num_params
              , Param
              )
            >
    class active_function_base< ChildType
                              , ReturnType
                                BOOST_PP_ENUM_TRAILING_PARAMS_Z
                                (
                                  z
                                , curr_num_params
                                , Param
                                )
                              >
    {
      // ToDo: Handle const better
    protected:
      typedef activity< active_object< ReturnType > > result_type;
    protected:
      // ToDo: Add enable_if conditions or change params
      BOOST_PP_IF( curr_num_params
                 , BOOST_ACTIVE_FUNCTION_DETAIL_CALL_TEMPLATE
                 , BOOST_ACTIVE_FUNCTION_DETAIL_CALL_DUMMY_TEMPLATE
                 )
      (
        z
      , curr_num_params
      , CallParam
      )
      result_type
      operator ()( BOOST_PP_ENUM_BINARY_PARAMS_Z( z
                                                , curr_num_params
                                                , CallParam
                                                , arg
                                                )
                 ) const
      {
        // ToDo: Make thread
        return child_function_object()
               (
                 BOOST_ACTIVE_FUNCTION_DETAIL_FORWARDED_ARGS
                 (
                   z
                 , curr_num_args
                 , arg
                 )
               );
      }
    };
  }

  template< typename FunObjectType, typename FunTraits >
  class active_function
  {
  public:
    active_function( FunObjectType fun_object_init = FunObjectType() )
      : fun_object_m( fun_object_init )
    {
    }
  public:
    typename FunTraits::result_type
    operator ()
  private:
    FunObjectType fun_object_m;
  };

template< typename LogicalFunctionType
        , typename FunctionObjectType
        , typename ActModel
        >
active_function< LogicalFunctionType, FunctionObjectType, ActModel >
make_active_function( FunctionObjectType const& function )
{
  return active_function< LogicalFunctionType
                        , FunctionObjectType
                        , ActModel
                        >
                        ( function );
}

}

#endif
