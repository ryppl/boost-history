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

#ifndef BOOST_ASSIGN_UBLAS_MATRIX_HPP
#define BOOST_ASSIGN_UBLAS_MATRIX_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
    #pragma once
#endif

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/assign/fixed_size_assigner.hpp>
#include <boost/assign/make_tuple_insertion.hpp>
#include <boost/assign/fixed_size_tuple_assigner.hpp>

namespace boost
{
namespace assignment
{
    namespace bnu = boost::numeric::ublas;
    
    template< typename V, typename F, typename A, typename V2 > 
    inline fixed_size_assigner<V, typename A::iterator>
    operator<<( bnu::matrix<V,F,A>& c, const V2& v )
    {
        A& a = c.data();
        return fixed_size_assigner<V, typename A::iterator>
            ( a.begin(), a.end(), v );
    }

    
    
    template< typename V, typename F, typename A > 
    inline fixed_size_tuple_assigner<typename A::iterator>
    assign_all( bnu::matrix<V,F,A>& c )
    {
        A& a = c.data();
        return fixed_size_tuple_assigner<typename A::iterator>
            ( a.begin(), a.end() );
    }

}
}

#endif
