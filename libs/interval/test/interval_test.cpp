/* boost interval_test.cpp interval test program
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

#include <boost/interval.hpp>
#include <boost/interval/io.hpp>
#include <iostream>
#include <iomanip>
#include <functional>
#include <string>
#include <algorithm>

using namespace boost;
using namespace interval_lib;

typedef interval<double, interval_traits<double,
					 compare_certainly<double>,
					 save_state<rounded_transc_opp<double> >,
					 checking_lax<double> > > R;

unsigned nb_errors;
std::string test_name;

struct test {
  static void init(const std::string& n) {
    test_name = n;
    nb_errors = 0;
  }

  test(const std::string& n, const R& r, double s) {
    init(n);
    std::cout << "Testing " << n << " on " << r << " step " << s << std::endl;
  }

  test(const std::string& n, const R& r, double s,
       double l, double h, double s2) {
    init(n);
    std::cout << "Testing " << n << " on " << r << " step " << s
	      << " x scalar(" << l << ".." << h << ") step " << s2
	      << std::endl;
  }

  test(const std::string& n, const R& r1, const R& r2, double s) {
    init(n);
    std::cout << "Testing " << n << " on " << r1 << " x " << r2
	      << " step " << s << std::endl;
  }

  template<class T1>
  static void error(double i_d, double o_d, const T1& i_r, const R& o_r) {
    if (nb_errors++ > 0) return;
    std::cerr << test_name << " " << i_d << " = " << o_d << " not in "
	      << test_name << " " << i_r << " = " << o_r << std::endl;
  }

  template<class T1, class T2>
  static void error(double i_d1, double i_d2, double o_d,
		    const T1& i_r1, const T2& i_r2, const R& o_r) {
    if (nb_errors++ > 0) return;
    std::cerr << i_d1 << " " << test_name << " " << i_d2 << " = " << o_d
	      << " not in "
	      << i_r1 << " " << test_name << " " << i_r2 << " = " << o_r
	      << std::endl;
  }

  template<class T1, class T2, class T3>
  static void error(double i_d1, double i_d2, double i_d3, double o_d,
		    const T1& i_r1, const T2& i_r2, const T3& i_r3,
		    const R& o_r) {
    if (nb_errors++ > 0) return;
    std::cerr << test_name << "(" << i_d1 << ", " << i_d2 << ", " << i_d3
	      << ") = " << o_d << " not in "
	      << test_name << "(" << i_r1 << ", " << i_r2 << ", " << i_r3
	      << ") = " << o_r << std::endl;
  }

  ~test() {
    if (nb_errors <= 1) return;
    std::cerr << test_name << ": " << nb_errors << " errors" << std::endl;
  }
};

template<class UnaryFunction>
void iterate_one_interval(UnaryFunction func, const std::string & msg,
			  double low = -10, double high = 10,
			  double step = 0.1)
{
  R rn(low, high);
  test t(msg, rn, step);
  try {
    for(double l = low; l <= high; l += step)
      for(double u = l+step/10; u <= high; u += step) {
	R r(l, u);
	func(r);
      }
  } catch(std::exception & ex) {
    std::cerr << "Exception with " << msg << ": " << ex.what() << std::endl;
  }
}

template<class Scalar, class BinaryFunction>
void iterate_one_interval_one_scalar(BinaryFunction func,
				     const std::string & msg,
				     double low = -10, double high = 10,
				     double step = 0.1,
				     Scalar low2 = -10, Scalar high2 = 10,
				     Scalar step2 = 1)
{
  R rn(low, high);
  test t(msg, rn, step, low2, high2, step2);
  try {
    for(double l = low; l <= high; l += step)
      for(double u = l+step/10; u <= high; u += step)
	for(Scalar x = low2; x <= high2; x += step2) {
	  R r(l, u);
	  func(r, x);
	}
  } catch(std::exception & ex) {
    std::cerr << "Exception with " << msg << ": " << ex.what() << std::endl;
  }
}

template<class BinaryFunction>
void iterate_two_intervals(BinaryFunction func, const std::string & msg,
			   double low = -2, double high = 2, double step = 0.3)
{
  R rn(low, high);
  test t(msg, rn, rn, step);
  try {
    for(double l1 = low; l1 <= high; l1 += step)
      for(double u1 = l1+step/10; u1 <= high; u1 += step)
	for(double l2 = low; l2 <= high; l2 += step)
	  for(double u2 = l2+step/10; u2 <= high; u2 += step) {
	    R r1(l1, u1), r2(l2, u2);
	    func(r1, r2);
	  }
  } catch(std::exception & ex) {
    std::cerr << "Exception with " << msg << ": " << ex.what() << std::endl;
  }
}

template<class FuncI, class FuncD>
struct test_binary_function : std::binary_function<R, R, void>
{
  test_binary_function(FuncI fi, FuncD fd, const std::string & name) 
    : funcr(fi), funcd(fd), name(name) { }
  void operator()(const R& r1, const R& r2)
  {
    R result = funcr(r1,r2);
    for(double f = r1.lower(); f <= r1.upper(); f += width(r1) / 10.0)
      for(double g = r2.lower(); g <= r2.upper(); g += width(r2) / 10.0) {
	double presult = funcd(f,g);
	if (presult == presult /* NaN check */ && !in(presult, result))
	  test::error(f, g, presult, r1, r2, result);
      }
  }
