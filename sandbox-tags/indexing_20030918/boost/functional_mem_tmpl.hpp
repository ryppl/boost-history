// (C) Copyright Jeremy Siek 2002. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_FUNCTIONAL_MEM_TMPL_HPP
#define BOOST_FUNCTIONAL_MEM_TMPL_HPP

// These functors are a lot like the Standard functors defined in
// <functional>. The difference is that these functors are not class
// templates, instead their operator() is a member template. This
// provides more flexibility, at the cost of the functors no longer
// being models of the Adaptable Unary and Binary Function concepts.
// Instead they are only models of the (Non-adaptable) Unary and
// Binary Function concepts.

namespace boost {

  struct plus {
    template <typename T>
    T operator()(const T& x, const T& y) const { return x + y; }
  };

  struct minus {
    template <typename T>
    T operator()(const T& x, const T& y) const { return x - y; }
  };

  struct multiplies {
    template <typename T>
    T operator()(const T& x, const T& y) const { return x * y; }
  };

  struct divides {
    template <typename T>
    T operator()(const T& x, const T& y) const { return x / y; }
  };

  struct modulus 
  {
    template <typename T>
    T operator()(const T& x, const T& y) const { return x % y; }
  };

  struct negate
  {
    template <typename T>
    T operator()(const T& x) const { return -x; }
  };
  struct equal_to
  {
    template <typename T>
    bool operator()(const T& x, const T& y) const { return x == y; }
  };

  struct not_equal_to
  {
    template <typename T>
    bool operator()(const T& x, const T& y) const { return x != y; }
  };

  struct greater
  {
    template <typename T>
    bool operator()(const T& x, const T& y) const { return x > y; }
  };

  struct less
  {
    template <typename T>
    bool operator()(const T& x, const T& y) const { return x < y; }
  };

  struct greater_equal
  {
    template <typename T>
    bool operator()(const T& x, const T& y) const { return x >= y; }
  };

  struct less_equal
  {
    template <typename T>
    bool operator()(const T& x, const T& y) const { return x <= y; }
  };

  struct logical_and
  {
    template <typename T>
    bool operator()(const T& x, const T& y) const { return x && y; }
  };

  struct logical_or
  {
    template <typename T>
    bool operator()(const T& x, const T& y) const { return x || y; }
  };

  struct logical_not
  {
    template <typename T>
    bool operator()(const T& x) const { return !x; }
  };

} // namespace boost

#endif // BOOST_FUNCTIONAL_MEM_TMPL_HPP
