// (C) Copyright 2003: Reece H. Dunn

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#ifndef BOOST_IOFM_DETAIL_FORMATTER_HPP
#define BOOST_IOFM_DETAIL_FORMATTER_HPP
   namespace boost { namespace io
   {
      template
      <
         typename FormatType,
         class    ReferenceType,
         class    FormatTraits = boost::io::detail::format_traits< FormatType >
      >
      class formatter_t: public openclose_formatter_t< FormatType, ReferenceType, FormatTraits >
      {
         public:
            typedef openclose_formatter_t< FormatType, ReferenceType, FormatTraits >
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
                                                    const openclose_formatter_t< FormatType, RT2, FormatTraits2 > & fmt
                                                 )
            {
               return( base_type::format( fmt.open(), fmt.close()));
            }

            template< typename RT2, class FormatTraits2 >
            inline ReferenceType &               format
                                                 (
                                                    const formatter_t< FormatType, RT2, FormatTraits2 > & fmt
                                                 )
            {
               return( format( fmt.open(), fmt.close(), fmt.separator()));
            }
         public: // constructors
            inline           formatter_t():
               openclose_formatter_t< FormatType, ReferenceType, FormatTraits >(),
               fmt_separator( traits_type::separator_default )
            {
            }
            inline           formatter_t
                             (
                                format_type o,
                                format_type c,
                                format_type s
#                               if !defined(BOOST_IOFM_NO_FMT_DEFAULT)
                                   = traits_type::separator_default
#                               endif
                             ):
               openclose_formatter_t< FormatType, ReferenceType, FormatTraits >( o, c ),
               fmt_separator( s )
            {
            }
            inline           formatter_t( format_type s ):
               openclose_formatter_t< FormatType, ReferenceType, FormatTraits >(),
               fmt_separator( s )
            {
            }
            template< typename RT2, class FormatTraits2 >
            inline           formatter_t
                             (
                                const openclose_formatter_t< FormatType, RT2, FormatTraits2 > & fmt,
                                format_type s
#                               if !defined(BOOST_IOFM_NO_FMT_DEFAULT)
                                   = traits_type::separator_default
#                               endif
                             ):
               openclose_formatter_t< FormatType, ReferenceType, FormatTraits >( fmt ),
               fmt_separator( s )
            {
            }
            template< typename RT2, class FormatTraits2 >
            inline           formatter_t
                             (
                                const formatter_t< FormatType, RT2, FormatTraits2 > & fmt
                             ):
               openclose_formatter_t< FormatType, ReferenceType, FormatTraits >( fmt ),
               fmt_separator( fmt.separator())
            {
            }
      };

      template
      <
         typename FormatType,
         class FormatTraits = boost::io::detail::format_traits< FormatType >
      >
      class formatter: public formatter_t
                              <
                                 FormatType,
                                 formatter< FormatType, FormatTraits >,
                                 FormatTraits
                              >
      {
         public:
            typedef formatter_t< FormatType, openclose_formatter< FormatType, FormatTraits >, FormatTraits >
                                                           base_type;
            typedef typename base_type::format_type        format_type;
            typedef typename base_type::traits_type        traits_type;
         public:
            inline           formatter():
               formatter_t< FormatType, formatter, FormatTraits >()
            {
            }
            inline           formatter
                             (
                                format_type o,
                                format_type c,
                                format_type s
#                               if !defined(BOOST_IOFM_NO_FMT_DEFAULT)
                                   = traits_type::separator_default
#                               endif
                             ):
               formatter_t< FormatType, formatter, FormatTraits >( o, c, s )
            {
            }
            inline           formatter( format_type s ):
               formatter_t< FormatType, formatter, FormatTraits >( s )
            {
            }
            template< typename RT2, class FormatTraits2 >
            inline           formatter
                             (
                                const openclose_formatter_t< FormatType, RT2, FormatTraits2 > & fmt,
                                format_type s
#                               if !defined(BOOST_IOFM_NO_FMT_DEFAULT)
                                   = traits_type::separator_default
#                               endif
                             ):
               formatter_t< FormatType, formatter, FormatTraits >( fmt, s )
            {
            }
#           if !defined(BOOST_MSVC) || _MSC_VER > 1200
               template< typename RT2, class FormatTraits2 >
               inline        formatter
                             (
                                const formatter_t< FormatType, RT2, FormatTraits2 > & fmt
                             ):
                  formatter_t< FormatType, formatter, FormatTraits >( fmt )
               {
               }
#           endif
      };
   }}
#endif
