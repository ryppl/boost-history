/*
 boost header: BOOST_NUMERIC_ODEINT/thrust_algebra.hpp

 Copyright 2009 Karsten Ahnert
 Copyright 2009 Mario Mulansky

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef BOOST_BOOST_NUMERIC_ODEINT_THRUST_ALGEBRA_HPP_INCLUDED
#define BOOST_BOOST_NUMERIC_ODEINT_THRUST_ALGEBRA_HPP_INCLUDED

#include <thrust/device_vector.h>
#include <thrust/for_each.h>
#include <thrust/iterator/zip_iterator.h>

namespace boost {
namespace numeric {
namespace odeint {


struct thrust_algebra
{
	template< class StateType1 , class StateType2 , class Operation >
	static void for_each2( StateType1 &s1 , StateType2 &s2 , Operation op )
	{
		thrust::for_each( thrust::make_zip_iterator( thrust::make_tuple( boost::begin(s1) , boost::begin(s2) ) ) ,
						  thrust::make_zip_iterator( thrust::make_tuple( boost::end(s1) , boost::end(s2) ) ) ,
						  op);
	}
};


} // odeint
} // numeric
} // boost


#endif //BOOST_BOOST_NUMERIC_ODEINT_THRUST_ALGEBRA_HPP_INCLUDED
