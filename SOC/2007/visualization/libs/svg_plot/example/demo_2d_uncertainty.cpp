/*! \file demo_2d_uncertainty.cpp
    \brief Demonstration of some 2D plot features.
    \details including showing values with uncertainty information as 'plus minus' and degrees of freedom estimates.
    \author Jacob Voytko and Paul A. Bristow
    \date Mar 2009
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2007, 2008, 2009

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.
// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

#ifdef _MSC_VER
#  pragma warning (disable : 4800) // forcing value to bool 'true' or 'false' (performance warning)
#  pragma warning (disable : 4180) // qualifier applied to function type has no meaning; ignored
#  pragma warning (disable : 4503) // decorated name length exceeded, name was truncated
#  pragma warning (disable : 4512) // assignment operator could not be generated
#  pragma warning (disable : 4100) // unreferenced formal parameter
#endif

//[demo_2d_uncertainty_1

/*`First we need some includes to use Boost.Plot and C++ Standard Library:
*/#include <boost/svg_plot/svg_2d_plot.hpp>
using namespace boost::svg;

#include <boost/svg_plot/show_2d_settings.hpp>
// using boost::svg::show_2d_plot_settings - Only needed for showing which settings in use.
#include <boost/svg_plot/uncertain.hpp>
using boost::svg::unc;
#include <boost/svg_plot/detail/pair.hpp>
// using boost::svg::detail::pair; operator<<

//#include <algorithm>
//#include <functional>

#include <map>
  using std::map;
  using std::multimap;

#include <utility>
  using std::pair;
  using std::make_pair;

#include <vector>
   using std::vector;

#include <cmath>
   using std::sqrt;
   using std::pair;
#include <iostream>
   using std::cout;
   using std::endl;
   using std::scientific;
   using std::hex;
   using std::ios;
   using std::boolalpha;

#include <iterator>
   using std::ostream_iterator;

//] [/demo_2d_uncertainty_1]

  enum side
  { //! \enum side Where axis labels go.
    left_side = -1,
    on_axis = 0,
    right_side = +1,
    bottom_side = -1,
    top_side = +1,
  };

int main()
{
  try
  {
//[demo_2d_uncertainty_2

/*`STL map is used as the container for our two data series,
and pairs of values and their uncertainty information
(approximately standard deviation and degrees of freedom)
are inserted using push_back.  Since this is a 2-D plot
the order of data values is important.
*/

  unc u1(1.23, 0.56F, 7); // For an X value.
  cout << "u1 = " << u1 << endl; // u1 = 1.23+-0.056 (7)
  unc u2(3.45, 0.67F, 9); // For a Y value.
  pair<unc, unc> mp1 = make_pair(u1, u2); // XY pair of values.
  cout << mp1 << endl; // 1.23+-0.056 (7), 2.345+-0.067 (9)

  map<unc, unc> data1; // Container for XY points.
  data1.insert(mp1); // u1, u2 = 1.23+-0.056 (7), 2.345+-0.067 (9)
  data1.insert(make_pair(unc(4.1, 0.4F, 7), unc(3.1, 0.3F, 18))); // 
  data1.insert(make_pair(unc(-2.234, 0.03F, 7), unc(-8.76, 0.9F, 9)));

  /*`Make very sure you don't forget either unc() like this
   `data1.insert(make_pair((-2.234, 0.12F, 7),(-8.76, 0.56F, 9)));`
   because, by the bizarre operation of the comma operator, the result will be an integer!
   So you will astonished to find that the values will be the pair of \b degrees of freedom, (7, 9)
   and the other parts of unc will be undefined!/n

   Echo the values input: */
  cout << data1.size() << " XY data pairs:" << endl;
  copy(data1.begin(), data1.end(), ostream_iterator<pair<unc, unc> >(cout, "\n"));
  cout << endl;

  svg_2d_plot my_plot;
  /*`if you can be confident taht the data set only contains normal, valid data,
  so none are 'at limits' too big or too small to be meaningful, infinite or NaN (NotANumber)
  then these checks can be skipped (for speed).
  An instrument or operator input might provide only normal data.
  For this example, we know this is true, so override the default autoscale_check_limits(true).
  */
  //my_plot.autoscale_check_limits(false);
  /*`The default is autoscale_plusminus(3.) so that confidence ellipses
  at 1, 2 and 3 (uncertainty nominally standard deviations)
  are all within the plot window, 
  but if you are less interested in seeing the 2 and 3 ellipses,
  you could risk the outer edges spilling over the borders
  by reducing autoscale_plusminus, for example, to 1.5, down to zero.*/
  my_plot.autoscale_plusminus(1.5); // default is 3.

  my_plot.autoscale(true);

  /*`Use data set `data` to autoscale (you can use a different data set to scale from the one you chose to plot).
  */
  my_plot.xy_autoscale(data1);
  my_plot
    .x_label("times (sec)")
    .xy_values_on(true)
    // Note use of Unicode space in all string - ANSI space has no effect.
    //.x_decor("", ",&#x00A0;") // Keep all on one line using spearator starting with a comma.
    .x_decor("","") // Split onto two lines because X separator does not start with comma.
    .y_decor("&#x00A0;&#x00A0;&#x00A0;", "time = ", "&#x00A0;sec")
    // Note a few padding spaces used to get Y values to lie more nearly under X values.
    // This is only necessary when label are not horizontal.
    .x_values_rotation(slopeup)
    .x_values_font_size(16)
    .x_plusminus_on(true)
    .x_plusminus_color(cyan)
    .x_df_on(true)
    .x_df_color(magenta)

    .y_values_rotation(uphill)
    .x_values_font_family("sans")
    .y_plusminus_on(true)
    .y_plusminus_color(red)
    .y_df_on(true)
    .y_df_color(green)
  /*`The default uncertainty ellipse colors (that apply to both x and y axes)
  can be changed thus:
  */
    .one_sd_color(lightblue)
    .two_sd_color(svg_color(200, 230, 255))//  	rgb(176, 224, 230)
    .three_sd_color(svg_color(230, 255, 255)) // rgb(240, 255, 255)
   ;

  my_plot.plot(data1, "data1").shape(unc_ellipse);

  my_plot.write("./demo_2d_uncertainty");

  //show_2d_plot_settings(my_plot);

  //] [/demo_2d_uncertainty_2]
  
  }
  catch (const std::exception& e)
  {
    std::cout <<
      "\n""Message from thrown exception was:\n   " << e.what() << std::endl;
  }
  return 0;
} // int main()

/*

Output :



*/
