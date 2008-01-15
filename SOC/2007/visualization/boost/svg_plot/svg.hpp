// svg.hpp

// Copyright Jacob Voytko 2007
// Copyright Paul A Bristow 2007

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_SVG_HPP
#define BOOST_SVG_SVG_HPP

#include <string>
#include <ostream>
#include <fstream>
#include <exception>
#include <vector>

//#include "stylesheet.hpp" // TODO better to be called svg_stylesheet.hpp?
#include "detail/svg_tag.hpp" // element class definitions.
#include "svg_style.hpp"
//#include "svg_fwd.hpp"

// SVG stands for Scalable Vector Graphics,
// an XML grammar for stylable graphics, usable as an XML namespace.

// http://www.adobe.com/svg/basics/intro.html SVG Workflow on optimising SVG
// Gzip compression - can give files that are 1/10th size of gif or jpeg.
// Use default values whenever possible rather than defining all attributes and properties explicitly.
// Take advantage of the path data compaction facilities — use relative coordinates;
// use h and v for horizontal and vertical lines;
// use s or t for cubic and quadratic Bezier segments whenever possible;
// eliminate extraneous white space and separators.
// Use symbols if the same graphic appears multiple times in the document.
//Use CSS property inheritance and selectors to consolidate commonly used properties into named styles
// or to assign the properties to a parent <g> element.
//Use filter effects to help construct graphics via client-side graphics operations. 

namespace boost
{
namespace svg
{ // Class to output Scalable Vector Graph XML,
  // graph elements, point, line, circle, rect and, text...
  // and handle plot stylesheets.

  // Inserted as a SVG comment, for example
  // "<!-- Demo of 1D plot features. --> \n"
  // and also as a <desc />
  // http://www.w3.org/TR/SVG/struct.html#DescriptionAndTitleElements
  // 5.4 The 'desc' and 'title' elements

    static const std::string package_info =
      "<!-- SVG plot written using Boost.Plot program (Creator Jacob Voytko) --> \n"
      "<!-- Use, modification and distribution of Boost.Plot subject to the --> \n"
      "<!-- Boost Software License, Version 1.0.--> \n"
      "<!-- (See accompanying file LICENSE_1_0.txt --> \n"
      "<!-- or copy at http://www.boost.org/LICENSE_1_0.txt) --> \n";

  // Copyright notice to be inserted into plot image produced by this program.
  // Note: can have more than one copyright date, or a range.
  // produces a copyright notice as an SVG comment like this:
  // "<!-- Copyright Paul A. Bristow, 2007  --> \n"
  // and as a meta item:
  // <meta name="copyright" content="Paul A. Bristow" />


class svg;

// See svg_fwd.hpp for forward declarations.
// Public member functions:
//   svg() constructor, and copy constructor.
// Chainable set and get functions for image size, description, title, copyright and date.
// image size x and y, and both.
// svg& x_size(unsigned int x)
// unsigned int x_size()
// svg& y_size(unsigned int y)
// unsigned int y_size()
// svg& image_size(unsigned int x, unsigned int y)
// information about the image file
// svg& description(std::string d)
// std::string& description()
// svg& document_title(std::string d)
// std::string document_title()
// svg& copyright_holder(std::string d)
// svg& copyright_date(std::string d)
// const std::string copyright_date()

// Write information about svg shapes:
// svg& line(double x1, double y1, double x2, double y2)
// svg& rect(double x1, double y1, double x2, double y2)
// svg& circle(double x, double y, unsigned int radius = 5)
// svg& ellipse(double rx, double ry, double cx, double cy)

// write image out to ostream and file:
// svg& write(std::ostream& s_out)
// svg& write(const std::string& file)

// svg& load_stylesheet(const std::string& input) // Load a stylesheet into string css from an input file.

// Possibles for license strings:
// static const std::string permit("permits");
// static const std::string require("requires");
// static const std::string prohibit("prohibits");

class svg
{
protected:
  unsigned int x_size_; // image size.
  unsigned int y_size_;

