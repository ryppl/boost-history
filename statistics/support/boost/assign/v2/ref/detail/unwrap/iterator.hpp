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
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/assign/v2/ref/detail/unwrap/reference.hpp>

// Calling
// 	*unwrap_iterator( it );
// is equivalent to
//	(*it).unwrap(); 
//  if *it is a reference wrapper, otherwise
//  (*it);

namespace boost{
namespace assign{ 
namespace v2{
namespace ref{
namespace unwrap_aux{

    template<typename I>
    struct reference_of_iterator : unwrap_aux::reference<
        typename boost::iterator_reference<I>::type
    >{};

    template<
        typename I, 
        typename R = typename unwrap_aux::reference_of_iterator<I>::type,
        typename V = typename boost::remove_reference<R>::type
    >
    struct iterator : boost::iterator_adaptor<
        iterator<I,R,V>           // Derived
        ,I                        // Base
        ,V                        // Value
        ,use_default              // CategoryOrTraversal
        ,R                        // Reference
        ,use_default              // Difference
    >{
        typedef boost::iterator_adaptor<
        	iterator<I,R,V>, 
            I, 
            V,
            use_default, 
            R, 
            use_default 
        > super_;

        iterator(){}
        iterator(I base):super_(base)
        {
            typedef typename boost::iterator_reference<I>::type ref_;
            typedef typename 
            	boost::is_convertible<ref_,R>::type is_convertible_;
            BOOST_MPL_ASSERT(( is_convertible_ ));
        }
    };
              
    template<typename It>
    struct is_iterator : boost::mpl::false_{};
    
	template<typename I, typename R, typename V>
    struct is_iterator< unwrap_aux::iterator<I,R,V> >
        : boost::mpl::true_{};

}// unwrap_aux

	template<typename It>
    struct is_unwrapped_iterator : unwrap_aux::is_iterator<
       typename boost::remove_cv<It>::type
    >{};

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

