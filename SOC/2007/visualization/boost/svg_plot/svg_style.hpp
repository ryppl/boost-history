// svg_style.hpp 

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2008

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

#include <iostream>
// using std::ostream;
#include <sstream>
// using std::stringstream;
#include <limits>
// using numeric_limits;

namespace boost
{
namespace svg
{

// Forward declarations of classes in svg_style.hpp
class svg_style;
class text_style;
class value_style;
class plot_point_style;
class plot_line_style;
class axis_line_style;
class ticks_labels_style;
class box_style;

enum rotate_style
{ // Rotation in degrees from horizontal.
  horizontal = 0, // normal left to right, centered.
  uphill = -45, // slope up.
  upward = -90, // vertical writing up.
  backup = -135, // // slope up backwards - upside down!
  leftward= -180, // horizontal to left.
  rightward = 360, // horizontal to right.
  downhill = 45, // slope down.
  downward = 90,  // vertical writing down.
  backdown = 135, // slope down backwards.
  upsidedown = 180 // == -180 - upside down!
};

const std::string strip_e0s(std::string s);
// Ugly hack to remove unwanted sign and leading zero(s) in exponent.
double string_svg_length(const std::string& s, const text_style& style);
// Estimate length of string when appears as svg units.

// -----------------------------------------------------------------
// This is the style information for any <g> tag.
// This may be expanded to include more data from the SVG standard.
// -----------------------------------------------------------------

// There are some strange effects for text on some browsers
// (Firefox especially) when only stroke is specified.
// fill is interpreted as black, and the font outline is fuzzy and bolder.
// <g id="title" stroke="rgb(255,0,0)"> .. is red border and black fill.
// (because created as a graphic not a builtin font?)
// <g id="title" fill="rgb(255,0,0)"> .. is red sharp font.
// <g id="title" stroke="rgb(255,0,0)" fill="rgb(255,0,0)"> red and red fill also fuzzy.
// So for text, only specific the fill unless a different outline is really wanted.
// Defaults for text provide a built-in glyph, for example for title:
// <g id="title">
//   <text x="250" y="36" text-anchor="middle" font-size="18" font-family="Verdana">
//     Plot of data
//   </text>
// </g>
// and this is not a graphic.

class svg_style
{ // Holds the basic stroke, fill colors and width, and their switches.
  friend std::ostream& operator<< (std::ostream&, svg_style&);

private: // Accesses only by set and get member functions below.
  // Private data member variables names end with _,
  // to permit use of names for set & get member functions.
  svg_color stroke_;
  svg_color fill_;
  double width_; // Only valid if > 0 & width_on_ == true
  bool stroke_on_;
  bool fill_on_; // true means there is fill info.
  bool width_on_;
    
public:
  // Constructors:
  svg_style();
  svg_style(const svg_color& stroke, const svg_color& fill, unsigned int width);

  // Set svg_style member functions
  // to set fill color and stroke color & width.
  svg_style& stroke_color(const svg_color& col) ;
  svg_style& fill_color(const svg_color& col);
  svg_style& stroke_width(double width);
  svg_style& fill_on(bool is);
  svg_style& stroke_on(bool is);
  svg_style& width_on(bool is);

  // Get svg_style member functions
  // to return fill color and stroke color & width.
  svg_color fill_color() const;
  svg_color stroke_color() const;
  double stroke_width() const;
  bool fill_on() const;
  bool stroke_on() const;
  bool width_on() const;

  void write(std::ostream& os); // Output to file or stream.

  // operators useful for testing at least.
  bool operator==(svg_style& s);
  bool operator!=(svg_style& s);

}; // class svg_style


// class svg Definitions.

  svg_style::svg_style(const svg_color& stroke, const svg_color& fill, unsigned int width)
  :
  stroke_(stroke), fill_(fill), width_(width),
  stroke_on_(false), fill_on_(false), width_on_(false)
  { // Construct with specified fill and stroke colors, and width.
  }

  svg_style::svg_style()
  :
  stroke_(svg_color(0, 0, 0)), // == black.
  fill_(blank), 
  width_(0), // No width specified.
  fill_on_(false), stroke_on_(false), width_on_(false)
  { // Default constructor initialises all private data.
  }

  svg_color svg_style::fill_color() const
  {
    return svg_color(fill_); 
  }

  svg_color svg_style::stroke_color() const
  {
    return svg_color(stroke_);
  }

  double svg_style::stroke_width() const
  { 
    return width_;
  }

  bool svg_style::fill_on() const
  {
    return fill_on_;
  }

  svg_style& svg_style::fill_on(bool is) 
  { // fill is wanted.
    fill_on_ = is;
    return *this; // Make chainable.
  }

  bool svg_style::stroke_on() const
  {
    return stroke_on_;
  }

  svg_style& svg_style::stroke_on(bool is) 
  {
    stroke_on_ = is;
    return *this; // Make chainable.
  }

  bool svg_style::width_on() const
  {
    return width_on_;
  }

  svg_style& svg_style::width_on(bool is) 
  {
    width_on_ = is;
    return *this; // Make chainable.
  }

  // Set svg_style member functions to set fill, stroke & width.
  svg_style& svg_style::stroke_color(const svg_color& col) 
  { 
      stroke_ = col;
      stroke_on_ = true; // Assume want a stroke if color is set.
      return *this; // Make chainable.
  }

