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

#ifndef INDEX_VALUE_PAIR_HPP
#define INDEX_VALUE_PAIR_HPP

#include <mtl/pair.hpp>
#include <mtl/promote.hpp>


template <class Idx, class T>
Idx index(const mtl::pair<Idx,T>& p) { return p.first; }
template <class Idx, class T>
T value(const mtl::pair<Idx,T>& p) { return p.second; }

template <class T>
const T& value(const T& x) { return x; }


#define DEFINE_PAIR_ASSIGN_OP(OP) \
template <class I1, class T1, class T2> \
mtl::pair<I1,T1>& operator OP (mtl::pair<I1,T1>& a, const T2& b) \
{ \
  a.second *= b; \
  return a; \
}

DEFINE_PAIR_ASSIGN_OP(*=)
DEFINE_PAIR_ASSIGN_OP(/=)
DEFINE_PAIR_ASSIGN_OP(+=)
DEFINE_PAIR_ASSIGN_OP(-=)

template <class A1, class A2>
typename unary_promote<op_negative, mtl::pair<A1,A2> >::type
operator-(const mtl::pair<A1,A2>& p) {
  return mtl::make_pair(p.first, - p.second);
}

namespace std {
  template <class A1, class A2>
  typename unary_promote<op_abs, mtl::pair<A1,A2> >::type
  abs(const mtl::pair<A1,A2>& p) {
    return mtl::make_pair(p.first, std::abs(p.second));
  }
  template <class A1, class A2>
  typename unary_promote<op_conj, mtl::pair<A1,A2> >::type
  conj(const mtl::pair<A1,A2>& p) {
    return mtl::make_pair(p.first, std::conj(p.second));
  }
}



//======================================================================

template <class A1, class A2, class B1, class B2>
class binary_promote<op_multiply, mtl::pair<A1,A2>, mtl::pair<B1,B2> > {
  typedef typename remove_const_ref<A1>::type A1_type;
  typedef typename remove_const_ref<A2>::type A2_type;
  typedef typename remove_const_ref<B2>::type B2_type;
  typedef typename binary_promote<op_multiply, A2_type, B2_type>::type
    value_type;
public:
  typedef mtl::pair<A1_type, value_type> type;
};

template <class I1, class T1, class I2, class T2>
typename binary_promote<op_multiply, mtl::pair<I1,T1>, mtl::pair<I2,T2> >::type
operator*(const mtl::pair<I1,T1>& a, const mtl::pair<I2,T2>& b)
{
  return mtl::make_pair(index(a), value(a) * value(b));
}

template <class A1, class A2, class B>
class binary_promote<op_multiply, mtl::pair<A1,A2>, B> {
  typedef typename remove_const_ref<A1>::type A1_type;
  typedef typename remove_const_ref<A2>::type A2_type;
public:
  typedef typename binary_promote<op_multiply, A2_type, B>::type value_type;
  typedef mtl::pair<A1_type, value_type> type;
};

template <class I1, class T1, class T2>
typename binary_promote<op_multiply, mtl::pair<I1,T1>, T2>::type
operator*(const mtl::pair<I1,T1>& a, const T2& b)
{
  return mtl::make_pair(index(a), value(a) * b);
}

template <class A, class B1, class B2>
class binary_promote<op_multiply, A, mtl::pair<B1,B2> >
{
  typedef typename remove_const_ref<B1>::type B1_type;
  typedef typename remove_const_ref<B2>::type B2_type;
public:
  typedef typename binary_promote<op_multiply, A, B2_type>::type value_type;
  typedef mtl::pair<B1_type, value_type> type;
};

template <class T1, class I2, class T2>
typename binary_promote<op_multiply, T1, mtl::pair<I2,T2> >::type
operator*(const T1& a, const mtl::pair<I2,T2>& b)
{
  return mtl::make_pair(index(b), a * value(b));
}


//======================================================================


