//  (C) Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/math/bindings/mpfr_interval.hpp>
#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/math/special_functions.hpp>
#include <boost/math/distributions.hpp>
#include <boost/math/tools/test.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <map>

#include <boost/math/tools/test_data.hpp>

using namespace boost::math::tools;
using namespace boost::math;
using namespace std;

template <class T, class Policy>
T non_central_beta_p(T a, T b, T lam, T x, T y, const Policy& pol)
{
   BOOST_MATH_STD_USING
   using namespace boost::math;
   //
   // Variables come first:
   //
   boost::uintmax_t max_iter = policies::get_max_series_iterations<Policy>();
   T errtol = ldexp(1.0, -boost::math::policies::digits<T, Policy>());
   T l2 = lam / 2;
   //
   // k is the starting point for iteration, and is the
   // maximum of the poisson weighting term:
   //
   int k = tools::real_cast<int>(l2);
   // Starting Poisson weight:
   T pois = gamma_p_derivative(k+1, l2, pol);
   // Starting beta term:
   T beta = x < y
      ? ibeta(a + k, b, x, pol)
      : ibetac(b, a + k, y, pol);
   // recurance term:
   T xterm = x < y
      ? ibeta_derivative(a + k, b, x, pol)
      : ibeta_derivative(b, a + k, y, pol);
   xterm *= (1 - x) / (a + b + k - 1);
   T poisf(pois), betaf(beta), xtermf(xterm);
   T sum = 0;

   //
   // Backwards recursion first, this is the stable
   // direction for recursion:
   //
   int count = 0;
   for(int i = k; i >= 0; --i)
   {
      T term = beta * pois;
      sum += term;
      if(tools::maybe_less(fabs(term/sum), errtol))
         break;
      pois *= i / l2;
      beta += xterm;
      xterm *= (a + i - 1) / (x * (a + b + i - 2));
      ++count;
   }
   for(int i = k + 1; i - k < max_iter; ++i)
   {
      poisf *= l2 / i;
      xtermf *= (x * (a + b + i - 2)) / (a + i - 1);
      betaf -= xtermf;
      T term = poisf * betaf;
      sum += term;
      if(tools::maybe_less(fabs(term/sum), errtol))
         break;
      ++count;
   }
   return sum;
}

template <class T, class Policy>
T non_central_beta_q(T a, T b, T lam, T x, T y, const Policy& pol)
{
   BOOST_MATH_STD_USING
   using namespace boost::math;
   //
   // Variables come first:
   //
   boost::uintmax_t max_iter = policies::get_max_series_iterations<Policy>();
   T errtol = ldexp(1.0, -boost::math::policies::digits<T, Policy>());
   T l2 = lam / 2;
   //
   // k is the starting point for iteration, and is the
   // maximum of the poisson weighting term:
   //
   int k = tools::real_cast<int>(l2);
   // Starting Poisson weight:
   T pois = gamma_p_derivative(k+1, l2, pol);
   // Starting beta term:
   T beta = x < y 
      ? ibetac(a + k, b, x, pol) 
      : ibeta(b, a + k, y, pol);
   // Recurance term:
   T xterm = x < y 
      ? ibeta_derivative(a + k, b, x, pol)
      : ibeta_derivative(b, a + k, y, pol);
   xterm *= (1 - x) / (a + b + k - 1);
   T poisf(pois), betaf(beta), xtermf(xterm);
   T sum = 0;

   //
   // Backwards recursion first, this is the unstable
   // direction for recursion:
   //
   for(int i = k; i >= 0; --i)
   {
      T term = beta * pois;
      sum += term;
      if(tools::maybe_less(fabs(term/sum), errtol))
         break;
      pois *= i / l2;
      beta -= xterm;
      xterm *= (a + i - 1) / (x * (a + b + i - 2));
   }
   for(int i = k + 1; i - k < max_iter; ++i)
   {
      poisf *= l2 / i;
      xtermf *= (x * (a + b + i - 2)) / (a + i - 1);
      betaf += xtermf;
      T term = poisf * betaf;
      sum += term;
      if(tools::maybe_less(fabs(term/sum), errtol))
         break;
   }
   return sum;
}

