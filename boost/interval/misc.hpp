/* boost detail/interval_misc.hpp template implementation file
 *
 * Copyright Jens Maurer 2000
 * Permission to use, copy, modify, sell, and distribute this software
 * is hereby granted without free provided that the above copyright notice
 * appears in all copies and that both that copyright notice and this
 * permission notice appear in supporting documentation,
 *
 * Jens Maurer makes no representations about the suitability of this
 * software for any purpose. It is provided "as is" without express or
 * implied warranty.
 *
 * $Id$
 *
 * Revision History
 *  2000-09-24  separated from interval.hpp
 */

#ifndef BOOST_INTERVAL_MISC_HPP
#define BOOST_INTERVAL_MISC_HPP

#include <iostream>
#include <string>        // for non-explicit string constructor
//#include <locale>


namespace boost {

/*
 * Input/Output
 */

// Ensure generation of outer bounds in all cases
#if 0 // FIXME: essayer de générer l'interval extérieur
template<class Ch, class ChTr, class T, class Traits>
std::basic_ostream<Ch, ChTr>&
operator<<(std::basic_ostream<Ch, ChTr>& os, const interval<T, Traits>& r)
{
  typename std::basic_ostream<Ch, ChTr>::sentry sentry(os);
  if (sentry) {
    T l = r.lower(), u = r.upper();
    os << '[' << l << ',' << u << ']';
  }
  return os;
}
#else
template<class T, class Traits> inline
std::ostream& operator<<(std::ostream& os, const interval<T, Traits>& r)
{
  os << "[" << r.lower() << "," << r.upper() << "]";
  return os;
}
#endif


#if 0 // FIXME: arranger le code pour les g++ 3.*
template<class Ch, class ChTr, class T, class Traits>
std::basic_istream<Ch, ChTr>&
operator>>(std::basic_istream<Ch, ChTr>& is, const interval<T, Traits>& r)
{
  T l = 0, u = 0;
  std::locale loc = is.getloc();
  const std::ctype<Ch> c_type = std::use_facet<c_type<Ch> >(loc);
  const char punct[] = "[,]";
  Ch wpunct[3];
  c_type.widen(punct, punct+3, wpunct);
  Ch c;
  is >> c;
  if(ChTr::eq(c, wpunct[0])) {
    is >> l >> c;
    if(ChTr::eq(c, wpunct[1]))
      is >> u >> c;
    if(!ChTr::eq(c, wpunct[2]))
      is.setstate(is.failbit);
  } else {
    is.putback(c);
    is >> l;
    u = l;
  }
  if(is)
    r = succ(interval<T, Traits>(l, r));   // round outward by 1 ulp
  return is;
}
#else
template<class T, class Traits> inline
std::istream& operator>>(std::istream& is, interval<T, Traits>& r)
{
  T l, u;
  char c = 0;

  if(is.peek() == '[') {
    is >> c;
    is >> l;
    if(is.peek() == ',') {
      is >> c;
      is >> u;
      is >> c;
    }
  }
  if(c != ']')
    is.setstate(is.failbit);
  else if(is.good())
    r.assign(l, u);
  return is;
}
#endif

} // namespace boost

#endif // BOOST_INTERVAL_MISC_HPP
