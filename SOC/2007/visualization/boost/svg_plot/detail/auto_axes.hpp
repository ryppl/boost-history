// auto_axes.hpp

// Copyright Paul A. Bristow 2006 - 2008.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_AUTO_AXES_HPP
#define BOOST_SVG_AUTO_AXES_HPP

#include <boost\svg_plot\detail\FP_compare.hpp> // is_small & is_close

#include <boost\math\special_functions\fpclassify.hpp> // infinite
  template <class T> bool boost::math::isfinite(T t);

#include <boost/algorithm/minmax_element.hpp>
 using boost::minmax_element;
 // minmax_element finds both min and max elements more efficiently than separately.

#include <cmath> // for fabs, pow, ceil, log10
#include <limits> // for std::numeric_limits;
#include <stdexcept> // for std::domain_error;
#include <iterator> // for std::iterator_traits;
#include <utility> // using std::pair;

// Autoscaling algorithm derived from:
// Michael P.D. Bramley. CUJ July 2000, p 20 - 26.
// Antonio Gomiz Bas, CUJ march 2000, p 42 - 45
// J. A. Nelder and W. Douglas Stirling, FORTRAN program SCALE.
// Daniel Herring

namespace boost
{
namespace svg
{

// Defines:

// Show and show_all to display size and contents of STL containers.
// range and range_all to find the min and max of STL containers.
// _all versions deal with container of containers.

// Roundup and Rounddown to 2, 4, 6, 8, 10, or 5, 10 or 2, 5, 10 systems functions:
  double roundup10(double value);
  double rounddown10(double value);
  double roundup5(double value);
  double rounddown5(double value);
  double roundup2(double value);
  double rounddown2(double value);