  svg_style& svg_style::fill_color(const svg_color& col)
  { 
      fill_ = col;
      fill_on_ = ! col.is_blank; // If blank fill is off or "none".
      return *this; // Make chainable.
  }

  svg_style& svg_style::stroke_width(double width)
  { 
      width_ = width;
      width_on_ = ((width > 0) ? true : false);
      return *this; // Make chainable.
  }
 
  bool svg_style::operator==(svg_style& s)
  { 
     return (s.fill_color() == fill_)
       && (s.stroke_color() == stroke_)
       && (s.stroke_width() == width_)
       && (s.fill_on() == fill_on_)
       && (s.stroke_on() == stroke_on_)
       && (s.width_on() == width_on_);       
   }

  bool svg_style::operator!=(svg_style& s)
   { 
     return (s.fill_color() != fill_)
       || (s.stroke_color() != stroke_)
       || (s.stroke_width() != width_)
       || (s.fill_on() != fill_on_)
       || (s.stroke_on() != stroke_on_)
       || (s.width_on() != width_on_);       
   }

  std::ostream& operator<< (std::ostream& os, svg_style& s)
  {  //
      os << "svg_style("
         << s.fill_ << ", "
         << s.stroke_ << ", "
         << s.width_ << ", " // italic
         << ((s.fill_on_) ? "fill, " : "no fill, ")
         << ((s.stroke_on_) ? "stroke, " : "no stroke, ")
         << ((s.fill_on_) ? "width)" : "no width)");
    // Usage: svg_style my_svg_style; cout << my_svg_style << endl;
    // Outputs:  svg_style(RGB(0,0,0), RGB(0,0,0), 0, no fill, no stroke, no width)
    return os;
  } // std::ostream& operator<<

  void svg_style::write(std::ostream& os)
  { // Write any stroke, fill colors and/or width info (start with space).
    if(stroke_on_)
    {
        os << " stroke=\"";
        stroke_.write(os);
        os << "\"";
    }
    if(fill_on_) //  && (fill_ != blank))
    { // Don't add fill info if color is blank.
        os << " fill=\"";
        fill_.write(os);
        os << "\"";
    }

    if(width_on_ && (width_ > 0))
    { // We never want a 0 (or <0) width output?
        os << " stroke-width=\""
            << width_
            << "\"";
    }
   // Examples: <g id="yMinorTicks" stroke="rgb(0,0,0)" stroke-width="1">
 } // void write


class text_style
{
  friend std::ostream& operator<< (std::ostream&, const text_style&);
  friend bool operator== (const text_style&, const text_style&);
  friend bool operator!= (const text_style&, const text_style&);

private:
  int font_size_;
  std::string font_family_;
  std::string weight_;
  std::string style_;
  std::string stretch_;
  std::string decoration_;

public: 
  text_style(int size = 20,  
  const std::string& font = "", // Examples: "Arial", "Times New Roman", "Verdana", "Lucida Sans Unicode"
  const std::string& style = "", // font-style: normal | bold | italic | oblique
  const std::string& weight = "", // Examples: "bold", "normal"  
  const std::string& stretch = "", // font-stretch: normal | wider | narrower ...
  const std::string& decoration = ""); // Examples: "underline" | "overline" | "line-through"
  
  text_style& font_size(unsigned int i);
  text_style& font_family(const std::string& s);
  text_style& font_style(const std::string& s);
  text_style& font_weight(const std::string& s);
  text_style& font_stretch(const std::string& s);
  text_style& font_decoration(const std::string& s);

  int font_size() const;
  const std::string& font_family() const;
  const std::string& font_style() const;
  const std::string& font_weight() const;
  const std::string& font_stretch() const;
  const std::string& font_decoration() const;

  // Comparison operators useful for testing at least.
  bool operator==(const text_style& ts);
  bool operator!=(const text_style& ts);
  // bool operator==(const text_style& lhs, const text_style& rhs);

}; //   class text_style

// class text_style function *Definitions* separated.

  text_style::text_style(
    int size,  
    const std::string& font , // Default for browser is sans with Firefox & IE but serif with Opera.
    const std::string& style, // font-style: normal | bold | italic | oblique
    const std::string& weight, // Examples: "bold", "normal" 
    const std::string& stretch, // font-stretch: normal | wider | narrower ...
    const std::string& decoration) // Examples: "underline" | "overline" | "line-through"
  : // Constructor.
  font_size_(size),
  font_family_(font),
  style_(style),
  weight_(weight),
  stretch_(stretch),
  decoration_(decoration)
  { // text_style default constructor, defines defaults for all private members.
  }

  int text_style::font_size() const
  {
    return font_size_;
  }

  text_style& text_style::font_size(unsigned int i)
  { // pixels, default 10.
    font_size_ = i;
    return *this; // Should be chainable (but isn't?).
    // error C2663: 'boost::svg::text_style::font_size' : 2 overloads have no legal conversion for 'this' pointer
    // label_style.font_size(20).font_family("sans");
    // MS docs say:
    // This error can be caused by invoking a non-const member function on a const object.
    // So removed const from text_style& font_size(unsigned i) and all others below.
  }

  const std::string& text_style::font_family() const
  {
    return font_family_;
  }

