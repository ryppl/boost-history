// (C) Copyright Thorsten Ottosen 2004. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_STAT_GENERAL_SUM_HPP
#define BOOST_STAT_GENERAL_SUM_HPP

#include <boost/stat/matrix_traits.hpp>
#include <boost/stat/univariate.hpp>
#include <boost/stat/stat_exception.hpp>
#include <boost/assert.hpp>
#include <boost/config.hpp>
#include <boost/type_traits.hpp>
#include <numeric>

#define BOOST_DEFINE_IS_RANDOM_ACCESS() \
const bool is_random_access = boost::is_same<typename std::iterator_traits< \
InputIterator>::iterator_category,std::random_access_iterator_tag>::value; \
if( is_random_access ) n = last - first; else n = 0
#define BOOST_DEFINE_INCREMENT_N() \
if( !is_random_access ) ++n

namespace boost
{
    namespace stat_detail
    {
        template< class Matrix, class Vector >
        inline void row_sum( const Matrix& m, Vector& vec )
        {
            typedef typename size_type_of<Matrix>::type  size_type;
            typedef typename value_type_of<Vector>::type value_type;
            
            const size_type rows = matrix_traits::rows( m );
            
            if( matrix_traits::rows( vec ) != rows )
                throw stat_exception( stat_constants::mismatch_range );
            
            for( size_type i = 0; i < rows; ++i )
                matrix_traits::row_value( vec, i ) = std::accumulate( matrix_traits::begin_row_n( m, i ), 
                                                                      matrix_traits::end_row_n( m, i ), value_type( 0 ) );
        }
            
        
    
        template< class Matrix, class Vector >
        inline void column_sum( const Matrix& m, Vector& vec )
        {
            typedef typename size_type_of<Matrix>::type size_type;
            typedef typename value_type_of<Vector>::type value_type;
            
            const size_type cols = matrix_traits::columns( m );
    
            if( matrix_traits::rows( vec ) != cols )
                throw stat_exception( stat_constants::mismatch_range );
    
            for( size_type i = 0; i < cols; ++i )
                matrix_traits::row_value( vec, i ) = std::accumulate( matrix_traits::begin_column_n( m, i ), 
                                                                      matrix_traits::end_column_n( m, i ), value_type( 0 ) );
        }



        // remark: how did MTL factor out row-major/column major?

    } // namespace 'stat_detail'
    
} // namespace 'boost'


#undef BOOST_DEFINE_IS_RANDOM_ACCESS
#undef BOOST_DEFINE_INCREMENT_N

#endif



