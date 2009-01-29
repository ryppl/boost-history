/*! \file svg_tag.hpp

   \brief Boost.Plot SVG plot Implemention details.
   \details See svg.hpp etc for user functions.
      svg_tag.hpp defines all classes that can occur in the SVG parse tree.

   \author Jacob Voytko and Paul A. Bristow
*/

// Copyright Jacob Voytko 2007, 2008
// Copyright Paul A Bristow 2007, 2008, 2009

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_TAG_HPP
#define BOOST_SVG_TAG_HPP

#include <boost/ptr_container/ptr_container.hpp>
// using boost::vec_ptr;
#include <boost/array.hpp>
// using boost::array;

#include "../svg_style.hpp"
#include "svg_style_detail.hpp"

#include <ostream>
// using std::ostream;
#include <string>
// using std::string;
#include <vector>
// using std::vector;

namespace boost
{
namespace svg
{
  // Forward declarations of classes defined in this module.
  // Also copied to svg_fwd.hpp.

  class svg_element; // svg_element is base class for:
  // g_element (group element)
  // rect_element, circle_element, line_element, text_element,
  // polyline_element, polygon_element, path_element, clip_path_element,
  class text_parent; // Ancestor to both tspan and strings for the text_element class.
  class text_element_text;
  class tspan_element; // Within a text_element, adjust text and font properties.
  class text_element; // text with position, size, font, (& styles) & orientation.
  class rect_element; // clipping path restricts the region to which paint can be applied.
  class circle_element; // Represents a single circle.
  class ellipse_element; // Represents a single ellipse.
  class line_element; // Represents a single line.
  struct path_point; // Base class for m_path, z_path, q_path, h_path, v_path, c_path, s_path.
  struct poly_path_point; // for polyline & polygon
  class polygon_element; // closed shape consisting of a set of connected straight line segments.
  class polyline_element; // a set of connected straight line segments.
  class path_element; // d= moveto, lineto...
  class clip_path_element; // Restricts the region to which paint can be applied.
  struct m_path; // moveto coordinates (x, y), outputs "M1.2,3.4"
  struct l_path; // lineto coordinates (x, y).
  struct z_path; // z indicates a closepath.
  struct h_path; // Draws a horizontal line from the current point (cpx, cpy) to (x, cpy).
  struct v_path; // Draws a vertical line from the current point (cpx, cpy) to (cpx, y).
  struct c_path; // Draws a cubic Bezier curve from the current point to (x,y) using (x1,y1).
  struct q_path; // Draws a quadratic Bezier curve from the current point to (x,y).
  struct s_path; // Draws a cubic Bezier curve from the current point to (x,y).
  struct t_path; // Draws a quadratic Bezier curve from the current point to (x,y).
  struct a_path; // Draws a elliptical arc from the current point to (x,y).
  struct P_path; // Adds another (absolute) point to a polyline or polygon.
  class g_element; /*! \verbatim
  'g' element is a container element, <g ... /> </g>
  for grouping together related graphics elements, for example:
   <g stroke="rgb(255,0,0)" <rect x="0" y="0"  width="500"  height="600"/> </g>
   \endverbatim
   */

  class svg_element
  { /*! \class boost::svg::svg_element
       \brief svg_element is base class for all the leaf elements.
       \details
       rect_element, circle_element, line_element, text_element,
       polygon_element, polyline_element, path_element, clip_path_element,
       g_element.\n

       g_element ('g' element is a container element
       for grouping together related graphics elements).\n
       See http://www.w3.org/TR/SVG/struct.html#NewDocument 5.2.1 Overview.
    */

  protected:
    svg_style style_info_; //! Colors fill, stroke, width, get by function style.
    std::string id_name_; //! SVG id name, set & get by function id.
    std::string class_name_; //! SVG class name, set & get by function class id.
    std::string clip_name_; //! SVG clip path name, set & get by function clip_id.

    void write_attributes(std::ostream& s_out)
    { //! Output group_element id and clip-path.
      if(id_name_.size() != 0)
      { // Example: id="imageBackground"
        s_out << " id=\"" << id_name_ << "\""; // Prefix with space.
      }
      if(class_name_.size() != 0)
      {
        s_out << " class=\"" << class_name_ << "\"";
      }
      if(clip_name_.size() != 0)
      { // Example: clip-path="url(#plot_window)"
        s_out << " clip-path=\"url(#" << clip_name_ << ")\""; // Prefix with space.
      }
      /*! \details
      \verbatim
        Classes inherited from svg_element add other references, 5.3.1, like color, fill, stroke, gradients...
        Example id: <g id="yMinorGrid" ></g>
        Example class: <g class="grid_style"></g>
        Example URI: fill="url(#Gradient01) // local URL
        \endverbatim
      */
    } // void write_attributes(std::ostream& s_out)

  public:

    svg_element(const svg_style& style_info,
                const std::string& id_name = "",
                const std::string& class_name = "",
                const std::string& clip_name = "")
                :
                style_info_(style_info),
                id_name_(id_name),
                class_name_(class_name),
                clip_name_(clip_name)
    {
    }

    svg_element()
    {
    }

    virtual void write(std::ostream& rhs) = 0;
    virtual ~svg_element()
    {
    }

    bool operator==(const svg_element& lhs)
    { //! Compare svg_elements, useful for Boost.Test.
      return lhs.id_name_ == id_name_;
    }

    bool operator!=(const svg_element& lhs)
    { //! Compare svg_elements for inequality, useful for Boost.Test.
      return lhs.id_name_ != id_name_;
    }

    // Set and get member functions.
    svg_style& style()
    { //! Get reference to svg_style to provide indirect access to colors & width via style().stroke_color(), fill_color(), width()
      return style_info_;
    }

    const svg_style& style() const
    { //! Get reference to const svg_style to provide indirect access to colors & width via style().stroke_color(), fill_color(), width() (const version).
      return style_info_;
    }

    void id(const std::string& id)
    { //! Provide a unique name for an element.
      /*! \details
        See http://www.w3.org/TR/SVG/struct.html#IDAttribute
        5.10.1 Attributes common to all elements: id and xml:base
        The id and xml:base attributes are available on all SVG elements:
        Attribute definitions:
        id = "name"
        Standard XML attribute for assigning a unique name to an element.
        Refer to the "Extensible Markup Language (XML) 1.0" Recommendation [XML10].
        xml:base = "<uri>"
        Specifies a base URI other than the base URI of the document or external entity.
        Refer to the "XML Base" specification [XML-BASE].
        A group of elements, as well as individual objects,
        can be given a name using the id attribute.
        Named groups are needed for several purposes such as animation and re-usable objects.
        Example: id="plotBackground"
      */
      id_name_ = id;
   }

    std::string id()
    { //! Get the unique name for an element, for example id() ="plotBackground".
      return id_name_;
    }

    void class_id(const std::string& class_id)
    { //! Class class id, non-unique identifier for an element.
      /*! \details
        http://www.w3.org/TR/2001/REC-SVG-20010904/styling.html#ClassAttribute
        6.12 Attributes common to all elements: id and xml:base
        Example: class="info"
      */
      class_name_ = class_id;
    }

    std::string class_id()
    { //! Class id, non-unique identifier for an element.
      return class_name_;
    }

    void clip_id(const std::string& id)
    { //! Set name of a clip path, for example: g_ptr.clip_id(plot_window_clip_);
      clip_name_ = id;
    }

    std::string clip_id()
    { //! Get name of a clip path, for example: g_ptr.clip_id(plot_window_clip_);
      return clip_name_;
    }
  }; // class svg_element

  // Derived elements whose write member functions
  // output SVG XML for line, circle, rectangle, text...
  // Reminder: Within a literal C string, \"  is needed to output a " ;-)

  // -----------------------------------------------------------------
  // Represents a line
  // -----------------------------------------------------------------
  class line_element: public svg_element
  { /*! \class boost::svg::line_element
        \brief Line from (x1, x2) to (y1, y2).
    */
  private:
    double x1_; //! Line from (x1_, x2_) to (y1_, y2_)
    double x2_; //! Line from (x1_, x2_) to (y1_, y2_)
    double y1_; //! Line from (x1_, x2_) to (y1_, y2_)
    double y2_; //! Line from (x1_, x2_) to (y1_, y2_)

