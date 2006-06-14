//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_ARGUMENT_PACKER_HPP_20060601
#define BOOST_COROUTINE_ARGUMENT_PACKER_HPP_20060601
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/tuple/eat.hpp>

namespace boost { namespace coroutines { namespace detail {
#define BOOST_COROUTINE_DETAIL_TEMPLATE_PARAMETERS(n)  \
  template<BOOST_PP_ENUM_PARAMS(n, typename T)> \
/**/

#define BOOST_COROUTINE_ARGUMENT_PACKER(z, n, parm_tuple)        \
  BOOST_PP_IF(n,                                                 \
	      BOOST_COROUTINE_DETAIL_TEMPLATE_PARAMETERS ,              \
	      BOOST_PP_TUPLE_EAT(1) )(n)                         \
  BOOST_PP_TUPLE_ELEM(3, 0, parm_tuple)                          \
    (BOOST_PP_ENUM_BINARY_PARAMS(n, T, arg)) {                   \
    typedef BOOST_PP_TUPLE_ELEM(3, 2, parm_tuple) parm_type;     \
    return BOOST_PP_TUPLE_ELEM(3, 1, parm_tuple)                 \
                             (parm_type                          \
			      (BOOST_PP_ENUM_PARAMS(n, arg)));   \
  }                                                              \
/**/


#define BOOST_COROUTINE_ARGUMENT_PACKER_EX(z, n, parm_tuple)     \
  template<typename Arg                                          \
    BOOST_PP_COMMA_IF(n)                                         \
    BOOST_PP_ENUM_PARAMS(n, typename T)>                         \
  BOOST_PP_TUPLE_ELEM(3, 0, parm_tuple)                          \
  (Arg arg                                                       \
   BOOST_PP_COMMA_IF(n)                                          \
   BOOST_PP_ENUM_BINARY_PARAMS(n, T, arg)) {                     \
    typedef BOOST_PP_TUPLE_ELEM(3, 2, parm_tuple) parm_type;     \
    return BOOST_PP_TUPLE_ELEM(3, 1, parm_tuple)( arg, parm_type \
     (BOOST_PP_ENUM_PARAMS(n, arg)));                            \
  }                                                              \
/**/

} } }
#endif
