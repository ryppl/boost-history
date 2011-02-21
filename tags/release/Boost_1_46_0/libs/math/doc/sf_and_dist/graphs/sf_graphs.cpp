//  (C) Copyright John Maddock 2008.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifdef _MSC_VER
#  pragma warning (disable : 4127) // conditional expression is constant
#  pragma warning (disable : 4180) // qualifier applied to function type has no meaning; ignored
#  pragma warning (disable : 4503) // decorated name length exceeded, name was truncated
#  pragma warning (disable : 4512) // assignment operator could not be generated
#  pragma warning (disable : 4224) // nonstandard extension used : formal parameter 'function_ptr' was previously defined as a type
#endif

#include <boost/math/special_functions.hpp>
#include <boost/math/tools/roots.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include <list>
#include <map>
#include <string>
#include <boost/svg_plot/svg_2d_plot.hpp>
#include <boost/svg_plot/show_2d_settings.hpp>

class function_arity1_plotter
{
public:
   function_arity1_plotter() : m_min_x(0), m_max_x(0), m_min_y(0), m_max_y(0), m_has_legend(false) {}

   void add(boost::function<double(double)> f, double a, double b, const std::string& name)
   {
      if(name.size())
         m_has_legend = true;
      //
      // Now set our x-axis limits:
      //
      if(m_max_x == m_min_x)
      {
         m_max_x = b;
         m_min_x = a;
      }
      else
      {
         if(a < m_min_x) 
            m_min_x = a;
         if(b > m_max_x)
            m_max_x = b;
      }
      m_points.push_back(std::pair<std::string, std::map<double,double> >(name, std::map<double,double>()));
      std::map<double,double>& points = m_points.rbegin()->second;
      double interval = (b - a) / 200;
      for(double x = a; x <= b; x += interval)
      {
         //
         // Evaluate the function, set the Y axis limits 
         // if needed and then store the pair of points:
         //
         double y = f(x);
         if((m_min_y == m_max_y) && (m_min_y == 0))
            m_min_y = m_max_y = y;
         if(m_min_y > y)
            m_min_y = y;
         if(m_max_y < y)
            m_max_y = y;
         points[x] = y;
      }
   }

   void add(const std::map<double, double>& m, const std::string& name)
   {
      if(name.size())
         m_has_legend = true;
      m_points.push_back(std::pair<std::string, std::map<double,double> >(name, m));
      std::map<double, double>::const_iterator i = m.begin();

      while(i != m.end())
      {
         if((m_min_x == m_min_y) && (m_min_y == 0))
         {
            m_min_x = m_max_x = i->first;
         }
         if(i->first < m_min_x)
         {
            m_min_x = i->first;
         }
         if(i->first > m_max_x)
         {
            m_max_x = i->first;
         }

         if((m_min_y == m_max_y) && (m_min_y == 0))
         {
            m_min_y = m_max_y = i->second;
         }
         if(i->second < m_min_y)
         {
            m_min_y = i->second;
         }
         if(i->second > m_max_y)
         {
            m_max_y = i->second;
         }

         ++i;
      }
   }

   void plot(const std::string& title, const std::string& file,
      const std::string& x_lable = std::string(),
      const std::string& y_lable = std::string())
   {
      using namespace boost::svg;

      static const svg_color colors[5] = 
      {
         darkblue,
         darkred,
         darkgreen,
         darkorange,
         chartreuse
      };

      svg_2d_plot plot;
      plot.image_x_size(600);
      plot.image_y_size(400);
      plot.copyright_holder("John Maddock").copyright_date("2008").boost_license_on(true);
      plot.coord_precision(4); // Could be 3 for smaller plots?
      plot.title(title).title_font_size(20).title_on(true);
      plot.legend_on(m_has_legend);

      double x_delta = (m_max_x - m_min_x) / 50;
      double y_delta = (m_max_y - m_min_y) / 50;
      plot.x_range(m_min_x, m_max_x + x_delta)
         .y_range(m_min_y, m_max_y + y_delta);
      plot.x_label_on(true).x_label(x_lable);
      plot.y_label_on(true).y_label(y_lable);
      plot.y_major_grid_on(false).x_major_grid_on(false);
      plot.x_num_minor_ticks(3);
      plot.y_num_minor_ticks(3);
      //
      // Work out axis tick intervals:
      //
      double l = std::floor(std::log10((m_max_x - m_min_x) / 10) + 0.5);
      double interval = std::pow(10.0, (int)l);
      if(((m_max_x - m_min_x) / interval) > 10)
         interval *= 5;
      plot.x_major_interval(interval);
      l = std::floor(std::log10((m_max_y - m_min_y) / 10) + 0.5);
      interval = std::pow(10.0, (int)l);
      if(((m_max_y - m_min_y) / interval) > 10)
         interval *= 5;
      plot.y_major_interval(interval);
      plot.plot_window_on(true);
      plot.plot_border_color(lightslategray)
          .background_border_color(lightslategray)
          .legend_border_color(lightslategray)
          .legend_background_color(white);

      int color_index = 0;

      for(std::list<std::pair<std::string, std::map<double,double> > >::const_iterator i = m_points.begin();
         i != m_points.end(); ++i)
      {
         plot.plot(i->second, i->first)
            .line_on(true)
            .line_color(colors[color_index])
            .line_width(1.)
            .shape(none);
         if(i->first.size())
            ++color_index;
         color_index = color_index % (sizeof(colors)/sizeof(colors[0]));
      }
      plot.write(file);
   }

