#include <boost/test/included/test_exec_monitor.hpp>

#include "../../../boost/svg_plot/svg_1d_plot.hpp"

#include <sstream>
#include <vector>

using namespace boost::unit_test;
using namespace boost::svg;
using namespace boost;
using std::stringstream;
using std::vector;

double f(int x)
{
	return 2.*x;
}

double g(int x)
{
	return 1.5*x;
}

double h(int x)
{
	return -1*x+3;
}

// test to make sure that the output is consistent
int test_main(int, char* [])
{
	stringstream test1, test2;
	vector<double> data1, data2, data3;
	svg_1d_plot my_plot;

	for(int i = 0; i < 10; ++i)
	{
		data1.push_back(f(i));
		data2.push_back(g(i));
		data3.push_back(h(i));
	}
	
	// size/scale settings
	my_plot.set_image_size(500, 350)
	       .set_x_scale(-3, 10);

	// Text settings
	my_plot.set_title("Oh My!")
	       .set_title_font_size(29)
	       .set_x_label("Time in Months");

// command settings
	my_plot.set_axis_on(true)
	       .set_legend_on(true)
	       .set_plot_window_on(true)
	       .set_x_label_on(true)
	       .set_x_major_labels_on(true);

	// color settings
	my_plot.set_background_color(svg_color(67, 111, 69))
	       	.set_legend_background_color(svg_color(207, 202,167))
	       	.set_plot_background_color(svg_color(136, 188, 126))
	       .set_title_color(white)
	       .set_x_axis_color(black)
	       .set_x_major_tick_color(black)
	       .set_legend_border_color(svg_color(102, 102, 84))
	       .set_x_minor_tick_color(black);
	
//axis 	settings
	my_plot.set_x_major_tick(2)
	       .set_x_num_minor_ticks(3)
	       .set_x_major_tick_length(14)
	       .set_x_minor_tick_length(7)
	       .set_x_major_tick_width(1)
	       .set_x_minor_tick_width(1);
	
	//legend settings
	my_plot.set_legend_title_font_size(15);
	
	plot(my_plot, data2, "Lions", blue);
	plot(my_plot, data1, "Tigers", purple);
	plot(my_plot, data3, "Bears", red);
	
	my_plot.write(test1);
	my_plot.write(test2);

	BOOST_CHECK(test1.str() == test2.str());

	return 0;
}
