/* boost interval_speed.cpp performance measurements
 *
 * Copyright Jens Maurer 2000
 * Permission to use, copy, modify, sell, and distribute this software
 * is hereby granted without free provided that the above copyright notice
 * appears in all copies and that both that copyright notice and this
 * permission notice appear in supporting documentation,
 *
 * Jens Maurer makes no representations about the suitability of this
 * software for any purpose. It is provided "as is" without express or
 * implied warranty.
 *
 * $Id$
 */

// allow for different interval implementations
// must be first, because <boost/interval.hpp> needs <limits> workaround for GCC
#include "interval_implementation_select.hpp"

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <cmath>
#include <boost/progress.hpp>
#include <boost/random.hpp>



/*
 * This program measures the speed of any given C++ interval
 * arithmetics package. It does not check correctness, so do validate
 * the compiler output before running it.
 *
 * It initially populates one or two vectors with random data.  Then,
 * it executes an arithmetic operation on each element of the
 * vector(s).  It measures how many sweeps it can do on the vector(s)
 * in about one second (auto-tuning).  From that, it calculates the
 * output figure "million operations per second".
 */

static void show_result(const std::string & s, int iter, double t)
{
  std::cout.precision(2);
  std::cout << s << " "
	    << (iter/t*1e-6) << " M ops/sec (@ "
	    << iter << " iter / " 
	    << t << " sec)"
	    << std::endl;
}

/*
 * Generate random data items for the vector initialization.
 */

static boost::mt19937 base;
static boost::uniform_real<boost::mt19937> rng(base, -2, 2);

template<class T> T random_element();

#ifdef USE_POINT
template<>
double random_element<double>()
{
  return rng();
}
#else
template<>
interval_type random_element<interval_type>()
{
  // return boost::hull(rng(), rng());
  // emulate hull for foreign interval packages
  double a = rng(), b = rng();
  if(a < b)
    return interval_type(a, b);
  else
    return interval_type(b, a);
}
#endif /* USE_POINT */


/*
 * Helper definitions for loop unrolling. You can use these in
 * interval_implementation_select.hpp to specify an adequate amount
 * of loop unrolling.
 */

#define UNROLL2(x) x; x
#define UNROLL4(x) UNROLL2(x); UNROLL2(x)
#define UNROLL8(x) UNROLL4(x); UNROLL4(x)


/*
 * Timing functions.  The do_timing*() functions are the measurement
 * kernel, the mops*() functions do the auto-tuning.
 */

template<class UnaryFunction, class ForwardIterator, 
  class ConstForwardIterator>
double do_timing1(UnaryFunction func, int iter,
		 ConstForwardIterator v1_first,
		 ConstForwardIterator v1_last,
		 ForwardIterator v2_first)
{
  boost::timer t;
#ifdef USE_BOOST_UNPROTECTED
  protected_rounding rnd;
#endif
  for(int k = 0; k < iter; ++k) {
    ConstForwardIterator i1 = v1_first;
    ForwardIterator i2 = v2_first;
    for(; i1 < v1_last; ) {
      UNROLL(*i2 = func(*i1); ++i1; ++i2);
    }
  }
#ifdef USE_BOOST_UNPROTECTED
  (void)&rnd;
#endif
  return t.elapsed();
}

template<class UnaryFunction>
void mops1(UnaryFunction func, unsigned int vsize,
	  const std::string & s)
{
  typedef typename UnaryFunction::result_type base_type;
  unsigned iter = 5;
  while(iter < 2*1000*1000*1000/vsize) {
    std::vector<base_type> v1(vsize), v2(vsize);
    for(unsigned int i = 0; i < vsize; ++i)
      v1[i] = random_element<base_type>();

    double ti = do_timing1(func, iter, v1.begin(), v1.end(), v2.begin());
    if(ti > 0.6) {
      show_result(s, iter * vsize, ti);
      return;
    }
    if(ti <= 0.01 && iter < 40*1000*1000)
      iter *= 50;      // jump to more appropriate iter values
    else
      iter *= std::max(2, static_cast<int>(0.5/ti));
  }
  std::cout << "not enough time reached for " << s 
	    << " at iter = " << iter << std::endl;
}

