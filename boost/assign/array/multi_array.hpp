/**
 * Non-intrusive Assignment Library
 * (C) Thorsten Ottosen 2002-2003
 * 
 * Permission to copy, use, modify,
 * sell and distribute this software is granted provided this
 * copyright notice appears in all copies. This software is provided
 * "as is" without express or implied warranty, and with no claim as
 * to its suitability for any purpose.
 *
 * See http://www.boost.org/libs/assign for documentation.
 *
 */

#ifndef BOOST_ASSIGN_ARRAY_MULTI_ARRAY_HPP
#define BOOST_ASSIGN_ARRAY_MULTI_ARRAY_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/assign/fixed_size_assigner.hpp>
//#include <boost/multi_array.hpp>
//#include <boost/multi_array_ref.hpp>

// namespace init
// {
//     template< typename V, std::size_t D, typename A, typename V2 > 
//     inline detail::fixed_size_assigner<V, typename boost::
// 				       multi_array<V,D,A>::iterator>
//     operator<<( boost::multi_array<V,D,A>& c, const V2& v )
//     {
// 	return detail::fixed_size_assigner<V, typename boost::
// 	    multi_array<V,D,A>::iterator>( c.begin(), c.end(), v );
//     }



//     template< typename V, std::size_t D, typename V2 > 
//     inline detail::fixed_size_assigner<V, typename boost::
// 				       multi_array_ref<V,D>::iterator>
//     operator<<( boost::multi_array_ref<V,D>& c, const V2& v )
//     {
// 	return detail::fixed_size_assigner<V, typename boost::
// 	    multi_array_ref<V,D>::iterator>( c.begin(), c.end(), v );
//     }

// } // namespace 'init'

#endif