  public:
    line_element(double x1, double y1, double x2,  double y2)
      :   x1_(x1), y1_(y1),  x2_(x2), y2_(y2)
    {
    }

    line_element(double x1, double y1,
                 double x2, double y2,
                 const svg_style& style_info,
                 const std::string& id_name="",
                 const std::string& class_name="",
                 const std::string& clip_name = "")
                : x1_(x1), y1_(y1), x2_(x2), y2_(y2),
                  svg_element(style_info, id_name, class_name, clip_name)
    {
    }

    void write(std::ostream& rhs)
    { //! output line from (x1_, x2_) to (y1_, y2_)
      //! \details \verbatim Example: <line x1="5" y1="185" x2="340" y2="185"/> \endverbatim
      rhs << "<line x1=\"" << x1_ << "\" y1=\"" << y1_
          << "\" x2=\"" << x2_ << "\" y2=\"" << y2_ << "\"/>";
    }
  }; // class line_element

  class rect_element : public svg_element
  { /*! \class boost::svg::rect_element
        \brief Rectangle from top left coordinate, width and height.
        \details
         Represents a single rectangle.
         http://www.w3.org/TR/SVG/shapes.html#RectElement
    */
    friend bool operator==(const rect_element&, const rect_element&);
    friend bool operator!=(const rect_element&, const rect_element&);

  private:
    double x_; //! x-axis coordinate of the side of the rectangle which has the smaller x-axis coordinate value.
    double y_; //! y-axis coordinate of the side of the rectangle which has the smaller y-axis coordinate value.
    //! So is top left corner of rectangle.
    double width_; //! x + width is top right.
    double height_; //! y + height is bottom left.
    //! x + width and y + height is bottom right.
  public:

    rect_element(double x, double y, double w, double h)
      : x_(x), y_(y), width_(w), height_(h)
    { //! Constructor defines all private data (no defaults).
    }

    rect_element(double x, double y, double w, double h,
                 const svg_style& style_info,
                 const std::string& id_name,
                 const std::string& class_name,
                 const std::string& clip_name)
      : x_(x), y_(y), width_(w), height_(h),
        svg_element(style_info, id_name, class_name, clip_name)
    { // Constructor defines all private data (no defaults).
    }

    double x() const
    { //! x-axis coordinate of the side of the rectangle which has the smaller x-axis coordinate value.
      return x_;
    }

    double y() const
    { //! y-axis coordinate of the side of the rectangle which has the smaller y-axis coordinate value.
      return y_;
    }

    double width() const
    { //! x + width is top right.
      return width_;
    }

    double height() const
    { //! y + height is bottom left.
      return height_;
    }

    void write(std::ostream& rhs)
    { /*! \verbatim
        Output SVG XML for rectangle.
       For example: <rect  x="0" y="0"  width="500"  height="350"/>
       \endverbatim
       */
      rhs << "<rect";
      write_attributes(rhs); // id (& clip_path)
      rhs << " x=\"" << x_ << "\""
        << " y=\"" << y_ << "\""
        << " width=\"" << width_ << "\""
        << " height=\""<< height_<< "\"/>";
    }

    bool operator==(const rect_element& lhs)
    { // Comparison useful for Boost.Test.
      return (lhs.x() == x_) && (lhs.y() == y_) &&  (lhs.width() == width_) && (lhs.height() == height_);
    }
    bool operator!=(const rect_element& lhs)
    {
      return (lhs.x() != x_) || (lhs.y() != y_) ||  (lhs.width() != width_) || (lhs.height() != height_);
    }
  }; // class rect_element

  bool operator==(const rect_element& lhs, const rect_element& rhs)
  { //
    return (lhs.x() == rhs.x()) && (lhs.y() == rhs.y()) && (lhs.width() == rhs.width()) && (lhs.height() == rhs.height());
  }

  bool operator!=(const rect_element& lhs, const rect_element& rhs)
  { //
    return (lhs.x() != rhs.x()) || (lhs.y() == rhs.y()) || (lhs.width() == rhs.width()) || (lhs.height() == rhs.height());
  }

  std::ostream& operator<< (std::ostream& os, const rect_element& r)
  { //! Example: rect_element r(20, 20, 50, 50);  cout << r << endl;
    //! Outputs:  rect(20, 20, 50, 50)
      os << "rect(" << r.x() << ", " << r.y()
         << ", " << r.width() << ", " << r.height() << ")" ;
    return os;
  } // std::ostream& operator<<

  // -----------------------------------------------------------------
  // class circle_element  Represents a single circle.
  // -----------------------------------------------------------------
  class circle_element : public svg_element
  {/*! \class boost::svg::circle_element
        \brief Circle from center coordinate, and radius.
        \details
         Represents a single circle.
         http://www.w3.org/TR/SVG/shapes.html#CircleElement
    */
  private:
    double x;
    double y;
    double radius;
  public:
    circle_element(double x, double y, double radius = 5)
      : x(x), y(y), radius(radius)
    { //! Constructor defines all private data (default radius only).
    }

    circle_element(double x, double y, double radius,
                 const svg_style& style_info,
                 const std::string& id_name="",
                 const std::string& class_name="",
                 const std::string& clip_name=""
                 )
      : x(x), y(y), radius(radius),
        svg_element(style_info, id_name, class_name, clip_name)
    { // Define all private data.
    }

    void write(std::ostream& rhs)
    { /*! Output SVG XML
    \verbatim
       Example: <circle cx="9.78571" cy="185" r="5"/>
    \endverbatim
    */
      rhs << "<circle";
      write_attributes(rhs);
      rhs << " cx=\"" << x << "\" cy=\"" << y << "\" r=\"" << radius << "\"/>";
    }
  }; // class circle_element


  // -----------------------------------------------------------------
  // Represents a single ellipse.
  // -----------------------------------------------------------------
  class ellipse_element : public svg_element
  { /*! \class boost::svg::ellipse_element
        \brief Ellipse from center coordinate, and radius.
        \details
        Represents a single ellipse.
        http://www.w3.org/TR/SVG/shapes.html#EllipseElement
        9.4 The 'ellipse'  element.
        */
  private:
    double cx; // coordinate x of center of ellipse, default 0
    double cy; // coordinate y, default 0
    double rx; // radius x
    double ry; // radius x
  public:
    ellipse_element(double cx, double cy, double rx = 4,  double ry = 8)
      : cx(cx), cy(cy), rx(rx), ry(ry)
    { //! Define all private data (default radii).
    }

    ellipse_element(double cx, double cy, double rx,  double ry,
                 const svg_style& style_info,
                 const std::string& id_name="",
                 const std::string& class_name="",
                 const std::string& clip_name="")
      : cx(cx), cy(cy), rx(rx), ry(ry),
        svg_element(style_info, id_name, class_name, clip_name)
    { // Define all private data.
    }

    ellipse_element(double cx, double cy,
                 const svg_style& style_info,
                 const std::string& id_name="",
                 const std::string& class_name="",
                 const std::string& clip_name="")
      : cx(cx), cy(cy), rx(4), ry(8), // 4 and 8 are the same defaults used above.
        svg_element(style_info, id_name, class_name, clip_name)
    { // Define all private data.
    }

    void write(std::ostream& rhs)
    { /*!
        \verbatim
        Output SVG XML for ellipse.
        Example: <ellipse rx="250" ry="100" fill="red"  />
       \endverbatim
     */
      rhs << "<ellipse";
      write_attributes(rhs);
      rhs << " cx=\"" << cx << "\" cy=\"" << cy << "\""
          << " rx=\"" << rx << "\" ry=\"" << ry  << "\"/>";
    }
  }; // class ellipse_element

  enum align_style
  { //! text_element Represents a single block of text, with font & alignment.
    left_align, //!< Align text to left.
    right_align, //!< Align text to right.
    center_align //!< Center text.
  };

class text_parent
{ /*! \class boost::svg::text_parent
    \brief An ancestor to both tspan and strings for the text_element class.
    \details This allows an array of both types to be stored in text_element.
  */
  protected:
    std::string text_;

  public:
    virtual void write(std::ostream& /* o_str */)
    {
    }