   void clear()
   {
      m_points.clear();
      m_min_x = m_min_y = m_max_x = m_max_y = 0;
      m_has_legend = false;
   }

private:
   std::list<std::pair<std::string, std::map<double, double> > > m_points;
   double m_min_x, m_max_x, m_min_y, m_max_y;
   bool m_has_legend;
};

template <class F>
struct location_finder
{
   location_finder(F _f, double t, double x0) : f(_f), target(t), x_off(x0){}

   double operator()(double x)
   {
      try
      {
         return f(x + x_off) - target;
      }
      catch(const std::overflow_error&)
      {
         return boost::math::tools::max_value<double>();
      }
      catch(const std::domain_error&)
      {
         if(x + x_off == x_off)
            return f(x_off + boost::math::tools::epsilon<double>() * x_off);
         throw;
      }
   }

private:
   F f;
   double target;
   double x_off;
};

template <class F>
double find_end_point(F f, double x0, double target, bool rising, double x_off = 0)
{
   boost::math::tools::eps_tolerance<double> tol(50);
   boost::uintmax_t max_iter = 1000;
   return boost::math::tools::bracket_and_solve_root(
      location_finder<F>(f, target, x_off), 
      x0, 
      double(1.5), 
      rising, 
      tol, 
      max_iter).first;
}

double sqrt1pm1(double x)
{
   return boost::math::sqrt1pm1(x);
}

double lbeta(double a, double b)
{
   return std::log(boost::math::beta(a, b));
}

