#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/svg_plot/svg_2d_plot.hpp>

using namespace boost::unit_test;
using namespace boost::svg;
using namespace boost;

// test to make sure that changes to colors in one section don't change
// colors in another section. Logically, writing different color options
// forwards and backwards checks for all possible overwrites
int test_main(int, char* [])
{
	svg_1d_plot test_plot;
	test_plot.set_background_color(svg_color(0, 0, 1))
	.set_background_border_color(svg_color(0, 0, 2))
	.set_legend_background_color(svg_color(0, 0, 3))
	.set_legend_border_color(svg_color(0, 0, 4))
	.set_plot_background_color(svg_color(0, 0, 5))
	.set_title_color(svg_color(0, 0, 6))
	.set_x_axis_color(svg_color(0, 0, 7))
	.set_x_label_color(svg_color(0, 0, 8))
	.set_x_major_grid_color(svg_color(0, 0, 9))
	.set_x_minor_grid_color(svg_color(0, 0, 10))
	.set_x_major_tick_color(svg_color(0, 0, 11))
	.set_x_minor_tick_color(svg_color(0, 0, 12))
	.set_y_axis_color(svg_color(0, 0, 13))
	.set_y_label_color(svg_color(0, 0, 14))
	.set_y_major_grid_color(svg_color(0, 0, 15))
	.set_y_minor_grid_color(svg_color(0, 0, 16))
	.set_y_major_tick_color(svg_color(0, 0, 17))
	.set_y_minor_tick_color(svg_color(0, 0, 18));

	BOOST_CHECK(test_plot.get_background_color() == svg_color(0, 0, 1));
	BOOST_CHECK(test_plot.get_background_border_color() 
			   == svg_color(0, 0, 2));
	BOOST_CHECK(test_plot.get_legend_background_color() == svg_color(0,0,3));
	BOOST_CHECK(test_plot.get_legend_border_color() == svg_color(0, 0, 4));
	BOOST_CHECK(test_plot.get_plot_background_color() == svg_color(0, 0, 5));
	BOOST_CHECK(test_plot.get_title_color() == svg_color(0, 0, 6));
	BOOST_CHECK(test_plot.get_x_axis_color() == svg_color(0, 0, 7));
	BOOST_CHECK(test_plot.get_x_label_color() == svg_color(0, 0, 8));
	BOOST_CHECK(test_plot.get_x_major_grid_color() == svg_color(0, 0, 9));
	BOOST_CHECK(test_plot.get_x_minor_grid_color() == svg_color(0, 0, 10));
	BOOST_CHECK(test_plot.get_x_major_tick_color() == svg_color(0, 0, 11));
	BOOST_CHECK(test_plot.get_x_minor_tick_color() == svg_color(0, 0, 12));
	BOOST_CHECK(test_plot.get_y_axis_color() == svg_color(0, 0, 13));
	BOOST_CHECK(test_plot.get_y_label_color() == svg_color(0, 0, 14));
	BOOST_CHECK(test_plot.get_y_major_grid_color() == svg_color(0, 0, 15));
	BOOST_CHECK(test_plot.get_y_minor_grid_color() == svg_color(0, 0, 16));
	BOOST_CHECK(test_plot.get_y_major_tick_color() == svg_color(0, 0, 17));
	BOOST_CHECK(test_plot.get_y_minor_tick_color() == svg_color(0, 0, 18));

	test_plot.set_y_minor_tick_color(svg_color(0, 0, 18))
	.set_y_major_tick_color(svg_color(0, 0, 17))
	.set_y_minor_grid_color(svg_color(0, 0, 16))
	.set_y_major_grid_color(svg_color(0, 0, 15))
	.set_y_label_color(svg_color(0, 0, 14))
	.set_y_axis_color(svg_color(0, 0, 13))
	.set_x_minor_tick_color(svg_color(0, 0, 12))
	.set_x_major_tick_color(svg_color(0, 0, 11))
	.set_x_minor_grid_color(svg_color(0, 0, 10))
	.set_x_major_grid_color(svg_color(0, 0, 9))
	.set_x_label_color(svg_color(0, 0, 8))
	.set_x_axis_color(svg_color(0, 0, 7))
	.set_title_color(svg_color(0, 0, 6))
	.set_plot_background_color(svg_color(0, 0, 5))
	.set_legend_border_color(svg_color(0, 0, 4))
	.set_legend_background_color(svg_color(0, 0, 3))
	.set_background_border_color(svg_color(0, 0, 2))
	.set_background_color(svg_color(0, 0, 1));

	BOOST_CHECK(test_plot.get_background_color() == svg_color(0, 0, 1));
	BOOST_CHECK(test_plot.get_background_border_color() 
			   == svg_color(0, 0, 2));
	BOOST_CHECK(test_plot.get_legend_background_color() == svg_color(0, 0, 3));
	BOOST_CHECK(test_plot.get_legend_border_color() == svg_color(0, 0, 4));
	BOOST_CHECK(test_plot.get_plot_background_color() == svg_color(0, 0, 5));
	BOOST_CHECK(test_plot.get_title_color() == svg_color(0, 0, 6));
	BOOST_CHECK(test_plot.get_x_axis_color() == svg_color(0, 0, 7));
	BOOST_CHECK(test_plot.get_x_label_color() == svg_color(0, 0, 8));
	BOOST_CHECK(test_plot.get_x_major_grid_color() == svg_color(0, 0, 9));
	BOOST_CHECK(test_plot.get_x_minor_grid_color() == svg_color(0, 0, 10));
	BOOST_CHECK(test_plot.get_x_major_tick_color() == svg_color(0, 0, 11));
	BOOST_CHECK(test_plot.get_x_minor_tick_color() == svg_color(0, 0, 12));
	BOOST_CHECK(test_plot.get_y_axis_color() == svg_color(0, 0, 13));
	BOOST_CHECK(test_plot.get_y_label_color() == svg_color(0, 0, 14));
	BOOST_CHECK(test_plot.get_y_major_grid_color() == svg_color(0, 0, 15));
	BOOST_CHECK(test_plot.get_y_minor_grid_color() == svg_color(0, 0, 16));
	BOOST_CHECK(test_plot.get_y_major_tick_color() == svg_color(0, 0, 17));
	BOOST_CHECK(test_plot.get_y_minor_tick_color() == svg_color(0, 0, 18));
	return 0;
}