    text_parent(const std::string& text): text_(text)
    {
    }
    text_parent(const text_parent& rhs): text_(rhs.text_)
    {
    }
}; // class text_parent

class text_element_text : public text_parent
{ /*! \class boost::svg::text_element_text
  \brief text (not tspan) element to be stored in text_parent.
  \details See 10.4 text element http://www.w3.org/TR/SVG/text.html#TextElement
  */
public:
  text_element_text(const std::string& text): text_parent(text)
  {
  }
  text_element_text(const text_element_text& rhs): text_parent(rhs)
  {
  }
  void write(std::ostream& o_str)
  {
    o_str << text_;
  }
}; // class text_element_text

class tspan_element : public text_parent, public svg_element
{ /*! \class boost::svg::tspan_element
    \brief tspan (not text) to be stored in text_parent.
    \details See 10.5 tspan element http://www.w3.org/TR/SVG/text.html#TSpanElement
    */
private:
  double x_;  //!< Absolute X position.
  double y_;  //!< Absolute Y position.
  double dx_; //!< Relative X position of a 1st single character of text.
  double dy_;//!< Relative Y position of a 1st single character of text.
  int rotate_; //!< Rotation of a 1st single character of text.
  // A list of shifts or rotations for several characters is not yet implemented.
  double text_length_;  //!< Allows the author to provide exact alignment.
  //! dx_, dy_, and rotate_ can all be omitted, usually meaning no shift or rotation,
  //! but see http://www.w3.org/TR/SVG/text.html#TSpanElement for ancestor rules.
  //! but x_, y_, and text_length need a flag.
  bool use_x_;
  bool use_y_;
  bool use_text_length_;

  text_style style_; //!< font variants.
  bool use_style_;

public:
  tspan_element(const std::string& text, const text_style& style = no_style)
    :
    x_(0), y_(0), dx_(0), dy_(0), rotate_(0), text_length_(0),
    use_x_(false), use_y_(false), use_text_length_(false),
    style_(style), text_parent(text)
  {
  }

  tspan_element(const tspan_element& rhs);
    // TODO all may need refactoring to separate declaration from definition - as example below.

  //tspan_element(const tspan_element& rhs)
  //  :
  //  x_(rhs.x_), y_(rhs.y_), dx_(rhs.dx_), dy_(rhs.dy_), rotate_(rhs.rotate_),
  //  text_length_(rhs.text_length_), use_x_(rhs.use_x_), use_y_(rhs.use_y_),
  //  use_text_length_(rhs.use_text_length_), style_(rhs.style_),
  //  text_parent(rhs)
  //{
  //}

  // All setters (all chainable).
  //tspan_element(const std::string& text, const text_style& style);
  //tspan_element(const tspan_element&);
  //tspan_element& text(const std::string& text);
  //tspan_element& dx(double dx);
  //tspan_element& dy(double dy);
  //tspan_element& rotation(int rotation);
  //tspan_element& x(double x);
  //tspan_element& y(double y);
  //tspan_element& text_length(double text_length);

  tspan_element& text(const std::string& text)
  {
    text_=text;
    return *this; //! \return tspan_element& to make chainable.
  }

  tspan_element& dx(double dx)
  {
    dx_ = dx;
    return *this; //! \return tspan_element& to make chainable.
  }

  tspan_element& dy(double dy)
  {
    dy_ = dy;
    return *this; //! \return tspan_element& to make chainable.
  }

  tspan_element& rotation(int rotation)
  { //!< Note implementation so far only rotates the 1st character in string.
    //!< text_element rotation rotates the whole text string, so it *much* more useful.
    rotate_ = rotation;
    return *this; //! \return tspan_element& to make chainable.
  }

  tspan_element& x(double x)
  {
    x_ = x;
    use_x_ = true;
    return *this; //! \return tspan_element& to make chainable.
  }

  tspan_element& y(double y)
  {
    y_ = y;
    use_y_ = true;
    return *this; //! \return tspan_element& to make chainable.
  }

  tspan_element& text_length(double text_length)
  {
    text_length_ = text_length;
    use_text_length_ = true;
    return *this; //! \return tspan_element& to make chainable.
  }

  tspan_element& font_size(unsigned int size)
  {
    style_.font_size(size);
    use_style_ = true;
    return *this; //! \return tspan_element& to make chainable.
  }

  tspan_element& font_family(const std::string& family)
  {
    style_.font_family(family);
    use_style_ = true;
    return *this; //! \return tspan_element& to make chainable.
  }

  tspan_element& font_style(const std::string& style)
  { //! font-style: normal | bold | italic | oblique
    //! Examples: "italic"
    //! http://www.croczilla.com/~alex/conformance_suite/svg/text-fonts-02-t.svg
    style_.font_style(style);
    use_style_ = true;
    return *this; //! \return tspan_element& to make chainable.
  }

  tspan_element& font_weight(const std::string& w)
  { //! svg font-weight: normal | bold | bolder | lighter | 100 | 200 .. 900
    //! Examples: "bold", "normal"
    //! http://www.croczilla.com/~alex/conformance_suite/svg/text-fonts-02-t.svg
    //! tests conformance.  Only two weights are supported by Firefox, Opera, Inkscape.
    style_.font_weight(w);
    return *this; //! \return tspan_element& to make chainable.
  }

  tspan_element& fill_color(const svg_color& color)
  {
    style_info_.fill_color(color);
    style_info_.fill_on(true);
    use_style_ = true;
    return *this; //! \return tspan_element& to make chainable.
  }

  // All getters.

  //tspan_element::std::string text();
  //double x();
  //double y();
  //double dx();
  //double dy();
  //int rotation();
  //double text_length();
  //text_style& font_style();
  //const text_style& font_style() const;

  std::string text(){ return text_; }
  double x() { return x_; }
  double y() { return y_; }
  double dx() { return dx_; }
  double dy() { return dy_; }
  int rotation() { return rotate_; }
  double text_length() { return text_length_; }

  unsigned int font_size()
  {
    return style_.font_size();
  }

  const std::string& font_family()
  {
    return style_.font_family();
  }

  const std::string& font_weight() const
  {
    return style_.font_weight();
  }

  text_style& font_style()
  { // Access to font family, size ...
    return style_;
  }

  const text_style& font_style() const
  { // Access to font family, size ... const version.
    return style_;
  }

  svg_color fill_color()
  {
    return style_info_.fill_color();
  }

  bool fill_on()
  {
    return style_info_.fill_on();
  }