  text_style& text_style::font_family(const std::string& s)
  { // Default for browser is sans with Firefox & IE but serif with Opera.
    // Examples: "Arial", "Times New Roman", "Verdana", "Lucida Sans Unicode"
    // http://www.croczilla.com/~alex/conformance_suite/svg/text-fonts-01-t.svg
    // tests three styles of font, serfi, sans-serif and mono-spaced.
		//	<text font-family="Georgia, 'Minion Web', 'Times New Roman', Times, 'MS PMincho', Heisei-Mincho, serif " x="20" y="80">A serifed face</text>
		//	<text font-family="Arial, 'Arial Unicode', 'Myriad Web', Geneva, 'Lucida Sans Unicode', 'MS PGothic', Osaka, sans-serif " x="20" y="160">A sans-serif face</text>
		//	<text font-family="'Lucida Console', 'Courier New', Courier, Monaco, 'MS Gothic', Osaka-Mono, monospace" x="20" y="240">A mono (iW) face</text>
    // Helvetica,sans-serif,
    font_family_ = s;
    return *this;
  }

  const std::string& text_style::font_style() const
  { // font-style: normal | bold | italic | oblique
    return style_; // example "normal"
  }

  text_style& text_style::font_style(const std::string& s)
  { // Examples: "italic"
    // http://www.croczilla.com/~alex/conformance_suite/svg/text-fonts-02-t.svg
    style_ = s;
    return *this;
  }

  const std::string& text_style::font_weight() const
  {
    return weight_;
  }

  text_style& text_style::font_weight(const std::string& s)
  { // svg font-weight: normal | bold | bolder | lighter | 100 | 200 .. 900
    // Examples: "bold", "normal" 
    // http://www.croczilla.com/~alex/conformance_suite/svg/text-fonts-02-t.svg
    // tests conformance.  Only two weights are supported by Firefox, Opera, Inkscape
    weight_ = s;
    return *this;
  }

  const std::string& text_style::font_stretch() const
  {
    return stretch_;
  }

  text_style& text_style::font_stretch(const std::string& s)
  { // Examples: "wider" but implementation?
    // font-stretch: normal | wider | narrower ...
    stretch_ = s;
    return *this;
  }

  const std::string& text_style::font_decoration() const
  {
    return decoration_;
  }

  text_style& text_style::font_decoration(const std::string& s)
  { // Examples: "underline" | "overline" | "line-through"
    decoration_ = s; // But implementation varies.
    // http://www.croczilla.com/~alex/conformance_suite/svg/text-deco-01-b.svg
    // tests line-through and underline.  (Firefox 3 Beta 4 fails)
    return *this;
  }

  // http://www.croczilla.com/~alex/conformance_suite/svg/text-align-02-b.svg
  // tests for baseline shifted text.  This is needed for subscript and superscript,
  // vital for nice display of units like m^2 and chemical formulae like H2O
  // IE (Adobe SVG viewer) and Opera conforms but not Firefox.

  // operators needed for testing at least.
  bool text_style::operator==(const text_style& ts)
  {
   return (ts.font_size_ == font_size_)
     && (ts.font_family_ == font_family_)
     && (ts.stretch_ == stretch_)
     && (ts.style_ == style_)
     && (ts.weight_ == weight_)
     && (ts.decoration_ == decoration_);
  } // operator==

  bool text_style::operator!=(const text_style& ts)
  {
   return (ts.font_size_ != font_size_)
     || (ts.font_family_ != font_family_)
     || (ts.stretch_ != stretch_)
     || (ts.style_ != style_)
     || (ts.weight_ != weight_)
     || (ts.decoration_ != decoration_);
  } //  operator!=

  //bool text_style::operator==(const text_style& lhs, const text_style& rhs)
  //{ // Note operator== and operator << both needed to use Boost.text.
  //   return (lhs.font_size_ == rhs.font_size_)
  //     && (lhs.font_family() == rhs.font_family())
  //     && (lhs.stretch_ ==  rhs.stretch_)
  //     && (lhs.style_ ==  rhs.style_)
  //     && (lhs.weight_ ==  rhs.weight_)
  //     && (lhs.decoration_ ==  rhs.decoration_);
  //} // operator==

//bool operator!= (const text_style& lhs, const text_style& rhs)
//{ // Note operator== and operator << both needed to use Boost.Test.
// But can be avoided with a macro whose name I forget.
//  return false;
//}

std::ostream& operator<< (std::ostream& os, const text_style& ts)
{  //
    os << "text_style("
       << ts.font_size_ << ", \""
       << ts.font_family_ << "\", \""
       << ts.style_ << "\", \"" // italic
       << ts.weight_ // bold
       // enable if implemented by rendering programs.
       << "\", \""
       << ts.stretch_ << "\", \""
       << ts.decoration_ 
       << "\")" ;
  // Usage: text_style ts(12, "Arial", "italic", "bold", "", "");  cout << t << endl; 
  // Outputs:  text_style(18, "Arial", "italic", "bold", "", "")

  return os;
} // std::ostream& operator<<

text_style no_style; // Uses all constructor defaults.

class value_style
{ // Data series point value information, text, color, uncertainty, orientation.
  // for example to output: 5.123 +- 0.01 (19)
public:
  //private:  // ??
  rotate_style value_label_rotation_; // Direction point value labels written.
  int value_precision_; // Precision of value.
  std::ios_base::fmtflags value_ioflags_; // Control of scientific, fixed etc.
  bool strip_e0s_;
  text_style values_text_style_; // Font etc used for data point value marking.
  // svg_style 
  svg_color fill_color_;
  svg_color stroke_color_;
  bool plusminus_on_; // If an uncertainty is to be appended.
  // http://en.wikipedia.org/wiki/Plus-minus_sign
  bool df_on_; // If an degrees of freedom estimate is to be appended.

