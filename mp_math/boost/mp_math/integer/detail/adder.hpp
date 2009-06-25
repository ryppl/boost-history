// Copyright Tom St Denis 2002 - 2007.
// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_DETAIL_ADDER_HPP
#define BOOST_MP_MATH_INTEGER_DETAIL_ADDER_HPP

#include <stdexcept>
#include <boost/mp_math/integer/detail/base/adder.hpp>

namespace boost {
namespace mp_math {
namespace detail {


template<class ApInt, bool IsSigned = ApInt::is_signed>
struct adder;


template<class ApInt>
struct adder<ApInt, false>
{
  typedef ApInt                 int_type;
  typedef base::adder<int_type> base_adder_type;

  static void add     (ApInt& z, const ApInt& x);
  static void subtract(ApInt& z, const ApInt& x);
};


template<class ApInt>
void adder<ApInt, false>::add(ApInt& z, const ApInt& x)
{
  const ApInt* a;
  const ApInt* b;

  // a will point to the number with the most digits
  if (z.size() > x.size())
  {
    a = &z;
    b = &x;
  }
  else
  {
    a = &x;
    b = &z;
  }

  z.reserve(a->size() + 1);

  base_adder_type::add_smaller_magnitude(z, *a, *b);
}

template<class ApInt>
void adder<ApInt, false>::subtract(ApInt& z, const ApInt& x)
{
  if (!(z < x)) // means z >= rhs
    base_adder_type::subtract_smaller_magnitude(z, x);
  else
    throw std::runtime_error("adder::subtract: negative result");
}


template<class ApInt>
struct adder<ApInt, true>
{
  typedef ApInt                                    int_type;
  typedef base::adder<int_type>                    base_adder_type;
  typedef typename int_type::magnitude_type        magnitude_type;
  typedef typename int_type::traits_type::ops_type ops_type;

  static void add          (ApInt& z, const ApInt& x);
  static void add_magnitude(ApInt& z, const ApInt& x);
  static void subtract     (ApInt& z, const ApInt& x);
};


template<class ApInt>
void adder<ApInt, true>::add(ApInt& z, const ApInt& x)
{
  if (z.sign_bit() == x.sign_bit())
    add_magnitude(z, x);
  else
  {
    // Subtract the one with the lesser magnitude from the one of the greater
    // magnitude. The result gets the sign of the one with the greater
    // magnitude.
    const ApInt* a;
    const ApInt* b;

    // a will point to the number with the most digits
    if (static_cast<const magnitude_type&>(z) >=
        static_cast<const magnitude_type&>(x))
    {
      a = &z;
      b = &x;
    }
    else
    {
      z.reserve(x.size());
      z.set_sign_bit(x.sign_bit());
      a = &x;
      b = &z;
    }

    ops_type::sub_smaller_magnitude(z.digits(), a->digits(), a->size(),
                                                b->digits(), b->size());
    z.set_size(a->size());

    z.clamp();

    if (!z)
      z.set_sign_bit(0);
  }
}

template<class ApInt>
void adder<ApInt, true>::add_magnitude(ApInt& z, const ApInt& x)
{
  const ApInt* a;
  const ApInt* b;

  // a will point to the number with the most digits
  if (z.size() > x.size())
  {
    a = &z;
    b = &x;
  }
  else
  {
    a = &x;
    b = &z;
  }

  z.reserve(a->size() + 1);

  base_adder_type::add_smaller_magnitude(z, *a, *b);
}

template<class ApInt>
void adder<ApInt, true>::subtract(ApInt& z, const ApInt& x)
{
  if (z.sign_bit() != x.sign_bit())
    // add magnitudes, and use the sign of z.
    add_magnitude(z, x);
  else
  {
    const ApInt* a;
    const ApInt* b;
    if (static_cast<const magnitude_type&>(z) >=
        static_cast<const magnitude_type&>(x))
    {
      a = &z;
      b = &x;
    }
    else
    {
      z.reserve(x.size());
      // result has opposite sign from z
      z.set_sign_bit(!z.sign_bit());
      a = &x;
      b = &z;
    }

    ops_type::sub_smaller_magnitude(z.digits(), a->digits(), a->size(),
                                                b->digits(), b->size());
    z.set_size(a->size());

    z.clamp();

    if (!z)
      z.set_sign_bit(0);
  }
}



} // namespace detail
} // namespace mp_math
} // namespace boost

#endif

