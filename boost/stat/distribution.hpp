// (C) Copyright Thorsten Ottosen 2004. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_STAT_DISTRIBUTION_HPP
#define BOOST_STAT_DISTRIBUTION_HPP

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
        template< class T >
        inline T factorial( T n )
        {
            BOOST_ASSERT( n >= 0 );
            /**
             * if n < small
             *   use while-loop
             * else
             *   stirling's formula
             */ 
            if( n == 0 )
                return 1;
            
            T result = n;
            for( --n; n != 0; --n )
            {
                BOOST_ASSERT( n > 0 );
                BOOST_ASSERT( result > 0 && "integer has wrapped around" );
                result *= n;
            }
            
            BOOST_ASSERT( result > 0 && "integer has wrapped around" );
            return result;
        }
        
        
        
        template< class T >
        inline T nPr( T n, T r )
        {
            BOOST_ASSERT( n >= 0 );
            BOOST_ASSERT( r >= 0 );

            const T old_n = n, old_r = r;
            
            if( r > n )
                return 0;
            
            if( n == r )
                return factorial( n );
            
            if( n == 1 )
                return 1;
            
            T result = n;
            --r;
            --n;
                    
            for( ; r != 0; --r, --n )
                result *= n;
            
            BOOST_ASSERT( result == 
                          factorial( old_n ) / factorial( old_n - old_r ) );
            return result; 
        }
        

        
        template< class T >
        inline T nCr( T n, T r )
        {
            BOOST_ASSERT( n >= 0 );
            BOOST_ASSERT( r >= 0 );

            if( r > n )
                return 0;
            
            if( n == r )
                return 1;
            
            T res = nPr( n, r ) / factorial( r ); 
            BOOST_ASSERT( res == nPr( n, n - r ) / factorial( n - r ) );
            return res;
        }
        
        
        
        /**
         * Remark: can these be generalized to support
         * boost::rational<> for exact computation?
         */ 
        template< class T, class Integer >
        inline T binomial( T p, Integer n, Integer r )
        {
            BOOST_ASSERT( n >= 0 );
            BOOST_ASSERT( r >= 0 );
            BOOST_ASSERT( n >= r );
            BOOST_ASSERT( p >= 0 && p <= 1 );
            if( n == 0 )
                return 0;
            
            return nCr( n, r ) * pow( p, r ) * pow( T( 1 ) - p, n - r );  
        }
        
        
        
        template< class T, class Integer >
        inline T negative_binomial( T p, Integer n, Integer r )
        {
            // static_assert( is_float<T>::value );
            BOOST_ASSERT( 0 <= p && p <= 1 );
            BOOST_ASSERT( n >= r );
            BOOST_ASSERT( n > 1 );
            return nCr( n - 1, r - 1 ) * pow( p, r ) * pow( 1 - p, n - r );
        }
        
        
        /*
        template< class T, class Integer >
        inline T multinomial( const vector<T>& p, const vector<Integer>& n )
        {
            BOOST_ASSERT( size( x ) == size( y ) ); 
            math_traits<X>::iterator i = begin( x );
            math_traits<X>::iterator e = end( x );

        }
        */
        
        
        template< class T, class Integer >
        inline T poisson( T lambda, Integer r )
        {
            return pow( lambda, r ) * exp( -lambda ) / factorial( r ); 
        }
        
        
        
        template< class T, class Integer >
        inline T hypergeometric( Integer n, Integer r, 
                                 Integer blue, Integer red )
        {
            BOOST_ASSERT( n >= 0 );
            BOOST_ASSERT( r >= 0 );
            BOOST_ASSERT( n >= r );
            BOOST_ASSERT( blue >= 0 );
            BOOST_ASSERT( red >= 0 );
            const Integer N = blue + red;
            BOOST_ASSERT( n <= N );
            // static_assert( is_float<T>::value );
            
            return T( nCr( blue, r ) * nCr( red, n - r ) ) / nCr( N, n );
        }
        
        
        template< class T, class Integer >
        inline T geometric( T p, Integer r )
        {
            BOOST_ASSERT( r > 0 );
            BOOST_ASSERT( p >= 0 && p <= 1 );
            // static_assert( is_float<T>::value );
            return p * pow( 1 - p, r - 1 ); 
        }

        
        
    } // namespace 'algorithm'
    
} // namespace 'boost'


#endif



