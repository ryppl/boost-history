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

#ifndef MTL_EXPR_HPP
#define MTL_EXPR_HPP

#include <iosfwd>
#include <numeric>
#include <cmath> // for std::abs()
#include <boost/utility.hpp> // for boost::next()
#include <mtl/range_adaptors.hpp>
#include <mtl/eval_expr.hpp>
#include <mtl/algobase.hpp>
#include <mtl/vector_fwd.hpp>
#include <mtl/sparse_pair_vector_fwd.hpp>

template <class C>
class get_range {
public:
  typedef iter_range<typename C::const_iterator,
     typename C::const_iterator> type;
};

// Tag is either vector_tag or matrix_tag
template <class Tag, class Sc, class E, class Orientation, class Sparsity>
class expr
{
public:
  typedef Sparsity sparsity;
  typedef Orientation orientation;
  typedef typename E::value_type value_type;
#if 0
  typedef typename E::size_type size_type;
  typedef typename E::iterator iterator;
  typedef typename E::const_iterator const_iterator;
#endif

  expr(const E& e) : e(e) { }
  expr(const expr& x) : e(x.e) { }

  value_type operator [](std::size_t i) const { return e[i]; }

#if 0
  iterator begin() { return e.begin(); }
  iterator end() { return e.end(); }
  const_iterator begin() const { return e.begin(); }
  const_iterator end() const { return e.end(); }
#endif

  template <class Result, class AssignOp>
  void eval(Result& result, AssignOp assign) const {
    typename get_range<E>::type r = make_range(*this);
    eval_expr(result, r, assign, Sparsity());
  }

  template <class Result>
  void eval(Result& result) const {
    this->eval(result, op_assign());
  }

  typedef typename E::result_type result_type;
  operator const result_type&() const { return e; }

  std::size_t size() const { return e.size(); }
  std::size_t nrows() const { return e.nrows(); }
  std::size_t ncols() const { return e.ncols(); }

//private:
  E e;
};
template <class Tag, class Sc, class E, class Orientation, class Sparsity>
inline typename get_range<E>::type
make_range(const expr<Tag, Sc, E, Orientation, Sparsity>& x) 
  { return make_range(x.e); }


struct vector_tag { };
struct matrix_tag { };

template <class Tag, class Scalar, class T, class Container, 
          class Orientation, class Sparsity>
class linalg_expr {
public:
  typedef linalg_expr expr_type;
  typedef Sparsity sparsity;
  typedef Orientation orien; // don't want to conflict with matrix orientation
  typedef const T& const_reference;
  typedef T& reference;
  typedef T value_type;
  typedef T* pointer;
  typedef const T* const_pointer;
  explicit linalg_expr ( ) {}
  std::size_t length() const {  // this was ambiguous with container adaptor
    return static_cast<const Container*>(this)->size(); 
  }
  std::size_t nrows() const { 
    return static_cast<const Container*>(this)->nrows(); 
  }
  std::size_t ncols() const { 
    return static_cast<const Container*>(this)->ncols(); 
  }

  template <class E, class Sp>
  Container& 
  operator=(const expr<Tag,Scalar,E,Orientation,Sp>& x)
  {
    Container& me = static_cast<Container&>(*this);
    MTL_ASSERT_SHAPE(me.size() == x.size());
    x.eval(me); 
    return me; 
  }

  template <class Sc, class TT, class C, class Sp>
  Container& 
  operator=(const linalg_expr<Tag, Sc,TT,C,Orientation,Sp>& x_) 
  { 
    Container& me = static_cast<Container&>(*this);
    const C& x = static_cast<const C&>(x_);
    MTL_ASSERT_SHAPE(me.size() == x.size());
    x.eval(me, op_assign()); 
    return me; 
  }

  Container& operator=(const Scalar& alpha) {
    Container& me = static_cast<Container&>(*this);
    std::fill(me.begin(), me.end(), alpha);
    return me;
  }

  Container& operator*=(const Scalar& alpha)
  {
    Container& me = static_cast<Container&>(*this);
    for (typename Container::iterator i = me.begin(); i != me.end(); ++i)
      *i *= alpha;
    return me;
  }

  template <class E, class S>
  Container& operator+=(const expr<Tag,Scalar,E,Orientation,S>& x)
  {
    Container& me = static_cast<Container&>(*this);
    if (me.size() != x.size())
      me.resize(x.size());
    x.eval(me, op_assign_add());
    return me;
  }
  template <class E, class S>
  Container& operator-=( const expr<Tag,Scalar,E,Orientation,S>& x )
  {
    Container& me = static_cast<Container&>(*this);
    if (me.size() != x.size())
      me.resize(x.size());
    x.eval(me, op_assign_subtract());
    return me;
  }
  template <class TT, class C, class S>
  Container& operator+=(const linalg_expr<Tag,Scalar,TT,C,Orientation,S>& x)
  {
    Container& me = static_cast<Container&>(*this);
    const C& c = static_cast<const C&>(x);
    if (me.size() != c.size())
      me.resize(c.size());
    x.eval(me, op_assign_add());
    return me;
  }
  template <class TT, class C, class S>
  Container& operator-=(const linalg_expr<Tag,Scalar,TT,C,Orientation,S>& x)
  {
    Container& me = static_cast<Container&>(*this);
    const C& c = static_cast<const C&>(x);
    if (me.size() != c.size())
      me.resize(c.size());
    x.eval(me, op_assign_subtract());
    return me;
  }

  template <class Result, class AssignOp>
  void eval(Result& result, AssignOp assign) const {
    iter_range<typename Container::const_iterator,
               typename Container::const_iterator>
      r = make_range(*this);
    eval_expr(result, r, assign, Sparsity());
  }

};
// naming this make_range() would conflict with container_adaptor
template <class Tag, class Sc, class T, class C, class O, class S>
inline iter_range<typename C::const_iterator,typename C::const_iterator>
make_range(const linalg_expr<Tag,Sc,T,C,O,S>& c_) {
  const C& c = static_cast<const C&>(c_);
  typedef iter_range<typename C::const_iterator,typename C::const_iterator>
    Range;
  return Range(c.begin(), c.end());
}


template <class Tag, class Sc, class T, class C, class O, class S>
class get_range< linalg_expr<Tag, Sc, T,C,O,S> > 
{
public:
  typedef iter_range<typename C::const_iterator,
     typename C::const_iterator> type;
};

template <class Tag, class Sc, class E, class O, class S>
class get_range< expr<Tag, Sc, E, O, S> >
{
public:
  typedef typename get_range<E>::type type;
};


template <class ContainerExpr>
class const_ref {
  const ContainerExpr& c;
  typedef typename ContainerExpr::const_reference const_reference;
public:
  typedef typename ContainerExpr::sparsity sparsity;
  typedef typename ContainerExpr::value_type value_type;
  const_ref( const ContainerExpr& c_) : c(c_) {}
  const_reference operator[] (std::size_t n) const { return c[n]; }
  std::size_t size() const { return c.length(); }
  std::size_t nrows() const { return c.nrows(); }
  std::size_t ncols() const { return c.ncols(); }
  const ContainerExpr& get_obj() const { return c; }
};

template <class C>
class get_range< const_ref<C> >
{
public:
  typedef typename get_range<C>::type type;
};

template <class ContainerExpr>
inline typename get_range<ContainerExpr>::type
make_range(const const_ref<ContainerExpr>& c) {
  return make_range(c.get_obj());
}

// main definition of to_expr is in promote.hpp
// if already an expression, leave as is
template <class Tag, class Sc, class E, class Orientation, class Sparsity>
class to_expr< expr<Tag, Sc, E, Orientation, Sparsity> >  {
public:
  typedef expr<Tag, Sc, E, Orientation, Sparsity> type;
};
template <class Tag, class Sc, class T, class C, class O, class S>
class to_expr< linalg_expr<Tag,Sc,T,C,O,S> >  {
public:
  typedef linalg_expr<Tag,Sc,T,C,O,S> type;
};

//============== ostream operators ===================================

template <class Tag, class Sc, class T, class C, class O, class S>
std::ostream& 
operator<<(std::ostream& os, const linalg_expr<Tag,Sc,T,C,O,S>& x)
{
  const C& c = static_cast<const C&>(x);
  os << "[";
  for (typename C::const_iterator i = c.begin(); i != c.end(); ++i) {
    os << *i;
    if (boost::next(i) != c.end())
      os << ", ";
  }
  os << "]";
  return os;
}

template <class Sc, class T, class Mat>
std::ostream&
operator<<(std::ostream& os, 
           const linalg_expr<matrix_tag, Sc, T, Mat, row, sparse>& A_)
{
  const Mat& A = static_cast<const Mat&>(A_);
  typename Mat::size_type r = 0;
  for (typename Mat::const_iterator i = A.begin();
       i != A.end(); ++i, ++r)
    for (typename Mat::value_type::const_iterator j = (*i).begin();
         j != (*i).end(); ++j)
      os << "(" << r << "," << index(*j) << ")\t" << value(*j) << std::endl;
  return os;
}

template <class Sc, class T, class Mat>
std::ostream&
operator<<(std::ostream& os, 
           const linalg_expr<matrix_tag, Sc, T, Mat, column, sparse>& A_)
{
  const Mat& A = static_cast<const Mat&>(A_);
  typename Mat::size_type c = 0;
  for (typename Mat::const_iterator i = A.begin();
       i != A.end(); ++i, ++c)
    for (typename Mat::value_type::const_iterator j = (*i).begin();
         j != (*i).end(); ++j)
      os << "(" << index(*j) << "," << c << ")\t" << value(*j) << std::endl;
  return os;
}


//============== sum() ===================================

template <class Sc, class T, class C, class O, class Sp>
Sc sum(const linalg_expr<vector_tag, Sc, T, C, O, Sp>& x)
{
  const C& c = static_cast<const C&>(x);
  return std::accumulate(c.begin(), c.end(), Sc());
}

//============== diag() ===================================

template <class Sc, class T, class Matrix, class O, class Sp>
typename Matrix::const_diagonal_view
diag(const linalg_expr<matrix_tag, Sc, T, Matrix, O, Sp>& A_)
{
  const Matrix& A = static_cast<const Matrix&>(A_);
  return A.diag();
}

template <class Sc, class T, class Matrix, class O, class Sp>
typename Matrix::diagonal_view
diag(linalg_expr<matrix_tag, Sc, T, Matrix, O, Sp>& A_)
{
  Matrix& A = static_cast<Matrix&>(A_);
  return A.diag();
}


//============== trace() ===================================

