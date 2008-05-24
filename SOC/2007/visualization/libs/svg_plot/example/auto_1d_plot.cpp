// auto_1d_plot.cpp

// Copyright Jacob Voytko 2007
// Copyright Paul A Bristow 2008

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// An example to demonstrate simple 1D settings, including *auto-scaling*.
// See auto_1d_containers.cpp for an example autoscaling with multiple data series.
// See also demo_1d_plot.cpp for a wider range of use.

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.

// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!


//[auto_1d_plot_1

/*`First we need a few includes to use Boost.Plot
(and some others only needed for this example).
*/

#include <boost/svg_plot/svg_1d_plot.hpp>
  using namespace boost::svg;
  using boost::svg::svg_1d_plot;

#include <boost/svg_plot/show_1d_settings.hpp>
// Only needed for showing which settings in use.
 void boost::svg::show_1d_plot_settings(svg_1d_plot&);
 // (Also provides operator<< for std::pair).

#include <boost/algorithm/minmax.hpp>
 using boost::minmax;
#include <boost/algorithm/minmax_element.hpp>
 using boost::minmax_element;

#include <iostream> // for debugging.
  using std::cout;
  using std::endl;
  using std::boolalpha;

#include <limits>
  using std::numeric_limits;

#include <vector>
  using std::vector;
#include <set>
  using std::multiset;

#include <utility>
  using std::pair;

#include <boost/svg_plot/detail/auto_axes.hpp>
  using boost::svg::show; // A single STL container.
  using boost::svg::show_all; // Multiple STL containers.
  using boost::svg::range; // Find min and max of a STL container.
  using boost::svg::range_all;// Find min and max of multipler STL containers.

//] [/auto_1d_plot_1]

void scale_axis(double min_value, double max_value, // input
               double* axis_min_value,  double* axis_max_value, double* axis_tick_increment, // updated.
               bool origin, double tight, int min_ticks, int steps); 

double tol100eps = 1000 * numeric_limits<double>::epsilon(); // suitable tight value.

