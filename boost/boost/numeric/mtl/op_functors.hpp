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

#ifndef OP_FUNCTIONS_HPP
#define OP_FUNCTIONS_HPP

#include <mtl/conj.hpp>

struct and_op { };
struct or_op { };

class op_negative {
public:
  template <class A>
  inline typename unary_promote<op_negative, A>::type
  operator() (const A& a) { return - a; }
};

class op_conj {
public:
  template <class A>
  inline typename unary_promote<op_conj, A>::type
  operator() (const A& a) { return std::conj(a); }
};

class op_abs {
public:
  template <class A>
  inline typename unary_promote<op_abs, A>::type
  operator() (const A& a) { return std::abs(a); }
};

class op_transpose {
public:
  template <class A>
  inline typename unary_promote<op_transpose, A>::type
  operator() (const A& a) { return trans(a); }
};

class op_add {
public:
  typedef or_op index_combine;
  template <class A, class B> 
  inline typename binary_promote<op_add,A,B>::type 
  operator() ( const A& a, const B& b) const { return a + b; } 
}; 

class op_subtract {
public:
  typedef or_op index_combine;
  template <class A, class B> 
  inline typename binary_promote<op_subtract,A,B>::type 
  operator() ( const A& a, const B& b) const { return a - b; } 
}; 

class op_multiply {
public:
  typedef and_op index_combine;
  template <class A, class B>
  inline typename binary_promote<op_multiply,A,B>::type
  operator() ( const A& a, const B& b) const { return a * b; }
};

class op_divide {
public:
  template <class A, class B>
  inline typename binary_promote<op_divide,A,B>::type
  operator() (const A& a, const B& b) { return a / b; }
};

class op_assign {
public:
  template <class A, class B>
  inline void operator() (A& a, const B& b) const { a = b; }
};

class op_assign_add {
public:
  template <class A, class B>
  inline void operator() (A& a, const B& b) const { a += b; }
};

class op_assign_subtract {
public:
  template <class A, class B>
  inline void operator() (A& a, const B& b) const { a -= b; }
};

class op_assign_multiply {
public:
  template <class A, class B>
  inline void operator() (A& a, const B& b) const { a *= b; }
};

class op_assign_divide {
public:
  template <class A, class B>
  inline void operator() (A& a, const B& b) const { a /= b; }
};

class op_assign_and {
public:
  template <class A, class B>
  inline void operator() (A& a, const B& b) const { a &= b; }
};

class op_assign_or {
public:
  template <class A, class B>
  inline void operator() (A& a, const B& b) const { a |= b; }
};

// comparisons ==============================================

class op_equal {
public: 
  typedef or_op index_combine;
  template <class A, class B> 
  inline bool
  operator() ( const A& a, const B& b) const { return a == b; }
}; 

class op_not_equal {
public: 
  typedef or_op index_combine;
  template <class A, class B> 
  inline bool
  operator() ( const A& a, const B& b) const { return a != b; }
}; 

class op_less_than {
public:
  typedef or_op index_combine;
  template <class A, class B> 
  inline bool
  operator() ( const A& a, const B& b) const { return a < b; }
}; 

template <class T, class AssignOp>
class op_mod_diag
{
public:
  op_mod_diag(T& diag)
    : m_diag(diag) { }

  op_mod_diag(const op_mod_diag& x) 
    : m_diag(x.m_diag) { }

  template <class PairOut, class PairIn>
  void operator()(PairOut& y, const PairIn& x) const {
    if (index(y) == index(x))
      assign_op(y.second, value(x));
    else
      assign_op(m_diag, value(x));
  }
  mutable T& m_diag;
  AssignOp assign_op;
};


class op_greater_than {
public:
  typedef or_op index_combine;
  template <class T, class U>
  inline bool
  operator()(const T& x, const U& y) const { return x > y; }
};

template <class T, class U>
class binary_promote<op_less_than, T, U>
{
public:
  typedef bool type;
};
template <class T, class U>
class binary_promote<op_greater_than, T, U>
{
public:
  typedef bool type;
};

// shitters...
template <>
class binary_promote<op_less_than, double, double>
{
public:
  typedef bool type;
};
template <>
class binary_promote<op_greater_than, double, double>
{
public:
  typedef bool type;
};

template <class T, class Op>
class bind_first {
public:
  bind_first(const T& x) : m_x(x) { }

  template <class U>
  typename binary_promote<Op, T, U>::type
  operator()(const U& y) {
    return m_op(m_x, y);
  }
  const T& m_x;
  Op m_op;
};

template <class T>
inline bind_first<T, op_less_than>
less_than(const T& x) {
  return bind_first<T, op_less_than>(x);
}

template <class T>
inline bind_first<T, op_greater_than>
greater_than(const T& x) {
  return bind_first<T, op_greater_than>(x);
}


#endif // OP_FUNCTIONS_HPP