template <class Sc, class T, class C, class O, class Sp>
Sc trace(const linalg_expr<matrix_tag, Sc, T, C, O, Sp>& A)
{
  return sum(diag(A));
}


//============== two_norm() ===================================

struct sqr_add
{
  template <class T, class U>
  T operator()(const T& a, const U& b) {
    return a + std::abs(b * b);
  }
};

template <class Tag, class Sc, class T, class C, class O, class Sp>
inline Sc
sum_squares(const linalg_expr<Tag,Sc,T,C,O,Sp>& x)
{ 
  const C& c = static_cast<const C&>(x);
  return std::accumulate(c.begin(), c.end(), Sc(), sqr_add());
}

template <class Tag, class Sc, class T, class C, class O, class Sp>
inline Sc
two_norm(const linalg_expr<Tag,Sc,T,C,O,Sp>& x)
{ 
  return std::sqrt(sum_squares(x));
}

struct sum_sqr_add
{
  template <class T, class U>
  T operator()(const T& a, const U& b) {
    return a + sum_squares(b);
  }
};

template <class Sc, class T, class C, class O, class Sp>
inline Sc
frobenius_norm(const linalg_expr<matrix_tag,Sc,T,C,O,Sp>& x)
{ 
  const C& c = static_cast<const C&>(x);
  return std::sqrt(std::accumulate(c.begin(), c.end(), Sc(), sum_sqr_add()));
}

//============== comparison operators ===================================


template <class Tag, class Sc, class T1, class C1, class T2, class C2,
          class O, class S>
inline bool equals(const linalg_expr<Tag,Sc,T1,C1,O,S>& a_, 
                   const linalg_expr<Tag,Sc,T2,C2,O,S>& b_, Sc tolerance)
{
  const C1& a = static_cast<const C1&>(a_);
  const C2& b = static_cast<const C2&>(b_);

  typename C1::const_iterator ai = a.begin();
  typename C2::const_iterator bi = b.begin();
  for (; ai != a.end(); ++ai, ++bi)
    if (std::abs(*ai - *bi) > tolerance)
      return false;
  return true;
}


template <class Tag, class Sc, class T1, class C1, class T2, class C2,
          class O, class S>
inline bool operator==(const linalg_expr<Tag,Sc,T1,C1,O,S>& a_, 
                       const linalg_expr<Tag,Sc,T2,C2,O,S>& b_)
{
  const C1& a = static_cast<const C1&>(a_);
  const C2& b = static_cast<const C2&>(b_);
  return std::equal(a.begin(), a.end(), b.begin());
}

template <class Tag, class Sc, class T1, class C1, class T2, class C2,
          class O, class S>
inline bool operator!=(const linalg_expr<Tag,Sc,T1,C1,O,S>& a_, 
                       const linalg_expr<Tag,Sc,T2,C2,O,S>& b_)
{
  return !(a == b);
}

template <class Tag, class Sc, class A, class B, class Orien, class Sparsity>
inline bool
operator == (const expr<Tag,Sc,A,Orien,Sparsity>& ax,
             const expr<Tag,Sc,B,Orien,Sparsity>& bx)
{ 
  typename get_range<A>::type a = make_range(ax);
  typename get_range<B>::type b = make_range(bx);
  bool result = true;
  detail::inner_product(a, b, ax.size(), result, Sparsity(), Sparsity(),
                        op_assign_and(), op_equal());
  return result;
}
template <class Tag, class Sc, class A, class B, class Orien, class Sparsity>
inline bool
operator != (const expr<Tag,Sc,A,Orien,Sparsity>& ax,
             const expr<Tag,Sc,B,Orien,Sparsity>& bx)
{
  return !(ax == bx);
}

template <class Tag, class Sc, class T, class C, class B, 
          class Orien, class Sparsity>
inline bool
operator == (const linalg_expr<Tag,Sc,T,C,Orien,Sparsity>& ax,
             const expr<Tag,Sc,B,Orien,Sparsity>& bx)
{ 
  const C& ac = static_cast<const C&>(ax);
  iter_range<typename C::const_iterator,typename C::const_iterator>
    a = make_range(ax);
  typename get_range<B>::type b = make_range(bx);
  bool result = true;
  detail::inner_product(a, b, ac.size(), result, Sparsity(), Sparsity(),
                        op_assign_and(), op_equal());
  return result;
}
template <class Tag, class Sc, class T, class C, class B, 
          class Orien, class Sparsity>
inline bool
operator != (const linalg_expr<Tag,Sc,T,C,Orien,Sparsity>& ax,
             const expr<Tag,Sc,B,Orien,Sparsity>& bx)
{
  return !(ax == bx);
}

template <class Tag, class Sc, class A, class T, class C, class Orien, 
          class Sparsity>
inline bool
operator == (const expr<Tag,Sc,A,Orien,Sparsity>& ax,
             const linalg_expr<Tag,Sc,T,C,Orien,Sparsity>& bx)
{ 
  typename get_range<A>::type a = make_range(ax);
  iter_range<typename C::const_iterator,typename C::const_iterator>
    b = make_range(bx);
  bool result = true;
  detail::inner_product(a, b, ax.size(), result, Sparsity(), Sparsity(),
                        op_assign_and(), op_equal());
  return result;
}
template <class Tag, class Sc, class A, class T, class C, class Orien, 
          class Sparsity>
inline bool
operator != (const expr<Tag,Sc,A,Orien,Sparsity>& ax,
             const linalg_expr<Tag,Sc,T,C,Orien,Sparsity>& bx)
{
  return !(ax == bx);
}

//============== element-wise binary operators ==========================

struct no_implicit_result { };

template <class A, class B, class Op, class LeftSparsity, class RightSparsity,
          class Combine>
class bin_op_expr {
  typedef typename A::value_type A_value;
  typedef typename B::value_type B_value;
public:
  typedef no_implicit_result result_type;
  typedef typename binary_promote<Op, A_value, B_value>::type value_type;
  bin_op_expr(const A& a_, const B& b_) : a(a_), b(b_) {}
  std::size_t size() const { return a.size(); }
  std::size_t nrows() const { return a.nrows(); }
  std::size_t ncols() const { return a.ncols(); }
  value_type operator[] (std::size_t i) const {
    return Op()(a[i], b[i]);
  }
//private:
  A a;
  B b;
};
template <class A, class B, class Op, class LS, class RS, class Comb>
class get_range< bin_op_expr<A,B,Op,LS,RS,Comb> > {
public:
  typedef bin_op_range< typename get_range<A>::type,
                typename get_range<B>::type, Op, LS, RS, Comb> type;
};

template <class A, class B, class Op, class LS, class RS, class Comb>
inline 
bin_op_range<typename get_range<A>::type, typename get_range<B>::type,
             Op, LS, RS, Comb>
make_range(const bin_op_expr<A, B, Op, LS, RS, Comb>& bin_expr)
{
  typedef bin_op_range< typename get_range<A>::type,
    typename get_range<B>::type, Op, LS, RS, Comb> BinRange;
  return BinRange(make_range(bin_expr.a), make_range(bin_expr.b));
}

// Binary operations between linalg_expr's and expr's
#define EXPR_DEFINE_GLOBAL_BINARY_OPERATOR(OP, FUN, COMBINE) \
template <class Tag, class Sc, class T, class A, class B, class O, class LS, class RS> \
class binary_promote<FUN, linalg_expr<Tag,Sc,T,A,O,LS>, \
       expr<Tag,Sc,B,O,RS> > \
{ \
  typedef typename or_sparsity<LS, RS>::type Sparsity; \
public: \
  typedef linalg_expr<Tag,Sc,T,A,O,LS> A_type; \
  typedef expr<Tag,Sc, bin_op_expr< const_ref< A_type >, \
    expr<Tag,Sc,B,O,RS>, \
    FUN, LS, RS, COMBINE>, O, Sparsity> type; \
}; \
\
template <class Tag, class Sc, class T, class A, class B, class O, class LS, class RS> \
typename binary_promote<FUN, linalg_expr<Tag,Sc,T,A,O,LS>, \
                 expr<Tag,Sc,B,O,RS> >::type \
operator OP (const linalg_expr<Tag,Sc,T,A,O,LS>& a, \
             const expr<Tag,Sc,B,O,RS>& b) \
{ \
  typedef typename or_sparsity<LS, RS>::type Sparsity; \
  typedef linalg_expr<Tag,Sc,T,A,O,LS> A_type; \
  typedef expr<Tag,Sc,B,O,RS> B_type; \
  typedef bin_op_expr< const_ref< A_type >, \
    B_type, FUN, \
    LS, RS, COMBINE > ExprT; \
  return expr<Tag,Sc,ExprT,O,Sparsity>( ExprT( \
     const_ref<A_type>(a), b ) ) ; \
} \
\
template <class Tag, class Sc, class A, class T, class B, class O, class LS, class RS> \
class binary_promote<FUN, expr<Tag,Sc,A,O,LS>, \
  linalg_expr<Tag,Sc,T,B,O,RS> > \
{ \
  typedef typename or_sparsity<LS, RS>::type Sparsity; \
public: \
  typedef expr<Tag,Sc, A,O,LS> A_type; \
  typedef linalg_expr<Tag,Sc,T,B,O,RS> B_type; \
  typedef expr<Tag, Sc, bin_op_expr< A_type, \
      const_ref<B_type>, FUN, LS, RS, COMBINE >, O, Sparsity > type; \
}; \
\
template <class Tag, class Sc, class A, class T, class B, class O, class LS, class RS> \
typename binary_promote< FUN, expr<Tag,Sc,A,O,LS>, \
   linalg_expr<Tag,Sc,T,B,O,RS> >::type \
operator OP (const expr<Tag,Sc, A,O,LS>& a, \
             const linalg_expr<Tag,Sc,T,B,O,RS>& b) \
{ \
  typedef typename or_sparsity<LS, RS>::type Sparsity; \
  typedef expr<Tag,Sc,A,O,LS> A_type; \
  typedef linalg_expr<Tag,Sc,T,B,O,RS> B_type; \
  typedef bin_op_expr< A_type , \
    const_ref<B_type>, FUN, LS, RS, COMBINE > ExprT; \
  return expr<Tag,Sc,ExprT,O,Sparsity>( ExprT( a, const_ref<B_type>(b) ) ) ; \
} \
\
template <class Tag, class Sc, class A, class B, class O, class LS, class RS> \
class binary_promote<FUN, expr<Tag,Sc,A,O,LS>, \
  expr<Tag,Sc,B,O,RS> > { \
  typedef typename or_sparsity<LS, RS>::type Sparsity; \
public: \
  typedef expr<Tag,Sc,bin_op_expr< expr<Tag,Sc,A,O,LS>, \
    expr<Tag,Sc,B,O,RS>, FUN, LS, RS, COMBINE>, O, Sparsity> type;\
};\
\
template <class Tag, class Sc,class A, class B, class O, class LS, class RS> \
  typename binary_promote<FUN, expr<Tag,Sc,A,O,LS>, \
    expr<Tag,Sc,B,O,RS> >::type \