  void write(std::ostream& os)
  { //! Output SVG XML for tspan_element
    os << "<tspan";
    write_attributes(os); // id & clip_path
    style_info_.write(os); // fill, stroke, width...

    // All of the conditional writes within tspan_element.

    // First, all elements that can be tested based on their value.
    if(rotate_ != 0)
    {
      os << " rotate=\"" << rotate_ << "\"";
    }
    if(dx_!= 0)
    {
      os << " dx=\"" << dx_ << "\"";
    }
    if(dy_!= 0)
    {
      os << " dy=\"" << dy_ << "\"";
    }

    // Now, add all elements that can be tested with the flags.
    if(use_x_ == true)
    {
      os << " x=\"" << x_ << "\"";
    }
    if(use_y_  == true)
    {
      os << " y=\"" << y_ << "\"";
    }
    if(use_text_length_ == true)
    {
      os << " textLength=\"" << text_length_ << "\"";
    }
    if (style_.font_size() != 0)
    {
      os << " font-size=\"" << style_.font_size() << "\"";
    }
    if (style_.font_family() != "")
    { // Example: Arial.
      os << " font-family=\"" << style_.font_family() << "\"";
    }
    if (style_.font_style().size() != 0)
    { // Example: italic.
      os << " font-style=\"" << style_.font_style() << "\"";
    }
    if (style_.font_weight().size() != 0)
    { // Example: bold.
    os << " font-weight=\"" << style_.font_weight() << "\"";
    }
    if (style_.font_stretch().size() != 0)
    {
    os << " font-stretch=\"" << style_.font_stretch() << "\"";
    }
    if (style_.font_decoration().size() != 0)
    {
    os << " font-decoration=\"" << style_.font_decoration() << "\"";
    }
    os << ">" << text_ << "</tspan>";
  } //   void write(std::ostream& os)
}; // class tspan_element

tspan_element::tspan_element(const tspan_element& rhs)
    :
    x_(rhs.x_), y_(rhs.y_), dx_(rhs.dx_), dy_(rhs.dy_), rotate_(rhs.rotate_),
    text_length_(rhs.text_length_), use_x_(rhs.use_x_), use_y_(rhs.use_y_),
    use_text_length_(rhs.use_text_length_), style_(rhs.style_),
    text_parent(rhs)
  { // Separately defined constructor.
  } // tspan_element::tspan_element

class text_element : public svg_element
{ /*! \class boost::svg::text_element
      \brief Holds text with position, size, font, (& styles) & orientation.
      \details
      \verbatim
      Not necessarily shown correctly by all browsers, alas.
      SVG Coordinates of 1st character EM box, see
      http://www.w3.org/TR/SVG/text.html#TextElement 10.2
      So any text with y coordinate = 0 shows only any roman lower case descenders!\n\n

      (Text may contain embedded xml Unicode characters
      for Greek, math etc, for example: &#x3A9;).
      int size; // " font-size = 12"
      http://www.w3.org/TR/SVG/text.html#CharactersAndGlyphs
      std::string font;  // font-family: "Arial" | "Times New Roman" | "Verdana" | "Lucida Sans Unicode"
      "sans", "serif", "times"
      http://www.w3.org/TR/SVG/text.html#FontFamilyProperty
      10.10 Font selection properties
      std::string style_; // font-style: normal | bold | italic | oblique
      std::string weight; // font-weight: normal | bold | bolder | lighter | 100 | 200 .. 900
      std::string stretch; // font-stretch: normal | wider | narrower ...
      std::string decoration; // // "underline" | "overline" | "line-through"
      Example:
      <text x="250" y="219.5" text-anchor="middle"  font-family="verdana" font-size="12">0 </text>
      \endverbatim

  */
 private: // Access only via member functions below.
  double x_; //!< Left edge.
  double y_; //!< Bottom of roman capital character.
  ptr_vector<text_parent> data_; //!< Stores all of the containing data.
  text_style style_; //!< font variants.
  align_style align_; //!< left_align, right_align, center_align
  rotate_style rotate_; //!< horizontal, upward, downward, upsidedown

  void _generate_text(std::ostream& os)
  {
    for(ptr_vector<text_parent>::iterator i = data_.begin();
        i != data_.end();
        ++i)
    {
      (*i).write(os);
    }
  }
public:
  // Set
  //void alignment(align_style a);
  //void rotation(rotate_style rot);
  //void x(double x);
  //void y(double y);
  //void text(const std::string& t);
  //tspan_element& tspan(const std::string& t);
  //text_element(double x, double y, const std::string text,text_style ts,align_style align, rotate_style rotate);
  //text_element(const text_element& rhs);

  // Get
  //text_style& style();
  //const text_style& style() const;
  //align_style alignment();
  //rotate_style rotation() const;
  //double x() const;
  //double y() const;

  text_style& style()
  { // Access to font family, size ...
    return style_;
  }

  const text_style& style() const
  {
    return style_;
  }

  text_element& style(text_style& ts)
  {
    style_ = ts;
    return *this; //! \return text_element& to make chainable.
  }

  text_element&  alignment(align_style a) // TODO Change name to align????
  { //! left_align, right_align, center_align
    align_ = a;
    return *this; //! \return text_element& to make chainable.
  }

  align_style alignment()
  { //! left_align, right_align, center_align
    return align_;
  }

  text_element&  rotation(rotate_style rot)// TODO Change name to rotate???
  { //! Degrees: horizontal  = 0, upward = -90, downward, upsidedown
    //! Generates: transform = "rotate(-45 100 100 )"
    rotate_ = rot;
    return *this; //! \return text_element& to make chainable.
  }

  rotate_style rotation() const
  {
    return rotate_;
  }

  // set functions now return *this to be chainable, for example:
  // my_text_element.style(no_style).x(999).y(555).alignment(right_align).rotation(vertical);

  text_element& x(double x)
  { //! x coordinate of text to write.
    x_ = x;
    return *this; //! \return text_element& to make chainable.
  }

  double x() const
  { //! x coordinate of text to write.
    return x_;
  }

  text_element& y(double y)
  { //! y coordinate of text to write.
    y_ = y;
    return *this; //! \return text_element& to make chainable.
  }

  double y() const
  { //! y coordinate of text to write.
    return y_;
  }

  void text(const std::string& t)
  { //! text string to write.
    data_.push_back(new text_element_text(t));
  }

  tspan_element& tspan(const std::string& t)
  {
    data_.push_back(new tspan_element(t, style_));
    return *(static_cast<tspan_element*>(&data_[data_.size()-1]));
  }

  text_element(
    //! Coordinates of 1st character EM box, see
    //! http://www.w3.org/TR/SVG/text.html#TextElement 10.2
    double x = 0., // Left edge.
    double y = 0., // Bottom of character (roman capital).
    // So any text with y coordinate = 0  shows only the roman lower case descenders!
    const std::string text = "",
    text_style ts = no_style, // Left to SVG defaults.
    align_style align = left_align,
    rotate_style rotate = horizontal)
    : // Constructor.
    x_(x), y_(y), // location.
    data_(ptr_vector<text_parent>()),
    //size(size), font(font), style_(style), weight(weight), stretch(stretch), decoration(decoration),
    style_(ts),
    align_(align),
    rotate_(rotate)
  { //! text_element Default Constructor, defines defaults for all private members.
    data_.push_back(new text_element_text(text)); // Adds new text string.
  }

  text_element(const text_element& rhs)
  :
    x_(rhs.x_), y_(rhs.y_), style_(rhs.style_), align_(rhs.align_), rotate_(rhs.rotate_)
  { // Copy constructor.
     data_ = (const_cast<text_element&>(rhs)).data_.release();
  }

  text_element& operator=(const text_element& rhs)
  {
    x_ = rhs.x_;
    y_ = rhs.y_;
    data_.clear(); // Copy data_
    data_.insert(data_.end(), rhs.data_.begin(), rhs.data_.end());
    style_ = rhs.style_;
    align_ = rhs.align_;
    rotate_ = rhs.rotate_;
    return *this; //! to make chainable.
  }

  std::string text()
  {
    std::stringstream os;
    _generate_text(os);
    return os.str();
  }

  void write(std::ostream& os)
  { //! Output text_element, style & attributes to stream.
    // Changed to new convention on spaces:
    // NO trailing space, but *start* each item with a space.
    // For debug, may be convenient to start with newline.
    // os << " <text x=\"" << x_ << "\" y=\"" << y_ << "\"";
    os << "\n<text x=\"" << x_ << "\" y=\"" << y_ << "\"";
    std::string anchor;
    switch(align_)
    {
    case left_align:
      // anchor = "start"; // This is the initial == default.
      // so should be possible to reduce file size this by:
      anchor = "";
      break;
    case right_align:
      anchor = "end";
      break;
    case center_align:
      anchor = "middle";
      break;
    default:
      anchor = "";
      break;
    }
    if(anchor != "")
    {
      os << " text-anchor=\"" << anchor << "\"";
    }
    if(rotate_ != 0)
    {
      os << " transform = \"rotate("
        << rotate_ << " "
        << x_ << " "
        << y_ << " )\"";
    }
    if (style_.font_size() != 0)
    {
      os << " font-size=\"" << style_.font_size() << "\"";
    }
    if (style_.font_family() != "")
    { // Example: Arial.
      os << " font-family=\"" << style_.font_family() << "\"";
    }
    if (style_.font_style().size() != 0)
    { // Example: italic.
      os << " font-style=\"" << style_.font_style() << "\"";
    }
    if (style_.font_weight().size() != 0)
    { // Example: bold.
    os << " font-weight=\"" << style_.font_weight() << "\"";
    }
    if (style_.font_stretch().size() != 0)
    {
    os << " font-stretch=\"" << style_.font_stretch() << "\"";
    }
    if (style_.font_decoration().size() != 0)
    {
    os << " font-decoration=\"" << style_.font_decoration() << "\"";
    }
    os << '>' ;
    _generate_text(os);
    os << "</text>";
    // Example:
  } // void write(std::ostream& os)
}; // class text_element_

