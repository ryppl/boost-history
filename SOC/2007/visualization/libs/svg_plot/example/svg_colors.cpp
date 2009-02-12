/*! \file svg_colors.cpp

  \brief Example to display all the SVG 1.1 named colors as rectangles.

  \details
    Deals with colors that have special names. The reason that the
    underscore separator convention does not match the normal Boost format
    is that these names that are specified by the SVG standard.
    http://www.w3.org/TR/SVG/types.html#ColorKeywords

  \author Paul A. Bristow
  \date 2007
*/

// Copyright Paul A. Bristow 2007
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Need completion to all colors, not just one!

#include <boost/svg_plot/svg.hpp>
// using boost::svg::rect;

#include <boost/svg_plot/svg_1d_plot.hpp>
using namespace boost::svg;

int main()
{
  svg& rect(double x1, double y1, double width, double height);
  svg my_image;
  my_image.x_size(100);
  my_image.y_size(200);

  my_image.g().style().stroke_color(red);
  my_image.rect(20, 20, 0, 0);

  my_image.write("./svg_colors.svg");
  return 0;
} // int main()


