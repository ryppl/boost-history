//=======================================================================
// Copyright 2001 University of Notre Dame.
// Authors: Jeremy G. Siek, Andrew Lumsdaine
//
// This file is part of the Matrix Template Library
//
// You should have received a copy of the License Agreement for the
// Matrix Template Library along with the software; see the file LICENSE.
// If not, contact Office of Research, University of Notre Dame, Notre
// Dame, IN 46556.
//
// Permission to modify the code and to distribute modified code is
// granted, provided the text of this NOTICE is retained, a notice that
// the code was modified is included with the above COPYRIGHT NOTICE and
// with the COPYRIGHT NOTICE in the LICENSE file, and that the LICENSE
// file is distributed with the modified code.
//
// LICENSOR MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.
// By way of example, but not limitation, Licensor MAKES NO
// REPRESENTATIONS OR WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
// PARTICULAR PURPOSE OR THAT THE USE OF THE LICENSED SOFTWARE COMPONENTS
// OR DOCUMENTATION WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS, TRADEMARKS
// OR OTHER RIGHTS.
//=======================================================================

#ifndef MTL_PAIR_HPP
#define MTL_PAIR_HPP

#include <utility>
// an index-value pair

namespace mtl {

  // This is different in subtle but important ways from std::pair
  template <class A, class B>
  struct pair {
    typedef A first_type;
    typedef B second_type;

    pair() { }
    pair(first_type a, second_type b) : first(a), second(b) { }
    pair(const pair& p) : first(p.first), second(p.second) { }

    template <class X, class Y>
    pair(const std::pair<X,Y>& p) : first(p.first), second(p.second) { }

    template <class X, class Y>
    pair(const pair<X,Y>& p) : first(p.first), second(p.second) { }

    template <class X, class Y>
    operator std::pair<X,Y>() const { return std::pair<X,Y>(first, second); }

    operator second_type() const { return second; }

    first_type first;
    second_type second;
  };

  template <class A, class B>
  inline pair<A, B> make_pair(const A& a, const B& b) {
    return pair<A, B>(a, b);
  }

} // namespace mtl

  template <class A1, class B1, class A2, class B2>
  bool operator==(const mtl::pair<A1,B1>& a, const mtl::pair<A2,B2>& b) {
    return a.first == b.first && a.second == b.second;
  }
  template <class A1, class B1, class A2, class B2>
  bool operator!=(const mtl::pair<A1,B1>& a, const mtl::pair<A2,B2>& b) {
    return !(a == b);
  }
  template <class A1, class B1, class A2, class B2>
  bool operator==(const mtl::pair<A1,B1>& a, const std::pair<A2,B2>& b) {
    return a.first == b.first && a.second == b.second;
  }
  template <class A1, class B1, class A2, class B2>
  bool operator!=(const mtl::pair<A1,B1>& a, const std::pair<A2,B2>& b) {
    return !(a == b);
  }
  template <class A1, class B1, class A2, class B2>
  bool operator==(const std::pair<A1,B1>& a, const mtl::pair<A2,B2>& b) {
    return a.first == b.first && a.second == b.second;
  }
  template <class A1, class B1, class A2, class B2>
  bool operator!=(const std::pair<A1,B1>& a, const mtl::pair<A2,B2>& b) {
    return !(a == b);
  }

  template <class A, class B>
  std::ostream& 
  operator<<(std::ostream& os, const mtl::pair<A,B>& p)
  {
    return os << "(" << p.first << "," << p.second << ")";
  }

#endif // MTL_PAIR_HPP
