//  Copyright John Maddock 2007-8.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_INTERVAL_BINDINGS_HPP
#define BOOST_MATH_INTERVAL_BINDINGS_HPP

#include <boost/numeric/interval.hpp>
#include <boost/numeric/interval/limits.hpp>
#include <boost/numeric/interval/utility.hpp>
#include <boost/numeric/interval/compare/possible.hpp>
#include <boost/math/special_functions/math_fwd.hpp>

namespace boost {
namespace numeric {
namespace interval_lib{

using boost::math::asinh;
using boost::math::acosh;
using boost::math::atanh;

}

template<class T, class Policies>
inline interval<T, Policies> floor(const interval<T, Policies>& x)
{
   using std::floor;
   return interval<T, Policies>(floor(x.lower()), floor(x.upper()));
}

template<class T, class Policies>
inline interval<T, Policies> ceil(const interval<T, Policies>& x)
{
   using std::ceil;
   return interval<T, Policies>(ceil(x.lower()), ceil(x.upper()));
}

template<class T, class Policies>
inline interval<T, Policies> fabs(const interval<T, Policies>& x)
{
   return abs(x);
}

template<class T, class Policies>
inline interval<T, Policies> pow(const interval<T, Policies>& x, const interval<T, Policies>& y)
{
  typedef interval<T, Policies> I;

  if(y.lower() < 0)
  {
     if(y.upper() < 0)
     {
        return 1 / pow(x, -y);
     }
     else
     {
        I y1(0, y.upper(), true);
        I y2(y.lower(), 0, true);
        return hull(pow(x, y1), 1 / pow(x, -y2));
     }
  }

  if (interval_lib::detail::test_input(x))
    return I::empty();
  typename Policies::rounding rnd;
  I result;
  if(x.lower() > 1)
  {
     result = I(rnd.pow_down(x.lower(), y.lower()), rnd.pow_up(x.upper(), y.upper()), true);
  }
  else if(x.upper() < 1)
  {
     result = I(rnd.pow_down(x.lower(), y.upper()), rnd.pow_up(x.upper(), y.lower()), true);
  }
  else
  {
     result = I(rnd.pow_down(x.lower(), y.upper()), rnd.pow_up(x.upper(), y.upper()), true);
  }
  return result;
}

template<class T, class Policies>
inline interval<T, Policies> ldexp(const interval<T, Policies>& x, int i)
{
   using std::ldexp;
   return interval<T, Policies>(ldexp(x.lower(), i), ldexp(x.upper(), i));
}

template<class T, class Policies>
inline interval<T, Policies> frexp(const interval<T, Policies>& x, int* i)
{
   using std::frexp;
   using std::ldexp;
   frexp(median(x), i);
   return interval<T, Policies>(ldexp(x.lower(), -*i), ldexp(x.upper(), -*i));
}

//
// Comparisons override the defaults.
// We require a very specific form of comparisons:
// Mixed interval and T with throw if the comparison is 
// ambiguous.
//
namespace interval_math_compare
{

template<class T, class Policies1, class Policies2> inline
bool operator<(const interval<T, Policies1>& x, const interval<T, Policies2>& y)
{
  if (boost::numeric::interval_lib::detail::test_input(x, y)) throw boost::numeric::interval_lib::comparison_error();
  if(x.upper() < y.lower())
     return true;
  if(x.lower() >= y.upper())
     return false;
  throw boost::numeric::interval_lib::comparison_error();
}

template<class T, class Policies, class T2> inline
bool operator<(const interval<T, Policies>& x, const T2& y)
{
  if (boost::numeric::interval_lib::detail::test_input(x, y)) throw boost::numeric::interval_lib::comparison_error();
  if(x.upper() < y)
     return true;
  if(x.lower() >= y)
     return false;
  throw boost::numeric::interval_lib::comparison_error();
}

template<class T, class Policies, class T2> inline
bool operator<(const T2& x, const interval<T, Policies>& y)
{
  if (boost::numeric::interval_lib::detail::test_input(x, y)) throw boost::numeric::interval_lib::comparison_error();
  if(x < y.lower())
     return true;
  if(x >= y.upper())
     return false;
  throw boost::numeric::interval_lib::comparison_error();
}

template<class T, class Policies1, class Policies2> inline
bool operator<=(const interval<T, Policies1>& x, const interval<T, Policies2>& y)
{
  if (boost::numeric::interval_lib::detail::test_input(x, y)) throw boost::numeric::interval_lib::comparison_error();
  if(x.upper() <= y.lower())
    return true;
  if(x.lower() > y.upper())
     return false;
  throw boost::numeric::interval_lib::comparison_error();
}

template<class T, class Policies, class T2> inline
bool operator<=(const interval<T, Policies>& x, const T2& y)
{
  if (boost::numeric::interval_lib::detail::test_input(x, y)) throw boost::numeric::interval_lib::comparison_error();
  if(x.upper() <= y)
     return true;
  if(x.lower() > y)
     return false;
  throw boost::numeric::interval_lib::comparison_error();
}

template<class T, class Policies, class T2> inline
bool operator<=(const T2& x, const interval<T, Policies>& y)
{
  if (boost::numeric::interval_lib::detail::test_input(x, y)) throw boost::numeric::interval_lib::comparison_error();
  if(x <= y.lower())
     return true;
  if(x > y.upper())
     return false;
  throw boost::numeric::interval_lib::comparison_error();
}

template<class T, class Policies1, class Policies2> inline
bool operator>(const interval<T, Policies1>& x, const interval<T, Policies2>& y)
{
   return !(x <= y);
}

template<class T, class Policies, class T2> inline
bool operator>(const interval<T, Policies>& x, const T2& y)
{
   return !(x <= y);
}

template<class T, class Policies, class T2> inline
bool operator>(const T2& x, const interval<T, Policies>& y)
{
   return !(x <= y);
}

template<class T, class Policies1, class Policies2> inline
bool operator>=(const interval<T, Policies1>& x, const interval<T, Policies2>& y)
{
   return !(x < y);
}

template<class T, class Policies, class T2> inline
bool operator>=(const interval<T, Policies>& x, const T2& y)
{
   return !(x < y);
}

template<class T, class Policies, class T2> inline
bool operator>=(const T2& x, const interval<T, Policies>& y)
{
   return !(x < y);
}

template<class T, class Policies1, class Policies2> inline
bool operator==(const interval<T, Policies1>& x, const interval<T, Policies2>& y)
{
  if (boost::numeric::interval_lib::detail::test_input(x, y)) throw boost::numeric::interval_lib::comparison_error();
  return x.upper() == y.lower() && x.lower() == y.upper();
}

template<class T, class Policies, class T2> inline
bool operator==(const interval<T, Policies>& x, const T2& y)
{
  if (boost::numeric::interval_lib::detail::test_input(x, y)) throw boost::numeric::interval_lib::comparison_error();
  return x.upper() == y && x.lower() == y;
}

template<class T, class Policies, class T2> inline
bool operator==(const T2& x, const interval<T, Policies>& y)
{
   return y == x;
}

template<class T, class Policies1, class Policies2> inline
bool operator!=(const interval<T, Policies1>& x, const interval<T, Policies2>& y)
{
  if (boost::numeric::interval_lib::detail::test_input(x, y)) throw boost::numeric::interval_lib::comparison_error();
  return x.upper() < y.lower() || x.lower() > y.upper();
}

template<class T, class Policies, class T2> inline
bool operator!=(const interval<T, Policies>& x, const T2& y)
{
   return !(x == y);
}

template<class T, class Policies, class T2> inline
bool operator!=(const T2& x, const interval<T, Policies>& y)
{
   return !(x == y);
}

}

}}

