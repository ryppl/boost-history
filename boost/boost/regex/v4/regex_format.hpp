/*
 *
 * Copyright (c) 1998-2002
 * Dr John Maddock
 *
 * Use, modification and distribution are subject to the 
 * Boost Software License, Version 1.0. (See accompanying file 
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */

 /*
  *   LOCATION:    see http://www.boost.org for most recent version.
  *   FILE         regex_format.hpp
  *   VERSION      see <boost/version.hpp>
  *   DESCRIPTION: Provides formatting output routines for search and replace
  *                operations.  Note this is an internal header file included
  *                by regex.hpp, do not include on its own.
  */

#ifndef BOOST_REGEX_FORMAT_HPP
#define BOOST_REGEX_FORMAT_HPP


namespace boost{

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

//
// Forward declaration:
//
template <class BidiIterator, class Allocator = BOOST_DEFAULT_ALLOCATOR(sub_match<BidiIterator> ) >
class match_results;

namespace re_detail{

//
// helper functions:
//
template <class charT>
std::ptrdiff_t global_length(const charT* p)
{
   std::ptrdiff_t n = 0;
   while(*p)
   {
      ++p;
      ++n;
   }
   return n;
}
inline std::ptrdiff_t global_length(const char* p)
{
   return (std::strlen)(p);
}
#ifndef BOOST_NO_WREGEX
inline std::ptrdiff_t global_length(const wchar_t* p)
{
   return (std::wcslen)(p);
}
#endif
template <class charT>
inline charT global_lower(charT c)
{
   return c;
}
template <class charT>
inline charT global_upper(charT c)
{
   return c;
}
BOOST_REGEX_DECL char BOOST_REGEX_CALL global_lower(char c);
BOOST_REGEX_DECL char BOOST_REGEX_CALL global_upper(char c);
#ifndef BOOST_NO_WREGEX
BOOST_REGEX_DECL wchar_t BOOST_REGEX_CALL global_lower(wchar_t c);
BOOST_REGEX_DECL wchar_t BOOST_REGEX_CALL global_upper(wchar_t c);
#endif
template <class charT>
int global_value(charT c)
{
   static const charT zero = '0';
   static const charT nine = '9';
   static const charT a = 'a';
   static const charT f = 'f';
   static const charT A = 'A';
   static const charT F = 'F';

   if((c >= zero) && (c <= nine))
      return c - zero;
   if((c >= a) && (c <= f))
      return 10 + (c - a);
   if((c >= A) && (c <= F))
      return 10 + (c - A);
   return -1;
}
template <class charT>
int global_toi(const charT*& p1, const charT* p2, int radix)
{
   int next_value = global_value(*p1);
   if((p1 == p2) || (next_value < 0) || (next_value >= radix))
      return -1;
   int result = 0;
   while(p1 != p2)
   {
      next_value = global_value(*p1);
      if((next_value < 0) || (next_value >= radix))
         break;
      result *= radix;
      result += next_value;
      ++p1;
   }
   return result;
}


//
// struct trivial_format_traits:
// defines minimum localisation support for formatting
// in the case that the actual regex traits is unavailable.
//
template <class charT>
struct trivial_format_traits
{
   typedef charT char_type;

   static std::ptrdiff_t length(const charT* p)
   {
      return global_length(p);
   }
   static charT tolower(charT c)
   {
      return ::boost::re_detail::global_lower(c);
   }
   static charT toupper(charT c)
   {
      return ::boost::re_detail::global_upper(c);
   }
   static int toi(const charT*& p1, const charT* p2, int radix)
   {
      return global_toi(p1, p2, radix);
   }
};

template <class OutputIterator, class Results, class traits>
class basic_regex_formatter
{
public:
   typedef typename traits::char_type char_type;
   basic_regex_formatter(OutputIterator o, const Results& r, const traits& t)
      : m_traits(t), m_results(r), m_out(o), m_state(output_copy) {}
   OutputIterator format(const char_type* p1, const char_type* p2, match_flag_type f);
   OutputIterator format(const char_type* p1, match_flag_type f)
   {
      return format(p1, p1 + m_traits.length(p1), f);
   }
private:
   typedef typename Results::value_type sub_match_type;
   enum output_state
   {
      output_copy,
      output_next_lower,
      output_next_upper,
      output_lower,
      output_upper,
      output_none
   };