template <class T, class Policy>
T non_central_t2_p(T n, T delta, T x, T y, const Policy& pol, T init_val)
{
   BOOST_MATH_STD_USING
   using namespace boost::math;
   //
   // Variables come first:
   //
   boost::uintmax_t max_iter = policies::get_max_series_iterations<Policy>();
   T errtol = ldexp(1.0, -boost::math::policies::digits<T, Policy>());
   T d2 = delta * delta / 2;
   //
   // k is the starting point for iteration, and is the
   // maximum of the poisson weighting term:
   //
   int k = tools::real_cast<int>(d2);
   // Starting Poisson weight:
   T pois = gamma_p_derivative(k+1, d2, pol) 
      * tgamma_delta_ratio(k + 1, T(0.5f))
      * delta / constants::root_two<T>();
   // Starting beta term:
   T beta = x < y
      ? ibeta(k + 1, n / 2, x, pol)
      : ibetac(n / 2, k + 1, y, pol);
   // Recurance term:
   T xterm = x < y
      ? ibeta_derivative(k + 1, n / 2, x, pol)
      : ibeta_derivative(n / 2, k + 1, y, pol);
   xterm *= (1 - x) / (n / 2 + k);
   T poisf(pois), betaf(beta), xtermf(xterm);
   T sum = init_val;

   //
   // Backwards recursion first, this is the stable
   // direction for recursion:
   //
   int count = 0;
   for(int i = k; i >= 0; --i)
   {
      T term = beta * pois;
      sum += term;
      if(tools::maybe_less(fabs(term/sum), errtol))
         break;
      pois *= (i + 0.5f) / d2;
      beta += xterm;
      xterm *= (i) / (x * (n / 2 + i - 1));
      ++count;
   }
   for(int i = k + 1; i - k < max_iter; ++i)
   {
      poisf *= d2 / (i + 0.5f);
      xtermf *= (x * (n / 2 + i - 1)) / (i);
      betaf -= xtermf;
      T term = poisf * betaf;
      sum += term;
      if(tools::maybe_less(fabs(term/sum), errtol))
         break;
      ++count;
   }
   return sum;
}

template <class T, class Policy>
T non_central_t2_q(T n, T delta, T x, T y, const Policy& pol, T init_val)
{
   BOOST_MATH_STD_USING
   using namespace boost::math;
   //
   // Variables come first:
   //
   boost::uintmax_t max_iter = policies::get_max_series_iterations<Policy>();
   T errtol = ldexp(1.0, -boost::math::policies::digits<T, Policy>());
   T d2 = delta * delta / 2;
   //
   // k is the starting point for iteration, and is the
   // maximum of the poisson weighting term:
   //
   int k = tools::real_cast<int>(d2);
   // Starting Poisson weight:
   T pois = gamma_p_derivative(k+1, d2, pol) 
      * tgamma_delta_ratio(k + 1, T(0.5f))
      * delta / constants::root_two<T>();
   // Starting beta term:
   T beta = x < y 
      ? ibetac(k + 1, n / 2, x, pol)
      : ibeta(n / 2, k + 1, y, pol);
   // Recurance term:
   T xterm = x < y
      ? ibeta_derivative(k + 1, n / 2, x, pol)
      : ibeta_derivative(n / 2, k + 1, y, pol);
   xterm *= (1 - x) / (n / 2 + k);
   T poisf(pois), betaf(beta), xtermf(xterm);
   T sum = init_val;

   //
   // Forward recursion first, this is the stable direction:
   //
   int count = 0;
   for(int i = k + 1; i - k < max_iter; ++i)
   {
      poisf *= d2 / (i + 0.5f);
      xtermf *= (x * (n / 2 + i - 1)) / (i);
      betaf += xtermf;

      T term = poisf * betaf;
      sum += term;
      if(tools::maybe_less(fabs(term/sum), errtol))
         break;
      ++count;
   }
   //
   // Backwards recursion:
   //
   for(int i = k; i >= 0; --i)
   {
      T term = beta * pois;
      sum += term;
      if(tools::maybe_less(fabs(term/sum), errtol))
         break;
      pois *= (i + 0.5f) / d2;
      beta -= xterm;
      xterm *= (i) / (x * (n / 2 + i - 1));
      ++count;
   }
   return sum;
}

template <class T, class Policy>
T get_non_central_t_cdf(T n, T delta, T t, bool invert, const Policy& pol)
{
   if(t < T(0))
   {
      t = -t;
      delta = -delta;
      invert = !invert;
   }
   T x = t * t / (n + t * t);
   T y = n / (n + t * t);
   T d2 = delta * delta;
   T a = 0.5f;
   T b = n / 2;
   T c = a + b + d2 / 2;
   T cross = 1 - (b / c) * (1 + d2 / (2 * c * c));
   T result;
   if(x < cross)
   {
      //
      // Calculate p:
      //
      result = non_central_beta_p(a, b, d2, x, y, pol);
      result = non_central_t2_p(n, delta, x, y, pol, result);
      result /= 2;
      result += cdf(boost::math::normal_distribution<T, Policy>(), -delta);
   }
   else
   {
      //
      // Calculate q:
      //
      invert = !invert;
      result = non_central_beta_q(a, b, d2, x, y, pol);
      result = non_central_t2_q(n, delta, x, y, pol, result);
      result /= 2;
   }
   if(invert)
      result = 1 - result;
   return result;
}

template <class T, class Policy>
T mean(T v, T delta, const Policy& pol)
{
   return  v <= 1 ? 0 : delta * sqrt(v / 2) * tgamma_delta_ratio((v - 1) * 0.5f, T(0.5f), pol);
}

