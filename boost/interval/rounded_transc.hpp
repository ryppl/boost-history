#ifndef BOOST_INTERVAL_ROUNDED_TRANSC_HPP
#define BOOST_INTERVAL_ROUNDED_TRANSC_HPP

#ifndef BOOST_INTERVAL_HPP
#error Internal header file: This header must be included by <boost/interval.hpp> only.
#endif

#include <cmath>

namespace boost {

  namespace interval_lib {

  template<class T, class Rounding>
  struct rounded_transc_dummy: Rounding {
    T exp_down  (const T& x) { return 0; }
    T exp_up    (const T& x) { return std::numeric_limits<T>::infinity(); }
    T log_down  (const T& x) { return -std::numeric_limits<T>::infinity(); }
    T log_up    (const T& x) { return std::numeric_limits<T>::infinity(); }
    T sin_down  (const T& x) { return -1; }
    T sin_up    (const T& x) { return 1; }
    T cos_down  (const T& x) { return -1; }
    T cos_up    (const T& x) { return 1; }
    T tan_down  (const T& x) { return -std::numeric_limits<T>::infinity(); }
    T tan_up    (const T& x) { return std::numeric_limits<T>::infinity(); }
    T asin_down (const T& x) { return -pi_1_2_up(); }
    T asin_up   (const T& x) { return pi_1_2_up(); }
    T acos_down (const T& x) { return 0; }
    T acos_up   (const T& x) { return pi_up(); }
    T atan_down (const T& x) { return -pi_1_2_up(); }
    T atan_up   (const T& x) { return pi_1_2_up(); }
    T sinh_down (const T& x) { return -std::numeric_limits<T>::infinity(); }
    T sinh_up   (const T& x) { return std::numeric_limits<T>::infinity(); }
    T cosh_down (const T& x) { return 1; }
    T cosh_up   (const T& x) { return std::numeric_limits<T>::infinity(); }
    T tanh_down (const T& x) { return -1; }
    T tanh_up   (const T& x) { return 1; }
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
    T exp_down  (const T& x) { using std::exp; return exp(x); }
    T exp_up    (const T& x) { using std::exp; return exp(x); }
    T log_down  (const T& x) { using std::log; return log(x); }
    T log_up    (const T& x) { using std::log; return log(x); }
    T sin_down  (const T& x) { using std::sin; return sin(x); }
    T sin_up    (const T& x) { using std::sin; return sin(x); }
    T cos_down  (const T& x) { using std::cos; return cos(x); }
    T cos_up    (const T& x) { using std::cos; return cos(x); }
    T tan_down  (const T& x) { using std::tan; return tan(x); }
    T tan_up    (const T& x) { using std::tan; return tan(x); }
    T asin_down (const T& x) { using std::asin; return asin(x); }
    T asin_up   (const T& x) { using std::asin; return asin(x); }
    T acos_down (const T& x) { using std::acos; return acos(x); }
    T acos_up   (const T& x) { using std::acos; return acos(x); }
    T atan_down (const T& x) { using std::atan; return atan(x); }
    T atan_up   (const T& x) { using std::atan; return atan(x); }
    T sinh_down (const T& x) { using std::sinh; return sinh(x); }
    T sinh_up   (const T& x) { using std::sinh; return sinh(x); }
    T cosh_down (const T& x) { using std::cosh; return cosh(x); }
    T cosh_up   (const T& x) { using std::cosh; return cosh(x); }
    T tanh_down (const T& x) { using std::tanh; return tanh(x); }
    T tanh_up   (const T& x) { using std::tanh; return tanh(x); }
#ifdef BOOST_HAVE_INV_HYPERBOLIC
    T asinh_down(const T& x) { using std::asinh; return asinh(x); }
    T asinh_up  (const T& x) { using std::asinh; return asinh(x); }
    T acosh_down(const T& x) { using std::acosh; return acosh(x); }
    T acosh_up  (const T& x) { using std::acosh; return acosh(x); }
    T atanh_down(const T& x) { using std::atanh; return atanh(x); }
    T atanh_up  (const T& x) { using std::atanh; return atanh(x); }
#endif
  };
  
