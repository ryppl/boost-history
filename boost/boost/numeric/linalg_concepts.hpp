#ifndef BOOST_NUMERIC_LINALG_CONCEPTS_HPP
#define BOOST_NUMERIC_LINALG_CONCEPTS_HPP

#include <boost/concept_check.hpp>
#include <boost/op_return_traits.hpp>

namespace boost {
  
  template <class X>
  struct AdditiveAbelianGroupConcept
  {
    typedef typename boost::binary_op_return<op_add, X, X>::type add_ret;
    typedef typename boost::unary_op_return<op_negate, X>::type neg_ret;
    typedef typename boost::binary_op_return<op_subtract, X, neg_ret>::type
      sub_ret;
    typedef typename boost::unary_op_return<op_zero, sub_ret>::type zero_ret;

    void constraints() {
      // std::valarray can't be equality comparable because it
      //   uses a broken array-wise definition of == :( 
      // function_requires< EqualityComparableConcept<X> >();

      const add_ret& c = a + b;
      X& d = (b += c);
      const neg_ret& e = -a;
      const sub_ret& f = d - e;
      const zero_ret& h = zero(f);
      X& g = (b -= h);
      ignore_unused_variable_warning(g);
    }
    X a, b;

    // Semantic Invariants
    //
    // Associativity: (a + b) + c == a + (b + c) 
    // Definition of Identity Element: a + zero(a) == a 
    // Definition of Additive Inverse: a + -a == zero(a) 
    // Commutativity: a + b == b + a 
    // Meaning of subtraction: a - b == a + -b 
    // Meaning of assignment-addition:
    //   If c == b then after executing b += a and c = c + a we still
    //   have c == b.
    // Meaning of assignment-subtraction:
    //   If c == b then after executing b -= a and c = c - a we still
    //   have c == b.
  };
  
  template <class X>
  struct MultiplicativeRingWithIdentityConcept
  {
    typedef typename boost::binary_op_return<op_multiply, X, X>::type mult_ret;
    typedef typename boost::unary_op_return<op_one, X>::type one_ret;    

    void constraints() {
      function_requires< AdditiveAbelianGroupConcept<X> >();
      const mult_ret& c = a * b;
      const one_ret& d = one(c);
      X& e = (a -= d);
      ignore_unused_variable_warning(e);
    }
    X a, b;
    // Semantic Invariants
    //
    // Associativity of Multiplication: a * (b * c) == (a * b) * c 
    // Distributivity: a*(b + c) == a*b + a*c, (b + c)*a == b*a + c*a 
    // Definition of Multiplicative Identity: a * one(a) == one(a) * a == a 
    // Meaning of multiplication-assignment:
    //   If c == b then after executing b *= a and c = c * a we still
    //   have c == b.
  };
  
  template <class X>
  struct CommutativeMultiplicativeRingWithIdentityConcept
  {
    void constraints() {
      function_requires< MultiplicativeRingWithIdentityConcept<X> >();
    }
    // Semantic Invariants
    //
    // Commutativity: a * b == b * a 
  };

  template <class X>
  struct FieldConcept {
    void constraints() {
    typedef typename boost::binary_op_return<op_divide, X, X>::type div_ret;

      function_requires< 
	CommutativeMultiplicativeRingWithIdentityConcept<X> >();
      const div_ret& c = a / b;
      X& d = (b /= a);
      ignore_unused_variable_warning(c);
      ignore_unused_variable_warning(d);
    }
    X a, b;

    // Semantic Invariants
    //
    // Definition of Multplicative Inverse:
    //   If a * x == b then x == b/a. 
    //   a / b is undefined if and only if b == zero(a). 
    // Meaning of assignment-division:
    //   If c == b then after executing b /= a and c = c / a we still
    //   have c == b.
  };
  
  template <class G, class R>
  struct RModuleConcept {
    typedef typename boost::binary_op_return<op_multiply,G,R>::type rmult_ret;
    typedef typename boost::binary_op_return<op_multiply, 
      R, rmult_ret>::type lmult_ret;
    typedef typename boost::binary_op_return<op_multiply, 
      lmult_ret, R>::type rmult_ret2;
    void constraints() {
      function_requires< AdditiveAbelianGroupConcept<G> >();
      function_requires< MultiplicativeRingWithIdentityConcept<R> >();
      G& y = (x *= a);
      const rmult_ret& z = y * a;
      const lmult_ret& w = a * z;
      const rmult_ret2& q = w * a;
      ignore_unused_variable_warning(q);
    }
    struct bar{};
    void foo(bar) { }
    G x;
    R a;
    // Semantic Invariants
    //
    // Distributive: (a + b)*x == a*x + b*x, a*(x + y) == a*x + a*y
    // Associative: a * (b * x) == (a * b) * x
    // Identity: one(a) * x == x 
    // Commutativity (if multiplication on R is commutative):
    //   a * x == x * a 
  };
  
  template <class G, class F>
  struct VectorSpaceConcept {
    typedef typename boost::binary_op_return<op_divide,G,F>::type div_ret;
    void constraints() {
      function_requires< RModuleConcept<G, F> >();
      function_requires< FieldConcept<F> >();

      const div_ret& y = x / a;
      G& z = (x /= a);
      ignore_unused_variable_warning(y);
      ignore_unused_variable_warning(z);
    }
    G x;
    F a;
  };

  // To be continued...
  
} // namespace boost

#endif // BOOST_NUMERIC_LINALG_CONCEPTS_HPP