template <class A1, class A2, class B1, class B2>
class binary_promote<op_add, mtl::pair<A1,A2>, mtl::pair<B1,B2> > {
  typedef typename remove_const_ref<A1>::type A1_type;
  typedef typename remove_const_ref<A2>::type A2_type;
  typedef typename remove_const_ref<B2>::type B2_type;
public:
  typedef typename binary_promote<op_add, A2_type, B2_type>::type value_type;
  typedef mtl::pair<A1_type, value_type> type;
};

template <class I1, class T1, class I2, class T2>
typename binary_promote<op_add, mtl::pair<I1,T1>, mtl::pair<I2,T2> >::type
operator+(const mtl::pair<I1,T1>& a, const mtl::pair<I2,T2>& b)
{
  return mtl::make_pair(index(a), value(a) + value(b));
}

template <class A1, class A2, class B>
class binary_promote<op_add, mtl::pair<A1,A2>, B> {
  typedef typename remove_const_ref<A1>::type A1_type;
  typedef typename remove_const_ref<A2>::type A2_type;
public:
  typedef typename binary_promote<op_add, A2_type, B>::type value_type;
  typedef mtl::pair<A1_type, value_type> type;
};

template <class I1, class T1, class T2>
typename binary_promote<op_add, mtl::pair<I1,T1>, T2>::type
operator+(const mtl::pair<I1,T1>& a, const T2& b)
{
  return mtl::make_pair(index(a), value(a) + b);
}

template <class A, class B1, class B2>
class binary_promote<op_add, A, mtl::pair<B1,B2> > {
  typedef typename remove_const_ref<B1>::type B1_type;
  typedef typename remove_const_ref<B2>::type B2_type;
public:
  typedef typename binary_promote<op_add, A, B2_type>::type value_type;
  typedef mtl::pair<B1_type, value_type> type;
};

template <class T1, class I2, class T2>
typename binary_promote<op_add, T1, mtl::pair<I2,T2> >::type
operator+(const T1& a, const mtl::pair<I2,T2>& b)
{
  return mtl::make_pair(index(b), a + value(b));
}



template <class A1, class A2, class B1, class B2>
class binary_promote<op_subtract, mtl::pair<A1,A2>, mtl::pair<B1,B2> > {
  typedef typename remove_const_ref<A1>::type A1_type;
  typedef typename remove_const_ref<A2>::type A2_type;
  typedef typename remove_const_ref<B2>::type B2_type;
public:
  typedef typename binary_promote<op_subtract, A2_type, B2_type>::type value_type;
  typedef mtl::pair<A1_type, value_type> type;
};

template <class I1, class T1, class I2, class T2>
typename binary_promote<op_subtract, mtl::pair<I1,T1>, mtl::pair<I2,T2> >::type
operator-(const mtl::pair<I1,T1>& a, const mtl::pair<I2,T2>& b)
{
  return mtl::make_pair(index(a), value(a) - value(b));
}

template <class A1, class A2, class B>
class binary_promote<op_subtract, mtl::pair<A1,A2>, B> {
  typedef typename remove_const_ref<A1>::type A1_type;
  typedef typename remove_const_ref<A2>::type A2_type;
public:
  typedef typename binary_promote<op_subtract, A2_type, B>::type value_type;
  typedef mtl::pair<A1_type, value_type> type;
};

template <class I1, class T1, class T2>
typename binary_promote<op_subtract, mtl::pair<I1,T1>, T2>::type
operator-(const mtl::pair<I1,T1>& a, const T2& b)
{
  return mtl::make_pair(index(a), value(a) - b);
}

template <class A, class B1, class B2>
class binary_promote<op_subtract, A, mtl::pair<B1,B2> > {
  typedef typename remove_const_ref<B1>::type B1_type;
  typedef typename remove_const_ref<B2>::type B2_type;
public:
  typedef typename binary_promote<op_subtract, A, B2_type>::type value_type;
  typedef mtl::pair<B1_type, value_type> type;
};

template <class T1, class I2, class T2>
typename binary_promote<op_subtract, T1, mtl::pair<I2,T2> >::type
operator-(const T1& a, const mtl::pair<I2,T2>& b)
{
  return mtl::make_pair(index(b), a - value(b));
}



#endif // INDEX_VALUE_PAIR_HPP
