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

#ifndef MTL_TUPLE_HPP
#define MTL_TUPLE_HPP

#include <mtl/pair.hpp>

namespace mtl {

  struct nil { };

  template <class H, class T>
  inline H head(const pair<H,T>& x) {
    return x.first;
  }
  template <class H, class T>
  inline T tail(const pair<H,T>& x) {
    return x.second;
  }


  template <class Pair, int N>
  struct nth_type {
    typedef typename nth_type<typename Pair::second_type, N-1>::type type;
  };
  template <class Pair>
  struct nth_type<Pair, 0> {
    typedef typename Pair::first_type type;
  };

  template <int N>
  struct get_nth {
    template <class Pair>
    static inline typename nth_type<Pair, N>::type
    apply(const Pair& p) { return get_nth<N-1>::apply(tail(p)); }
  };
  template <>
  struct get_nth<0> {
    template <class Pair>
    static inline typename Pair::first_type
    apply(const Pair& p) { return head(p); }
  };

  template <class X1>
  inline pair<const X1&, nil> 
  make_tuple_ref(const X1& x1) {
    return pair<const X1&, nil>(x1, nil());
  }

  template <class X1, class X2>
  inline pair<const X1&, pair<const X2&, nil> >
  make_tuple_ref(const X1& x1, const X2& x2) {
    typedef pair<const X1&, pair<const X2&, nil> > Pair;
    return Pair(x1, make_tuple_ref(x2));
  }

  template <class X1, class X2, class X3>
  inline pair<const X1&, pair<const X2&, pair<const X3&, nil> > >
  make_tuple_ref(const X1& x1, const X2& x2, const X3& x3) {
    typedef pair<const X1&, pair<const X2&, pair<const X3&, nil> > > Pair;
    return Pair(x1, make_tuple_ref(x2, x3));
  }

  template <class X1, class X2, class X3, class X4>
  inline pair<const X1&, pair<const X2&, pair<const X3&, 
    pair<const X4&, nil> > > >
  make_tuple_ref(const X1& x1, const X2& x2, const X3& x3, const X4& x4) {
    typedef pair<const X1&, pair<const X2&, pair<const X3&, 
      pair<const X4&, nil> > > > Pair;
    return Pair(x1, make_tuple_ref(x2, x3, x4));
  }

  template <class X1, class X2, class X3, class X4, class X5>
  inline pair<const X1&, pair<const X2&, pair<const X3&, 
    pair<const X4&, pair<const X5&, nil> > > > >
  make_tuple_ref(const X1& x1, const X2& x2, const X3& x3, 
		 const X4& x4, const X5& x5) {
    typedef pair<const X1&, pair<const X2&, pair<const X3&, 
       pair<const X4&, pair<const X5&, nil> > > > > Pair;
    return Pair(x1, make_tuple_ref(x2, x3, x4, x5));
  }

  template <class X1, class X2, class X3, class X4, class X5, class X6>
  inline pair<const X1&, pair<const X2&, pair<const X3&, 
     pair<const X4&, pair<const X5&, pair<const X6&, nil> > > > > >
  make_tuple_ref(const X1& x1, const X2& x2, const X3& x3, 
		 const X4& x4, const X5& x5) {
    typedef pair<const X1&, pair<const X2&, pair<const X3&, 
      pair<const X4&, pair<const X5&, pair<const X6&, nil> > > > > > Pair;
    return Pair(x1, make_tuple_ref(x2, x3, x4, x5, x6));
  }
  
} // namespace mtl

#endif // MTL_TUPLE_HPP