  public:
    value_style()
    :
    value_label_rotation_(horizontal),
    value_precision_(3), // Reduced from default of 6 which is usually too long.
    value_ioflags_(std::ios::dec),
    strip_e0s_(true), // See also similar x_ticks value 
    values_text_style_(no_style),  // All defaults, black etc.
    stroke_color_(black), // == black.
    fill_color_(svg_color(0, 0, 0)), // == black.
    plusminus_on_(false),
    df_on_(false)
    { // Default constructor initialises all private data.
    }

    value_style(rotate_style r, int p,  std::ios_base::fmtflags f, bool s,
      text_style ts, const svg_color& scol = black, svg_color fcol = black, bool pm = false, bool df = false)
    :
    value_label_rotation_(r), value_precision_(p), value_ioflags_(f), strip_e0s_(s),
    values_text_style_(ts), stroke_color_(scol), fill_color_(fcol), plusminus_on_(pm), df_on_(df)
    { // Constructor.
    }

}; // class value_style 

enum point_shape
{ // Marking a data point.
  // Used in draw_plot_point in axis_plot_frame.hpp
  none = 0,
  round,
  // name circle changed to round to avoid clash with function named circle.
  square, point, egg, // ellipse
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
  symbol //Unicode symbol including letters, digits, greek & 'squiggles'.
  // Default "X", "&#x3A9;"= greek omega, "&#x2721;" = Star of David hexagram
  // &#2720 Maltese cross & other dingbats
  // http://en.wikipedia.org/wiki/List_of_Unicode_characters#Basic_Latin geometric shapes
  // may be a better way to make these symbols.
  // &#25A0 black square ...to &#25FF 
  // But unclear how many browsers implement these properly.
}; 

class plot_point_style
{
  friend std::ostream& operator<< (std::ostream&, plot_point_style);

public:
  svg_color fill_color_; // Color of the centre of the shape.
  svg_color stroke_color_; // Color of circumference of shape.
  int size_; // diameter of circle, height of square, font_size  ...
  point_shape shape_; // round, square, point...
  std::string symbols_; // Unicode symbol (letters, digits, squiggles etc)
  // Caution: not all Unicode symbols are output by all browsers! Example:
  // U2721 is Star of David or hexagram http://en.wikipedia.org/wiki/Hexagram
  // symbols("&#x2721;")
  text_style symbols_style_;
  bool show_x_value_; // Show the X value near the point.
  bool show_y_value_;  // Show the Y value near the point.
  // If both true, then shows both as a pair "1.2, 3.4"
  //int dist; // from the point to the value.
  //rotate_style orient_; // Orientation of the value from the point.
  //// Note that this needs to alter the text alignment, center, left or right,
  //// to avoid writing over the point marker.
  //rotate_style rotation_; // Rotation of the value text string itself.
  //// Note that this also needs to alter the text alignment, center, left or right,
  //// to avoid writing over the point marker.
  //text_style value_style_; // Size, font, color etc of the value.

  plot_point_style(const svg_color& stroke = black, const svg_color& fill = blank,
    int size = 10, point_shape shape = round, const std::string& symbols = "X")
    :
  fill_color_(fill), stroke_color_(stroke), size_(size), shape_(shape), symbols_(symbols),
    show_x_value_(false), show_y_value_(false)
  { // Best to have a fixed-width font for symbols?
    symbols_style_.font_family("Lucida Sans Unicode");
    symbols_style_.font_size(size);
  }

  plot_point_style& size(int i)
  {
    size_ = i;
    symbols_style_.font_size(i); // in case using a symbol.
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

  plot_point_style& symbols(const std::string s)
  { // Override default symbol "X" - only effective if .shape(symbol) used.
    symbols_ = s;
    return *this;
  }

  std::string& symbols()
  {
    return symbols_;
  }

  plot_point_style& style(text_style ts)
  {
    symbols_style_ = ts;
    return *this;
  }

  text_style& style() const
  { // To allow control of symbol font, size, decoration etc.
    return const_cast<text_style&>(symbols_style_); 
  }
}; // struct plot_point_style

std::ostream& operator<< (std::ostream& os, plot_point_style p)
{  //
  os << "point_style("
     << p.shape_ << ", "
     << p.stroke_color_ << ", "
     << p.fill_color_ << ", "
     << p.size_ << ", "
     << p.symbols_ << ")" ;
// Usage: plot_point_style p;  cout << p << endl; 
// Outputs:  point_style(1, RGB(0,0,0), RGB(0,0,0), 10, X)
  return os;
} // std::ostream& operator<<

plot_point_style default_plot_point_style(); // Uses all the defaults.

class plot_line_style
{ // Style of line joining data series values. 
  // TODO dotted and dashed line style?  Useful for B&W?
public:
    svg_color stroke_color_; // line stroke color. (no fill color for lines)
    svg_color area_fill_; // Fill color from line to axis. == false means color.is_blank = true, or = blank.
    double width_;
    bool line_on_;
    bool bezier_on_;

