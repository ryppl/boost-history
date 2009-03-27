/*! \file demo_1d_uncertainty.cpp
    \brief Simple plot of uncertainty of 1D data.
    \details An example to demonstrate simple 1D plot using two vectors,
     including showing values with uncertainty information as 'plus minus' and degrees of freedom estimates.
    \author Paul A. Bristow
    \date Mar 2009
*/

// Copyright Paul A Bristow 2009

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.
// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

//[demo_1d_uncertainty_1

/*`First we need a few includes to use Boost.Plot:
*/
#include <boost/svg_plot/uncertain.hpp>
 // using boost::svg::unc; // Holds value and uncertainty formation.
#include <boost/svg_plot/detail/functors.hpp>
//  using boost::svg::detail::unc_1d_convert;
#include <boost/svg_plot/svg_1d_plot.hpp>
  using namespace boost::svg;

#include <iostream>
  using std::cout;
  using std::endl;
#include <vector>
  using std::vector;
#include <iterator>
  using std::ostream_iterator;
#include <algorithm>
  using std::copy;

//] [/demo_1d_uncertainty_1]

int main()
{
//[demo_1d_uncertainty_2

/*`STL vector is used as the container for our two data series,
and values are inserted using push_back.  Since this is a 1-D plot
the order of data values is not important.
*/
  vector<unc> A_times;
  A_times.push_back(unc(3.1, 0.02F, 8));
  A_times.push_back(unc(4.2, 0.01F, 14));

  vector<unc> B_times;
  B_times.push_back(unc(2.1, 0.001F, 30, 0xFF)); // Value, uncertainty, degrees of freedom and type known.
  // (But use of type is not yet implemented.)
  B_times.push_back(unc(5.1, 0.025F, 20)); // Value, uncertainty, and degrees of freedom known - the usual case.
  B_times.push_back(unc(7.8, 0.0025F)); // Value and uncertainty known, but not degrees of freedom.
  B_times.push_back(unc(3.4, 0.03F)); // Value and uncertainty known, but not degrees of freedom.
  B_times.push_back(unc(6.9)); // Only value known - no information available about uncertainty.

  vector<unc> C_times;
  C_times.push_back(unc(0.6, 0.1F, 5));
  C_times.push_back(unc(1.4, 0.2F, 11));

  /*`Echo the values input: */
  copy(A_times.begin(), A_times.end(), ostream_iterator<unc>(std::cout, " "));
  cout << endl;
  copy(B_times.begin(), B_times.end(), ostream_iterator<unc>(std::cout, " "));
  cout << endl;
 /*`The constructor initializes a new 1D plot, called `my_plot`, and also sets all the very many defaults for axes, width, colors, etc. 
*/
  svg_1d_plot my_plot;
/*`A few (member) functions that set are fairly self-explanatory:

* title provides a title at the top for the whole plot,
* `legend_on(true)` will mean that titles of data series and markers will display in the legend box.
* `x_range(-1, 11)` sets the axis limits from -1 to +11 (instead of the default -10 to +10).
* `background_border_color(blue)` sets just one of the very many options.
*/
    my_plot.autoscale_check_limits(false); // default is true.
    my_plot.autoscale_plusminus(3); // default is 3.


  my_plot
    .plot_window_on(true)
    .background_border_color(blue)
    .plot_border_color(yellow)
    .plot_border_width(1)
    //.x_ticks_on_window_or_axis(0) // now the default
    .legend_on(false)
    .title("Times")
    .x_range(0, 10)
    .x_label("times (sec)")
    .x_values_on(true)
    .x_values_precision(2)
    .x_values_rotation(slopeup)
    .x_plusminus_color(blue)
    .x_plusminus_on(true)
    .x_df_on(true)
    .x_df_color(green)
    .x_autoscale(A_times)
    ;
/*`
Then we add our data series,
and add optional data series titles
(very helpful if we want them to show on the legend).

The A_times mark data points with a red border circle with a green fill,
The B_times use a blue vertical line,
while C_times use an ellipse whose width (x radius) is from the uncertainty.
All the data points are also labelled with their value,
and uncertainty and degrees of freedom if known.

*/

  my_plot.plot(A_times, "A").shape(round).size(10).stroke_color(red).fill_color(green);
  my_plot.plot(B_times, "B").shape(vertical_line).stroke_color(blue);
  my_plot.plot(C_times, "C").shape(unc_ellipse).fill_color(lightyellow).stroke_color(magenta);
     
/*`Finally, we can write the SVG to a file of our choice.
*/
  my_plot.write("./demo_1d_uncertainty.svg");
//] [/demo_1d_uncertainty_2]

  return 0;
} // int main()

/*

Output:
//[demo_1d_uncertainty_output
demo_1d_uncertainty.cpp
Linking...
Embedding manifest...
Autorun j:\Cpp\SVG\debug\demo_1d_uncertainty.exe
3.1±0.02 (8)  4.2±0.01 (14)  
2.1±0.001 (30)  7.8±0.0025 (21)  3.4±0.03 6.9 
Build Time 0:03
//] [/demo_1d_uncertainty_output]
*/

/*
Data for another 1D example, also use Boost.Math?

The data is taken from J. C. Banford el al, Analyst (1983) 107, 195
and compares the concentration of thiol of the lysate of the blood of two groups
of volunteers as control and a second suffering from rhematoid arthritis.

  map<pair<unc, unc> > control;
  control.push_back(unc(1.921, 0.076F, 7));
  map<pair<unc, unc> > rheumatoid;
  control.push_back(unc(3.456, 0.44F, 6));

  */
