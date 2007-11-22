// svg_style.hpp 

// Copyright Jacob Voytko 2007
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_SVG_STYLE_HPP
#define BOOST_SVG_SVG_STYLE_HPP

// svg style information is fill, stroke, width, line & bezier curve.
// This module provides struct plot_point_style & struct plot_line_style
// and class svg_style holding the styles.

// http://www.w3.org/TR/SVG11/styling.html 

#include "svg_color.hpp"
#include "detail/svg_style_detail.hpp"

#include <ostream>
// using std::ostream;

namespace boost
{
namespace svg
{

enum point_shape
{ // Marking a data point.
  // Used in draw_plot_point in axis_plot_frame.hpp
  none = 0,
  round, square, point, egg, // ellipse
  vertical_line,  // line up & down from axis.
  horizontal_line, // line left & right from axis.
  vertical_tick, // tick up from axis.
  horizontal_tick, // line left & right from axis.
  // Note horizontal will not be useful for 1D plot - will be on the axis.
  cone, // pointing up - 'rightwayup'.
  triangle, // pointing down 'upsidedown'.
  star, // using polygon.
  lozenge,
  diamond,
  heart,
  club,
  spade,
  asterisk, // *
  x, // x
  cross,
  symbol // Unicode symbol including letters, digits, greek & 'squiggles'.
  // &#x3A9; = greek omega
  // TODO add other shapes? 
// name circle changed to round to avoid clash with function named circle.
}; 

struct plot_point_style
{
    svg_color fill_color; // Color of the centre of the shape.
    svg_color stroke_color; // Color of circumference of shape.
    int size; // diameter of circle, height of square, font_size  ...
    point_shape shape; // round, square, point...
    std::string symbols; // Unicode symbol (letters, digits, squiggles etc)
    // Caution: not all Unicode symbols are output by all browsers!
    // const std::string font = "Lucida Sans Unicode";
    // TODO? devise way of setting this font?

    plot_point_style(const svg_color& fill, const svg_color& stroke,
      int size = 10, point_shape shape = round, const std::string& symbols = "X")
        :
        fill_color(fill), stroke_color(stroke), size(size), shape(shape), symbols(symbols)
    { // TODO Should these be default colors?
    }
}; // struct plot_point_style

struct plot_line_style
{    // TODO dotted and dashed line style?  Useful for B&W?
    svg_color color;
    svg_color area_fill; // Fill color from line to axis.
    bool line_on;
    bool bezier_on;

    plot_line_style(const svg_color& col, const svg_color& acol = blank, bool on = true, bool bezier_on = false)
      :
      color(col), line_on(on), area_fill(acol), bezier_on(bezier_on)
    {
    }
}; // struct plot_line_style

// -----------------------------------------------------------------
// This is the style information for any <g> tag.
// This may be expanded to include more data from the SVG standard.
// -----------------------------------------------------------------
class svg_style
{
private: // Accesses only by set and get member functions below.
  // Names changed so private data member variables end with _,
  // to permit use of names for set & get member functions.
    svg_color fill_;
    svg_color stroke_;
    unsigned int width_;
    bool fill_on_; // true means there is fill info.
    bool stroke_on_;
    bool width_on_;
    
public:
    svg_style() :
      fill_(svg_color(0, 0, 0)), // == black
      stroke_(svg_color(0, 0, 0)),
      width_(0),
      fill_on_(false), stroke_on_(false), width_on_(false)
    { // Default constructor initialises all private data.
    }

    svg_style(const svg_color& fill, const svg_color& stroke,  unsigned int width = 0) :
                     fill_(fill), stroke_(stroke), width_(width),
                     fill_on_(false), stroke_on_(false), width_on_(false)
    { // Construct with specified fill and stroke colors, and width.
    }

    // Set & get svg_style member functions
    // to return fill color and stroke color & width.
    svg_color fill_color() const
    {
      return svg_color(fill_); 
    }

    svg_color stroke_color() const
    {
      return svg_color(stroke_);
    }

    unsigned int stroke_width() const
    { // TODO probably should be double???
      return width_;
    }

    bool fill_on() const
    {
      return fill_on_;
    }

    void fill_on(bool is) 
    {
      fill_on_ = is;
    }

    bool stroke_on() const
    {
      return stroke_on_;
    }

    void stroke_on(bool is) 
    {
      stroke_on_ = is;
    }

    bool width_on() const
    {
      return width_on_;
    }

    void width_on(bool is) 
    {
      width_on_ = is;
    }
 
    // Set svg_style member functions to set fill, stroke & width.
    svg_style& fill_color(const svg_color& col)
    { 
        fill_ = col;
        fill_on_ = true;
        return *this;
    }

    svg_style& stroke_color(const svg_color& col) 
    { 
        stroke_ = col;
        stroke_on_ = true;
        return *this;
    }

    svg_style& stroke_width(unsigned int width)
    { 
        width_ = width;
        width_on_ = true;
        return *this;
    }
    
    void write(std::ostream& rhs)
    { // Write any stroke, fill colors and/or width info (start with space).
        if(stroke_on_)
        {
            rhs << " stroke=\"";
            stroke_.write(rhs);
            rhs << "\"";
        }
        if(fill_on_)
        {
            rhs << " fill=\"";
            fill_.write(rhs);
            rhs<<"\"";
        }
        if(width_on_)
        {
            rhs << " stroke-width=\""
                << width_
                << "\"";
        }
    } // void write
}; // class svg_style

}//svg
}//boost

#endif // BOOST_SVG_SVG_STYLE_HPP