//
// Special functions that are specialised/overloaded for interval
// types come last, after we've included whatever headers they need:
//
#include <boost/math/special_functions/fpclassify.hpp>
#include <boost/math/tools/roots.hpp>

namespace boost{
namespace math{

template <class T, class Policy>
inline int fpclassify BOOST_NO_MACRO_EXPAND(boost::numeric::interval<T,Policy> x)
{
   int l = boost::math::fpclassify(x.lower());
   int u = boost::math::fpclassify(x.upper());
   if(l == u)
      return l;
   //
   // The two values are different, have a hierarchy of meanings:
   //
   if((l == FP_NAN) || (u == FP_NAN))
      return FP_NAN;
   if((l == FP_INFINITE) || (u == FP_INFINITE))
      return FP_INFINITE;
   if((l == FP_SUBNORMAL) || (u == FP_SUBNORMAL))
      return FP_SUBNORMAL;
   //
   // Since l and u are different, one must be normal the other zero 
   // as that's all that's left:
   //
   return FP_NORMAL;
}

namespace tools{

namespace detail{

template <class F, class R>
struct upper_bound_functor2
{
   upper_bound_functor2(F f_) : f(f_){}

   template <class U>
   std::tr1::tuple<U, U> operator()(const U& u)
   {
      R r = f(u);
      return std::tr1::make_tuple(
         std::tr1::get<0>(r).upper(),
         std::tr1::get<1>(r).upper());
   }
private:
   F f;
};

template <class F, class R>
struct lower_bound_functor2
{
   lower_bound_functor2(F f_) : f(f_){}

   template <class U>
   std::tr1::tuple<U, U> operator()(const U& u)
   {
      R r = f(u);
      return std::tr1::make_tuple(
         std::tr1::get<0>(r).lower(),
         std::tr1::get<1>(r).lower());
   }
private:
   F f;
};

template <class F, class R>
struct upper_bound_functor3
{
   upper_bound_functor3(F f_) : f(f_){}

