// (C) Copyright 2004: Reece H. Dunn; Jonathan Turkanis

#ifndef BOOST_IOFM_DETAIL_StringTraits_HPP
#define BOOST_IOFM_DETAIL_StringTraits_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <boost/outfmt/detail/format_traits.hpp>

   namespace boost { namespace io { namespace detail
   {
      // RHD: string_traits_base - raw character string traits

      template< typename T >
      struct string_traits_base     {};

      // formatting for characters:

      template<>
      struct string_traits_base< char >
      {
         static const char             open_default;
         static const char             close_default;
         static const char             separator_default;
      };

      const char string_traits_base< char >::open_default      = '"';
      const char string_traits_base< char >::close_default     = '"';

      // formatting for wide characters:

      template<>
      struct string_traits_base< wchar_t >
      {
         static const wchar_t          open_default;
         static const wchar_t          close_default;
      };

      const wchar_t string_traits_base< wchar_t >::open_default      = L'"';
      const wchar_t string_traits_base< wchar_t >::close_default     = L'"';

      // formatting for strings:

      template<>
      struct string_traits_base< char * >
      {
         static char *                 open_default;
         static char *                 close_default;
      };

      char * string_traits_base< char * >::open_default      = "\"";
      char * string_traits_base< char * >::close_default     = "\"";

      template<>
      struct string_traits_base< const char * >: public string_traits_base< char * >
      {
      };

      // formatting for wide strings:

      template<>
      struct string_traits_base< wchar_t * >
      {
         static wchar_t *              open_default;
         static wchar_t *              close_default;
      };

      wchar_t * string_traits_base< wchar_t * >::open_default  = L"\"";
      wchar_t * string_traits_base< wchar_t * >::close_default = L"\"";

      template<>
      struct string_traits_base< const wchar_t * >: public string_traits_base< wchar_t * >
      {
      };

      // JDT: string_string_traits

      template< typename String >
      struct string_string_traits
      {
         typedef typename String::char_type                          char_type;
         typedef typename String::traits_type                        traits_type;

         static String                 open_default;
         static String                 close_default;
      };

      template< typename String >
      String string_string_traits< String >::open_default =
         String( string_traits_base< typename String::char_type * >::open_default );

      template< typename String >
      String string_string_traits< String >::close_default =
         String( string_traits_base< typename String::char_type * >::close_default );

      // JDT: string_traits

      template
      <
         typename T,
         typename Base = // VC6 Workaround (error C2516)
            BOOST_DEDUCED_TYPENAME
            mpl::apply_if
            <
               is_std_string< T >,
               mpl::identity< string_string_traits< T > >,
               mpl::identity< string_traits_base< T > >
            >::type
      >
      struct string_traits: public Base
      {
      };          
   }}}
#endif
