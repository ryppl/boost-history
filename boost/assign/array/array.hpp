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

#ifndef BOOST_ASSIGN_ARRAY_ARRAY_HPP
#define BOOST_ASSIGN_ARRAY_ARRAY_HPP

#include "../fixed_size_assigner.hpp"
#include <boost/array.hpp>

namespace boost
{
    namespace assignment
    {
    	template< typename V, std::size_t N, typename V2 >
    	inline detail::fixed_size_assigner<V, V*>
    	operator<<( boost::array<V,N>& c, const V2& v )
    	{
    	    return detail::fixed_size_assigner<V,V*>( c.begin(), c.end(), v );
    	}
    }

}

#endif
