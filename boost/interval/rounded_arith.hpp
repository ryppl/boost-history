#ifndef BOOST_INTERVAL_ROUNDED_ARITH_HPP
#define BOOST_INTERVAL_ROUNDED_ARITH_HPP

#ifndef BOOST_INTERVAL_HPP
#error Internal header file: This header must be included by <boost/interval.hpp> only.
#endif

namespace boost {

  namespace detail {

    template <class T> inline bool sign(const T& x) { return x < 0; }
    template <class T> inline bool is_nan(const T& x) { return x != x; }

  } // namespace detail

  namespace interval_lib {

    namespace detail {

      template <class T>
      inline T smallest()
      {
        if (std::numeric_limits<T>::is_integer)
	  return 1;
        else
	  return std::numeric_limits<T>::min();
      }

    } // namespace detail

  /*
   * Three classes of rounding: exact, std, opp
   * See documentation for details.
   */

  template<class T, class Rounding>
  struct rounded_arith_exact: Rounding {
    void init() { }
    T add_down (const T& x, const T& y) { return x + y; }
    T add_up   (const T& x, const T& y) { return x + y; }
    T sub_down (const T& x, const T& y) { return x - y; }
    T sub_up   (const T& x, const T& y) { return x - y; }
    T mul_down (const T& x, const T& y) { return x * y; }
    T mul_up   (const T& x, const T& y) { return x * y; }
    T div_down (const T& x, const T& y) { return x / y; }
    T div_up   (const T& x, const T& y) { return x / y; }
    T median   (const T& x, const T& y) { return (x + y) / 2; }
    T sqrt_down(const T& x)             { return std::sqrt(x); }
    T sqrt_up  (const T& x)             { return std::sqrt(x); }
    T int_down (const T& x)             { return std::floor(x); }
    T int_up   (const T& x)             { return std::ceil(x); }
  };

  template<class T, class Rounding>
  struct rounded_arith_std: Rounding {
    #define ROUND_DOWN(EXPR) (downward(),   force_rounding( EXPR ))
    #define ROUND_NEAR(EXPR) (to_nearest(), force_rounding( EXPR ))
    #define ROUND_UP  (EXPR) (upward(),     force_rounding( EXPR ))
    void init() { }
    T add_down (const T& x, const T& y) { return ROUND_DOWN ( x + y        ); }
    T add_up   (const T& x, const T& y) { return ROUND_UP   ( x + y        ); }
    T sub_down (const T& x, const T& y) { return ROUND_DOWN ( x - y        ); }
    T sub_up   (const T& x, const T& y) { return ROUND_UP   ( x - y        ); }
    T mul_down (const T& x, const T& y) { return ROUND_DOWN ( x * y        ); }
    T mul_up   (const T& x, const T& y) { return ROUND_UP   ( x * y        ); }
    T div_down (const T& x, const T& y) { return ROUND_DOWN ( x / y        ); }
    T div_up   (const T& x, const T& y) { return ROUND_UP   ( x / y        ); }
    T median   (const T& x, const T& y) { return ROUND_NEAR ( (x+y)/2      ); }
    T sqrt_down(const T& x)             { return ROUND_DOWN ( std::sqrt(x) ); }
    T sqrt_up  (const T& x)             { return ROUND_UP   ( std::sqrt(x) ); }
    T int_down (const T& x)             { return ROUND_DOWN ( to_int(x)    ); }
    T int_up   (const T& x)             { return ROUND_UP   ( to_int(x)    ); }
    #undef ROUND_DOWN
    #undef ROUND_NEAR
    #undef ROUND_UP
  };
  
  template<class T, class Rounding>
  struct rounded_arith_opp: Rounding {
    void init() { upward(); }
    #define ROUND_DOWN(EXPR) downward(),  force_rounding( EXPR )
    #define ROUND_NEAR(EXPR) tonearest(), force_rounding( EXPR )
    #define ROUND_UP  (EXPR)              force_rounding( EXPR )
    T add_down (const T& x, const T& y) { return -ROUND_UP  ( (-x) - y ); }
    T add_up   (const T& x, const T& y) { return ROUND_UP   ( x + y    ); }
    T sub_down (const T& x, const T& y) { return -ROUND_UP  ( y - x    ); }
    T sub_up   (const T& x, const T& y) { return ROUND_UP   ( x - y    ); }
    T mul_down (const T& x, const T& y) { return -ROUND_UP  ( x * (-y) ); }
    T mul_up   (const T& x, const T& y) { return ROUND_UP   ( x * y    ); }
    T div_down (const T& x, const T& y) { return -ROUND_UP  ( x / (-y) ); }
    T div_up   (const T& x, const T& y) { return ROUND_UP   ( x / y    ); }
    T median   (const T& x, const T& y) { T r = ROUND_NEAR  ( (x + y) / 2  );
                                          upward();
                                          return r;
					}
    T sqrt_down(const T& x)             { T r = ROUND_DOWN ( std::sqrt(x) );
                                          upward();
                                          return r;
					}
    T sqrt_up  (const T& x)             { return ROUND_UP   ( std::sqrt(x) ); }
    T int_down (const T& x)             { return -to_int(-x); }
    T int_up   (const T& x)             { return to_int(x); }
    #undef ROUND_DOWN
    #undef ROUND_NEAR
    #undef ROUND_UP
  };

  } // namespace interval_lib

} // namespace boost


#endif // BOOST_INTERVAL_ROUNDED_ARITH_HPP
