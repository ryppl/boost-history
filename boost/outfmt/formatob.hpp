// (C) Copyright 2003: Reece H. Dunn 

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#ifndef BOOST_IOFM_FormatOb_HPP
#define BOOST_IOFM_FormatOb_HPP
#  include <boost/outfmt/format_objects.hpp>
#  include <boost/outfmt/detail/config.hpp>
#  if !defined(BOOST_IOFM_NO_OUTPUT_DEDUCTION) // automatic type deduction
#     include <boost/outfmt/deducer.hpp>
#  endif

#  include <iostream>
#  include <boost/noncopyable.hpp>

   namespace boost { namespace io
   {
      template< typename T, typename FormatType = char *, class FmtObject = boost::io::basic_object >
      class formatob_t: public FmtObject, public boost::noncopyable
      {
         public:
            typename mpl::if_
            <
               mpl::or_
               <
                  is_range< T >, is_formatter< FmtObject, range_type >,
                  is_basic< T >, is_formatter< FmtObject, basic_type >
               >,
               typename boost::call_traits< T >::value_type,
               typename boost::call_traits< T >::const_reference
            >::type                    ob;
         public:
            typedef typename FmtObject::format_type        format_type;
            typedef typename FmtObject::traits_type        traits_type;
         public:
            inline formatob_t &                  format
                                                 (
                                                    format_type o,
                                                    format_type c,
                                                    format_type s
                                                 )
            {
               FmtObject::format( o, c, s );
               return( *this );
            }
            inline formatob_t &                  format
                                                 (
                                                    format_type o,
                                                    format_type c
                                                 )
            {
               FmtObject::format( o, c );
               return( *this );
            }
            inline formatob_t &                  format
                                                 (
                                                    format_type s
                                                 )
            {
               FmtObject::format( s );
               return( *this );
            }

            template< typename RT2, class FormatTraits2 >
            inline formatob_t &                  format
                                                 (
                                                    const openclose_formatter_t< FormatType, RT2, FormatTraits2 > & fmt
                                                 )
            {
               FmtObject::format( fmt );
               return( *this );
            }

            template< typename RT2, class FormatTraits2 >
            inline formatob_t &                  format
                                                 (
                                                    const formatter_t< FormatType, RT2, FormatTraits2 > & fmt
                                                 )
            {
               FmtObject::format( fmt );
               return( *this );
            }
         public: // constructors
            inline           formatob_t( const formatob_t & fo ):
               FmtObject( fo ),
               ob( fo.ob )
            {
            }
            inline           formatob_t( const T & val ): ob( val )
            {
            }
            inline           formatob_t( const T & val, const FmtObject & o ):
               FmtObject( o ),
               ob( val )
            {
            }
      };

#     if !defined(BOOST_IOFM_NO_BASIC_STREAM)
         template< typename CharT, class TraitsT, typename T, typename FormatType, typename FmtObject >
         inline std::basic_ostream< CharT, TraitsT > & operator<<
         (
            std::basic_ostream< CharT, TraitsT > & os,
            const formatob_t< T, FormatType, FmtObject > & fo
         )
         {
            return( fo.write( os, fo.ob ));
         }

         template< typename CharT, class TraitsT, typename T, typename FormatType, typename FmtObject >
         inline std::basic_istream< CharT, TraitsT > & operator>>
         (
            std::basic_istream< CharT, TraitsT >         & is,
            const formatob_t< T, FormatType, FmtObject > & fo
         )
         {
            boost::io::detail::input_helper< std::basic_istream< CharT, TraitsT > >
                                                 in( is );
            fo.read( in, const_cast< T & >( fo.ob ));
            return( is );
         }
#     else
         template< typename T, typename FormatType, typename FmtObject >
         inline std::ostream & operator<<
         (
            std::ostream & os,
            const formatob_t< T, FormatType, FmtObject > & fo
         )
         {
            return( fo.write( os, fo.ob ));
         }

         template< typename T, typename FormatType, typename FmtObject >
         inline std::istream & operator>>
         (
            std::istream & is,
            const formatob_t< T, FormatType, FmtObject > & fo
         )
         {
            boost::io::detail::input_helper< std::istream >
                                                 in( is );
            fo.read( in, const_cast< T & >( fo.ob ));
            return( is );
         }
#     endif

      // format object generators

#     if !defined(BOOST_IOFM_NO_OUTPUT_DEDUCTION) // automatic type deduction
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

      template< typename T, class FmtObject >
      inline formatob_t< T, typename FmtObject::format_type, FmtObject >
                                                 formatob( const T & t, const FmtObject & o )
      {
         return( formatob_t< T, BOOST_DEDUCED_TYPENAME FmtObject::format_type, FmtObject >( t, o ));
      }
   }}
#endif
