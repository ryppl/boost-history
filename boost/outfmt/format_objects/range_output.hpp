// (C) Copyright 2003: Reece H. Dunn 

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#ifndef BOOST_IOFM_FormatObjects_RangeOutput_HPP
#define BOOST_IOFM_FormatObjects_RangeOutput_HPP
#  include <boost/outfmt/format_objects/detail/list_output.hpp>

#  include <utility> // std::pair

   namespace boost { namespace io
   {
      template< typename FormatType, class Outputter >
      class range_output: public detail::list_output< FormatType, range_output
                                                                  <
                                                                     FormatType,
                                                                     Outputter
                                                                  >, Outputter >
      {
         public:
            template< typename ForwardIterator, class OutputStream >
            inline OutputStream & operator()
                                  (
                                     OutputStream & os,
                                     const std::pair< ForwardIterator, ForwardIterator > & ip
                                  ) const 
            {
               typedef detail::list_output< FormatType, range_output< FormatType, Outputter >, Outputter >
                                                                     base_type;

               const base_type *       self = static_cast< const base_type * >( this );
               return(( *self )( os, ip.first, ip.second ));
            }
         public:
            inline           range_output()
            {
            }
            inline           range_output( const range_output & o ):
               detail::list_output< FormatType, range_output< FormatType, Outputter >, Outputter >( o )
            {
            }
            inline           range_output( const Outputter & o ):
               detail::list_output< FormatType, range_output< FormatType, Outputter >, Outputter >( o )
            {
            }
      };

      // range construction helpers

      template< typename ForwardIterator >
      std::pair< ForwardIterator, ForwardIterator >
                                                 range( ForwardIterator f, ForwardIterator l )
      {
         return( std::pair< ForwardIterator, ForwardIterator >( f, l ));
      }

      template< class Container >
      std::pair< typename Container::iterator, typename Container::iterator >
                                                 range( Container & c )
      {
         return( range( c.begin(), c.end()));
      }

      template< typename T, std::size_t n >
      std::pair< const T *, const T * >
                                                 range( const T a[ n ] )
      {
         return( range( a, a + n ));
      }

      template< typename T, std::size_t n >
      std::pair< const T *, const T * >
                                                 range( const T a[ n ], std::size_t off )
      {
         // pre: 0 <= off < n
         return( range( a + off, a + n ));
      }

      template< typename T, std::size_t n >
      std::pair< const T *, const T * >
                                                 range( const T a[ n ], std::size_t off, std::size_t len )
      {
         // pre: 0 <= off < n
         // pre: 0 <= ( off + len ) < n
         return( range( a + off, a + off + len ));
      }

      // range format generators

      template< class FormatType >
      inline range_output< FormatType >          rangefmtex()
      {
         range_output< FormatType >    out;
         return( out );
      }

      inline range_output< char * >              rangefmt()
      {
         return( rangefmtex< char * >());
      }

      template< class Outputter >
      inline range_output< BOOST_DEDUCED_TYPENAME Outputter::format_type, Outputter >
                                                 rangefmt
                                                 (
                                                    const Outputter & out
                                                 )
      {
         return( range_output< BOOST_DEDUCED_TYPENAME Outputter::format_type, Outputter >( out ));
      }
   }}
#endif