private:
  FuncI funcr;
  FuncD funcd;
  std::string name;
};

template<class FuncR, class FuncD>
void check_binary(FuncR funcr, FuncD funcd, const std::string & s)
{
  iterate_two_intervals(test_binary_function<FuncR, FuncD>(funcr, funcd, s), s);
}

template<class FuncI, class FuncD>
struct test_binary_scalar_function
  : std::binary_function<R, typename FuncI::second_argument_type, void>
{
  test_binary_scalar_function(FuncI fi, FuncD fd, const std::string & name) 
    : funcr(fi), funcd(fd), name(name) { }
  void operator()(const R& r, typename FuncI::second_argument_type x)
  {
    R result = funcr(r, x);
    for(double f = r.lower(); f <= r.upper(); f += width(r) / 10.0) {
      double presult = funcd(f, x);
      if (!empty(result) && !in(presult, result))
	test::error(f, x, presult, r, x, result);
    }
  }
private:
  FuncI funcr;
  FuncD funcd;
  std::string name;
};

template<class FuncR, class FuncD>
void check_binary_scalar(FuncR funcr, FuncD funcd, const std::string & name)
{
  iterate_one_interval_one_scalar<typename FuncR::second_argument_type>
    (test_binary_scalar_function<FuncR, FuncD>(funcr, funcd, name), name,
     -5, 5, 0.17, -5, 5, 0.17);
}

// this is only require for pow_int
template<class FuncR, class FuncD>
void check_binary_int(FuncR funcr, FuncD funcd, const std::string & name)
{
  iterate_one_interval_one_scalar<int>
    (test_binary_scalar_function<FuncR, FuncD>(funcr, funcd, name), name,
     -10, 10, 0.1, -10, 10, 1);
}

template<class FuncI, class FuncD>
struct test_scalar_binary_function
  : std::binary_function<typename FuncI::first_argument_type, R, void>
{
  test_scalar_binary_function(FuncI fi, FuncD fd, const std::string & name) 
    : funcr(fi), funcd(fd), name(name) { }
  void operator()(const R& r, typename FuncI::first_argument_type x)
  {
    R result = funcr(x, r);
    for(double f = r.lower(); f <= r.upper(); f += width(r) / 10.0) {
      double presult = funcd(x, f);
      if(!empty(result) && !in(funcd(x, f), result))
	test::error(x, f, presult, x, r, result);
    }
  }
private:
  FuncI funcr;
  FuncD funcd;
  std::string name;
};

template<class FuncR, class FuncD>
void check_scalar_binary(FuncR funcr, FuncD funcd, const std::string & name)
{
  iterate_one_interval_one_scalar<typename FuncR::first_argument_type>
    (test_scalar_binary_function<FuncR, FuncD>(funcr, funcd, name),
     name + " (arg rev)", -5, 5, 0.17, -5, 5, 0.17);
}

template<class A1, class A2, class R>
struct my_plus : std::binary_function<A1,A2,R>
{ 
  R operator()(A1 x, A2 y) { return x+y; }
};
template<class A1, class A2, class R>
struct my_minus : std::binary_function<A1,A2,R>
{
  R operator()(A1 x, A2 y) { return x-y; }
};

template<class A1, class A2, class R>
struct my_multiplies : std::binary_function<A1,A2,R>
{
  R operator()(A1 x, A2 y) { return x*y; }
};

