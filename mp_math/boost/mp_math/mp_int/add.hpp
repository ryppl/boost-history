// Copyright Kevin Sopp 2008.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

template<class A, class T>
void mp_int<A,T>::add_digit(digit_type b)
{
  if (is_positive())
  {
    grow_capacity(size_ + 1);
    const digit_type carry =
      ops_type::add_single_digit(digits_, digits_, size_, b);
    if (carry)
      push(carry);
  }
  else
  {
    if (digits_[0] >= b) // example: -16 + 5 = -11; or -5 + 5 = 0
    {
      digits_[0] -= b;
      if (!*this)
        set_sign(1);
    }
    else
    {
      if (size_ == 1) // example: -1 + 5 = 4
        digits_[0] = b - digits_[0];
      else            // example -11 + 5 = -6
      {
        set_sign(1);
        sub_digit(b);
        set_sign(-1);
      }
    }
  }
}

/* low level addition, based on HAC pp.594, Algorithm 14.7 */
// does not handle sign
template<class A, class T>
void mp_int<A,T>::add_magnitude(const mp_int& rhs)
{
  const mp_int* x;
  const mp_int* y;

  // x will point to the number with the most digits
  if (size_ > rhs.size_)
  {
    x = this;
    y = &rhs;
  }
  else
  {
    x = &rhs;
    y = this;
  }

  grow_capacity(x->size_ + 1);

  digit_type carry = ops_type::add_digits(digits_,
                                          x->digits_,
                                          y->digits_, y->size_);

  size_type n = ops_type::ripple_carry(digits_ + y->size_,
                                       x->digits_ + y->size_,
                                       x->size_ - y->size_, carry);
  n += y->size_;

  if (n < x->size_) // this implies that there is no carry left
  {
    if (x != this)
    {
      std::memcpy(digits_ + n, x->digits_ + n, sizeof(digit_type) * (x->size_ - n));
      size_ = x->size_;
    }
    return;
  }
  else if (carry) // at this point n equals x->size_
    digits_[n++] = carry;

  size_ = n;
}

