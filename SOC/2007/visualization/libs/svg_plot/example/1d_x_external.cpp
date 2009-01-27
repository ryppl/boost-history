// 1d_x_external.cpp
// Copyright (C) Jacob Voytko 2007
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org
// -----------------------------------------------------------------

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

    // Adding grid information.
    my_plot.x_major_grid_on(true)
           .x_minor_grid_on(true);


    // Styling grid.
    my_plot.x_major_grid_color(black)
           .x_minor_grid_color(lightgray);

    my_plot.x_ticks_down(true); // external.
    my_plot.y_ticks_left(true); // external.

    // Write to plot.
  my_plot.plot(dan_times, "Dan", blue);
  my_plot.plot(elaine_times, "Elaine", orange);

    // Write to file.
    my_plot.write("./1d_x_external.svg");
  return 0;
}