int main()
{
   function_arity1_plotter plot;
   double (*f)(double);
   double (*f2)(double, double);
   double (*f2u)(unsigned, double);
   double (*f2i)(int, double);
   double (*f3)(double, double, double);
   double (*f4)(double, double, double, double);
   
   f = boost::math::zeta;
   plot.add(f, 1 + find_end_point(f, 0.1, 40.0, false, 1.0), 10, "");
   plot.add(f, -20, 1 + find_end_point(f, -0.1, -40.0, false, 1.0), "");
   plot.plot("Zeta Function Over [-20,10]", "zeta1.svg", "z", "zeta(z)");

   plot.clear();
   plot.add(f, -14, 0, "");
   plot.plot("Zeta Function Over [-14,0]", "zeta2.svg", "z", "zeta(z)");

   f = boost::math::tgamma;
   double max_val = f(6);
   plot.clear();
   plot.add(f, find_end_point(f, 0.1, max_val, false), 6, "");
   plot.add(f, -1 + find_end_point(f, 0.1, -max_val, true, -1), find_end_point(f, -0.1, -max_val, false), "");
   plot.add(f, -2 + find_end_point(f, 0.1, max_val, false, -2), -1 + find_end_point(f, -0.1, max_val, true, -1), "");
   plot.add(f, -3 + find_end_point(f, 0.1, -max_val, true, -3), -2 + find_end_point(f, -0.1, -max_val, false, -2), "");
   plot.add(f, -4 + find_end_point(f, 0.1, max_val, false, -4), -3 + find_end_point(f, -0.1, max_val, true, -3), "");
   plot.plot("tgamma", "tgamma.svg", "z", "tgamma(z)");

   f = boost::math::lgamma;
   max_val = f(10);
   plot.clear();
   plot.add(f, find_end_point(f, 0.1, max_val, false), 10, "");
   plot.add(f, -1 + find_end_point(f, 0.1, max_val, false, -1), find_end_point(f, -0.1, max_val, true), "");
   plot.add(f, -2 + find_end_point(f, 0.1, max_val, false, -2), -1 + find_end_point(f, -0.1, max_val, true, -1), "");
   plot.add(f, -3 + find_end_point(f, 0.1, max_val, false, -3), -2 + find_end_point(f, -0.1, max_val, true, -2), "");
   plot.add(f, -4 + find_end_point(f, 0.1, max_val, false, -4), -3 + find_end_point(f, -0.1, max_val, true, -3), "");
   plot.add(f, -5 + find_end_point(f, 0.1, max_val, false, -5), -4 + find_end_point(f, -0.1, max_val, true, -4), "");
   plot.plot("lgamma", "lgamma.svg", "z", "lgamma(z)");

   f = boost::math::digamma;
   max_val = 10;
   plot.clear();
   plot.add(f, find_end_point(f, 0.1, -max_val, true), 10, "");
   plot.add(f, -1 + find_end_point(f, 0.1, -max_val, true, -1), find_end_point(f, -0.1, max_val, true), "");
   plot.add(f, -2 + find_end_point(f, 0.1, -max_val, true, -2), -1 + find_end_point(f, -0.1, max_val, true, -1), "");
   plot.add(f, -3 + find_end_point(f, 0.1, -max_val, true, -3), -2 + find_end_point(f, -0.1, max_val, true, -2), "");
   plot.add(f, -4 + find_end_point(f, 0.1, -max_val, true, -4), -3 + find_end_point(f, -0.1, max_val, true, -3), "");
   plot.plot("digamma", "digamma.svg", "z", "digamma(z)");

   f = boost::math::erf;
   plot.clear();
   plot.add(f, -3, 3, "");
   plot.plot("erf", "erf.svg", "z", "erf(z)");
   f = boost::math::erfc;
   plot.clear();
   plot.add(f, -3, 3, "");
   plot.plot("erfc", "erfc.svg", "z", "erfc(z)");

   f = boost::math::erf_inv;
   plot.clear();
   plot.add(f, -1 + find_end_point(f, 0.1, -3, true, -1), 1 + find_end_point(f, -0.1, 3, true, 1), "");
   plot.plot("erf_inv", "erf_inv.svg", "z", "erf_inv(z)");
   f = boost::math::erfc_inv;
   plot.clear();
   plot.add(f, find_end_point(f, 0.1, 3, false), 2 + find_end_point(f, -0.1, -3, false, 2), "");
   plot.plot("erfc_inv", "erfc_inv.svg", "z", "erfc_inv(z)");

   f = boost::math::log1p;
   plot.clear();
   plot.add(f, -1 + find_end_point(f, 0.1, -10, true, -1), 10, "");
   plot.plot("log1p", "log1p.svg", "z", "log1p(z)");

   f = boost::math::expm1;
   plot.clear();
   plot.add(f, -4, 2, "");
   plot.plot("expm1", "expm1.svg", "z", "expm1(z)");

   f = boost::math::cbrt;
   plot.clear();
   plot.add(f, -10, 10, "");
   plot.plot("cbrt", "cbrt.svg", "z", "cbrt(z)");

   f = sqrt1pm1;
   plot.clear();
   plot.add(f, -1 + find_end_point(f, 0.1, -10, true, -1), 5, "");
   plot.plot("sqrt1pm1", "sqrt1pm1.svg", "z", "sqrt1pm1(z)");

   f2 = boost::math::powm1;
   plot.clear();
   plot.add(boost::bind(f2, 0.0001, _1), find_end_point(boost::bind(f2, 0.0001, _1), -1, 10, false), 5, "a=0.0001");
   plot.add(boost::bind(f2, 0.001, _1), find_end_point(boost::bind(f2, 0.001, _1), -1, 10, false), 5, "a=0.001");
   plot.add(boost::bind(f2, 0.01, _1), find_end_point(boost::bind(f2, 0.01, _1), -1, 10, false), 5, "a=0.01");
   plot.add(boost::bind(f2, 0.1, _1), find_end_point(boost::bind(f2, 0.1, _1), -1, 10, false), 5, "a=0.1");
   plot.add(boost::bind(f2, 0.75, _1), -5, 5, "a=0.75");
   plot.add(boost::bind(f2, 1.25, _1), -5, 5, "a=1.25");
   plot.plot("powm1", "powm1.svg", "z", "powm1(a, z)");

   f = boost::math::sinc_pi;
   plot.clear();
   plot.add(f, -10, 10, "");
   plot.plot("sinc_pi", "sinc_pi.svg", "z", "sinc_pi(z)");

   f = boost::math::sinhc_pi;
   plot.clear();
   plot.add(f, -5, 5, "");
   plot.plot("sinhc_pi", "sinhc_pi.svg", "z", "sinhc_pi(z)");

   f = boost::math::acosh;
   plot.clear();
   plot.add(f, 1, 10, "");
   plot.plot("acosh", "acosh.svg", "z", "acosh(z)");

   f = boost::math::asinh;
   plot.clear();
   plot.add(f, -10, 10, "");
   plot.plot("asinh", "asinh.svg", "z", "asinh(z)");

   f = boost::math::atanh;
   plot.clear();
   plot.add(f, -1 + find_end_point(f, 0.1, -5, true, -1), 1 + find_end_point(f, -0.1, 5, true, 1), "");
   plot.plot("atanh", "atanh.svg", "z", "atanh(z)");

   f2 = boost::math::tgamma_delta_ratio;
   plot.clear();
   plot.add(boost::bind(f2, _1, -0.5), 1, 40, "delta = -0.5");
   plot.add(boost::bind(f2, _1, -0.2), 1, 40, "delta = -0.2");
   plot.add(boost::bind(f2, _1, -0.1), 1, 40, "delta = -0.1");
   plot.add(boost::bind(f2, _1, 0.1), 1, 40, "delta = 0.1");
   plot.add(boost::bind(f2, _1, 0.2), 1, 40, "delta = 0.2");
   plot.add(boost::bind(f2, _1, 0.5), 1, 40, "delta = 0.5");
   plot.add(boost::bind(f2, _1, 1.0), 1, 40, "delta = 1.0");
   plot.plot("tgamma_delta_ratio", "tgamma_delta_ratio.svg", "z", "tgamma_delta_ratio(delta, z)");

   f2 = boost::math::gamma_p;
   plot.clear();
   plot.add(boost::bind(f2, 0.5, _1), 0, 20, "a = 0.5");
   plot.add(boost::bind(f2, 1.0, _1), 0, 20, "a = 1.0");
   plot.add(boost::bind(f2, 5.0, _1), 0, 20, "a = 5.0");
   plot.add(boost::bind(f2, 10.0, _1), 0, 20, "a = 10.0");
   plot.plot("gamma_p", "gamma_p.svg", "z", "gamma_p(a, z)");

   f2 = boost::math::gamma_q;
   plot.clear();
   plot.add(boost::bind(f2, 0.5, _1), 0, 20, "a = 0.5");
   plot.add(boost::bind(f2, 1.0, _1), 0, 20, "a = 1.0");
   plot.add(boost::bind(f2, 5.0, _1), 0, 20, "a = 5.0");
   plot.add(boost::bind(f2, 10.0, _1), 0, 20, "a = 10.0");
   plot.plot("gamma_q", "gamma_q.svg", "z", "gamma_q(a, z)");

   f2 = lbeta;
   plot.clear();
   plot.add(boost::bind(f2, 0.5, _1), 0.00001, 5, "a = 0.5");
   plot.add(boost::bind(f2, 1.0, _1), 0.00001, 5, "a = 1.0");
   plot.add(boost::bind(f2, 5.0, _1), 0.00001, 5, "a = 5.0");
   plot.add(boost::bind(f2, 10.0, _1), 0.00001, 5, "a = 10.0");
   plot.plot("beta", "beta.svg", "z", "log(beta(a, z))");

   f = boost::math::expint;
   max_val = f(4);
   plot.clear();
   plot.add(f, find_end_point(f, 0.1, -max_val, true), 4, "");
   plot.add(f, -3, find_end_point(f, -0.1, -max_val, false), "");
   plot.plot("Exponential Integral Ei", "expint_i.svg", "z", "expint(z)");

   f2u = boost::math::expint;
   max_val = 1;
   plot.clear();
   plot.add(boost::bind(f2u, 1, _1), find_end_point(boost::bind(f2u, 1, _1), 0.1, max_val, false), 2, "n = 1 ");
   plot.add(boost::bind(f2u, 2, _1), find_end_point(boost::bind(f2u, 2, _1), 0.1, max_val, false), 2, "n = 2 ");
   plot.add(boost::bind(f2u, 3, _1), 0, 2, "n = 3 ");
   plot.add(boost::bind(f2u, 4, _1), 0, 2, "n = 4 ");
   plot.plot("Exponential Integral En", "expint2.svg", "z", "expint(n, z)");

   f3 = boost::math::ibeta;
   plot.clear();
   plot.add(boost::bind(f3, 9, 1, _1), 0, 1, "a = 9, b = 1");
   plot.add(boost::bind(f3, 7, 2, _1), 0, 1, "a = 7, b = 2");
   plot.add(boost::bind(f3, 5, 5, _1), 0, 1, "a = 5, b = 5");
   plot.add(boost::bind(f3, 2, 7, _1), 0, 1, "a = 2, b = 7");
   plot.add(boost::bind(f3, 1, 9, _1), 0, 1, "a = 1, b = 9");
   plot.plot("ibeta", "ibeta.svg", "z", "ibeta(a, b, z)");

   f2i = boost::math::legendre_p;
   plot.clear();
   plot.add(boost::bind(f2i, 1, _1), -1, 1, "l = 1");
   plot.add(boost::bind(f2i, 2, _1), -1, 1, "l = 2");
   plot.add(boost::bind(f2i, 3, _1), -1, 1, "l = 3");
   plot.add(boost::bind(f2i, 4, _1), -1, 1, "l = 4");
   plot.add(boost::bind(f2i, 5, _1), -1, 1, "l = 5");
   plot.plot("Legendre Polynomials", "legendre_p.svg", "x", "legendre_p(l, x)");

   f2u = boost::math::legendre_q;
   plot.clear();
   plot.add(boost::bind(f2u, 1, _1), -0.95, 0.95, "l = 1");
   plot.add(boost::bind(f2u, 2, _1), -0.95, 0.95, "l = 2");
   plot.add(boost::bind(f2u, 3, _1), -0.95, 0.95, "l = 3");
   plot.add(boost::bind(f2u, 4, _1), -0.95, 0.95, "l = 4");
   plot.add(boost::bind(f2u, 5, _1), -0.95, 0.95, "l = 5");
   plot.plot("Legendre Polynomials of the Second Kind", "legendre_q.svg", "x", "legendre_q(l, x)");

   f2u = boost::math::laguerre;
   plot.clear();
   plot.add(boost::bind(f2u, 0, _1), -5, 10, "n = 0");
   plot.add(boost::bind(f2u, 1, _1), -5, 10, "n = 1");
   plot.add(boost::bind(f2u, 2, _1), 
      find_end_point(boost::bind(f2u, 2, _1), -2, 20, false), 
      find_end_point(boost::bind(f2u, 2, _1), 4, 20, true), 
      "n = 2");
   plot.add(boost::bind(f2u, 3, _1), 
      find_end_point(boost::bind(f2u, 3, _1), -2, 20, false), 
      8 + find_end_point(boost::bind(f2u, 3, _1), 1, 20, false, 8), 
      "n = 3");
   plot.add(boost::bind(f2u, 4, _1), 
      find_end_point(boost::bind(f2u, 4, _1), -2, 20, false), 
      8 + find_end_point(boost::bind(f2u, 4, _1), 1, 20, true, 8), 
      "n = 4");
   plot.add(boost::bind(f2u, 5, _1), 
      find_end_point(boost::bind(f2u, 5, _1), -2, 20, false), 
      8 + find_end_point(boost::bind(f2u, 5, _1), 1, 20, true, 8), 
      "n = 5");
   plot.plot("Laguerre Polynomials", "laguerre.svg", "x", "laguerre(n, x)");

   f2u = boost::math::hermite;
   plot.clear();
   plot.add(boost::bind(f2u, 0, _1), -1.8, 1.8, "n = 0");
   plot.add(boost::bind(f2u, 1, _1), -1.8, 1.8, "n = 1");
   plot.add(boost::bind(f2u, 2, _1), -1.8, 1.8, "n = 2");
   plot.add(boost::bind(f2u, 3, _1), -1.8, 1.8, "n = 3");
   plot.add(boost::bind(f2u, 4, _1), -1.8, 1.8, "n = 4");
   plot.plot("Hermite Polynomials", "hermite.svg", "x", "hermite(n, x)");

   f2 = boost::math::cyl_bessel_j;
   plot.clear();
   plot.add(boost::bind(f2, 0, _1), -20, 20, "v = 0");
   plot.add(boost::bind(f2, 1, _1), -20, 20, "v = 1");
   plot.add(boost::bind(f2, 2, _1), -20, 20, "v = 2");
   plot.add(boost::bind(f2, 3, _1), -20, 20, "v = 3");
   plot.add(boost::bind(f2, 4, _1), -20, 20, "v = 4");
   plot.plot("Bessel J", "cyl_bessel_j.svg", "x", "cyl_bessel_j(v, x)");

   f2 = boost::math::cyl_neumann;
   plot.clear();
   plot.add(boost::bind(f2, 0, _1), find_end_point(boost::bind(f2, 0, _1), 0.1, -5, true), 20, "v = 0");
   plot.add(boost::bind(f2, 1, _1), find_end_point(boost::bind(f2, 1, _1), 0.1, -5, true), 20, "v = 1");
   plot.add(boost::bind(f2, 2, _1), find_end_point(boost::bind(f2, 2, _1), 0.1, -5, true), 20, "v = 2");
   plot.add(boost::bind(f2, 3, _1), find_end_point(boost::bind(f2, 3, _1), 0.1, -5, true), 20, "v = 3");
   plot.add(boost::bind(f2, 4, _1), find_end_point(boost::bind(f2, 4, _1), 0.1, -5, true), 20, "v = 4");
   plot.plot("Bessel Y", "cyl_neumann.svg", "x", "cyl_neumann(v, x)");

   f2 = boost::math::cyl_bessel_i;
   plot.clear();
   plot.add(boost::bind(f2, 0, _1), find_end_point(boost::bind(f2, 0, _1), -0.1, 20, false), find_end_point(boost::bind(f2, 0, _1), 0.1, 20, true), "v = 0");
   plot.add(boost::bind(f2, 2, _1), find_end_point(boost::bind(f2, 2, _1), -0.1, 20, false), find_end_point(boost::bind(f2, 2, _1), 0.1, 20, true), "v = 2");
   plot.add(boost::bind(f2, 5, _1), find_end_point(boost::bind(f2, 5, _1), -0.1, -20, true), find_end_point(boost::bind(f2, 5, _1), 0.1, 20, true), "v = 5");
   plot.add(boost::bind(f2, 7, _1), find_end_point(boost::bind(f2, 7, _1), -0.1, -20, true), find_end_point(boost::bind(f2, 7, _1), 0.1, 20, true), "v = 7");
   plot.add(boost::bind(f2, 10, _1), find_end_point(boost::bind(f2, 10, _1), -0.1, 20, false), find_end_point(boost::bind(f2, 10, _1), 0.1, 20, true), "v = 10");
   plot.plot("Bessel I", "cyl_bessel_i.svg", "x", "cyl_bessel_i(v, x)");

   f2 = boost::math::cyl_bessel_k;
   plot.clear();
   plot.add(boost::bind(f2, 0, _1), find_end_point(boost::bind(f2, 0, _1), 0.1, 10, false), 10, "v = 0");
   plot.add(boost::bind(f2, 2, _1), find_end_point(boost::bind(f2, 2, _1), 0.1, 10, false), 10, "v = 2");
   plot.add(boost::bind(f2, 5, _1), find_end_point(boost::bind(f2, 5, _1), 0.1, 10, false), 10, "v = 5");
   plot.add(boost::bind(f2, 7, _1), find_end_point(boost::bind(f2, 7, _1), 0.1, 10, false), 10, "v = 7");
   plot.add(boost::bind(f2, 10, _1), find_end_point(boost::bind(f2, 10, _1), 0.1, 10, false), 10, "v = 10");
   plot.plot("Bessel K", "cyl_bessel_k.svg", "x", "cyl_bessel_k(v, x)");

   f2u = boost::math::sph_bessel;
   plot.clear();
   plot.add(boost::bind(f2u, 0, _1), 0, 20, "v = 0");
   plot.add(boost::bind(f2u, 2, _1), 0, 20, "v = 2");
   plot.add(boost::bind(f2u, 5, _1), 0, 20, "v = 5");
   plot.add(boost::bind(f2u, 7, _1), 0, 20, "v = 7");
   plot.add(boost::bind(f2u, 10, _1), 0, 20, "v = 10");
   plot.plot("Bessel j", "sph_bessel.svg", "x", "sph_bessel(v, x)");

   f2u = boost::math::sph_neumann;
   plot.clear();
   plot.add(boost::bind(f2u, 0, _1), find_end_point(boost::bind(f2u, 0, _1), 0.1, -5, true), 20, "v = 0");
   plot.add(boost::bind(f2u, 2, _1), find_end_point(boost::bind(f2u, 2, _1), 0.1, -5, true), 20, "v = 2");
   plot.add(boost::bind(f2u, 5, _1), find_end_point(boost::bind(f2u, 5, _1), 0.1, -5, true), 20, "v = 5");
   plot.add(boost::bind(f2u, 7, _1), find_end_point(boost::bind(f2u, 7, _1), 0.1, -5, true), 20, "v = 7");
   plot.add(boost::bind(f2u, 10, _1), find_end_point(boost::bind(f2u, 10, _1), 0.1, -5, true), 20, "v = 10");
   plot.plot("Bessel y", "sph_neumann.svg", "x", "sph_neumann(v, x)");

   f4 = boost::math::ellint_rj;
   plot.clear();
   plot.add(boost::bind(f4, _1, _1, _1, _1), find_end_point(boost::bind(f4, _1, _1, _1, _1), 0.1, 10, false), 4, "RJ");
   f3 = boost::math::ellint_rf;
   plot.add(boost::bind(f3, _1, _1, _1), find_end_point(boost::bind(f3, _1, _1, _1), 0.1, 10, false), 4, "RF");
   plot.plot("Elliptic Integrals", "ellint_carlson.svg", "x", "");

   f2 = boost::math::ellint_1;
   plot.clear();
   plot.add(boost::bind(f2, _1, 0.5), -0.9, 0.9, "&#x3C6;=0.5");
   plot.add(boost::bind(f2, _1, 0.75), -0.9, 0.9, "&#x3C6;=0.75");
   plot.add(boost::bind(f2, _1, 1.25), -0.9, 0.9, "&#x3C6;=1.25");
   plot.add(boost::bind(f2, _1, boost::math::constants::pi<double>() / 2), -0.9, 0.9, "&#x3C6;=&#x3C0;/2");
   plot.plot("Elliptic Of the First Kind", "ellint_1.svg", "k", "ellint_1(k, phi)");

   f2 = boost::math::ellint_2;
   plot.clear();
   plot.add(boost::bind(f2, _1, 0.5), -1, 1, "&#x3C6;=0.5");
   plot.add(boost::bind(f2, _1, 0.75), -1, 1, "&#x3C6;=0.75");
   plot.add(boost::bind(f2, _1, 1.25), -1, 1, "&#x3C6;=1.25");
   plot.add(boost::bind(f2, _1, boost::math::constants::pi<double>() / 2), -1, 1, "&#x3C6;=&#x3C0;/2");
   plot.plot("Elliptic Of the Second Kind", "ellint_2.svg", "k", "ellint_2(k, phi)");

   f3 = boost::math::ellint_3;
   plot.clear();
   plot.add(boost::bind(f3, _1, 0, 1.25), -1, 1, "n=0 &#x3C6;=1.25");
   plot.add(boost::bind(f3, _1, 0.5, 1.25), -1, 1, "n=0.5 &#x3C6;=1.25");
   plot.add(boost::bind(f3, _1, 0.25, boost::math::constants::pi<double>() / 2), 
      find_end_point(
         boost::bind(f3, _1, 0.25, boost::math::constants::pi<double>() / 2), 
         0.5, 4, false, -1) - 1, 
      find_end_point(
         boost::bind(f3, _1, 0.25, boost::math::constants::pi<double>() / 2), 
         -0.5, 4, true, 1) + 1, "n=0.25 &#x3C6;=&#x3C0;/2");
   plot.add(boost::bind(f3, _1, 0.75, boost::math::constants::pi<double>() / 2), 
      find_end_point(
         boost::bind(f3, _1, 0.75, boost::math::constants::pi<double>() / 2), 
         0.5, 4, false, -1) - 1, 
      find_end_point(
         boost::bind(f3, _1, 0.75, boost::math::constants::pi<double>() / 2), 
         -0.5, 4, true, 1) + 1, "n=0.75 &#x3C6;=&#x3C0;/2");
   plot.plot("Elliptic Of the Third Kind", "ellint_3.svg", "k", "ellint_3(k, n, phi)");

   return 0;
}

