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

#ifndef BOOST_ACT_DETAIL_IS_CONST_MEM_FUN_PTR_HPP
#define BOOST_ACT_DETAIL_IS_CONST_MEM_FUN_PTR_HPP

#include <boost/mpl/bool.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

#include "../config/max_active_call_params.hpp"

namespace boost
{
namespace act
{
namespace detail
{

template< typename Type >
struct is_const_mem_fun_ptr
  : ::boost::mpl::false_
{
};

#define BOOST_ACT_DETAIL_IS_CONST_MEM_FUN_PTR_SPEC( z, num_params, dummy )     \
template< typename ReturnType, typename DataType                               \
          BOOST_ENUM_TRAILING_PARAMS_Z( z, num_params, typename Param )        \
        >                                                                      \
struct is_const_mem_fun_ptr                                                    \
       <                                                                       \
         ReturnType (DataType::*)                                              \
         ( BOOST_PP_ENUM_PARAMS_Z( z, num_params, Param ) ) const              \
       >                                                                       \
  : ::boost::mpl::true_                                                        \
{                                                                              \
}; 

BOOST_PP_REPEAT( BOOST_ACT_MAX_PARAMS
               , BOOST_ACT_DETAIL_IS_CONST_MEM_FUN_PTR_SPEC
               , BOOST_PP_NIL
               )

#undef BOOST_ACT_DETAIL_IS_CONST_MEM_FUN_PTR_SPEC

}
}
}

#endif
