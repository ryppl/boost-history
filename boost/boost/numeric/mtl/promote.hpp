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

#ifndef PROMOTE_HPP
#define PROMOTE_HPP

#include <boost/type_traits.hpp>
#include <mtl/pair.hpp>

// compressed_iterator uses ref's inside of the pairs, and
// we need to make sure that new pairs for temporaries don't
// use ref's.

template <class T>
class remove_const_ref {
public:
  typedef typename boost::remove_const<
    typename boost::remove_reference<T>::type
  >::type type;
};


// if not an expression, convert to the expression type
template <class E>
class to_expr {
public:
  typedef typename E::expr_type type;
};
// leave scalar's as is. This needs work...
template <>
class to_expr<double> {
public:
  typedef double type;
};

template <class E>
class to_expr<const E&> {
public:
  typedef E type;
};

template <class A, class B>
class to_expr< mtl::pair<A,B> > {
public:
  typedef mtl::pair<A,B> type;
};
// specializations of to_expr for expr's is in expr.hpp

template <class Op, class A, class B>
class binary_promote {
  typedef typename to_expr<A>::type A_expr;
  typedef typename to_expr<B>::type B_expr;
public:
  typedef typename binary_promote<Op, A_expr, B_expr>::type type;
};

template <class Op>
class binary_promote<Op,double,double> {
public:
  typedef double type;
};

template <class Op, class A>
class unary_promote {
  typedef typename A::expr_type A_expr;
public:
  typedef typename unary_promote<Op, A_expr>::type type;
};

template <class Op>
class unary_promote<Op, double> {
public:
  typedef double type;
};

template <class Op>
class unary_promote<Op, const double&> {
public:
  typedef double type;
};

template <class Op, class A, class B>
class unary_promote<Op, mtl::pair<A,B> > {
  typedef typename remove_const_ref<A>::type A_type;
  typedef typename remove_const_ref<B>::type B_type;
public:
  typedef mtl::pair<A_type,B_type> type;
};

#include <mtl/op_functors.hpp>

#endif // PROMOTE_HPP
