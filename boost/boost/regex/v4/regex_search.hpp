/*
 *
 * Copyright (c) 1998-2002
 * Dr John Maddock
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Dr John Maddock makes no representations
 * about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 *
 */

 /*
  *   LOCATION:    see http://www.boost.org for most recent version.
  *   FILE         regex_search.hpp
  *   VERSION      see <boost/version.hpp>
  *   DESCRIPTION: Provides regex_search implementation.
  */

#ifndef BOOST_REGEX_V4_REGEX_SEARCH_HPP
#define BOOST_REGEX_V4_REGEX_SEARCH_HPP


namespace boost{

#ifdef __BORLANDC__
   #pragma option push -a8 -b -Vx -Ve -pc  -w-8037
#endif

template <class BidiIterator, class Allocator, class charT, class traits, class Allocator2>
bool regex_search(BidiIterator first, BidiIterator last, 
                  match_results<BidiIterator, Allocator>& m, 
                  const reg_expression<charT, traits, Allocator2>& e, 
                  match_flag_type flags = match_default)
{
   if(e.flags() & regbase::failbit)
      return false;

   re_detail::perl_matcher<BidiIterator, Allocator, traits, Allocator2> matcher(first, last, m, e, flags);
   return matcher.find();
}

//
// regex_search convenience interfaces:
#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
//
// this isn't really a partial specialisation, but template function
// overloading - if the compiler doesn't support partial specialisation
// then it really won't support this either:
template <class charT, class Allocator, class traits, class Allocator2>
inline bool regex_search(const charT* str, 
                        match_results<const charT*, Allocator>& m, 
                        const reg_expression<charT, traits, Allocator2>& e, 
                        match_flag_type flags = match_default)
{
   return regex_search(str, str + traits::length(str), m, e, flags);
}

template <class ST, class SA, class Allocator, class charT, class traits, class Allocator2>
inline bool regex_search(const std::basic_string<charT, ST, SA>& s, 
                 match_results<typename std::basic_string<charT, ST, SA>::const_iterator, Allocator>& m, 
                 const reg_expression<charT, traits, Allocator2>& e, 
                 match_flag_type flags = match_default)
{
   return regex_search(s.begin(), s.end(), m, e, flags);
}
#else  // partial specialisation
inline bool regex_search(const char* str, 
                        cmatch& m, 
                        const regex& e, 
                        match_flag_type flags = match_default)
{
   return regex_search(str, str + regex::traits_type::length(str), m, e, flags);
}
#ifndef BOOST_NO_WREGEX
inline bool regex_search(const wchar_t* str, 
                        wcmatch& m, 
                        const wregex& e, 
                        match_flag_type flags = match_default)
{
   return regex_search(str, str + wregex::traits_type::length(str), m, e, flags);
}
#endif
inline bool regex_search(const std::string& s, 
                        smatch& m,
                        const regex& e, 
                        match_flag_type flags = match_default)
{
   return regex_search(s.begin(), s.end(), m, e, flags);
}
#if !defined(BOOST_NO_WREGEX)
inline bool regex_search(const std::basic_string<wchar_t>& s, 
                        wsmatch& m,
                        const wregex& e, 
                        match_flag_type flags = match_default)
{
   return regex_search(s.begin(), s.end(), m, e, flags);
}
#endif

#endif

#ifdef __BORLANDC__
  #pragma option pop
#endif

} // namespace boost

#endif  // BOOST_REGEX_V4_REGEX_SEARCH_HPP