int main()
{
  //[auto_1d_plot_2
  /*`This example uses containers to demonstrate autoscaling.
  Autoscaling must inspect the container in order to find axis ranges that will be suitable.
  First we create a container and fill with some fictional data.
  */
	vector<double> my_data;
	// Initialize my_data with some entirely fictional data.
  my_data.push_back(0.2);
  my_data.push_back(1.1); // [1]
  my_data.push_back(4.2); // [2]
  my_data.push_back(3.3); // [3]
  my_data.push_back(5.4); // [4]
  my_data.push_back(6.5); // [5]

  /*`Also included is an 'at limit' value that could confuse autoscaling.
  Obviously we do not want the plot range to include infinity.*/
  my_data.push_back(numeric_limits<double>::infinity()); // [6]

  try
  { // Ensure error, warning and information messages are displayed by the catch block.
    double mn;
    double mx;
    int good = mnmx(my_data.begin(), my_data.end(), &mn, &mx);
    cout << good << " good values, " << my_data.size() - good << " limit values." << endl;

    using boost::svg::detail::operator<<; // For displaying std::pair.
    svg_1d_plot my_1d_plot; // Construct a plot with all the default constructor values.
    my_1d_plot.x_autoscale(my_data);  // Compute autoscale values for the plot.
    my_1d_plot.plot(my_data, "Default 1D"); // Add the one data series, and give it a title.
    my_1d_plot.write("auto_1d_plot.svg"); // Write the plot to file.

    /*`It may be useful to display that range chosen by autoscaling. */
    cout << "x_range() " << my_1d_plot.x_range() << endl; // x_range() 
  }
  catch(const std::exception& e)
  { // Error, warning and information messages are displayed by the catch block.
    std::cout <<
      "\n""Message from thrown exception was:\n   " << e.what() << std::endl;
  }
  //] [/auto_1d_plot_2]

  //[auto_1d_plot_3
/*`Other STL containers can also be used, for example set, or multiset (allowing duplicates):*/

  try
  { // Ensure error, warning and information messages are displayed by the catch block.

  multiset<double> my_set;
	// Initialize my_set with some entirely fictional data.
  my_set.insert(1.2);
  my_set.insert(2.3);
  my_set.insert(3.4);
  my_set.insert(4.5);
  my_set.insert(5.6);
  my_set.insert(6.7);
  my_set.insert(7.8);
  my_set.insert(8.9);
  // Some examples of showing the set using various iterators:
  multiset<double>::const_iterator si;

  show(my_data); // Entire container contents,
  show(my_set); // for two different types of container.

  // show(&my_data[0], &my_data[my_data.size()]); // pointers - wrong! uses more all data ;-)
  show(&my_data[0], &my_data[my_data.size()-1]); // pointers, all data.
  show(&my_data[1], &my_data[5]); // pointers, part data.
  show(my_data.begin(), my_data.end()); // iterators.
  show(++(my_data.begin()), --(my_data.end())); // Just the 4 middle values.

  vector<double>::const_iterator idb = my_data.begin();
  vector<double>::const_iterator ide = my_data.end();
  show(idb, ide); // All
  ++idb; // Move to 2nd value.
  --ide; // move back from last value.
  show(idb, ide); // Just the 4 middle values.

  typedef vector<double>::const_iterator vector_iterator;
  pair<vector_iterator, vector_iterator> result = boost::minmax_element(my_data.begin(), my_data.end());
  cout << "The smallest element is " << *(result.first) << endl; // 0.2
  cout << "The largest element is  " << *(result.second) << endl; // 6.5

/*`Autoscaling can also use two double min and max values provided by the user program.
Using `x_autoscale(my_set)` effectively uses the first and last items in the STL container.
If the container is sorted, then these are the minimum and maximum values.*/

  double min_value = *(my_data.begin());
  double max_value = *(--my_data.end());
  cout << "my_set min " << min_value << ", max = " << max_value << endl;

/*`Function scale_axis is used by autoscale, but is also available for use direct by the user.
It accepts parameters controlling the scaling and updates 4 items. Its signature is:

  ``void scale_axis(double min_value, double max_value, // Input range
               double* axis_min_value,  double* axis_max_value, double* axis_tick_increment, int* auto_ticks, // All 4 updated.
               bool check_limits, // Whether to check all values for infinity, NaN etc.
               bool origin, // If true, ensures that zero is a tick value.
               double tight, // Allows user to avoid a small fraction over a tick using another tick.
               int min_ticks, // Minimum number of ticks.
               int steps); // Round up and down to 2, 4, 6, 8, 10, or 5, 10 or 2, 5, 10 systems.
  ``

  */
  double axis_min_value; // Values to be updated by autoscale.
  double axis_max_value;
  double axis_tick_increment;
  int axis_ticks;

  /*`min_value` and `max_value` could be provided by the user program:
  usually these values are derived in some way from the user data.
  Several examples follow.*/

  // Scaling using first and last values in container,
  // assuming the data are ordered in ascending value,
  // for example, set, map, multimap, or a sorted vector or array.
  //scale_axis(*my_data.begin(),*(--my_data.end()),
  scale_axis(1., 9.,
    &axis_min_value, &axis_max_value, &axis_tick_increment, &axis_ticks,
    false, tol100eps, 6, 0); // Display range.
  cout << "scaled min " << axis_min_value << ", max = " << axis_max_value
    << ", increment " << axis_tick_increment << ", axis ticks " << axis_ticks << endl;

  // Scaling using two begin & end iterators into STL container,
  // scale_axis does finding min and max.
  scale_axis(my_data.begin(), my_data.end(),
    &axis_min_value, &axis_max_value, &axis_tick_increment, &axis_ticks,
    true, false, tol100eps, 6); // Display range.
  cout << "scaled min " << axis_min_value << ", max = " << axis_max_value
    << ", increment " << axis_tick_increment << ", axis ticks " << axis_ticks << endl;

  // Scaling using two begin & end iterators into STL container,
  // scale_axis does finding min and max.
  scale_axis(my_data[1], my_data[4], // Only middle part of the container used, ignoring 1st and last values.
    &axis_min_value, &axis_max_value, &axis_tick_increment, &axis_ticks,
    false, tol100eps, 6); // Display range.
  cout << "scaled min " << axis_min_value << ", max = " << axis_max_value
    << ", increment " << axis_tick_increment << ", axis ticks " << axis_ticks << endl;

  // Scaling using whole  STL vector container,
  // scale_axis does finding min and max.
  scale_axis(my_data, &axis_min_value, &axis_max_value, &axis_tick_increment, &axis_ticks,
    true, false, tol100eps, 6); // Display range.
  cout << "scaled min " << axis_min_value << ", max = " << axis_max_value
    << ", increment " << axis_tick_increment << ", axis ticks " << axis_ticks << endl;
  
  // Scaling using whole STL set container,
  // scale_axis does finding min and max.
  scale_axis(my_set, &axis_min_value, &axis_max_value, &axis_tick_increment, &axis_ticks,
    true, false, tol100eps, 6); // Display range.
  cout << "scaled min " << axis_min_value << ", max = " << axis_max_value
    << ", increment " << axis_tick_increment << ", axis ticks " << axis_ticks << endl;

  /*`However there may be trouble if the data could contain values that are outside normal limits.
  Infinity (+ and -), and maximum value, and  NotaNumbers, are separated by the plot program to allow them to be shown,
  but separate from 'normal' values.  These values similarly can distort automatic scaling:
  a single infinity would result in useless scaling!
  When the plot range is set, the maximum and minimum values are checked,
  and if not normal then an exception will be thrown, and no plot will be produced.
  However, when autoscaling, it is more useful to ignore 'limit' values.
  But this means checking all values individually.  If it known that all values are normal,
  for example because they come from some measuring equipment that is known only to produce normal values,
  it will be much quicker to use std::min_max_element which can take advantange of knowledge of the container.

  The function `autoscale_check_limits(bool)` is provided to control this. 
  If set true, all values will be checked, and those at 'limits' will be ignored in autoscaling.
  The default is true, to check all values.

  If we had many very known normal values to plot and want to autoscale,
  we might instead opt to ignore these checks, and write:
  */
	svg_1d_plot my_1d_plot; // Construct a plot with all the default constructor values.

  //my_1d_plot.autoscale_check_limits(false);
  // This *will throw an exception* if checks are avoided and any values are at 'limits'.

  // One could also intercept and change any values calculated by scale_axis here.
  // Set the plot to use range and interval from the scale_axis values.
  my_1d_plot.x_range(axis_min_value, axis_max_value)
    .x_major_interval(axis_tick_increment);

  my_1d_plot.x_autoscale(false);  // Ensure autoscale values are *not* recalculated for the plot.

/*`There are also some parameters which can fine-tune the autoscaling to produce more
aesthetically pleasing ranges. One can:
* enforce the inclusion of zero on the plot.
* Specify a minimum number of major ticks.
* Specify the steps between major ticks, default 0,  or 2 for 2, 4, 6, 8, 10, 5 for 1, 5, 10, or 10 (2, 5, 10).
* Avoid values that are a tiny amount over the minimum or maximum from causing an apparently empty tick at the minimum or maximum.
*/

  // Set some autoscaling parameters:
  my_1d_plot.x_with_zero(false);
  my_1d_plot.x_min_ticks(10);
  my_1d_plot.x_steps(0);
  my_1d_plot.x_tight(0.001);

  // Show the flags just set.
  cout << (my_1d_plot.x_with_zero() ? "x_with_zero, " : "not x_with_zero, ")
    << my_1d_plot.x_min_ticks() << " x_min_ticks, "
    << my_1d_plot.x_steps() << " x_steps, " 
    << my_1d_plot.x_tight() << " tightness." << endl;

/*`Finally here are some examples of using autoscaling using all or part of containers.
*/

  my_1d_plot.x_autoscale(my_data);  // Use all my_data to autoscale.
  cout << "Autoscaled " // Show the results of autoscale:
    "min " << my_1d_plot.x_auto_min_value() 
    << ", max "<< my_1d_plot.x_auto_max_value() 
    << ", interval " << my_1d_plot.x_auto_tick_interval() << endl; // Autoscaled min 0, max 6.5, interval 0.5

  my_1d_plot.x_autoscale(my_data.begin(), my_data.end());  // Use all my_data to autoscale.

  cout << "Autoscaled " // Show the results of autoscale:
    "min " << my_1d_plot.x_auto_min_value() 
    << ", max "<< my_1d_plot.x_auto_max_value() 
    << ", interval " << my_1d_plot.x_auto_tick_interval() << endl; // Autoscaled min 0, max 6.5, interval 0.5

  my_1d_plot.x_autoscale(my_data[1], my_data[4]);  // Use only part of my_data to autoscale.

  cout << "Autoscaled " // Show the results of autoscale:
    "min " << my_1d_plot.x_auto_min_value() 
    << ", max "<< my_1d_plot.x_auto_max_value() 
    << ", interval " << my_1d_plot.x_auto_tick_interval() << endl; // Autoscaled min 1, max 5.5, interval 0.5

  //my_1d_plot.x_autoscale(true);  // Ensure autoscale values are used for the plot.
  // This is also automatically set true by any call of `x_autoscale(some_data);`


/*`The actual addition of data values to the plot is, of course, quite separate from any autoscaling.
*/
  my_1d_plot.plot(my_data, "Auto 1D"); // Add the one data series.
  cout << "Autoscaled " // Show the results of autoscale:
    " min " << my_1d_plot.x_auto_min_value() 
    << ", max "<< my_1d_plot.x_auto_max_value() 
    << ", interval " << my_1d_plot.x_auto_tick_interval() << endl; // Autoscaled min 1, max 5.5, interval 0.5

  my_1d_plot.plot(my_set.begin(), my_set.end(), "Auto 1D"); // Add another data series from my_set.
  my_1d_plot.plot(my_set, "Auto 1D"); // Add another whole data series from my_set.
  my_1d_plot.plot(&my_data[1], &my_data[4], "Auto 1D"); // Add part (1,2 3 but *not* 4) of the one data series.
  //my_1d_plot.plot(&my_set[1], &my_set[4], "Auto 1D"); // operator[] is not defined for set container!

  my_1d_plot.write("auto_1d_plot.svg"); // Write the plot to file.
  
  using boost::svg::detail::operator<<;
  cout << "x_range() " << my_1d_plot.x_range() << endl; // x_range() 1, 5.5
  show_1d_plot_settings(my_1d_plot);
  }
  catch(const std::exception& e)
  { // Error, warning and information messages are displayed by the catch block.
    std::cout <<
      "\n""Message from thrown exception was:\n   " << e.what() << std::endl;
  }

/*`If necessary, one can obtain a complete listing of all the settings used.
This is often useful when the plot does not meet expectations.
*/
 
//] [auto_1d_plot_3]

	return 0;
} // int main()