  g_element document; // group_element to hold all elements of svg document.
  std::vector<clip_path_element> clip_paths;
  std::string title_document_; // SVG document title (for header as <title>).
  std::string image_desc_; // SVG image description (for header as <desc>).
  std::string holder_copyright_; // SVG info.
  std::string date_copyright_; // SVG info.
  std::string css; // Cascading Style Sheet.
  std::string filename_; // file written to.
  std::string author_; // Probably == copyright holder.
  bool is_license_;
  bool is_boost_license_;
  std::string reproduction_; // "permits", "requires", or "prohibits"
  std::string attribution_;
  std::string commercialuse_;
  std::string distribution_;
  int coord_precision_; // decimal digits precision for output of x and y coordinates to svg.

private:

  // ---------------------------------------------------------
  // Print the svg 1.1 header into the svg g_element document.
  // ---------------------------------------------------------
  void write_header(std::ostream& s_out)
  {
    s_out << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
      << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" "
      << "\"http://www.w3.org/graphics/svg/1.1/dtd/svg11.dtd\">"
      << std::endl;
    // TODO IE6 with updates does not recognise this DOCTYPE, but displays OK.
    // http://jwatt.org/svg/authoring/#namespace-binding recommends NO DOCTYPE!


    //s_out << "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n"
    //         "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 20000303 Stylable//EN\"
    //         \"http://www.w3.org/TR/2000/03/WD-SVG-20000303/DTD/svg-20000303-stylable.dtd\">\n"
    // This IS out of date? version 1.0 TR 2000, but the Adobe SVG viewer complains
    // if it is missing - but ignore.
    // But Firefox says document does not contain style.
    // http://www.adobe.com/svg/basics/getstarted.html

    // Inkscape uses encoding=\"UTF-8\" Unicode
    // (encoding=\"iso-8859-1\" == Latin now obselete)

    // example of style command:
    // style="font-size:20px;font-weight:bold;text-decoration: line-through;fill:aqua"
    
    // font-family:Arial Narrow

    // INscape uses: xmlns:svg="http://www.w3.org/2000/svg" xmlns="http://www.w3.org/2000/svg"
  }

  void write_css(std::ostream& s_out)
  { // CSS (Cascading Style Sheet)
    if (css.size() != 0) // css != ""
    { // TODO confirm that this isn't useful if css is "".
      // [CDATA[ ... ]] enclosing the style information
      // is a standard XML construct for hiding information
      // necessary since CSS style sheets can include characters,
      // such as ">", which conflict with XML parsers.
      s_out << "<defs><style type=\"text/css\"><![CDATA[" << css
        << "]]></style></defs>" << std::endl;
      // CSS inline style can be declared within a style attribute in SVG 
      // by specifying a semicolon-separated list of property declarations, 
      // where each property declaration has the form "name: value".
      // For example a style: style="fill:red; stroke:blue; stroke-width:3"
      // class=
      // Multiple class names must be separated by whitespace.

      // Example: <defs><style type="text/css"><![CDATA[]]>
      // .axes { fill:none;stroke:#333333;stroke-width:1.6 }
      // .title{ font-size:20px;font-weight:bold;font-style:italic;fill:magenta }
      // .legend_header{ font-size:16px;font-weight:bold;fill:darkblue;text-anchor:middle }
      // .legend_item{ font-size:16px;font-weight:normal;fill:blue }
      // .x_axis_value{ font-size:12px;font-weight:normal;fill:black }
      //   </style></defs>
    }
  }

  // ----------------------------------------------------------------
  // Internal function to write all of the image to the svg document.
  // ----------------------------------------------------------------
  void write_document(std::ostream& s_out)
  {
    // Write all clip paths that 
    // define a region of the output device
    // to which paint can be applied.
    for(size_t i = 0; i < clip_paths.size(); ++i)
    {
      clip_paths[ (unsigned int)i ].write(s_out);
    }
    // Write all visual elements.
    for(size_t i = 0; i < document.size(); ++i)
    { // plot_background, grids, axes ... title
      document[ (unsigned int)i ].write(s_out);
    }
  } // write_document

public:

