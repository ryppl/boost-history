/*
 *
 * Copyright (c) 1998-2004
 * Dr John Maddock
 *
 * Use, modification and distribution are subject to the 
 * Boost Software License, Version 1.0. (See accompanying file 
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */

 /*
  *   LOCATION:    see http://www.boost.org for most recent version.
  *   FILE         basic_regex.cpp
  *   VERSION      see <boost/version.hpp>
  *   DESCRIPTION: Declares template class basic_regex.
  */

#ifndef BOOST_REGEX_V4_BASIC_REGEX_HPP
#define BOOST_REGEX_V4_BASIC_REGEX_HPP

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost{
#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable : 4251 4231 4660)
#endif

namespace re_detail{

//
// forward declaration, we will need this one later:
//
template <class charT, class traits>
class basic_regex_parser;

//
// class regex_data:
// represents the data we wish to expose to the matching algorithms.
//
template <class charT, class traits>
struct regex_data
{
   typedef regex_constants::syntax_option_type   flag_type;
   typedef std::size_t                           size_type;  

   regex_data(const traits& t) : m_traits(t) {}
   regex_data(){}

   traits                      m_traits;                  // traits class instance
   flag_type                   m_flags;                   // flags with which we were compiled
   int                         m_status;                  // error code (0 implies OK).
   const charT*                m_expression;              // the original expression
   std::ptrdiff_t              m_expression_len;          // the length of the original expression
   size_type                   m_mark_count;              // the number of marked sub-expressions
   re_detail::re_syntax_base*  m_first_state;             // the first state of the machine
   unsigned                    m_restart_type;            // search optimisation type
   unsigned char               m_startmap[1 << CHAR_BIT]; // which characters can start a match
   unsigned int                m_can_be_null;             // whether we can match a null string
   re_detail::raw_storage      m_data;                    // the buffer in which our states are constructed
};
//
// class basic_regex_implementation
// pimpl implementation class for basic_regex.
//
template <class charT, class traits>
class basic_regex_implementation
   : protected regex_data<charT, traits>
{
public:
   typedef regex_constants::syntax_option_type   flag_type;
   typedef std::ptrdiff_t                        difference_type;
   typedef std::size_t                           size_type; 
   typedef typename traits::locale_type          locale_type;
   typedef const charT*                          const_iterator;

   basic_regex_implementation(){}
   basic_regex_implementation(const traits& t)
      : regex_data<charT, traits>(t) {}
   void assign(const charT* arg_first,
                          const charT* arg_last,
                          flag_type f)
   {
      regex_data<charT, traits>* pdat = this;
      basic_regex_parser<charT, traits> parser(pdat);
      parser.parse(arg_first, arg_last, f);
   }

   locale_type BOOST_REGEX_CALL imbue(locale_type l)
   { 
      return this->m_traits.imbue(l); 
   }
   locale_type BOOST_REGEX_CALL getloc()const
   { 
      return this->m_traits.getloc(); 
   }
   std::basic_string<charT> BOOST_REGEX_CALL str()const
   {
      std::basic_string<charT> result;
      if(this->m_status == 0)
         result = std::basic_string<charT>(this->m_expression, this->m_expression_len);
      return result;
   }
   const_iterator BOOST_REGEX_CALL expression()const
   {
      return this->m_expression;
   }
   //
   // begin, end:
   const_iterator BOOST_REGEX_CALL begin()const
   { 
      return (!this->m_status ? 0 : this->m_expression); 
   }
   const_iterator BOOST_REGEX_CALL end()const
   { 
      return (!this->m_status ? 0 : this->m_expression + this->m_expression_len); 
   }
   flag_type BOOST_REGEX_CALL flags()const
   {
      return this->m_flags;
   }
   size_type BOOST_REGEX_CALL size()const
   {
      return this->m_expression_len;
   }
   int BOOST_REGEX_CALL status()const
   {
      return this->m_status;
   }
   size_type BOOST_REGEX_CALL mark_count()const
   {
      return this->m_mark_count;
   }
   const re_detail::re_syntax_base* get_first_state()const
   {
      return this->m_first_state;
   }
   unsigned get_restart_type()const
   {
      return this->m_restart_type;
   }
   const unsigned char* get_map()const
   {
      return this->m_startmap;
   }
   const traits& get_traits()const
   {
      return this->m_traits;
   }
   bool can_be_null()const
   {
      return this->m_can_be_null;
   }
   const regex_data<charT, traits>& get_data()const
   {
      basic_regex_implementation<charT, traits> const* p = this;
      return *static_cast<const regex_data<charT, traits>*>(p);
   }
};

} // namespace re_detail
//
// class basic_regex:
// represents the compiled
// regular expression:
//

#ifdef BOOST_REGEX_NO_FWD
template <class charT, class traits = regex_traits<charT> >
#else
template <class charT, class traits >
#endif
class basic_regex : public regbase
{
public:
   // typedefs:
   typedef typename traits::size_type            traits_size_type;
   typedef typename traits::string_type          traits_string_type;
   typedef charT                                 char_type;
   typedef traits                                traits_type;

   typedef charT                                 value_type;
   typedef charT&                                reference;
   typedef const charT&                          const_reference;
   typedef const charT*                          const_iterator;
   typedef const_iterator                        iterator;
   typedef std::ptrdiff_t                        difference_type;
   typedef std::size_t                           size_type;   
   typedef regex_constants::syntax_option_type   flag_type;
   // locale_type
   // placeholder for actual locale type used by the
   // traits class to localise *this.
   typedef typename traits::locale_type          locale_type;
   
public:
   explicit basic_regex(){}
   explicit basic_regex(const charT* p, flag_type f = regex_constants::normal)
   {
      assign(p, f);
   }
   basic_regex(const charT* p1, const charT* p2, flag_type f = regex_constants::normal)
   {
      assign(p1, p2, f);
   }
   basic_regex(const charT* p, size_type len, flag_type f)
   {
      assign(p, len, f);
   }
   basic_regex(const basic_regex& that)
      : m_pimpl(that.m_pimpl) {}
   ~basic_regex(){}
   basic_regex& BOOST_REGEX_CALL operator=(const basic_regex& that)
   {
      return assign(that);
   }
   basic_regex& BOOST_REGEX_CALL operator=(const charT* ptr)
   {
      return assign(ptr, regex_constants::normal);
   }

   //
   // assign:
   basic_regex& assign(const basic_regex& that)
   { 
      m_pimpl = that.m_pimpl;
      return *this; 
   }
   basic_regex& assign(const charT* p, flag_type f = regex_constants::normal)
   {
      return assign(p, p + traits::length(p), f);
   }
   basic_regex& assign(const charT* p, size_type len, flag_type f)
   {
      return assign(p, p + len, f);
   }

   basic_regex& assign(const charT* p1,
                          const charT* p2,
                          flag_type f = regex_constants::normal)
   {
      cow();
      m_pimpl->assign(p1, p2, f);
      return *this;
   }
#if !defined(BOOST_NO_MEMBER_TEMPLATES) && !defined(__IBMCPP__)

   template <class ST, class SA>
   unsigned int BOOST_REGEX_CALL set_expression(const std::basic_string<charT, ST, SA>& p, flag_type f = regex_constants::normal)
   { 
      return set_expression(p.data(), p.data() + p.size(), f); 
   }

   template <class ST, class SA>
   explicit basic_regex(const std::basic_string<charT, ST, SA>& p, flag_type f = regex_constants::normal)
   { 
      assign(p, f); 
   }

   template <class InputIterator>
   basic_regex(InputIterator arg_first, InputIterator arg_last, flag_type f = regex_constants::normal)
   {
      std::basic_string<charT> a(arg_first, arg_last);
      assign(a.data(), a.data() + a.size(), f);
   }

   template <class ST, class SA>
   basic_regex& BOOST_REGEX_CALL operator=(const std::basic_string<charT, ST, SA>& p)
   {
      return assign(p.data(), p.data() + p.size(), regex_constants::normal);
   }

   template <class string_traits, class A>
   basic_regex& BOOST_REGEX_CALL assign(
       const std::basic_string<charT, string_traits, A>& s,
       flag_type f = regex_constants::normal)
   {
      return assign(s.data(), s.data() + s.size(), f);
   }

   template <class InputIterator>
   basic_regex& BOOST_REGEX_CALL assign(InputIterator arg_first,
                          InputIterator arg_last,
                          flag_type f = regex_constants::normal)
   {
      std::basic_string<charT> a(arg_first, arg_last);
      return assign(a.data(), a.data() + a.size(), f);
   }
#else
   unsigned int BOOST_REGEX_CALL set_expression(const std::basic_string<charT>& p, flag_type f = regex_constants::normal)
   { 
      return set_expression(p.data(), p.data() + p.size(), f); 
   }

   basic_regex(const std::basic_string<charT>& p, flag_type f = regex_constants::normal)
   { 
      assign(p, f); 
   }

   basic_regex& BOOST_REGEX_CALL operator=(const std::basic_string<charT>& p)
   {
      return assign(p.data(), p.data() + p.size(), regex_constants::normal);
   }

   basic_regex& BOOST_REGEX_CALL assign(
       const std::basic_string<charT>& s,
       flag_type f = regex_constants::normal)
   {
      return assign(s.data(), s.data() + s.size(), f);
   }

#endif

   //
   // locale:
   locale_type BOOST_REGEX_CALL imbue(locale_type l)
   { 
      cow();
      return m_pimpl->imbue(l); 
   }
   locale_type BOOST_REGEX_CALL getloc()const
   { 
      return m_pimpl.get() ? m_pimpl->getloc() : locale_type(); 
   }
   //
   // getflags:
   // retained for backwards compatibility only, "flags"
   // is now the prefered name:
   flag_type BOOST_REGEX_CALL getflags()const
   { 
      return flags();
   }
   flag_type BOOST_REGEX_CALL flags()const
   { 
      return m_pimpl.get() ? m_pimpl->flags() : 0;
   }
   //
   // str:
   std::basic_string<charT> BOOST_REGEX_CALL str()const
   {
      return m_pimpl.get() ? m_pimpl->str() : std::basic_string<charT>();
   }
   //
   // begin, end:
   const_iterator BOOST_REGEX_CALL begin()const
   { 
      return (m_pimpl.get() ? m_pimpl->begin() : 0); 
   }
   const_iterator BOOST_REGEX_CALL end()const
   { 
      return (m_pimpl.get() ? m_pimpl->end() : 0); 
   }
   //
   // swap:
   void BOOST_REGEX_CALL swap(basic_regex& that)throw()
   {
      m_pimpl.swap(that.m_pimpl);
   }
   //
   // size:
   size_type BOOST_REGEX_CALL size()const
   { 
      return (m_pimpl.get() ? m_pimpl->size() : 0); 
   }
   //
   // max_size:
   size_type BOOST_REGEX_CALL max_size()const
   { 
      return UINT_MAX; 
   }
   //
   // empty:
   bool BOOST_REGEX_CALL empty()const
   { 
      return (m_pimpl.get() ? 0 != m_pimpl->status() : 0); 
   }

   size_type BOOST_REGEX_CALL mark_count()const 
   { 
      return (m_pimpl.get() ? m_pimpl->mark_count() : 0); 
   }

   int status()const
   {
      return (m_pimpl.get() ? m_pimpl->status() : regex_constants::error_empty);
   }

   int BOOST_REGEX_CALL compare(const basic_regex& that) const
   {
      if(m_pimpl.get() == that.m_pimpl.get())
         return 0;
      if(!m_pimpl.get())
         return -1;
      if(!that.m_pimpl.get())
         return 1;
      if(status() != that.status())
         return status() - that.status();
      if(flags() != that.flags())
         return flags() - that.flags();
      return str().compare(that.str());
   }
   bool BOOST_REGEX_CALL operator==(const basic_regex& e)const
   { 
      return compare(e) == 0; 
   }
   bool operator != (const basic_regex& e)
   { 
      return compare(e) != 0; 
   }
   bool BOOST_REGEX_CALL operator<(const basic_regex& e)const
   { 
      return compare(e) < 0; 
   }
   bool BOOST_REGEX_CALL operator>(const basic_regex& e)const
   { 
      return compare(e) > 0; 
   }
   bool BOOST_REGEX_CALL operator<=(const basic_regex& e)const
   { 
      return compare(e) <= 0; 
   }
   bool BOOST_REGEX_CALL operator>=(const basic_regex& e)const
   { 
      return compare(e) >= 0; 
   }

   //
   // The following are deprecated as public interfaces
   // but are available for compatibility with earlier versions.
   const charT* BOOST_REGEX_CALL expression()const 
   { 
      return (m_pimpl.get() && !m_pimpl->status() ? m_pimpl->expression() : 0); 
   }
   unsigned int BOOST_REGEX_CALL set_expression(const charT* p1, const charT* p2, flag_type f = regex_constants::normal)
   {
      assign(p1, p2, f | regex_constants::no_except);
      return status();
   }
   unsigned int BOOST_REGEX_CALL set_expression(const charT* p, flag_type f = regex_constants::normal) 
   { 
      assign(p, f | regex_constants::no_except); 
      return status();
   }
   unsigned int BOOST_REGEX_CALL error_code()const
   {
      return status();
   }
   //
   // private access methods:
   //
   const re_detail::re_syntax_base* get_first_state()const
   {
      assert(m_pimpl.get());
      return m_pimpl->get_first_state();
   }
   unsigned get_restart_type()const
   {
      assert(m_pimpl.get());
      return m_pimpl->get_restart_type();
   }
   const unsigned char* get_map()const
   {
      assert(m_pimpl.get());
      return m_pimpl->get_map();
   }
   const traits& get_traits()const
   {
      assert(m_pimpl.get());
      return m_pimpl->get_traits();
   }
   bool can_be_null()const
   {
      assert(m_pimpl.get());
      return m_pimpl->can_be_null();
   }
   const re_detail::regex_data<charT, traits>& get_data()const
   {
      assert(m_pimpl.get());
      return m_pimpl->get_data();
   }

private:
   shared_ptr<re_detail::basic_regex_implementation<charT, traits> > m_pimpl;
   void cow()
   {
      // copy-on-write
      if(!m_pimpl.get())
      {
         m_pimpl = shared_ptr<re_detail::basic_regex_implementation<charT, traits> >(new re_detail::basic_regex_implementation<charT, traits>());
      }
      else if(!m_pimpl.unique())
      {
         m_pimpl = shared_ptr<re_detail::basic_regex_implementation<charT, traits> >(new re_detail::basic_regex_implementation<charT, traits>(m_pimpl->get_traits()));
      }
   }
};

#ifndef BOOST_NO_STD_LOCALE
template <class charT, class traits, class traits2>
std::basic_ostream<charT, traits>& 
   operator << (std::basic_ostream<charT, traits>& os, 
                const basic_regex<charT, traits2>& e)
{
   return (os << e.str());
}
#else
template <class traits>
std::ostream& operator << (std::ostream& os, const basic_regex<char, traits>& e)
{
   return (os << e.str());
}
#endif

#ifdef BOOST_MSVC
#pragma warning (pop)
#endif

} // namespace boost

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif

