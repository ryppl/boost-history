// svg_colors.cpp

// Copyright Paul A. 2007
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)


// Example to display all the SVG 1.1 named colors as rectangles.
// Need completion to all colors, not just one!

// -----------------------------------------------------------------
// Deals with colors that have special names. The reason that the
// underscore separator convention does not match the normal Boost format
// is that these names that are specified by the SVG standard.
// http://www.w3.org/TR/SVG/types.html#ColorKeywords
// -----------------------------------------------------------------
enum svg_color_constant
#include svg.hpp;
using svg::rect;

#include <boost/svg_plot/svg_1d_plot.hpp>
using namespace boost::svg;

int main()
{
  svg& rect(double x1, double y1, double width, double height);
	svg my_doc;

	my_doc.background_border_color(black)
         .legend_on(true)
	       .title("SVG standard colors")
	       .x_range(0, 1);

  my_doc.rect(20, 20, 0, 0);

  my_doc.write("./1d_simple.svg");
	return 0;
}

