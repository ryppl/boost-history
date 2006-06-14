//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_DETAIL_IS_CALLABLE_HPP_20060601
#define BOOST_COROUTINE_DETAIL_IS_CALLABLE_HPP_20060601
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/logical.hpp>
#include <boost/mpl/has_xxx.hpp>
namespace boost { namespace coroutines { namespace detail {
  template<typename T>
  struct is_function_pointer : 
    boost::mpl::and_<
    boost::is_pointer<T>,
    boost::is_function<typename boost::remove_pointer<T>::type > > {
    typedef is_function_pointer<T> type;
  };

  BOOST_MPL_HAS_XXX_TRAIT_DEF(result_type);
  BOOST_MPL_HAS_XXX_TRAIT_DEF(result);
  
  template<typename T>
  struct is_functor : 
    boost::mpl::or_<typename has_result_type<T>::type,
		    typename has_result<T>::type>
  {
    typedef is_functor<T> type;
  };

  template<typename T>
  struct is_callable : boost::mpl::or_<
    is_functor<T>,
    is_function_pointer<T> >::type {
    typedef is_callable<T> type;
  };
} } }
#endif