operator OP (const expr<Tag,Sc,A,O,LS>& a, \
             const expr<Tag,Sc,B,O,RS>& b) \
{ \
  typedef typename or_sparsity<LS, RS>::type Sparsity; \
  typedef bin_op_expr< expr<Tag,Sc,A,O,LS> , \
    expr<Tag,Sc,B,O,RS>, FUN, LS, RS, COMBINE > ExprT; \
  return expr<Tag,Sc,ExprT,O,Sparsity>( ExprT(a, b) ) ; \
} \
\
template <class Tag, class Sc, class T1, class T2, class A, class B, class O, class LS, class RS> \
class binary_promote<FUN, linalg_expr<Tag,Sc,T1,A,O,LS>, \
  linalg_expr<Tag,Sc,T2,B,O,RS> > \
{ \
  typedef typename or_sparsity<LS, RS>::type Sparsity; \
public:\
  typedef expr<Tag,Sc, bin_op_expr< \
   const_ref< linalg_expr<Tag,Sc,T1,A,O,LS> >,\
   const_ref< linalg_expr<Tag,Sc,T2,B,O,RS> >, \
   FUN, LS, RS, COMBINE >, O,Sparsity> type; \
}; \
template <class Tag, class Sc, class T1, class T2, class A, class B, class O, class LS, class RS> \
typename binary_promote<FUN, linalg_expr<Tag,Sc,T1,A,O,LS>, \
  linalg_expr<Tag,Sc,T2,B,O,RS> >::type \
operator OP (const linalg_expr<Tag,Sc,T1,A,O,LS>& a, \
             const linalg_expr<Tag,Sc,T2,B,O,RS>& b) \
{ \
  typedef typename or_sparsity<LS, RS>::type Sparsity; \
  typedef linalg_expr<Tag,Sc,T1,A,O,LS> A_type; \
  typedef linalg_expr<Tag,Sc,T2,B,O,RS> B_type; \
  typedef bin_op_expr< const_ref<A_type>, const_ref<B_type>, \
              FUN, LS, RS, COMBINE > ExprT; \
  return expr<Tag,Sc,ExprT,O,Sparsity>( ExprT( \
     const_ref<A_type>(a), const_ref<B_type>(b) ) ) ; \
}

EXPR_DEFINE_GLOBAL_BINARY_OPERATOR(+, op_add, or_op)
EXPR_DEFINE_GLOBAL_BINARY_OPERATOR(-, op_subtract, or_op)
#undef EXPR_DEFINE_GLOBAL_BINARY_OPERATOR


// ============ unary operators =============================

template <class A, class Op>
class unary_op_expr
{
  typedef typename A::value_type A_value;
public:
  typedef no_implicit_result result_type;
  typedef typename unary_promote<Op, A_value>::type value_type;
  unary_op_expr(const A& a_)
    : a(a_) {}
  value_type operator[] (std::size_t n) const {
    return Op()(a[n]);
  }
  std::size_t size() const { return a.size(); }
  std::size_t nrows() const { return a.nrows(); }
  std::size_t ncols() const { return a.ncols(); }
//private:
  A a;
};
template <class A, class Op>
class get_range< unary_op_expr<A,Op> >
{
public:
  typedef unary_op_range<typename get_range<A>::type, Op> type;
};
template <class A, class Op>
inline 
unary_op_range< typename get_range<A>::type, Op>
make_range(const unary_op_expr<A, Op>& unary_expr)
{
  typedef unary_op_range< typename get_range<A>::type, Op> Range;
  return Range(make_range(unary_expr.a));
}


#define EXPR_DEFINE_GLOBAL_UNARY_PROMOTE(FUN) \
template <class Tag, class Sc, class T, class A, class O, class S> \
class unary_promote<FUN, linalg_expr<Tag,Sc,T,A,O,S> > { \
public: \
  typedef expr<Tag, Sc, unary_op_expr<const_ref< linalg_expr<Tag,Sc,T,A,O,S> >,  \
    FUN >, O, S> type; \
}; \
 \
template <class Tag, class Sc,class A, class O, class S> \
class unary_promote<FUN, expr<Tag,Sc,A,O,S> > { \
public: \
  typedef expr<Tag, Sc, unary_op_expr< expr<Tag,Sc,A,O,S>, FUN >, O, S> type; \
};

#define EXPR_DEFINE_GLOBAL_UNARY_OPERATOR(OP,FUN) \
template <class Tag, class Sc, class T, class A, class O, class S> \
  typename unary_promote<FUN, linalg_expr<Tag,Sc, T,A,O,S> >::type \
OP ( const linalg_expr<Tag,Sc,T,A,O,S>& a) \
{ \
  typedef unary_op_expr< const_ref< linalg_expr<Tag,Sc,T,A,O,S> >, \
    FUN > ExprT; \
  return expr<Tag,Sc,ExprT,O,S>( ExprT( \
     const_ref< linalg_expr<Tag,Sc,T,A,O,S> >(a)) ) ; \
} \
\
template <class Tag, class Sc, class A, class O, class S> \
  typename unary_promote< FUN, expr<Tag,Sc,A,O,S> >::type \
OP ( const expr<Tag,Sc,A,O,S>& a) \
{ \
  typedef unary_op_expr< expr<Tag,Sc,A,O,S>, FUN > ExprT; \
  return expr<Tag,Sc,ExprT,O,S>( ExprT(a) ) ; \
}

EXPR_DEFINE_GLOBAL_UNARY_PROMOTE(op_negative)
EXPR_DEFINE_GLOBAL_UNARY_OPERATOR(operator-, op_negative)
EXPR_DEFINE_GLOBAL_UNARY_PROMOTE(op_conj)
EXPR_DEFINE_GLOBAL_UNARY_PROMOTE(op_abs)
namespace std {
  EXPR_DEFINE_GLOBAL_UNARY_OPERATOR(conj, op_conj)
  EXPR_DEFINE_GLOBAL_UNARY_OPERATOR(abs, op_abs)
}
#undef EXPR_DEFINE_GLOBAL_UNARY_OPERATOR


// when op ==============================================================

// filter based on indices

template <class Predicate, class A>
class when_op_expr
{
public:
  typedef typename A::value_type value_type;
  typedef no_implicit_result result_type;
  when_op_expr(const A& a_, const Predicate& p_)
    : a(a_), p(p_) {}
  std::size_t size() const { return a.size(); }
  std::size_t nrows() const { return a.nrows(); }
  std::size_t ncols() const { return a.ncols(); }
//private:
  A a;
  Predicate p;
};
template <class P, class A>
class get_range< when_op_expr<P,A> >
{
public:
  typedef when_op_range<P, typename get_range<A>::type, 
     typename A::sparsity> type;
};
template <class P, class A>
inline 
when_op_range<P, typename get_range<A>::type, typename A::sparsity>
make_range(const when_op_expr<P, A>& when_expr)
{
  typedef when_op_range<P, typename get_range<A>::type, 
    typename A::sparsity> Range;
  return Range(make_range(when_expr.a), when_expr.p);
}

template <class P>
class op_when { };

template <class P, class Tag, class Sc, class T, class A, class O, class Sp>
class unary_promote<op_when<P>, linalg_expr<Tag,Sc,T,A,O,Sp> > {
public:
  typedef expr<Tag, Sc, when_op_expr< P, 
      const_ref< linalg_expr<Tag,Sc,T,A,O,Sp> > >, 
      O, Sp> type;
};
template <class P, class Tag, class Sc, class T, class A, class O, class Sp>
  typename unary_promote< op_when<P>, linalg_expr<Tag,Sc,T,A,O,Sp> >::type
when(const linalg_expr<Tag,Sc,T,A,O,Sp>& a, const P& p)
{
  typedef const_ref< linalg_expr<Tag,Sc,T,A,O,Sp> > A_ref;
  typedef when_op_expr<P, A_ref> ExprT;
  return expr<Tag,Sc,ExprT,O,Sp>( ExprT(A_ref(a), p) ) ;
}

template <class P, class Tag, class Sc,class A, class O, class Sp>
class unary_promote<op_when<P>, expr<Tag,Sc,A,O,Sp> > {
public:
  typedef expr<Tag, Sc, when_op_expr< P, expr<Tag,Sc,A,O,Sp> >, 
      O, Sp> type;
};
template <class P, class Tag, class Sc, class A, class O, class Sp>
  typename unary_promote< op_when<P>, expr<Tag,Sc,A,O,Sp> >::type
when(const expr<Tag,Sc,A,O,Sp>& a, const P& p)
{
  typedef when_op_expr<P, expr<Tag,Sc,A,O,Sp> > ExprT;
  return expr<Tag,Sc,ExprT,O,Sp>( ExprT(a, p) ) ;
}

// would also be nice to filter based on indices

// ================== scale right ============================

template <class T, class B>
class scale_right_expr {
  typedef typename B::value_type B_value;
public:
  typedef no_implicit_result result_type;
  typedef typename binary_promote<op_multiply, T, B_value>::type value_type;
  scale_right_expr(const T& a_, const B& b_) : a(a_), b(b_) {}
  value_type operator[] (std::size_t i) const { return a * b[i]; }
  std::size_t size() const { return b.size(); }
  std::size_t nrows() const { return b.nrows(); }
  std::size_t ncols() const { return b.ncols(); }
//private:
  const T& a;
  B b;
};
template <class T, class B>
class get_range< scale_right_expr<T,B> >
{
public:
  typedef scale_right_range<T, typename get_range<B>::type> type;
};

template <class T, class B>
inline scale_right_range<T, typename get_range<B>::type>
make_range(const scale_right_expr<T, B>& e) {
  return scale_right_range<T, typename get_range<B>::type>(e.a, make_range(e.b));
}

