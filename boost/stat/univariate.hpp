// (C) Copyright Thorsten Ottosen 2004. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_STAT_UNIVARIATE_HPP
#define BOOST_STAT_UNIVARIATE_HPP

#include <boost/stat/sum.hpp>
#include <boost/stat/stat_exception.hpp>
#include <boost/assert.hpp>
#include <algorithm>
#include <cmath>
#include <iterator>

namespace boost
{
    namespace algorithm
    {
        template< class InputIterator >
        inline typename std::iterator_traits<InputIterator>::value_type 
        mode( InputIterator first, InputIterator last )  
        {
            // aka most frequent number
            // use a map<value_type, size_t> ?
        }
        
        
        
        /**
         * Complexity = O(1)
         */ 
        template< class T, class RandomAccessIterator >
        inline T
        sorted_median( RandomAccessIterator first, RandomAccessIterator last )
        {
            BOOST_ASSERT( std::distance( first, last ) > 0 );
            // throw exception instead
            const size_t half = ( last - first ) / 2;
            if( half % 2 == 0 && std::distance( first, last ) != 1 )
                return T( *first[half-1] + *first[half] ) / 2;
            return *first[half];
        }
        
        
        
        /**
         * Comlexity = O(nlog(n))
         */ 
        template< class T, class RandomAccessIterator >
        inline typename std::iterator_traits<RandomAccessIterator>::value_type 
        unsorted_meadian( RandomAccessIterator first, RandomAccessIterator last )      
        {
            std::sort( first, last );
            return sorted_median( first, last );
        }
        
        
        
        /**
         * Unbiased sample mean.
         * Complexity = (approx) O(4n)
         */
        template< class T, class InputIterator >
        inline T mean( InputIterator first, InputIterator last )      
        {
            stat_detail::x_sum<T> s( first, last );        
            if( s.n == 0 )
                throw stat_exception( stat_constants::empty_range );

            return stat_detail::mean_x<T>( s );
        }
        
        
        /**
         * Unbiased sample variance.
         * Complexity = (approx) O(6n)
         */
        template< class T, class InputIterator >
        inline T variance( InputIterator first, InputIterator last )
        {
            stat_detail::x_xx_sum<T> s( first, last );
            if( s.n == 0 )
                throw stat_exception( stat_constants::empty_range );
            
            return stat_detail::variance_x<T>( s );
        }
            
            
        
        /**
         * Complexity = (approx) O(6n)
         */  
        template< class T, class InputIterator >
        inline T std_deviation( InputIterator first, InputIterator last )
        {
            return std::sqrt( variance<T>( first, last ) );
        }
        
        
        
        struct identity
        {
            template< class T >
            T operator()( T t )
            {
                return t;
            }
        };
        
        template< class T, class InputIterator, 
                  class UnaryFunction1, class UnaryFunction2 >
        inline T 
        expected_value( InputIterator first, InputIterator last, 
                        UnaryFunction1 probability_function,
                        UnaryFunction2 random_variable_function = identity() )
        {
            T result = 0;
            for( ; first != last; ++first )
                result += random_variable_function( *first ) * 
                          probability_function( *first );
            return result;
        }
        
        //
        // skewness: mu_3 / sigma^3
        //
        
        //
        // kurtosis: mu_4 / sigma^4
        //
        
    } // namespace 'algorithm'
    
} // namespace 'boost'


#endif