  std::ostream& operator<< (std::ostream& os, text_element& t)
  { //! Outputs: text & style (useful for diagnosis).
    //! Usage: text_element t(20, 30, "sometest", left_align, horizontal);  cout << t << endl;
      t.write(os);

    return os;
  } // std::ostream& operator<<

  class clip_path_element: public svg_element
  {  /*! \class boost::svg::clip_path_element
      \brief The clipping path restricts the region to which paint can be applied.
      \details 14.3 Clipping paths http://www.w3.org/TR/SVG/masking.html#ClipPathProperty.
    */
  private:
    std::string element_id; // SVG element id.
    rect_element rect; //! Clipping rectangle.

  public:

    clip_path_element(const std::string& id, const rect_element& rect)
      : element_id(id), rect(rect)
      { // Constructor defines all member variables.
      }

      void write(std::ostream& os)
      {
        os << "<clipPath id=\"" << element_id << "\">";
        rect.write(os);
        os  << "</clipPath>" << std::endl;
      }
  }; // class clip_path_element

  struct path_point
  { /*! \struct boost::svg::path_point
      \brief Base class for m_path, z_path, q_path, h_path, v_path, c_path, s_path.
      \details Paths represent the outline of a shape which can be
      filled, stroked, used as a clipping path, or any combination of the three.
     */
    bool relative; // or if false then absolute.

    virtual void write(std::ostream& rhs) = 0;
    virtual ~path_point()
    {
    }

    path_point(bool relative) : relative(relative)
    { // Constructor defines all member variables.
    }
  }; // struct path_point


  struct m_path: public path_point
  { /*! \struct boost::svg::m_path
      \brief moveto coordinates (x, y)
     \details   8.3.2 The "moveto" commands.
     */
    double x;
    double y;

    void write(std::ostream& o_str)
    { //! Example: "M52.8571,180 "
      if(relative)
      {
        o_str << "m";
      }
      else
      { // absolute
        o_str << "M";
      }
      o_str << x << "," << y << " "; // separator changed to comma for clarity.

    } // void write(std::ostream& o_str)

    m_path(double x, double y, bool relative = false)
      : x(x), y(y), path_point(relative)
    {
    }
  }; // struct m_path

  struct z_path: public path_point
  { /*! \struct boost::svg::z_path
       \brief Close current path.
       \details
       http://www.w3.org/TR/SVG/paths.html#PathElement
       8.3.1 General information about path data.
       Close the current subpath by drawing a straight line
       from the current point to current subpath's initial point.
    */
    void write(std::ostream& o_str)
    {
      o_str << "Z";
    }

    z_path() : path_point(false)
    { // Constructor defines all member variables.
    }
  }; // struct z_path

  // 8.3.4 The "lineto" commands L, H & V.
  struct l_path: public path_point
  {  /*! \struct boost::svg::l_path
      \brief Draw a line from the current point to the given (x,y) coordinate
       which becomes the new current point.
    */
    double x;
    double y;

    void write(std::ostream& o_str)
    {
      if(relative)
      {
        o_str << "l";
      }
      else
      { // Absolute.
        o_str << "L";
      }
      o_str << x << "," << y << " ";
    }

    l_path(double x, double y, bool relative = false)
      : x(x), y(y), path_point(relative)
    { // Constructor defines all member variables.
    }
  }; // struct l_path

  struct h_path: public path_point
  { /*! \struct boost::svg::h_path
      \brief  Draws a horizontal line from the current point (cpx, cpy) to (x, cpy).
       which becomes the new current point. No y needed, start from current point y.
    */
    double x;
    void write(std::ostream& o_str)
    {
      if(relative)
      {
        o_str << "h";
      }
      else
      { // Absolute.
        o_str << "H";
      }
      o_str << x << " ";
    }

    h_path(double x, bool relative = false)
      : x(x), path_point(relative)
    { // Constructor defines all member variables.
    }
  }; // struct h_path

  struct v_path: public path_point
  { /*! \struct boost::svg::v_path
        \brief Draws a vertical line from the current point (cpx, cpy) to (cpx, y).
        No x coordinate needed - use current point x.
    */
    double y;
    void write(std::ostream& o_str)
    {
      if(relative)
      {
        o_str << "v";
      }
      else
      { // Absolute.
        o_str << "V";
      }
      o_str << y << " ";
    }

    v_path(double y, bool relative = false)
      : y(y), path_point(relative)
    { // Constructor defines all member variables.
    }
  }; // struct v_path


  struct c_path: public path_point
  { /*! \struct boost::svg::c_path
     \brief Draws a cubic Bezier curve from the current point to (x, y) using (x1, y1).
     \details 8.3.5 The curve commands: C, Q & A.
    */
    double x1;
    double y1;
    double x2;
    double y2;
    double x;
    double y;

    void write(std::ostream& o_str)
    {
      if(relative)
      {
        o_str << "c";
      }
      else
      { // Absolute.
        o_str<<"C";
      }
      o_str << x1 << "," << y1 << " "
        << x2 << "," << y2 << " "
        << x << "," << y << " ";
    }

    c_path(double x1, double y1, double x2, double y2,
            double x, double y, bool relative = false)
      : x1(x1), y1(y1), x2(x2), y2(y2), x(x), y(y), path_point(relative)
    { // Constructor defines all member variables.
    }
  }; // struct c_path

  struct q_path: public path_point
  { /*! \struct boost::svg::q_path
      \brief Draws a quadratic Bezier curve from the current point to (x,y).
       using (x1,y1) as the control point.
    */
    double x1, y1, x, y;

    void write(std::ostream& o_str)
    {
      if(relative)
      {
        o_str << "q";
      }
      else
      { // Absolute.
        o_str << "Q";
      }
      o_str << x1 << " " << y1 << " "
         << x << " " << y << " ";
    }

    q_path(double x1, double y1, double x, double y, bool relative = false)
      : x1(x1), y1(y1), x(x), y(y), path_point(relative)
    { // Constructor.
    }
  }; //struct q_path

  struct s_path : public path_point
  { /*! \struct boost::svg::s_path
      \brief Draws a cubic Bezier curve from the current point to (x,y).
      \details see also t_path for a quadratic Bezier curve.

    */
    double x1, y1, x, y;

    void write(std::ostream& o_str)
    {
      if(relative)
      {
        o_str << "s";
      }
      else
      { // Absolute.
        o_str << "S";
      }
      o_str << x1 << "," << y1 << " "
        << x << "," << y << " ";
    }

    s_path(double x1, double y1, double x, double y, bool relative = false)
      : x1(x1), y1(y1), x(x), y(y), path_point(relative)
    { // Constructor.
    }
  }; // struct s_path

  struct t_path: public path_point
  { /*! \struct boost::svg::t_path
      \brief Draws a quadratic Bezier curve from the current point to (x,y).
      \details see also s_path for a cubic Bezier curve.
    */
    double x;
    double y;

    void write(std::ostream& o_str)
    {
      if(relative)
      {
        o_str << "t";
      }
      else
      { // Absolute.
        o_str << "T";
      }
      o_str << x << "," << y << " ";
    }

    t_path(double x, double y, bool relative = false)
      : x(x), y(y), path_point(relative)
    { // Constructor
    }
  }; // struct t_path

  struct a_path : public path_point
  { /*! \struct boost::svg::a_path
      \brief Draws a elliptical arc from the current point to (x,y),
        using two radii, axis rotation, and control two flags.
      \details See 8.3.8 The elliptical arc curve commands.!
        Useful for pie charts, etc.
     */
    double x; //!< X End of arc from current point.
    double y; //!< Y End of arc from current point.
    double rx; //!< X radius
    double ry; //!< Y radius
    double x_axis_rotation; //!< Any rotation of the X axis.
    bool large_arc; //!< true if arc >= 180 degrees wanted.
    bool sweep; //!< true if to draw in positive-angle direction

    void write(std::ostream& o_str)
    {
      if(relative)
      {
        o_str << "a";
      }
      else
      { // Absolute.
        o_str << "A";
      }
      o_str << rx << "," << ry << " " << x_axis_rotation
        << ((large_arc) ? 1 : 0) << "," << ((sweep) ? 1 : 0) << " "
        << x << "," << y << " ";
    }

