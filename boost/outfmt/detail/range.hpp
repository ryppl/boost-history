// (C) Copyright 2003: Reece H. Dunn

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#ifndef BOOST_IOFM_DETAIL_RANGE_T_HPP
#define BOOST_IOFM_DETAIL_RANGE_T_HPP
#  include <iomanip>

   namespace boost { namespace io
   {
      template< typename RangeT >
      class range_t: public boost::noncopyable
      {
         public:
            RangeT                     first;
            RangeT                     last;
         public:
            inline           range_t():
               first(),
               last()
            {
            }
            inline           range_t( const RangeT & r1, const RangeT & r2 ):
               first( r1 ),
               last(  r2 )
            {
            }
            inline           range_t( const range_t & r ):
               first( r.first ),
               last(  r.last )
            {
            }
      };

      // range construction

      template< typename ForwardIterator >
      range_t< ForwardIterator >                 range( ForwardIterator f, ForwardIterator l )
      {
         return( range_t< ForwardIterator >( f, l ));
      }

      template< class Container >
      range_t< typename Container::iterator >    range( Container & c )
      {
         return( range( c.begin(), c.end()));
      }

      template< typename T, std::size_t n >
      range_t< const T * >                       range( const T a[ n ] )
      {
         return( range( a, a + n ));
      }

      template< typename T, std::size_t n >
      range_t< const T * >                       range( const T a[ n ], std::size_t off )
      {
         // pre: 0 <= off < n
         return( range( a + off, a + n ));
      }

      template< typename T, std::size_t n >
      range_t< const T * >                       range( const T a[ n ], std::size_t off, std::size_t len )
      {
         // pre: 0 <= off < n
         // pre: 0 <= ( off + len ) < n
         return( range( a + off, a + off + len ));
      }
   }}
#endif
