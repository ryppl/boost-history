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

#ifndef BOOST_ASSIGN_UBLAS_VECTOR_HPP
#define BOOST_ASSIGN_UBLAS_VECTOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
    #pragma once
#endif

#include <boost/assign/fixed_size_assigner.hpp>
#include <boost/numeric/ublas/vector.hpp>

namespace boost
{
namespace assignment
{
    namespace bnu = boost::numeric::ublas;

    //
    // @note: the extra template parameter is necessary
    //        to help template matching when eg.
    //        'V=double' and 'V2=int'.
    //

    template< typename V, typename A, typename V2 > 
    inline fixed_size_assigner<V, typename A::iterator> 
    operator<<( bnu::vector<V,A>& c, const V2& v )
    {
        A& a = c.data();
        return fixed_size_assigner<V, typename A::iterator>
        ( a.begin(), a.end(), v );
    }

}
}

#endif