template <class T, class Policy>
T variance(T v, T delta, const Policy& pol)
{
   if(v <= 2)
      return 0.1;
   T result = ((delta * delta + 1) * v) / (v - 2);
   T m = mean(v, delta, pol);
   result -= m * m;
   return result;
}

int i = 0;
int j = 4;
float factors[] = 
{
   1/10.0F,
   0.25f,
   0.5f,
   0.75f,
   0.9f,
   0.99f,
   0.999f,
   1.0f,
   1.0001f,
   1.001f,
   1.01f,
   1.1f,
   1.2f,
   1.3f,
   1.5f,
   2,
   3,
   4,
   5
};

int signs[] = {
1,1,1,-1,1,-1,-1,-1,1,1,-1,1,-1,-1,-1,-1,1,1,1,-1,-1,1,-1,1,-1,1,1,-1,1,-1,-1,-1,-1,-1,1,1
};

volatile float external_f;
float force_truncate(const float* f)
{
   external_f = *f;
   return external_f;
}

struct non_central_t_cdf
{
   std::tr1::tuple<mpfr_class, mpfr_class, mpfr_class, mpfr_class, mpfr_class> 
      operator()(
         mpfr_class v)
   {
      //
      // Use a Policy that ups the max number of iterations, so that
      // we should get convergence even in extreme cases!
      //
      boost::math::policies::policy<boost::math::policies::max_series_iterations<BOOST_MATH_MAX_SERIES_ITERATION_POLICY*10> > pol;

      float factor = factors[j % (sizeof(factors)/sizeof(factors[0]))];
      ++j;
      float delta = signs[j % (sizeof(signs)/sizeof(signs[0]))] * factor * boost::math::tools::real_cast<float>(v);
      factor = factors[i % (sizeof(factors)/sizeof(factors[0]))];
      ++i;
      float m = mean(boost::math::tools::real_cast<float>(v), delta, pol);
      float sd = sqrt(variance(boost::math::tools::real_cast<float>(v), delta, pol));
      float x = m + sd * factor * signs[j % (sizeof(signs)/sizeof(signs[0]))];

      mpfr_class p, q;
      std::cout << v << " " << delta << " " << x << "(x" << factor << ") ";
      if(x < m)
      {
         mpfr_interval pi = get_non_central_t_cdf(
            mpfr_interval(v), mpfr_interval(delta), 
            mpfr_interval(x), false, pol);
         
         std::cout << median(pi) << "(" 
            << width(pi)/median(pi) << ") ";
         std::cout << median(1-pi) << "("
            << width(1-pi)/median(1-pi) << ")" << std::endl;
         
         if(width(pi)/median(pi) > 1e-50)
            throw std::domain_error("Error too large");
         if(width(1-pi)/median(1-pi) > 1e-50)
            throw std::domain_error("Error too large");
         p = median(pi);
         q = median(1-pi);
      }
      else
      {
         mpfr_interval qi = get_non_central_t_cdf(
            mpfr_interval(v), mpfr_interval(delta), 
            mpfr_interval(x), true, pol);
         
         std::cout << median(1-qi) << "(" 
            << width(1-qi)/median(1-qi) << ") ";
         std::cout << median(qi) << "("
            << width(qi)/median(qi) << ")" << std::endl;

         if(width(qi)/median(qi) > 1e-50)
            throw std::domain_error("Error too large");
         if(width(1-qi)/median(1-qi) > 1e-50)
            throw std::domain_error("Error too large");
         p = median(1-qi);
         q = median(qi);
      }
      return std::tr1::make_tuple(v, delta, x, p, q);
   }
};

int test_main(int argc, char*argv [])
{
   using namespace boost::math::tools;

   mpfr_class::set_dprec(300);

   parameter_info<mpfr_class> arg1, arg2;
   test_data<mpfr_class> data;
   std::string line;
   bool cont = false;

   std::cout << "Welcome.\n"
      "This program will generate spot tests for the non central T CDF:\n\n";
   do{
      //get_user_parameter_info(arg1, "alpha");
      //get_user_parameter_info(arg2, "non centrality");
      arg1 = make_power_param(mpfr_class(0), 7, 15);
      arg2 = make_random_param(mpfr_class(0.0001), mpfr_class(200), 200);
      arg1.type |=dummy_param;
      arg2.type |=dummy_param;
      data.insert(non_central_t_cdf(), arg2);
      data.insert(non_central_t_cdf(), arg1);
      /*
      std::cout << "Any more data [y/n]?";
      std::getline(std::cin, line);
      boost::algorithm::trim(line);
      */
      cont = false; //(line == "y");
   }while(cont);

   /*
   std::cout << "Enter name of test data file [default=ncbeta.ipp]";
   std::getline(std::cin, line);
   boost::algorithm::trim(line);
   if(line == "")
      line = "ncbeta.ipp";
      */
   std::ofstream ofs("nct_big.ipp");
   ofs << std::setprecision(40);
   write_code(ofs, data, "nct_big");
   
   return 0;
}


