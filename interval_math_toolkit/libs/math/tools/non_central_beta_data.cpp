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
T non_central_beta_p(T a, T b, T lam, T x, const Policy& pol)
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
   int k = boost::math::tools::real_cast<int>(l2);
   // Starting Poisson weight:
   T pois = gamma_p_derivative(k+1, l2, pol);
   // Starting beta term:
   T beta = ibeta(a + k, b, x, pol);
   // recurance term:
   T xterm = ibeta_derivative(a + k, b, x, pol) * (1 - x) / (a + b + k - 1);
   T poisf(pois), betaf(beta), xtermf(xterm);
   T sum = 0;

   //
   // Backwards recursion first, this is the stable
   // direction for recursion:
   //
   for(int i = k; i >= 0; --i)
   {
      //T beta_c = ibeta(a + i, b, x);
      //T pois_c = pdf(poisson_distribution<T>(l2), i);
      T term = beta * pois;
      //std::cout << i << "  " << term << std::endl;
      sum += term;
      if(fabs(term/sum) < errtol)
         break;
      pois *= i / l2;
      beta += xterm;
      xterm *= (a + i - 1) / (x * (a + b + i - 2));
   }
   for(int i = k + 1; i - k < max_iter; ++i)
   {
      poisf *= l2 / i;
      xtermf *= (x * (a + b + i - 2)) / (a + i - 1);
      betaf -= xtermf;
      //T beta_c = ibeta(a + i, b, x);
      //T pois_c = pdf(poisson_distribution<T>(l2), i);

      T term = poisf * betaf;
      //std::cout << i << "  " << term << std::endl;
      sum += term;
      if(fabs(term/sum) < errtol)
         break;
   }
   return sum;
}

template <class T, class Policy>
T non_central_beta_q(T a, T b, T lam, T x, const Policy& pol)
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
   int k = boost::math::tools::real_cast<int>(l2);
   // Starting Poisson weight:
   T pois = gamma_p_derivative(k+1, l2, pol);
   // Starting beta term:
   T beta = ibetac(a + k, b, x, pol);
   // recurance term:
   T xterm = ibeta_derivative(a + k, b, x, pol) * (1 - x) / (a + b + k - 1);
   T poisf(pois), betaf(beta), xtermf(xterm);
   T sum = 0;

   //
   // Backwards recursion first, this is the unstable
   // direction for recursion:
   //
   for(int i = k; i >= 0; --i)
   {
      T beta_c = ibetac(a + i, b, x);
      T pois_c = pdf(poisson_distribution<T>(l2), i);
      T term = beta * pois;
      //std::cout << i << "  " << term << std::endl;
      sum += term;
      if(fabs(term/sum) < errtol)
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
      T beta_c = ibeta(a + i, b, x);
      T pois_c = pdf(poisson_distribution<T>(l2), i);

      T term = poisf * betaf;
      //std::cout << i << "  " << term << std::endl;
      sum += term;
      if(fabs(term/sum) < errtol)
         break;
   }
   return sum;
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

volatile float external_f;
float force_truncate(const float* f)
{
   external_f = *f;
   return external_f;
}

struct non_central_beta_cdf
{
   std::tr1::tuple<mpfr_class, mpfr_class, mpfr_class, mpfr_class, mpfr_class, mpfr_class> 
      operator()(
         mpfr_class alpha, 
         mpfr_class ncp)
   {
      float factor = factors[j % (sizeof(factors)/sizeof(factors[0]))];
      ++j;
      float beta = factor * boost::math::tools::real_cast<float>(alpha);
      factor = factors[i % (sizeof(factors)/sizeof(factors[0]))];
      ++i;
      mpfr_class c = alpha + beta + ncp / 2;
      mpfr_class cross = 1 - (beta / c) * (1 + ncp / (2 * c * c));
      float x;
      if(factor < 1)
         x = factor * boost::math::tools::real_cast<float>(cross);
      else
         x = 1 - (1 - boost::math::tools::real_cast<float>(cross)) / factor;
      mpfr_class p, q;
      //
      // Use a Policy that ups the max number of iterations, so that
      // we should get convergence even in extreme cases!
      //
      boost::math::policies::policy<boost::math::policies::max_series_iterations<BOOST_MATH_MAX_SERIES_ITERATION_POLICY*10> > pol;
      std::cout << alpha << " " << beta << " " << ncp << " " << x << "(x" << factor << ") ";
      if(x < cross)
      {
         mpfr_interval pi = non_central_beta_p(
            mpfr_interval(alpha), mpfr_interval(beta), 
            mpfr_interval(ncp), mpfr_interval(x), pol);
         
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
         mpfr_interval qi = non_central_beta_q(
            mpfr_interval(alpha), mpfr_interval(beta), 
            mpfr_interval(ncp), mpfr_interval(x), pol);
         
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
      return std::tr1::make_tuple(alpha, beta, ncp, x, p, q);
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
      "This program will generate spot tests for the non central beta CDF:\n\n";
   do{
      //get_user_parameter_info(arg1, "alpha");
      //get_user_parameter_info(arg2, "non centrality");
      arg1 = make_power_param(mpfr_class(0), 7, 15);
      arg2 = make_power_param(mpfr_class(0), 7, 15);
      arg1.type |=dummy_param;
      arg2.type |=dummy_param;
      data.insert(non_central_beta_cdf(), arg1, arg2);
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
   std::ofstream ofs("ncbeta_big.ipp");
   ofs << std::setprecision(40);
   write_code(ofs, data, "ncbeta_big");
   
   return 0;
}