template<class A1, class A2, class R>
struct my_divides : std::binary_function<A1,A2,R>
{
  R operator()(A1 x, A2 y) { return x/y; }
};

void runtest_binary_functions()
{
  // interval * interval
  check_binary(std::plus<R>(), std::plus<double>(), "+");
  check_binary(std::minus<R>(), std::minus<double>(), "-");
  check_binary(std::multiplies<R>(), std::multiplies<double>(), "*");
  check_binary(std::divides<R>(), std::divides<double>(), "/");

  // interval * scalar
  check_binary_scalar(my_plus<R, double, R>(), std::plus<double>(), "+");
  check_binary_scalar(my_minus<R, double, R>(), std::minus<double>(), "-");
  check_binary_scalar(my_multiplies<R, double, R>(),
		      std::multiplies<double>(), "*");
  check_binary_scalar(my_divides<R, double, R>(), std::divides<double>(), "/");

  // scalar * interval
  check_scalar_binary(my_plus<double, R, R>(), std::plus<double>(), "+");
  check_scalar_binary(my_minus<double, R, R>(), std::minus<double>(), "-");
  check_scalar_binary(my_multiplies<double, R, R>(),
		      std::multiplies<double>(), "*");
  check_scalar_binary(my_divides<double, R, R>(), std::divides<double>(), "/");

#if 0
  // misc.
  check_binary(std::ptr_fun( (R (*)(const R&, const R&)) boost::pow),
	       std::ptr_fun( (double(*)(double,double))std::pow), "pow");
  check_binary_int(std::ptr_fun( (R (*)(R, long)) boost::pow),
		   std::ptr_fun( (double(*)(double,double))std::pow), "pow_int");
  check_binary(std::ptr_fun( (R (*)(const R&, const R&)) boost::atan2),
		  std::ptr_fun((double(*)(double,double))std::atan2), "atan2");
#endif
  // Note: fmod is not here, because it does not fulfill the operator
  // definition, neither are the relational operators
}

template<class FuncR, class FuncD>
struct test_unary_function : std::unary_function<R, void>
{
  test_unary_function(FuncR fr, FuncD fd, const std::string & s)
    : funcr(fr), funcd(fd), name(s) { }
  void operator()(const R& r)
  {
    R result = funcr(r);
    for(double f = r.lower(); f <= r.upper(); f += width(r) / 10.0) {
      double presult = funcd(f);
      if (presult == presult /* NaN check */ && !in(presult, result))
	test::error(f, presult, r, result);
    }
  }
private:
  FuncR funcr;
  FuncD funcd;
  std::string name;
};

template<class FuncR, class FuncD>
void check_unary(FuncR rfunc, FuncD dfunc, const std::string& func,
		 double low = -10, double high = 10, double step = 0.1)
{
  iterate_one_interval(test_unary_function<FuncR, FuncD>(rfunc, dfunc, func),
		       func, low, high, step);
}

template<class T> T my_square(T x)
{
  return x * x;
}


