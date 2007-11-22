// 2d_area_fill.cpp
// Copyright (C) Jacob Voytko 2007
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org
// -----------------------------------------------------------------

#include <boost/svg_plot/svg_2d_plot.hpp>
#include <map>
#include <cmath>

using std::map;
using namespace boost::svg;

double f(double x)
{
	return 50. * sin(x);
}

int main()
{
	map<double, double> data1;
	
    double inter = 3.14159265 / 8.;

	for(double i=0; i<=10.; i+=inter)
	{
		data1[i] = f(i);
	}

	svg_2d_plot my_plot;

	// Size/scale settings.
	my_plot.image_size(700, 500)
	       .x_range(-1, 10)
	       .y_range(-75, 75);
           

	// Text settings.
	my_plot.title("Plot of 50 * sin(x)")
	       .title_font_size(29)
	       .x_label("X Axis Units")
           .y_major_labels_on(true)
           .y_major_grid_on(true);
	
	// Commands.
	my_plot.plot_window_on(true)
	       .x_label_on(true)
	       .x_major_labels_on(true);
	
	// Color settings.
	my_plot.background_color(svg_color(67, 111, 69))
	       .legend_background_color(svg_color(207, 202,167))
	       .legend_border_color(svg_color(102, 102, 84))
	       .plot_background_color(svg_color(136, 188, 126))
	       .title_color(white)
           .y_major_grid_color(black);

	//X axis settings.
	my_plot.x_major_interval(2)
	       .x_major_tick_length(14)
	       .x_major_tick_width(1)
	       .x_minor_tick_length(7)
	       .x_minor_tick_width(1)
	       .x_num_minor__ticks(3)
	
	//Y axis settings.
	       .y_major_interval(25)
	       .y_num_minor_ticks(5);		

	//legend settings
	my_plot.legend_title_font_size(15);
	
	my_plot.plot(data1, "Sqrt(x)",  point_style = none,
        area_fill_color = red);

    my_plot.write("./2d_area_fill.svg");

	return 0;
}
