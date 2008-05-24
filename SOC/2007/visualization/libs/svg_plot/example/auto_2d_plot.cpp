// auto_2d_plot.cpp

// Copyright Paul A Bristow 2008

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// An example to demonstrate autoscaling with *multiple* STL containers.
// See also auto_1d_plot.cpp and auto_1d_container.cpp.

// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

//[auto_2d_plot_1

/*`First we need a few includes to use Boost.Plot
*/

#include <boost/svg_plot/svg_2d_plot.hpp>
  using namespace boost::svg;
#include <utility>
  using std::pair;
#include <map>
  using std::map;
#include <set>
  using std::multiset;
#include <iostream>
  using std::cout;
  using std::endl;

#include <limits>
  using std::numeric_limits;

#include <boost\math\special_functions\fpclassify.hpp>
  using boost::math::isfinite;

  // demo of getting the max and min of x and y
template <class T> // T an STL container: array, vector ...
void s(const T& container, // Container Data series to plot - entire container.
               // (not necessarily ordered, so will find min and max).
               double* x_min,  double* x_max,
               double* y_min,  double* y_max
               )
{
  typedef T::const_iterator iter;
  std::pair<iter, iter> result = boost::minmax_element(container.begin(), container.end());
  // minmax_element is efficient for maps because can use knowledge of being sorted,
  // BUT only if it can be assumed that no values are 'at limits',
  // infinity, NaN, max_value, min_value, denorm_min.
  // Otherwise it is necessary to inspect all values individually.
  pair<const double, double> px = *result.first;
  pair<const double, double> py = *result.second;
  *x_min = px.first;
  *x_max = py.first;
  *y_min = px.second;
  *y_max = py.second;

  cout << "s x_min " << *x_min << ", x_max " << *x_max << endl; // x_min 1, x_max 7.3
  cout << "s y_min " << *y_min << ", y_max " << *y_max << endl; // y_min 3.2, y_max 9.1
} // template <class T> int scale_axis  T an STL container: array, vector ...


//] [/auto_2d_plot_1]

int main()
{
//[auto_2d_plot_2
  /*`This example uses a single map to demonstrate autoscaling.
  We create a map to hold our data series.
  */
	map<const double, double> my_map;
  /*`
  Inserting some fictional values also sorts the data.
  The index value in [] is the x value.
  */
	my_map[1.1] = 3.2;
	my_map[7.3] = 9.1;
	my_map[2.1] = 5.4;

/*`Also include some 'limits' values that would confuse autoscaling.
*/
	my_map[99.99] = numeric_limits<double>::quiet_NaN();
	my_map[999.9] = numeric_limits<double>::infinity();
	my_map[999.] = +numeric_limits<double>::infinity();



  /*`Next a 2D plot is created using defaults for the very many possible settings.
  */
	svg_2d_plot my_plot;

  /*`Add at least a title,
  specify the both x and y axes are to use autoscaling,
  and add the one data series to plotted.
  */
	my_plot.title("Autoscale example");
  //my_plot.autoscale_check_limits(false);  // Skip checks for speed.
  // Will fail at run-time if any infinite or NaNs.

	my_plot.xy_autoscale(my_map);

	my_plot.plot(my_map);
  /*`Finally write the SVG image to a file. */
  my_plot.write("./auto_2d_plot.svg");

  cout << "X min " << my_plot.x_range().first <<  ", X max " << my_plot.x_range().second << endl; 
  cout << "Y min " << my_plot.y_range().first << ", Y max "  << my_plot.y_range().second << endl; 

//] [/auto_2d_plot_2]

	return 0;
}

/*
//[auto_2d_plot_output

Output:

auto_2d_plot.cpp
Linking...
Embedding manifest...
Autorun "j:\Cpp\SVG\debug\auto_2d_plot.exe"
2 goods, 2.1 5.4
3 goods, 7.3 9.1
limit value: 99.99 1.#QNAN
limit value: 999 1.#INF
limit value: 999.9 1.#INF
x_min 1.1, x_max 7.3
y_min 3.2, y_max 9.1
limits 3
1 8
3 10
Build Time 0:21


Linking...
Embedding manifest...
Autorun "j:\Cpp\SVG\debug\auto_2d_plot.exe"
X min 1, X max 8
Y min 3, Y max 10
Build Time 0:03


//] [/auto_2d_plot_output]


Parked pro tem.


// double tol100eps = 1000 * numeric_limits<double>::epsilon(); // suitable tight value.

 // double axis_min_value; // Values to be updated by autoscale.
 // double axis_max_value;
 // double axis_tick_increment;
 // int axis_ticks;

 // multiset<double> my_set;
	//// Initialize my_set with some entirely fictional data.
 // my_set.insert(1.2);
 // my_set.insert(2.3);
 // my_set.insert(3.4);

 // // Feed STL 1D container to scale_axis.
 // scale_axis(my_set, &axis_min_value, &axis_max_value, &axis_tick_increment, &axis_ticks, false, tol100eps, 6, 0);

*/
