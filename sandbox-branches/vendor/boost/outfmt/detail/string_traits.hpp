// (C) Copyright 2004: Reece H. Dunn; Jonathan Turkanis
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

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
         static const char open_default() { return '"'; }
         static const char close_default() { return '"'; }
      };

      // formatting for wide characters:

      template<>
      struct string_traits_base< wchar_t >
      {
         static const wchar_t open_default() { return L'"'; }
         static const wchar_t close_default() { return L'"'; }
      };

      // formatting for strings:

      template<>
      struct string_traits_base< const char * >
      {
         static const char* open_default() { return "\""; }
         static const char* close_default() { return "\""; }
      };

      // formatting for wide strings:

      template<>
      struct string_traits_base< const wchar_t * >
      {
         static const wchar_t* open_default() { return L"\""; }
         static const wchar_t* close_default() { return L"\""; }
      };

      // JDT: string_string_traits

      template< typename String >
      struct string_string_traits
      {
         typedef typename String::char_type                          char_type;
         typedef typename String::traits_type                        traits_type;

         static String open_default();
         static String close_default();
      };

      template< typename String >
      String string_string_traits< String >::open_default() 
      { return string_traits_base< typename String::char_type * >::open_default(); }

      template< typename String >
      String string_string_traits< String >::close_default()
      { return string_traits_base< typename String::char_type * >::close_default(); }

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
      struct default_string_traits: public Base
      {
      };          
   }}}
#endif
