// (C) Copyright 2003-2004: Reece H. Dunn; Jonathan Turkanis 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_DETAIL_FormatTraits_HPP
#define BOOST_IOFM_DETAIL_FormatTraits_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <boost/outfmt/detail/type_traits.hpp>

   namespace boost { namespace io { namespace detail
   {
      // RHD: format_traits_base - raw character string traits

      template< typename T >
      struct format_traits_base     {};

      // formatting for characters:

      template<>
      struct format_traits_base< char >
      {
         static char open_default() { return '['; }
         static char close_default() { return ']'; }
         static char separator_default() { return ','; }
      };

      // formatting for wide characters:

      template<>
      struct format_traits_base< wchar_t >
      {
         static wchar_t open_default() { return L'['; }
         static wchar_t close_default() { return L']'; }
         static wchar_t separator_default() { return L','; }
      };

      // formatting for strings:

      template<>
      struct format_traits_base< const char * >
      {  
         static const char* open_default() { return "[ "; }
         static const char* close_default() { return " ]"; }
         static const char* separator_default() { return ", "; }
      };

      // formatting for wide strings:

      template<>
      struct format_traits_base< const wchar_t * >
      {
         static const wchar_t* open_default() { return L"[ "; }
         static const wchar_t* close_default() { return L" ]"; }
         static const wchar_t* separator_default() { return L", "; }
      };

      // JDT: string_format_traits

      template< typename String >
      struct string_format_traits
      {
         typedef typename String::char_type                          char_type;
         typedef typename String::traits_type                        traits_type;

         static String open_default();      // Return by reference raises thread-safety
         static String close_default();     // issues. JDT.
         static String separator_default();
      };

      template< typename String >
      String string_format_traits< String >::open_default()
      { return format_traits_base< typename String::char_type * >::open_default(); }

      template< typename String >
      String string_format_traits< String >::close_default()
      { return format_traits_base< typename String::char_type * >::close_default(); }

      template<typename String>
      String string_format_traits< String >::separator_default()
      { return format_traits_base< typename String::char_type * >::separator_default(); }

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
      struct default_sequence_traits: public Base
      {
      };          
   }}}
#endif
