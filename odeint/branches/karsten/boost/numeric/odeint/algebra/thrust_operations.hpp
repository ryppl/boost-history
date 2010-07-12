/*
 boost header: BOOST_NUMERIC_ODEINT/thrust_operations.hpp

 Copyright 2009 Karsten Ahnert
 Copyright 2009 Mario Mulansky

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef BOOST_BOOST_NUMERIC_ODEINT_THRUST_OPERATIONS_HPP_INCLUDED
#define BOOST_BOOST_NUMERIC_ODEINT_THRUST_OPERATIONS_HPP_INCLUDED

namespace boost {
namespace numeric {
namespace odeint {

#include <thrust/iterator/zip_iterator.h>


template< class Time >
struct thrust_operations
{
	typedef Time time_type;

	struct increment
	{
		time_type m_dt;

		increment( time_type dt ) : m_dt( dt ) { }

		template< class Tuple >
		__host__ __device__
		void operator()( Tuple t ) const
		{
			thrust::get<0>(t) += m_dt * thrust::get<1>(t);
		}
	};
};

} // odeint
} // numeric
} // boost


#endif //BOOST_BOOST_NUMERIC_ODEINT_THRUST_OPERATIONS_HPP_INCLUDED