  svg()
    : x_size_(400), y_size_(400), // of the whole SVG image.
    title_document_(""),  // This is document title, not plot title.
    image_desc_(""), // Information about the plot, for example the program that created it.
    author_(""), // Default to copyright holder.
    holder_copyright_(""),  //
    date_copyright_(""),  //
    css(""), // stylesheet.
    filename_(""), // If written only to ostream, filename will not appear in comment.
    is_license_(false), // No default license.
    is_boost_license_(false), // No Boost license unless requested.
    reproduction_("permits"), // Set with license:
    distribution_("permits"), // permits, requires, or prohibits.
    attribution_("requires"),
    commercialuse_("permits"),
    coord_precision_(3) // enough for 1 in 1000 resolution to suit small image use.
  { // Default constructor.
  }

  svg(const svg& rhs) : x_size_(rhs.x_size_), y_size_(rhs.y_size_)
  { // Defined image size copy constructor.
    // TODO Other member data items are NOT copied.  OK?
  }

  // Set & get functions for x_size_ and y_size_
  svg& x_size(unsigned int x)
  {
    x_size_ = x;
    return *this;
  }

  unsigned int x_size()
  { //  x_size_ name is now used in class to permit function called x_size.
    // TODO Better called image_x_size()?
    return x_size_;
  }

  svg& y_size(unsigned int y)
  {
    y_size_ = y;
    return *this;
  }

  unsigned int y_size()
  { //  y_size_ name is now used in class to permit function called y_size.
    return y_size_;
  }

  unsigned int document_size()
  { // How many elements have been added to document.
    return static_cast<unsigned int>(document.size());
  }

  svg& coord_precision(int digits)
  { // Set decimal digits to be output for output of coordinates.
    // Default stream precision 6 decimal digits is probably excessive.
    // 4.1 Basic data types, integer or float in decimal or scientific (using e format).
    // - probably enough if image size is under 1000 x 1000.
    // This will reduce .svg file sizes significantly for curves represented with many data points.
    // For example, if a curve is shown using 100 points,
    // reducing to precision(3) from 6 will reduce file size by 300 bytes.
    // So a default of 3 is used in the default constructor above,
    // but can be changed using this function.
    // Used in svg.write below and so applies to all the svg document.
    coord_precision_ = digits;
    return *this;
  }

  int coord_precision()
  { // 
    return coord_precision_;
  }

  // svg.write() also has two flavors, a file and an ostream.
  // The file version opens an ostream, and calls the stream version.
  // The stream version first clears all unnecessary data from the graph,
  // builds the document tree, and then calls the write function for the root
  // document node, which calls all other nodes through the Visitor pattern.

  // TODO provide a filtered-stream version that writes in zipped format type .svgz ?
  // http://lists.w3.org/Archives/Public/www-svg/2005Dec/0308.html
  // recommends MUST have  correct Content-Encoding headers.
  // --------------------------------------------------------------------------------

  svg& write(const std::string& file)
  {// Write whole .svg 'file' contents to file.
    std::ofstream f_out(file.c_str());
    if(f_out.fail())
    {
      filename_ = ""; // Erase any previous filename,
      // so does not appear in comment if later written just to stream.
      throw std::runtime_error("Unable to open file " + file);
    }
    filename_ = file; // Note so that can embed into file as comment.
    write(f_out);
    return *this;
  }

