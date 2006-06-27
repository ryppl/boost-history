//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_TUPLE_HPP_20060613
#define BOOST_COROUTINE_TUPLE_HPP_20060613
#include <boost/tuple/tuple.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/bool.hpp>

namespace boost { namespace coroutines {
  namespace detail {
    /*
     * NOTE: coroutine_traits relies on the fact that we can construct a
     * boost::tuple specifing more arguments of type null_type than
     * required without generating an error. This is an undocumented
     * 'feature'. This actually does not work for nullary tuples,
     * so we need to detect this case and handle it with tuple_workaround.
     * If ever boost::tuple is changed (for example by switching to fusion)
     * tuple_workaround should be used in all cases to handle extra 
     * parameters.
     */

    /* 
     * A boost::tuple<> is not constructible from an arbitrary
     * number of null_types (while non nullary tuples are).
     * This class takes care of this assimmetry.
     */
    struct tuple_workaround : boost::tuple<> {
	
#     define BOOST_COROUTINE_arg_null_typecr(z, n, unused)\
	const boost::tuples::null_type&                   \
      /**/
	
      tuple_workaround
      (BOOST_PP_ENUM
       (BOOST_COROUTINE_ARG_MAX, 
	BOOST_COROUTINE_arg_null_typecr,
	~)){}
	
      tuple_workaround(const tuple_workaround&) {}
      tuple_workaround() {}
#     undef BOOST_COROUTINE_arg_null_typecr
    };
  } /* detail */

  struct tuple_tag {};
      
  template<typename T>
  struct get_length {
    static const int length = T::length;
  };

  template<typename T>
  struct make_as_tuple :
    boost::mpl::if_c<
      T::length == 0,
      detail::tuple_workaround,
      typename T::internal_type
    > {};

  template<typename T>
  struct step_2 :
      boost::mpl::eval_if_c<
      get_length<T>::length == 1,
      boost::tuples::element<0, typename make_as_tuple<T>::type >,
    boost::mpl::identity<typename make_as_tuple<T>::type> > { };

  template<typename T>
  struct make_result_type :
    boost::mpl::eval_if_c<
    get_length<T>::length == 0,
    boost::mpl::identity<void>,
    step_2<T> > { };

  template<
    BOOST_PP_ENUM_BINARY_PARAMS
  (BOOST_COROUTINE_ARG_MAX,
   typename T, 
   = boost::tuples::null_type BOOST_PP_INTERCEPT)>
  struct tuple : tuple_tag {
  private:
    /* this is a pure compile time type */
    tuple();
    tuple(const tuple&);
    void operator=(const tuple&);
  public:	
    typedef boost::tuple
    <BOOST_PP_ENUM_PARAMS
    (BOOST_COROUTINE_ARG_MAX, T)> internal_type;

    typedef boost::tuples::null_type null_type;
    static const int length = boost::tuples::length<internal_type>::value;

    template<int Index> 
    struct at : 
      boost::mpl::eval_if_c<
      Index < 
      boost::tuples::length<typename tuple::internal_type>::value,
      boost::tuples::element<Index, typename tuple::internal_type>,	
      boost::mpl::identity<typename tuple::null_type> >{};

    typedef typename make_as_tuple<tuple>::type
    as_tuple;	

    typedef typename make_result_type<tuple>::type as_result;

  };
    
  template<typename T>
  struct is_tuple : boost::is_convertible<T, tuple_tag> {};
} }
#endif
