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

#ifndef RANGE_ADAPTORS_HPP
#define RANGE_ADAPTORS_HPP

#include <mtl/promote.hpp>
#include <mtl/range.hpp>
#include <mtl/index_value_pair.hpp>
#include <mtl/tags.hpp>

// element-wise binary operator range ========================================
template <class A, class B, class Op, class LeftSparsity, class RightSparsity,
          class Combine>
class bin_op_range { };

// dense-dense
template <class A, class B, class Op, class Combine>
class bin_op_range<A, B, Op, dense, dense, Combine>
{
  typedef bin_op_range self;
  A a;
  B b;
  typedef typename range_traits<A>::value_type A_value;
  typedef typename range_traits<B>::value_type B_value;
public:
  typedef typename binary_promote<Op, A_value, B_value>::type value_type;
  bin_op_range( const A& a_, const B& b_ )
    : a(a_), b(b_) {}

  value_type operator*() const { return Op()(*a, *b); }
  value_type operator[](std::ptrdiff_t i) const { 
    return Op()(a[i], b[i]); 
  }
  self& operator++() { ++a; ++b; return *this; }
  friend bool empty(const self& x) { return empty(x.a) || empty(x.b); }
};

enum iter_state { equal_index, left_less, right_less, 
		  left_done, right_done, both_done };

// sparse-sparse, "or" index set
template <class A, class B, class Op>
class bin_op_range<A, B, Op, sparse, sparse, or_op>
{
  typedef bin_op_range self;
  A a;
  B b;
  iter_state state;
  typedef typename range_traits<A>::value_type A_pair;
  typedef typename A_pair::first_type index_t;
  typedef typename A_pair::second_type A_value;
  typedef typename range_traits<B>::value_type B_pair;
  typedef typename B_pair::second_type B_value;
  typedef typename binary_promote<Op, A_value, B_value>::type value_t;
public:
  typedef mtl::pair<index_t, value_t> value_type;
  bin_op_range( const A& a_, const B& b_ )
    : a(a_), b(b_)
  {
    if (empty(a))
      if(empty(b))
	state = both_done;
      else
	state = left_done;
    else if (empty(b))
      state = right_done;
    else if (index(*a) == index(*b))
      state = equal_index;
    else if (index(*a) < index(*b))
      state = left_less;
    else
      state = right_less;
	     
  }
  value_type operator*() const {
    assert(state != both_done);
    switch (state) {
    case equal_index:
      return Op()(*a, *b);
    case left_less:
      return Op()(*a, B_pair(index(*a), B_value()));
    case right_less:
      return Op()(A_pair(index(*b), A_value()), *b);
    case left_done:
      return Op()(A_pair(index(*b), A_value()), *b);
    case right_done:
      return Op()(*a, B_pair(index(*a), B_value()));
    case both_done:
      return value_type();// never should get here
    default:
      return value_type();// never should get here
    }
    //return value_type();// never should get here either
  }
  self& operator++() {
    switch (state) {
    case equal_index:
	++a;
	++b;
	if (empty(a))
	  if (empty(b))
	    state = both_done;
	  else
	    state = left_done;
	else if (empty(b))
	  state = right_done;
	else if (index(*a) < index(*b))
	  state = left_less;
	else if (index(*b) < index(*a))
	  state = right_less;
	break;
    case left_less:
      ++a;
      if (empty(a))
	state = left_done;
      else if (index(*a) == index(*b))
	state = equal_index;
      else if (index(*b) < index(*a))
	state = right_less;
      break;
    case right_less:
      ++b;
      if (empty(b))
	state = right_done;
      else if (index(*a) == index(*b))
	state = equal_index;
      else if (index(*a) < index(*b))
	state = left_less;
      break;
    case right_done:
      ++a;
      if (empty(a))
	state = both_done;
      break;
    case left_done:
      ++b;
      if (empty(b))
	state = both_done;
      break;
    case both_done:
      // do nothing
      break;
    }
    return *this; 
  }
  friend bool empty(const self& x) { return x.state == both_done; }
};

// Note, the following sparse-dense for "or" (addition) implementation
// is not as efficient as scattering the sparse into the result, and
// then adding the dense to the result in a second pass. Not sure how
// to work this into the expression template framework.

// sparse-dense, "or" index set
template <class A, class B, class Op>
class bin_op_range<A, B, Op, sparse, dense, or_op>
{
  typedef bin_op_range self;
  A a;
  B b;
  std::size_t bi;
  typedef typename range_traits<A>::value_type A_pair;
  typedef typename A_pair::second_type A_value;
  typedef typename range_traits<B>::value_type B_value;
public:
  typedef typename binary_promote<Op, A_value, B_value>::type value_type;

  bin_op_range( const A& a_, const B& b_ )
    : a(a_), b(b_), bi(0) { }

  value_type operator*() const {
    if (!empty(a) && index(*a) == bi)
      return Op()(value(*a), *b);
    else
      return Op()(A_value(), *b);
  }
  self& operator++() {
    if (!empty(a) && index(*a) == bi) {
      ++a; ++b; ++bi;
    } else {
      ++b; ++bi;
    }
    return *this;
  }  
  friend bool empty(const self& x) { return empty(x.b); }
};

