//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_WRAPPER_UNWRAP_ITERATOR_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_WRAPPER_UNWRAP_ITERATOR_ER_2010_HPP
#include <boost/range/reference.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/ref.hpp> // is_reference_wrapper<>
#include <boost/assign/v2/ref/detail/unwrap/iterator.hpp>

// Calling
// 	*unwrap_iterator( it );
// is equivalent to
//	(*it).unwrap(); 
//  if *it is a reference wrapper, otherwise
//  (*it);
// Note this is necessary for chaining, at least.

namespace boost{
	struct use_default;
namespace assign{ 
namespace v2{
namespace ref{
namespace unwrap_aux{

	template<typename I,
        typename R = typename boost::iterator_reference<I>::type,
    	typename V = typename boost::remove_reference<R>::type
	> struct iterator;

    template<
    	typename I, typename R, typename V,
        bool is_rw = boost::is_reference_wrapper<V>::value
    >
    struct iterator_traits{
		
        typedef R reference;
        typedef V value_type;
    
    	typedef unwrap_aux::iterator<I, R, V> derived_;
        typedef boost::iterator_adaptor<
        	derived_		// Derived
        	,I              // Base
        	,value_type     // Value
        	,use_default    // CategoryOrTraversal
        	,reference		// Reference
        	,use_default    // Difference
    	> adaptor_;
    
    };

	template<typename I, typename R, typename W>
    struct iterator_traits<I, R, W, true> : unwrap_aux::iterator_traits<
    	I,
		typename boost::add_reference<
        	typename boost::unwrap_reference<W>::type
        >::type,
        W,
        false
    >
    {
    	typedef typename boost::unwrap_reference<W>::type inner_value_type;
        // If inner_value_type == T[], and value_type were set to that type,
        // compiler complains that iterator_facade's operator[] would copy T[]
        // So, instead, value_type = W, such that the returned value acts a bit
        // like operator_brackets_proxy<I>.
		//    
    	// In contrast, the solution adapted in Boost.Assign v1 in this kind
        // of situation is to use boost::decay<inner_value_type>::type.
    
    };

    template<typename I, typename R, typename V>
    struct iterator : unwrap_aux::iterator_traits<I, R, V>::adaptor_
    {
 		typedef typename unwrap_aux::iterator_traits<I, R, V>::adaptor_ super_t;
 
        iterator(){}
        iterator(I base) : super_t(base)
        {
            typedef typename boost::iterator_reference<I>::type ref_;
            typedef typename 
            	boost::is_convertible<ref_, R>::type is_convertible_;
            BOOST_MPL_ASSERT(( is_convertible_ ));
        }
    };
              
    template<typename It>
    struct is_unwrapped_iterator : boost::mpl::false_{};
    
	template<typename I, typename R, typename V>
    struct is_unwrapped_iterator< unwrap_aux::iterator<I, R, V> >
        : boost::mpl::true_{};

}// unwrap_aux
namespace result_of{

    template<typename I>
    struct unwrap_iterator
    {
        typedef  unwrap_aux::iterator<I> type;
    };

}// result_of

    template<typename I>
    typename result_of::unwrap_iterator<I>::type
    unwrap_iterator(const I& base)
    {
        typedef typename result_of::unwrap_iterator<I>::type result_;
        return result_( base );
    }

}// ref
}// v2
}// assign
}// boost

#endif