    plot_line_style(const svg_color& col = black, const svg_color& fill_col = blank, double width = 2, bool line_on = true, bool bezier_on = false)
    :
    stroke_color_(col), area_fill_(fill_col), width_(width), line_on_(line_on), bezier_on_(bezier_on)
    { // Defaults for all private data.
    }

  plot_line_style& width(double w)
  {
    width_ = w;
    return *this; // Make chainable.
  }

  double width()
  {
    return width_;
  }

  plot_line_style& color(const svg_color& f)
  {
    stroke_color_ = f;
    return *this; // Make chainable.
  }

  svg_color& color()
  {
    return stroke_color_;
  }

  plot_line_style& area_fill(const svg_color& f)
  {
    area_fill_ = f;
    return *this; // Make chainable.
  }

  svg_color& area_fill()
  {
    return area_fill_;
  }

  bool line_on() const
  {
    return line_on_;
  }

  plot_line_style& line_on(bool is) 
  {
    line_on_ = is;
    return *this; // Make chainable.
  }

  bool bezier_on() const
  {
    return bezier_on_;
  }

  plot_line_style& bezier_on(bool is) 
  {
    bezier_on_ = is;
    return *this; // Make chainable.
  }

}; // class plot_line_style

std::ostream& operator<< (std::ostream& os, plot_line_style p)
{  //
  os << "point_line_style("
     << p.stroke_color_ << ", "
     << p.area_fill_ << " area fill, "
     << ((p.line_on_) ? "line, " : "no line, ")
     << ((p.bezier_on_) ? "bezier)" : "no bezier)");
// Usage: plot_line_style p;   cout << p << endl; 
// Outputs:  point_line_style(RGB(0,0,0), blank, line, )
  return os;
} // std::ostream& operator<<

enum dim{N = 0, X = 1, Y = 2, Z = 3}; // Used so that an axis knows what type it is, or none = N.

class axis_line_style
{ // Style of the x and/or y axes lines.
  // But NOT the ticks and value labels.
  // (Different styles for x and y are possible).
public:
  dim dim_; // x, y or z
  double min_; // minimum x value (Cartesian units).
  double max_; // maximum x value (Cartesian units).
  // Note that these duplicate the same named in ticks_labels_style,
  // but they might have different uses, so are left pro tem.
  // TODO reconsider the implications of this (largely accidental) decision.
  // double interval_; does NOT duplicate major_interval_ in ticks_label_style.
  double axis_; //
  // X-axis (y = 0) transformed into SVG Y coordinates. -1 if not calculated yet.
  // or Y-axis (x = 0) transformed into SVG X coordinates. -1 if not calculated yet.

  svg_color color_; // line stroke color.
  double axis_width_; // line width.
  int axis_position_; // How the axes intersect with values as below:
  // enum x_axis_intersect {bottom = -1, x_intersects_y = 0, top = +1};
  // enum y_axis_intersect {left = -1, y_intersects_x = 0, right = +1};
  // If axes look like an L, then is bottom left.
  // If a T then y intersects and x is at bottom.
  bool label_on_; // Label X-axis with text - example: "length".
  bool label_units_on_; // Label X-axis units, example: "cm".
  bool axis_line_on_; // Draw a X horizontal or Y vertical axis line.

  axis_line_style(dim d = X,
    double min = -10., double max = +10., // Defaults.
    // See also default in ticks_labels_style.
    const svg_color col = black, double width = 1,
    int axis_position = 0, bool label_on = true,
    bool label_units_on = false,
    bool axis_lines_on = true)
    :
    dim_(d), min_(min), max_(max), color_(col), axis_width_(width),
    axis_position_(axis_position),
    label_on_(label_on),
    label_units_on_(label_units_on), // default is include units.
    axis_line_on_(axis_lines_on),
    axis_(-1) // Not calculated yet.
 { // Initialize all private data.
    if(max_ <= min_)
    { // max_ <= min_.
      throw std::runtime_error("Axis range: max <= min!");
    }
    if((max_ - min_) < std::numeric_limits<double>::epsilon() * 1000)
    { // Range too small to display.
      throw std::runtime_error("Axis range too small!" );
    }
 } // axis_line_style constructor

  // Set and get functions.
  axis_line_style& color(const svg_color& color)
  {
    color_ = color;
    return *this; // Make chainable.
  }

  svg_color color()
  {
    return color_;
  }
   
  axis_line_style& width(double w)
  {
    axis_width_ = w;
    return *this; // Make chainable.
  }

  double width()
  {
    return axis_width_;
  }

  bool label_on() const
  {
    return label_on_;
  }

  axis_line_style& label_on(bool is) 
  {
    label_on_ = is;
    return *this; // Make chainable.
  }

  bool label_units_on() const
  {
    return label_units_on_;
  }

  axis_line_style& label_units_on(bool is) 
  {
    label_units_on_ = is;
    return *this; // Make chainable.
  }
}; // class axis_line_style


class ticks_labels_style
{ // Style of the x and y axes ticks, grids and their value labels.
  // But NOT the x and y axes lines.
  // These can be either on the axis lines or on the plot window edge(s).
  // (Different styles for x and y are possible).
  friend class svg_2d_plot;

public:
    dim dim_;
    double min_; // minimum x value (Cartesian units).
    double max_; // maximum x value (Cartesian units).
    double major_interval_; // x_major_interval_ is the stride or interval for major x ticks.
    // (Cartesian units)  set/get by x or y _major_interval
    double minor_interval_; // Interval (Cartesian units) between minor ticks.
    // No set function because x_num_minor_ticks_ used to determine this instead,
    // but one could calculate x_minor_interval_.
    unsigned int num_minor_ticks_; // number of minor ticks, eg 4 gives major 0, minor 1,2,3,4, major 5
    // All units in pixels.
    svg_color major_tick_color_; // line stroke color.
    double major_tick_width_;
    double major_tick_length_;
    svg_color minor_tick_color_; // line stroke color.
    double minor_tick_width_;
    double minor_tick_length_;
    svg_color major_grid_color_;
    double major_grid_width_; // pixels.
    svg_color minor_grid_color_;
    double minor_grid_width_; // pixels.

