// (C) Copyright Thorsten Ottosen 2004. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_STAT_REGRESSION_HPP
#define BOOST_STAT_REGRESSION_HPP

#include <boost/stat/sum.hpp>
#include <boost/stat/stat_exception.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/assert.hpp>
#include <boost/tuple/tuple.hpp>

namespace boost
{
    namespace algorithm
    {
        /**
         * Complexity = (approx) O(12n)
         */ 
        template< class T, class InputIterator, class InputIterator2 >
        inline tuple<T,T,T> 
        least_square_line( InputIterator first, InputIterator last, 
                           InputIterator2 first2, InputIterator2 last2 )
        {
            // BOOST_ASSERT( std::distance( first, last ) == 
            //               std::distance( first2,last2 ) );
            stat_detail::x_y_xx_yy_xy_sum<T> s( first, last, first2, last2 );
            if( s.n == 0 )
                throw stat_exception( stat_constants::empty_range );
            
            T b = stat_detail::least_square_slope<T>( s );
            T a = stat_detail::least_square_intersection<T>( s, b );
            T c = stat_detail::correlation_coefficient<T>( s );
            return make_tuple( b, a, c );
        }
        
        
        
        /**
         * Complexity = (approx) O(12n)
         */ 
        template< class T, class InputIterator, 
                  class InputIterator2, class UnaryFunction >
        inline tuple<T,T,T> 
        least_square_line( InputIterator first, InputIterator last, 
                           UnaryFunction transformation, 
                           InputIterator2 first2, InputIterator2 last2 )
        {
            // BOOST_ASSERT( std::distance( first, last ) == 
            //               std::distance( first2,last2 ) );
            return least_square_line<T>( make_transform_iterator( first, transformation ),
                                         make_transform_iterator( last, transformation ),
                                         first2, last2 );
        }

        
        
        /**
         * Complexity = (approx) O(12n)
         */ 
        template< class T, class InputIterator, class InputIterator2, 
                  class UnaryFunction, class UnaryFunction2 >
        inline tuple<T,T,T> 
        least_square_line( InputIterator first, InputIterator last, 
                           UnaryFunction transformation, 
                           InputIterator2 first2, InputIterator2 last2,
                           UnaryFunction2 transformation2 )
        {
            // BOOST_ASSERT( std::distance( first, last ) == 
            //               std::distance( first2,last2 ) );
            return least_square_line<T>( make_transform_iterator( first, transformation ),
                                         make_transform_iterator( last, transformation ),
                                         make_transform_iterator( first2, transformation2 ),
                                         make_transform_iterator( last2, transformation2 ) );

        }

        
        
        /**
         * Complexity = (approx) O(10n)
         */ 
        template< class T, class InputIterator, class InputIterator2 >
        inline tuple<T,T> 
        least_square_line_without_correlation( InputIterator first, InputIterator last, 
                                               InputIterator2 first2, InputIterator2 last2 )
        {
            // BOOST_ASSERT( std::distance( first, last ) == 
            //               std::distance( first2,last2 ) );
            stat_detail::x_y_xx_yy_xy_sum<T> s( first, last, first2, last2 );
            if( s.n == 0 )
                throw stat_exception( stat_constants::empty_range );
            
            T b = stat_detail::least_square_slope<T>( s );
            T a = stat_detail::least_square_intersection<T>( s, b );
            return make_tuple( b, a );
        }
        
        
        
        /**
         * Complexity = (approx) O(10n)
         */ 
        template< class T, class InputIterator, 
                  class InputIterator2, class UnaryFunction >
        inline tuple<T,T> 
        least_square_line_without_correlation( InputIterator first, InputIterator last, 
                                               UnaryFunction transformation, 
                                               InputIterator2 first2, InputIterator2 last2 )
        {
            // BOOST_ASSERT( std::distance( first, last ) == 
            //               std::distance( first2,last2 ) );
            return least_square_line_without_correlation<T>( make_transform_iterator( first, transformation ),
                                                             make_transform_iterator( last, transformation ),
                                                             first2, last2 );
        }

        
        
        /**
         * Complexity = (approx) O(10n)
         */ 
        template< class T, class InputIterator, class InputIterator2, 
                  class UnaryFunction, class UnaryFunction2 >
        inline tuple<T,T> 
        least_square_line_without_correlation( InputIterator first, InputIterator last, 
                                               UnaryFunction transformation, 
                                               InputIterator2 first2, InputIterator2 last2,
                                               UnaryFunction2 transformation2 )
        {
            // BOOST_ASSERT( std::distance( first, last ) == 
            //               std::distance( first2,last2 ) );
            return least_square_line_without_correlation<T>( make_transform_iterator( first, transformation ),
                                                             make_transform_iterator( last, transformation ),
                                                             make_transform_iterator( first2, transformation2 ),
                                                             make_transform_iterator( last2, transformation2 ) );

        }
                
        //
        // remark 1: solving least-square parabola and above gets
        //           really complicated. Such a fit is probably best
        //           defined generally for all n and solved numerically
        //           using matrixes.
        // remark 2: multivariate linear regression is also best done using
        //           matrix algebra. 
        //
        // remark 3: transform versions might not be neccessary if the new iterator adaptors
        //           would include make_transform_collection( col, trans ) etc for each iterator
        //           version. Then the effective use would be least_square_line( make_transform_collection( v, &log10 ), v2 ); 
        //
        //
            
    } // namespace 'algorithm'
    
} // namespace 'boost'

#endif
