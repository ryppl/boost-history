#ifndef BOOST_OP_RETURN_TRAITS_HPP
#define BOOST_OP_RETURN_TRAITS_HPP

namespace boost {

  // Operator Tags
  struct op_add { };
  struct op_negate { };
  struct op_subtract { };
  struct op_zero { };
  struct op_multiply { };
  struct op_divide { };
  struct op_one { };

  // Default (unspecialized) binary_op_return, a traits class that
  // computes the *exact* return type of a binary operator expression.
  template <class OperatorTag, class A, class B>
  struct binary_op_return {
    typedef A type;
  };

  // Default (unspecialized) unary_op_return, a traits class that
  // computes the *exact* return type of a unary operator expression.
  template <class OperatorTag, class A>
  struct unary_op_return {
    typedef A type;
  };

  template <class T> T zero(const T& x) { return T(0); }
  template <class T> T one(const T& x) { return T(1); }

} // namespace boost

#endif // BOOST_OP_RETURN_TRAITS_HPP
