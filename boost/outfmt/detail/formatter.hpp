// (C) Copyright 2003-2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_DETAIL_FORMATTER_HPP
#define BOOST_IOFM_DETAIL_FORMATTER_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

   namespace boost { namespace io
   {
      template
      <
         typename DelimeterType,
         class    ReferenceType,
         class    FormatTraits = boost::io::detail::default_sequence_traits< DelimeterType >
      >
      class formatter_t: public openclose_formatter_t< DelimeterType, ReferenceType, FormatTraits >
      {
         public:
            typedef openclose_formatter_t< DelimeterType, ReferenceType, FormatTraits >
                                                           base_type;
            typedef typename base_type::format_type        format_type;
            typedef typename base_type::traits_type        traits_type;
         private:
            format_type                fmt_separator;
         public:
            inline format_type                   separator() const
            {
               return( fmt_separator );
            }
         public:
            inline ReferenceType &               format
                                                 (
                                                    format_type o,
                                                    format_type c,
                                                    format_type s
                                                 )
            {
               fmt_separator = s;
               return( base_type::format( o, c ));
            }
            inline ReferenceType &               format
                                                 (
                                                    format_type o,
                                                    format_type c
                                                 )
            {
               return( base_type::format( o, c ));
            }
            inline ReferenceType &               format
                                                 (
                                                    format_type s
                                                 ) 
            {
               fmt_separator = s;
               return( *static_cast< ReferenceType * >( this ));
            }

            template< typename RT2, class FormatTraits2 >
            inline ReferenceType &               format
                                                 (
                                                    const openclose_formatter_t< DelimeterType, RT2, FormatTraits2 > & fmt
                                                 )
            {
               return( base_type::format( fmt.open(), fmt.close()));
            }

            template< typename RT2, class FormatTraits2 >
            inline ReferenceType &               format
                                                 (
                                                    const formatter_t< DelimeterType, RT2, FormatTraits2 > & fmt
                                                 )
            {
               return( format( fmt.open(), fmt.close(), fmt.separator()));
            }
         public: // constructors
            inline           formatter_t():
               openclose_formatter_t< DelimeterType, ReferenceType, FormatTraits >(),
               fmt_separator( traits_type::separator_default() )
            {
            }
            inline           formatter_t
                             (
                                format_type o,
                                format_type c,
                                format_type s // = traits_type::separator_default
                             ):
               openclose_formatter_t< DelimeterType, ReferenceType, FormatTraits >( o, c ),
               fmt_separator( s )
            {
            }
            inline           formatter_t // VC workaround
                             (
                                format_type o,
                                format_type c
                             ):
               openclose_formatter_t< DelimeterType, ReferenceType, FormatTraits >( o, c ),
               fmt_separator( traits_type::separator_default())
            {
            }
            inline           formatter_t( format_type s ):
               openclose_formatter_t< DelimeterType, ReferenceType, FormatTraits >(),
               fmt_separator( s )
            {
            }
            template< typename RT2, class FormatTraits2 >
            inline           formatter_t
                             (
                                const openclose_formatter_t< DelimeterType, RT2, FormatTraits2 > & fmt,
                                format_type s // = traits_type::separator_default
                             ):
               openclose_formatter_t< DelimeterType, ReferenceType, FormatTraits >( fmt ),
               fmt_separator( s )
            {
            }
            template< typename RT2, class FormatTraits2 >
            inline           formatter_t // VC workaround
                             (
                                const openclose_formatter_t< DelimeterType, RT2, FormatTraits2 > & fmt
                             ):
               openclose_formatter_t< DelimeterType, ReferenceType, FormatTraits >( fmt ),
               fmt_separator( traits_type::separator_default())
            {
            }
            template< typename RT2, class FormatTraits2 >
            inline           formatter_t
                             (
                                const formatter_t< DelimeterType, RT2, FormatTraits2 > & fmt
                             ):
               openclose_formatter_t< DelimeterType, ReferenceType, FormatTraits >( fmt ),
               fmt_separator( fmt.separator())
            {
            }
      };

      template
      <
         typename DelimeterType,
         class FormatTraits = boost::io::detail::default_sequence_traits< DelimeterType >
      >
      class formatter: public formatter_t
                              <
                                 DelimeterType,
                                 formatter< DelimeterType, FormatTraits >,
                                 FormatTraits
                              >
      {
         public:
            typedef formatter_t< DelimeterType, openclose_formatter< DelimeterType, FormatTraits >, FormatTraits >
                                                           base_type;
            typedef typename base_type::format_type        format_type;
            typedef typename base_type::traits_type        traits_type;
         public:
            inline           formatter():
               formatter_t< DelimeterType, formatter, FormatTraits >()
            {
            }
            inline           formatter
                             (
                                format_type o,
                                format_type c,
                                format_type s // = traits_type::separator_default
                             ):
               formatter_t< DelimeterType, formatter, FormatTraits >( o, c, s )
            {
            }
            inline           formatter // VC workaround
                             (
                                format_type o,
                                format_type c
                             ):
               formatter_t< DelimeterType, formatter, FormatTraits >( o, c, traits_type::separator_default() )
            {
            }
            inline           formatter( format_type s ):
               formatter_t< DelimeterType, formatter, FormatTraits >( s )
            {
            }
            template< typename RT2, class FormatTraits2 >
            inline           formatter
                             (
                                const openclose_formatter_t< DelimeterType, RT2, FormatTraits2 > & fmt,
                                format_type s // = traits_type::separator_default
                             ):
               formatter_t< DelimeterType, formatter, FormatTraits >( fmt, s )
            {
            }
            template< typename RT2, class FormatTraits2 >
            inline           formatter // VC workaround
                             (
                                const openclose_formatter_t< DelimeterType, RT2, FormatTraits2 > & fmt
                             ):
               formatter_t< DelimeterType, formatter, FormatTraits >( fmt, traits_type::separator_default() )
            {
            }
#           if !defined(BOOST_MSVC) || _MSC_VER > 1200
               template< typename RT2, class FormatTraits2 >
               inline        formatter
                             (
                                const formatter_t< DelimeterType, RT2, FormatTraits2 > & fmt
                             ):
                  formatter_t< DelimeterType, formatter, FormatTraits >( fmt )
               {
               }
#           endif
      };
   }}
#endif
