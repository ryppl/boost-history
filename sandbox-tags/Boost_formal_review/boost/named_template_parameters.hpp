// (C) Copyright Jaakko Järvi 2002. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_NAMED_TEMPLATE_PARAMS_HPP
#define BOOST_NAMED_TEMPLATE_PARAMS_HPP

#include "boost/type_traits.hpp"

#include "boost/tuple/tuple.hpp"

#include "boost/mpl/if.hpp"

// This is a named template parameter implementation modified after the 
// solution in Nicolas Josuttis' and Daveed Vandevoorde's book.

namespace boost {

  namespace ntp {

    class named_parameter_tag {};

    class unspecified {};

    namespace detail {
        
      // anything that derives from named_parameter_tag is considered
      // as named parameter    
      template <class T> struct is_named_parameter {
	BOOST_STATIC_CONSTANT( bool, value = 
	  (boost::is_base_and_derived<named_parameter_tag, T>::value));
      };

    } // namespace detail 

    template<class T, template <class U> class Named>
    struct as_named {
      typedef typename boost::mpl::if_c<
        detail::is_named_parameter<T>::value,
	T, 
        Named<T> 
      >::type type;
    };

    namespace detail {

      template <class Base, int D>
      class discriminator : virtual public Base {};
    
      template <class Feature_list, int N>
      class feature_selector : 
	public discriminator<typename Feature_list::head_type, N>,
	public feature_selector<typename Feature_list::tail_type, N+1>
      {
	BOOST_STATIC_ASSERT(detail::is_named_parameter<
			      typename Feature_list::head_type
			    >::value);
      };
      
      template <int N> class feature_selector<boost::tuples::null_type, N> {};

    } // namespace detail 
    
  
    template <class Feature_list>
    class named_parameter_extractor
      : public detail::feature_selector<Feature_list, 1> {};

  } // namespace ntp

} // namespace boost

#endif





