  void scale_axis(double min_value, double max_value, // Scale axis from Input range min & max.
               double* axis_min_value,  double* axis_max_value, double* axis_tick_increment, int* auto_ticks, // All 4 updated.
               // NO check_limits parameter.
               bool origin, // do not include the origin unless the range min_value <= 0 <= max_value.
               double tight, // tightest - fraction of 'overrun' allowed before another tick used.
               // for visual effect up to about 0.001 might suit a 1000 pixel wide image,
               // allowing values just 1 pixel over the tick to be shown.
               int min_ticks, // Minimum number of major ticks.
               int steps); // Round up and down to 2, 4, 6, 8, 10, or 5, 10 or 2, 5, 10 systems.

template <typename iter>
int mnmx(iter begin, iter end, double* min, double* max)
{ // Inspect all values between begin and (before) end to work out min and max.
  // Similar to boost::minmax_element, but ignoring at 'limit': non-finite, +-infinity, max & min, & NaN).
  // If can't find a max and a min, then will throw exception in x_range.
  *max = std::numeric_limits<double>::quiet_NaN();
  *min = std::numeric_limits<double>::quiet_NaN();
  using boost::svg::detail::is_limit; // either x and/or y not a proper data value.
  int goods = 0; // Count of values within limits.
  int limits = 0;
  iter pos = begin;
  while(pos != end && is_limit(*pos))
  { // Count any limits before the first good.
    limits++;
    pos++;
  }
  if (pos == end) 
  { // ALL values are at limit!
      throw std::runtime_error("Autoscale could not find any useful values to scale axis!");
    //cout << "all values at limit!" << endl;
    // min and max are both == NaN
  }
  else
  {
    double x = *pos;
    *max = x;
    *min = x;
    //cout << "Initial min & max " << x << endl;
    pos++;
    goods++;
    while(pos != end)
    {
      if (!is_limit(*pos))
      { // x is finite.
        x = *pos;
        if (x > *max)
        {
          *max = x;
        }
        if (x < *min)
        {
          *min = x;
        }
        goods++;
        //cout << goods << " goods, " << x << endl;
     } // if finite
      else
      { // If x not finite, then the y value won't be plotted.
        cout << "limit value: " << *pos  << endl;
        limits++;
      }
      ++pos;
    } // while
    //cout << "min " << *min << ", max " << *max << endl; //
    //cout << "limits " << limits << endl;
  }
  if (goods < 2) 
  {
    throw std::runtime_error("Autoscale could not find useful min & max to scale axis!");
  }
  return goods; // If goods < 2,  
} // inmmax(iter begin, iter end, double* min, double* max)


#if defined (_MSC_VER)
#  pragma warning (push)
#  pragma warning (disable: 4100) // 'check_limits' : unreferenced formal parameter
#endif

// scale axis function to define axis marker ticks based on two min & max values of the data.
void scale_axis(double min_value, double max_value, // Input range
               double* axis_min_value,  double* axis_max_value, double* axis_tick_increment, int* auto_ticks, // All 4 updated.
               bool check_limits, // Whether to check all values for infinity, NaN etc.
               bool origin, // If true, ensures that zero is a tick value.
               double tight, // Allows user to avoid a small fraction over a tick using another tick.
               int min_ticks, // Minimum number of ticks.
               int steps) // Round up and down to 2, 4, 6, 8, 10, or 5, 10 or 2, 5, 10 systems.
{

 //             int steps); // 0,  or 2 for 2, 4, 6, 8, 10, 5 for 1, 5, 10, or 10 (2, 5, 10).

  // Must assume max and min are OK (can't ignore limit values)
  // If either at limit then will be caught and exception thrown later by x_range.
  // So deliberately ignore check_limits parameter & supress any warning.
  scale_axis(min_value, max_value,
    axis_min_value, axis_max_value, axis_tick_increment, auto_ticks, // All 4 updated.
    origin, tight, min_ticks, steps); // Display range.
} 

#if defined (BOOST_MSVC)
#  pragma warning(pop)
#endif

template <typename iter> // T an STL container: array, vector, set ...
void scale_axis(iter begin, iter end, // Scale axis from data series (usually to plot), perhaps only part of container.
               // (not necessarily ordered, so will find min and max).
               double* axis_min_value,  double* axis_max_value, double* axis_tick_increment, int* auto_ticks, // All 4 updated.
               bool check_limits, // Whether to check all values for infinity, NaN etc.
               bool origin = false, // do not include the origin unless the range min_value <= 0 <= max_value.
               double tight = 0., // tightest - fraction of 'overrun' allowed before another tick used.
               // for visual effect up to about 0.001 might suit a 1000 pixel wide image,
               // allowing values just 1 pixel over the tick to be shown.
               int min_ticks = 6, // Minimum number of major ticks.
               int steps = 0) // 0,  or 2 for 2, 4, 6, 8, 10, 5 for 1, 5, 10, or 10 (2, 5, 10).
{
    void scale_axis(double min_value, double max_value, // Input range.
               double* axis_min_value,  double* axis_max_value, double* axis_tick_increment, int* auto_ticks, // All 4 updated.
               bool origin, // If true, ensures that zero is a tick value.
               double tight, // Allows user to avoid a small fraction over a tick using another tick.
               int min_ticks, // Minimum number of ticks.
               int steps); // Round up and down to 2, 4, 6, 8, 10, or 5, 10 or 2, 5, 10 systems.
  double x_min;
  double x_max;
  if (!check_limits)
  { // minmax_element is efficient for maps because can use knowledge of being sorted,
    // BUT only if it can be assumed that no values are 'at limits',
    // infinity, NaN, max_value, min_value, denorm_min.
    // Otherwise it is necessary to inspect all values individually.
    std::pair<iter, iter> result = boost::minmax_element(begin, end); // min & max
    // scale_axis (not check_limits version) forward declaration to ensure compiler finds right version.
     x_min = *(result.first);
     x_max = *(result.second);
  }
  else
  { // Must check limits.
    int good = mnmx(begin, end, &x_min, &x_max);
    if (good < 2)
    {
      throw std::runtime_error("Autoscale could not find useful min & max to scale axis!");
    }
    scale_axis(x_min, x_max,
    axis_min_value, axis_max_value, axis_tick_increment, auto_ticks, // All 4 updated.
      origin, tight, min_ticks, steps); // Display range.
  }
} // template <typename iter> void scale_axis(iter begin, iter end, ...

template <class T> // T an STL container: array, vector ...
void scale_axis(const T& container, // Entire Container Data series, usually to plot.
               // (not necessarily ordered, so will find min and max).
               double* axis_min_value,  double* axis_max_value, double* axis_tick_increment, int* auto_ticks, // All 4 updated.
               bool check_limits, // Whether to check all values for infinity, NaN etc.
               bool origin = false, // do not include the origin unless the range min_value <= 0 <= max_value.
               double tight = 0., // tightest - fraction of 'overrun' allowed before another tick used.
               // for visual effect up to about 0.001 might suit a 1000 pixel wide image,
               // allowing values just 1 pixel over the tick to be shown.
               int min_ticks = 6, // Minimum number of major ticks.
               int steps = 0) // 0,  or 2 for 2, 4, 6, 8, 10, 5 for 1, 5, 10, or 10 (2, 5, 10).
{
  double x_min;
  double x_max;
  if (!check_limits)
  {
    std::pair<T::const_iterator, T::const_iterator> result = boost::minmax_element(container.begin(), container.end());
    // minmax_element is efficient because can use knowledge of being sorted,
    // BUT only if it can be assumed that no values are 'at limits',
    // infinity, NaN, max_value, min_value, denorm_min.
     x_min = *(result.first);
     x_max = *(result.second);
  }
  else
  { // It is necessary to inspect all values individually.
    //cout << container.size() << " values." << endl;
    // Work out min and max, ignoring non-finite, +-infinity, max & min, & NaN).
    // If can't find a max and a min, then will throw exception.
    int good = mnmx(container.begin(), container.end(), &x_min, &x_max);
    if (good < 2)
    {
      throw std::runtime_error("Autoscale could not find useful min & max to scale axis!");
    }
    // cout << "x_min " << x_min << ", x_max " << x_max << endl; //
  }

  scale_axis(x_min, x_max,
    axis_min_value, axis_max_value, axis_tick_increment, auto_ticks, // All 4 updated.
    origin, tight, min_ticks, steps); // Display range.
} // template <class T> int scale_axis  T an STL container: array, vector ...

template <class T> // Scale X and Y axis using T a 2D STL container: array, vector ...
void scale_axis(const T& container, // Container Data series to plot - entire 2D container.
  // (not necessarily ordered, so will find min and max).
  double* x_axis_min_value,  double* x_axis_max_value, double* x_axis_tick_increment, int* x_auto_ticks,
  double* y_axis_min_value,  double* y_axis_max_value, double* y_axis_tick_increment, int* y_auto_ticks,
  // All 8 updated.
  bool check_limits = true, // Whether to check all values for infinity, NaN etc.
  bool x_origin = false, // do not include the origin unless the range min_value <= 0 <= max_value.
  double x_tight = 0., // tightest - fraction of 'overrun' allowed before another tick used.
  // for visual effect up to about 0.001 might suit a 1000 pixel wide image,
  // allowing values just 1 pixel over the tick to be shown.
  int x_min_ticks = 6, // Minimum number of major ticks.
  int x_steps = 0, // 0,  or 2 for 2, 4, 6, 8, 10, 5 for 1, 5, 10, or 10 (2, 5, 10).

  bool y_origin = false, // do not include the origin unless the range min_value <= 0 <= max_value.
  double y_tight = 0., // tightest - fraction of 'overrun' allowed before another tick used.
  // for visual effect up to about 0.001 might suit a 1000 pixel wide image,
  // allowing values just 1 pixel over the tick to be shown.
  int y_min_ticks = 6, // Minimum number of major ticks.
  int y_steps = 0) // 0,  or 2 for 2, 4, 6, 8, 10, 5 for 1, 5, 10, or 10 (2, 5, 10).
{
  typedef T::const_iterator iter;
  double x_max = std::numeric_limits<double>::quiet_NaN();
  double x_min = std::numeric_limits<double>::quiet_NaN();
  double y_max = std::numeric_limits<double>::quiet_NaN();
  double y_min = std::numeric_limits<double>::quiet_NaN();

  if (!check_limits)
  { // BUT only if it can be assumed that no values are 'at limits',
    // infinity, NaN, max_value, min_value, denorm_min.
    // minmax_element is efficient for maps because can use knowledge of being sorted,
    std::pair<iter, iter> result = boost::minmax_element(container.begin(), container.end());
    pair<const double, double> px = *result.first; // x min & max
    pair<const double, double> py = *result.second; // y min & max
    x_min = px.first;
    x_max = py.first;
    y_min = px.second;
    y_max = py.second;
  }
  else
  { // Otherwise it is necessary to inspect all values individually.
    // It seems that X and Y need to be examined in pairs, so sadly, we can't use:
    // int good_x = mnmx(container.begin(), container.end(), &x_min, &x_max);
    // int good_y = mnmx(container.begin(), container.end(), &y_min, &y_max);

    // Work out min and max, ignoring non-finite (+-infinity & NaNs).
    using boost::svg::detail::pair_is_limit; // either x and/or y not a proper data value.

    int goods = 0; // count of values where both X and Y are within limits.
    int limits = 0;
    T::const_iterator pos = container.begin();
    while(pos != container.end() && pair_is_limit(*pos))
    { // Count any limits before the first good.
      limits++;
      pos++;
    }
    if (pos == container.end()) 
    { // ALL values are at limit!
      //cout << "all values at limit" << endl;
      throw std::runtime_error("Autoscale could not find any useful values to scale axes!");
    }
    else
    {
      double x = pos->first;
      x_max = x;
      x_min = x;
      double y = pos->second;
      y_max = y;
      y_min = y;
      //cout << "Initial min & max " << x << ' ' << y << endl;
      pos++;
      goods++;
      while(pos != container.end())
      {
        if (!pair_is_limit(*pos))
        { // Either x and/or y are finite.
          x = pos->first;
          if (x > x_max)
          {
            x_max = x;
          }
          if (x < x_min)
          {
            x_min = x;
          }
          y = pos->second;
          if (y > y_max)
          {
            y_max = y;
          }
          if (y < y_min)
          {
            y_min = y;
          }
          goods++;
          // cout << goods << " goods, " << x << ' ' << y << endl;
       } // if finite
        else
        { // If either are not finite, then neither useful for autoscaling.
          // If x not finite, then the y value won't be plotted.
          // If y value not finite, then it will be 'off limits'.
          // cout << "limit value: " << pos->first << ' ' << pos->second  << endl;
          limits++;
        }
        ++pos;
      } // while
      //cout << "x_min " << x_min << ", x_max " << x_max << endl; // x_min 1, x_max 7.3
      //cout << "y_min " << y_min << ", y_max " << y_max << endl; // y_min 3.2, y_max 9.1
      //cout << "limits " << limits << endl;
    }
  }
  scale_axis(x_min, x_max,
    x_axis_min_value, x_axis_max_value, x_axis_tick_increment, x_auto_ticks,
    x_origin, x_tight, x_min_ticks, x_steps);

  scale_axis(y_min, y_max,
    y_axis_min_value, y_axis_max_value, y_axis_tick_increment, y_auto_ticks,
    y_origin, y_tight, y_min_ticks, y_steps);
} // template <class T> int scale_axis  T an STL container: array, vector ...


// Above versions use version below that does the real scaling work.

void scale_axis(double min_value, double max_value, // Scale axis from Input range min & max.
               double* axis_min_value,  double* axis_max_value, double* axis_tick_increment, int* auto_ticks, // All 4 updated.
               // NO check_limits parameter.
               bool origin = false, // do not include the origin unless the range min_value <= 0 <= max_value.
               double tight = 0., // tightest - fraction of 'overrun' allowed before another tick used.
               // for visual effect up to about 0.001 might suit a 1000 pixel wide image,
               // allowing values just 1 pixel over the tick to be shown.
               int min_ticks = 6, // Minimum number of major ticks.
               int steps = 0) // 0,  or 2 for 2, 4, 6, 8, 10, 5 for 1, 5, 10, or 10 (2, 5, 10).
{
  int ticks = -1; // Negative to warn of 'bad' value.
  double test_max;
  double test_min;
  double test_increment;
  switch (steps)
  { // Optionally expand the range by rounding actual max and min values up and down.
  case 0 :
    break; // No steps.
  case 10 :
    max_value = roundup10(max_value);
    min_value = rounddown10(min_value);
    break;
  case 5 :
    max_value = roundup5(max_value);
    min_value = rounddown5(min_value);
    break;
  case 2 :
    max_value = roundup2(max_value);
    min_value = rounddown2(min_value);
    break;
  default:
    throw std::domain_error("Unimplemented steps!");
  } // switch
  double range = max_value - min_value;  // range of data.

  smallest<> is_small(1000. * std::numeric_limits<double>::min()); // 1000 * min value
  close_to<> is_near_100eps(100. * std::numeric_limits<double>::epsilon()); // 100 * epsilon

  if ((tight < 0.) || tight > 1.)
  { // Tight can't be negative and > 1 is very likely a mistake, 0.01 = 1% more reasonable.
    throw std::domain_error("tight not in range 0 to 1 !");
  }

  using boost::math::isfinite;
  if(!(isfinite)(min_value))
  {
    throw std::domain_error("min_value not finite!");
  }
  if(!(isfinite)(max_value))
  {
    throw std::domain_error("max_value not finite!");
  }
  if (origin == true)
  { // Ensure the axis includes zero.
    if (min_value > 0.)
    { // All positive case.
      min_value = 0.;
    }
    else if(max_value < 0.)
    { // All negative case.
      max_value = 0.;
    }
  } // origin

  if (min_value > max_value)
  { // max and min are transposed!
    throw(std::domain_error("min > max!"));
  }
  else if (is_small(range)
    // range <= 1000. * std::numeric_limits<double>::min()) // Absolute range > ~1e-308 * 1000 ~= 1e-305
    // Range has already been checked to be > 0.
    // This checks for range too near to zero to be useful.
    // is_small is similar to Boost.Test check_is_small

    || is_near_100eps(max_value, min_value)

    //|| (range <= 100. * std::numeric_limits<double>::epsilon() * abs(min_value))
    // This checks the relative range is not too near to epsilon to be useful.
    // Knuth Vol II, avoiding over and underflow, as used in Boost.Test close_at_tolerance.
    )
  { // Factor of 1000 is to ensure range is more than a few epsilon relative wide.
    // Special cases of max ~== min values *and* exactly max == min (including == 0).
    // This could be two or more duplicate (repeat) measurements on x or y axis,
    // or only a modest number of epsilons apart even,
    // so it not necessarily an error, but some special handling is required.
    // At what point does range become big enough to be plausible to provide axis max & min tick values?
    // A few numeric_limits<double>::epsilon() only covers smallish compute errors.
    // at least 1000 epsilon absolute seems more plausible, and relative to biggest?
    // Return 3 ticks: -1, max==min==mid, and +1 ticks.
    // But uncertain if this is best solution?

    double mean = (min_value + max_value) /2;
    test_increment = 1;
    test_min = mean - test_increment;
    test_max = mean + test_increment;

    ticks = 3; // ticks - OK, but ticks == 3 warns that max_value ~== min_value.
  }
  else
  { // Range is reasonably large, so
    // compute candidate for increment - must be smaller than range, so divide by 10.
    test_increment = std::pow(10., ceil(log10(abs(range)/10.)));
    // Must be a decimal multiple or decimal fraction,
    // but is not necessarily exactly representable in floating-point format.
    // Establish maximum axis scale value, using this increment.

    test_max = (static_cast<long>(max_value / test_increment)) * test_increment;

    if(test_max < max_value)
    {
      test_max += test_increment;
    }
    ticks = 1; // Must be 1 'extra' tick at the end.
    // Establish minimum axis tick value by decrementing from test_max.
    test_min = test_max;
    do
    {
      ticks++;
      test_min -= test_increment;
    }
    while (test_min > min_value); // min_value);

    // Subtracting small values can screw up the scale limits,
    // eg: if scale_axis is called with (min, max)=(0.01, 0.1),
    // then the calculated scale is 1.0408E17 TO 0.05 BY 0.01,
    // rather than 0, 0.05, 0.01.
    // I suspect 1.e-10 is bigger than necessary?  related to std::numeric_limits<>::epsilon?
    if(abs(test_min) < 1.E-14)
    { // test_min is very near zero,
      test_min = 0.; // so treat as exact zero to avoid risk of a switch to e format.
    }
    while(ticks < min_ticks)
    {  // Adjust for too few tick marks by
      test_increment /= 2.; // halving the increment.
      // (divide by two should not cause trouble by being inexact).
      ticks = static_cast<int>((test_max - test_min) / test_increment) +1;
      if (steps == 0)
      { // Remove any superflous ticks above max and below min.
        while((test_min + test_increment) <= min_value)
        { // min_value is > 2nd from bottom tick,
          test_min += test_increment;
          ticks--; // so we can scrap the 1st bottom tick.
        }
        while((test_max - test_increment) >= max_value)
        { // max_value is > top_but_one tick,
          ticks--; // so ditch the top tick.
          test_max -= test_increment;
        }
      }
    } // while

    if (tight > 0.)
    { // Check that can't use a tick less at top or bottom.
      double max_plus_margin = test_max - test_increment + test_increment * tight;
      if (max_value < max_plus_margin)
      { // max is too big, so remove top tick.
        ticks -= 1;
        test_max -= test_increment;
      }

      double min_plus_margin = test_min + test_increment - test_increment * tight;
      if (min_value > min_plus_margin)
      { // min is too small, so remove the bottom tick.
        ticks -= 1;
        test_min += test_increment;
      }
      // Check again to make quite sure can't reduce again.
      max_plus_margin = test_max - test_increment + test_increment * tight;
      if (max_value < max_plus_margin)
      { // max is too big
        ticks -= 1;
        test_max -= test_increment;
      }
      min_plus_margin = test_min + test_increment - test_increment * tight;
      if (min_value > min_plus_margin)
      { // min is too small, so remove the bottom tick.
        ticks -= 1;
        test_min += test_increment;
      }
    } // if (tight != 0.)
  } // range reasonable

  // Pass computed min & max axis tick values back to caller.
  *axis_min_value = test_min; //
  *axis_max_value = test_max;
  *axis_tick_increment = test_increment; // major_tick_interval.
  *auto_ticks = ticks; // major ticks.
} // scale_axis

// Utility functions to display containers
// and to find min and max values in containers.

template <typename T> // T an STL container: array, vector ...
size_t show(const T& container)
{
  cout << container.size() << " values in container: ";
  for (T::const_iterator it = container.begin(); it != container.end(); it++)
  {
    cout << *it << ' ';
  }
  cout << endl;
  return container.size();
}// Container Data series to plot.

// Pointer version is not needed - iterator version is used instead.

template <typename iter> // T an STL container: array, vector ...
size_t show(iter begin, iter end) // Iterators
{
  size_t count = 0;
  while (begin != end)
  {
    count++;
    cout << *begin << ' ';
    ++begin;
  }
  cout << ": " << count << " values used.";
  cout << endl;
  return count;
}// Container Data series to plot.

template <typename T> // T an STL container: container of containers.
size_t show_all(const T& containers)
{ // Show all the containers values.
  for (T::const_iterator it = containers.begin(); it != containers.end(); it++)
  {
    show(*it);
  }
  return containers.size();
} // Container Data series to plot.

template <class T> // T an STL container: array, vector ...
std::pair<double, double> range(const T& container) // Container Data series
{
  pair<T::const_iterator, T::const_iterator> result = boost::minmax_element(container.begin(), container.end());
  pair<double, double> minmax;
  minmax.first = *result.first;
  minmax.second = *result.second;
  return minmax;
} // template <class T> scale

template <class T> // T an STL container: array, vector, set, map ...
std::pair<double, double> range_all(const T& containers) // Container of STL containers of Data series.
{
  std::pair<double, double> minmax(numeric_limits<double>::max(), numeric_limits<double>::min());
  for (T::const_iterator it = containers.begin(); it != containers.end(); it++)
  {
    pair<double, double> mm = range(*it); // Scale of this container.
    minmax.first = (std::min)(mm.first, minmax.first); //
    minmax.second = (std::max)(mm.second, minmax.second);
  }
  return minmax;
} // template <class T> scale_all


double roundup10(double value)
{
  smallest<> is_small(100. * std::numeric_limits<double>::min()); // 100 * min value.
  if (is_small(value) )
  { // Value very close to zero.
    return 0.; // Just return zero.
  }
  bool is_neg = (value >= 0) ? false : true;
  value = abs(value);

  // Decimal scaling, so value is 0.1, 0.2, 0.5, 1., 2., 5. or 10., 20., 50., 100. ...
  int order = int(floor(log10(value))); // 0 to 9.999, gives 0, 10 to 99.9 gives 2 ...
  double scaled = value * pow(10., -order); // 0 to 9.99 is unchanged, 10 to 9.99 scaled down to 1. to 9.99
  double pow10order = is_neg ? -pow(10., order) : pow(10., order); //  power of ten, signed.
  if(scaled > 5.)
  {
    return 10. * pow10order;
  }
  else if(scaled > 2.)
  {
    return 5. * pow10order;
  }
  else if(scaled > 1.)
  {
    return  2. * pow10order;
  }
  else
  {
    return  1. * pow10order;
  }
} // double roundup10(double value)

double rounddown10(double value)
{
  smallest<> is_small(100. * std::numeric_limits<double>::min()); // 100 * min value
  if (is_small(value))
  { // Value very close to zero.
    return 0.; // Just return zero.
  }
  bool is_neg = (value >= 0) ? false : true;
  value = abs(value);
  // Decimal scaling, so value is 0.1, 0.2, 0.5, 1., 2., 5. or 10., 10., 20., 100. ...
  int order = int(floor(log10(value))); // 0 to 9.999, gives 0, 10 to 99.9 gives 2 ...
  double scaled = value * pow(10., -order); // 0 to 9.99 is unchanged, 10 to 9.99 scaled down to 1. to 9.99
  double pow10order = is_neg ? -pow(10., order) : pow(10., order); //  power of ten, signed.

  if(scaled <= 2.)
  {
    return 1. * pow10order;
  }
  else if(scaled <= 5.)
  {
    return 2. * pow10order;
  }
  else if(scaled <= 10.)
  {
    return 5. * pow10order;
  }
  else
  {
    return 10. * pow10order;
  }
} // double rounddown10(double value)

double roundup5(double value)
{
  // Binary scaling, so return 0.1, 0.5, 1, 5, 10, 50, 100 ....
  smallest<> is_small(100. * std::numeric_limits<double>::min()); // 100 * min value.
  if (is_small(value) )
  { // Value very close to zero.
    return 0.; // Just return zero.
  }
  bool is_neg = (value >= 0) ? false : true;
  value = abs(value);
  int order = int(floor(log10(value))); // 0 to 9.999, gives 0, 10 to 99.9 gives 2 ...
  double scaled = value * pow(10., -order); // 0 to 9.99 is unchanged, 10 to 9.99 scaled down to 1. to 9.99
  double pow10order = is_neg ? -pow(10., order) : pow(10., order); //  power of ten, signed.

  if(scaled > 5.)
  { // Scale down to 1.
    return 10. * pow10order;
  }
  else if(scaled > 1.)
  { //
    return 5. * pow10order;
  }
  else
  { // is < 1
  }
  return 1. * pow10order;
} // double roundup2(double value)

double rounddown5(double value)
{
  smallest<> is_small(100. * std::numeric_limits<double>::min()); // 100 * min value
  if (is_small(value))
  { // Value very close to zero.
    return 0.; // Just return zero.
  }
  bool is_neg = (value >= 0) ? false : true;
  value = abs(value);
  // Decimal scaling, so value is 0.1, 0.5, 1., 5. or 10., 10.,  100. ...
  int order = int(floor(log10(value))); // 0 to 9.999, gives 0, 10 to 99.9 gives 2 ...
  double scaled = value * pow(10., -order); // 0 to 9.99 is unchanged, 10 to 9.99 scaled down to 1. to 9.99
  double pow10order = is_neg ? -pow(10., order) : pow(10., order); //  power of ten, signed.

  if(scaled < 2.)
  { //
    return 1. * pow10order;
  }
  else if(scaled < 10.)
  { //
    return 5. * pow10order;
  }
  else
  { //
    return 10. * pow10order;
  }
} // double rounddow5(double value)

double roundup2(double value)
{
  // Binary scaling, so return 0.1, 0.2, 0.4, 0.6, 0.8, 1.0, 2, 4, 6, 8, 10, 20, 40 60, 80, 100...
  smallest<> is_small(100. * std::numeric_limits<double>::min()); // 100 * min value.
  if (is_small(value) )
  { // Value very close to zero.
    return 0.; // Just return zero.
  }
  bool is_neg = (value >= 0) ? false : true;
  value = abs(value);
  int order = int(floor(log10(value))); // 0 to 9.999, gives 0, 10 to 99.9 gives 2 ...
  double scaled = value * pow(10., -order); // 0 to 9.99 is unchanged, 10 to 9.99 scaled down to 1. to 9.99
  double pow10order = is_neg ? -pow(10., order) : pow(10., order); //  power of ten, signed.

  if(scaled > 8.)
  {
    return 10. * pow10order;
  }
  else if(scaled > 6.)
  {
    return 8. * pow10order;
  }
  else if(scaled > 4.)
  {
    return 6. * pow10order;
  }
  else if(scaled > 2.)
  {
    return 4. * pow10order;
  }
  else
  {
    return 2. * pow10order;
  }
} // double roundup2(double value)

double rounddown2(double value)
{
  smallest<> is_small(100. * std::numeric_limits<double>::min()); // 100 * min value
  if (is_small(value))
  { // Value very close to zero.
    return 0.; // Just return zero.
  }
  bool is_neg = (value >= 0) ? false : true;
  value = abs(value);
  // Binary scaling, so value is 0.1, 0.2, 0.4, 0.6, 0.8, 0., 2., 4., 6., 8., 10., 20., 40., 60, 80, 100. ...
  int order = int(floor(log10(value))); // 0 to 9.999, gives 0, 10 to 99.9 gives 2 ...
  double scaled = value * pow(10., -order); // 0 to 9.99 is unchanged, 10 to 9.99 scaled down to 1. to 9.99
  double pow10order = is_neg ? -pow(10., order) : pow(10., order); //  power of ten, signed.

  if(scaled < 2.)
  { // Not scaled.
    return 1. * pow10order;
  }
  else if(scaled < 4.)
  {
    return 2. * pow10order;
  }
  else if(scaled < 6.)
  {
    return 4. * pow10order;
  }
  else if(scaled < 8.)
  {
    return 6. * pow10order;
  }
  else
  { // > 8
    return 8. * pow10order;
  }
} // double rounddown2(double value)

  } // svg
} // boost

#endif // BOOST_SVG_AUTO_AXES_HPP
