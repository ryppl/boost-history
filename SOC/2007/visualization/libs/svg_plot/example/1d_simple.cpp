/*! \file 1d_simple.cpp
    \brief Example of a simple 1D plot of two vectors of data.
    \detail Creates file 1d_simple.svg

    \author Jacob Voytko
  */

// Copyright Jacob Voytko 2007

// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

#include <boost/svg_plot/svg_1d_plot.hpp>
#include <vector>

using std::vector;
using namespace boost::svg;

int main()
{
  vector<double> dan_times;
  vector<double> elaine_times;

  dan_times.push_back(3.1);
  dan_times.push_back(4.2);
  elaine_times.push_back(2.1);
  elaine_times.push_back(7.8);

  svg_1d_plot my_plot;

  my_plot.background_border_color(black)
         .legend_on(true)
         .title("Race Times")
         .x_range(-1, 11);

  my_plot.plot(dan_times, "Dan", blue);
  my_plot.plot(elaine_times, "Elaine", orange);

  my_plot.write("./1d_simple.svg");
  return 0;
} // int main()

