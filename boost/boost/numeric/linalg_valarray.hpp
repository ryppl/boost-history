#ifndef BOOST_NUMERIC_LINALG_VALARRAY_HPP
#define BOOST_NUMERIC_LINALG_VALARRAY_HPP

/*
  The functions and traits classes in this file turn a std::valarray
  into a model of Vector Space.

  UNDER CONSTRUCTION (this has not been fully tested)
 */

#include <valarray>

namespace boost {

# if defined __GNUC__ && !defined(__SGI_STL)

  // v + v
  template <class R>
  struct binary_op_return<op_add, std::valarray<R>, std::valarray<R> >
  {
    typedef std::_Expr<std::_BinClos<std::plus, std::_ValArray, std::_ValArray,
      R, R>, R> type;
  };
  // e + e
  template <class F, class ClosA, class ClosB>
  struct binary_op_return<op_add, 
    std::_Expr<ClosA, F>, std::_Expr<ClosB, F> >
  {
    typedef std::_Expr<std::_BinClos<std::plus, std::_Expr,
      std::_Expr, ClosA, ClosB>, F> type;
  };
  // e + v
  template <class F, class Clos>
  struct binary_op_return<op_add, 
    std::_Expr<Clos, F>, std::valarray<F> >
  {
    typedef std::_Expr<std::_BinClos<std::plus, std::_Expr,
      std::_ValArray, Clos, F>, F> type;
  };
  // v + e
  template <class F, class Clos>
  struct binary_op_return<op_add, 
    std::valarray<F>, std::_Expr<Clos, F> >
  {
    typedef std::_Expr<std::_BinClos<std::plus, std::_ValArray,
      std::_Expr, F, Clos>, F> type;
  };

  // v - v
  template <class R>
  struct binary_op_return<op_subtract, std::valarray<R>, std::valarray<R> >
  {
    typedef std::_Expr<std::_BinClos<std::minus, std::_ValArray, 
      std::_ValArray, R, R>, R> type;
  };
  // e - e
  template <class F, class ClosA, class ClosB>
  struct binary_op_return<op_subtract, 
    std::_Expr<ClosA, F>, std::_Expr<ClosB, F> >
  {
    typedef std::_Expr<std::_BinClos<std::minus, std::_Expr,
      std::_Expr, ClosA, ClosB>, F> type;
  };
  // e - v
  template <class F, class Clos>
  struct binary_op_return<op_subtract, 
    std::_Expr<Clos, F>, std::valarray<F> >
  {
    typedef std::_Expr<std::_BinClos<std::minus, std::_Expr,
      std::_ValArray, Clos, F>, F> type;
  };
  // v - e
  template <class F, class Clos>
  struct binary_op_return<op_subtract, 
    std::valarray<F>, std::_Expr<Clos, F> >
  {
    typedef std::_Expr<std::_BinClos<std::minus, std::_ValArray,
      std::_Expr, F, Clos>, F> type;
  };

  // -v
  template <class R>
  struct unary_op_return<op_negate, std::valarray<R> >
  {
    typedef std::_Expr<std::_UnClos<std::negate, std::_ValArray, R>, R> type;
  };
  // -e
  template <class R, class Clos>
  struct unary_op_return<op_negate, std::_Expr<Clos,R> >
  {
    typedef std::_Expr<std::_UnClos<std::negate, std::_Expr, Clos>, R> type;
  };


  // v / a
  template <class F>
  struct binary_op_return<op_divide, std::valarray<F>, F>
  {
    typedef std::_Expr<std::_BinClos<std::divides, std::_ValArray,
      std::_Constant, F, F>, F> type;
  };
  // e / a
  template <class F, class Clos>
  struct binary_op_return<op_divide, std::_Expr<Clos, F>, F>
  {
    typedef std::_Expr<std::_BinClos<std::divides, std::_Expr,
      std::_Constant, Clos, F>, F> type;
  };


  // v * a
  template <class F>
  struct binary_op_return<op_multiply, std::valarray<F>, F>
  {
    typedef std::_Expr<std::_BinClos<std::multiplies, std::_ValArray,
      std::_Constant, F, F>, F> type;
  };
  // e * a
  template <class F, class Clos>
  struct binary_op_return<op_multiply, std::_Expr<Clos, F>, F >
  {
    typedef std::_Expr<std::_BinClos<std::multiplies, std::_Expr, 
      std::_Constant, Clos, F>, F> type;
  };
  // a * v
  template <class F>
  struct binary_op_return<op_multiply, F, std::valarray<F> >
  {
    typedef std::_Expr<std::_BinClos<std::multiplies, std::_Constant,
      std::_ValArray, F, F>, F> type;
  };
  // a * e
  template <class F, class Clos>
  struct binary_op_return<op_multiply, F, std::_Expr<Clos, F> >
  {
    typedef std::_Expr<std::_BinClos<std::multiplies, std::_Constant,
      std::_Expr, F, Clos>, F> type;
  };

  // zero(v)
  template <class T>
  std::valarray<T> zero(const std::valarray<T>& x) {
    std::valarray<T> ret(x.size(), T(0));
    return ret;
  }

  template <class Clos, class T>
  struct unary_op_return<op_zero, std::_Expr<Clos, T> >
  {
    typedef std::valarray<T> type;
  };

  // zero(e)
  template <class Clos, class T>
  std::valarray<T> zero(const std::_Expr<Clos, T>& e) {
    std::valarray<T> ret(T(0), e.size());
    return ret;
  }

  // one(v)
  template <class T>
  std::valarray<T> one(const std::valarray<T>& x) {
    std::valarray<T> ret(T(1), x.size());
    return ret;
  }

  template <class Clos, class T>
  struct unary_op_return<op_one, std::_Expr<Clos, T> >
  {
    typedef std::valarray<T> type;
  };

  // one(e)
  template <class Clos, class T>
  std::valarray<T> one(const std::_Expr<Clos, T>& e) {
    std::valarray<T> ret(T(1), e.size());
    return ret;
  }

#else

#error The definitions in this file need to be customized for your compilers implementation of std::valarray.

#endif

} // namespace boost


#endif // BOOST_NUMERIC_LINALG_VALARRAY_HPP
