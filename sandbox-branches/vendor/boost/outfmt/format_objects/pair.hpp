// (C) Copyright 2003-2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_FormatObjects_Pair_HPP
#define BOOST_IOFM_FormatObjects_Pair_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <boost/outfmt/formatter.hpp>
#  include <boost/outfmt/detail/naryval.hpp>

   namespace boost { namespace io
   {
      template< typename DelimeterType, class FormatObject1, class FormatObject2 >
      class pairfmt_t: public formatter_t
                              <
                                 DelimeterType,
                                 pairfmt_t< DelimeterType, FormatObject1, FormatObject2 >,
                                 boost::io::detail::default_nary_traits< DelimeterType >
                              >
      {
         public:
            typedef pairfmt_t< DelimeterType, FormatObject1, FormatObject2 >
                                                                     this_type;
            typedef typename formatter_t< DelimeterType, this_type, boost::io::detail::default_nary_traits< DelimeterType > >::traits_type
                                                                     traits_type;
            typedef seq_type< pair_type >                            formatter_type;
         private:
            FormatObject1              fo1;
            FormatObject2              fo2;
         public:
            template< typename T, class InputStream >
            inline bool                          read( InputStream & is, T & v ) const
            {
               return( read( is, v, narytype( v )));
            }
         private: 
            template< typename T, class InputStream >
            inline bool                          read( InputStream & is, T & v, seperable_pair ) const
            {
               return( in( is, refval< 1 >( v ), refval< 2 >( v )));
            }
            template< typename T, typename T1, typename T2, class InputStream >
            inline bool                          read( InputStream & is, T & v, inseperable_pair< T1, T2 > ) const
            {
               T1                      a;
               T2                      b;
               if( in( is, a, b ))
               {
                  assignval( v, a, b );
                  return( true );
               }
               return( false );
            }
            template< typename T1, typename T2, class InputStream >
            inline bool                          in
                                                 (
                                                    InputStream & is,
                                                    T1          & t1,
                                                    T2          & t2
                                                 ) const
            {
               if( is.match( open()) && fo1.read( is, t1 ))
               {
                  if( is.match( separator()) && fo2.read( is, t2 ) && is.match( close()))
                     return( true );
               }
               return( false );
            }
         public:
            template< class T, class OutputStream >
            inline OutputStream &                write( OutputStream & os, const T & v ) const
            {
               os << open();
               fo1.write( os, getval< 1 >( v )) << separator();
               return( fo2.write( os, getval< 2 >( v )) << close());
            }

         // constructors

         public:
            inline           pairfmt_t()
            {
            }
            inline           pairfmt_t( const pairfmt_t & po ):
               formatter_t< DelimeterType, this_type, boost::io::detail::default_nary_traits< DelimeterType > >( po ),
               fo1( po.fo1 ),
               fo2( po.fo2 )
            {
            }
            inline           pairfmt_t( const FormatObject1 & o1 ): fo1( o1 )
            {
            }
            inline           pairfmt_t
                             (
                                const FormatObject1 & o1,
                                const FormatObject2 & o2
                             ):
               fo1( o1 ),
               fo2( o2 )
            {
            }
      };

      template< class DelimeterType >
      inline pairfmt_t< DelimeterType >          pairfmtex()
      {
         pairfmt_t< DelimeterType >     ob;
         return( ob );
      }

      inline pairfmt_t< const char * >           pairfmt()
      {
         return( pairfmtex< const char * >());
      }

      template< class FormatObject1 >
      inline pairfmt_t< typename FormatObject1::format_type, FormatObject1 > //, boost::io::basicfmt_t >
                                                 pairfmt
                                                 (
                                                    const FormatObject1 & fo
                                                 )
      {
         return( pairfmt_t< typename FormatObject1::format_type, FormatObject1 >( fo ));
      }

      template< class FormatObject1, class FormatObject2 >
      inline pairfmt_t< typename FormatObject1::format_type, FormatObject1, FormatObject2 >
                                                 pairfmt
                                                 (
                                                    const FormatObject1 & fo1,
                                                    const FormatObject2 & fo2
                                                 )
      {
         return( pairfmt_t< BOOST_DEDUCED_TYPENAME FormatObject1::format_type, FormatObject1, FormatObject2 >( fo1, fo2 ));
      }
   }}
#endif
