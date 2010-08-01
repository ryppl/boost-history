// Copyright Justinas Vygintas Daugmaudis, 2010.
// Use, modification and distribution is subject to the
// Boost Software License, Version 1.0. (See accompanying
// file LICENSE-1.0 or http://www.boost.org/LICENSE-1.0)

#ifndef HEADER_VECTOR_N_HPP_INCLUDED
#define HEADER_VECTOR_N_HPP_INCLUDED

#include <boost/call_traits.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include "detail/list_assignment.hpp"
#include "detail/constant_size_array.hpp"

namespace math {

template<class T, std::size_t N>
class vector_n : public boost::numeric::ublas::vector<
    T
  , detail::constant_size_array<T, N> >
{
private:
  typedef detail::constant_size_array<T, N>
      array_type;
  typedef typename boost::call_traits<T>::param_type
      param_type;

  typedef boost::numeric::ublas::vector<
      T
    , array_type > base_vector;

  typedef detail::scalar_or_list_assignment<
      param_type
    , typename array_type::iterator > assignment_expression;
public:
  //! Default construction.
  vector_n(): base_vector(N) {}

  //! Convenience constructors
  vector_n(T v0) : base_vector(N)
  {
    BOOST_STATIC_ASSERT( N == 1 );
    this->data()[0] = v0;
  }

  vector_n(T v0, T v1) : base_vector(N)
  {
    BOOST_STATIC_ASSERT( N == 2 );
    this->data()[0] = v0;
    this->data()[1] = v1;
  }

  vector_n(T v0, T v1, T v2) : base_vector(N)
  {
    BOOST_STATIC_ASSERT( N == 3 );
    this->data()[0] = v0;
    this->data()[1] = v1;
    this->data()[2] = v2;
  }

  vector_n(T v0, T v1, T v2, T v3) : base_vector(N)
  {
    BOOST_STATIC_ASSERT( N == 4 );
    this->data()[0] = v0;
    this->data()[1] = v1;
    this->data()[2] = v2;
    this->data()[3] = v3;
  }

  //! Construction and assignment
  //! from a uBLAS vector expression
  //! or copy assignment.
  template<class R>
  vector_n(const boost::numeric::ublas::vector_expression<R>& r)
    : base_vector(r)
  {}

  template<class R>
  vector_n& operator=(const boost::numeric::ublas::vector_expression<R>& r)
  {
    base_vector::operator=(r);
    return *this;
  }

  // Assignment from a scalar operand or a list of scalar operands.
  assignment_expression operator=(param_type value)
  {
    return assignment_expression(this->data().begin(), this->data().end(), value);
  }

  // Convenient getters.
  T x() const { return this->data()[0]; }
  T y() const { BOOST_STATIC_ASSERT(N > 1); return this->data()[1]; }
  T z() const { BOOST_STATIC_ASSERT(N > 2); return this->data()[2]; }
  T w() const { BOOST_STATIC_ASSERT(N > 3); return this->data()[3]; }
};

template<class T, std::size_t N>
inline T length_sq(const vector_n<T, N>& v)
{
  return boost::numeric::ublas::inner_prod(v, v);
}

template<typename T, std::size_t N>
inline T distance(vector_n<T, N> lhs, const vector_n<T, N>& rhs)
{
  lhs -= rhs;
  return std::sqrt(length_sq(lhs));
}

} // namespace math

#endif // HEADER_VECTOR_N_HPP_INCLUDED