    bool up_ticks_on_; // Draw ticks up from horizontal X-axis line.
    bool down_ticks_on_; // Draw ticks down from horizontal X-axis line.
    bool left_ticks_on_; // Draw ticks left from vertical Y-axis line.
    bool right_ticks_on_; // Draw ticks right from vertical Y-axis line.
    // Simplest to have all of these although only one pair (up or down) or (left or right) is used.
    // Unused are always false.
    int major_value_labels_side_; // Label values for major ticks, and direction.
    // < 0 means to left or down (default), 0 (false) means none, > 0 means to right (or top)/
    rotate_style label_rotation_; // Direction axis value labels written.
    bool major_grid_on_;  // Draw X grid at major ticks.
    bool minor_grid_on_;// Draw X grid at minor ticks.
    int value_precision_; // precision for tick value labels, usually 3 will suffice.
    std::ios_base::fmtflags value_ioflags_;  // IO formatting flags for the axis default std::ios::dec.
    bool strip_e0s_; // If redundant zero, + and e are to be stripped.
    double label_max_length_;  // width (in SVG units) of longest value label text on axis.
    double label_max_space_;  // Space needed for value label adjusted for rotation.
    int ticks_on_window_or_axis_; // Value labels & ticks on the plot window border 
    // (rather than on X or Y-axis).
    // For Y-axis -1 = left, 0 = false, +1 = right. Default -1 to left of plot window.
    // For X-axis -1 = bottom, 0 = false, +1 = top. Default -1 below bottom of plot window.
    const text_style& value_label_style_;

    ticks_labels_style(dim d = X,
      const text_style& style = no_style,
      double max = 10., double min = -10.,
      double major_interval = 2.,
      unsigned int num_minor_ticks = 4)
      : // Constructor.
      dim_(d),
      value_label_style_(style),
      max_(max), min_(min),
      major_interval_(major_interval),
      num_minor_ticks_(num_minor_ticks),
    // Other data can be changed by set functions.

    // Initialize all private data.
    // These are the defaults.
    //major_interval_(2.), // x stride between major ticks & value label.
    //num_minor_ticks_(4), // suits: major 0, minor 2, 4, 6, 8, major 10
    minor_interval_(0), // Calculated from x & y_num_minor_ticks_
    // but given a value here for safety.
    major_tick_color_(black), // line stroke color.
    major_tick_width_(2),
    major_tick_length_(5),
    minor_tick_color_(black), // line stroke color.
    minor_tick_width_(1),
    minor_tick_length_(2),
    major_grid_color_(svg_color(200, 220, 255)),
    minor_grid_color_(svg_color(200, 220, 255)),
    major_grid_width_(1.), // pixels.
    minor_grid_width_(0.5), // pixels.
    up_ticks_on_(false), // Draw ticks up from horizontal X-axis line.
    down_ticks_on_(true), // Draw ticks down from horizontal X-axis line.
    left_ticks_on_(true), // Draw ticks left from vertical Y-axis line.
    right_ticks_on_(false), // Draw ticks right from vertical Y-axis line.
    // Simplest to have all of these although only one pair like up or down is used.
    // Unused are always false.
    major_value_labels_side_(-1), // Label values side for major ticks left (right or none).
    label_rotation_(horizontal), // Direction axis value labels written.
    major_grid_on_(false),  // Draw grid at major ticks.
    minor_grid_on_(false),// Draw grid at minor ticks.
    value_precision_(3), // precision for tick value labels, usually 3 will suffice.
    value_ioflags_(std::ios::dec),  // IO formatting flags for the axis.
    // Note that ALL the flags are set, overwriting any defaults, so std::dec is wise.
    strip_e0s_(true), // strip superflous zeros and signs.
    label_max_length_(0.), // length (estimated in SVG units) of longest label on axis.
    label_max_space_(0.), // Space (estimated in SVG units) of longest label on axis
    // adjusted for rotation.
    ticks_on_window_or_axis_(-1) // Value labels & ticks on the plot window
    // rather than on X or Y-axis.
    // Default -1 means left or bottom.
  {
      if(max_ <= min_)
      { // max_ <= min_.
        throw std::runtime_error("Axis ticks & labels range: max <= min!");
      }
      if((max_ - min_) < std::numeric_limits<double>::epsilon() * 1000)
      { // Range too small to display.
        throw std::runtime_error("Axis ticks & labels range too small!" );
      }
  } // ticks_labels_style constructor.