    a_path(double x, double y, double rx, double ry, double x_axis_rotation, bool large_arc = false, bool sweep = false, bool relative = false)
      : x(x), y(y), rx(rx), ry(ry), x_axis_rotation(x_axis_rotation), large_arc(large_arc), sweep(sweep), path_point(relative)
    { // Constructor
    }
  }; // struct a_path

  class path_element: public svg_element
  {  /*! \class boost::svg::path_element
     \brief Path element holds places on a path used by move, line ...
     \details
     http://www.w3.org/TR/SVG/paths.html#PathElement
     8.3.1 General information about path data.
     A path is defined by including a 'path'  element
     which contains a d="(path data)"  attribute,
     where the d attribute contains the moveto, line, curve
     (both cubic and quadratic Beziers), arc and closepath instructions.
     */
  private:
    ptr_vector<path_point> path; // All the (x, y) coordinate pairs,
    // filled by calls of m, M, l , L... that push_back.
  public:
    // bool fill; now inherited from parent svg class.

    path_element(const path_element& rhs)
    {
      path = (const_cast<path_element&>(rhs)).path.release();
    }

    path_element(const svg_style& style_info,
      const std::string& id_name="",
      const std::string& class_name="",
      const std::string& clip_name="")
      :
      svg_element(style_info, id_name, class_name, clip_name)
    {
    }

    path_element()
    {
      // fill now got from the parent svg fill color.
    }

    path_element& fill_on(bool on_)
    { // Set area fill, on or off.
      style_info_.fill_on(on_);
      return *this; //! \return path_element& to make chainable.
    }

    bool fill_on()
    { // Get area fill, on or off.
      return style_info_.fill_on();
    }
    // Note 1: return of path_element& permits chaining calls like
    // my_path.M(3, 3).l(150, 150).l(200, 200)...;

    // Note 2: By convention:
    // lower case (like m call m_path(x, y, true) for relative
    // but upper case (like M) calls m_path(x, y, false) for absolute.