template <class Tag, class Sc, class T, class B, class O, class Sparsity>
class binary_promote<op_multiply, Sc, linalg_expr<Tag,Sc,T,B,O,Sparsity> >{
public:
  typedef expr<Tag,Sc, scale_right_expr< Sc,
    const_ref< linalg_expr<Tag,Sc,T,B,O,Sparsity> > >, O, Sparsity> type;
};
template <class Tag, class Sc, class T, class B, class O, class Sparsity> 
typename binary_promote<op_multiply, Sc, linalg_expr<Tag,Sc,T,B,O,Sparsity> >::type 
operator * (const Sc& a, const linalg_expr<Tag,Sc,T,B,O,Sparsity>& b) 
{ 
  typedef scale_right_expr<Sc,const_ref<linalg_expr<Tag,Sc,T,B,O,Sparsity> > > ExprT; 
  return expr<Tag,Sc,ExprT,O,Sparsity>( ExprT(a,
     const_ref< linalg_expr<Tag,Sc,T,B,O,Sparsity> >(b) ) );
}

template <class Tag, class Sc, class B, class O, class Sparsity>
class binary_promote<op_multiply, Sc, expr<Tag,Sc,B,O,Sparsity> >{
public:
  typedef expr<Tag,Sc, scale_right_expr<Sc, expr<Tag,Sc,B,O,Sparsity> >, O, Sparsity> type;
};
template <class Tag, class Sc, class B, class O, class Sparsity> 
typename binary_promote<op_multiply, Sc, expr<Tag,Sc,B,O,Sparsity> >::type 
operator * (const Sc& a, const expr<Tag,Sc,B,O,Sparsity>& b) 
{ 
  typedef scale_right_expr<Sc, expr<Tag,Sc,B,O,Sparsity> > ExprT; 
  return expr<Tag,Sc,ExprT,O,Sparsity>( ExprT(a, b) );
}

// allow "pair" scalars for scalars coming from sparse vectors

template <class Tag, class Idx, class Sc, class T, class B, class O, 
          class Sparsity>
class binary_promote<op_multiply, mtl::pair<Idx,Sc>, 
  linalg_expr<Tag,Sc,T,B,O,Sparsity> >
{
public:
  typedef expr<Tag, Sc, scale_right_expr< Sc,
    const_ref< linalg_expr<Tag,Sc,T,B,O,Sparsity> > >, O, Sparsity> type;
};
template <class Tag, class Idx, class Sc, class T, class B, class O, 
          class Sparsity> 
typename binary_promote<op_multiply, mtl::pair<Idx,Sc>,
              linalg_expr<Tag,Sc,T,B,O,Sparsity> >::type 
operator * (const mtl::pair<Idx,Sc>& a, 
            const linalg_expr<Tag,Sc,T,B,O,Sparsity>& b) 
{
  typedef scale_right_expr<Sc,
           const_ref<linalg_expr<Tag,Sc,T,B,O,Sparsity> > > ExprT; 
  return expr<Tag,Sc,ExprT,O,Sparsity>( ExprT(a.second,
     const_ref< linalg_expr<Tag,Sc,T,B,O,Sparsity> >(b) ) );
}

template <class Tag, class Idx, class Sc, class B, class O, class Sparsity>
class binary_promote<op_multiply, mtl::pair<Idx,Sc>, expr<Tag,Sc,B,O,Sparsity> >
{
public:
  typedef expr<Tag,Sc, scale_right_expr<Sc, expr<Tag,Sc,B,O,Sparsity> >, O, 
    Sparsity> type;
};
template <class Tag, class Idx, class Sc, class B, class O, class Sparsity> 
typename binary_promote<op_multiply, mtl::pair<Idx,Sc>,
      expr<Tag,Sc,B,O,Sparsity> >::type 
operator * (const mtl::pair<Idx,Sc>& a, const expr<Tag,Sc,B,O,Sparsity>& b) 
{ 
  typedef scale_right_expr<Sc, expr<Tag,Sc,B,O,Sparsity> > ExprT; 
  return expr<Tag,Sc,ExprT,O,Sparsity>( ExprT(a.second, b) );
}


// ================== scale left ============================


template <class A, class T>
class scale_left_expr {
  typedef typename A::value_type A_value;
public:
  typedef no_implicit_result result_type;
  typedef typename binary_promote<op_multiply, A_value, T>::type value_type;
  scale_left_expr( const A& a_, const T& b_ )
    : a(a_), b(b_) {}
  value_type operator[] (std::size_t i) const { return a[i] * b; }
  std::size_t size() const { return a.size(); }
  std::size_t nrows() const { return a.nrows(); }
  std::size_t ncols() const { return a.ncols(); }
//private:
  A a;
  const T& b;
};
template <class A, class T>
class get_range< scale_left_expr<A,T> >
{
public:
  typedef scale_left_range<typename get_range<A>::type, T> type;
};
template <class A, class T>
inline scale_left_range<typename get_range<A>::type, T>
make_range(const scale_left_expr<A,T>& e) {
  return scale_left_range<typename get_range<A>::type, T>(make_range(e.a), 
                                                          e.b);
}


template <class Tag, class Sc, class T, class A, class O, class Sparsity>
class binary_promote<op_multiply, linalg_expr<Tag,Sc,T,A,O,Sparsity>, Sc >{
  typedef linalg_expr<Tag,Sc,T,A,O,Sparsity> A_type;
  typedef Sc B_type;
public:
  typedef expr<Tag, Sc, scale_left_expr<
    const_ref<A_type>, B_type>, O, Sparsity> type;
};
template <class Tag, class Sc, class T, class A, class O, class Sparsity> 
typename binary_promote<op_multiply, linalg_expr<Tag,Sc,T,A,O,Sparsity>, Sc >::type 
operator * (const linalg_expr<Tag,Sc,T,A,O,Sparsity>& a, const Sc& b) 
{ 
  typedef scale_left_expr<const_ref<linalg_expr<Tag,Sc,T,A,O,Sparsity> >, Sc > ExprT; 
  return expr<Tag,Sc,ExprT,O,Sparsity>( ExprT(
     const_ref< linalg_expr<Tag,Sc,T,A,O,Sparsity> >(a), b ) );
}

template <class Tag, class Sc, class A, class O, class Sparsity>
class binary_promote<op_multiply, expr<Tag,Sc,A,O,Sparsity>, Sc>{
public:
  typedef expr<Tag,Sc, scale_left_expr< expr<Tag,Sc,A,O,Sparsity>, Sc>, O, Sparsity> type;
};
template <class Tag, class Sc, class A, class O, class Sparsity> 
typename binary_promote<op_multiply, expr<Tag,Sc,A,O,Sparsity>, Sc>::type 
operator * (const expr<Tag,Sc,A,O,Sparsity>& a, const Sc& b) 
{ 
  typedef scale_left_expr<expr<Tag,Sc,A,O,Sparsity>, Sc> ExprT; 
  return expr<Tag,Sc,ExprT,O,Sparsity>( ExprT(a, b) );
}

// allow "pair" scalars for scalars coming from sparse vectors
template <class Tag, class Idx, class Sc, class T, class A, class O, class Sparsity>
class binary_promote<op_multiply, linalg_expr<Tag,Sc,T,A,O,Sparsity>, 
  mtl::pair<Idx,Sc> >
{
public:
  typedef expr<Tag, Sc, scale_left_expr< 
    const_ref< linalg_expr<Tag,Sc,T,A,O,Sparsity> >, Sc>, O, Sparsity> type;
};
template <class Tag, class Idx, class Sc, class T, class A, class O, class Sparsity> 
typename binary_promote<op_multiply, linalg_expr<Tag,Sc,T,A,O,Sparsity>, 
  mtl::pair<Idx,Sc> >::type 
operator * (const linalg_expr<Tag,Sc,T,A,O,Sparsity>& a, 
            const mtl::pair<Idx,Sc>& b) 
{ 
  typedef scale_left_expr<const_ref<linalg_expr<Tag,Sc,T,A,O,Sparsity> >, Sc> ExprT; 
  return expr<Tag,Sc,ExprT,O,Sparsity>( ExprT(
     const_ref< linalg_expr<Tag,Sc,T,A,O,Sparsity> >(a), b.second ) );
}


template <class Tag, class Idx, class Sc, class A, class O, class Sparsity>
class binary_promote<op_multiply, expr<Tag,Sc,A,O,Sparsity>, mtl::pair<Idx,Sc> >
{
public:
  typedef expr<Tag,Sc, scale_left_expr< expr<Tag,Sc,A,O,Sparsity>, Sc>, O, Sparsity> type;
};
template <class Tag, class Idx, class Sc, class A, class O, class Sparsity> 
typename binary_promote<op_multiply, expr<Tag,Sc,A,O,Sparsity>, 
  mtl::pair<Idx,Sc> >::type 
operator * (const expr<Tag,Sc,A,O,Sparsity>& a, const mtl::pair<Idx,Sc>& b) 
{ 
  typedef scale_left_expr<expr<Tag,Sc,A,O,Sparsity>, Sc> ExprT; 
  return expr<Tag,Sc,ExprT,O,Sparsity>( ExprT(a, b.second) );
}


// =================== inner product ========================

template <class E>
class expr_to_concrete { };

template <class Sc, class E, class Orien>
class expr_to_concrete< expr<vector_tag, Sc, E, Orien, dense> >
{
public:
  typedef typename mtl::vector<Sc, Orien>::type type;
};

template <class Sc, class E, class Orien>
class expr_to_concrete< expr<vector_tag, Sc, E, Orien, sparse> >
{
public:
  typedef typename mtl::sparse_pair_vector<Sc, Orien>::type type;
};


template <>
class expr_to_concrete<double>
{
public:
  typedef double type;
};

template <class A, class B>
class expr_to_concrete< mtl::pair<A,B> >
{
public:
  typedef mtl::pair<A,B> type;
};

template <class T>
class to_value_type {
public:
  typedef T type;
};

template <class Idx, class T>
class to_value_type< mtl::pair<Idx,T> > {
public:
  typedef typename to_value_type<T>::type type;
};


// This will be a problem...
template <class C>
class get_value {
public:
  typedef typename C::value_type type;
};

template <>
class get_value<double> {
public:
  typedef double type;
};

template <class A, class B, class LeftSparsity, class RightSparsity>
class inner_product_expr
{
  typedef typename A::value_type A_val;
  typedef typename B::value_type B_val;

  typedef typename to_value_type<A_val>::type A_value;
  typedef typename to_value_type<B_val>::type B_value;

  typedef typename binary_promote<op_multiply, A_value, B_value>::type
    value_expr_type;
public:
  typedef typename expr_to_concrete<value_expr_type>::type result_type;

  typedef typename get_value<result_type>::type value_type;

  inner_product_expr(const A& a, const B& b)
    : m_evaluated(false), m_a(a), m_b(b),
      m_result(result_type()) // explicit initialization needed for g++
  { }
  
  std::size_t size() const {
    return std::max(m_a.nrows(), m_b.ncols());
  }

