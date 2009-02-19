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

#ifdef _MSC_VER_
#  pragam warning (disable : 4512) //  assignment operator could not be generated.
#endif

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

  my_image.g(); // Add first (zeroth) new element,
  g_element& g0 = my_image.g(0); // so index is zero.
  g0.push_back(new rect_element(0, 0, my_image.x_size(),  my_image.y_size() ) ); // border to all image.

  // Create a small test rectangle.
  g0.style().fill_color(azure); 
  g0.style().fill_on(true); 
  g0.style().stroke_color(yellow);
  g0.push_back(new rect_element(20, 20, 10, 10) ); // border to rectangle.
  g0.text(490, 100, "my color", no_style, right_align); // SVG name of color to the right.
 
  // The need to repeat down the page for all the colors.  TODO
  // Need a 'reverse lookup' of the enum colors as "azure" :-((

  my_image.write("./svg_colors.svg");
  return 0;
} // int main()