  svg& write(std::ostream& s_out)
  { // Write whole .svg 'file' contents to stream (perhaps a file).
    write_header(s_out); // "<?xml version=...
    // write svg document, begin <svg tag.
    // <svg xml:space="preserve" width="5.5in" height=".5in">

    s_out << "<svg width=\"" << x_size_ << "\" height =\"" << y_size_
      << "\" version=\"1.1\"" // http://www.w3.org/TR/SVG11/
      // 1.2 draft specification at http://www.w3.org/TR/SVG12/
      << " xmlns=\"http://www.w3.org/2000/svg\"\n"
      // xml namespace containing svg shapes rect, circle...
      // so can write rect or circle avoiding need for qualification svg:rect, svg:circle...
      // This site isn't visited, but if missing Firefox, at least, will fail to render.
         " xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n"
      // tells that elements and attributes which are prefixed by "xlink:"
      // are a part of the xlink specification http://www.w3.org/1999/xlink.
      // Need to use xlink:href to refer to xlink.
         " xmlns:ev=\"http://www.w3.org/2001/xml-events\">\n"
      << std::endl;
    // Bind the required namespaces, see http://jwatt.org/svg/authoring/#namespace-binding
    // Added xlink and ev in case needed.

    s_out << package_info << std::endl;

    if (author_ == "")
    {
      author_ = holder_copyright_;
    }
    else
    {
      if (holder_copyright_ == "")
      {
        holder_copyright_ = author_;
      }
      else
      { // Copyright has been assigned to another, so list separately.
        s_out << "<!-- " << author_ << " --> "<< std::endl;
      }
    }
    if (holder_copyright_ != "")
    { // Output copyright & date as both comment and meta data.
      s_out << "<!-- SVG Plot Copyright " << holder_copyright_ << " " << date_copyright_ << " --> "<< std::endl;
      s_out << "<meta name=\"copyright\" content=\"" << holder_copyright_ << "\" />" << std::endl;
      //  Example:  <meta name="copyright" content="Paul A. Bristow" />
      s_out << "<meta name=\"date\" content=\"" << date_copyright_ << "\" />" << std::endl;
      // Example: <meta name="Date" content="20071101">
    }
    if (image_desc_ != "")
    {
      s_out << "<!-- " << image_desc_ << " --> "<< std::endl;
      s_out << "<desc>" << image_desc_ << "</desc>" << std::endl;
    }
    if (title_document_ != "")
    {
       s_out << "<title>" << title_document_ << "</title>" << std::endl;
    }
    if (filename_ != "")
    { // Example: <!-- File demo_1d_plot.svg -->
      s_out << "<!-- File " << filename_ << " --> "<< std::endl;
    }
    s_out.precision(coord_precision());

    if (is_boost_license_ == true)
    {
      s_out << 
        "<!-- Use, modification and distribution of this Scalable Vector Graphic file -->"
        "\n<!-- are subject to the Boost Software License, Version 1.0. -->"
        "\n<!-- (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt) -->\n" 
        << std::endl;
    } // is_boost_license
    if (is_license_ == true)
    {
      s_out <<
        "<metadata>\n"
          "<rdf:RDF xmlns:cc=\"http://web.resource.org/cc/\"\n"
             "xmlns:dc=\"http://purl.org/dc/elements/1.1/\"\n"
             "xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\">\n"
             "<cc:work rdf:about=\" " << filename_ << "\">\n" // Presumably .svg (or svgz?)
               "<dc:title>" << (title_document_ != "" ? title_document_ : filename_) << "</dc:title>\n"
               "<dc:creator>Boost.Plot</dc:creator>\n"
               "<dc:author> " << author_ << "</dc:author>\n"
               "<dc:rights> " << holder_copyright_ << ", " << date_copyright_ << "</dc:rights>\n"
               "<dc:format>application/xhtml+xml+svg</dc:format>\n"
               "<cc:license rdf:about=\"http://creativecommons.org/licenses\">\n"
               "<cc:requires rdf:resource=\"http://web.resource.org/cc/Notice\"/>\n"
               "<cc:" << reproduction_ << " rdf:resource=\"http://web.resource.org/cc/Reproduction\"/>\n"
               "<cc:" << distribution_ << " rdf:resource=\"http://web.resource.org/cc/Distribution\"/>\n"
               "<cc:" << attribution_ << " rdf:resource=\"http://web.resource.org/cc/Attribution\"/>\n"
               "<cc:" << commercialuse_ << " rdf:resource=\"http://web.resource.org/cc/CommercialUse\"/>\n"
               "</cc:license>\n"
             "</cc:work>\n"
           "</rdf:RDF>\n"
         "</metadata>"
       << std::endl;
    } // is_license
    write_css(s_out);// stylesheet, if any.
    write_document(s_out); // write clip paths and all document elements.
    s_out << "</svg>" << std::endl;   // close off svg tag.
    return *this;
  }