  template<class T, class Rounding>
  struct rounded_transc_std: Rounding
  {
    #define ROUND_DOWN(EXPR) (downward(),   force_rounding( EXPR ))
    #define ROUND_UP(EXPR)   (upward(),     force_rounding( EXPR ))
    T exp_down  (const T& x) { using std::exp; return ROUND_DOWN( exp(x)   ); }
    T exp_up    (const T& x) { using std::exp; return ROUND_UP  ( exp(x)   ); }
    T log_down  (const T& x) { using std::log; return ROUND_DOWN( log(x)   ); }
    T log_up    (const T& x) { using std::log; return ROUND_UP  ( log(x)   ); }
    T sin_down  (const T& x) { using std::sin; return ROUND_DOWN( sin(x)   ); }
    T sin_up    (const T& x) { using std::sin; return ROUND_UP  ( sin(x)   ); }
    T cos_down  (const T& x) { using std::cos; return ROUND_DOWN( cos(x)   ); }
    T cos_up    (const T& x) { using std::cos; return ROUND_UP  ( cos(x)   ); }
    T tan_down  (const T& x) { using std::tan; return ROUND_DOWN( tan(x)   ); }
    T tan_up    (const T& x) { using std::tan; return ROUND_UP  ( tan(x)   ); }
    T asin_down (const T& x) { using std::asin; return ROUND_DOWN( asin(x)  ); }
    T asin_up   (const T& x) { using std::asin; return ROUND_UP  ( asin(x)  ); }
    T acos_down (const T& x) { using std::acos; return ROUND_DOWN( acos(x)  ); }
    T acos_up   (const T& x) { using std::acos; return ROUND_UP  ( acos(x)  ); }
    T atan_down (const T& x) { using std::atan; return ROUND_DOWN( atan(x)  ); }
    T atan_up   (const T& x) { using std::atan; return ROUND_UP  ( atan(x)  ); }
    T sinh_down (const T& x) { using std::sinh; return ROUND_DOWN( sinh(x)  ); }
    T sinh_up   (const T& x) { using std::sinh; return ROUND_UP  ( sinh(x)  ); }
    T cosh_down (const T& x) { using std::cosh; return ROUND_DOWN( cosh(x)  ); }
    T cosh_up   (const T& x) { using std::cosh; return ROUND_UP  ( cosh(x)  ); }
    T tanh_down (const T& x) { using std::tanh; return ROUND_DOWN( tanh(x)  ); }
    T tanh_up   (const T& x) { using std::tanh; return ROUND_UP  ( tanh(x)  ); }
#ifdef BOOST_HAVE_INV_HYPERBOLIC
    T asinh_down(const T& x) { using std::asinh; return ROUND_DOWN( asinh(x) );}
    T asinh_up  (const T& x) { using std::asinh; return ROUND_UP  ( asinh(x) );}
    T acosh_down(const T& x) { using std::acosh; return ROUND_DOWN( acosh(x) );}
    T acosh_up  (const T& x) { using std::acosh; return ROUND_UP  ( acosh(x) );}
    T atanh_down(const T& x) { using std::atanh; return ROUND_DOWN( atanh(x) );}
    T atanh_up  (const T& x) { using std::atanh; return ROUND_UP  ( atanh(x) );}
#endif
    #undef ROUND_DOWN
    #undef ROUND_UP
  };