template<class BinaryFunction, class ForwardIterator,
  class ConstForwardIterator>
double do_timing2a(BinaryFunction func, int iter,
		  ForwardIterator v1_first,
		  ForwardIterator v1_last,
		  ConstForwardIterator v2_first)
{
  boost::timer t;
#ifdef USE_BOOST_UNPROTECTED
  protected_rounding rnd;
#endif
  for(int k = 0; k < iter; ++k) {
    ForwardIterator i1 = v1_first;
    ConstForwardIterator i2 = v2_first;
    for(; i1 < v1_last; ) {
      UNROLL4(func(*i1, *i2); ++i1; ++i2);
    }
  }
#ifdef USE_BOOST_UNPROTECTED
  (void)&rnd;
#endif
  return t.elapsed();
}

template<class BinaryFunction>
void mops2a(BinaryFunction func, unsigned int vsize,
	   const std::string & s)
{
  unsigned int iter = 5;
  while(iter < 2*1000*1000*1000/vsize) {
    std::vector<interval_type> v1(vsize), v2(vsize);
    for(unsigned int i = 0; i < vsize; ++i) {
      v1[i] = random_element<interval_type>();
      v2[i] = random_element<interval_type>();
    }

    double ti = do_timing2a(func, iter, v1.begin(), v1.end(),
			    std::vector<interval_type>::const_iterator(v2.begin()));
    if(ti > 0.6) {
      show_result(s, iter * vsize, ti);
      return;
    }
    if(ti <= 0.01 && iter < 40*1000*1000)
      iter *= 50;      // jump to more appropriate iter values
    else
      iter *= std::max(2, static_cast<int>(0.5/ti));
  }
  std::cout << "not enough time reached for " << s 
	    << " at iter = " << iter << std::endl;
}

template<class FuncR, class ForwardIterator, class ConstForwardIterator>
double do_timing2(FuncR funcr, int iter, ConstForwardIterator v1_first,
		 ConstForwardIterator v1_last,
		 ConstForwardIterator v2_first,
		 ForwardIterator v3_first)
{
  boost::timer t;
#ifdef USE_BOOST_UNPROTECTED
  protected_rounding rnd;
#endif
  for(int k = 0; k < iter; ++k) {
    ConstForwardIterator i1 = v1_first, i2 = v2_first;
    ForwardIterator i3 = v3_first;
    for(; i1 < v1_last; ) {
      UNROLL(*i3 = funcr(*i1, *i2); ++i1; ++i2; ++i3);
    }
  }
#ifdef USE_BOOST_UNPROTECTED
  (void)&rnd;
#endif
  double tmp = t.elapsed();
  for(int i = 0; i < (v1_last-v1_first); ++i) {
    volatile typename FuncR::result_type x = *(v3_first+i);
    (void) &x;
  }
  return tmp;
}

template<class BinaryFunction>
void mops2(BinaryFunction func, unsigned int vsize, const std::string & s)
{
  typedef typename BinaryFunction::result_type base_type;
  unsigned int iter = 5;
  while(iter < 100000000/vsize) {
    std::vector<base_type> v1(vsize), v2(vsize), v3(vsize);
    for(unsigned int i = 0; i < vsize; ++i) {
      v1[i] = random_element<base_type>();
      v2[i] = random_element<base_type>();
    }
    double ti = do_timing2(func, iter, v1.begin(), v1.end(),
			   v2.begin(), v3.begin());
    if(ti > 0.6) {
      show_result(s, iter * vsize, ti);
      return;
    }
    if(ti <= 0.01)
      iter *= 50;      // jump to more appropriate iter values
    else
      iter *= std::max(2, static_cast<int>(1.0/ti));
  }
  std::cout << "not enough time reached for " << s 
	    << " at iter = " << iter << std::endl;
}