  template <class Result>
  void eval(Result& result) const
  {
    m_evaluated = true;
    typename get_range<A>::type a = make_range(m_a);
    typename get_range<B>::type b = make_range(m_b);
    detail::inner_product(a, b, m_a.size(), result, 
                          LeftSparsity(), RightSparsity());
  }

  operator const result_type&() const { 
    if (!m_evaluated) {
      this->eval(m_result);
      m_evaluated = true;
    }
    return m_result; 
  }

//private:
  mutable bool m_evaluated;
  A m_a;
  B m_b;
  mutable result_type m_result;
};
template <class A, class B, class LS, class RS>
class get_range< inner_product_expr<A, B, LS, RS> >
{
  typedef typename inner_product_expr<A,B,LS,RS>::result_type C;
public:
  typedef iter_range<typename C::const_iterator,
   typename C::const_iterator> type;
};

template <class A, class B, class LS, class RS>
inline
iter_range<typename inner_product_expr<A,B,LS,RS>::result_type::const_iterator,
  typename inner_product_expr<A,B,LS,RS>::result_type::const_iterator>
make_range(const inner_product_expr<A,B,LS,RS>& x)
{
  if (!x.m_evaluated)
    x.eval(x.m_result);
  return make_range(x.m_result);
}

// Have to make sure inner_product expressions can interact with
// scalars.
template <class Op, class Tag, class O, class S, class T,
          class A, class B, class LS, class RS>
class binary_promote<Op, T, expr<Tag, T, inner_product_expr<A,B,LS,RS>, O, S> >
{
  typedef typename inner_product_expr<A,B,LS,RS>::result_type dot_result;
public:
  typedef typename binary_promote<Op, T, dot_result>::type type;
};

template <class Op, class Tag, class O, class S, class T, 
          class A, class B, class LS, class RS>
class binary_promote<Op, expr<Tag, T, inner_product_expr<A,B,LS,RS>, O, S>, T >
{
  typedef typename inner_product_expr<A,B,LS,RS>::result_type dot_result;
public:
  typedef typename binary_promote<Op, dot_result, T>::type type;
};



template <class A_type, class B_type>
class inner_prod_result {
  typedef typename to_value_type<typename A_type::value_type>::type A_value;
  typedef typename to_value_type<typename B_type::value_type>::type B_value;
public:
  typedef typename binary_promote<op_multiply, A_value, B_value>::type type;
};

template <class E>
class get_orien {
public:
  typedef typename E::orientation type;
};
template <>
class get_orien<double> {
public:
  typedef column type;
};

template <class E>
class get_sparsity {
public:
  typedef typename E::sparsity type;
};

template <>
class get_sparsity<double> {
public:
  typedef dense type;
};


template <class Tag, class Sc, class T1, class T2, class A, class B, class LS, class RS>
class binary_promote<op_multiply, 
  linalg_expr<Tag,Sc,T1,A,row,LS>,
  linalg_expr<Tag,Sc,T2,B,column,RS> >
{
  typedef linalg_expr<Tag,Sc,T1,A,row,LS> A_type;
  typedef linalg_expr<Tag,Sc,T2,B,column,RS> B_type;
  typedef typename inner_prod_result<A_type,B_type>::type result_type;
  typedef typename get_orien<result_type>::type Orien;
  typedef typename get_sparsity<result_type>::type Sparsity;
public:
  typedef const_ref<A_type> left_type;
  typedef const_ref<B_type> right_type;
  typedef inner_product_expr<left_type, right_type,
    LS, RS> expr_type;
  typedef expr<Tag, Sc, expr_type, Orien, Sparsity > type;
};

template <class Tag, class Sc, class T1, class T2, class A, class B, class LS, class RS>
typename binary_promote<op_multiply, 
  linalg_expr<Tag,Sc,T1,A,row,LS>,
  linalg_expr<Tag,Sc,T2,B,column,RS>
>::type 
operator * (const linalg_expr<Tag,Sc,T1,A,row,LS>& a, 
            const linalg_expr<Tag,Sc,T2,B,column,RS>& b) 
{ 
  typedef binary_promote<op_multiply, 
    linalg_expr<Tag,Sc,T1,A,row,LS>,
    linalg_expr<Tag,Sc,T2,B,column,RS>
  > P;
  typedef typename P::expr_type ExprT; 
  typedef typename P::type Wrap; 
  return Wrap( ExprT(typename P::left_type(a), typename P::right_type(b)) );
}

template <class Tag, class Sc, class T1, class A, class B, class LS, class RS> 
class binary_promote<op_multiply, 
  linalg_expr<Tag,Sc,T1,A,row,LS>,
  expr<Tag,Sc,B,column,RS> >
{
  typedef linalg_expr<Tag,Sc,T1,A,row,LS> A_type;
  typedef expr<Tag,Sc,B,column,RS> B_type;
  typedef typename inner_prod_result<A_type,B_type>::type result_type;
  typedef typename get_orien<result_type>::type Orien;
  typedef typename get_sparsity<result_type>::type Sparsity;
public:
  typedef const_ref<A_type> left_type;
  typedef B_type right_type;
  typedef inner_product_expr<const_ref< A_type >, B_type,
    LS, RS > expr_type; 
  typedef expr<Tag, Sc, expr_type, Orien, Sparsity> type;
};

template <class Tag, class Sc, class T1, class A, class B, class LS, class RS>
typename binary_promote<op_multiply,
  linalg_expr<Tag,Sc,T1,A,row,LS>,
  expr<Tag,Sc,B,column,RS>
>::type 
operator * (const linalg_expr<Tag,Sc,T1,A,row,LS>& a, 
            const expr<Tag,Sc,B,column,RS>& b) 
{
  typedef binary_promote<op_multiply, 
    linalg_expr<Tag,Sc,T1,A,row,LS>,
    expr<Tag,Sc,B,column,RS>
  > P;
  typedef typename P::expr_type ExprT;
  typedef typename P::type Wrap;
  return Wrap(ExprT(typename P::left_type(a), b));
}

template <class Tag, class Sc, class T2, class A, class B, class LS, class RS> 
class binary_promote<op_multiply, 
  expr<Tag,Sc,A,row,LS>,
  linalg_expr<Tag,Sc,T2,B,column,RS> >
{
  typedef expr<Tag,Sc,A,row,LS> A_type;
  typedef linalg_expr<Tag,Sc,T2,B,column,RS> B_type;
  typedef typename inner_prod_result<A_type,B_type>::type result_type;
  typedef typename get_orien<result_type>::type Orien;
  typedef typename get_sparsity<result_type>::type Sparsity;
public:
  typedef A_type left_type;
  typedef const_ref<B_type> right_type;
  typedef inner_product_expr<left_type, right_type,
    LS, RS > expr_type;
  typedef expr<Tag, Sc, expr_type, Orien, Sparsity> type;
};

template <class Tag, class Sc, class T2, class A, class B, class LS, class RS> 
typename binary_promote<op_multiply,
  expr<Tag,Sc,A,row,LS>,
  linalg_expr<Tag,Sc,T2,B,column,RS>
>::type 
operator * (const expr<Tag,Sc,A,row,LS>& a, 
            const linalg_expr<Tag,Sc,T2,B,column,RS>& b) 
{
  typedef binary_promote<op_multiply,
    expr<Tag,Sc,A,row,LS>,
    linalg_expr<Tag,Sc,T2,B,column,RS>
  > P;
  typedef typename P::expr_type ExprT;
  typedef typename P::type Wrap;
  return Wrap( ExprT(a, typename P::right_type(b)) );
}

template <class Tag, class Sc, class A, class B, class LS, class RS> 
class binary_promote<op_multiply,
  expr<Tag,Sc,A,row,LS>, expr<Tag,Sc,B,column,RS> >
{
  typedef expr<Tag,Sc,A,row,LS> A_type;
  typedef expr<Tag,Sc,B,column,RS> B_type;
  typedef typename inner_prod_result<A_type,B_type>::type result_type;
  typedef typename get_orien<result_type>::type Orien;
  typedef typename get_sparsity<result_type>::type Sparsity;
public:
  typedef A_type left_type;
  typedef B_type right_type;
  typedef inner_product_expr<left_type, right_type,
    LS, RS > expr_type;
  typedef expr<Tag, Sc, expr_type, Orien, Sparsity> type;
};

template <class Tag, class Sc, class A, class B, class LS, class RS>
typename binary_promote<op_multiply,
  expr<Tag,Sc,A,row,LS>, expr<Tag,Sc,B,column,RS>
>::type
operator * (const expr<Tag,Sc,A,row,LS>& a,
            const expr<Tag,Sc,B,column,RS>& b)
{
  typedef binary_promote<op_multiply,
    expr<Tag,Sc,A,row,LS>, expr<Tag,Sc,B,column,RS>
  > P;
  typedef typename P::expr_type ExprT;
  typedef typename P::type Wrap;
  return Wrap( ExprT(a, b) );
}

// Provide explicit dot() functions for taking dot product 
// regardless of the orientation of the vectors. Just
// implemented them in terms of the expressions, applying
// the transpose in the proper places.

#define DEFINE_DOT_PRODUCT(LEFT_ORIEN, RIGHT_ORIEN, AEXPR, BEXPR) \
template <class Sc, class T1, class T2, class A, class B, class LSp, class RSp> \
inline Sc \
dot(const linalg_expr<vector_tag,Sc,T1,A,LEFT_ORIEN,LSp>& a, \
    const linalg_expr<vector_tag,Sc,T2,B,RIGHT_ORIEN,RSp>& b) \
{ \
  return AEXPR * BEXPR; \
} \
template <class Sc, class T1, class A, class B, class LSp, class RSp> \
inline Sc \
dot(const linalg_expr<vector_tag,Sc,T1,A,LEFT_ORIEN,LSp>& a, \
    const expr<vector_tag,Sc,B,RIGHT_ORIEN,RSp>& b) \
{ \
  return AEXPR * BEXPR; \
} \
template <class Sc, class T2, class A, class B, class LSp, class RSp> \
inline Sc \
dot(const expr<vector_tag,Sc,A,LEFT_ORIEN,LSp>& a, \
    const linalg_expr<vector_tag,Sc,T2,B,RIGHT_ORIEN,RSp>& b) \
{ \
  return AEXPR * BEXPR; \
} \
template <class Sc, class A, class B, class LSp, class RSp> \
inline Sc \
dot(const expr<vector_tag,Sc,A,LEFT_ORIEN,LSp>& a, \
    const expr<vector_tag,Sc,B,RIGHT_ORIEN,RSp>& b) \
{ \
  return AEXPR * BEXPR; \
}

