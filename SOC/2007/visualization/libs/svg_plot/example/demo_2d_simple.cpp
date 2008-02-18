// demoe_2d_plot.cpp

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2007

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// -----------------------------------------------------------------

// This demonstrates plotting some math functions with most of the 2-D defaults,
// just chaning a few typical details.
// See default_2d_plot.cpp for using *all* defaults.
// See also demo_2d_plot.cpp for use of some of the very many options.

#ifdef _MSC_VER
#  pragma warning (disable : 4800) // forcing value to bool 'true' or 'false' (performance warning)
#  pragma warning (disable : 4180) // qualifier applied to function type has no meaning; ignored
//#  pragma warning (disable : 4172) // returning address of local variable or temporary
#  pragma warning (disable : 4512) // assignment operator could not be generated
#  pragma warning (disable : 4503) // decorated name length exceeded, name was truncated
#  pragma warning (disable : 4180) // qualifier applied to function type has no meaning; ignored
#endif

#include <boost/svg_plot/svg_2d_plot.hpp>
  using boost::svg::svg_2d_plot;

#include <boost/svg_plot/show_2d_settings.hpp>
// Only needed for showing which settings in use.

#include <iostream>
  using std::cout;
  using std::endl;
  using std::boolalpha;
#include <map>
  using std::map;
#include <cmath>
  using std::sqrt;
#include <string>
  using std::string;

using namespace boost::svg;
// Will be *very convenient* if using any SVG named colors,
// to avoid writing
  using boost::svg::red;
// for every color used.

double f(double x)
{
  return sqrt(x);
}

double g(double x)
{
  return -2 + x*x;
}

double h(double x)
{
  return -1 + 2 * x;
}

int main()
{

  // Some containers for (sorted) sample data.
  map<double, double> data1;
  map<double, double> data2;
  map<double, double> data3;

  for(double i = -5; i <= 10.; i += 1.)
  { // Several data points for each function.
    data1[i] = f(i);
    data2[i] = g(i);
    data3[i] = h(i);
    // List if desired:
    // cout << i << ' '<< data1[i] << ' ' << data2[i] << ' '<< data3[i] << endl;
  }

  svg_2d_plot my_plot;
  // Uses most defaults.

  // Scale settings are often sensible.
  // (Until autoscale is available).

  // Add the data series to the plot:
  my_plot.title("demo_2d_simple");
  my_plot.title(" title");
  cout << " my_plot.title() " << my_plot.title() << endl;
  my_plot.x_label("X-axis");
  my_plot.y_label("Y-axis");

  std::string s = my_plot.title();

  my_plot.plot(data1, "Sqrt(x)").fill_color(red);
	my_plot.plot(data2, "-2 + x^2").fill_color(orange).size(5);
	my_plot.plot(data3, "-1 + 2x").fill_color(yellow).bezier_on(true).line_color(blue).shape(square);
  cout << " my_plot.title() " << my_plot.title() << endl;

  my_plot.write("./demo_2d_simple.svg");
  cout << " my_plot.title() " << my_plot.title() << endl;

  show_plot_settings(my_plot);

  return 0;
} // int main()
