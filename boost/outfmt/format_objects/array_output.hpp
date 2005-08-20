// (C) Copyright 2003: Reece H. Dunn 

#ifndef BOOST__IOFM__FORMAT_OBJECTS__ARRAY_OUTPUT__HPP
#define BOOST__IOFM__FORMAT_OBJECTS__ARRAY_OUTPUT__HPP
#  include <boost/outfmt/format_objects/detail/list_output.hpp>

   namespace boost { namespace io
   {
      template< typename FormatType, class Outputter >
      class array_output: public detail::list_output< FormatType, array_output
                                                                  <
                                                                     FormatType,
                                                                     Outputter
                                                                  >, Outputter >
      {
         private:
            long                       length;
            long                       offset;
         public:
            template< typename T, class OutputStream >
            inline OutputStream & operator()
                                  (
                                     OutputStream & os,
                                     const T      * array
                                  ) const 
            {
               typedef detail::list_output< FormatType, array_output< FormatType, Outputter >, Outputter >
                                                                     base_type;

               const base_type *       self = static_cast< const base_type * >( this );
               return(( *self )( os, array + offset, array + offset + length ));
            }
         public:
            inline array_output & operator()( long len )
            {
               offset = 0;
               length = len;
               return( *this );
            }
            inline array_output & operator()( long off, long len )
            {
               offset = off;
               length = len;
               return( *this );
            }
         public:
            inline           array_output( const array_output & ao ):
               detail::list_output< FormatType, array_output< FormatType, Outputter >, Outputter >( ao ),
               length( ao.length ),
               offset( ao.offset )
            {
            }
            inline           array_output( long len = 0 ):
               length( len ),
               offset( 0 )
            {
            }
            inline           array_output( long off, long len ):
               length( len ),
               offset( off )
            {
            }
            inline           array_output( const Outputter & o, long len = 0 ):
               detail::list_output< FormatType, array_output< FormatType, Outputter >, Outputter >( o ),
               length( len ),
               offset( 0 )
            {
            }
            inline           array_output( const Outputter & o, long off, long len ):
               detail::list_output< FormatType, array_output< FormatType, Outputter >, Outputter >( o ),
               length( len ),
               offset( off )
            {
            }
      };

      template< class FormatType >
      inline array_output< FormatType >          arrayfmtex( long off, long len )
      {
         array_output< FormatType > out( off, len );
         return( out );
      }
      template< class FormatType >
      inline array_output< FormatType >          arrayfmtex( long len = 0 )
      {
         return( arrayfmtex< FormatType >( 0, len ));
      }

      inline array_output< char * >              arrayfmt( long off, long len )
      {
         return( arrayfmtex< char * >( off, len ));
      }
      inline array_output< char * >              arrayfmt( long len = 0 )
      {
         return( arrayfmtex< char * >( 0, len ));
      }

      template< class Outputter >
      inline array_output< typename Outputter::format_type, Outputter >
                                                 arrayfmtout
                                                 (
                                                    const Outputter & out,
                                                    long              off,
                                                    long              len
                                                 )
      {
         return( array_output< BOOST_DEDUCED_TYPENAME Outputter::format_type, Outputter >( out, off, len ));
      }
      template< class Outputter >
      inline array_output< typename Outputter::format_type, Outputter >
                                                 arrayfmtout
                                                 (
                                                    const Outputter & out,
                                                    long              len = 0
                                                 )
      {
         return( array_output< BOOST_DEDUCED_TYPENAME Outputter::format_type, Outputter >( out, len ));
      }
   }}
#endif