   template <class U>
   std::tr1::tuple<U, U, U> operator()(const U& u)
   {
      R r = f(u);
      return std::tr1::make_tuple(
         std::tr1::get<0>(r).upper(),
         std::tr1::get<1>(r).upper(),
         std::tr1::get<2>(r).upper());
   }
private:
   F f;
};

template <class F, class R>
struct lower_bound_functor3
{
   lower_bound_functor3(F f_) : f(f_){}

   template <class U>
   std::tr1::tuple<U, U, U> operator()(const U& u)
   {
      R r = f(u);
      return std::tr1::make_tuple(
         std::tr1::get<0>(r).lower(),
         std::tr1::get<1>(r).lower(),
         std::tr1::get<2>(r).lower());
   }
private:
   F f;
};

}

template <class F, class T, class Policy>
boost::numeric::interval<T, Policy> halley_iterate(
    F f, 
    boost::numeric::interval<T, Policy> guess, 
    boost::numeric::interval<T, Policy> vmin, 
    boost::numeric::interval<T, Policy> vmax, 
    int digits, 
    boost::uintmax_t& max_iter)
{
   typedef std::tr1::tuple<
      boost::numeric::interval<T, Policy>,
      boost::numeric::interval<T, Policy>,
      boost::numeric::interval<T, Policy> > f_result_type;
   typedef boost::numeric::interval<T, Policy> policy_type;
   typedef boost::numeric::interval<T, Policy> interval_type;

   boost::uintmax_t miter = max_iter;

   T a = halley_iterate(
      detail::lower_bound_functor3<F, f_result_type>(f),
      median(guess),
      lower(vmin),
      upper(vmax),
      digits,
      miter);
   max_iter -= miter;
   miter = max_iter;
   T b = halley_iterate(
      detail::upper_bound_functor3<F, f_result_type>(f),
      median(guess),
      lower(vmin),
      upper(vmax),
      digits,
      miter);
   max_iter -= miter;

   return a < b ? interval_type(a, b) : interval_type(b, a);
}

template <class F, class T, class Policy>
boost::numeric::interval<T, Policy> schroeder_iterate(
    F f, 
    boost::numeric::interval<T, Policy> guess, 
    boost::numeric::interval<T, Policy> vmin, 
    boost::numeric::interval<T, Policy> vmax, 
    int digits, 
    boost::uintmax_t& max_iter)
{
   typedef std::tr1::tuple<
      boost::numeric::interval<T, Policy>,
      boost::numeric::interval<T, Policy>,
      boost::numeric::interval<T, Policy> > f_result_type;
   typedef boost::numeric::interval<T, Policy> policy_type;
   typedef boost::numeric::interval<T, Policy> interval_type;

   boost::uintmax_t miter = max_iter;

   T a = schroeder_iterate(
      detail::lower_bound_functor3<F, f_result_type>(f),
      median(guess),
      lower(vmin),
      upper(vmax),
      digits,
      miter);
   max_iter -= miter;
   miter = max_iter;
   T b = schroeder_iterate(
      detail::upper_bound_functor3<F, f_result_type>(f),
      median(guess),
      lower(vmin),
      upper(vmax),
      digits,
      miter);
   max_iter -= miter;

   return a < b ? interval_type(a, b) : interval_type(b, a);
}

template <class F, class T, class Policy>
boost::numeric::interval<T, Policy> newton_raphson_iterate(
    F f, 
    boost::numeric::interval<T, Policy> guess, 
    boost::numeric::interval<T, Policy> vmin, 
    boost::numeric::interval<T, Policy> vmax, 
    int digits, 
    boost::uintmax_t& max_iter)
{
   typedef std::tr1::tuple<
      boost::numeric::interval<T, Policy>,
      boost::numeric::interval<T, Policy> > f_result_type;
   typedef boost::numeric::interval<T, Policy> policy_type;
   typedef boost::numeric::interval<T, Policy> interval_type;

   boost::uintmax_t miter = max_iter;

   T a = newton_raphson_iterate(
      detail::lower_bound_functor2<F, f_result_type>(f),
      median(guess),
      lower(vmin),
      upper(vmax),
      digits,
      miter);
   max_iter -= miter;
   miter = max_iter;
   T b = newton_raphson_iterate(
      detail::upper_bound_functor2<F, f_result_type>(f),
      median(guess),
      lower(vmin),
      upper(vmax),
      digits,
      miter);
   max_iter -= miter;

   return a < b ? interval_type(a, b) : interval_type(b, a);
}

}

}

} // namespace boost

#endif // BOOST_MATH_INTERVAL_BINDINGS_HPP
