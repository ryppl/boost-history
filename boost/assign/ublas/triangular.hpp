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

#ifndef BOOST_ASSIGN_UBLAS_TRIANGULAR_HPP
#define BOOST_ASSIGN_UBLAS_TRIANGULAR_HPP

#include "../fixed_size_assigner.hpp"
#include <boost/numeric/ublas/triangular.hpp>

namespace boost
{
    namespace assignment
    {
    	namespace bnu = boost::numeric::ublas;
    	
    	template< typename V, typename F1, typename F2, 
    		  typename A, typename V2 >
    	inline fixed_size_assigner<V, typename A::iterator>
    	operator<<( bnu::triangular_matrix<V,F1,F2,A>& c, const V2& v )
    	{
    	    A& a = c.data();
    	    return fixed_size_assigner<V, typename A::iterator>
    		( a.begin(), a.end(), v );
    	}
    }
}

#endif
