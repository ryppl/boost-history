// (C) Copyright Thorsten Ottosen 2004. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_STAT_BIVARIATE_HPP
#define BOOST_STAT_BIVARIATE_HPP

#include <boost/stat/sum.hpp>
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
        /**
         * Complexity = (approx) O(6n)
         */
        template< class T, class InputIterator, class InputIterator2 >
        inline tuple<T,T> mean( InputIterator first, InputIterator last, 
                                InputIterator2 first2, InputIterator2 last2 )
        {
            stat_detail::x_y_sum<T> s( first, last, first2, last2 );
                        
            if( s.n == 0 )
                throw stat_exception( stat_constants::empty_range );

            return make_tuple( stat_detail::mean_x<T>( s ), 
                               stat_detail::mean_y<T>( s ) );
        }


        
        /**
         * Complexity = (approx) O(9n)
         */
        template< class T, class InputIterator, class InputIterator2 >
        inline tuple<T,T> variance( InputIterator first, InputIterator last, 
                                    InputIterator2 first2, InputIterator2 last2 )
        {
            stat_detail::x_y_xx_yy_sum<T> s( first, last, first2, last2 );
                        
            if( s.n == 0 )
                throw stat_exception( stat_constants::empty_range );

            return make_tuple( stat_detail::variance_x<T>( s ), 
                               stat_detail::variance_y<T>( s ) );
        }
        
        
        
        /**
         * Complexity = (approx) O(9n)
         */
        template< class T, class InputIterator, class InputIterator2 >
        inline tuple<T,T> 
        std_deviation( InputIterator first, InputIterator last, 
                       InputIterator2 first2, InputIterator2 last2 )
        {
            tuple<T,T> var = variance<T>( first, last, first2, last2 );
            return make_tuple( std::sqrt( get<0>( var ) ), std::sqrt( get<1>( var ) ) );
        }
        

        
        /*
         * Complexity = (approx) O(8n)
         */
        template< class T, class InputIterator, class InputIterator2 >
        inline T covariance( InputIterator first, InputIterator last, 
                             InputIterator2 first2, InputIterator2 last2 )
        {
            // assert( std::distance( first, last ) 
            //         == std::distance( first2, last2 ) );
            
            stat_detail::x_y_xy_sum<T> s( first, last, first2, last2 );
            
            if( s.n == 0 )
                throw stat_exception( stat_constants::empty_range );
            
            return stat_detail::covariance<T>( s );
        }
        
        
        /**
         * Complexity = (approx) O(12n)
         */ 
        template< class T, class InputIterator, class InputIterator2 >
        inline T 
        correlation_coefficient( InputIterator first, InputIterator last, 
                                 InputIterator2 first2, InputIterator2 last2 )
        {
            // assert( std::distance( first, last ) 
            //         == std::distance( first2, last2 ) );
            
            stat_detail::x_y_xx_yy_xy_sum<T> s( first, last, first2, last2 );
            
            if( s.n == 0 )
                throw stat_exception( stat_constants::empty_range );
            
            return stat_detail::correlation_coefficient<T>( s );  
        }
                    
        
        
        /**
         * Complexity = (approx) O(12n)
         */
        template< class T, class InputIterator, class InputIterator2, class UnaryFunction >
        inline T 
        correlation_coefficient( InputIterator first, InputIterator last, 
                                 UnaryFunction transformation,
                                 InputIterator2 first2, InputIterator2 last2 )
        {
            return correlation_coefficient( make_transform_iterator( first, transformation ),
                                            make_transform_iterator( last, transformation ),
                                            first2, last2 ); 
        }
         
        
        
        /**
         * Complexity = (approx) O(12n)
         */
        template< class T, class InputIterator, class InputIterator2, 
                  class UnaryFunction, class UnaryFunction2 >
        inline T 
        correlation_coefficient( InputIterator first, InputIterator last, 
                                 UnaryFunction transformation,
                                 InputIterator2 first2, InputIterator2 last2,
                                 UnaryFunction2 transformation2 )
        {
            return correlation_coefficient( make_transform_iterator( first, transformation ),
                                            make_transform_iterator( last, transformation ),
                                            make_transform_iterator( first2, transformation2 ),
                                            make_transform_iterator( last2, transformation2 ) ); 
        }

    } // namespace 'algorithm'
    
} // namespace 'boost'

#endif
