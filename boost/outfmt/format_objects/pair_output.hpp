// (C) Copyright 2003: Reece H. Dunn

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#ifndef BOOST_IOFM_FormatObjects_PairOutput_HPP
#define BOOST_IOFM_FormatObjects_PairOutput_HPP
#  include <boost/outfmt/formatter.hpp>
#  include <boost/outfmt/getval.hpp>

   namespace boost { namespace io
   {
      template< typename FormatType, class Outputter1, class Outputter2 >
      class pair_output: public formatter_t
                                <
                                   FormatType,
                                   pair_output< FormatType, Outputter1, Outputter2 >,
                                   boost::io::detail::pair_traits< FormatType >
                                >
      {
         public:
            typedef pair_output< FormatType, Outputter1, Outputter2 >
                                                                     this_type;
            typedef typename formatter_t< FormatType, this_type, boost::io::detail::pair_traits< FormatType > >::traits_type
                                                                     traits_type;
         private:
            Outputter1                 out1;
            Outputter2                 out2;
         public: // nary< 2 > types
            template< class T, class OutputStream >
            inline OutputStream & operator()
                                  (
                                     OutputStream & os,
                                     const T      & v
                                  ) const
            {
               os << open();
               out1( os, getval< 1 >( v )) << separator();
               return( out2( os, getval< 2 >( v )) << close());
            }

         // input

         public:
            template< typename T, class InputStream >
            inline bool                          read( InputStream & is, T & v ) const
            {
               return( read( is, v, narytype( v )));
            }
         private: // internal implementation
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
               if( is.match( open()) && out1.read( is, t1 ))
               {
                  if( is.match( separator()) && out2.read( is, t2 ) && is.match( close()))
                     return( true );
               }
               return( false );
            }

         // constructors

         public:
            inline           pair_output()
            {
            }
            inline           pair_output( const pair_output & po ):
               formatter_t< FormatType, this_type, boost::io::detail::pair_traits< FormatType > >( po ),
               out1( po.out1 ),
               out2( po.out2 )
            {
            }
            inline           pair_output( const Outputter1 & o1 ): out1( o1 )
            {
            }
            inline           pair_output
                             (
                                const Outputter1 & o1,
                                const Outputter2 & o2
                             ):
               out1( o1 ),
               out2( o2 )
            {
            }
      };

      template< class FormatType >
      inline pair_output< FormatType >           pairfmtex()
      {
         pair_output< FormatType >     out;
         return( out );
      }

      inline pair_output< char * >               pairfmt()
      {
         return( pairfmtex< char * >());
      }

      template< class Outputter1 >
      inline pair_output< typename Outputter1::format_type, Outputter1, boost::io::basic_output >
                                                 pairfmt
                                                 (
                                                    const Outputter1 & out1
                                                 )
      {
         return( pair_output< typename Outputter1::format_type, Outputter1, boost::io::basic_output >( out1 ));
      }

      template< class Outputter1, class Outputter2 >
      inline pair_output< typename Outputter1::format_type, Outputter1, Outputter2 >
                                                 pairfmt
                                                 (
                                                    const Outputter1 & out1,
                                                    const Outputter2 & out2
                                                 )
      {
         return( pair_output< typename Outputter1::format_type, Outputter1, Outputter2 >( out1, out2 ));
      }
   }}
#endif