DEFINE_DOT_PRODUCT(row, column, a, b)
DEFINE_DOT_PRODUCT(row, row, a, trans(b))
DEFINE_DOT_PRODUCT(column, column, trans(a), b)
DEFINE_DOT_PRODUCT(column, row, trans(a), trans(b))
#undef DEFINE_DOT_PRODUCT



// =================== matrix vector product (using scale left) ===============

// the matrix is made of rows, it is row-major... but when looking at
// the matrix as a vector of vectors, the outer vector is a "column".

//  matrix * vector
template <class Sc, class T1, class A, class T2, class B, class Sp1, class Sp2>
class binary_promote<op_multiply, 
  linalg_expr<matrix_tag, Sc, T1, A, column, Sp1>, 
  linalg_expr<vector_tag, Sc, T2, B, column, Sp2> >
{
public:
  typedef expr<vector_tag, Sc, scale_left_expr< 
    const_ref< linalg_expr<matrix_tag,Sc,T1,A,column,Sp1> >, 
    linalg_expr<vector_tag,Sc,T2,B,column,Sp2> >, 
    column, dense> type;
};
template <class Sc, class T1, class A, class T2, class B, class Sp1,class Sp2> 
typename binary_promote<op_multiply, 
  linalg_expr<matrix_tag,Sc,T1,A,column,Sp1>, 
  linalg_expr<vector_tag,Sc,T2,B,column,Sp2>
   >::type 
operator * (const linalg_expr<matrix_tag,Sc,T1,A,column,Sp1>& a, 
            const linalg_expr<vector_tag,Sc,T2,B,column,Sp2>& b) 
{ 
  typedef linalg_expr<matrix_tag,Sc,T1,A,column,Sp1> A_type;
  typedef linalg_expr<vector_tag,Sc,T2,B,column,Sp2> B_type;
  typedef scale_left_expr<const_ref<A_type>, B_type> ExprT; 
  return expr<vector_tag,Sc,ExprT,column,dense>( ExprT(
     const_ref<A_type>(a), b ) );
}

//  matrix-expr * vector
template <class Sc, class A, class T2, class B, class Sp1, class Sp2>
class binary_promote<op_multiply, 
  expr<matrix_tag, Sc, A, column, Sp1>, 
  linalg_expr<vector_tag, Sc, T2, B, column, Sp2> >
{
public:
  typedef expr<vector_tag, Sc, scale_left_expr< 
    expr<matrix_tag,Sc,A,column,Sp1>, 
    linalg_expr<vector_tag,Sc,T2,B,column,Sp2> >, 
    column, dense> type;
};
template <class Sc, class A, class T2, class B, class Sp1,class Sp2> 
typename binary_promote<op_multiply, 
  expr<matrix_tag,Sc,A,column,Sp1>, 
  linalg_expr<vector_tag,Sc,T2,B,column,Sp2>
   >::type 
operator * (const expr<matrix_tag,Sc,A,column,Sp1>& a, 
            const linalg_expr<vector_tag,Sc,T2,B,column,Sp2>& b) 
{ 
  typedef expr<matrix_tag,Sc,A,column,Sp1> A_type;
  typedef linalg_expr<vector_tag,Sc,T2,B,column,Sp2> B_type;
  typedef scale_left_expr<A_type, B_type> ExprT; 
  return expr<vector_tag,Sc,ExprT,column,dense>( ExprT(a, b) );
}

//  matrix * vector-expr
template <class Sc, class T1, class A, class B, class Sp1, class Sp2>
class binary_promote<op_multiply, 
  linalg_expr<matrix_tag, Sc, T1, A, column, Sp1>, 
  expr<vector_tag, Sc, B, column, Sp2> >
{
public:
  typedef expr<vector_tag, Sc, scale_left_expr< 
    const_ref< linalg_expr<matrix_tag,Sc,T1,A,column,Sp1> >, 
    expr<vector_tag,Sc,B,column,Sp2> >, 
    column, dense> type;
};
template <class Sc, class T1, class A, class B, class Sp1,class Sp2> 
typename binary_promote<op_multiply, 
  linalg_expr<matrix_tag,Sc,T1,A,column,Sp1>, 
  expr<vector_tag,Sc,B,column,Sp2>
   >::type 
operator * (const linalg_expr<matrix_tag,Sc,T1,A,column,Sp1>& a, 
            const expr<vector_tag,Sc,B,column,Sp2>& b) 
{ 
  typedef linalg_expr<matrix_tag,Sc,T1,A,column,Sp1> A_type;
  typedef expr<vector_tag,Sc,B,column,Sp2> B_type;
  typedef scale_left_expr<const_ref<A_type>, B_type> ExprT; 
  return expr<vector_tag,Sc,ExprT,column,dense>( ExprT(
     const_ref<A_type>(a), b ) );
}

//  matrix-expr * vector-expr
template <class Sc, class A, class B, class Sp1, class Sp2>
class binary_promote<op_multiply, 
  expr<matrix_tag, Sc, A, column, Sp1>, 
  expr<vector_tag, Sc, B, column, Sp2> >
{
public:
  typedef expr<vector_tag, Sc, scale_left_expr< 
    expr<matrix_tag,Sc,A,column,Sp1>, 
    expr<vector_tag,Sc,B,column,Sp2> >, 
    column, dense> type;
};
template <class Sc, class A, class B, class Sp1,class Sp2> 
typename binary_promote<op_multiply, 
  expr<matrix_tag,Sc,A,column,Sp1>, 
  expr<vector_tag,Sc,B,column,Sp2>
   >::type 
operator * (const expr<matrix_tag,Sc,A,column,Sp1>& a, 
            const expr<vector_tag,Sc,B,column,Sp2>& b) 
{ 
  typedef expr<matrix_tag,Sc,A,column,Sp1> A_type;
  typedef expr<vector_tag,Sc,B,column,Sp2> B_type;
  typedef scale_left_expr<A_type, B_type> ExprT; 
  return expr<vector_tag,Sc,ExprT,column,dense>( ExprT(a, b) );
}


// =================== matrix vector product (using inner product) ============

// the matrix is made of columns, it is columns-major... but when looking at
// the matrix as a vector of vectors, the outer vector is a "row".


// matrix * vector
template <class Sc, class T1, class A, class T2, class B, class Sp1, class Sp2>
class binary_promote<op_multiply, 
  linalg_expr<matrix_tag, Sc, T1, A, row, Sp1>, 
  linalg_expr<vector_tag, Sc, T2, B, column, Sp2> >
{
  typedef typename or_sparsity<typename T1::sparsity, Sp2>::type Sparsity;
public:
  typedef expr<vector_tag, Sc, inner_product_expr< 
    const_ref< linalg_expr<matrix_tag,Sc,T1,A,row,Sp1> >, 
    const_ref< linalg_expr<vector_tag,Sc,T2,B,column,Sp2> >, Sp1, Sp2>, 
    column, Sparsity> type;
};
template <class Sc, class T1, class A, class T2, class B, class Sp1,class Sp2> 
typename binary_promote<op_multiply, 
  linalg_expr<matrix_tag,Sc,T1,A,row,Sp1>, 
  linalg_expr<vector_tag,Sc,T2,B,column,Sp2>
   >::type 
operator * (const linalg_expr<matrix_tag,Sc,T1,A,row,Sp1>& a, 
            const linalg_expr<vector_tag,Sc,T2,B,column,Sp2>& b) 
{ 
  typedef typename or_sparsity<typename T1::sparsity, Sp2>::type Sparsity;
  typedef linalg_expr<matrix_tag,Sc,T1,A,row,Sp1> A_type;
  typedef linalg_expr<vector_tag,Sc,T2,B,column,Sp2> B_type;
  typedef inner_product_expr<const_ref<A_type>, const_ref<B_type>, Sp1, Sp2>
    ExprT; 
  return expr<vector_tag,Sc,ExprT,column,Sparsity>( ExprT(
     const_ref<A_type>(a), const_ref<B_type>(b) ) );
}

// matrix-expr * vector
template <class Sc, class A, class T2, class B, class Sp1, class Sp2>
class binary_promote<op_multiply, 
  expr<matrix_tag, Sc, A, row, Sp1>, 
  linalg_expr<vector_tag, Sc, T2, B, column, Sp2> >
{
  typedef typename or_sparsity<typename A::value_type::sparsity, Sp2>::type
    Sparsity;
public:
  typedef expr<vector_tag, Sc, inner_product_expr< 
    expr<matrix_tag,Sc,A,row,Sp1>, 
    const_ref< linalg_expr<vector_tag,Sc,T2,B,column,Sp2> >, Sp1, Sp2>, 
    column, Sparsity> type;
};
template <class Sc, class A, class T2, class B, class Sp1,class Sp2> 
typename binary_promote<op_multiply, 
  expr<matrix_tag,Sc,A,row,Sp1>, 
  linalg_expr<vector_tag,Sc,T2,B,column,Sp2>
   >::type 
operator * (const expr<matrix_tag,Sc,A,row,Sp1>& a, 
            const linalg_expr<vector_tag,Sc,T2,B,column,Sp2>& b) 
{ 
  typedef typename or_sparsity<typename A::value_type::sparsity, Sp2>::type
    Sparsity;
  typedef expr<matrix_tag,Sc,A,row,Sp1> A_type;
  typedef linalg_expr<vector_tag,Sc,T2,B,column,Sp2> B_type;
  typedef inner_product_expr<A_type, const_ref<B_type>, Sp1, Sp2> ExprT; 
  return expr<vector_tag,Sc,ExprT,column,Sparsity>( ExprT(
     a, const_ref<B_type>(b) ) );
}

// matrix * vector-expr
template <class Sc, class T1, class A, class B, class Sp1, class Sp2>
class binary_promote<op_multiply, 
  linalg_expr<matrix_tag, Sc, T1, A, row, Sp1>, 
  expr<vector_tag, Sc, B, column, Sp2> >
{
  typedef typename or_sparsity<typename T1::sparsity, Sp2>::type Sparsity;
public:
  typedef expr<vector_tag, Sc, inner_product_expr< 
    const_ref< linalg_expr<matrix_tag,Sc,T1,A,row,Sp1> >, 
    expr<vector_tag,Sc,B,column,Sp2>, Sp1, Sp2>, 
    column, Sparsity> type;
};
template <class Sc, class T1, class A, class B, class Sp1,class Sp2> 
typename binary_promote<op_multiply, 
  linalg_expr<matrix_tag,Sc,T1,A,row,Sp1>, 
  expr<vector_tag,Sc,B,column,Sp2>
   >::type 
