// (C) Copyright Jeremy Siek 2000. Permission to copy, use, modify, sell and
// distribute this software is granted provided this copyright notice appears
// in all copies. This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

// The ct_if implementation that avoids partial specialization is
// based on the IF class by Ulrich W. Eisenecker and Krzysztof
// Czarnecki.

#ifndef BOOST_CT_IF_HPP
#define BOOST_CT_IF_HPP

#include <boost/config.hpp>

/*
  There is a bug in the Borland compiler with regards to using
  integers to specialize templates. This made it hard to use ct_if in
  the graph library. Changing from 'ct_if' to 'ct_if_t' fixed the
  problem.
*/

namespace boost {

  struct ct_if_error { };

  struct true_type { enum { value = true }; };
  struct false_type { enum { value = false }; };

  template <class A, class B>
  struct ct_and { typedef false_type type; };
  template <> struct ct_and<true_type,true_type> { typedef true_type type; };

  template <class A> struct ct_not { typedef ct_if_error type; };
  template <> struct ct_not<true_type> { typedef false_type type; };
  template <> struct ct_not<false_type> { typedef true_type type; };

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

  template <int cond, class A, class B>
  struct ct_if { typedef ct_if_error type; };
  template <class A, class B>
  struct ct_if<1, A, B> { typedef A type; };
  template <class A, class B>
  struct ct_if<0, A, B> { typedef B type; };

  template <class cond, class A, class B>
  struct ct_if_t { typedef ct_if_error type; };
  template <class A, class B>
  struct ct_if_t<true_type, A, B> { typedef A type; };
  template <class A, class B>
  struct ct_if_t<false_type, A, B> { typedef B type; };

#else

  namespace detail {

    template <int condition, class A, class B> struct IF;
    template <int condition> struct SlectSelector;
    struct SelectFirstType;
    struct SelectSecondType;
    
    struct SelectFirstType {
      template<class A, class B>
      struct Template {        typedef A type; };
    };
    
    struct SelectSecondType {
      template<class A, class B>
      struct Template { typedef B type; };
    };
    
    template<int condition>
    struct SlectSelector {
      typedef SelectFirstType type;
    };
    
    template <>
    struct SlectSelector<0> {
      typedef SelectSecondType type;
    };

  } // namespace detail
    
  template<int condition, class A, class B>
  struct ct_if
  {
    typedef typename detail::SlectSelector<condition>::type Selector;
    typedef typename Selector::template Template<A, B>::type type;
  };
  
  template <class cond, class A, class B>
  struct ct_if_t { 
    typedef typename ct_if<cond::value, A, B>::type type;
  };

#endif

} // namespace boost

#endif // BOOST_CT_IF_HPP