void runtest_unary_functions()
{
  check_unary(std::negate<R>(), std::negate<double>(), "-");
  check_unary(std::ptr_fun( (R (*)(const R &)) boost::abs),
	      std::ptr_fun( (double(*)(double)) std::fabs), "abs");
  check_unary(std::ptr_fun( (R (*)(const R &)) boost::square),
	      std::ptr_fun( (double (*)(double)) my_square), "square");
  check_unary(std::ptr_fun( (R (*)(const R &)) boost::sqrt),
	      std::ptr_fun( (double(*)(double)) std::sqrt), "sqrt", -1, 10);
  check_unary(std::ptr_fun( (R (*)(const R &)) boost::exp),
	      std::ptr_fun( (double(*)(double)) std::exp), "exp");
  check_unary(std::ptr_fun( (R (*)(const R &)) boost::log),
	      std::ptr_fun( (double(*)(double)) std::log), "log", -1, 10);
#if 0
  check_unary(std::ptr_fun( (R (*)(const R &)) boost::log10),
	      std::ptr_fun( (double(*)(double)) std::log10), "log10", -1, 10);
#endif

  // trigonometric functions
  check_unary(std::ptr_fun( (R (*)(const R &)) boost::sin),
	      std::ptr_fun( (double(*)(double)) std::sin), "sin");
  check_unary(std::ptr_fun( (R (*)(const R &)) boost::cos),
	      std::ptr_fun( (double(*)(double)) std::cos), "cos");
  check_unary(std::ptr_fun( (R (*)(const R &)) boost::tan),
	      std::ptr_fun( (double(*)(double)) std::tan), "tan");
  check_unary(std::ptr_fun( (R (*)(const R &)) boost::asin),
	      std::ptr_fun( (double(*)(double)) std::asin), "asin");
  check_unary(std::ptr_fun( (R (*)(const R &)) boost::acos),
	      std::ptr_fun( (double(*)(double)) std::acos), "acos");
  check_unary(std::ptr_fun( (R (*)(const R &)) boost::atan),
	      std::ptr_fun( (double(*)(double)) std::atan), "atan");

  // hyperbolic functions
  check_unary(std::ptr_fun( (R (*)(const R &)) boost::sinh),
	      std::ptr_fun((double(*)(double))std::sinh), "sinh", -3, 3, 0.17);
  check_unary(std::ptr_fun( (R (*)(const R &)) boost::cosh),
	      std::ptr_fun((double(*)(double))std::cosh), "cosh", -3, 3, 0.17);
  check_unary(std::ptr_fun( (R (*)(const R &)) boost::tanh),
	      std::ptr_fun((double(*)(double))std::tanh), "tanh", -3, 3, 0.17);
#ifdef BOOST_HAVE_INV_HYPERBOLIC
  check_unary(std::ptr_fun( (R (*)(const R &)) boost::asinh),
	      std::ptr_fun((double(*)(double))std::asinh), "asinh", -3, 3, 0.17);
  check_unary(std::ptr_fun( (R (*)(const R &)) boost::acosh),
	      std::ptr_fun((double(*)(double))std::acosh), "acosh", -3, 3, 0.17);
  check_unary(std::ptr_fun( (R (*)(const R &)) boost::atanh),
	      std::ptr_fun((double(*)(double))std::atanh), "atanh", -3, 3, 0.17);
#else
  std::cout << "Skipping inverse hyperbolic functions\n";
#endif // BOOST_HAVE_INV_HYPERBOLIC
}

template<class FuncR, class FuncD>
struct test_compare_certain
{
  test_compare_certain(FuncR fr, FuncD fd, const std::string& func)
    : funcr(fr), funcd(fd), name(func) { }
  void operator()(const R& r1, const R& r2)
  {
    bool result = funcr(r1,r2);
    if (!result) return;
    for(double f = r1.lower(); f <= r1.upper(); f += width(r1)/10.0)
      for(double g = r2.lower(); g <= r2.upper(); g += width(r2)/10.0) {
	bool presult = funcd(f, g);
	if (!presult) test::error(f, g, presult, r1, r2, result);
      }
  }
private:
  FuncR funcr;
  FuncD funcd;
  std::string name;
};

template<class FuncR, class FuncD>
struct test_compare_possible
{
  test_compare_possible(FuncR fr, FuncD fd, const std::string& func)
    : funcr(fr), funcd(fd), name(func) { }
  void operator()(const R& r1, const R& r2)
  {
    bool result = funcr(r1,r2);
    if (result) return;
    for(double f = r1.lower(); f <= r1.upper(); f += width(r1)/10.0)
      for(double g = r2.lower(); g <= r2.upper(); g += width(r2)/10.0) {
	bool presult = funcd(f, g);
	if (presult) test::error(f, g, presult, r1, r2, result);
      }
  }
private:
  FuncR funcr;
  FuncD funcd;
  std::string name;
};

template<class FuncR, class FuncD>
void check_compare_certain(FuncR funcr, FuncD funcd, const std::string& func)
{
  iterate_two_intervals
    (test_compare_certain<FuncR, FuncD>(funcr, funcd, func), func);
}

template<class FuncR, class FuncD>
void check_compare_possible(FuncR funcr, FuncD funcd, const std::string& func)
{
  iterate_two_intervals
    (test_compare_possible<FuncR, FuncD>(funcr, funcd, func), func);
}

