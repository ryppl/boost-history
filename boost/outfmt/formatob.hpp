// (C) Copyright 2003: Reece H. Dunn 

#ifndef BOOST__IOFM__FORMATOB__HPP
#define BOOST__IOFM__FORMATOB__HPP
#  include <boost/outfmt/format_objects.hpp>
#  include <boost/outfmt/detail/config.hpp>
#  if !defined(BOOST_IOFM__NO_OUTPUT_DEDUCTION) // automatic type deduction
#     include <boost/outfmt/deducer.hpp>
#  endif

#  include <iostream>
#  include <boost/noncopyable.hpp>

   namespace boost { namespace io
   {
      template< typename T, typename FormatType = char *, class Outputter = boost::io::basic_output >
      class formatob_t: public Outputter, public boost::noncopyable
      {
         public:
            const T &                  ob;
         public: // constructors
            inline           formatob_t( const formatob_t & fo ):
               Outputter( fo ),
               ob( fo.ob )
            {
            }
            inline           formatob_t( const T & val ) throw(): ob( val )
            {
            }
            inline           formatob_t( const T & val, const Outputter & o ) throw():
               Outputter( o ),
               ob( val )
            {
            }
      };

#     if !defined(BOOST_IOFM__NO_BASIC_STREAM)
         template< typename CharT, class TraitsT, typename T, typename FormatType, typename Outputter >
         inline std::basic_ostream< CharT, TraitsT > & operator<<
         (
            std::basic_ostream< CharT, TraitsT >         & os,
            const formatob_t< T, FormatType, Outputter > & fo
         )
         {
            return( fo( os, fo.ob ));
         }
#     else
         template< typename T, typename FormatType, typename Outputter >
         inline std::ostream & operator<<
         (
            std::ostream & os,
            const formatob_t< T, FormatType, Outputter > & fo
         )
         {
            return( fo( os, fo.ob ));
         }
#     endif

      // format object generators

#     if !defined(BOOST_IOFM__NO_OUTPUT_DEDUCTION) // automatic type deduction
         template< typename T >
         inline formatob_t< T, char *, typename deducer< T, char * >::type::outputter >
                                                 formatob( const T & t )
         {
            return( formatob_t< T, char *, BOOST_DEDUCED_TYPENAME deducer< T, char * >::type::outputter >
            (
               t,
               deducer< T, char * >::type::generate()
            ));
         }

         template< typename FormatType, typename T >
         inline formatob_t< T, FormatType, typename deducer< T, FormatType >::type::outputter >
                                                 formatobex( const T & t )
         {
            return( formatob_t< T, FormatType, BOOST_DEDUCED_TYPENAME deducer< T, FormatType >::type::outputter >
            (
               t,
               deducer< T, FormatType >::type::generate()
            ));
         }
#     else // no automatic deduction: use basic_output instead
         template< typename T >
         inline formatob_t< T >                  formatob( const T & t )
         {
            return( formatob_t< T >( t ));
         }

         template< typename FormatType, typename T >
         inline formatob_t< T, FormatType >      formatobex( const T & t )
         {
            return( formatob_t< T, FormatType >( t ));
         }
#     endif

      template< typename T, class Outputter >
      inline formatob_t< T, typename Outputter::format_type, Outputter >
                                                 formatobout( const T & t, const Outputter & o )
      {
         return( formatob_t< T, BOOST_DEDUCED_TYPENAME Outputter::format_type, Outputter >( t, o ));
      }
   }}
#endif