  double label_length(double value)
  { // Find the length of label for a value.
    // Needs to know the IO precision & flags for the axis,
    // and if zeros are to be stripped, so can't be a free function.
    std::stringstream label;
    label.precision(value_precision_);
    label.flags(value_ioflags_);
    label << value; // "1.2" or "3.4e+000"...
    double r;
    if (strip_e0s_)
    { // Do want to strip unecessary e, +, & leading exponent zeros.
      std::string stripped = strip_e0s(label.str());
      r = string_svg_length(stripped, value_label_style_);
      // want x_or y_value_label_style_ here!
      return r;
    }
    r = string_svg_length(label.str(), value_label_style_);
    return r;
  } // double label_length

  double longest_label()
  { // Update label_max_length_ with the longest value label as pixels,
    // return the count of digits etc.
    if(major_value_labels_side_ != 0) // ! none
    { // Show values by the tick as "1.2" or "3.4e+000"...
      double longest = 0;
      
      //axis_line_style& axis = (dim_ = X) ? y_axis() : x_axis(); // The intersecting *other* axis.
      //  || !axis.axis_line_on_ ignore these tests to avoid the above that doesn't work.
      // Zero isn't likely to be the longest value label.
      // ticks_labels_style& tick_labels = (dim_ = X) ? x_ticks() : y_ticks(); // doesn't work.
      // 

      // Check length of label for the ticks on the positive side (right or above zero).
      for(double v = 0.; v <= max_; v += major_interval_)
      {
        if (v != 0. || ticks_on_window_or_axis_)
        { // Avoid a major tick at x == 0 where there *is* a vertical Y-axis line,
          // or avoid a major tick at y == 0 where there *is* a horizontal X-axis line.
          // (won't be a Y-axis line for 1-D,
          // where both the zero tick & value label is always wanted).
          double l = label_length(v);
          if (l > longest)
          {
            longest = l;
          }
        }
      } // for v
      // Check length of label of the ticks on the negative side (left of zero).
      for(double v = 0.; v >= min_; v -= major_interval_)
      {
        if (v != 0. || ticks_on_window_or_axis_)
        { // Avoid a major tick at x == 0 where there *is* a vertical Y-axis line.
          // (won't be Y-axis line for 1-D where the zero tick is always wanted).
          // But no tick means no value label 0 either unless on_plot_window.
          double l = label_length(v);
          if (l > longest)
          {
            longest = l;
          }
        }
      } // for v
      label_max_length_ = longest;
      return longest; 
    }
    else
    {
      label_max_length_ = 0;
      return 0;
    }
  } // longest_label()


  bool use_up_ticks() const
  {
    return up_ticks_on_;
  }

  ticks_labels_style& use_up_ticks(bool is) 
  {
    up_ticks_on_ = is;
    return *this; // Make chainable.
  }

  bool use_down_ticks() const
  {
    return down_ticks_on_;
  }

  ticks_labels_style& use_down_ticks(bool side) 
  {
    down_ticks_on_ = side;
    return *this; // Make chainable.
  }

  int major_value_labels_side() const
  { // Get tick value labels to left (<0), none (==0) or right (>0).
    return major_value_labels_side_;
  }

  ticks_labels_style& major_value_labels_side(int is) 
  { // Set tick value labels to left (<0), none (==0) or right (>0).
    major_value_labels_side_ = is;
    return *this; // Make chainable.
  }

}; // class ticks_labels_style

class box_style
{ // Style of a rectangular box.
  // Use for image and plot window.
public:
    svg_color stroke_; // box line stroke color.
    svg_color fill_; // box fill color.
    double width_; // plot border rectangle width.
    double margin_; // Marginal (pixels) space around the box (inside or out).
    bool border_on_; // Display the border.
    bool fill_on_; // Color fill the box.

    box_style(const svg_color& scolor = black,
      const svg_color& fcolor = white, // No fill.
      double width = 1, // of border
      double margin = 4., // 
      bool border_on = true, // Draw a border of width.
      bool fill_on = false) // Apply fill color.
      :
    stroke_(scolor), fill_(fcolor), width_(width),
    margin_(margin),
    border_on_(border_on),
    fill_on_(fill_on)
    { // Initializes all private data with defaults.
    }

  box_style& stroke(const svg_color& color)
  {
    stroke_ = color;
    return *this; // Make chainable.
  }

  svg_color stroke()
  {
    return stroke_;
  }

  box_style& fill(const svg_color& color)
  {
    fill_ = color;
    return *this; // Make chainable.
  }

  svg_color fill()
  {
    return fill_;
  }

  box_style& width(double w)
  {
    width_ = w;
    return *this; // Make chainable.
  }

  double width()
  {
    return width_;
  }

  box_style& margin(double w)
  {
    margin_ = w;
    return *this; // Make chainable.
  }

  double margin()
  {
    return margin_;
  }

  bool border_on() const
  {
    return border_on_;
  }

  box_style& border_on(bool is) 
  {
    border_on_ = is;
    return *this; // Make chainable.
  }  
  
  bool fill_on() const
  {
    return fill_on_;
  }

  box_style& fill_on(bool is) 
  {
    fill_on_ = is;
    return *this; // Make chainable.
  }

}; // class box_style

enum bar_option
{
  y_block = -2, // Rectangular (optionally filled) block style horizontal to Y-axis,
  y_stick = -1, // Bar or row line (stroke width) horizontal to Y-axis.
  no_bar = 0, // No bar.
  x_stick = +1, // Stick or column line (stroke width) vertical to X-axis.
  x_block = +2  // Rectangular (optionally filled) block style vertical to X-axis,
  // Other options like cylinders and cones might be added here?
  // x_cyl = +3, x_cone = +4 ...
};

enum histogram_option
{
  //row = -1, // Row line (stroke width) horizontal to Y-axis. Not implemented.
  // See svg_2d_plot for details of why not.
  no_histogram = 0,
  column = +1 // Stick or column line (stroke width) vertically to/from X-axis.
  // Column is the most common histogram style.
};

class histogram_style
{
public:
  histogram_option histogram_option_; // bar, no_histogram or column.

