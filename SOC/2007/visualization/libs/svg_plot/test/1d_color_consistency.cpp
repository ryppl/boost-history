// 1d_color_consistency.cpp

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2007

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma warning(disable : 4267) //  '=' : conversion from 'size_t' to 'unsigned int'
// in spirit

#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/svg_plot/svg_1d_plot.hpp>
using namespace boost::svg;

int test_main(int, char*[])
{
	svg_1d_plot my_plot;

	my_plot
	.background_border_color(svg_color(0, 0, 1))
	.background_color(svg_color(0, 0, 2))
	.legend_background_color(svg_color(0, 0, 3))
	.legend_border_color(svg_color(0, 0, 4))
	.plot_background_color(svg_color(0, 0, 5))
	.title_color(svg_color(0, 0, 6))
	.x_axis_color(svg_color(0, 0, 7))
	.x_label_color(svg_color(0, 0, 8))
	.x_major_grid_color(svg_color(0, 0, 9))
	.x_major_tick_color(svg_color(0, 0, 10))
	.x_minor_grid_color(svg_color(0, 0, 11))
	.x_minor_tick_color(svg_color(0, 0, 12));

	BOOST_CHECK(my_plot.get_background_border_color() == svg_color(0, 0, 1));
	BOOST_CHECK(my_plot.get_background_color() == svg_color(0, 0, 2));
	BOOST_CHECK(my_plot.get_legend_background_color() == svg_color(0, 0, 3));
	BOOST_CHECK(my_plot.get_legend_border_color() == svg_color(0, 0, 4));
	BOOST_CHECK(my_plot.get_plot_background_color() == svg_color(0, 0, 5));
	BOOST_CHECK(my_plot.get_title_color() == svg_color(0, 0, 6));
	BOOST_CHECK(my_plot.get_x_axis_color() == svg_color(0, 0, 7));
	BOOST_CHECK(my_plot.get_x_label_color() == svg_color(0, 0, 8));
	//BOOST_CHECK_EQUAL(svg_color(1, 1, 1), svg_color(0, 0, 0));
  // check svg_color(1, 1, 1) == svg_color(0, 0, 0) failed [RGB(1,1,1) != RGB(0,0,0)]

	BOOST_CHECK_EQUAL(red, red);  // OK
	// BOOST_CHECK_EQUAL(white, black);
  // check white == black failed [143 != 7]
  // Note that named colors are enums!

	BOOST_CHECK_EQUAL(svg_color(0, 0, 8), svg_color(0, 0, 8));
	BOOST_CHECK_EQUAL(my_plot.get_x_label_color(), svg_color(0, 0, 8));
  // using BOOST_CHECK_EQUAL is more convenient because it shows the difference!
  // provided operator== and operator >> is provided.

	BOOST_CHECK(my_plot.get_x_major_grid_color() == svg_color(0, 0, 9));
	BOOST_CHECK(my_plot.get_x_major_tick_color() == svg_color(0, 0, 10));
	BOOST_CHECK(my_plot.get_x_minor_grid_color() == svg_color(0, 0, 11));
	BOOST_CHECK(my_plot.get_x_minor_tick_color() == svg_color(0, 0, 12));

  // Repeat - intentional? - would different values be a better test?
	my_plot
	.x_minor_tick_color(svg_color(0, 0, 12))
	.x_minor_grid_color(svg_color(0, 0, 11))
	.x_major_tick_color(svg_color(0, 0, 10))
	.x_major_grid_color(svg_color(0, 0, 9))
	.x_label_color(svg_color(0, 0, 8))
	.x_axis_color(svg_color(0, 0, 7))
	.title_color(svg_color(0, 0, 6))
	.plot_background_color(svg_color(0, 0, 5))
	.legend_border_color(svg_color(0, 0, 4))
	.legend_background_color(svg_color(0, 0, 3))
	.background_color(svg_color(0, 0, 2))
	.background_border_color(svg_color(0, 0, 1));

	BOOST_CHECK(my_plot.get_x_minor_tick_color() == svg_color(0, 0, 12));
	BOOST_CHECK(my_plot.get_x_minor_grid_color() == svg_color(0, 0, 11));
	BOOST_CHECK(my_plot.get_x_major_tick_color() == svg_color(0, 0, 10));
	BOOST_CHECK(my_plot.get_x_major_grid_color() == svg_color(0, 0, 9));
	BOOST_CHECK(my_plot.get_x_label_color() == svg_color(0, 0, 8));
	BOOST_CHECK(my_plot.get_x_axis_color() == svg_color(0, 0, 7));
	BOOST_CHECK(my_plot.get_title_color() == svg_color(0, 0, 6));
	BOOST_CHECK(my_plot.get_plot_background_color() == svg_color(0, 0, 5));
	BOOST_CHECK(my_plot.get_legend_border_color() == svg_color(0, 0, 4));
	BOOST_CHECK(my_plot.get_legend_background_color() == svg_color(0, 0, 3));
	BOOST_CHECK(my_plot.get_background_color() == svg_color(0, 0, 2));
	BOOST_CHECK(my_plot.get_background_border_color() == svg_color(0, 0, 1));
	return 0;
} // int test_main(int, char*[])


//**passed** ../../../bin.v2/libs/svg_plot/test/1d_color_consistency.test/gcc-4.1.2/debug/1d_color_consistency.test
/*

Compiling...
1d_color_consistency.cpp
Linking...
Embedding manifest...
Autorun "j:\Cpp\SVG\debug\test_1d_color_consistency.exe"
Running 1 test case...
*** No errors detected

*/