operator * (const linalg_expr<matrix_tag,Sc,T1,A,row,Sp1>& a, 
            const expr<vector_tag,Sc,B,column,Sp2>& b) 
{ 
  typedef typename or_sparsity<typename T1::sparsity, Sp2>::type Sparsity;
  typedef linalg_expr<matrix_tag,Sc,T1,A,row,Sp1> A_type;
  typedef expr<vector_tag,Sc,B,column,Sp2> B_type;
  typedef inner_product_expr<const_ref<A_type>, B_type, Sp1, Sp2> ExprT; 
  return expr<vector_tag,Sc,ExprT,column,Sparsity>( ExprT(
     const_ref<A_type>(a), b ) );
}

// matrix-expr * vector-expr
template <class Sc, class A, class B, class Sp1, class Sp2>
class binary_promote<op_multiply, 
  expr<matrix_tag, Sc, A, row, Sp1>, 
  expr<vector_tag, Sc, B, column, Sp2> >
{
  typedef typename or_sparsity<typename A::value_type::sparsity, Sp2>::type Sparsity;
public:
  typedef expr<vector_tag, Sc, inner_product_expr< 
    expr<matrix_tag,Sc,A,row,Sp1>, 
    expr<vector_tag,Sc,B,column,Sp2>, Sp1, Sp2>, 
    column, Sparsity> type;
};
template <class Sc, class A, class B, class Sp1,class Sp2> 
typename binary_promote<op_multiply, 
  expr<matrix_tag,Sc,A,row,Sp1>, 
  expr<vector_tag,Sc,B,column,Sp2>
   >::type 
operator * (const expr<matrix_tag,Sc,A,row,Sp1>& a, 
            const expr<vector_tag,Sc,B,column,Sp2>& b) 
{ 
  typedef typename or_sparsity<typename A::value_type::sparsity, Sp2>::type
    Sparsity;
  typedef expr<matrix_tag,Sc,A,row,Sp1> A_type;
  typedef expr<vector_tag,Sc,B,column,Sp2> B_type;
  typedef inner_product_expr<A_type, B_type, Sp1, Sp2> ExprT; 
  return expr<vector_tag,Sc,ExprT,column,Sparsity>( ExprT(a, b) );
}

// =================== vector matrix product (using scale right) ==============

// vector * matrix
template <class Sc, class T1, class A, class T2, class B, class Sp1, class Sp2>
class binary_promote<op_multiply, 
  linalg_expr<vector_tag, Sc, T1, A, row, Sp1>, 
  linalg_expr<matrix_tag, Sc, T2, B, row, Sp2> >
{
public:
  typedef expr<vector_tag, Sc, scale_right_expr< 
    linalg_expr<vector_tag,Sc,T1,A,row,Sp1>, 
    const_ref< linalg_expr<matrix_tag,Sc,T2,B,row,Sp2> > >, 
    row, dense> type;
};
template <class Sc, class T1, class A, class T2, class B, class Sp1,class Sp2> 
typename binary_promote<op_multiply, 
  linalg_expr<vector_tag,Sc,T1,A,row,Sp1>, 
  linalg_expr<matrix_tag,Sc,T2,B,row,Sp2>
   >::type 
operator * (const linalg_expr<vector_tag,Sc,T1,A,row,Sp1>& a, 
            const linalg_expr<matrix_tag,Sc,T2,B,row,Sp2>& b) 
{ 
  typedef linalg_expr<vector_tag,Sc,T1,A,row,Sp1> A_type;
  typedef linalg_expr<matrix_tag,Sc,T2,B,row,Sp2> B_type;
  typedef scale_right_expr<A_type, const_ref<B_type> > ExprT; 
  return expr<vector_tag,Sc,ExprT,row,dense>( ExprT(
     a, const_ref<B_type>(b) ) );
}

// vector-expr * matrix
template <class Sc, class A, class T2, class B, class Sp1, class Sp2>
class binary_promote<op_multiply, 
  expr<vector_tag, Sc, A, row, Sp1>, 
  linalg_expr<matrix_tag, Sc, T2, B, row, Sp2> >
{
public:
  typedef expr<vector_tag, Sc, scale_right_expr< 
    expr<vector_tag,Sc,A,row,Sp1>, 
    const_ref< linalg_expr<matrix_tag,Sc,T2,B,row,Sp2> > >, 
    row, dense> type;
};
template <class Sc, class A, class T2, class B, class Sp1,class Sp2> 
typename binary_promote<op_multiply, 
  expr<vector_tag,Sc,A,row,Sp1>, 
  linalg_expr<matrix_tag,Sc,T2,B,row,Sp2>
   >::type 
operator * (const expr<vector_tag,Sc,A,row,Sp1>& a, 
            const linalg_expr<matrix_tag,Sc,T2,B,row,Sp2>& b) 
{ 
  typedef expr<vector_tag,Sc,A,row,Sp1> A_type;
  typedef linalg_expr<matrix_tag,Sc,T2,B,row,Sp2> B_type;
  typedef scale_right_expr<A_type, const_ref<B_type> > ExprT; 
  return expr<vector_tag,Sc,ExprT,row,dense>( ExprT(
     a, const_ref<B_type>(b) ) );
}

// vector * matrix-expr
template <class Sc, class T1, class A, class B, class Sp1, class Sp2>
class binary_promote<op_multiply, 
  linalg_expr<vector_tag, Sc, T1, A, row, Sp1>, 
  expr<matrix_tag, Sc, B, row, Sp2> >
{
public:
  typedef expr<vector_tag, Sc, scale_right_expr< 
    linalg_expr<vector_tag,Sc,T1,A,row,Sp1>, 
    expr<matrix_tag,Sc,B,row,Sp2> >, 
    row, dense> type;
};
template <class Sc, class T1, class A, class B, class Sp1,class Sp2> 
typename binary_promote<op_multiply, 
  linalg_expr<vector_tag,Sc,T1,A,row,Sp1>, 
  expr<matrix_tag,Sc,B,row,Sp2>
   >::type 
operator * (const linalg_expr<vector_tag,Sc,T1,A,row,Sp1>& a, 
            const expr<matrix_tag,Sc,B,row,Sp2>& b) 
{ 
  typedef linalg_expr<vector_tag,Sc,T1,A,row,Sp1> A_type;
  typedef expr<matrix_tag,Sc,B,row,Sp2> B_type;
  typedef scale_right_expr<A_type, B_type > ExprT; 
  return expr<vector_tag,Sc,ExprT,row,dense>( ExprT(a, b) );
}

// vector-expr * matrix-expr
template <class Sc, class A, class B, class Sp1, class Sp2>
class binary_promote<op_multiply, 
  expr<vector_tag, Sc, A, row, Sp1>, 
  expr<matrix_tag, Sc, B, row, Sp2> >
{
public:
  typedef expr<vector_tag, Sc, scale_right_expr< 
    expr<vector_tag,Sc,A,row,Sp1>, 
    expr<matrix_tag,Sc,B,row,Sp2> >, 
    row, dense> type;
};
template <class Sc, class A, class B, class Sp1,class Sp2> 
typename binary_promote<op_multiply, 
  expr<vector_tag,Sc,A,row,Sp1>, 
  expr<matrix_tag,Sc,B,row,Sp2>
   >::type 
operator * (const expr<vector_tag,Sc,A,row,Sp1>& a, 
            const expr<matrix_tag,Sc,B,row,Sp2>& b) 
{ 
  typedef expr<vector_tag,Sc,A,row,Sp1> A_type;
  typedef expr<matrix_tag,Sc,B,row,Sp2> B_type;
  typedef scale_right_expr<A_type, B_type > ExprT; 
  return expr<vector_tag,Sc,ExprT,row,dense>( ExprT(a, b) );
}

// =================== vector matrix product (using inner product) ============

// vector * matrix
template <class Sc, class T1, class A, class T2, class B, class Sp1, class Sp2>
class binary_promote<op_multiply, 
  linalg_expr<vector_tag, Sc, T1, A, row, Sp1>, 
  linalg_expr<matrix_tag, Sc, T2, B, column, Sp2> >
{
  typedef typename or_sparsity<Sp1, typename T2::sparsity>::type Sparsity;
public:
  typedef expr<vector_tag, Sc, inner_product_expr< 
    const_ref< linalg_expr<vector_tag,Sc,T1,A,row,Sp1> >, 
    const_ref< linalg_expr<matrix_tag,Sc,T2,B,column,Sp2> >, Sp1, Sp2>, 
    row, Sparsity> type;
};
template <class Sc, class T1, class A, class T2, class B, class Sp1,class Sp2> 
typename binary_promote<op_multiply, 
  linalg_expr<vector_tag,Sc,T1,A,row,Sp1>, 
  linalg_expr<matrix_tag,Sc,T2,B,column,Sp2>
   >::type 
operator * (const linalg_expr<vector_tag,Sc,T1,A,row,Sp1>& a, 
            const linalg_expr<matrix_tag,Sc,T2,B,column,Sp2>& b) 
{ 
  typedef typename or_sparsity<Sp1, typename T2::sparsity>::type Sparsity;
  typedef linalg_expr<vector_tag,Sc,T1,A,row,Sp1> A_type;
  typedef linalg_expr<matrix_tag,Sc,T2,B,column,Sp2> B_type;
  typedef inner_product_expr<const_ref<A_type>, const_ref<B_type>,Sp1,Sp2> ExprT; 
  return expr<vector_tag,Sc,ExprT,row,Sparsity>( ExprT(
     const_ref<A_type>(a), const_ref<B_type>(b) ) );
}

// vector-expr * matrix
template <class Sc, class A, class T2, class B, class Sp1, class Sp2>
class binary_promote<op_multiply, 
  expr<vector_tag, Sc, A, row, Sp1>, 
  linalg_expr<matrix_tag, Sc, T2, B, column, Sp2> >
{
  typedef typename or_sparsity<Sp1, typename T2::sparsity>::type Sparsity;
public:
  typedef expr<vector_tag, Sc, inner_product_expr< 
    expr<vector_tag,Sc,A,row,Sp1>, 
    const_ref< linalg_expr<matrix_tag,Sc,T2,B,column,Sp2> >, Sp1, Sp2>, 
    row, Sparsity> type;
};
template <class Sc, class A, class T2, class B, class Sp1,class Sp2> 
typename binary_promote<op_multiply, 
  expr<vector_tag,Sc,A,row,Sp1>, 
  linalg_expr<matrix_tag,Sc,T2,B,column,Sp2>
   >::type 
