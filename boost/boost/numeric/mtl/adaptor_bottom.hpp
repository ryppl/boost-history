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

#ifndef MTL_ADAPTOR_BOTTOM_HPP
#define MTL_ADAPTOR_BOTTOM_HPP

#include <mtl/tuple.hpp>

namespace mtl {

  //  This technique is based on: "A Solution to the
  //  Constructor-Problem of Mixin-Based Programming in C++", by
  //  Ulrich W.  Eisenecker and Frank Blinn and Krzysztof Czarnecki in
  //  Proceedings of the 2000 Workshop on C++ Template Programming

  template <class Base>
  class adaptor_bottom : public Base
  {
  public:
    adaptor_bottom() { }

    template <class X1>
    adaptor_bottom(const X1& x1)
      : Base(make_tuple_ref(x1)) { }

    template <class X1, class X2>
    adaptor_bottom(const X1& x1, const X2& x2)
      : Base(make_tuple_ref(x1, x2)) { }

    template <class X1, class X2, class X3>
    adaptor_bottom(const X1& x1, const X2& x2, const X3& x3)
      : Base(make_tuple_ref(x1, x2, x3)) { }

    template <class X1, class X2, class X3, class X4>
    adaptor_bottom(const X1& x1, const X2& x2, const X3& x3, const X4& x4)
      : Base(make_tuple_ref(x1, x2, x3, x4)) { }

    template <class X1, class X2, class X3, class X4, class X5>
    adaptor_bottom(const X1& x1, const X2& x2, const X3& x3, const X4& x4,
                        const X5& x5)
      : Base(make_tuple_ref(x1, x2, x3, x4, x5)) { }

    template <class X1, class X2, class X3, class X4, class X5, class X6>
    adaptor_bottom(const X1& x1, const X2& x2, const X3& x3, const X4& x4,
                        const X5& x5, const X6& x6)
      : Base(make_tuple_ref(x1, x2, x3, x4, x5,x6)) { }

    template <class X>
    adaptor_bottom& operator=(const X& x) {
      Base::operator=(x);
      return *this;
    }

  };  
  
} // namespace mtl

#endif // MTL_ADAPTOR_BOTTOM_HPP
