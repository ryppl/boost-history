// (C) Copyright 2003-2004: Reece H. Dunn

#ifndef BOOST_IOFM_FormatObjects_StaticNary_HPP
#define BOOST_IOFM_FormatObjects_StaticNary_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <boost/outfmt/formatter.hpp>
#  include <boost/outfmt/detail/naryval.hpp>

   namespace boost { namespace io
   {
      template< typename FormatType, class FmtObject >
      class static_nary_object: public formatter_t
                                <
                                   FormatType,
                                   static_nary_object< FormatType, FmtObject >,
                                   boost::io::detail::pair_traits< FormatType >
                                >
      {
         public:
            typedef static_nary_object< FormatType, FmtObject >      this_type;
            typedef typename formatter_t< FormatType, this_type, boost::io::detail::pair_traits< FormatType > >::traits_type
                                                                     traits_type;
            typedef seq_type< nary_type >                            formatter_type;
         public:
            FmtObject                  fo;

         // output

         public:
            template< class T, class OutputStream >
            inline OutputStream &                write( OutputStream & os, const T & v ) const
            {
               return( outval( os, v, narytype( v )));
            }
         private:
            template< typename T, typename U, class OutputStream >
            inline OutputStream &                outval
                                                 (
                                                    OutputStream & os,
                                                    const T      & v,
                                                    nary4_type< U >
                                                 ) const
            {
               os << open();
               fo.write( os, getval< 1 >( v )) << separator();
               fo.write( os, getval< 2 >( v )) << separator();
               fo.write( os, getval< 3 >( v )) << separator();
               return( fo.write( os, getval< 4 >( v )) << close());
            }
            template< typename T, typename U, class OutputStream >
            inline OutputStream &                outval
                                                 (
                                                    OutputStream & os,
                                                    const T      & v,
                                                    nary8_type< U >
                                                 ) const
            {
               os << open();
               fo.write( os, getval< 1 >( v )) << separator();
               fo.write( os, getval< 2 >( v )) << separator();
               fo.write( os, getval< 3 >( v )) << separator();
               fo.write( os, getval< 4 >( v )) << separator();
               fo.write( os, getval< 5 >( v )) << separator();
               fo.write( os, getval< 6 >( v )) << separator();
               fo.write( os, getval< 7 >( v )) << separator();
               return( fo.write( os, getval< 8 >( v )) << close());
            }

         // input

         public:
            template< class T, class InputStream >
            inline bool                          read( InputStream & is, T & v ) const
            {
               return( inval( is, v, narytype( v )));
            }
         private:
            template< typename T, typename U, class InputStream >
            inline bool                          inval
                                                 (
                                                    InputStream & is,
                                                    T           & v,
                                                    nary4_type< U >
                                                 ) const
            {
               // [review]: is there a better way to do this?
               U                       a;
               if( is.match( open()) && fo.read( is, a ))
               {
                  U                    b;
                  if( is.match( separator()) && fo.read( is, b ))
                  {
                     U                 c;
                     if( is.match( separator()) && fo.read( is, c ))
                     {
                        U              d;
                        if( is.match( separator()) && fo.read( is, d ) && is.match( close()))
                        {
                           assignval( v, a, b, c, d );
                           return( true );
                        }
                     }
                  }
               }
               return( false );
            }
            template< typename T, typename U, class InputStream >
            inline bool                          inval
                                                 (
                                                    InputStream & is,
                                                    T           & v,
                                                    nary8_type< U >
                                                 ) const
            {
               // [review]: is there a better way to do this?
               U                       a;
               U                       b;
               if( is.match( open()) && fo.read( is, a ) && is.match( separator()) && fo.read( is, b ))
               {
                  U                    c;
                  U                    d;
                  if( is.match( separator()) && fo.read( is, c ) && is.match( separator()) && fo.read( is, d ))
                  {
                     U                 e;
                     U                 f;
                     if( is.match( separator()) && fo.read( is, e ) && is.match( separator()) && fo.read( is, f ))
                     {
                        U              g;
                        U              h;
                        if( is.match( separator()) && fo.read( is, g ) && is.match( separator()) && fo.read( is, h ) && is.match( close()))
                        {
                           assignval( v, a, b, c, d, e, f, g, h );
                           return( true );
                        }
                     }
                  }
               }
               return( false );
            }

         // constructors

         public:
            inline           static_nary_object()
            {
            }
            inline           static_nary_object( const static_nary_object & o ):
               formatter_t< FormatType, this_type, boost::io::detail::pair_traits< FormatType > >( o ),
               fo( o.fo )
            {
            }
            inline           static_nary_object( const FmtObject & o ):
               fo( o )
            {
            }
      };

      template< class FormatType >
      inline static_nary_object< FormatType >    naryfmtex()
      {
         static_nary_object< FormatType > ob;
         return( ob );
      }

      inline static_nary_object< const char * >        naryfmt()
      {
         return( naryfmtex< const char * >());
      }

      template< class FmtObject >
      inline static_nary_object< typename FmtObject::format_type, FmtObject >
                                                 naryfmt
                                                 (
                                                    const FmtObject & fo
                                                 )
      {
         return( static_nary_object< typename FmtObject::format_type, FmtObject >( fo ));
      }
   }}
#endif