   void put(char_type c);
   void put(const sub_match_type& sub);
   void format_all();
   void format_perl();
   void format_escape();
   void format_conditional();
   void format_until_scope_end();

   const traits& m_traits;       // the traits class for localised formatting operations
   const Results& m_results;     // the match_results being used.
   OutputIterator m_out;         // where to send output.
   const char_type* m_position;  // format string, current position
   const char_type* m_end;       // format string end
   match_flag_type m_flags;      // format flags to use
   output_state    m_state;      // what to do with the next character
};

template <class OutputIterator, class Results, class traits>
OutputIterator basic_regex_formatter<OutputIterator, Results, traits>::format(const char_type* p1, const char_type* p2, match_flag_type f)
{
   m_position = p1;
   m_end = p2;
   m_flags = f;
   format_all();
   return m_out;
}

template <class OutputIterator, class Results, class traits>
void basic_regex_formatter<OutputIterator, Results, traits>::format_all()
{
   // over and over:
   while(m_position != m_end)
   {
      switch(*m_position)
      {
      case '&':
         if(m_flags & ::boost::regex_constants::format_sed)
         {
            ++m_position;
            put(m_results[0]);
            break;
         }
         put(*m_position++);
         break;
      case '\\':
         format_escape();
         break;
      case '(':
         if(m_flags & boost::regex_constants::format_all)
         {
            ++m_position;
            format_until_scope_end();
            BOOST_ASSERT(*m_position == static_cast<char_type>(')'));
            ++m_position;  // skip the closing ')'
            break;
         }
         put(*m_position);
         ++m_position;
         break;
      case ')':
      case ':':
         if(m_flags & boost::regex_constants::format_all)
         {
            return;
         }
         put(*m_position);
         ++m_position;
         break;
      case '?':
         if(m_flags & boost::regex_constants::format_all)
         {
            ++m_position;
            format_conditional();
            break;
         }
         put(*m_position);
         ++m_position;
         break;
      case '$':
         if((m_flags & format_sed) == 0)
         {
            format_perl();
            break;
         }
         // fall through, not a special character:
      default:
         put(*m_position);
         ++m_position;
         break;
      }
   }
}

template <class OutputIterator, class Results, class traits>
void basic_regex_formatter<OutputIterator, Results, traits>::format_perl()
{
   //
   // On entry *m_position points to a '$' character
   // output the information that goes with it:
   //
   BOOST_ASSERT(*m_position == '$');
   //
   // see if this is a trailing '$':
   //
   if(++m_position == m_end)
   {
      --m_position;
      put(*m_position);
      ++m_position;
      return;
   }
   //
   // OK find out what kind it is:
   //
   switch(*m_position)
   {
   case '&':
      ++m_position;
      put(this->m_results[0]);
      break;
   case '`':
      ++m_position;
      put(this->m_results.prefix());
      break;
   case '\'':
      ++m_position;
      put(this->m_results.suffix());
      break;
   case '$':
      put(*m_position++);
      break;
   default:
      // see if we have a number:
      {
         std::ptrdiff_t len = (std::min)(static_cast<std::ptrdiff_t>(2), std::distance(m_position, m_end));
         int v = m_traits.toi(m_position, m_position + len, 10);
         if(v < 0)
         {
            // leave the $ as is, and carry on:
            --m_position;
            put(*m_position);
            ++m_position;
            break;
         }
         // otherwise output sub v:
         put(this->m_results[v]);
      }
   }
}

template <class OutputIterator, class Results, class traits>
void basic_regex_formatter<OutputIterator, Results, traits>::format_escape()
{
   // skip the escape and check for trailing escape:
   if(++m_position == m_end)
   {
      put(static_cast<char_type>('\\'));
      return;
   }
   // now switch on the escape type:
   switch(*m_position)
   {
   case 'a':
      put(static_cast<char_type>('\a'));
      ++m_position;
      break;
   case 'f':
      put(static_cast<char_type>('\f'));
      ++m_position;
      break;
   case 'n':
      put(static_cast<char_type>('\n'));
      ++m_position;
      break;
   case 'r':
      put(static_cast<char_type>('\r'));
      ++m_position;
      break;
   case 't':
      put(static_cast<char_type>('\t'));
      ++m_position;
      break;
   case 'v':
      put(static_cast<char_type>('\v'));
      ++m_position;
      break;
   case 'x':
      if(++m_position == m_end)
      {
         put(static_cast<char_type>('x'));
         return;
      }
      // maybe have \x{ddd}
      if(*m_position == static_cast<char_type>('{'))
      {
         ++m_position;
         int val = m_traits.toi(m_position, m_end, 16);
         if(val < 0)
         {
            // invalid value treat everything as literals:
            put(static_cast<char_type>('x'));
            put(static_cast<char_type>('{'));
            return;
         }
         if(*m_position != static_cast<char_type>('}'))
         {
            while(*m_position != static_cast<char_type>('\\'))
               --m_position;
            ++m_position;
            put(*m_position++);
            return;
         }
         ++m_position;
         put(static_cast<char_type>(val));
         return;
      }
      else
      {
         std::ptrdiff_t len = (std::min)(static_cast<std::ptrdiff_t>(2), std::distance(m_position, m_end));
         int val = m_traits.toi(m_position, m_position + len, 16);
         if(val < 0)
         {
            --m_position;
            put(*m_position++);
            return;
         }
         put(static_cast<char_type>(val));
      }
      break;
   case 'c':
      if(++m_position == m_end)
      {
         --m_position;
         put(*m_position++);
         return;
      }
      put(static_cast<char_type>(*m_position++ % 32));
      break;
   case 'e':
      put(static_cast<char_type>(27));
      ++m_position;
      break;
   default:
      // see if we have a \n sed style backreference:
      int v = m_traits.toi(m_position, m_position+1, 10);
      if((v > 0) || ((v == 0) && (m_flags & ::boost::regex_constants::format_sed)))
      {
         put(m_results[v]);
         break;
      }
      else if(v == 0)
      {
         // octal ecape sequence:
         --m_position;
         std::ptrdiff_t len = (std::min)(static_cast<std::ptrdiff_t>(4), std::distance(m_position, m_end));
         v = m_traits.toi(m_position, m_position + len, 8);
         BOOST_ASSERT(v >= 0);
         put(static_cast<char_type>(v));
         break;
      }
      // Otherwise output the character "as is":
      put(*m_position++);
      break;
   }
}

template <class OutputIterator, class Results, class traits>
void basic_regex_formatter<OutputIterator, Results, traits>::format_conditional()
{
   if(m_position == m_end)
   {
      // oops trailing '?':
      put(static_cast<char_type>('?'));
      return;
   }
   std::ptrdiff_t len = (std::min)(static_cast<std::ptrdiff_t>(2), std::distance(m_position, m_end));
   int v = m_traits.toi(m_position, m_position + len, 10);
   if(v < 0)
   {
      // oops not a number:
      put(static_cast<char_type>('?'));
      return;
   }

   // output varies depending upon whether sub-expression v matched or not:
   if(m_results[v].matched)
   {
      format_all();
      if((m_position != m_end) && (*m_position == static_cast<char_type>(':')))
      {
         // skip the ':':
         ++m_position;
         // save output state, then turn it off:
         output_state saved_state = m_state;
         m_state = output_none;
         // format the rest of this scope:
         format_until_scope_end();
         // restore output state:
         m_state = saved_state;
      }
   }
   else
   {
      // save output state, then turn it off:
      output_state saved_state = m_state;
      m_state = output_none;
      // format until ':' or ')':
      format_all();
      // restore state:
      m_state = saved_state;
      if((m_position != m_end) && (*m_position == static_cast<char_type>(':')))
      {
         // skip the ':':
         ++m_position;
         // format the rest of this scope:
         format_until_scope_end();
      }
   }
}

template <class OutputIterator, class Results, class traits>
void basic_regex_formatter<OutputIterator, Results, traits>::format_until_scope_end()
{
   do
   {
      format_all();
      if((m_position == m_end) || (*m_position == static_cast<char_type>(')')))
         return;
      put(*m_position++);
   }while(m_position != m_end);
}

template <class OutputIterator, class Results, class traits>
void basic_regex_formatter<OutputIterator, Results, traits>::put(char_type c)
{
   // write a single character to output
   // according to which case translation mode we are in:
   switch(this->m_state)
   {
   case output_none:
      return;
   case output_next_lower:
      c = m_traits.tolower(c);
      this->m_state = output_copy;
      break;
   case output_next_upper:
      c = m_traits.toupper(c);
      this->m_state = output_copy;
      break;
   case output_lower:
      c = m_traits.tolower(c);
      break;
   case output_upper:
      c = m_traits.toupper(c);
      break;
   default:
      break;
   }
   *m_out = c;
   ++m_out;
}

template <class OutputIterator, class Results, class traits>
void basic_regex_formatter<OutputIterator, Results, traits>::put(const sub_match_type& sub)
{
   typedef typename sub_match_type::iterator iterator_type;
   iterator_type i = sub.first;
   while(i != sub.second)
   {
      put(*i);
      ++i;
   }
}

template <class S>
class string_out_iterator
{
   S* out;
public:
   string_out_iterator(S& s) : out(&s) {}
   string_out_iterator& operator++() { return *this; }
   string_out_iterator& operator++(int) { return *this; }
   string_out_iterator& operator*() { return *this; }
   string_out_iterator& operator=(typename S::value_type v) 
   { 
      out->append(1, v); 
      return *this; 
   }
};

template <class OutputIterator, class Iterator, class charT, class traits>
OutputIterator regex_format_imp(OutputIterator out,
                          const match_results<Iterator>& m,
                          const charT* p1, const charT* p2,
                          match_flag_type flags,
                          const traits& t
                         )
{
   re_detail::basic_regex_formatter<
      OutputIterator, 
      match_results<Iterator>, 
      traits > f(out, m, t);
   return f.format(p1, p2, flags);
}


} // namespace re_detail

template <class OutputIterator, class Iterator, class charT>
OutputIterator regex_format(OutputIterator out,
                          const match_results<Iterator>& m,
                          const charT* fmt,
                          match_flag_type flags = format_all
                         )
{
   re_detail::trivial_format_traits<charT> traits;
   return regex_format_imp(out, m, fmt, fmt + traits.length(fmt), flags, traits);
}

template <class OutputIterator, class Iterator, class charT>
OutputIterator regex_format(OutputIterator out,
                          const match_results<Iterator>& m,
                          const std::basic_string<charT>& fmt,
                          match_flag_type flags = format_all
                         )
{
   re_detail::trivial_format_traits<charT> traits;
   return regex_format_imp(out, m, fmt.data(), fmt.data() + fmt.size(), flags, traits);
}  

template <class Iterator, class charT>
std::basic_string<charT> regex_format(const match_results<Iterator>& m, 
                                      const charT* fmt, 
                                      match_flag_type flags = format_all)
{
   std::basic_string<charT> result;
   re_detail::string_out_iterator<std::basic_string<charT> > i(result);
   re_detail::trivial_format_traits<charT> traits;
   regex_format_imp(i, m, fmt, fmt + traits.length(fmt), flags, traits);
   return result;
}

template <class Iterator, class charT>
std::basic_string<charT> regex_format(const match_results<Iterator>& m, 
                                      const std::basic_string<charT>& fmt, 
                                      match_flag_type flags = format_all)
{
   std::basic_string<charT> result;
   re_detail::string_out_iterator<std::basic_string<charT> > i(result);
   re_detail::trivial_format_traits<charT> traits;
   regex_format_imp(i, m, fmt.data(), fmt.data() + fmt.size(), flags, traits);
   return result;
}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

} // namespace boost

#endif  // BOOST_REGEX_FORMAT_HPP






