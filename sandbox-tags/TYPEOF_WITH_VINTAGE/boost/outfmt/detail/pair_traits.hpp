// (C) Copyright 2003-2004: Reece H. Dunn; Jonathan Turkanis 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_DETAIL_PairTraits_HPP
#define BOOST_IOFM_DETAIL_PairTraits_HPP
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <boost/outfmt/detail/type_traits.hpp>

   namespace boost { namespace io { namespace detail
   {
      // RHD: pair_traits_base - raw character string traits

      template< typename T >
      struct pair_traits_base       {};

      // formatting for characters:

      template<>
      struct pair_traits_base< char >
      {
         static char open_default() { return '('; }
         static char close_default() { return ')'; }
         static char separator_default() { return ','; }
      };

      // formatting for wide characters:

      template<>
      struct pair_traits_base< wchar_t >
      {
         static wchar_t open_default() { return L'('; }
         static wchar_t close_default() { return L')'; }
         static wchar_t separator_default() { return L','; }
      };

      // formatting for strings:

      template<>
      struct pair_traits_base< const char * >
      {
         static const char* open_default() { return "( "; }
         static const char* close_default() { return " )"; }
         static const char* separator_default() { return ", "; }
      };

      // formatting for wide strings:

      template<>
      struct pair_traits_base< const wchar_t * >
      {
         static const wchar_t* open_default() { return L"( "; }
         static const wchar_t* close_default() { return L" )"; }
         static const wchar_t* separator_default() { return L", "; }
      };

      // JDT: string_string_traits

      template< typename String >
      struct string_pair_traits
      {
         typedef typename String::char_type                          char_type;
         typedef typename String::traits_type                        traits_type;

         static String                 open_default();
         static String                 close_default();
         static String                 separator_default();
      };

      template< typename String >
      String string_pair_traits< String >::open_default()
      { return pair_traits_base< typename String::char_type * >::open_default(); }

      template< typename String >
      String string_pair_traits< String >::close_default()
      { return pair_traits_base< typename String::char_type * >::close_default(); }

      template<typename String>
      String string_pair_traits< String >::separator_default()
      { return pair_traits_base< typename String::char_type * >::separator_default(); }

      // JDT: pair_traits

      template
      <
         typename T,
         typename Base = // VC6 Workaround (error C2516)
            BOOST_DEDUCED_TYPENAME
            mpl::apply_if
            <
               is_std_string< T >,
               mpl::identity< string_pair_traits< T > >,
               mpl::identity< pair_traits_base< T > >
            >::type
      >
      struct default_nary_traits: public Base
      {
      };          
   }}}
#endif
