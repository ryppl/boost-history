#ifndef BOOST_INTERVAL_ROUNDED_TRANSC_HPP
#define BOOST_INTERVAL_ROUNDED_TRANSC_HPP

#ifndef BOOST_INTERVAL_HPP
#error Internal header file: This header must be included by <boost/interval.hpp> only.
#endif

namespace boost {
  namespace interval_lib {

template<class T, class Rounding>
struct rounded_transc_dummy: Rounding {
  T exp_down(const T& x) { return 0; }
  T exp_up  (const T& x) { return std::numeric_limits<T>::infinity(); }
  T log_down(const T& x) { return -std::numeric_limits<T>::infinity(); }
  T log_up  (const T& x) { return std::numeric_limits<T>::infinity(); }
  T sin_down(const T& x) { return -1; }
  T sin_up  (const T& x) { return 1; }
  T cos_down(const T& x) { return -1; }
  T cos_up  (const T& x) { return 1; }
  T tan_down(const T& x) { return -std::numeric_limits<T>::infinity(); }
  T tan_up  (const T& x) { return std::numeric_limits<T>::infinity(); }
  T asin_down(const T& x) { return -pi_1_2_up(); }
  T asin_up  (const T& x) { return pi_1_2_up(); }
  T acos_down(const T& x) { return 0; }
  T acos_up  (const T& x) { return pi_up(); }
  T atan_down(const T& x) { return -pi_1_2_up(); }
  T atan_up  (const T& x) { return pi_1_2_up(); }
  T sinh_down(const T& x) { return -std::numeric_limits<T>::infinity(); }
  T sinh_up  (const T& x) { return std::numeric_limits<T>::infinity(); }
  T cosh_down(const T& x) { return 1; }
  T cosh_up  (const T& x) { return std::numeric_limits<T>::infinity(); }
  T tanh_down(const T& x) { return -1; }
  T tanh_up  (const T& x) { return 1; }
  T asinh_down(const T& x) { return -std::numeric_limits<T>::infinity(); }
  T asinh_up  (const T& x) { return std::numeric_limits<T>::infinity(); }
  T acosh_down(const T& x) { return 0; }
  T acosh_up  (const T& x) { return std::numeric_limits<T>::infinity(); }
  T atanh_down(const T& x) { return -std::numeric_limits<T>::infinity(); }
  T atanh_up  (const T& x) { return std::numeric_limits<T>::infinity(); }
};

template<class T, class Rounding>
struct rounded_transc_exact: Rounding
{
  T exp_down(const T& x) { return std::exp(x); }
  T exp_up  (const T& x) { return std::exp(x); }
  T log_down(const T& x) { return std::log(x); }
  T log_up  (const T& x) { return std::log(x); }
  T sin_down(const T& x) { return std::sin(x); }
  T sin_up  (const T& x) { return std::sin(x); }
  T cos_down(const T& x) { return std::cos(x); }
  T cos_up  (const T& x) { return std::cos(x); }
  T tan_down(const T& x) { return std::tan(x); }
  T tan_up  (const T& x) { return std::tan(x); }
  T asin_down(const T& x) { return std::asin(x); }
  T asin_up  (const T& x) { return std::asin(x); }
  T acos_down(const T& x) { return std::acos(x); }
  T acos_up  (const T& x) { return std::acos(x); }
  T atan_down(const T& x) { return std::atan(x); }
  T atan_up  (const T& x) { return std::atan(x); }
  T sinh_down(const T& x) { return std::sinh(x); }
  T sinh_up  (const T& x) { return std::sinh(x); }
  T cosh_down(const T& x) { return std::cosh(x); }
  T cosh_up  (const T& x) { return std::cosh(x); }
  T tanh_down(const T& x) { return std::tanh(x); }
  T tanh_up  (const T& x) { return std::tanh(x); }
  T asinh_down(const T& x) { return std::asinh(x); }
  T asinh_up  (const T& x) { return std::asinh(x); }
  T acosh_down(const T& x) { return std::acosh(x); }
  T acosh_up  (const T& x) { return std::acosh(x); }
  T atanh_down(const T& x) { return std::atanh(x); }
  T atanh_up  (const T& x) { return std::atanh(x); }
};

template<class T, class Rounding>
struct rounded_transc_std: Rounding
{
  T exp_down(const T& x) { return (downward(), force_rounding(std::exp(x))); }
  T exp_up  (const T& x) { return (upward()  , force_rounding(std::exp(x))); }
  T log_down(const T& x) { return (downward(), force_rounding(std::log(x))); }
  T log_up  (const T& x) { return (upward()  , force_rounding(std::log(x))); }
  T sin_down(const T& x) { return (downward(), force_rounding(std::sin(x))); }
  T sin_up  (const T& x) { return (upward()  , force_rounding(std::sin(x))); }
  T cos_down(const T& x) { return (downward(), force_rounding(std::cos(x))); }
  T cos_up  (const T& x) { return (upward()  , force_rounding(std::cos(x))); }
  T tan_down(const T& x) { return (downward(), force_rounding(std::tan(x))); }
  T tan_up  (const T& x) { return (upward()  , force_rounding(std::tan(x))); }
  T asin_down(const T& x) { return (downward(), force_rounding(std::asin(x))); }
  T asin_up  (const T& x) { return (upward()  , force_rounding(std::asin(x))); }
  T acos_down(const T& x) { return (downward(), force_rounding(std::acos(x))); }
  T acos_up  (const T& x) { return (upward()  , force_rounding(std::acos(x))); }
  T atan_down(const T& x) { return (downward(), force_rounding(std::atan(x))); }
  T atan_up  (const T& x) { return (upward()  , force_rounding(std::atan(x))); }
  T sinh_down(const T& x) { return (downward(), force_rounding(std::sinh(x))); }
  T sinh_up  (const T& x) { return (upward()  , force_rounding(std::sinh(x))); }
  T cosh_down(const T& x) { return (downward(), force_rounding(std::cosh(x))); }
  T cosh_up  (const T& x) { return (upward()  , force_rounding(std::cosh(x))); }
  T tanh_down(const T& x) { return (downward(), force_rounding(std::tanh(x))); }
  T tanh_up  (const T& x) { return (upward()  , force_rounding(std::tanh(x))); }
  T asinh_down(const T& x) { return (downward(), force_rounding(std::asinh(x))); }
  T asinh_up  (const T& x) { return (upward()  , force_rounding(std::asinh(x))); }
  T acosh_down(const T& x) { return (downward(), force_rounding(std::acosh(x))); }
  T acosh_up  (const T& x) { return (upward()  , force_rounding(std::acosh(x))); }
  T atanh_down(const T& x) { return (downward(), force_rounding(std::atanh(x))); }
  T atanh_up  (const T& x) { return (upward()  , force_rounding(std::atanh(x))); }
};

template<class T, class Rounding>
struct rounded_transc_opp: Rounding
{
  T exp_down(const T& x)
  {
    downward();
    T y = force_rounding(std::exp(x));
    upward();
    return y;
  }
  T exp_up(const T& x) { return force_rounding(std::exp(x)); }
  T log_down(const T& x)
  {
    downward();
    T y = force_rounding(std::log(x));
    upward();
    return y;
  }
  T log_up  (const T& x) { return force_rounding(std::log(x)); }
  T sin_down(const T& x) { return -force_rounding(std::sin(-x)); }
  T sin_up  (const T& x) { return force_rounding(std::sin(x)); }
  T cos_down(const T& x)
  {
    downward();
    T y = force_rounding(std::cos(x));
    upward();
    return y;
  }
  T cos_up  (const T& x) { return force_rounding(std::cos(x)); }
  T tan_down(const T& x) { return -force_rounding(std::tan(-x)); }
  T tan_up  (const T& x) { return force_rounding(std::tan(x)); }
  T asin_down(const T& x) { return -force_rounding(std::asin(-x)); }
  T asin_up  (const T& x) { return force_rounding(std::asin(x)); }
  T acos_down(const T& x)
  {
    downward();
    T y = force_rounding(std::acos(x));
    upward();
    return y;
  }
  T acos_up  (const T& x) { return force_rounding(std::acos(x)); }
  T atan_down(const T& x) { return -force_rounding(std::atan(-x)); }
  T atan_up  (const T& x) { return force_rounding(std::atan(x)); }
  T sinh_down(const T& x) { return -force_rounding(std::sinh(-x)); }
  T sinh_up  (const T& x) { return force_rounding(std::sinh(x)); }
  T cosh_down(const T& x)
  {
    downward();
    T y = force_rounding(std::cosh(x));
    upward();
    return y;
  }
  T cosh_up  (const T& x) { return force_rounding(std::cosh(x)); }
  T tanh_down(const T& x) { return -force_rounding(std::tanh(-x)); }
  T tanh_up  (const T& x) { return force_rounding(std::tanh(x)); }
  T asinh_down(const T& x) { return -force_rounding(std::asinh(-x)); }
  T asinh_up  (const T& x) { return force_rounding(std::asinh(x)); }
  T acosh_down(const T& x)
  {
    downward();
    T y = force_rounding(std::acosh(x));
    upward();
    return y;
  }
  T acosh_up  (const T& x) { return force_rounding(std::acosh(x)); }
  T atanh_down(const T& x) { return -force_rounding(std::atanh(-x)); }
  T atanh_up  (const T& x) { return force_rounding(std::atanh(x)); }
};

  } // interval_lib
} // boost

