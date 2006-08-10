//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_TUPLE_TRAITS_HPP_20060613
#define BOOST_COROUTINE_TUPLE_TRAITS_HPP_20060613
#include <boost/tuple/tuple.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/coroutine/detail/arg_max.hpp>

namespace boost { namespace coroutines {
  namespace detail {
    /*
     * NOTE & FIXME: coroutine_traits relies on the fact that we can construct a
     * boost::tuple specifing more arguments of type null_type than
     * required without generating an error. This is an undocumented
     * 'feature' and doesn't actually  work for nullary tuples,
     * so we need to detect this case and handle it with tuple_workaround.
     * If ever boost::tuple is changed (for example by switching to fusion)
     * tuple_workaround should be used in all cases to handle extra 
     * parameters.
     * The real solution would be to have an internal tuple
     * type that derives from boost::tuple and handles all cases we care about.
     * Or better, just use Boost.Fusion.
     * In general the code in this file needs to be put in a better shape,
     * eliminating all corner cases.
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

  // All tuple traits must be derived from this
  // class to be correctly recognized.
  struct tuple_traits_tag {};
      
  template<typename T>
  struct get_length {
    static const int length = T::length;
  };

  // Given a tuple_traits, makes a tuple of it
  // Simply returns the internal tuple type, unless
  // the tuple is nullary, then apply the nullary tuple workaround
  // See above comments.
  template<typename T>
  struct make_as_tuple :
    boost::mpl::if_c<
      T::length == 0,
      detail::tuple_workaround,
      typename T::internal_tuple_type
    > {};

  // Used to implement the next metafunction,
  // Splitted in two parts to satisfy the compiler.
  template<typename T>
  struct step_2 :
      boost::mpl::eval_if_c<
      get_length<T>::length == 1,
      boost::tuples::element<0, typename make_as_tuple<T>::type >,
    boost::mpl::identity<typename make_as_tuple<T>::type> > { };

  // Given a trait class return the internal tuple type modified 
  // as a return value.
  // The algorithm is as follow:
  // - If the tuple is nullary returns 'void'. 
  // - If it singular returns the first type
  // - Else return the tuple itself.
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
  struct tuple_traits : tuple_traits_tag {
  public:	

    // This is the straightforward boost::tuple trait
    // derived from the argument list. It is not 
    // directly used in all cases.
    typedef boost::tuple
    <BOOST_PP_ENUM_PARAMS
    (BOOST_COROUTINE_ARG_MAX, T)> internal_tuple_type;

    // FIXME: Currently coroutine code does not use this typedef in all cases
    // and expect it to be equal to boost::tuples::null_type
    typedef boost::tuples::null_type null_type;
    static const int length = boost::tuples::length<internal_tuple_type>::value;

    // Return the element at the Indext'th position in the typelist.
    // If the index is not less than the tuple length, it returns 
    // null_type.
    template<int Index> 
    struct at : 
      boost::mpl::eval_if_c<
      Index < 
      boost::tuples::length<typename tuple_traits::internal_tuple_type>::value,
      boost::tuples::element<Index, typename tuple_traits::internal_tuple_type>,	
      boost::mpl::identity<typename tuple_traits::null_type> >{};

    typedef typename make_as_tuple<tuple_traits>::type
    as_tuple;	

    typedef typename make_result_type<tuple_traits>::type as_result;

  };
    
  template<typename T>
  struct is_tuple_traits : boost::is_base_of<tuple_traits_tag, T> {};
} }
#endif
