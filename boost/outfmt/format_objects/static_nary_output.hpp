// (C) Copyright 2003: Reece H. Dunn

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#ifndef BOOST_IOFM_FormatObjects_StaticNaryOutput_HPP
#define BOOST_IOFM_FormatObjects_StaticNaryOutput_HPP
#  include <boost/outfmt/formatter.hpp>
#  include <boost/outfmt/getval.hpp>

   namespace boost { namespace io
   {
      template< typename FormatType, class Outputter >
      class static_nary_output: public formatter_t
                                <
                                   FormatType,
                                   static_nary_output< FormatType, Outputter >,
                                   boost::io::detail::pair_traits< FormatType >
                                >
      {
         public:
            typedef static_nary_output< FormatType, Outputter >      this_type;
            typedef typename formatter_t< FormatType, this_type, boost::io::detail::pair_traits< FormatType > >::traits_type
                                                                     traits_type;
         public:
            Outputter                  out;

         // output

         public:
            template< class T, class OutputStream >
            inline OutputStream & operator()
                                  (
                                     OutputStream & os,
                                     const T      & v
                                  ) const
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
               out( os, getval< 1 >( v )) << separator();
               out( os, getval< 2 >( v )) << separator();
               out( os, getval< 3 >( v )) << separator();
               return( out( os, getval< 4 >( v )) << close());
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
               out( os, getval< 1 >( v )) << separator();
               out( os, getval< 2 >( v )) << separator();
               out( os, getval< 3 >( v )) << separator();
               out( os, getval< 4 >( v )) << separator();
               out( os, getval< 5 >( v )) << separator();
               out( os, getval< 6 >( v )) << separator();
               out( os, getval< 7 >( v )) << separator();
               return( out( os, getval< 8 >( v )) << close());
            }

         // input

         public:
            template< class T, class InputStream >
            inline bool                          read
                                                 (
                                                    InputStream & is,
                                                    T           & v
                                                 ) const
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
               if( is.match( open()) && out.read( is, a ))
               {
                  U                    b;
                  if( is.match( separator()) && out.read( is, b ))
                  {
                     U                 c;
                     if( is.match( separator()) && out.read( is, c ))
                     {
                        U              d;
                        if( is.match( separator()) && out.read( is, d ) && is.match( close()))
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
               if( is.match( open()) && out.read( is, a ) && is.match( separator()) && out.read( is, b ))
               {
                  U                    c;
                  U                    d;
                  if( is.match( separator()) && out.read( is, c ) && is.match( separator()) && out.read( is, d ))
                  {
                     U                 e;
                     U                 f;
                     if( is.match( separator()) && out.read( is, e ) && is.match( separator()) && out.read( is, f ))
                     {
                        U              g;
                        U              h;
                        if( is.match( separator()) && out.read( is, g ) && is.match( separator()) && out.read( is, h ) && is.match( close()))
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
            inline           static_nary_output()
            {
            }
            inline           static_nary_output( const static_nary_output & o ):
               formatter_t< FormatType, this_type, boost::io::detail::pair_traits< FormatType > >( o ),
               out( o.out )
            {
            }
            inline           static_nary_output( const Outputter & o ):
               out( o )
            {
            }
      };

      template< class FormatType >
      inline static_nary_output< FormatType >    naryfmtex()
      {
         static_nary_output< FormatType > out;
         return( out );
      }

      inline static_nary_output< char * >        naryfmt()
      {
         return( naryfmtex< char * >());
      }

      template< class Outputter >
      inline static_nary_output< typename Outputter::format_type, Outputter >
                                                 naryfmt
                                                 (
                                                    const Outputter & out
                                                 )
      {
         return( static_nary_output< typename Outputter::format_type, Outputter >( out ));
      }
   }}
#endif
