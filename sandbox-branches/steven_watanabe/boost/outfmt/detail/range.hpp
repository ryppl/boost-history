// (C) Copyright 2003-2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_DETAIL_RANGE_T_HPP
#define BOOST_IOFM_DETAIL_RANGE_T_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <cstddef>
#  include <utility>
#  include <boost/detail/iterator.hpp>

   namespace boost { namespace io
   {
      template< typename RangeT >
      class range_t: public std::pair< RangeT, RangeT >
      {
         public:
            typedef std::pair< RangeT, RangeT >                      base_type;
            typedef typename boost::detail::iterator_traits< RangeT >::value_type
                                                                     elem_type;
         public:
            inline RangeT                        begin() const
            {
               return( base_type::first );
            }
            inline RangeT                        end() const
            {
               return( base_type::second );
            }
         private:
            inline           range_t();
         public:
            inline           range_t( const RangeT & r1, const RangeT & r2 ):
               std::pair< RangeT, RangeT >( r1, r2 )
            {
            }
            inline           range_t( const std::pair< RangeT, RangeT > & r ):
               std::pair< RangeT, RangeT >( r )
            {
            }
            inline           range_t( const range_t & r ):
               std::pair< RangeT, RangeT >( r )
            {
            }
      };

      // range construction

      template< typename ForwardIterator >
      range_t< ForwardIterator >                 range( ForwardIterator f, ForwardIterator l )
      {
         // precondition : f <= l
         return( range_t< ForwardIterator >( f, l ));
      }

      template< typename ForwardIterator >
      range_t< ForwardIterator >                 range( const std::pair< ForwardIterator, ForwardIterator > pi )
      {
         // precondition : pi.first <= pi.last
         return( range_t< ForwardIterator >( pi ));
      }

      template< class Container >
      range_t< typename Container::const_iterator >
                                                 range( const Container & c )
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
         // precondition : 0 <= off < n
         return( range( a + off, a + n ));
      }

      template< typename T, std::size_t n >
      range_t< const T * >                       range( const T a[ n ], std::size_t off, std::size_t len )
      {
         // precondition : 0 <= off       < n
         // precondition : 0 <= off + len < n
         // precondition : off <= off + len
         return( range( a + off, a + off + len ));
      }
   }}
#endif