/*
 * Assignment variants of std::plus<> etc.
 */

template<class T> struct plus_assign
{ void operator()(T & x, const T& y) { x += y; } };

template<class T> struct minus_assign
{ void operator()(T & x, const T& y) { x -= y; } };

template<class T> struct multiplies_assign
{ void operator()(T & x, const T& y) { x *= y; } };

template<class T> struct divides_assign
{ void operator()(T & x, const T& y) { x /= y; } };


/*
 * The basic arithmetic functions: + - * / and their assignment
 * variants.  Note that the assignment variants modify the random
 * vector in a somewhat recursive manner.  *= and /= have poor
 * performance because the vectors often converge to values which are
 * at the end of a chain of comparisons in *= and /=.
 */

template<class T>
void base_arith_test(unsigned int vsize, const std::string & s)
{
  mops2(std::plus<T>(), vsize, s + " binary +");
  mops2(std::minus<T>(), vsize, s + " binary -");
  mops2(std::multiplies<T>(), vsize, s + " binary *");
  mops2(std::divides<T>(), vsize, s + " binary /");

  mops2a(plus_assign<T>(), vsize, s + " +=");
  mops2a(minus_assign<T>(), vsize, s + " -=");
  mops2a(multiplies_assign<T>(), vsize, s + " *=");
  mops2a(divides_assign<T>(), vsize, s + " /=");
}


/*
 * Define a few function objects.  We cannot use std::ptr_fun,
 * because we need to specify the exact types to disambiguate
 * between several overloaded functions and, depending on
 * interval_type, these functions either take values (double)
 * or const references (intervals).  Thus, we avoid special cases
 * for double all over and do it only once here.  This might
 * profit from the boost type_traits header.
 */

template<class T>
class math_unary_func : public std::unary_function<T,T>
{
public:
  math_unary_func(T (*f)(T)) : _f(f) { }
  T operator()(T x) { return _f(x); }
private:
  T (*_f)(T);
};

#ifndef USE_POINT
template<>
class math_unary_func<interval_type>
  : public std::unary_function<const interval_type &, interval_type>
{
public:
  math_unary_func(interval_type (*f)(const interval_type &)) : _f(f) { }
  interval_type operator()(const interval_type & x) { return _f(x); }
private:
  interval_type (*_f)(const interval_type &);
};
#endif

template<class T> class math_binary_func
  : public std::binary_function<T,T,T>
{
public:
  math_binary_func(T (*f)(T,T)) : _f(f) { }
  T operator()(T x, T y) { return _f(x, y); }
private:
  T (*_f)(T,T);
};

#ifndef USE_POINT
template<>
class math_binary_func<interval_type>
  : public std::binary_function<const interval_type &, const interval_type &,
                                interval_type>
{
public:
  math_binary_func(interval_type (*f)(const interval_type &,
				     const interval_type &)) 
    : _f(f) { }
  interval_type operator()(const interval_type & x, const interval_type & y)
  { return _f(x,y); }
private:
  interval_type (*_f)(const interval_type &, const interval_type &);
};
#endif

// SunPRO has problems with the direct solution (is it right ?).

#ifdef __SUNPRO_CC
namespace {
interval_type  abs(const interval_type &t) { return boost::abs(t); }
interval_type  exp(const interval_type &t) { return boost::exp(t); }
interval_type  log(const interval_type &t) { return boost::log(t); }
interval_type  sin(const interval_type &t) { return boost::sin(t); }
interval_type  cos(const interval_type &t) { return boost::cos(t); }
interval_type  tan(const interval_type &t) { return boost::tan(t); }
interval_type sqrt(const interval_type &t) { return boost::sqrt(t); }
interval_type asin(const interval_type &t) { return boost::asin(t); }
interval_type acos(const interval_type &t) { return boost::acos(t); }
interval_type atan(const interval_type &t) { return boost::atan(t); }
interval_type sinh(const interval_type &t) { return boost::sinh(t); }
interval_type cosh(const interval_type &t) { return boost::cosh(t); }
interval_type tanh(const interval_type &t) { return boost::tanh(t); }
interval_type fmod(const interval_type &t1,
		   const interval_type &t2) { return boost::fmod(t1, t2); }
}
#endif