void runtest_compare()
{
  check_compare_certain(std::less<R>(), std::less<double>(), "<");
  check_compare_certain(std::greater<R>(), std::greater<double>(), ">");
  check_compare_possible(std::less_equal<R>(),
			 std::less_equal<double>(), "<=");
  check_compare_possible(std::greater_equal<R>(),
			 std::greater_equal<double>(), ">=");
  check_compare_certain(std::equal_to<R>(), std::equal_to<double>(), "==");
  check_compare_possible(std::not_equal_to<R>(),
			 std::not_equal_to<double>(), "!=");
}

void test_bisect_median(const R& r)
{
  std::pair<R, R> bis = bisect(r);
  if(!upper(bis.first) == median(r))
    std::cerr << "bisect(" << r << ").first does not have median "
	      << median(r) << " as upper bound" << std::endl;
  if(!lower(bis.second) == median(r))
    std::cerr << "bisect(" << r << ").second does not have median "
	      << median(r) << " as lower bound" << std::endl;
}

void test_overlap(const R& r1, const R& r2)
{
  if(!overlap(r1,r2))
    return;
  double inc = std::min(width(r1), width(r2))/100.0;
  for(double f = r1.lower(); f <= r1.upper(); f+=inc)
    if(in(f, r1) && in(f, r2))
      return;
  std::cerr << "No overlap found for " << r1 << " and " << r2 << std::endl;
}

#if 0
void test_combine(const R& r1, const R& r2)
{
  if (!overlap(r1, r2))
    return;
  R comb = combine(r1, r2);
  for(double f = std::min(r1.lower(), r2.lower());
      f <= std::max(r1.upper(), r2.upper());
      f += std::min(width(r1), width(r2))/100.0) {
    if (!in(f, comb))
      std::cerr << "combine(" << r1 << ", " << r2 << ") = " 
		<< comb << " does not contain "
		<< f << std::endl;
  }
}
#endif

void test_intersect(const R& r1, const R& r2)
{
  if (!overlap(r1, r2))
    return;
  R is = intersect(r1, r2);
  for(double f = std::min(r1.lower(), r2.lower());
      f <= std::max(r1.upper(), r2.upper());
      f += std::min(width(r1), width(r2))/100.0) {
    if (in(f, r1) && in(f, r2) && !in(f, is))
      std::cerr << "intersect(" << r1 << ", " << r2 << ") = "
		<< is << " does not contain "
		<< f << std::endl;
  }
}

void test_hull(const R& r1, const R& r2)
{
  R h = hull(r1, r2);
  for(double f = std::min(r1.lower(), r2.lower());
      f <= std::max(r1.upper(), r2.upper());
      f += width(h)/100)
    if (!in(f, h))
      std::cerr << "hull(" << r1 << ", " << r2 << ") = "
		<< h << " does not contain "
		<< f << std::endl;
}

#if 0
void test_scale(const R& r, double factor)
{
  for(double m = r.lower(); m <= r.upper(); m += width(r)/10) {
    R res = scale(r, m, factor);
    for(double f = r.lower(); f < r.upper(); f += width(r)/10) {
      double pres = (f - m) * factor;
      if (!in(pres, res))
	test::error(f, m, factor, pres, r, m, factor, res);
    }
  }
}
#endif

int main()
{
  // check for proper NaN and infinity values to avoid excess output
  double inf = std::numeric_limits<double>::infinity();
  std::cout << "Infinity: " << inf << "\n";
  double nan = std::numeric_limits<double>::quiet_NaN();
  std::cout << "NaN: " << nan << "\n";
  R whole = R::whole();
  std::cout << "[-inf, inf]: " << whole << "\n";
  if(inf < 10 || nan <= 10 || nan >= 10 || whole.lower() != -inf
     || whole.upper() != inf) {
    std::cerr << "(terminated)\n";
    exit(1);
  }

  // verify that image intervals are correctly computed for all functions
  // i.e. for all x in I: f(x) in f(I)
  runtest_binary_functions();
  runtest_unary_functions();
  runtest_compare();
  iterate_one_interval(test_bisect_median, "bisect & median");
  iterate_two_intervals(test_overlap, "overlap");
  // iterate_two_intervals(test_combine, "combine");
  iterate_two_intervals(test_intersect, "intersect");
  iterate_two_intervals(test_hull, "hull");
#if 0
  iterate_one_interval_one_scalar<double>(test_scale,
		  // Borland C++ requires this explicit conversion
					  std::string("scale"),
					  -5, 5, 0.17, -5, 5, 0.17);
#endif

  return 0;
}