operator * (const expr<vector_tag,Sc,A,row,Sp1>& a, 
            const linalg_expr<matrix_tag,Sc,T2,B,column,Sp2>& b) 
{ 
  typedef typename or_sparsity<Sp1, typename T2::sparsity>::type Sparsity;
  typedef expr<vector_tag,Sc,A,row,Sp1> A_type;
  typedef linalg_expr<matrix_tag,Sc,T2,B,column,Sp2> B_type;
  typedef inner_product_expr<A_type, const_ref<B_type>, Sp1, Sp2> ExprT; 
  return expr<vector_tag,Sc,ExprT,row,Sparsity>( ExprT(
     a, const_ref<B_type>(b) ) );
}

// vector * matrix-expr
template <class Sc, class T1, class A, class B, class Sp1, class Sp2>
class binary_promote<op_multiply, 
  linalg_expr<vector_tag, Sc, T1, A, row, Sp1>, 
  expr<matrix_tag, Sc, B, column, Sp2> >
{
  typedef typename or_sparsity<Sp1, typename B::value_type::sparsity>::type Sparsity;
public:
  typedef expr<vector_tag, Sc, inner_product_expr< 
    const_ref< linalg_expr<vector_tag,Sc,T1,A,row,Sp1> >, 
    expr<matrix_tag,Sc,B,column,Sp2>, Sp1, Sp2>, 
    row, Sparsity> type;
};
template <class Sc, class T1, class A, class B, class Sp1,class Sp2> 
typename binary_promote<op_multiply, 
  linalg_expr<vector_tag,Sc,T1,A,row,Sp1>, 
  expr<matrix_tag,Sc,B,column,Sp2>
   >::type 
operator * (const linalg_expr<vector_tag,Sc,T1,A,row,Sp1>& a, 
            const expr<matrix_tag,Sc,B,column,Sp2>& b) 
{ 
  typedef typename or_sparsity<Sp1, typename B::value_type::sparsity>::type Sparsity;
  typedef linalg_expr<vector_tag,Sc,T1,A,row,Sp1> A_type;
  typedef expr<matrix_tag,Sc,B,column,Sp2> B_type;
  typedef inner_product_expr<const_ref<A_type>, B_type, Sp1, Sp2> ExprT; 
  return expr<vector_tag,Sc,ExprT,row,Sparsity>( ExprT(
     const_ref<A_type>(a), b ) );
}

// vector-expr * matrix-expr
template <class Sc, class A, class B, class Sp1, class Sp2>
class binary_promote<op_multiply, 
  expr<vector_tag, Sc, A, row, Sp1>, 
  expr<matrix_tag, Sc, B, column, Sp2> >
{
  typedef typename or_sparsity<Sp1, typename B::value_type::sparsity>::type Sparsity;
public:
  typedef expr<vector_tag, Sc, inner_product_expr< 
    expr<vector_tag,Sc,A,row,Sp1>, 
    expr<matrix_tag,Sc,B,column,Sp2>, Sp1, Sp2>, 
    row, Sparsity> type;
};
template <class Sc, class A, class B, class Sp1,class Sp2> 
typename binary_promote<op_multiply, 
  expr<vector_tag,Sc,A,row,Sp1>, 
  expr<matrix_tag,Sc,B,column,Sp2>
   >::type 
operator * (const expr<vector_tag,Sc,A,row,Sp1>& a, 
            const expr<matrix_tag,Sc,B,column,Sp2>& b) 
{ 
  typedef typename or_sparsity<Sp1, typename B::value_type::sparsity>::type Sparsity;
  typedef expr<vector_tag,Sc,A,row,Sp1> A_type;
  typedef expr<matrix_tag,Sc,B,column,Sp2> B_type;
  typedef inner_product_expr<A_type, B_type, Sp1, Sp2> ExprT; 
  return expr<vector_tag,Sc,ExprT,row,Sparsity>( ExprT( a, b ) );
}

// transpose unary operator =================================================

template <class A>
class transpose_expr
{
public:
  typedef no_implicit_result result_type;
  typedef typename A::value_type A_value;
  typedef typename unary_promote<op_transpose, A_value>::type value_type;
  transpose_expr(const A& a_) : a(a_) { }
  value_type operator[] (std::size_t n) const {
    return trans(a[n]);
  }
  std::size_t size() const { return a.size(); }
  // nrows() and ncols() are flipped
  std::size_t nrows() const { return a.ncols(); }
  std::size_t ncols() const { return a.nrows(); }
//private:
  A a;
};

template <class A>
class get_range< transpose_expr<A> >
{
public:
  typedef unary_op_range<typename get_range<A>::type, op_transpose> type;
};
template <class A>
inline 
unary_op_range<typename get_range<A>::type, op_transpose>
make_range(const transpose_expr<A>& x)
{
  typedef unary_op_range<typename get_range<A>::type, op_transpose> Range;
  return Range(make_range(x.a));
}

template <>
class unary_promote<op_transpose, double> {
public:
  typedef double type;
};

template <class Tag, class Sc, class T, class A, class O, class S>
class unary_promote<op_transpose, linalg_expr<Tag,Sc,T,A,O,S> > {
public:
  // The "main event" of the transpose expression is
  // that the expr's orientation gets flipped around.
  typedef expr<Tag, Sc, 
    transpose_expr<const_ref< linalg_expr<Tag,Sc,T,A,O,S> > >,
    typename trans_orien<O>::type, S> type;
}; 

template <class Tag, class Sc, class T, class C, class Orien, class Sp>
inline typename unary_promote<op_transpose, 
  linalg_expr<Tag,Sc,T,C,Orien,Sp> >::type
trans(const linalg_expr<Tag,Sc,T,C,Orien,Sp>& x)
{
  typedef linalg_expr<Tag,Sc,T,C,Orien,Sp> A_type;
  typedef typename unary_promote<op_transpose, A_type>::type Trans;
  return Trans(const_ref<A_type>(x));
}

double trans(const double& x) { return x; }

template <class A, class B>
mtl::pair<A,B> trans(const mtl::pair<A,B>& p) { return p; }

template <class Tag, class Sc, class E, class Orien, class Sp>
class unary_promote<op_transpose, expr<Tag, Sc, E, Orien, Sp> >
{
  typedef expr<Tag, Sc, E, Orien, Sp> A_type;
public:
  typedef expr<Tag, Sc, transpose_expr<A_type>, 
    typename trans_orien<Orien>::type, Sp> type;
};

template <class Tag, class Sc, class E, class Orien, class Sp>
inline typename unary_promote<op_transpose,
  expr<Tag, Sc, E, Orien, Sp> >::type
trans(const expr<Tag, Sc, E, Orien, Sp>& x)
{
  typedef typename unary_promote<op_transpose,
    expr<Tag, Sc, E, Orien, Sp> >::type Trans;
  return Trans(x);
}


// static sparse operator =================================================

template <class Sc, class T, class C, class O>
class static_sparse_wrap
  : public linalg_expr<vector_tag, Sc, T, 
     static_sparse_wrap<Sc,T,C,O>, O, static_sparse>
{
public:
  typedef static_sparse sparsity;
  typedef typename C::iterator iterator;
  typedef typename C::const_iterator const_iterator;
  typedef typename C::size_type size_type;

  static_sparse_wrap(C& c) : m_c(c) { }

  // operator= expr
  // operator= linalg_expr

  iterator begin() { return m_c.begin(); }
  iterator end() { return m_c.end(); }

  const_iterator begin() const { return m_c.begin(); }
  const_iterator end() const { return m_c.end(); }

  size_type size() const { return m_c.size(); }

  void resize(std::size_t n) { m_c.resize(n); }
private:
  C& m_c;
};

template <class Sc, class T, class C, class O>
inline static_sparse_wrap<Sc,T,C,O>
freeze(linalg_expr<vector_tag, Sc, T, C, O, sparse>& x)
{
  return static_sparse_wrap<Sc,T,C,O>(static_cast<C&>(x));
}

// static mod sparse operator =================================================

template <class Sc, class T, class C, class O>
class static_mod_sparse_wrap
{
  typedef static_mod_sparse_wrap self;
public:
  typedef static_sparse sparsity;
  typedef typename C::iterator iterator;
  typedef typename C::const_iterator const_iterator;

  static_mod_sparse_wrap(C& c, Sc& y)
    : m_c(c), m_y(y) { }

  std::size_t size() const { 
    return m_c.size(); 
  }
  std::size_t nrows() const { 
    return m_c.nrows(); 
  }
  std::size_t ncols() const { 
    return m_c.ncols(); 
  }

  template <class E>
  self& operator+=( const expr<vector_tag,Sc,E,O,sparse>& x )
  {
    typedef op_mod_diag<Sc, op_assign_add> Op;
    m_c.resize(x.size());
    x.eval(*this, Op(m_y));
    return *this;
  }
  template <class E>
  self& operator-=( const expr<vector_tag,Sc,E,O,sparse>& x )
  {
    typedef op_mod_diag<Sc, op_assign_subtract> Op;
    m_c.resize(x.size());
    x.eval(*this, Op(m_y));
    return *this;
  }
  template <class TT, class CC>
  self& operator+=(const linalg_expr<vector_tag,Sc,TT,CC,O,sparse>& x)
  {
    typedef op_mod_diag<Sc, op_assign_add> Op;
    m_c.resize(x.size());
    x.eval(*this, Op(m_y));
    return *this;
  }
  template <class TT, class CC>
  self& operator-=(const linalg_expr<vector_tag,Sc,TT,CC,O,sparse>& x)
  {
    typedef op_mod_diag<Sc, op_assign_subtract> Op;
    m_c.resize(x.size());
    x.eval(*this, Op(m_y));
    return *this;
  }

  template <class Result, class AssignOp>
  void eval(Result& result, AssignOp assign) const {
    iter_range<typename C::const_iterator,
               typename C::const_iterator>
      r = make_range(m_c);
    eval_expr(result, r, assign, sparse());
  }

  iterator begin() { return m_c.begin(); }
  iterator end() { return m_c.end(); }

  const_iterator begin() const { return m_c.begin(); }
  const_iterator end() const { return m_c.end(); }

  void resize(std::size_t n) { m_c.resize(n); }
private:
  C& m_c;
  Sc& m_y;
};

template <class Sc, class T, class C, class O>
inline static_mod_sparse_wrap<Sc,T,C,O>
freeze_mod(linalg_expr<vector_tag, Sc, T, C, O, sparse>& x, Sc& y)
{
  return static_mod_sparse_wrap<Sc,T,C,O>(static_cast<C&>(x), y);
}



#endif // MTL_EXPR_HPP
