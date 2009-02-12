/*! \file 1d_x_grid.cpp

  \brief Simple 1D plot with x axis grid.

  \author Jacob Voytko

  \date 2007
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

  // Adding generic settings.
  my_plot.background_border_color(black)
    .legend_on(true)
    .plot_window_on(true)
    .title("Race Times")
    .x_range(-1, 11);

  // Adding grids.
  my_plot.x_major_grid_on(true)
         .x_minor_grid_on(true);

  // Styling the grid lines.
  my_plot.x_major_grid_color(black)
         .x_minor_grid_color(lightgray);

  // Write to plot.
  my_plot.plot(dan_times, "Dan").stroke_color(blue);
  my_plot.plot(elaine_times, "Elaine").stroke_color(orange);

   // Write to file.
  my_plot.write("./1d_x_grid.svg");
  return 0;
} // int main()

