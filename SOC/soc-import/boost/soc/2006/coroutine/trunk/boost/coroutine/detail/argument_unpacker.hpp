//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_ARGUMENT_UNPACKER_HPP_20060601
#define BOOST_COROUTINE_ARGUMENT_UNPACKER_HPP_20060601
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/coroutine/detail/index.hpp>

namespace boost { namespace coroutines { namespace detail {
  template<typename Traits, int Len>
  struct unpacker_n;

  template<typename Traits, int Len>
  struct unpacker_ex_n;


#define BOOST_COROUTINE_ARGUMENT_UNPACKER(z, len, unused)              \
    template<typename Traits>                                          \
    struct unpacker_n<Traits, len> {                                   \
        template<typename Functor, typename Tuple>                     \
      struct result {/*for result_of compatibility*/                   \
	  typedef typename  boost::result_of                           \
             <Functor(BOOST_PP_ENUM_BINARY_PARAMS                      \
		      (len, typename Traits::template at<index_ , >    \
		       ::type BOOST_PP_INTERCEPT))>::type type;        \
      };                                                               \
      template<typename Functor, typename Tuple>                       \
      typename result<Functor, Tuple>::type operator()                 \
       (Functor& f, Tuple& parms){                                     \
           using boost::get; /*tuples::get cannot be found via ADL*/   \
           return f(BOOST_PP_ENUM_BINARY_PARAMS                        \
		    (len,                                              \
		     get<index_, >                                     \
		     (parms) BOOST_PP_INTERCEPT));                     \
      }                                                                \
    };                                                                 \
/**/
    
#define BOOST_COROUTINE_ARGUMENT_UNPACKER_EX(z, len, unused)           \
    template<typename Traits>                                          \
    struct unpacker_ex_n<Traits, len >           {                     \
    template<typename Functor,                                         \
             typename First,                                           \
	     typename Tuple>                                           \
      struct result {                                                  \
	  typedef typename  boost::result_of                           \
             <Functor(First BOOST_PP_COMMA_IF(len)                     \
		      BOOST_PP_ENUM_BINARY_PARAMS                      \
		      (len, typename Traits                            \
		       ::template at<index_ , >::type BOOST_PP_INTERCEPT))>     \
          ::type type;                                                 \
	};                                                             \
                                                                       \
      template<typename Functor,                                       \
               typename First,                                         \
	       typename Tuple>                                         \
      typename result<Functor, First, Tuple>::type                     \
      operator()(Functor& f, First arg0, Tuple& parms){                \
           using boost::get; /*tuples::get cannot be found via ADL*/  \
           return f(arg0                                               \
		    BOOST_PP_COMMA_IF(len)                             \
		    BOOST_PP_ENUM_BINARY_PARAMS                        \
		    (len,                                              \
		     get<index_, >                                     \
		     (parms) BOOST_PP_INTERCEPT) );                    \
      }                                                                \
    };                                                                 \
/**/

  BOOST_PP_REPEAT(BOOST_COROUTINE_ARG_MAX, 
		  BOOST_COROUTINE_ARGUMENT_UNPACKER, ~);
  BOOST_PP_REPEAT(BOOST_COROUTINE_ARG_MAX, 
		  BOOST_COROUTINE_ARGUMENT_UNPACKER_EX, ~);

  /**
   * Inovke function object @p f passing  all 
   * elements in tuple @p parms as distinct parameters.
   */
  template<typename Traits, typename Functor, typename Tuple>
  inline
  typename unpacker_n<Traits, Traits::length>
  ::template result<Functor, Tuple>::type
  unpack(Functor f, Tuple& parms) {
    return unpacker_n<Traits, Traits::length>()(f, parms);
  }

  /**
   * Inovke function object @p f passing argument @p arg0 and all 
   * elements in tuple @p parms as distinct parameters.
   */
  template<typename Traits, typename Functor, typename First, typename Tuple>
  inline
  typename unpacker_ex_n<Traits, Traits::length>::
  template result<Functor, First, Tuple>::type 
  unpack_ex(Functor f, First arg0, Tuple& parms) {
    return unpacker_ex_n<Traits, Traits::length>()
      (f, arg0, parms);
  }
} } }
#endif
