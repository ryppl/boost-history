// (C) Copyright Thorsten Ottosen 2004. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_STAT_SUM_HPP
#define BOOST_STAT_SUM_HPP

#include <boost/assert.hpp>
#include <boost/config.hpp>
#include <boost/type_traits.hpp>
#include <cmath>
#include <iterator>

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
        // complexity = 3n
        template< class T >
        class x_sum
        {
            x_sum();
        public:
            T       sum_x;
            size_t  n;

            template< class InputIterator >
            x_sum( InputIterator first, InputIterator last )
            {
                BOOST_DEFINE_IS_RANDOM_ACCESS();
                sum_x = 0;
                
                for( ; first != last; ++first )
                {
                    sum_x += *first;
                    BOOST_DEFINE_INCREMENT_N();
                }
                
                BOOST_ASSERT( n > 0 );               
            }
        };
        
        
        // complexity = 5n
        template< class T >
        class xx_sum
        {
            xx_sum();
            
        public:
            typedef T value_type;
            
            T      sum_xx;
            size_t n;
            
            template< class InputIterator >
            xx_sum( InputIterator first, InputIterator last )
            {
                BOOST_DEFINE_IS_RANDOM_ACCESS();
                sum_xx = 0;
                
                for( ; first != last; ++first )
                {
                    T _1    = *first;
                    sum_xx += _1 * _1;
                    BOOST_DEFINE_INCREMENT_N();
                }
                BOOST_ASSERT( n > 0 );
                BOOST_ASSERT( sum_xx >= 0 );
            }
        };

        
        // complexity = 6n
        template< class T >
        class x_y_sum
        {
            x_y_sum();
            
        public:
            typedef T value_type;
            
            T      sum_x;
            T      sum_y;
            size_t n;
                        
            template< class InputIterator, class InputIterator2 >
            x_y_sum( InputIterator first, InputIterator last,
                     InputIterator2 first2, InputIterator2 last2 )
            {
                BOOST_DEFINE_IS_RANDOM_ACCESS();
                sum_x  = 0;
                sum_y  = 0;
                
                for( ; first != last; ++first, ++first2 )
                {
                    T _1    = *first;
                    T _2    = *first2;
                    sum_x  += _1;
                    sum_y  += _2;
                    BOOST_DEFINE_INCREMENT_N();
                }
                BOOST_ASSERT( n > 0 );
             }
        };

                
        // complexity = 6n        
        template< class T >
        class x_xx_sum
        {
            x_xx_sum();
            
        public:
            typedef T value_type;
            
            T      sum_x;
            T      sum_xx;
            size_t n;
            
            template< class InputIterator >
            x_xx_sum( InputIterator first, InputIterator last )
            {
                BOOST_DEFINE_IS_RANDOM_ACCESS();
                sum_x  = 0;
                sum_xx = 0;
                
                for( ; first != last; ++first )
                {
                    T _1    = *first;
                    sum_x  += _1;
                    sum_xx += _1 * _1;
                    BOOST_DEFINE_INCREMENT_N();
                }
                BOOST_ASSERT( n > 0 );
                BOOST_ASSERT( sum_xx >= 0 );
             }  
        };

        
        // complexity = 8n
        template< class T >
        class x_y_xy_sum
        {
            x_y_xy_sum();
        
        public:
            typedef T value_type;
            
            T      sum_x;
            T      sum_y;
            T      sum_xy;
            size_t n;
        
            template< class InputIterator, class InputIterator2 >
            x_y_xy_sum( InputIterator first, InputIterator last,
                        InputIterator2 first2, InputIterator2 last2 )
            {
                BOOST_DEFINE_IS_RANDOM_ACCESS();
                sum_x  = 0;
                sum_y  = 0;
                sum_xy = 0;
        
                for( ; first != last; ++first, ++first2 )
                {
                    T _1    = *first;
                    T _2    = *first2;
                    sum_x  += _1;
                    sum_y  += _2; 
                    sum_xy += _1 * _2;
                    BOOST_DEFINE_INCREMENT_N();
                }
                BOOST_ASSERT( n > 0 );                
             }  
        };
       
        
        // complexity = 10n
        template< class T >
        class x_y_xx_yy_sum
        {
            x_y_xx_yy_sum();
        
        public:
            typedef T value_type;
            
            T      sum_x;
            T      sum_y;
            T      sum_xx;
            T      sum_yy;
            size_t n;
        
            template< class InputIterator, class InputIterator2 >
            x_y_xx_yy_sum( InputIterator first, InputIterator last,
                           InputIterator2 first2, InputIterator2 last2 )
            {
                BOOST_DEFINE_IS_RANDOM_ACCESS();
                sum_x  = 0;
                sum_y  = 0;
                sum_xx = 0;
                sum_yy = 0;
        
                for( ; first != last; ++first, ++first2 )
                {
                    T _1    = *first;
                    T _2    = *first2;
                    sum_x  += _1;
                    sum_y  += _2;
                    sum_xx += _1 * _1; 
                    sum_yy += _1 * _2;
                    BOOST_DEFINE_INCREMENT_N();
                }
                
                BOOST_ASSERT( n > 0 );
                BOOST_ASSERT( sum_xx >= 0 );
                BOOOS_ASSERT( sum_yy >= 0 );
            }  
        };
        
        
        // complexity = 10n
        template< class T >
        class x_y_xx_xy_sum
        {
            x_y_xx_xy_sum();
        
        public:
            typedef T value_type;
            
            T      sum_x;
            T      sum_y;
            T      sum_xx;
            T      sum_xy;
            size_t n;
        
            template< class InputIterator, class InputIterator2 >
            x_y_xx_xy_sum( InputIterator first, InputIterator last,
                           InputIterator2 first2, InputIterator2 last2 )
            {
                BOOST_DEFINE_IS_RANDOM_ACCESS();
                sum_x  = 0;
                sum_y  = 0;
                sum_xx = 0;
                sum_xy = 0;
        
                for( ; first != last; ++first, ++first2 )
                {
                    T _1    = *first;
                    T _2    = *first2;
                    sum_x  += _1;
                    sum_y  += _2;
                    sum_xx += _1 * _1; 
                    sum_xy += _1 * _2;
                    BOOST_DEFINE_INCREMENT_N();
                }
                
                BOOST_ASSERT( n > 0 );
                BOOST_ASSERT( sum_xx >= 0 );
            }  
        };

        
        // complexity = 12n
        template< class T >
        class x_y_xx_yy_xy_sum
        {
            x_y_xx_yy_xy_sum();
        
        public:
            typedef T value_type;
            
            T      sum_x;
            T      sum_y;
            T      sum_xx;
            T      sum_yy;
            T      sum_xy;
            size_t n;
        
            template< class InputIterator, class InputIterator2 >
            x_y_xx_yy_xy_sum( InputIterator first, InputIterator last,
                              InputIterator2 first2, InputIterator2 last2 )
            {
                BOOST_DEFINE_IS_RANDOM_ACCESS();
                sum_x  = 0;
                sum_y  = 0;
                sum_xx = 0;
                sum_yy = 0;
                sum_xy = 0;
        
                for( ; first != last; ++first, ++first2 )
                {
                    T _1    = *first;
                    T _2    = *first2;
                    sum_x  += _1;
                    sum_y  += _2;
                    sum_xx += _1 * _1;
                    sum_yy += _2 * _2; 
                    sum_xy += _1 * _2;
                    BOOST_DEFINE_INCREMENT_N();
                }
                
                BOOST_ASSERT( n > 0 );
                BOOST_ASSERT( sum_xx >= 0 );
                BOOST_ASSERT( sum_yy >= 0 );
            }  
        };

        
        
        template< class T, class Sum >
        inline T mean_x( const Sum& s )
        {
            return T( s.sum_x ) / s.n;
        }
        
        
        
        template< class T, class Sum >
        inline T mean_y( const Sum& s )
        {
            return T( s.sum_y ) / s.n;
        }
        
        
        // unbiased
        template< class T, class Sum >
        inline T variance_x( const Sum& s )
        {
            BOOST_ASSERT( s.n > 1 );
            return T( 1 ) / ( s.n - 1 ) * ( s.sum_xx - T( 1 ) / s.n * s.sum_x * s.sum_x );
        }
        
        
        // unbiased
        template< class T, class Sum >
        inline T variance_y( const Sum& s )
        {
            BOOST_ASSERT( s.n > 1 );
            return T( 1 ) / ( s.n - 1 )* ( s.sum_yy - T( 1 ) / s.n * s.sum_y * s.sum_y );
        }

        
        // unbiased
        template< class T, class Sum >
        inline  T covariance( const Sum& s )
        {
            BOOST_ASSERT( s.n > 1 );
            return T( 1 ) / ( s.n - 1 ) * ( s.sum_xy - T( 1 ) / s.n * s.sum_x * s.sum_y );
        }
        
        
        
        template< class T, class Sum >
        inline T correlation_coefficient( const Sum& s )
        {
            T numerator   = s.sum_xy - T( 1 ) / s.n * s.sum_x * s.sum_y;
            T denominator = ( s.sum_xx - T( 1 ) / s.n * s.sum_x * s.sum_x ) * 
                            ( s.sum_yy - T( 1 ) / s.n * s.sum_y * s.sum_y );
            T res         = numerator / std::sqrt( denominator );
            BOOST_ASSERT( res >= -1.0000001 && res <= 1.0000001 );
            return res;
        }
        
        
        
        template< class T, class Sum >
        inline T least_square_slope( const Sum& s )
        {
            return ( s.sum_xy - T( 1 ) / s.n * s.sum_x * s.sum_y ) / 
                   ( s.sum_xx - T( 1 ) / s.n * s.sum_x * s.sum_x );
        }
        
            
            
        template< class T, class Sum >
        inline T least_square_intersection( const Sum& s, T slope ) 
        {
            return T( 1 ) / s.n * ( s.sum_y - slope * s.sum_x );
        }
        
    } // namespace 'stat_detail'
    
} // namespace 'boost'


#undef BOOST_DEFINE_IS_RANDOM_ACCESS
#undef BOOST_DEFINE_INCREMENT_N

#endif



