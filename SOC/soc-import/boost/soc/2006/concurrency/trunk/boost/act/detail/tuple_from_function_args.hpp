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

#ifndef BOOST_ACT_DETAIL_TUPLE_FROM_FUNCTION_ARGS_HPP
#define BOOST_ACT_DETAIL_TUPLE_FROM_FUNCTION_ARGS_HPP

namespace boost
{
namespace act
{
namespace detail
{

template< typename FunctionType
        , typename Enabler = void
        >
struct tuple_from_function_args;

template< typename FunctionType >
struct tuple_from_function_args
       <
         FunctionType
       , typename ::boost::enable_if
           < ::boost::is_function< FunctionType > >::type
       >
  : tuple_from_sequence< typename argument_sequence< FunctionType >::type >
{
};

template< typename ActableFunctionType >
struct tuple_from_function_args
       <
         ActableFunctionType
       , typename ::boost::enable_if
         <
           is_actable_function< ActableFunctionType >
         >
         ::type
       >
  : tuple_from_function< typename ActableFunctionType::logical_function_type >
{
};

}
}
}

#endif
