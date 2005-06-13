// (C) Copyright 2003-2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_FormatObjects_StaticNary_HPP
#define BOOST_IOFM_FormatObjects_StaticNary_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <boost/outfmt/formatter.hpp>
#  include <boost/outfmt/detail/naryval.hpp>

   namespace boost { namespace io
   {
      template< typename DelimeterType, class FormatObject >
      class naryfmt_t: public formatter_t
                              <
                                 DelimeterType,
                                 naryfmt_t< DelimeterType, FormatObject >,
                                 boost::io::detail::default_nary_traits< DelimeterType >
                              >
      {
         public:
            typedef naryfmt_t< DelimeterType, FormatObject >         this_type;
            typedef typename formatter_t< DelimeterType, this_type, boost::io::detail::default_nary_traits< DelimeterType > >::traits_type
                                                                     traits_type;
            typedef seq_type< nary_type >                            formatter_type;
         public:
            FormatObject               fo;

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
            inline           naryfmt_t()
            {
            }
            inline           naryfmt_t( const naryfmt_t & o ):
               formatter_t< DelimeterType, this_type, boost::io::detail::default_nary_traits< DelimeterType > >( o ),
               fo( o.fo )
            {
            }
            inline           naryfmt_t( const FormatObject & o ):
               fo( o )
            {
            }
      };

      template< class DelimeterType >
      inline naryfmt_t< DelimeterType >          naryfmtex()
      {
         naryfmt_t< DelimeterType > ob;
         return( ob );
      }

      inline naryfmt_t< const char * >           naryfmt()
      {
         return( naryfmtex< const char * >());
      }

      template< class FormatObject >
      inline naryfmt_t< typename FormatObject::format_type, FormatObject >
                                                 naryfmt
                                                 (
                                                    const FormatObject & fo
                                                 )
      {
         return( naryfmt_t< typename FormatObject::format_type, FormatObject >( fo ));
      }
   }}
#endif