  svg& license( // Set license requirements for the svg document.
    const std::string repro = "permits",
    const std::string distrib = "permits",
    const std::string attrib = "requires",
    const std::string commercial = "permits")
  { // Might check these are "permits", "requires", or "prohibits"?
    reproduction_ = repro;
    distribution_ = distrib;
    attribution_ = attrib;
    commercialuse_ = commercial;
    is_license_ = true;  // Assume want this if set these?
    return *this;
  }

  svg& license_on(bool l)
  { // Set (or not) license using all requirement (default permits).
    // Implicitly set by setting a license requirement using license above.
    is_license_ = l;
    return *this;
  }

  bool license_on()
  { // Shows if a license has been requested in the svg header metatadata.
    return is_license_;
  }

  svg& boost_license_on(bool l)
  { // Set (or not) Boost license.
    is_boost_license_ = l;
    return *this;
  }

  bool boost_license_on()
  { // Shows if a license has been requested in the svg header metatadata.
    return is_boost_license_;
  }

  const std::string& reproduction()
  { // Gets license reproduction requirement.
    return reproduction_;
  }

  const std::string& distribution()
  { // Gets license distribution requirement.
    return distribution_;
  }
  const std::string& attribution()
  { // Gets license attribution requirement.
    return attribution_;
  }
  const std::string& commercialuse()
  { // Gets license commercialuse requirement.
    return commercialuse_;
  }

  // -------------------------------------------------------
  // Writes the information about the image to the document.
  // TODO: allow other unit identifiers.
  // -------------------------------------------------------
  svg& image_size(unsigned int x, unsigned int y)
  {
    x_size_ = x;
    y_size_ = y;
    return *this;
  }

  svg& description(const std::string d)
  { // Writes description to the document(for header as <desc>).
    image_desc_ = d;
    return *this;
  }

  const std::string& description()
  { // Gets description of the document(for header as <desc>).
    return image_desc_;
  }

  svg& author(const std::string a)
  { // Writes author to the document (default is copyright_holder).
    author_ = a;
    return *this;
  }

  const std::string& author()
  { // Gets description of the document(for header as <desc>).
    return author_;
  }

  svg& document_title(const std::string d)
  { // Writes document title for the document(for header as <title>)..
    title_document_ = d;
    return *this;
  }

  const std::string document_title()
  { // Get document title for the document(for header as <title>)..
    return title_document_;
  }

  svg& copyright_holder(const std::string d)
  { // Writes document title for the document(for header as <title>)..
    holder_copyright_ = d;
    return *this;
  }

  const std::string copyright_holder()
  { // Get document title for the document(for header as <title>)..
    return holder_copyright_;
  }

  svg& copyright_date(const std::string d)
  { // Writes document title for the document(for header as <title>)..
    date_copyright_ = d;
    return *this;
  }

  const std::string copyright_date()
  { // Get document title for the document(for header as <title>)..
    return date_copyright_;
  }

  svg& image_filename(const std::string filename)
  { // Writes image filename for the document(for header as <title>)..
    filename_ = filename;
    return *this;
  }

  const std::string image_filename()
  { // Get image filename for the document(for header as <title>)..
    return filename_;
  }

  // ------------------------------------------------------------------------
  // push_back information about line, rec, circle & ellipse to the document.
  // ------------------------------------------------------------------------
  svg& line(double x1, double y1, double x2, double y2)
  { // 'line' element defines a line segment
    // that starts at one point and ends at another.
    document.push_back(new line_element(x1, y1, x2, y2));
    return *this;
  }

  svg& rect(double x1, double y1, double x2, double y2)
  {
    document.push_back(new rect_element(x1, y1, x2, y2));

    return *this;
  }

  svg& circle(double x, double y, unsigned int radius = 5)
  {
    document.push_back(new circle_element(x, y, radius));
    return *this;
  }

  svg& ellipse(double rx, double ry, double cx, double cy)
  { //
    document.push_back(new ellipse_element(rx, ry, cx, cy));
    return *this;
  }

  // -------------------------------------------------
  // push_back information about text to the document.
  // -------------------------------------------------
  svg& text(double x, double y, const std::string& text,
    const text_style& style, // size, font etc.
    align_style align = center_align, rotate_style rotate = horizontal
    )
  {
    document.push_back(new text_element(x, y, text, style, align, rotate) );
    return *this;
  }

