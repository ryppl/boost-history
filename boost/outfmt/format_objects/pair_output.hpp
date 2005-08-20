// (C) Copyright 2003: Reece H. Dunn 

#ifndef BOOST__IOFM__FORMAT_OBJECTS__PAIR_OUTPUT__HPP
#define BOOST__IOFM__FORMAT_OBJECTS__PAIR_OUTPUT__HPP
#  include <boost/outfmt/formatter.hpp>

#  include <utility>                             // std::pair
#  include <complex>                             // std::complex
#  include <boost/compressed_pair.hpp>           // boost::compressed_pair
#  if !defined(BOOST_IOFM__NO_LIB_INTERVAL)
#     include <boost/numeric/interval/interval.hpp> // boost::numeric::interval
#  endif
#  if !defined(BOOST_IOFM__NO_LIB_RATIONAL)
#     include <boost/rational.hpp>               // boost::rational
#  endif

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
         public: // standard library dual-valued types
            template< typename T1, typename T2, class OutputStream >
            inline OutputStream & operator()
                                  (
                                     OutputStream & os,
                                     const std::pair< T1, T2 > & p
                                  ) const
            {
               return( out( os, p.first, p.second ));
            }
            template< typename T, class OutputStream >
            inline OutputStream & operator()
                                  (
                                     OutputStream & os,
                                     const std::complex< T > & c
                                  ) const
            {
               return( out( os, c.real(), c.imag()));
            }
         public: // boost dual-valued types
#           if !defined(BOOST_IOFM__NO_LIB_INTERVAL)
               template< typename T, class Traits, class OutputStream >
               inline OutputStream &
                                  operator()
                                  (
                                     OutputStream & os,
                                     const boost::numeric::interval< T, Traits > & i
                                  ) const
               {
                  return( out( os, i.lower(), i.upper()));
               }
#           endif
#           if !defined(BOOST_IOFM__NO_LIB_RATIONAL)
               template< typename T, class OutputStream >
               inline OutputStream &
                                  operator()
                                  (
                                     OutputStream & os,
                                     const boost::rational< T > & r
                                  ) const
               {
                  return( out( os, r.numerator(), r.denominator()));
               }
#           endif
            template< typename T1, typename T2, class OutputStream >
            inline OutputStream & operator()
                                  (
                                     OutputStream & os,
                                     const boost::compressed_pair< T1, T2 > & cp
                                  ) const
            {
               return( out( os, cp.first(), cp.second()));
            }
         private: // internal implementation
            template< typename T1, typename T2, class OutputStream >
            inline OutputStream &                out
                                                 (
                                                    OutputStream & os,
                                                    const T1 &     first,
                                                    const T2 &     second
                                                 ) const 
            {
               os << open();
               out1( os, first ) << separator();
               return( out2( os, second ) << close());
            }
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
                                                 pairfmtout
                                                 (
                                                    const Outputter1 & out1
                                                 )
      {
         return( pair_output< typename Outputter1::format_type, Outputter1, boost::io::basic_output >( out1 ));
      }

      template< class Outputter1, class Outputter2 >
      inline pair_output< typename Outputter1::format_type, Outputter1, Outputter2 >
                                                 pairfmtout
                                                 (
                                                    const Outputter1 & out1,
                                                    const Outputter2 & out2
                                                 )
      {
         return( pair_output< typename Outputter1::format_type, Outputter1, Outputter2 >( out1, out2 ));
      }
   }}
#endif
