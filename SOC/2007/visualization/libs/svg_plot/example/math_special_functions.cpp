// JM_Specials.cpp

// Copyright John Maddock 2007
// Copyright Paul A. Bristow 2007

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifdef _MSC_VER
#  pragma warning (disable : 4800) // forcing value to bool 'true' or 'false' (performance warning)
#  pragma warning (disable : 4180) // qualifier applied to function type has no meaning; ignored
#  pragma warning (disable : 4224) // nonstandard extension used : formal parameter 'function_ptr' was previously defined as a type
//#  pragma warning (disable : 4267) // conversion from 'size_t' to 'const unsigned int', possible loss of data
#  pragma warning (disable : 4127) // conditional expression is constant
#  pragma warning (disable : 4512) // assignment operator could not be generated (in fusion)
#  pragma warning (disable : 4503) // decorated name length exceeded, name was truncated
#  pragma warning (disable : 4180) // qualifier applied to function type has no meaning; ignored
#  pragma warning (disable : 4267) //  conversion from 'size_t' to 'const unsigned int', possible loss of data
#  pragma warning (disable : 4172) //  returning address of local variable or temporary
#endif

#include <boost/math/special_functions.hpp>
#include <boost/math/tools/roots.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/svg_plot/svg_2d_plot.hpp>
#include <boost/svg_plot/show_2d_settings.hpp>
// Only needed for showing which settings in use.

#include <list>
#include <map>
#include <string>


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
   } // add

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
      plot.image_size(600, 400);
      plot.copyright_holder("John Maddock").copyright_date("2008").boost_license_on(true);
      plot.coord_precision(4); // Ciould be 3 for smaller plots.
      plot.title(title).legend_title_font_size(15).title_on(true);
      plot.legend_on(m_has_legend);
      //plot.legend_place(outside_right);

      plot
        .x_major_value_labels_side(-1).y_major_value_labels_side(-1) // left and bottom
        .x_label_units_on(false).y_label_units_on(false);
      double x_delta = (m_max_x - m_min_x) / 50;
      double y_delta = (m_max_y - m_min_y) / 50;
      plot.x_range(m_min_x - x_delta, m_max_x + x_delta)
          .y_range(m_min_y - y_delta, m_max_y + y_delta);
      plot.x_label_on(true).x_label(x_lable);
      plot.y_label_on(true).y_label(y_lable);
      plot.y_major_grid_on(false).x_major_grid_on(false);
      plot.x_num_minor_ticks(3);
      plot.y_num_minor_ticks(3);
      //
      // Work out axis tick intervals:
      double l = std::floor(std::log10((m_max_x - m_min_x) / 10) + 0.5);
      double interval = std::pow(10.0, (int)l);
      if(((m_max_x - m_min_x) / interval) > 10)
      {
         interval *= 5;
      }
      plot.x_major_interval(interval);
      l = std::floor(std::log10((m_max_y - m_min_y) / 10) + 0.5);
      interval = std::pow(10.0, (int)l);
      if(((m_max_y - m_min_y) / interval) > 10)
      {
         interval *= 5;
      }
      plot.y_major_interval(interval);
      plot.plot_window_on(true).plot_border_color(floralwhite); // 

      int color_index = 0;

      for(std::list<std::pair<std::string, std::map<double,double> > >::const_iterator i = m_points.begin();
         i != m_points.end(); ++i)
      {
         	plot.plot(i->second, i->first)
            .shape(none)
            .line_on(true)
            //.bezier(true)
            .line_color(colors[color_index]);

         if(i->first.size())
         {
            ++color_index;
         }
         color_index = color_index % (sizeof(colors)/sizeof(colors[0]));
      }
      plot.write(file);
      show_plot_settings(plot);
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

struct location_finder
{
   location_finder(double(*_f)(double), double t, double x0) : f(_f), target(t), x_off(x0){}

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
   double(*f)(double);
   double target;
   double x_off;
};

double find_end_point(double (*f)(double), double x0, double target, bool rising, double x_off = 0)
{
   boost::math::tools::eps_tolerance<double> tol(50);
   boost::uintmax_t max_iter = 1000;
   return boost::math::tools::bracket_and_solve_root(
      location_finder(f, target, x_off), 
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
   
   f = boost::math::zeta;
   plot.add(f, 1.01, 20, "");
   plot.add(f, -20, 0.99, "");
   plot.plot("Zeta Function Over [-20,20]", "zeta1.svg", "z", "zeta(z)");

   plot.clear();
   plot.add(f, -14, 0, "");
   plot.plot("Zeta Function Over [-14,0.5]", "zeta2.svg", "z", "zeta(z)");

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

   /*
   std::list<std::pair<double, double> > circle;
   for(double x = -2; x <= 2; x += 0.1)
   {
      circle.push_back(std::make_pair(x, -sqrt(4 - x * x)));
   }
   for(double x = 2; x >= -2; x -= 0.1)
   {
      circle.push_back(std::make_pair(x, sqrt(4 - x * x)));
   }

   boost::svg::svg_2d_plot surface;
   surface.image_size(600, 400);
   surface.title_font_size(20);
   surface.legend_title_font_size(15);
   surface.title("Circle");
   surface.legend_on(false).title_on(true);
   surface.x_major_value_labels_side(true).y_major_value_labels_side(true);
   surface.x_range(-2.2, 2.2)
      .y_range(-2.2, 2.2);
   surface.x_label_on(true).x_label("x");
   surface.y_label_on(true).y_label("y");
   surface.y_major_grid_on(false).x_major_grid_on(false);
   surface.x_num_minor_ticks(3);
   surface.y_num_minor_ticks(3);
   surface.x_major_interval(0.5);
   surface.y_major_interval(0.5);
   surface.plot_window_on(false);


   using namespace boost::svg;

   surface.plot(circle, "",
         //_bezier_on = true,
         _line_on = true,
         _point_style = none);
   surface.write("circle.svg");
   */

   return 0;
}