  // push_back info about polygon shapes:
  // Polygon for shapes with many vertices.
  
  svg& polygon(double x, double y, bool f = true) // 1st point only, add others later with .P(x, y).
  {
    document.push_back(new polygon_element(x, y, f));
    return *this; 
  }

  svg& polygon(std::vector<poly_path_point>& v, bool f = true)
  { // push_back a complete many-sided polygon to the document.
    document.push_back(new polygon_element(v, f));
    return *this; // svg& 
  }

  // Specific polygon shapes: triangle, rhombus, pentagon & hexagon.

  svg& triangle(double x1, double y1, double x2, double y2, double x3, double y3, bool f = true)
  { // push_back a complete triangle to the document.
    document.push_back(new polygon_element(x1, y1, x2, y2, x3, y3, f));
    return *this; // svg& 
  }

  svg& rhombus(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, bool f = true)
  { // push_back a complete rhombus to the document.
    document.push_back(new polygon_element(x1, y1, x2, y2, x3, y3, x4, y4, f));
    return *this; // svg& 
  }

  svg& pentagon(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double x5, double y5, bool f = true)
  { // push_back a complete pentagon to the document.
    document.push_back(new polygon_element(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, f));
    return *this; // svg& 
  }

  svg& hexagon(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double x5, double y5, double x6, double y6, bool f = true)
  { // push_back a complete 6-sided star to the document.
    document.push_back(new polygon_element(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, f));
    return *this; // svg& 
  }

  // push_back info about polylines.

  svg& polyline(double x, double y) // 1st point only, add others later with .P(x, y).
  {
    document.push_back(new polyline_element(x, y));
    return *this; 
  }

  svg& polyline(double x1, double y1, double x2, double y2)
  { // Two points only, add others later with .P(x, y).
    document.push_back(new polyline_element(x1, y1));
    document.push_back(new polyline_element(x2, y2));
    return *this; 
  }

  svg& polyline(std::vector<poly_path_point>& v, bool f = true)
  { // push_back a complete many-sided polygon to the document,
    // from a vector of points.
    document.push_back(new polygon_element(v, f));
    return *this; // svg& 
  }

  // --------------------------------------------------------------------------------
  // Writes the information about path, clip_path to the document.
  // --------------------------------------------------------------------------------

  path_element& path()
  {
    document.push_back(new path_element());  // empty path, ready for additions with M., L. ...
    return *(static_cast<path_element*>(&(document[(unsigned int)(document.size()-1)])));
    // reference to the path_element just pushed.
  }

  svg& clip_path(const rect_element& rect, const std::string& id)
  { // Rectangle outside which 'painting' is 'clipped' so doesn't show.
    clip_paths.push_back(clip_path_element(id, rect));
    return *this;
  }

  // -------------------------------------------------------------
  // Writes information about a group element to the document.
  // -------------------------------------------------------------

  g_element& add_g_element()
  {
    return document.add_g_element();
  }

  g_element& get_g_element(int i)
  { // Array of g_elements document,
    // indexed by group type, PLOT_BACKGROUND, PLOT_WINDOW_BACKGROUND, ... SVG_PLOT_DOC_CHILDREN
    return document.g_tag(i);
  }

  //// -------------------------------------------------------------
  //// Load stylesheet
  //// -------------------------------------------------------------

  //svg& load_stylesheet(const std::string& input)
  //{ // Load a stylesheet into string css from an input file.
  //  std::ifstream if_str(input.c_str());

  //  if(if_str.fail())
  //  {
  //    throw std::runtime_error("Error opening file " + input);
  //  }
  //  if(!validate_stylesheet(if_str))
  //  {
  //    throw std::runtime_error("Error loading stylesheet!");
  //  }
  //  if_str.clear();
  //  if_str.seekg(0);
  //  std::string tmp;
  //  css = "";
  //  while(std::getline(if_str, tmp))
  //  {
  //    css += tmp;
  //  }
  //  return *this;
  //} // svg& load_stylesheet
}; // class svg

} // namespace svg
} // namespace boost

#endif // BOOST_SVG_SVG_HPP