/*
 * Miscellaneous arithmetic functions.
 */

template<class T>
void misc_arith_test(unsigned int vsize)
{
  mops1(std::negate<T>(), vsize, "unary -");
  mops1(math_unary_func<T>(abs), vsize, "abs");
#ifdef HAVE_FMOD
  mops2(math_binary_func<T>(fmod), 100, "fmod");
#else
  std::cout << "  (skipping fmod)\n";
#endif
}


/*
 * Transcendental functions, if available in the implementation.
 */

template<class T>
void transc_test(unsigned int vsize)
{
  mops1(math_unary_func<T>(sqrt), vsize, "sqrt");
#ifdef HAVE_TRANSCENDENTAL
#undef HAVE_ATAN2 // FIXME
#ifdef HAVE_ATAN2
  mops2(math_binary_func<T>(atan2), 100, "atan2");
#endif
  mops1(math_unary_func<T>(exp), vsize, "exp");
#ifdef HAVE_CLOSED_SEMANTICS
  mops1(math_unary_func<T>(log), vsize, "log");
#else
  std::cout << "  (skipping log)\n";
#endif
#ifdef HAVE_SINCOS
  mops1(math_unary_func<T>(sin), vsize, "sin");
  mops1(math_unary_func<T>(cos), vsize, "cos");
#else
  std::cout << "  (skipping sin)\n  (skipping cos)\n";
#endif
#ifdef HAVE_TAN
  mops1(math_unary_func<T>(tan), vsize, "tan");
#else
  std::cout << "  (skipping tan)\n";
#endif
#if defined(HAVE_CLOSED_SEMANTICS) && defined(HAVE_ASINCOS)
  mops1(math_unary_func<T>(asin), vsize, "asin");
  mops1(math_unary_func<T>(acos), vsize, "acos");
#else
  std::cout << "  (skipping asin)\n  (skipping acos)\n";
#endif
  mops1(math_unary_func<T>(atan), vsize, "atan");
#ifdef HAVE_HYPERBOLIC
  mops1(math_unary_func<T>(sinh), vsize, "sinh");
  mops1(math_unary_func<T>(cosh), vsize, "cosh");
  mops1(math_unary_func<T>(tanh), vsize, "tanh");
#if defined(BOOST_HAVE_INV_HYPERBOLIC)
  mops1(math_unary_func<T>(asinh), vsize, "asinh");
  mops1(math_unary_func<T>(acosh), vsize, "acosh");
  mops1(math_unary_func<T>(atanh), vsize, "atanh");
#else
  std::cout << "  (skipping inverse hyperbolic functions)\n";
#endif
#else
  std::cout << "  (skipping hyperbolic functions)\n";
#endif
#else
  std::cout << "  (skipping transcendental functions)\n";
#endif // HAVE_TRANSCENDENTAL
}

static void run_test(unsigned int vsize)
{
  std::cout << "Testing with vector size = " << vsize << std::endl;
  base_arith_test<interval_type>(vsize, interval_name);
}


/*
 * This shows how the size of the vector(s) affects performance for
 * the basic arithmetic operations. The output should be
 * perl-postprocessed and then fed into gnuplot for display.
 * The speed differences between L1 cache, L2 cache and main memory
 * are usually clearly visible as sharp steps in the curve.
 */
static void test_vector_size_effects()
{
  for(int i = 1; i < 1000000; i *= 2) {
    std::cout << "vector<> size: " << i << " ";
    mops2(std::multiplies<interval_type>(), i, "");
  }
}

int main()
{
  //test_vector_size_effects();
  //run_test(8);
  run_test(96);
  //run_test(1000);

  misc_arith_test<interval_type>(100);
  transc_test<interval_type>(100);
}

