// (C) Copyright 2003: Reece H. Dunn

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#ifndef BOOST_IOFM_DETAIL_RANGE_T_HPP
#define BOOST_IOFM_DETAIL_RANGE_T_HPP
   namespace boost { namespace io
   {
      template< typename RangeT >
      class range_t: public std::pair< RangeT, RangeT >
      {
         public:
            inline RangeT                        begin() const
            {
               return( first );
            }
            inline RangeT                        end() const
            {
               return( second );
            }
         private:
            inline           range_t();
         public:
            inline           range_t( const RangeT & r1, const RangeT & r2 ):
               std::pair< RangeT, RangeT >( r1, r2 )
            {
            }
            template< typename RangeT2 >
            inline           range_t( const range_t< RangeT2 > & r ):
               std::pair< RangeT, RangeT >( r )
            {
            }
      };

      // range construction

      template< typename ForwardIterator >
      range_t< ForwardIterator >                 range( ForwardIterator f, ForwardIterator l )
      {
         // boost::precondition<>::test( f <= l, "invalid range" );
         return( range_t< ForwardIterator >( f, l ));
      }

      template< class Container >
      range_t< typename Container::iterator >    range( Container & c )
      {
         return( range_t< ForwardIterator >( c.begin(), c.end()));
      }

      template< typename T, std::size_t n >
      range_t< const T * >                       range( const T a[ n ] )
      {
         return( range_t< ForwardIterator >( a, a + n ));
      }

      template< typename T, std::size_t n >
      range_t< const T * >                       range( const T a[ n ], std::size_t off )
      {
         // boost::precondition<>::test( boost::between( off, 0, n ), "range: invalid offset" );
         return( range_t< ForwardIterator >( a + off, a + n ));
      }

      template< typename T, std::size_t n >
      range_t< const T * >                       range( const T a[ n ], std::size_t off, std::size_t len )
      {
         // boost::precondition<>::test( boost::between( off,       0, n ), "range: invalid offset" );
         // boost::precondition<>::test( boost::between( off + len, 0, n ), "range: invalid offset/length" );
         return( range_t< ForwardIterator >( a + off, a + off + len ));
      }
   }}
#endif