  template<class T, class Rounding>
  struct rounded_transc_opp: Rounding
  {
    #define ROUND_DOWN(EXPR) (downward(),  force_rounding( EXPR ))
    #define ROUND_NEAR(EXPR) (tonearest(), force_rounding( EXPR ))
    #define ROUND_UP(EXPR)   force_rounding( EXPR )
    T exp_down  (const T& x) { using std::exp;
	                       T y = ROUND_DOWN ( exp(x)    );
                               upward();
                               return y;
                             }
    T exp_up    (const T& x) { using std::exp; return ROUND_UP  ( exp(x)    ); }
    T log_down  (const T& x) { using std::log; T y = ROUND_DOWN ( log(x)    );
                               upward();
                               return y;
                             }
    T log_up    (const T& x) { using std::log; return ROUND_UP  ( log(x)    ); }
    T sin_down  (const T& x) { using std::sin; return -ROUND_UP ( sin(-x)   ); }
    T sin_up    (const T& x) { using std::sin; return ROUND_UP  ( sin(x)    ); }
    T cos_down  (const T& x) { using std::cos; T y = ROUND_DOWN ( cos(x)    );
			       upward();
                               return y;
			     }
    T cos_up    (const T& x) { using std::cos; return ROUND_UP  ( cos(x)    ); }
    T tan_down  (const T& x) { using std::tan; return -ROUND_UP ( tan(-x)   ); }
    T tan_up    (const T& x) { using std::tan; return ROUND_UP  ( tan(x)    ); }
    T asin_down (const T& x) { using std::asin; return -ROUND_UP ( asin(-x) ); }
    T asin_up   (const T& x) { using std::asin; return ROUND_UP  ( asin(x)  ); }
    T acos_down (const T& x) { using std::acos; T y = ROUND_DOWN ( acos(x)  );
                               upward();
                               return y;
			     }
    T acos_up   (const T& x) { using std::acos; return ROUND_UP  ( acos(x)  ); }
    T atan_down (const T& x) { using std::atan; return -ROUND_UP ( atan(-x) ); }
    T atan_up   (const T& x) { using std::atan; return ROUND_UP  ( atan(x)  ); }
    T sinh_down (const T& x) { using std::sinh; return -ROUND_UP ( sinh(-x) ); }
    T sinh_up   (const T& x) { using std::sinh; return ROUND_UP  ( sinh(x)  ); }
    T cosh_down (const T& x) { using std::cosh; T y = ROUND_DOWN ( cosh(x)  );
                               upward();
                               return y;
                             }
    T cosh_up   (const T& x) { using std::cosh; return ROUND_UP  ( cosh(x)  ); }
    T tanh_down (const T& x) { using std::tanh; return -ROUND_UP ( tanh(-x) ); }
    T tanh_up   (const T& x) { using std::tanh; return ROUND_UP  ( tanh(x)  ); }
#ifdef BOOST_HAVE_INV_HYPERBOLIC
    T asinh_down(const T& x) { using std::asinh; return -ROUND_UP ( asinh(-x));}
    T asinh_up  (const T& x) { using std::asinh; return ROUND_UP  ( asinh(x) );}
    T acosh_down(const T& x) { using std::acosh; T y = ROUND_DOWN ( acosh(x) );
                               upward();
                               return y;
			     }
    T acosh_up  (const T& x) { using std::acosh; return ROUND_UP  ( acosh(x) );}
    T atanh_down(const T& x) { using std::atanh; return -ROUND_UP ( atanh(-x));}
    T atanh_up  (const T& x) { using std::atanh; return ROUND_UP  ( atanh(x) );}
#endif
    #undef ROUND_DOWN
    #undef ROUND_NEAR
    #undef ROUND_UP
  };
  
  } // interval_lib

} // boost

#if 0

namespace boost {

  namespace interval_lib {

  template<class T, class Rounding>
  struct hyperbolic_from_explog: Rounding
  {
    T sinh_down (const T& x);
    T sinh_up   (const T& x);
    T cosh_down (const T& x);
    T cosh_up   (const T& x);
    T tanh_down (const T& x);
    T tanh_up   (const T& x);
    T asinh_down(const T& x);
    T asinh_up  (const T& x);
    T acosh_down(const T& x);
    T acosh_up  (const T& x);
    T atanh_down(const T& x);
    T atanh_up  (const T& x);
  };

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