  histogram_style(histogram_option opt = no_histogram)
  :
  histogram_option_(opt)
  { // Default for all private data.
    // Line width and area-fill are taken from the plot_line_style style.
  }

  histogram_style& histogram(histogram_option opt)
  { // stick or bar.
    histogram_option_ = opt;
    return *this; // Make chainable.
  }

  double histogram()
  { // 
    return histogram_option_;
  }

}; // class histogram_style

class bar_style
{
public:
  svg_color color_; // line stroke color. (no fill color for lines).
  svg_color area_fill_; // Fill color from line to axis. == true means color.blank = true.
  double width_; // of bar, not enclosing line width.
  bar_option bar_option_; // stick or bar.

  bar_style(const svg_color& col = black, const svg_color& acol = true, double width = 2, bar_option opt = no_bar)
  :
  color_(col), area_fill_(acol), width_(width),  bar_option_(opt)
  { // Defaults for all private data.
  }

  bar_style& width(double w)
  { // of bar, not the enclosing line (stroke) width.
    width_ = w;
    return *this; // Make chainable.
  }

  double width()
  { // of bar, not enclosing line width.
    return width_;
  }

  bar_style& color(const svg_color& f)
  { // of line or enclosing line.
    color_ = f;
    return *this; // Make chainable.
  }

  svg_color& color()
  { // of line or enclosing line.
    return color_;
  }

  bar_style& area_fill(const svg_color& f)
  { // rectangle fill color.
    area_fill_ = f;
    return *this; // Make chainable.
  }

  svg_color& area_fill()
  { // rectangle fill color.
    return area_fill_;
  }

  bar_style& bar(bar_option option)
  { // stick or bar.
    bar_option_ = option;
    return *this; // Make chainable.
  }

  double bar_option()
  { // stick or bar.
    return bar_option_;
  }

}; // class bar_style

const std::string strip_e0s(std::string s);

const std::string strip_e0s(std::string s)
{ // Ugly hack to remove unwanted sign and leading zero(s) in exponent.
  // Use to work out the longest value label before calculate_plot_window.
  // Should also be useful for values that spill over into exponent format
  // 'by accident' - when leading zeros are likely.
  // For example, "1.2e+000" becomes "1.2"
  // (Could also do the same for uppercase E cases).
  // (Considered doing a repeated strip but complicated).

  using std::string;
  size_t j = s.find("e+000");
  if (j != string::npos)
  {
    s.erase(j, 5); // remove "e+000"
    return s;
  }
  j = s.find("e-000");
  if (j != string::npos)
  {
    s.erase(j, 5); // remove "e-000"
    return s;
  }
  j = s.find("e+00");
  if (j != string::npos)
  {
    s.erase(j + 1, 3); // remove "+00", leave d
    return s;
  }

  j = s.find("e-00");
  if (j != string::npos)
  {
    s.erase(j+2, 2); // remove "00", leave "-"
    return s;
  }
  j = s.find("e+0");
  if (j != string::npos)
  {
    s.erase(j + 1, 2); // remove "+0", leave "dd"
    return s;
  }
  j = s.find("e-0");
  if (j != string::npos)
  {
    s.erase(j+2, 1); // remove "0", leave "-dd"
  }
  return s; // Perhaps unchanged.
} // const std::string strip(double d)

 static const double wh = 0.7; // font text width/height ratio.
  // Even after reading http://www.w3.org/TR/SVG/fonts.html, unclear how to
  // determine the exact width of digits, so an 
  // arbitrary average width height ratio wh = 0.7 is used as a good approximation.

double string_svg_length(const std::string& s, const text_style& style)
{ // Return length of string in SVG units depending on font size etc.
  // If possible use an actual length, but probably platform and/or browser-dependent,
  // else use average char width,
  // and deal with Unicode, for example &#x3A9; = greek omega, 
  // counting each symbol(s) embedded between & amd ; as one character,
  // and ignore embedded xml like <sub> (not implemented by browsers yet).

 double d = 0.; // Estimated or actual width of resulting svg string.
 bool in_esc = false;
 for (std::string::const_iterator i = s.begin(); i != s.end(); i++)
 {
    if (*i == '&')
    { // Start of Unicode 'escape sequence' &#x3A9;
      in_esc = true;
       while ((*i != ';')
         && (i != s.end())) // In case mistakenly not terminated.
       {
          i++; // Only count &#x3A9; as 1 character wide.
       }
       in_esc = false;
    }
    if (*i == '<')
    {
      in_esc = true;
       while ((*i != '>')
         && (i != s.end())) // In case mistakenly not terminated.
       {
          i++; // Only count <...>; as NO character wide.
       }
       d--;
       in_esc = false;
    }
    d++;
 }
 // std::cout << "string " << s << " has " << d << " characters." << std::endl; 
 return d * style.font_size() * wh;
} // double string_svg_length(


}//svg
}//boost

#endif // BOOST_SVG_SVG_STYLE_HPP