/*

auto_1d_plot.cpp
Linking...
Embedding manifest...
Autorun "j:\Cpp\SVG\debug\auto_1d_plot.exe"
limit value: 1.#INF
6 good values, 1 limit values.
7 values in container: 0.2 1.1 4.2 3.3 5.4 6.5 1.#INF 
8 values in container: 1.2 2.3 3.4 4.5 5.6 6.7 7.8 8.9 
0.2 1.1 4.2 3.3 5.4 6.5 : 6 values used.
1.1 4.2 3.3 5.4 : 4 values used.
0.2 1.1 4.2 3.3 5.4 6.5 1.#INF : 7 values used.
1.1 4.2 3.3 5.4 6.5 : 5 values used.
0.2 1.1 4.2 3.3 5.4 6.5 1.#INF : 7 values used.
1.1 4.2 3.3 5.4 6.5 : 5 values used.
The smallest element is 0.2
The largest element is  1.#INF
my_set min 0.2, max = 1.#INF
scaled min 1, max = 9, increment 1, axis ticks 9
limit value: 1.#INF
scaled min 0, max = 7, increment 1, axis ticks 8
scaled min 1, max = 6, increment 1, axis ticks 6
limit value: 1.#INF
scaled min 0, max = 7, increment 1, axis ticks 8
scaled min 1, max = 9, increment 1, axis ticks 9
not x_with_zero, 10 x_min_ticks, 0 x_steps, 0.001 tightness.
limit value: 1.#INF
Autoscaled min 0, max 6.5, interval 0.5
limit value: 1.#INF
Autoscaled min 0, max 6.5, interval 0.5
Autoscaled min 1, max 5.5, interval 0.5
Autoscaled  min 1, max 5.5, interval 0.5
x_range() 1, 5.5
Build Time 0:03


*/