#if 0
namespace boost {
  namespace interval_lib {

template<class T, class Rounding>
struct hyperbolic_from_explog: Rounding
{
  T sinh_down(const T& x);
  T sinh_up  (const T& x);
  T cosh_down(const T& x);
  T cosh_up  (const T& x);
  T tanh_down(const T& x);
  T tanh_up  (const T& x);
  T asinh_down(const T& x);
  T asinh_up  (const T& x);
  T acosh_down(const T& x);
  T acosh_up  (const T& x);
  T atanh_down(const T& x);
  T atanh_up  (const T& x);
};

  } // interval_lib
} // boost

namespace boost {
  namespace interval_lib {

template<class T, class Rounding> inline
T hyperbolic_from_explog<T, Rounding>::sinh_down(const T& x)
{
  return div_down(sub_down(exp_down(x), exp_up(-x)), 2);
}

template<class T, class Rounding> inline
T hyperbolic_from_explog<T, Rounding>::sinh_up(const T& x)
{
  return div_up(sub_up(exp_up(x), exp_down(-x)), 2);
}

template<class T, class Rounding> inline
T hyperbolic_from_explog<T, Rounding>::cosh_down(const T& x)
{
  return div_down(add_down(exp_down(x), exp_down(-x)), 2);
}

template<class T, class Rounding> inline
T hyperbolic_from_explog<T, Rounding>::cosh_up(const T& x)
{
  return div_up(add_up(exp_up(x), exp_up(-x)), 2);
}

template<class T, class Rounding> inline
T hyperbolic_from_explog<T, Rounding>::tanh_down(const T& x)
{
  return div_down(sub_down(exp_down(mul_down(x, 2)), 1),
		  add_up(exp_up(mul_up(x, 2)), 1));
}

template<class T, class Rounding> inline
T hyperbolic_from_explog<T, Rounding>::tanh_up(const T& x)
{
  return div_up(sub_up(exp_up(mul_up(x, 2)), 1),
		add_down(exp_down(mul_down(x, 2)), 1));
}

template<class T, class Rounding> inline
T hyperbolic_from_explog<T, Rounding>::asinh_down(const T& x)
{
  return log_down(add_down(x, sqrt_down(add_down(mul_down(x, x), 1))));
}

template<class T, class Rounding> inline
T hyperbolic_from_explog<T, Rounding>::asinh_up(const T& x)
{
  return log_up(add_up(x, sqrt_up(add_up(mul_up(x, x), 1))));
}

template<class T, class Rounding> inline
T hyperbolic_from_explog<T, Rounding>::acosh_down(const T& x)
{
  return log_down(add_down(x, sqrt_down(sub_down(mul_down(x, x), 1))));
}

template<class T, class Rounding> inline
T hyperbolic_from_explog<T, Rounding>::acosh_up(const T& x)
{
  return log_up(add_up(x, sqrt_up(sub_up(mul_up(x, x), 1))));
}

template<class T, class Rounding> inline
T hyperbolic_from_explog<T, Rounding>::atanh_down(const T& x)
{
  return div_down(log_down(div_down(add_down(1, x), sub_up(1, x))), 2);
}

template<class T, class Rounding> inline
T hyperbolic_from_explog<T, Rounding>::atanh_up(const T& x)
{
  return div_up(log_up(div_up(add_up(1, x), sub_down(1, x))), 2);
}

  } // interval_lib
} // boost
#endif

#endif // BOOST_INTERVAL_ROUNDED_TRANSC_HPP
