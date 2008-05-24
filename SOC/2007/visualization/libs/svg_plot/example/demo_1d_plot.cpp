// svg_1d_plot.hpp

// Copyright Jacob Voytko 2007
// Copyright Paul A Bristow 2007

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// An entirely contrived example designed to demonstration
// as many features as possible in a single example.
// The results is not intended to be useful,
// nor is it intended to be pretty.
// See other examples for more practical (and tasteful) examples using typical features.

#  pragma warning(disable : 4267) //  '=' : conversion from 'size_t' to 'unsigned int'
#  pragma warning(disable : 4172) //  
// in spirit  TODO remove when not using Boost.Parameter
#  pragma warning(disable : 4310) //  cast truncates constant value
#  pragma warning(disable : 4512) //  assignment operator could not be generated
#  pragma warning(disable : 4702) //  unreachable code
#  pragma warning(disable : 4224) //  nonstandard extension used : formal parameter 'function_ptr' was previously defined as a type
#  pragma warning(disable : 4800) //  const char *' : forcing value to bool 'true' or 'false' (performance warning)

#include <boost/svg_plot/svg.hpp>

#include <boost/svg_plot/svg_1d_plot.hpp>
  using namespace boost::svg;
  using boost::svg::svg_1d_plot;

#include <boost/svg_plot/svg_fwd.hpp> // forward declarations.
// for testing its correctness.

#include <iostream> // for debugging.
  using std::cout;
  using std::endl;

#include <vector>
  using std::vector;

int main()
{
	vector<double> my_data;
	// Initialize my_data here with some entirely fictional data.
  my_data.push_back(0.1);
  my_data.push_back(1.1);
  my_data.push_back(4.2);
  my_data.push_back(3.3);
  my_data.push_back(5.4);
  my_data.push_back(6.5);

  vector<double> my_data2;
	// Initialize my_data here with some more entirely fictional data.
  my_data2.push_back(0.5);
  my_data2.push_back(1.5);
  my_data2.push_back(4.7);

  vector<double> my_data3;
	// Initialize my_data here with some more entirely fictional data.
  my_data3.push_back(0.7);
  my_data3.push_back(2.5);
  my_data3.push_back(5.8);

  vector<double> my_data4;
	// Initialize my_data with some integral values so can check points are marked exactly right..
  my_data4.push_back(1.);
  my_data4.push_back(2.);
  my_data4.push_back(3.);
  my_data4.push_back(6.);
  my_data4.push_back(7.);
  my_data4.push_back(8.);

	svg_1d_plot my_1d_plot; // Construct with all the default constructor values.
  cout << "Image x & y " << my_1d_plot.image_x_size() << " to " << my_1d_plot.image_y_size() << endl;
  //my_1d_plot.image_size(100,100); // Alter both together.
  //cout << "Image x & y " << my_1d_plot.image_x_size() << " to " << my_1d_plot.image_y_size() << endl;
  //// And alter both separately.
  //my_1d_plot.image_x_size(200);
  //my_1d_plot.image_y_size(600);
  //cout << "Image x & y " << my_1d_plot.image_x_size() << " to " << my_1d_plot.image_y_size() << endl;

  my_1d_plot.document_title("My Document title");
  my_1d_plot.description("My Document description");
  my_1d_plot.copyright_date("2007");
  my_1d_plot.copyright_holder("Paul A. Bristow");
  //my_1d_plot.license("permits", "permits", "permits", "permits");
  //my_1d_plot.coord_precision(4);

  cout << "font-family was " << my_1d_plot.title_font_family() << endl;

	my_1d_plot
    
  .title("Demo 1D plot <sup>-&#945; </sup> &#x3A9; &#x3A6; &#x221A; &#x221E; &#x3B6; &#x00B1;")
    // domain of the random variable is [0, &#8734;]") //  Capital Omega <superscript> &#x3A9; </superscript>") doesn't work yet.
  .title_font_size(20)
  .title_font_family("Times New Roman")
	.legend_on(true)
  .legend_title("My Legend &#956;") // generates <em>&#956;</em>  greek mu 
  .legend_title_font_size(12)
  //.legend_title("") // generates <em>&#956;</em>  greek mu 
  //.plot_area(true) // TODO in docs..
  .background_color(ghostwhite) // whole image
  .plot_window_on(true)
  .image_y_size(250)
  .plot_background_color(aliceblue) // just the plot area
  .x_label_on(true) // show x-axis text label.
  .x_label("volume") // Care: this doesn't show unless .x_label_on() == true!
  .x_axis_color(green)
  .x_label_color(blue)
  .x_label_font_family("verdana") 
//  .x_label_font_family("Lucida sans console") 
//  .x_label_font_family("Times New Roman") 
  .x_label_units_on(true)
  .x_label_font_size(12)
  .x_label_units("meter&#179; or m&#179;") // super 2 = &#xB2; super 3 = &#179;
  // Care: this doesn't show unless .x_label_units_on() == true!
  .x_ticks_up_on(false)
  .x_ticks_down_on(true) // So only have downward ticks.
  .x_major_grid_on(true)
  .x_major_grid_width(2)
  .x_major_grid_color(green) 
  .x_minor_grid_on(true)
  .x_minor_grid_width(1)
  .x_minor_grid_color(red) 
	.x_range(0., 7.);

  cout << "font-family is " << my_1d_plot.title_font_family() << endl;

	my_1d_plot.plot(my_data, "my values 1", _stroke_color = red, _fill_color = blue, _point_style = vertical_tick, _size = 12);
  // Produces a blue vertical bar!

	my_1d_plot.plot(my_data4, "values symbol", red, _point_style = symbol, _size = 14);
  // Hardcoded X only TODO other when Boost.Parameter goes have way to set symbol(s)
  // U+2721 is Star of David or hexagram http://en.wikipedia.org/wiki/Hexagram
	my_1d_plot.plot(my_data2, "my values round", red, _point_style = round, _size = 10);
	my_1d_plot.plot(my_data3, "my values 3", red, _point_style = diamond, _size = 14);
	my_1d_plot.plot(my_data, "my values spade", blue, _point_style = spade, _size = 14);
	 my_1d_plot.plot(my_data2, "values asterisk", green, _point_style = asterisk, _size = 14);
	my_1d_plot.plot(my_data, "my cone", red, _point_style = cone, _size = 14);

  my_1d_plot.write("demo_1d_plot.svg");

  cout << "Plot window x " << my_1d_plot.plot_window_x().first << " to " << my_1d_plot.plot_window_x().second 
    << ", y " << my_1d_plot.plot_window_y().first << " to " << my_1d_plot.plot_window_y().second << endl;
	return 0;
} // int main()

/*

Compiling...
demo_1d_plot.cpp
Linking...
Embedding manifest...
Autorun "j:\Cpp\SVG\debug\demo_1d_plot.exe"
Build Time 0:11

*/