// dense-sparse, "or" index set
template <class A, class B, class Op>
class bin_op_range<A, B, Op, dense, sparse, or_op>
{
  typedef bin_op_range self;
  A a;
  std::size_t ai;
  B b;
  typedef typename range_traits<A>::value_type A_value;
  typedef typename range_traits<B>::value_type B_pair;
  typedef typename B_pair::second_type B_value;
public:
  typedef typename binary_promote<Op, A_value, B_value>::type value_type;

  bin_op_range( const A& a_, const B& b_ )
    : a(a_), ai(0), b(b_) { }

  value_type operator*() const {
    if (!empty(b) && ai == index(*b))
      return Op()(*a, value(*b));
    else
      return Op()(*a, B_value());
  }
  self& operator++() {
    if (!empty(b) && ai == index(*b)) {
      ++a; ++ai; ++b;
    } else {
      ++a; ++ai;
    }
    return *this;
  }  
  friend bool empty(const self& x) { return empty(x.a); }
};

// element-wise unary operator range ===================================
template <class A, class Op>
class unary_op_range {
  typedef unary_op_range self;
  A a;
  typedef typename range_traits<A>::value_type A_value;
public:
  typedef typename unary_promote<Op, A_value>::type value_type;
  unary_op_range(const A& a_)
    : a(a_) {}

  value_type operator*() const { return Op()(*a); }
  value_type operator[](std::ptrdiff_t i) const { return Op()(a[i]); }
  self& operator++() { ++a; return *this; }
  friend bool empty(const self& x) { return empty(x.a); }
};

// drop values under threshold sparse range adaptor ========================
// this could be generalized to use a predicate... call it when_op_range
// and create a when() function.
template <class T, class A>
class drop_op_range {
  typedef drop_op_range self;
  A m_a;
  T m_thresh;
public:
  typedef typename range_traits<A>::value_type value_type;
  drop_op_range(const A& a, const T& threshold)
    : m_a(a), m_thresh(threshold) 
  {
    while (!empty(m_a) && value(*m_a) < m_thresh)
      ++m_a; 
  }

  value_type operator*() const { return *m_a; }
  self& operator++() {
    ++m_a;
    while (!empty(m_a) && value(*m_a) < m_thresh)
      ++m_a; 
    return *this; 
  }
  friend bool empty(const self& x) { return empty(x.m_a); }
};


template <class Predicate, class A, class Sparsity>
class when_op_range { };


template <class Predicate, class A>
class when_op_range<Predicate, A, sparse> {
  typedef when_op_range self;
  A m_a;
  Predicate m_pred;
public:
  typedef typename range_traits<A>::value_type value_type;
  when_op_range(const A& a, const Predicate& pred)
    : m_a(a), m_pred(pred) 
  {
    while (!empty(m_a) && m_pred(value(*m_a)))
      ++m_a; 
  }
  value_type operator*() const { return *m_a; }
  self& operator++() {
    ++m_a;
    while (!empty(m_a) && m_pred(value(*m_a)))
      ++m_a; 
    return *this; 
  }
  friend bool empty(const self& x) { return empty(x.m_a); }
};


template <class Predicate, class A>
class when_op_range<Predicate, A, dense> {
  typedef when_op_range self;
  A m_a;
  Predicate m_pred;
public:
  typedef typename range_traits<A>::value_type value_type;
  when_op_range(const A& a, const Predicate& pred)
    : m_a(a), m_pred(pred) { }
  value_type operator*() const { 
    if (m_pred(*m_a))
      return *m_a; 
    else
      return value_type(); // zero
  }
  self& operator++() { ++m_a; return *this; }
  friend bool empty(const self& x) { return empty(x.m_a); }
};

// scale left range ===================================================
template <class A, class T>
class scale_left_range {
  typedef scale_left_range self;
  typedef typename range_traits<A>::value_type A_value;
public:
  typedef typename binary_promote<op_multiply, A_value, T>::type value_type;
  scale_left_range(const A& a_, const T& b_) : a(a_), b(b_) { }
  value_type operator*() const { return *a * b; }
  value_type operator[](std::ptrdiff_t i) const { return a[i] * b; }
  self& operator++() { ++a; return *this; }
  friend bool empty(const self& x) { return empty(x.a); }
//private:
  A a;
  const T& b;
};

// scale right range ===================================================

template <class T, class B>
class scale_right_range {
  typedef scale_right_range self;
  typedef typename range_traits<B>::value_type B_value;
public:
  typedef typename binary_promote<op_multiply, T, B_value>::type value_type;
  scale_right_range(const T& a_, const B& b_) : a(a_), b(b_) { }
  value_type operator*() const { return a * *b; }
  value_type operator[](std::ptrdiff_t i) const { return a * b[i]; }
  self& operator++() { ++b; return *this; }
  friend bool empty(const self& x) { return empty(x.b); }
//private:
  const T& a;
  B b;
};

#endif // RANGE_ADAPTORS_HPP
