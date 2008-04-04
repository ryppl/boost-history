//  (C) Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/math/bindings/mpfr_interval.hpp>
#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/math/special_functions/gamma.hpp>
#include <boost/math/tools/test.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <map>

#include <boost/math/tools/test_data.hpp>

using namespace boost::math::tools;
using namespace boost::math;
using namespace std;

template <class T, class Policy>
T non_central_chi_square_q(T x, T f, T theta, const Policy& pol)
{
   //
   // Computes the complement of the Non-Central Chi-Square
   // Distribution CDF by summing a weighted sum of complements
   // of the central-distributions.  The weighting factor is
   // a Poisson Distribution.
   //
   BOOST_MATH_STD_USING
   using namespace boost::math;

   T lambda = theta / 2;
   T del = f / 2;
   T y = x / 2;
   boost::uintmax_t max_iter = policies::get_max_series_iterations<Policy>();
   T errtol = ldexp(1.0, -boost::math::policies::digits<T, Policy>());
   T sum = 0;
   int k = tools::real_cast<int>(lambda);
   T poisf = boost::math::gamma_p_derivative(1 + k, lambda);
   T poisb = poisf * k / lambda;
   T gamf = boost::math::gamma_q(del + k, y);
   T xtermf = boost::math::gamma_p_derivative(del + 1 + k, y);
   T xtermb = xtermf * (del + k) / y;
   T gamb = gamf - xtermb;


   for(int i = k; i < max_iter; ++i)
   {
      T term = poisf * gamf;
      sum += term;
      poisf *= lambda / (i + 1);
      gamf += xtermf;
      xtermf *= y / (del + i + 1);
      if(tools::maybe_less(term / sum, errtol))
         break;
   }
   for(int i = k - 1; i >= 0; --i)
   {
      T term = poisb * gamb;
      sum += term;
      poisb *= i / lambda;
      xtermb *= (del + i) / y;
      gamb -= xtermb;
      if(tools::maybe_less(term / sum, errtol))
         break;
   }

   return sum;
}

template <class T, class Policy>
T non_central_chi_square_p(T y, T n, T lambda, const Policy& pol)
{
   using namespace boost::math;
   BOOST_MATH_STD_USING
   boost::uintmax_t max_iter = policies::get_max_series_iterations<Policy>();
   T errtol = ldexp(1.0, -boost::math::policies::digits<T, Policy>());
   T errorf, errorb;

   T x = y / 2;
   T del = lambda / 2;

   int k = //tools::real_cast<int>(detail::inverse_poisson_cornish_fisher(del, 1 - errtol * 100, errtol * 100, pol));
      tools::real_cast<int>(del);
   T a = n / 2 + k;
   T gamkf = boost::math::gamma_p(a, x, pol);
   BOOST_MATH_INSTRUMENT_VARIABLE(gamkf);

   if(lambda == 0)
      return gamkf;
   
   T gamkb = gamkf;
   T poiskf = gamma_p_derivative(k+1, del, pol);
   BOOST_MATH_INSTRUMENT_VARIABLE(poiskf)
   
   T poiskb = poiskf;
   T xtermf = boost::math::gamma_p_derivative(a, x, pol);
   BOOST_MATH_INSTRUMENT_VARIABLE(xtermf)

   T xtermb = xtermf * x / a;
   T sum = poiskf * gamkf;
   int i = 1;
   //bool recurse_fwd(true), recurse_bwd(true);

   while(i <= k)
   {
      xtermb *= (a - i + 1) / x;
      gamkb += xtermb;
      poiskb = poiskb * (k - i + 1) / del;
      errorb = gamkb * poiskb;
      sum += errorb;
      if(tools::maybe_less(errorb / sum, errtol))
         break;
      ++i;
   }
   i = 1;
   do
   {
      xtermf = xtermf * x / (a + i - 1);
      gamkf = gamkf - xtermf;
      poiskf = poiskf * del / (k + i);
      errorf = poiskf * gamkf;
      sum += errorf;
      ++i;
   }while(!tools::maybe_less(errorf / sum, errtol));

   return sum;
}

int i = 0;
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

struct non_central_chi_square_cdf
{
   std::tr1::tuple<mpfr_class, mpfr_class, mpfr_class> 
      operator()(
         mpfr_class df, 
         mpfr_class ncp)
   {
      float factor = factors[i % (sizeof(factors)/sizeof(factors[0]))];
      ++i;
      float x = factor * boost::math::tools::real_cast<float>(mpfr_class(df + ncp));
      mpfr_class p, q;
      boost::math::policies::policy<> pol;
      std::cout << df << " " << ncp << " " << x << "(x" << factor << ") ";
      if(x < df + ncp)
      {
         mpfr_interval pi = non_central_chi_square_p(
            mpfr_interval(x), mpfr_interval(df), mpfr_interval(ncp), pol);
         
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
         mpfr_interval qi = non_central_chi_square_q(
            mpfr_interval(x), mpfr_interval(df), mpfr_interval(ncp), pol);
         
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
      return std::tr1::make_tuple(x, p, q);
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

   if((argc >= 2) && (std::strcmp(argv[1], "-inverse") == 0))
   {
      std::cout << "Welcome.\n"
      "This program will generate spot tests for the non central chi square CDF:\n";

      do{
         get_user_parameter_info(arg1, "degrees of freedom");
         get_user_parameter_info(arg2, "non centrality");
         data.insert(non_central_chi_square_cdf(), arg1, arg2);

         std::cout << "Any more data [y/n]?";
         std::getline(std::cin, line);
         boost::algorithm::trim(line);
         cont = (line == "y");
      }while(cont);
   }
   else
   {
      std::cout << "Welcome.\n"
         "This program will generate spot tests for the non central chi square CDF:\n\n";
      do{
         get_user_parameter_info(arg1, "degrees of freedom");
         get_user_parameter_info(arg2, "non centrality");
         data.insert(non_central_chi_square_cdf(), arg1, arg2);

         std::cout << "Any more data [y/n]?";
         std::getline(std::cin, line);
         boost::algorithm::trim(line);
         cont = (line == "y");
      }while(cont);
   }

   std::cout << "Enter name of test data file [default=nccs.ipp]";
   std::getline(std::cin, line);
   boost::algorithm::trim(line);
   if(line == "")
      line = "nccs.ipp";
   std::ofstream ofs(line.c_str());
   ofs << std::setprecision(40);
   write_code(ofs, data, "nccs");
   
   return 0;
}