    path_element& m(double x, double y)
    { // relative.
      path.push_back(new m_path(x, y, true));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& M(double x, double y)
    { // absolute.
      path.push_back(new m_path(x, y, false));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& z()
    { // Note lower case z, see path_element& Z() below.
      path.push_back(new z_path());
      return *this; //! \return path_element& to make chainable.
    }

    path_element& Z()
    { // Upper case Z also provided for compatibility with
      // http://www.w3.org/TR/SVG/paths.html#PathDataClosePathCommand 8.3.3 which allows either case.
      path.push_back(new z_path());
      return *this; //! \return path_element& to make chainable.
    }

    path_element& l(double x, double y)
    {
      path.push_back(new l_path(x, y, true));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& L(double x, double y)
    {
      path.push_back(new l_path(x, y, false));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& h(double x)
    {
      path.push_back(new h_path(x, true));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& H(double x)
    {
      path.push_back(new h_path(x, false));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& v(double y)
    {
      path.push_back(new v_path(y, true));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& V(double y)
    {
      path.push_back(new v_path(y, false));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& c(double x1, double y1, double x2, double y2, double x, double y)
    {
      path.push_back(new c_path(x1, y1, x2, y2, x, y, true));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& C(double x1, double y1, double x2, double y2, double x, double y)
    {
      path.push_back(new c_path(x1, y1, x2, y2, x, y, false));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& q(double x1, double y1, double x, double y)
    {
      path.push_back(new q_path(x1, y1, x, y, true));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& Q(double x1, double y1, double x, double y)
    {
      path.push_back(new q_path(x1, y1, x, y, false));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& s(double x1, double y1, double x, double y)
    {
      path.push_back(new s_path(x1, y1, x, y, true));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& S(double x1, double y1, double x, double y)
    { //
      path.push_back(new s_path(x1, y1, x, y, false));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& t(double x, double y)
    {
      path.push_back(new t_path(x, y, true));
      return *this; //! \return path_element& to make chainable.
    }

    path_element& T(double x, double y)
    {
      path.push_back(new t_path(x, y, false));
      return *this; //! \return path_element& to make chainable.
    }

    void write(std::ostream& o_str)
    {
      if (path.begin() != path.end() )
      { // Is some path info (trying to avoid useless <path d=""/>"
        // TODO or would this omit useful style & attributes?
        o_str << "<path d=\"";
        for(ptr_vector<path_point>::iterator i = path.begin(); i != path.end(); ++i)
        {
          (*i).write(o_str); // M1,2
        }
        o_str << "\"";

        write_attributes(o_str); // id & clip_path
        style_info_.write(o_str); // fill, stroke, width...

        // line above should write fill = "none" that
        // seems to be needed for reasons unclear.
        // Even when g_element does not specify a fill, it seems to be interpreted as black fill.
        if(!fill_on())
        {
          o_str << " fill=\"none\"";
        }
        o_str<<"/>"; // closing to match <path d=
      }
      // Example: <path d="M5,175 L5,195 M148.571,175" />
    } // void write(std::ostream& o_str)
  }; // class path_element

  struct poly_path_point
  { /*! \struct boost::svg::poly_path_point
      \brief polyline or polygon point coordinates (x, y)
      \details  9.6 polyline & 9.7 The 'polygon' element.
      */
    double x;
    double y;
    // Polygon & polyline points are always absolute, never relative,
    // and values have no preceeding letter like M or L,
    // So NOT derived from path_point.

    void write(std::ostream& o_str)
    { //! Output SVG XML,
      //! Example: " 250,180"
      //! Leading space is redundant for 1st after "points= ",
      //! but others are separators, and arkward to know which is 1st.
      o_str << " " << x << "," << y; // x, y separator comma for clarity.
    } // void write(std::ostream& o_str)

    poly_path_point(double x, double y)
      : x(x), y(y)
    {
    }

    poly_path_point()
      : x(0.), y(0.)
    { // Default constructor.
    }

  }; // struct poly_path_point

  std::ostream& operator<< (std::ostream& os, const poly_path_point& p)
  { //! Output may be useful for Boost.Test.
    //! Usage:  poly_path_point p0(100, 200);
    //! cout << p0 << endl;
    //! Outputs: (100, 200)
     os << "(" << p.x << ", " << p.y  << ")";
    return os;
  } // std::ostream& operator<<

  class polygon_element: public svg_element
  {  /*! \struct boost::svg::polygon_element
     \brief The 'polygon' element defines a closed shape
     consisting of a set of connected straight line segments.

     \details http://www.w3.org/TR/SVG/shapes.html#PolygonElement
     The 'polygon'  element 9.9.7.
     A polygon is defined by including a 'path'  element
     which contains a points="(path data)"  attribute,
     where the d attribute contains the x, y coordinate pairs.
    */
    friend std::ostream& operator<< (std::ostream&, const polygon_element&);
    friend std::ostream& operator<< (std::ostream&, polygon_element&);

  private:
    //using boost::ptr_vector;
    ptr_vector<poly_path_point> poly_points; //!< All the x, y coordinate pairs,
    //!< push_backed by calls of p_path(x, y).
  public:
    bool fill; // polygon to have fill color.

    polygon_element(const polygon_element& rhs)
    { // Copy constructor.
      poly_points = (const_cast<polygon_element&>(rhs)).poly_points.release();
      // 'empty' the vector of points.
    }

    polygon_element() : fill(true)
    {
    }

    polygon_element (double x, double y, bool f = true) : fill(f)
    { //! Constructor - One absolute (x, y) point only.
      //! Can add more path points using member function P.
      poly_points.push_back(new poly_path_point(x, y));
    }

    polygon_element (double x1, double y1, double x2, double y2, double x3, double y3, bool f = true) : fill(f)
    { //! Constructor - Absolute (x, y) only. Used by triangle.
      poly_points.push_back(new poly_path_point(x1, y1));
      poly_points.push_back(new poly_path_point(x2, y2));
      poly_points.push_back(new poly_path_point(x3, y3));
    }

    polygon_element (double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, bool f = true)
      :
      fill(f)
    { //! Constructor - Absolute (x, y) only. Used by rhombus.
      poly_points.push_back(new poly_path_point(x1, y1));
      poly_points.push_back(new poly_path_point(x2, y2));
      poly_points.push_back(new poly_path_point(x3, y3));
      poly_points.push_back(new poly_path_point(x4, y4));
    }

    polygon_element (double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double x5, double y5, bool f = true)
      :
      fill(f)
    { //! Constructor - Absolute (x, y) only. Used by pentagon.
      poly_points.push_back(new poly_path_point(x1, y1));
      poly_points.push_back(new poly_path_point(x2, y2));
      poly_points.push_back(new poly_path_point(x3, y3));
      poly_points.push_back(new poly_path_point(x4, y4));
      poly_points.push_back(new poly_path_point(x5, y5));
    }

    polygon_element (double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double x5, double y5, double x6, double y6, bool f = true)
      :
      fill(f)
    { //! Constructor - Six absolute (x, y) only. Used by hexagon.
      // Might be done more efficiently with fixed size boost::array?
      poly_points.push_back(new poly_path_point(x1, y1));
      poly_points.push_back(new poly_path_point(x2, y2));
      poly_points.push_back(new poly_path_point(x3, y3));
      poly_points.push_back(new poly_path_point(x4, y4));
      poly_points.push_back(new poly_path_point(x5, y5));
      poly_points.push_back(new poly_path_point(x6, y6));
    }

    polygon_element (std::vector<poly_path_point>& points, bool f = true)
      :
      fill(f)
    { //! Constructor from vector of path points.
      poly_points.reserve(points.size()); // Since we know how many will be pushed.
      for(std::vector<poly_path_point>::iterator i = points.begin(); i != points.end(); ++i)
      {
        poly_path_point p = (*i);
        poly_points.push_back(new poly_path_point(p.x, p.y));
      }
    }
/*
    template<int n>
    polygon_element (boost::array<const poly_path_point, n>& points, bool f = true)
      :
      fill(f)
    { // Constructor from const array of path points.
      poly_points.reserve(n); // Since we know how many will be pushed.
      //poly_points.reserve(points.size()); // Since we know how many will be pushed.
      for(array<const poly_path_point, n>::iterator i = points.begin(); i != points.end(); ++i)
      {
        poly_path_point p = (*i);
        poly_points.push_back(new poly_path_point(p.x, p.y));
      }
    }

    // And non-const poly_path_point version, in case needed.
    template<int n>
    polygon_element (boost::array<poly_path_point, n>& points, bool f = true)
      :
      fill(f)
    { // Constructor from const array of path points.
      poly_points.reserve(n); // Since we know how many will be pushed.
      // == poly_points.reserve(points.size()); // Since we know how many will be pushed.
      for(array<poly_path_point, n>::iterator i = points.begin(); i != points.end(); ++i)
      {
        poly_path_point p = (*i);
        poly_points.push_back(new poly_path_point(p.x, p.y));
      }
    }
*/
    // Member function to add more points to polygon.
    polygon_element& P(double x, double y)
    { //! Add another point (x, y) - absolute only.
      poly_points.push_back(new poly_path_point(x, y));
      return *this; //! \return polygon_element& to make chainable.
    }

    void write(std::ostream& o_str)
    {  /*! \verbatim SVG XML:
            Example: <polygon fill="lime" stroke="blue" stroke-width="10"
            points="850,75  958,137.5 958,262.5 850,325 742,262.6 742,137.5" />
         \endverbatim
       */
      o_str << "<polygon points=\"";
      for(ptr_vector<poly_path_point>::iterator i = poly_points.begin(); i != poly_points.end(); ++i)
      {
        (*i).write(o_str); //  x, y coordinates as " 1,2"
      }
      o_str << "\"";
      write_attributes(o_str);
      style_info_.write(o_str);
      if(!fill)
      {
        o_str << " fill = \"none\"";
      }
      o_str<<"/>";
    } // void write(std::ostream& o_str)

    std::ostream& operator<< (std::ostream& os)
    { /*! Output polygon info. (May be useful for Boost.Test.
         using os << "(" << p.x << ", " << p.y  << ")" ;
         Usage:  polygon_element p(1, 2, 3, 4, 5, 6);
           my_polygon.operator << (cout);
         (But NOT cout << my_polygon << endl;)
         Outputs: (1, 2)(3, 4)(5, 6)
      */
      for(ptr_vector<poly_path_point>::iterator i = poly_points.begin(); i != poly_points.end(); ++i)
      {
        os << (*i); //  x, y coordinates as " (1, 2)"
      }
      return os;
    } // std::ostream& operator<<

  }; // class polygon_element

  std::ostream& operator<< (std::ostream& os, polygon_element& p)
  { /*! Output poly_path_ points (May be useful  for Boost.Test).
        ptr_vector<poly_path_point> poly_points; All the x, y coordinate pairs,
        Usage:  polygon_element p(1, 2, 3, 4, 5, 6);
        cout << p << endl;
        Outputs: (1, 2)(3, 4)(5, 6)
    */
    for(ptr_vector<poly_path_point>::iterator i = p.poly_points.begin(); i != p.poly_points.end(); ++i)
    {
      os << (*i); //  x, y coordinates as " (1, 2)(3, 4)..."
      // using os << "(" << p.x << ", " << p.y  << ")" ;
    }
    return os;
  } // std::ostream& operator<<

  class polyline_element: public svg_element
  { /*! \class boost::svg::polyline_element
     \brief The 'polyline'  element: defines a set of connected straight line segments.
     \details
      http://www.w3.org/TR/SVG/shapes.html#PolylineElement
     9.6 The polyline  element: defines a set of connected straight line segments.
     Typically, polyline elements define open shapes.
     A polyline is defined by including a 'path'  element
     which contains a points="(path data)"  attribute,
     where the points attribute contains the x, y coordinate pairs.
     * perform an absolute moveto operation
       to the first coordinate pair in the list of points
     * for each subsequent coordinate pair,
       perform an absolute lineto operation to that coordinate pair.
     The advantage of polyline is in reducing file size,
     avoiding M and repeated L before x & y coordinate pairs.
     */
  friend std::ostream& operator<< (std::ostream&, polyline_element&);

  private:
    ptr_vector<poly_path_point> poly_points; //!< All the (x, y) coordinate pairs,
    // push_back by calls of p_path(x, y).
  public:
    //bool fill; // not needed for polyline, unlike polygon.

    polyline_element(const polyline_element& rhs)
    { // copy constructor.
      poly_points = (const_cast<polyline_element&>(rhs)).poly_points.release();
    }

    polyline_element()
    { //! Construct an 'empty' line.
      //! Can new line path points add using polyline_element member function P.
    }

    polyline_element (double x1, double y1)
    { //! One (x, y) path point, absolute only.
      poly_points.push_back(new poly_path_point(x1, y1));
    }

    polyline_element (double x1, double y1, double x2, double y2)
    { //! Two (x, y) path points, absolute only.
      poly_points.push_back(new poly_path_point(x1, y1));
      poly_points.push_back(new poly_path_point(x2, y2));
    }

    polyline_element (std::vector<poly_path_point>& points)
    { //! Constructor from vector of path points.
      for(std::vector<poly_path_point>::iterator i = points.begin(); i != points.end(); ++i)
      {
        poly_path_point p = (*i);
        poly_points.push_back(new poly_path_point(p.x, p.y));
      }
    }

    // Member function to add new points to existing line.
    polyline_element& P(double x, double y)
    { //! Absolute (x, y) only, so Capital letter P.
      poly_points.push_back(new poly_path_point(x, y));
      return *this; //! \return polyline_element& to make chainable.
    }

    void write(std::ostream& o_str)
    { /*! \verbatim
          Output polyline info (useful for Boost.Test).
          Example: <polyline points=" 100,100 200,100 300,200 400,400"/>
          \endverbatim
      */
      o_str << "<polyline points=\"";
      for(ptr_vector<poly_path_point>::iterator i = poly_points.begin(); i!= poly_points.end(); ++i)
      {
        (*i).write(o_str); //  x, y coordinates as " 1,2"
      }
      o_str << "\"";
      write_attributes(o_str);
      style_info_.write(o_str);
      o_str<<"/>";
    } // void write(std::ostream& o_str)

  }; // class polyline_element

  std::ostream& operator<< (std::ostream& os, polyline_element& p)
  { /*! \verbatim
         Output polyline info (useful for Boost.Test).
         Example: <polyline points=" 100,100 200,100 300,200 400,400"/>
         ptr_vector<poly_path_point> poly_points; // All the x, y coordinate pairs.
        \endverbatim
      */
    for(ptr_vector<poly_path_point>::iterator i = p.poly_points.begin(); i != p.poly_points.end(); ++i)
    {
      os << (*i); //  x, y coordinates as " (1, 2)(3, 4)..."
      // using os << "(" << p.x << ", " << p.y  << ")" ;
    }
    // Usage:  polyline_element p(1, 2, 3, 4, 5, 6);
    // cout << p << endl;
    // Outputs: (1, 2)(3, 4)(5, 6)
    return os;
  } // std::ostream& operator<<


  class g_element: public svg_element
  { /*! \class boost::svg::g_element
      \brief g_element (group element) is the node element of our document tree.
      \verbatim
      'g' element is a container element for grouping together  <g /> ... </g>
     \endverbatim.
      \details g_element ('g' element is a container element
      for grouping together related graphics elements).\n
      See http://www.w3.org/TR/SVG/struct.html#NewDocument 5.2.1 Overview.

      \verbatim
      'g' element is a container element for grouping together <g /> </g>
      related graphics elements, for example:
      <g id="background" fill="rgb(255,255,255)"><rect width="500" height="350"/></g>
      \endverbatim.
   */

  private:
    ptr_vector<svg_element> children; /*! Children of this group element node,
      containg graphics elements like text, circle line, polyline...
    */
    std::string clip_name;
    bool clip_on;
  public:

    g_element() : clip_on(false)
    {
    }

    svg_element& operator[](unsigned int i)
    {
      return children[i];
    }

    size_t size()
    { //! \return Number of child nodes.
      return children.size();
    }

    void write(std::ostream& os)
    { /*! Output all children of a group element.
    \verbatim
         Example:
         <g fill="rgb(255,255,255)" id="background"><rect x="0" y="0" width="500" height="350"/></g>
         \endverbatim
      */

      if (children.size() > 0)
      { /*!
          \verbatim
          Avoid useless output like: <g id="legendBackground"></g>
          TODO check this doesn't mean that useful style is lost?
        \endverbatim
        */

        os << "<g"; // Do NOT need space if convention is to start following item with space.
        write_attributes(os); // id="background" (or clip_path)
        style_info_.write(os); // stroke="rgb(0,0,0)" fill= "rgb(255,0,0)" ...
        os << ">" ;
        for(unsigned int i = 0; i < children.size(); ++i)
        {
          children[i].write(os);
        }
        os << "</g>" << std::endl;
      }
    } // void write(std::ostream& rhs)

    g_element& g(int i)
    { // i is index of children nodes.
      return *(static_cast<g_element*>(&children[i]));
    }

    g_element& g()
    { //! Add a new group element.
      //! \return A reference to the new child node just created.
      // was add_g_element
      children.push_back(new g_element());
      return *(static_cast<g_element*>(&children[children.size()-1]));
    }

    line_element& line(double x1, double y1, double x2, double y2)
    { //! Add a new line element.
      //! \return A reference to the new child node just created.
      children.push_back(new line_element(x1, y1, x2, y2));
      return *(static_cast<line_element*>(&children[children.size()-1]));
    }

    rect_element& rect(double x1, double y1, double x2, double y2)
    { //! Add a new rect element.
      //! \return A reference to the new child node just created.
      children.push_back(new rect_element(x1, y1, x2, y2));
      return *(static_cast<rect_element*>(&children[children.size()-1]));
    }

    circle_element& circle(double x, double y, unsigned int radius = 5)
    { //! Add a new circle element.
      //! \return A reference to the new child node just created.
      children.push_back(new circle_element(x, y, radius));
      return *(static_cast<circle_element*>(&children[children.size()-1]));
    }

    ellipse_element& ellipse(double rx, double ry, double cx, double cy)
    { //! Add a new ellipse element.
      //! \return A reference to the new child node just created.
      children.push_back(new ellipse_element(rx, ry, cx, cy));
      return *(static_cast<ellipse_element*>(&children[children.size()-1]));
    }

    text_element& text(double x = 0., double y = 0.,
    const std::string& text = "",
    const text_style& style = no_style, // Use svg implementation's defaults.
    const align_style& align = left_align,
    const rotate_style& rotate = horizontal)
    { //! Add a new text element.
      //! \return A reference to the new child node just created.
      children.push_back(new text_element(x, y, text, style, align, rotate));
      return *(static_cast<text_element*>(&children[children.size()-1]));
    }

    // push_back info about polygon shapes:

    // Polygon for shapes with many vertices.
    polygon_element& polygon(double x, double y, bool f = true)
    { //! Add a new polygon element.
      //! \return A reference to the new child node just created.
      children.push_back(new polygon_element(x, y, f));
      return *(static_cast<polygon_element*>(&children[children.size()-1]));
    }

    //JVTODO: Replace with template version
    polygon_element& polygon(std::vector<poly_path_point>& v, bool f = true)
    { //! Add a new complete polygon element.
      //! \return A reference to the new child node just created.// push_back a complete many-sided polygon to the document.
      children.push_back(new polygon_element(v, f));
      return *(static_cast<polygon_element*>(&children[children.size()-1]));
    }

    //JVTODO: Replace with template version
    polyline_element& polyline(std::vector<poly_path_point>& v)
    {  //! Add a new complete polyline.
       //! \return A reference to the new child node just created.
      children.push_back(new polyline_element(v));
      return *(static_cast<polyline_element*>(&children[children.size()-1]));
    }

    polyline_element& polyline(double x, double y)
    { //! Add a new polyline element, but 1st point only, add others later with .P(x, y)...
      //! \return A reference to the new child node just created.
      children.push_back(new polyline_element(x, y));
      return *(static_cast<polyline_element*>(&children[children.size()-1]));
    }

    polygon_element& triangle(double x1, double y1, double x2, double y2, double x3, double y3, bool f = true)
    { //! Add a new triangle element.
      //! \return A reference to the new child node just created.
      children.push_back(new polygon_element(x1, y1, x2, y2, x3, y3, f));
      return *(static_cast<polygon_element*>(&(children[(unsigned int)(children.size()-1)])));
    }

    polygon_element& rhombus(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, bool f = true)
    { //! Add a new rhombus element.
      //! \return A reference to the new child node just created.
      children.push_back(new polygon_element(x1, y1, x2, y2, x3, y3, x4, y4, f = true));
      return *(static_cast<polygon_element*>(&(children[(unsigned int)(children.size()-1)])));
    }

    polygon_element& pentagon(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double x5, double y5, bool f = true)
    { //! Add a new pentagon element.
      //! \return A reference to the new child node just created. // push_back a complete pentagon to the document.
      children.push_back(new polygon_element(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, f));
      return *(static_cast<polygon_element*>(&(children[(unsigned int)(children.size()-1)])));
    }

    polygon_element& hexagon(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double x5, double y5, double x6, double y6, bool f = true)
    { //! Add a new hexagon element.
      //! \return A reference to the new child node just created. // push_back a complete 6-sided star to the document.
      children.push_back(new polygon_element(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, f));
      return *(static_cast<polygon_element*>(&(children[(unsigned int)(children.size()-1)])));
    }

    polygon_element& polygon()
    { //! Add a new polygon element.
      //! \return A reference to the new child node just created.
      children.push_back(new polygon_element()); // Empty polygon,
      // to which poly_path_points can be added later using member function P.
      return *(static_cast<polygon_element*>(&(children[(unsigned int)(children.size()-1)])));
    }

    polyline_element& polyline()
    { //! Add a new polyline element.
      //! \return A reference to the new child node just created.
      children.push_back(new polyline_element()); // Empty polyline.
      return *(static_cast<polyline_element*>(&(children[(unsigned int)(children.size()-1)])));
    }

    path_element& path()
    { //! Add a new path element.
      //! \return A reference to the new child node just created.
      children.push_back(new path_element()); // Empty path.
      return *(static_cast<path_element*>(&(children[(unsigned int)(children.size()-1)])));
    }

    void push_back(svg_element* g)
    { //! Add a new g_element.
      children.push_back(g);
    }

    void clear()
    {
      children.clear();
    }
  }; // class g_element

} // namespace svg
} // namespace boost

#endif // BOOST_SVG_TAG_HPP
