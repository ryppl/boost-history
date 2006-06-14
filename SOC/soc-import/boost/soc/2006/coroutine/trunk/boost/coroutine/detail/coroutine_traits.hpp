//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_DETAIL_COROUTINE_TRAITS_HPP_20060613
#define BOOST_COROUTINE_DETAIL_COROUTINE_TRAITS_HPP_20060613
#include <boost/type_traits.hpp>
#include <boost/coroutine/tuple.hpp>
#include <boost/coroutine/detail/yield_result_type.hpp>


namespace boost { namespace coroutines { namespace detail {

  template<typename T>
  struct as_tuple {
    typedef typename T::as_tuple type;
  };

  template<typename Signature>
  struct coroutine_traits {    
  private:
    typedef typename boost::function_traits<Signature>::result_type 
    internal_result_type;
  public:
    typedef typename 
    boost::mpl::eval_if<is_tuple<internal_result_type>,
			as_tuple<internal_result_type>,
			boost::mpl::identity<internal_result_type> >
    ::type result_type;

    typedef typename 
    boost::mpl::eval_if<is_tuple<internal_result_type>,
			boost::mpl::identity<internal_result_type>,
			boost::mpl::if_
			<boost::is_same<internal_result_type, void>,
			 tuple<>,
			 tuple<internal_result_type> > >
    ::type result_slot_traits;

    typedef typename result_slot_traits
    ::as_tuple result_slot_type;
    	     
    typedef  typename detail::make_tuple
    <typename detail::signature<Signature>::type >
    ::type arg_slot_traits;

    typedef typename arg_slot_traits
    ::as_tuple arg_slot_type;

    typedef typename arg_slot_traits
    ::as_result yield_result_type;

  };
} } }
#endif
