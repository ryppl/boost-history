// 1d_full_layout.cpp
// Copyright (C) Jacob Voytko 2007
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org
// -----------------------------------------------------------------
#include <boost/svg_plot/svg_1d_plot.hpp>
#include <vector>
#include <deque>
#include <cmath>
#include <boost/array.hpp>

using std::vector;
using std::deque;
using namespace boost::svg;

double f(double x)
{
	return sqrt(x);
}

double g(double x)
{
	return -2 + x*x;
}

double h(double x)
{
	return -1 + 2*x;
}

int main()
{
	vector<double> data1;
	deque<double> data2;
	boost::array<double, 10> data3;
	
	int j=0;
	for(double i=0; i<9.5; i+=1.)
	{
		data1.push_back(f(i));
		data2.push_front(g(i));
		data3[j++] = h(i);
	}

	svg_1d_plot my_plot;

	// Size/scale settings.
	my_plot.image_size(500, 350)
	       .x_range(-3, 10);

	// Text settings.
	my_plot.title("Oh My!")
	       .title_font_size(29)
	       .x_label("Time in Months");
	
	// Commands.
	my_plot.legend_on(true)
	       .plot_window_on(true)
	       .x_label_on(true)
	       .x_major_labels_on(true);
	
	// color settings
	my_plot.background_color(svg_color(67, 111, 69))
	       .legend_background_color(svg_color(207, 202,167))
	       .legend_border_color(svg_color(102, 102, 84))
	       .plot_background_color(svg_color(136, 188, 126))
	       .title_color(white);

	//axis settings
	my_plot.x_major_interval(2)
	       .x_major_tick_length(14)
	       .x_major_tick_width(1)
	       .x_minor_tick_length(7)
	       .x_minor_tick_width(1)
	       .x_num_minor_ticks(3);
	
	//legend settings
	my_plot.legend_title_font_size(15);
	
	my_plot.plot(data1, "Lions",  blue);
	my_plot.plot(data2, "Tigers", orange);
	my_plot.plot(data3, "Bears",  red);

    my_plot.write("./1d_complex.svg");

	return 0;
}
