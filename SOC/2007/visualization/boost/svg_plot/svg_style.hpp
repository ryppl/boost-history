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

// Forward declarations of classes in svg_style.hpp
class svg_style;
class plot_point_style;
class plot_line_style;
class text_style;

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

class plot_point_style
{
public:
    svg_color fill_color_; // Color of the centre of the shape.
    svg_color stroke_color_; // Color of circumference of shape.
    int size_; // diameter of circle, height of square, font_size  ...
    point_shape shape_; // round, square, point...
    std::string symbols_; // Unicode symbol (letters, digits, squiggles etc)
    // Caution: not all Unicode symbols are output by all browsers!
    // const std::string font = "Lucida Sans Unicode";
    // TODO? devise way of setting this font?
    //text_style style_;

    plot_point_style(const svg_color& fill = blank, const svg_color& stroke = blank,
      int size = 10, point_shape shape = round, const std::string& symbols = "X")
        :
        fill_color_(fill), stroke_color_(stroke), size_(size), shape_(shape), symbols_(symbols)
    { // TODO Should there be default colors? or "none" == blank?
    }

  plot_point_style& size(int i)
  {
    size_ = i;
    return *this;
  }

  int size()
  {
    return size_;
  }

  plot_point_style& fill_color(const svg_color& f)
  {
    fill_color_ = f;
    return *this;
  }

  svg_color& fill_color()
  {
    return fill_color_;
  }

  plot_point_style& stroke_color(const svg_color& f)
  {
    stroke_color_ = f;
    return *this;
  }

  svg_color& stroke_color()
  {
    return stroke_color_;
  }

  plot_point_style& shape(point_shape s)
  {
    shape_ = s;
    return *this;
  }

  point_shape shape()
  {
    return shape_;
  }

  //text_style& style()
  //{
  //  return style_;
  //}

}; // struct plot_point_style

class plot_line_style
{    // TODO dotted and dashed line style?  Useful for B&W?
public:
    svg_color color; // line stroke color.
    svg_color area_fill; // Fill color from line to axis. == true means color.blank = true.
    bool line_on;
    bool bezier_on;

    plot_line_style(const svg_color& col, const svg_color& acol = true, bool on = true, bool bezier_on = false)
      :
      color(col), area_fill(acol), line_on(on), bezier_on(bezier_on)
    {
    }
}; // struct plot_line_style


class text_style
{
private:
  int font_size_;
  std::string font_family_;
  std::string weight_;
  std::string style_;
  std::string stretch_;
  std::string decoration_;

public:
  text_style(int size = 12,  
  const std::string& font = "Lucida Sans Unicode",
  const std::string& style = "",
  const std::string& weight = "",
  const std::string& stretch = "",
  const std::string& decoration = "")
  : // Constructor.
  font_size_(size),
    font_family_(font),
    style_(style),
    weight_(weight),
    stretch_(stretch),
    decoration_(decoration)
  { //text_style default constructor, defines defaults for all private members.
  }

  int font_size()
  {
    return font_size_;
  }

  text_style& font_size(unsigned int i)
  { // pixels, default 10.
    font_size_ = i;
    return *this;
  }

  const std::string& font_family() const
  {
    return font_family_;
  }

  text_style& font_family(const std::string& s)
  { // Examples: "Arial", "Times New Roman", "Verdana", "Lucida Sans Unicode"
    font_family_ = s;
    return *this;
  }

  const std::string& font_style() const
  { // font-style: normal | bold | italic | oblique
    return style_; // example "normal"
  }

  text_style& font_style(const std::string& s)
  { // Examples: "italic"
    style_ = s;
    return *this;
  }

  const std::string& font_weight() const
  {
    return weight_;
  }

  text_style& font_weight(const std::string& s)
  { // svg font-weight: normal | bold | bolder | lighter | 100 | 200 .. 900
    // Examples: "bold", "normal" 
    weight_ = s;
    return *this;
  }

  const std::string& font_stretch() const
  {
    return stretch_;
  }

  text_style& font_stretch(const std::string& s)
  { // Examples: "wider" but implementation?
    // font-stretch: normal | wider | narrower ...
    stretch_ = s;
    return *this;
  }

  const std::string& font_decoration() const
  {
    return decoration_;
  }

  text_style& font_decoration(const std::string& s)
  { // Examples: "underline" | "overline" | "line-through"
    decoration_ = s; // But implementation doubtful.
    return *this;
  }


}; //   class text_style

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
    double width_;
    bool fill_on_; // true means there is fill info.
    bool stroke_on_;
    bool width_on_;
    
public:
    svg_style() :
      fill_(svg_color(0, 0, 0)), // == black
      stroke_(svg_color(0, 0, 0)), // == black
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

    double stroke_width() const
    { 
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
        fill_on_ = ! col.blank; // if blank fill is off or "none"
        return *this; // Chainable.
    }

    svg_style& stroke_color(const svg_color& col) 
    { 
        stroke_ = col;
        stroke_on_ = true;
        return *this;
    }

    svg_style& stroke_width(double width)
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
          rhs << "\"";
      }
      if(width_on_)
      { // TODO DO we ever want a 0 (or <0) width output???
          rhs << " stroke-width=\""
              << width_
              << "\"";
      }
     // Examples: <g id="yMinorTicks" stroke="rgb(0,0,0)" stroke-width="1">
   } // void write
}; // class svg_style

}//svg
}//boost

#endif // BOOST_SVG_SVG_STYLE_HPP
