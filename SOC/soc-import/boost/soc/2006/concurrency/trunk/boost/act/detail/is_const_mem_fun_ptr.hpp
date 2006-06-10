//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

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

BOOST_PP_REPEAT( BOOST_ACT_MAX_ACTIVE_CALL_PARAMS
               , BOOST_ACT_DETAIL_IS_CONST_MEM_FUN_PTR_SPEC
               , BOOST_PP_NIL
               )

#undef BOOST_ACT_DETAIL_IS_CONST_MEM_FUN_PTR_SPEC

}
}
}

#endif
