// (C) Copyright 2003: Reece H. Dunn 
// (C) Copyright Jonathan Turkanis 2003.

#ifndef BOOST__IOFM__DETAIL__FORMAT_TRAITS__HPP
#define BOOST__IOFM__DETAIL__FORMAT_TRAITS__HPP
#  include <boost/outfmt/detail/config.hpp>
#  include <boost/outfmt/detail/type_traits.hpp>
#  include <boost/mpl/if.hpp>

   namespace boost { namespace io { namespace detail
   {
      // RHD: format_traits_base - raw character string traits

      template< typename T >
      struct format_traits_base     {};

      // formatting for characters:

      template<>
      struct format_traits_base< char >
      {
         static const char             open_default;
         static const char             close_default;
         static const char             separator_default;
      };

      const char format_traits_base< char >::open_default      = '[';
      const char format_traits_base< char >::close_default     = ']';
      const char format_traits_base< char >::separator_default = ',';

      // formatting for wide characters:

      template<>
      struct format_traits_base< wchar_t >
      {
         static const wchar_t          open_default;
         static const wchar_t          close_default;
         static const wchar_t          separator_default;
      };

      const wchar_t format_traits_base< wchar_t >::open_default      = L'[';
      const wchar_t format_traits_base< wchar_t >::close_default     = L']';
      const wchar_t format_traits_base< wchar_t >::separator_default = L',';

      // formatting for strings:

      template<>
      struct format_traits_base< char * >
      {
         static char *                 open_default;
         static char *                 close_default;
         static char *                 separator_default;
      };

      char * format_traits_base< char * >::open_default      = "[ ";
      char * format_traits_base< char * >::close_default     = " ]";
      char * format_traits_base< char * >::separator_default = ", ";

      template<>
      struct format_traits_base< const char * >: public format_traits_base< char * >
      {
      };

      // formatting for wide strings:

      template<>
      struct format_traits_base< wchar_t * >
      {
         static wchar_t *              open_default;
         static wchar_t *              close_default;
         static wchar_t *              separator_default;
      };

      wchar_t * format_traits_base< wchar_t * >::open_default      = L"[ ";
      wchar_t * format_traits_base< wchar_t * >::close_default     = L" ]";
      wchar_t * format_traits_base< wchar_t * >::separator_default = L", ";

      template<>
      struct format_traits_base< const wchar_t * >: public format_traits_base< wchar_t * >
      {
      };

      // JDT: string_format_traits

      template< typename String >
      struct string_format_traits
      {
         typedef typename String::char_type                          char_type;
         typedef typename String::traits_type                        traits_type;

         static String                 open_default;
         static String                 close_default;
         static String                 separator_default;
      };

      template< typename String >
      String string_format_traits< String >::open_default =
         String( format_traits_base< typename String::char_type * >::open_default );

      template< typename String >
      String string_format_traits< String >::close_default =
         String( format_traits_base< typename String::char_type * >::close_default );

      template<typename String>
      String string_format_traits< String >::separator_default =
         String( format_traits_base< typename String::char_type * >::separator_default );

      // JDT: format_traits

      template
      <
         typename T,
         typename Base = // VC6 Workaround (error C2516)
            BOOST_DEDUCED_TYPENAME
            mpl::apply_if
            <
               is_std_string< T >,
               mpl::identity< string_format_traits< T > >,
               mpl::identity< format_traits_base< T > >
            >::type
      >
      struct format_traits: public Base
      {
      };          
   }}}
#endif
