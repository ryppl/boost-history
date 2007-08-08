// 2d_limit.cpp
// Copyright (C) Jacob Voytko 2007
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org
// -----------------------------------------------------------------

#include <boost/svg_plot/svg_2d_plot.hpp>
#include <map>
#include <limits>

using std::map;
using namespace boost::svg;

double f(double x)
{
	return 1. / x;
}

int main()
{
	map<double, double> data1;
	
    double inter = .5;

	for(double i = -10; i <= 10.; i += inter)
	{
		data1[i] = f(i);
	}

	svg_2d_plot my_plot;

	// Size/scale settings.
	my_plot.image_size(500, 350)
	       .x_range(-10.5, 10.5)
	       .y_range(-1.1, 1.1);
           

	// Text settings.
	my_plot.title("Plot of 1 / x")
	       .x_label("X Axis Units")
           .y_major_labels_on(true)
           .y_major_grid_on(true);
	
	// Commands.
	my_plot.plot_window_on(true)
	       .x_major_labels_on(true);

	//X axis settings.
	my_plot.x_major_interval(2)
	       .x_major_tick_length(14)
	       .x_major_tick_width(1)
	       .x_minor_tick_length(7)
	       .x_minor_tick_width(1)
	       .x_num_minor_ticks(3)
	
	//Y axis settings.
	       .y_major_interval(25)
	       .y_num_minor_ticks(5);		

	//legend settings
	my_plot.legend_title_font_size(15);
	
	my_plot.plot(data1, "1 / x",  _point_style = square,
        _size = 5, _line_on = false);

    my_plot.write("D:\\2d_limit.svg");

	return 0;
}
