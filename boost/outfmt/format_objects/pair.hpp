// (C) Copyright 2003: Reece H. Dunn

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#ifndef BOOST_IOFM_FormatObjects_Pair_HPP
#define BOOST_IOFM_FormatObjects_Pair_HPP
#  include <boost/outfmt/formatter.hpp>
#  include <boost/outfmt/detail/naryval.hpp>

   namespace boost { namespace io
   {
      template< typename FormatType, class FmtObject1, class FmtObject2 >
      class pair_object: public formatter_t
                                <
                                   FormatType,
                                   pair_object< FormatType, FmtObject1, FmtObject2 >,
                                   boost::io::detail::pair_traits< FormatType >
                                >
      {
         public:
            typedef pair_object< FormatType, FmtObject1, FmtObject2 >
                                                                     this_type;
            typedef typename formatter_t< FormatType, this_type, boost::io::detail::pair_traits< FormatType > >::traits_type
                                                                     traits_type;
         private:
            FmtObject1                 fo1;
            FmtObject2                 fo2;
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
            inline           pair_object()
            {
            }
            inline           pair_object( const pair_object & po ):
               formatter_t< FormatType, this_type, boost::io::detail::pair_traits< FormatType > >( po ),
               fo1( po.fo1 ),
               fo2( po.fo2 )
            {
            }
            inline           pair_object( const FmtObject1 & o1 ): fo1( o1 )
            {
            }
            inline           pair_object
                             (
                                const FmtObject1 & o1,
                                const FmtObject2 & o2
                             ):
               fo1( o1 ),
               fo2( o2 )
            {
            }
      };

      template< class FormatType >
      inline pair_object< FormatType >           pairfmtex()
      {
         pair_object< FormatType >     ob;
         return( ob );
      }

      inline pair_object< char * >               pairfmt()
      {
         return( pairfmtex< char * >());
      }

      template< class FmtObject1 >
      inline pair_object< typename FmtObject1::format_type, FmtObject1 > //, boost::io::basic_object >
                                                 pairfmt
                                                 (
                                                    const FmtObject1 & fo
                                                 )
      {
         return( pair_object< typename FmtObject1::format_type, FmtObject1 >( fo ));
      }

      template< class FmtObject1, class FmtObject2 >
      inline pair_object< typename FmtObject1::format_type, FmtObject1, FmtObject2 >
                                                 pairfmt
                                                 (
                                                    const FmtObject1 & fo1,
                                                    const FmtObject2 & fo2
                                                 )
      {
         return( pair_object< typename FmtObject1::format_type, FmtObject1, FmtObject2 >( fo1, fo2 ));
      }
   }}
#endif
