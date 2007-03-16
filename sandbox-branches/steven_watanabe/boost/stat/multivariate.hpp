// (C) Copyright Thorsten Ottosen 2004. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_STAT_MULTIVARIATE_HPP
#define BOOST_STAT_MULTIVARIATE_HPP

#include <boost/stat/general_sum.hpp>
#include <boost/stat/stat_exception.hpp>
#include <boost/assert.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <cmath>
#include <iterator>
#include <numeric>

namespace boost
{
    namespace algorithm
    { 
        template< class ColumnVector, class Matrix >
        inline ColumnVector
        row_mean( const Matrix& samples )
        {
            ColumnVector res;
            stat_detail::row_sum( samples, res );
            return res;
        }

        template< class RowVector, class Matrix >
        inline RowVector
        column_mean( const Matrix& samples )
        {
            RowVector res;
            stat_detail::column_sum( samples, res );
            return res;
        }
                
        template< class ColumnVector, class Matrix >
        inline ColumnVector 
        row_variance( const Matrix& );

        template< class RowVector, class Matrix >
        inline RowVector 
        column_variance( const Matrix& );

        template< class Matrix >
        inline Matrix 
        row_covariance( const Matrix& );
        
        template< class Matrix >
        inline Matrix 
        column_covariance( const Matrix& );

        template< class Matrix >
        inline Matrix 
        row_correlation( const Matrix& );

        template< class Matrix >
        inline Matrix 
        column_correlation( const Matrix& );

        template< class Matrix >
        inline Matrix principal_components( const Matrix& );
        // eigen_vectors ?
        
        template< class Vector, class Matrix >
        inline Vector eigen_values( const Matrix& );
        
        template< class PrincipalComponent, class Matrix >
        inline Matrix principal_correlation( const PrincipalComponent, const Matrix& );
        

    } // namespace 'algorithm'
    
} // namespace 'boost'

#endif
