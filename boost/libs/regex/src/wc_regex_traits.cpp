/*
 *
 * Copyright (c) 2004
 * Dr John Maddock
 *
 * Use, modification and distribution are subject to the 
 * Boost Software License, Version 1.0. (See accompanying file 
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */

 /*
  *   LOCATION:    see http://www.boost.org for most recent version.
  *   FILE:        wc_regex_traits.cpp
  *   VERSION:     see <boost/version.hpp>
  *   DESCRIPTION: Implements out of line members for c_regex_traits<wchar_t>
  */


#define BOOST_REGEX_SOURCE

#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>

#if !BOOST_WORKAROUND(__BORLANDC__, < 0x560)

#include <boost/regex/v4/c_regex_traits.hpp>
#ifndef BOOST_NO_WREGEX
#include <boost/regex/v4/primary_transform.hpp>
#include <boost/regex/v4/regex_traits_defaults.hpp>

#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std{
   using ::wcstol;
}
#endif

namespace boost{

c_regex_traits<wchar_t>::string_type BOOST_REGEX_CALL c_regex_traits<wchar_t>::transform(const wchar_t* p1, const wchar_t* p2) const
{ 
   std::size_t r;
   std::size_t s = 10;
   std::wstring src(p1, p2);
   std::wstring result(s, L' ');
   while(s < (r = std::wcsxfrm(&*result.begin(), src.c_str(), result.size())))
   {
      result.append(r - s + 3, L' ');
   }
   result.erase(r);
   return result; 
}

c_regex_traits<wchar_t>::string_type BOOST_REGEX_CALL c_regex_traits<wchar_t>::transform_primary(const wchar_t* p1, const wchar_t* p2) const
{
   static wchar_t s_delim;
   static const int s_collate_type = ::boost::re_detail::find_sort_syntax(this, &s_delim);
   std::wstring result;
   //
   // What we do here depends upon the format of the sort key returned by
   // sort key returned by this->transform:
   //
   switch(s_collate_type)
   {
   case ::boost::re_detail::sort_C:
   case ::boost::re_detail::sort_unknown:
      // the best we can do is translate to lower case, then get a regular sort key:
      {
         result.assign(p1, p2);
         for(std::wstring::size_type i = 0; i < result.size(); ++i)
            result[i] = (std::towlower)(result[i]);
         result = this->transform(&*result.begin(), &*result.begin() + result.size());
         break;
      }
   case ::boost::re_detail::sort_fixed:
      {
         // get a regular sort key, and then truncate it:
         result = this->transform(&*result.begin(), &*result.begin() + result.size());
         result.erase(s_delim);
         break;
      }
   case ::boost::re_detail::sort_delim:
         // get a regular sort key, and then truncate everything after the delim:
         result = this->transform(&*result.begin(), &*result.begin() + result.size());
         std::size_t i;
         for(i = 0; i < result.size(); ++i)
         {
            if(result[i] == s_delim)
               break;
         }
         result.erase(i);
         break;
   }
   return result;
}

enum
{
   char_class_space=1<<0, 
   char_class_print=1<<1, 
   char_class_cntrl=1<<2, 
   char_class_upper=1<<3, 
   char_class_lower=1<<4,
   char_class_alpha=1<<5, 
   char_class_digit=1<<6, 
   char_class_punct=1<<7, 
   char_class_xdigit=1<<8,
   char_class_alnum=char_class_alpha|char_class_digit, 
   char_class_graph=char_class_alnum|char_class_punct,
   char_class_blank=1<<9,
   char_class_word=1<<10,
   char_class_unicode=1<<11
};

c_regex_traits<wchar_t>::char_class_type BOOST_REGEX_CALL c_regex_traits<wchar_t>::lookup_classname(const wchar_t* p1, const wchar_t* p2) const
{
   static const char_class_type masks[] = 
   {
      0,
      char_class_alnum, 
      char_class_alpha,
      char_class_blank,
      char_class_cntrl,
      char_class_digit,
      char_class_digit,
      char_class_graph,
      char_class_lower,
      char_class_lower,
      char_class_print,
      char_class_punct,
      char_class_space,
      char_class_space,
      char_class_upper,
      char_class_unicode,
      char_class_upper,
      char_class_alnum | char_class_word, 
      char_class_alnum | char_class_word, 
      char_class_xdigit,
   };

   int id = ::boost::re_detail::get_default_class_id(p1, p2);
   if(id < 0)
   {
      std::wstring s(p1, p2);
      for(std::wstring::size_type i = 0; i < s.size(); ++i)
         s[i] = (std::towlower)(s[i]);
      id = ::boost::re_detail::get_default_class_id(&*s.begin(), &*s.begin() + s.size());
   }
   BOOST_ASSERT(id+1 < sizeof(masks) / sizeof(masks[0]));
   return masks[id+1];
}

bool BOOST_REGEX_CALL c_regex_traits<wchar_t>::isctype(wchar_t c, char_class_type mask) const
{
   return
      ((mask & char_class_space) && (std::iswspace)(c))
      || ((mask & char_class_print) && (std::iswprint)(c))
      || ((mask & char_class_cntrl) && (std::iswcntrl)(c))
      || ((mask & char_class_upper) && (std::iswupper)(c))
      || ((mask & char_class_lower) && (std::iswlower)(c))
      || ((mask & char_class_alpha) && (std::iswalpha)(c))
      || ((mask & char_class_digit) && (std::iswdigit)(c))
      || ((mask & char_class_punct) && (std::iswpunct)(c))
      || ((mask & char_class_xdigit) && (std::iswxdigit)(c))
      || ((mask & char_class_blank) && (std::iswspace)(c) && !::boost::re_detail::is_separator(c))
      || ((mask & char_class_word) && (c == '_'))
      || ((mask & char_class_unicode) && (c & ~static_cast<wchar_t>(0xff)));
}

c_regex_traits<wchar_t>::string_type BOOST_REGEX_CALL c_regex_traits<wchar_t>::lookup_collatename(const wchar_t* p1, const wchar_t* p2) const
{
#if !defined(BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS) && !BOOST_WORKAROUND(BOOST_MSVC, < 1300)
   std::string name(p1, p2);
#else
   std::string name;
   const charT* p0 = p1;
   while(p0 != p2)
	   name.append(1, char(*p0++));
#endif
   name = ::boost::re_detail::lookup_default_collate_name(name);
#if !defined(BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS) && !BOOST_WORKAROUND(BOOST_MSVC, < 1300)
   if(name.size())
      return string_type(name.begin(), name.end());
#else
   if(name.size())
   {
	   string_type result;
	   typedef std::string::const_iterator iter;
	   iter b = name.begin();
	   iter e = name.end();
	   while(b != e)
		   result.append(1, charT(*b++));
	   return result;
   }
#endif
   if(p2 - p1 == 1)
      return string_type(1, *p1);
   return string_type();
}

int BOOST_REGEX_CALL c_regex_traits<wchar_t>::value(wchar_t c, int radix) const
{
#ifdef __BORLANDC__
   // workaround for broken wcstol:
   if((std::iswxdigit)(c) == 0)
      return -1;
#endif
   wchar_t b[2] = { c, '\0', };
   wchar_t* ep;
   int result = std::wcstol(b, &ep, radix);
   if(ep == b)
      return -1;
   return result;
}

}

#endif // BOOST_NO_WREGEX

#endif // __BORLANDC__

