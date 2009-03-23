/*!
  \file svg_1d_plot.hpp
  \brief Create 1D plots in Scalable Vector Graphic (SVG) format.
  \details Provides svg_1d_plot data and function to create plots, and svg_1d_plot_series to allow data values to be added.
  Very many functions allow fine control of the appearance and layout of plots and data markers.
  Items common to 1D and 2D use axis_plot_frame.
  \date Mar 2009
  \author Jacob Voytko & Paul A. Bristow
 */

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2008, 2009

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_SVG_1D_PLOT_HPP
#define BOOST_SVG_SVG_1D_PLOT_HPP

#if defined (_MSC_VER)
#  pragma warning (push)
#  pragma warning (disable: 4512) // "assignment operator could not be generated."
#  pragma warning (disable: 4180) // qualifier applied to function type has no meaning; ignored
#endif

#include <boost/iterator/transform_iterator.hpp>
// using boost::make_transform_iterator;

#include "svg.hpp"
#include "svg_style.hpp"
#include "detail/axis_plot_frame.hpp" // Code shared with 2D.
#include <boost/svg_plot/detail/functors.hpp>
using boost::svg::detail::unc_1d_convert;
#include "detail/numeric_limits_handling.hpp"
#include <boost/svg_plot/uncertain.hpp>
// using boost::svg::unc;

using boost::svg::detail::limit_NaN;

#include <boost/svg_plot/detail/auto_axes.hpp> // provides:
//void scale_axis(double min_value, double max_value, // Input range
//               double* axis_min_value,  double* axis_max_value, double* axis_tick_increment, int* auto_ticks, // All 4 updated.
//               bool origin, // If true, ensures that zero is a tick value.
//               double tight, // Allows user to avoid a small fraction over a tick using another tick.
//               int min_ticks, // Minimum number of ticks.
//               int steps); // Round up and down to 2, 4, 6, 8, 10, or 5, 10 or 2, 5, 10 systems.

#include <vector>
#include <ostream>
#include <sstream>
#include <fstream>
#include <string>
#include <exception>

namespace boost
{
namespace svg
{
  // Forward declarations.
  const std::string strip_e0s(std::string s); // Strip unnecessary zeros and e and sign.

  class svg_1d_plot; // 1D Plot (that may include one or more data series).
  class svg_1d_plot_series; // 1D Plot a data series.

// ------------------------------------------------------------------
// This allows us to store plot state locally in svg_plot.
// Not stored in "svg" because transforming the points after they are
// written to the document would be difficult. We store the Cartesian
// coordinates locally and transform them before we write them.
// ------------------------------------------------------------------
class svg_1d_plot_series
{ /*! \class boost::svg::svg_1d_plot_series
   \brief Holds a series of data values (points) to be plotted.
   \details Scan each data point sorting them into the appropriate
   std::vectors, normal or not (NaN or infinite).\n
   Member functions allow control of data points markers and lines joining them,
   and their appearance, shape, color and size.\n
   Each data series can have a title that can be shown on a legend box with identifying symbols.
*/

public:
  std::vector<unc> series_; //!< Normal 'OK to plot' data values.
  std::vector<double> series_limits_; //!< 'limit' values: too big, too small or NaN.
  // TODO should these be unc too?

  std::string title_; //!< title of data series (to show on legend).
  plot_point_style point_style_; //!< circle, square...
  plot_point_style limit_point_style_; //!< Default is cone pointing down.
  plot_line_style line_style_; //!< No line style for 1-D, only for 2-D.

  // Constructor svg_1d_plot_series.
  //! Scan each data point between the iterators that are passed,
  //! sorting them into the appropriate std::vectors, normal or not (NaN or infinite).
  template <class T> // \tparam T an STL container: array, vector<double>, set, map ...
  svg_1d_plot_series(T begin, T end, const std::string& title = "");

  // Declarations of Set functions for the plot series.

  svg_1d_plot_series& fill_color(const svg_color& col_); //!< Set fill color for plot point marker(s) (chainable).
  svg_1d_plot_series& stroke_color(const svg_color& col_); //!< Set stroke color for plot point marker(s) (chainable).
  svg_1d_plot_series& shape(point_shape shape_); //!< Set shape for plot point marker(s) (chainable).
  svg_1d_plot_series& symbols(const std::string s); //!< Set symbol(s) for plot point marker(s) (chainable).
  svg_1d_plot_series& size(int size_); //!< Set font size for plot point marker(s) (chainable).
  svg_1d_plot_series& line_color(const svg_color& col_); //!< Set color for line joining data points (chainable).
  svg_1d_plot_series& line_width(double wid_); //!< Set line width for plot point marker(s) (chainable).
  svg_1d_plot_series& line_on(bool on_); //!< Set true for line joining points for plot point marker(s) (chainable).
  svg_1d_plot_series& bezier_on(bool on_); //!< Set true for curve joining points for plot point marker(s) (chainable).

  // Get functions for the plot series.
  svg_color fill_color(); //!< Get fill color for plot point marker(s).
  svg_color stroke_color(); //!< Get stroke color for plot point marker(s).
  int size(); //!< Get size for plot point marker(s).
  point_shape shape(); //!< Get shape for plot point marker(s).
  const std::string symbols(); //!< Get sumbols for plot point marker(s).
  double line_width(); //!< Get line width for plot point marker(s).
  bool line_on(); //!< Get true if line is to join data points.
  bool bezier_on(); //!< Get true if curve if to join data points.
  size_t series_count(); //!< Get number of normal data points in this data series.
  size_t series_limits_count();//!< Get number of 'at limits' data points in this data series.
}; // class svg_1d_plot_series

// class svg_1d_plot_series Constructor
template <class T> // T an STL container: array, vector<double>, set, map ...
svg_1d_plot_series::svg_1d_plot_series(T begin, T end, const std::string& title)
: // Constructor.
title_(title),
point_style_(black, blank, 5, vertical_line), // Default point style.
limit_point_style_(lightgrey, red, 10, cone), // Default limit (inf or NaN) point style.
//limit_point_style_(lightgrey, whitesmoke, 10, cone), // Default limit (inf or NaN) point style.
line_style_(black, blank, 2, false, false) // Default line style, black, no fill, width, line_on, bezier_on false
{
  for(T i = begin; i != end; ++i)
  {  // No defaults for begin and end?
    unc temp = *i;
    if(detail::is_limit(temp.value()))
    {
      series_limits_.push_back(temp.value()); // 'limit' values: too big, too small or NaN.
    }
    else
    {
      series_.push_back(temp); // Normal 'OK to plot' data values.
    }
  }
} // svg_plot_series constructor.

// Definitions of svg_plot_series Member Functions.
svg_1d_plot_series& svg_1d_plot_series::fill_color(const svg_color& col_)
{ //! Set fill color for plot point marker(s) (chainable).
  point_style_.fill_color_ = col_;
  return *this; //! \return reference to svg_1d_plot_series to make chainable.
}

svg_1d_plot_series& svg_1d_plot_series::stroke_color(const svg_color& col_)
{ //! Set stroke color for plot point marker(s) (chainable).
  point_style_.stroke_color_ = col_;
  return *this; //! \return reference to svg_1d_plot_series to make chainable.
}

svg_1d_plot_series& svg_1d_plot_series::shape(point_shape shape_)
{ //! Set shape for plot point marker(s) (chainable).
  point_style_.shape_ = shape_;
  return *this; //! \return reference to svg_1d_plot_series to make chainable.
}

point_shape svg_1d_plot_series::shape()
{ //! \return  shape for plot point marker(s).
  return point_style_.shape_;
}

svg_1d_plot_series& svg_1d_plot_series::symbols(const std::string s)
{ //! Set symbol for plot point marker(s).
  point_style_.symbols_ = s;
  return *this; //! \return reference to svg_1d_plot_series to make chainable.
}

svg_1d_plot_series& svg_1d_plot_series::size(int size_)
{ //! Set size of plot point marker(s).
  //point_style_.size_ = size_;
  //point_style_.symbols_style_.font_size(i); // in case using a symbol.
  point_style_.size(size_);
  return *this; //! \return reference to svg_1d_plot_series to make chainable.
}

int svg_1d_plot_series::size()
{ //! \return  size of plot point marker(s).
  return point_style_.size();
}

const std::string svg_1d_plot_series::symbols()
{ //! \return  symbol for plot point marker(s).
  return point_style_.symbols_;
}

svg_1d_plot_series& svg_1d_plot_series::line_color(const svg_color& col_)
{ //! Set color of any line joining plot points.
  line_style_.stroke_color_ = col_;
  return *this; // Make chainable.
}

svg_1d_plot_series& svg_1d_plot_series::line_width(double wid_)
{ //! Set width of any line joining plot points.
  line_style_.width_ = wid_;
  return *this; // Make chainable.
}

double svg_1d_plot_series::line_width()
{ //! \return  width of any line joining plot points.
  return line_style_.width_;
}

svg_1d_plot_series& svg_1d_plot_series::line_on(bool on_)
{ //! Set true if to draw a line joining plot points.
  line_style_.line_on_ = on_;
  return *this; // Make chainable.
}

bool svg_1d_plot_series::line_on()
{ //! \return true if to draw a line joining plot points.
  return line_style_.bezier_on_;
}

svg_1d_plot_series& svg_1d_plot_series::bezier_on(bool on_)
{ //! Set true if to draw bezier curved line joining plot points.
  line_style_.bezier_on_ = on_;
  return *this; // Make chainable.
}

bool svg_1d_plot_series::bezier_on()
{ //! \return true if to draw bezier curved line joining plot points.
  return line_style_.bezier_on_;
}

size_t svg_1d_plot_series::series_count()
{ //! Number of normal 'OK to plot' data values in data series.
  return series_.size();
}

size_t svg_1d_plot_series::series_limits_count()
{ //!  Number of  'at limit' values: too big, too small or NaN data values in data series.
  return series_limits_.size();
}

// End Definitions of svg_plot_series Public Member Functions.

class svg_1d_plot : public detail::axis_plot_frame<svg_1d_plot>
{ /*! \class boost::svg::svg_1d_plot
     \brief Hold all data about a plot, and functions to get and set.
     \details All data about a plot, and functions to get and set appearance.
      axis_plot_frame.hpp contains functions common to 1 and 2-D.
     See also svg_2d_plot.hpp for 2-D version.
    */

  friend void show_1d_plot_settings(svg_1d_plot&);
  friend void show_2d_plot_settings(svg_1d_plot&);
  friend class detail::axis_plot_frame<svg_1d_plot>;

 protected:
  // Member data names conventionally end with _.
  // for example: border_margin_,
  // and set & get accessor functions are named without _ suffix,
  // for example: border_margin() & border_margin(int).

  double x_scale_; //! scale used for transform from Cartesian to SVG coordinates.
  double x_shift_; //! shift from SVG origin is top left, Cartesian is bottom right.
  double y_scale_; //! scale used for transform from Cartesian to SVG coordinates.
  double y_shift_; //! shift from SVG origin is top left, Cartesian is bottom right.

  svg image; // Stored so as to avoid rewriting style information constantly.

  double text_margin_; //! Marginal space around text items like title,

  text_style a_style_; //! Default text style that contains font size & type etc.
  text_style title_style_;
  text_style legend_style_;
  text_style x_axis_label_style_;
  text_style y_axis_label_style_;  //! Not used for 1D but needed by axis_plot_frame.hpp.
  text_style x_value_label_style_;
  text_style y_value_label_style_; //! Not used for 1D but needed by axis_plot_frame.hpp.
  text_style point_symbols_style_; //! Used for data point marking.
  text_style value_style_; //! Used for data point value label.

  value_style x_values_style_; //! Used for data point value marking.
  //rotate_style x_value_label_rotation_; // Direction point value labels written.
  //int x_value_precision_;
  //std::ios_base::fmtflags x_value_ioflags_;

  text_element title_info_; //! Title of whole plot.
  text_element legend_header_; //! legend box header or title (if any).
  text_element x_label_info_; //! X-axis label, Example: "length of widget"
  text_element x_label_value_; // For example: "1.2" or "1.2e1"
  text_element x_units_info_; // For example, to display, "length (meter)"

  // No Y-axis info for 1D.
  // Note that text_elements hold font_size, bold, italic...

  // Border information for the plot window (not the full image size).
  box_style image_border_; // rectangular border of all image width, color...
  box_style plot_window_border_; // rectangular border of plot window width, color...
  box_style legend_box_; // rectangular box of legend width, color...

  double plot_left_; // calculate_plot_window() sets these values.
  double plot_top_;
  double plot_right_;
  double plot_bottom_;

  // enum legend_places{ nowhere, inside...}
  legend_places legend_place_; // Place for any legend box.
  double legend_width_; // Width of legend box (pixels).
  double legend_height_; // Height of legend box (in pixels).
  // Size of legend box is controlled by its contents,
  // but helpful to store computed coordinates.
  double legend_left_; // Left of legend box.
  double legend_top_; // Top of legend box.
  // Both optionally set by legend_top_left.
  double legend_right_; // SVG Coordinates of right of legend box,
  double legend_bottom_; // bottom of legend box.
  size_t legend_longest_; // longest (both header & data) string in legend box,

  axis_line_style x_axis_;
  axis_line_style y_axis_;

  ticks_labels_style x_ticks_;
  ticks_labels_style y_ticks_; // Added to permit shared code!

  bool title_on_; // Provide a title for the whole plot.
  bool legend_on_; // Provide a legend box.
  bool outside_legend_on_; // Legend box outside the plot window.
  bool legend_lines_; // Data colored line type in legend box.
  bool plot_window_on_; // Use a separate plot window (not whole image).
  bool x_ticks_on_; // TODO check these are really useful.
  bool x_values_on_; // values of data are shown by markers.
  int  x_axis_position_;
  bool autoscale_check_limits_; // Whether to check autoscale values for infinity, NaN, max, min.
  bool x_autoscale_; // Whether to use any autoscale values.
  bool x_include_zero_; // If autoscaled, include zero.
  int  x_min_ticks_;  // If autoscaled, set a minimum number of ticks.
  double x_tight_;
  int  x_steps_;  // If autoscaled, set any prescaling to decimal 1, 2, 5, 10 etc.

  // Values calculated by scale_axis, and is used only if x_autoscale == true.
  double x_auto_min_value_;
  double x_auto_max_value_;
  double x_auto_tick_interval_; // tick major interval.
  int x_auto_ticks_; // Number of ticks.

  // Similar y_auto value for 2-D
  bool y_autoscale_; // Stays false for 1-D plot.

  std::string plot_window_clip_; // = "clip_plot_window" id for clippath
  // http://www.w3.org/TR/SVG/masking.html#ClipPathElement 14.1 Introduction
  // clipping paths, which uses any combination of 'path', 'text' and basic shapes
  // to serve as the outline where everything on the "inside" of the outline
  // is allowed to show through but everything on the outside is masked out.
  // So the plot_window_clip_ limits display to a plot_window rectangle.

  // Storing the (perhaps several) series of data points for transformation.
  std::vector<svg_1d_plot_series> serieses_;
  // These are sorted into two vectors for normal and abnormal (max, inf and NaN).

public:
  svg_1d_plot()
    : // Default constructor.
    // Many ( but not all - see below) default values here.
    // See documentation for default settings rationale.

    title_style_(18, "Verdana", "", ""),  // last "bold" ?
    legend_style_(14, "Verdana", "", ""), // 2nd "italic"?
    x_axis_label_style_(14, "Verdana", "", ""),
    x_value_label_style_(12, "Verdana", "", ""),
    point_symbols_style_(12, "Lucida Sans Unicode"), // Used for data point marking.
    value_style_(10, "Verdana", "", ""), // Used for data point values.

    title_info_(0, 0, "", title_style_, center_align, horizontal),
    //title_info_(0, 0, "Plot of data", title_style_, center_align, horizontal), when text concatenation solved?
    //x_label_info_(0, 0, "X Axis", x_axis_label_style_, center_align, horizontal),
    //x_units_info_(0, 0, " (units)", x_value_label_style_, center_align, horizontal),
    x_label_info_(0, 0, "", x_axis_label_style_, center_align, horizontal), // Null strings for now.
    x_label_value_(0, 0, "", x_value_label_style_, center_align, horizontal),
    x_units_info_(0, 0, "", x_value_label_style_, center_align, horizontal),
    x_axis_(X, -10., +10., black, 1, 0, true, false, true),
    y_axis_(Y, 0., +1., black, 1, 0, false, false, false), // Not used for 1D.

    // Might fill in all values, but there are rather many for ticks_labels_style,
    x_ticks_(X, x_value_label_style_),// so for defaults see ticks_labels_style.
    text_margin_(2.), // for text as a multiplier of the font size.
    image_border_(yellow, white, 1, 10, true, true), // margin should be about axis label font size.
    plot_window_border_(yellow, svg_color(255, 255, 255), 1, 3, true, false),
    legend_box_(yellow, white, 1, 2, true, true),
    legend_header_(0, 0, "", legend_style_, center_align, horizontal),
    legend_width_(200), // width of legend box (pixels) // TODO isn't this calculated?
    legend_height_(0), // height of legend box (pixels)
    legend_left_(-1), legend_right_(-1),legend_top_(-1),legend_bottom_(-1), // Default top left of plot window.
    legend_place_(outside_right), // default but interacts with using plot_window.
    legend_on_(false),
    legend_longest_(0),
    outside_legend_on_(true),
    legend_lines_(false), // Not very useful for 1 D as already showing data point marker shapes.
    plot_window_clip_("plot_window"), // for <clipPath id="plot_window" ...
    title_on_(true),
    plot_window_on_(true),
    x_values_on_(false), // By default, don't label point values.
    x_values_style_(horizontal, 3, std::ios::dec, true, value_style_, black, black, false, false),
    // Autoscaling.
    autoscale_check_limits_(true), // Do check all value for limits, infinity, max, min, NaN.
    x_autoscale_(false),
    x_include_zero_(false), // If autoscaled, include zero.
    x_min_ticks_(6),  // If autoscaled, set a minimum number of ticks, default 6.
    x_steps_(0),  // If autoscaled, set any prescaling to decimal 1, 2, 5, 10 etc, default none.
    x_tight_(1e-6), // margin that point can lie outside top and bottom tick.
    y_autoscale_(false), // Not used for 1-D.
    // Used to transform Cartesian to SVG.
    x_scale_(1.), x_shift_(0.),
    y_scale_(1.), y_shift_(0.),
    x_axis_position_(0) // Move into axis_style?
  {
    size(500, 200); // Default image size.
    // Only needs to be quite shallow (y_size) for a 1-D plot.
    // (But may need more height if long value labels are used, for example: "[time = 1.23 +-0.02 sec]").
    // 200 barely leaves enough room for five data series in the legend).
    // (2-D usually needs to be much more rectangular).

    using namespace boost::svg::detail; // Avoid detail:: specification.

    // Build the document tree & add all the children of the root node.
    for(int i = 0; i < detail::SVG_PLOT_DOC_CHILDREN; ++i)
    {
      image.g();
    }
    set_ids();

    // Set other SVG color, stroke & width defaults for various child PLOT nodes.
    image.g(PLOT_BACKGROUND).style().stroke_color(image_border_.stroke_);
    image.g(PLOT_BACKGROUND).style().stroke_width(image_border_.width_); //
    image.g(PLOT_BACKGROUND).style().fill_color(image_border_.fill_);
    image.g(PLOT_WINDOW_BACKGROUND).style().fill_color(plot_window_border_.fill_);
    image.g(PLOT_WINDOW_BACKGROUND).style().stroke_width(plot_window_border_.width_).stroke_color(plot_window_border_.stroke_);
    image.g(PLOT_LIMIT_POINTS).style().stroke_color(lightslategray).fill_color(antiquewhite);
    image.g(PLOT_X_AXIS).style().stroke_color(black).stroke_width(x_axis_.width());
    image.g(PLOT_DATA_UNC3).style().stroke_color(lightgoldenrodyellow).fill_color(lightgoldenrodyellow).stroke_width(1);
    image.g(PLOT_DATA_UNC2).style().stroke_color(peachpuff).fill_color(peachpuff).stroke_width(1);
    image.g(PLOT_DATA_UNC1).style().stroke_color(magenta).fill_color(pink).stroke_width(1);

    // Note that widths are stored in member data *and* copied here.
    // Not sure if this is wise but ...
    // Font info defaults are set by the constructor above.
    // Ticks.

    y_ticks_.left_ticks_on_ = false; // Needed to ensure don't extend X-axis line.
    y_ticks_.right_ticks_on_ = false;

    if(x_ticks_.use_up_ticks() || x_ticks_.use_down_ticks())
    {
      image.g(PLOT_X_MAJOR_TICKS).style().stroke_width(x_ticks_.major_tick_width_).stroke_color(black);
      image.g(PLOT_X_MINOR_TICKS).style().stroke_width(x_ticks_.minor_tick_width_).stroke_color(black);
    }
    // Grids.
    // Default color & width for grid, used or not.
    image.g(PLOT_X_MAJOR_GRID).style().stroke_width(x_ticks_.major_grid_width_).stroke_color(svg_color(200, 220, 255));
    image.g(PLOT_X_MINOR_GRID).style().stroke_width(x_ticks_.minor_grid_width_).stroke_color(svg_color(200, 220, 255));
    //image.g(PLOT_DATA_LINES).style().stroke_width(2); // default width.
    // Alter with, for example: plot.data_lines_width(4);

    legend_place_ = (plot_window_on_) ? outside_right : inside; // Defaults.
    // Note if set plot_window_on() then also need to set legend_place.
    // (if set a default in plot_window then call to set legend place must to come *after* it.
    // So not set a default?  But leaving it as inside is worse?)

    x_ticks_on_ = x_ticks_.up_ticks_on_ || x_ticks_.down_ticks_on_;
    // Only 2D has left and right y ticks.
    x_ticks_.ticks_on_window_or_axis_ = 0; // Make ticks (and value labels) on axis the default.
  } // svg_1d_plot() Default constructor.

  void set_ids()
  { //! Document ids for use in identifying group elements, for example: <g id = "PLOT_TITLE".../g>
    for(int i = 0; i < detail::SVG_PLOT_DOC_CHILDREN; ++i)
    {
      image.g(i).id(detail::document_ids_[i]);
    }
  } //  void set_ids()

  void calculate_plot_window()
  { //! Calculate the size and position of the plot window,
    //! taking account of the length and font size of axes labels, axis ticks, title and legend box.
    //! This version is only for 1-D.
    //! All calculations use SVG units, pixels by default.

    // Start by assuming we can use all the svg image,
    // but reduce by the width of any image border.
    plot_left_ = 0 + image_border_width(); // Top left of image.
    plot_top_ = 0 + image_border_width();
    plot_right_ = image.x_size() - image_border_width(); // Bottom right of image.
    plot_bottom_ = image.y_size() - image_border_width();

    if(title_on_)
    { // Leave space at top for title.
      // TODO what if want to put title at bottom?
      plot_top_ += title_font_size() * (text_margin_ + 0.5);
    }

    // Assume that X-axis labels are always at bottom for 1D plot.
    if(x_axis_.label_on_)
    { // Leave space at bottom for X axis label.
       plot_bottom_ -= x_axis_label_style_.font_size() * text_margin_;
    }
    if(plot_window_on_)
    { // Needed to allow any plot window border rectangle to show OK.
      // A small margin is to prevent it overlapping the image border.
      // Also allows for axis value labels that mark the min and max
      // that must extend half a font width beyond the plot window border.
      plot_left_ +=  image_border_.margin_;
      plot_right_ -=  image_border_.margin_;
      plot_top_ += image_border_.margin_;
      plot_bottom_ -=  image_border_.margin_;
    }

    size_legend_box(); // depending on its contents.
    place_legend_box();

    // Because there may be several datasets,
    // and the scaling can be done by any one or all of them.
    // my_plot.autoscale(my_data1) // for 1 dataset.
    // or my_plot.autoscale(my_datas) // for a vector of several datasets.
    // calculates the min & max, increments & ticks.
    if (x_autoscale_)
    { // Use calculated autoscale values.
      // Autoscale has been done in my_data.autoscale(my_data);
      // and saved in x_auto_min_value_, x_auto_max_value_, & x_auto_tick_interval_,
      // so copy these values to use them:
      x_axis_.min_ = x_auto_min_value_;
      x_axis_.max_ = x_auto_max_value_;
      x_ticks_.major_interval_  = x_auto_tick_interval_;
      // else ignore auto values, even if have been calculated.
    }
    // Copy min & max to ticks.
    x_ticks_.min_ = x_axis_.min_;
    x_ticks_.max_ = x_axis_.max_;

    x_axis_.axis_ = (plot_bottom_ + plot_top_) / 2.; // Put X-axis halfway up plot window.
    // Ensure both axis and ticks have the *same* range.
    // (To use the separation, made to give the potential for different ranges,
    // one would have to *not* do this,
    // but to make sure they are both assigned correctly).

    if(plot_window_on_) // 
    {
      // Calculate the number of chars of the longest value label.
      x_ticks_.longest_label(); // Updates label_max_length_
      x_ticks_.label_max_space_ = 0; // Work out the longest value label for X-Axis.
      if (x_ticks_.label_rotation_ == horizontal)
      { // Only 1 char height & 1 space needed if labels are horizontal.
        x_ticks_.label_max_space_ = 2 * x_value_label_style_.font_size() * wh; // SVG chars
      }
      else if ((x_ticks_.label_rotation_ == upward) || (x_ticks_.label_rotation_ == downward))
      { // ! horizontal so will need more than 2 chars worth.
          x_ticks_.label_max_space_+= x_ticks_.label_max_length_ * x_value_label_style_.font_size() * wh; // SVG chars.
      }
      else
      { // Assume label is sloping, say 45, so * sin(45) = 0.707.
          x_ticks_.label_max_space_+= x_ticks_.label_max_length_ * x_value_label_style_.font_size() * wh * sin45; // SVG 'chars'.
      }

      if (x_ticks_.major_value_labels_side_ != 0)
      { // Some value labels.
        if ((x_ticks_.ticks_on_window_or_axis_ < 0) // on bottom of plot window.
           && (x_ticks_.major_value_labels_side_ < 0) ) // & labels on bottom.
        {  // Contract plot window bottom edge up to make space for x value labels on bottom.
          plot_bottom_ -= x_ticks_.label_max_space_; // Move up.
        }
        else if ((x_ticks_.ticks_on_window_or_axis_ > 0) //
           && (x_ticks_.major_value_labels_side_ > 0) ) // & x labels to top.
        { // Move top of plot window down to give space for x value labels.
          plot_top_ += x_ticks_.label_max_space_; // Move down.
        }
        else
        { // no labels on plot window (may be on mid-plot X-axis).
        }
      } // x_ticks_. major_value_labels_side

      // Make space for any ticks.
      if(x_ticks_.down_ticks_on_)
      { // Start bottom of plot higher to give space for any external down ticks.
        plot_bottom_ -= (std::max)(x_ticks_.major_tick_length_, x_ticks_.minor_tick_length_);// Avoid macro max trap!
      }
      if (x_axis_.axis_line_on_)
      { // Want a X-axis line, so check if range includes zero, so axes intersect,
        // and x_axis_ is svg coordinate of Y-axis (usually y = 0).
        // If not fix axis to bottom of the plot window.
        if ((x_axis_position_ == bottom) // All definitely > zero.
          && !(x_ticks_.ticks_on_window_or_axis_ < 0) ) // & not already at bottom.
        { // y_min_ > 0 so X-axis will not intersect Y-axis, so use plot window.
          plot_bottom_ -= x_ticks_.label_max_space_; // Move up for the value labels.
          x_axis_.axis_ = plot_bottom_; // Put X-axis on bottom.
        }
        else if ((x_axis_position_ == top)  // definitely < zero.
          && !(x_ticks_.ticks_on_window_or_axis_ > 0) ) // & not already at top.
        { // // y_max_ < 0 so X-axis will not intersect Y-axis, so use plot window.
           plot_top_ += x_ticks_.label_max_space_; // Move down for labels.
           x_axis_.axis_ = plot_top_; // Put X-axis on top.
        }
        else
        { // y_axis_position_ == y_intersects_x
          // Calculate below after transform is calculated.
        }
      } // if (use_x_axis_line_)

    } // plot_window_on_

    if(plot_window_on_)
    { // Draw plot window rectangle box.
      image.g(detail::PLOT_WINDOW_BACKGROUND).push_back(
        new rect_element(plot_left_, plot_top_, (plot_right_ - plot_left_), plot_bottom_ - plot_top_));
    } // plot_window_on_
  } //  void calculate_plot_window()

  void calculate_transform()
  { //! Calculate scale and shift factors for transform from Cartesian to plot.
    //! SVG image is (0, 0) at top left, Cartesian (0, 0) at bottom left.
    x_scale_ = (plot_right_ - plot_left_) / (x_axis_.max_ - x_axis_.min_);
    x_shift_ = plot_left_ - (x_axis_.min_ * (plot_right_ - plot_left_) / (x_axis_.max_ - x_axis_.min_));
    y_scale_ = 1.;
    y_shift_ = plot_top_ - (plot_top_ - plot_bottom_) / 2.;
  } // void calculate_transform()

  void draw_axes()
  { // Add information to the plot image for X axis lines.
    // For 1-D, there is, of course, only the horizontal X-axis!
    double x(0.);
    double y1(0.);
    double y2(image.y_size());
    transform_x(x);
    // Draw origin, making sure it is in the plot window.
    if(x_axis_.axis_line_on_ && (x >= plot_left_) && (x <= plot_right_))
    {
      if(!plot_window_on_)
      { // Use whole image.
        if(title_on_)
        { // Allow space for title, taking account of font size.
          y1 += title_info_.textstyle().font_size() * text_margin_;
        }
        if(x_axis_.label_on_)
        {// Allow space for x tick values, taking account of font size.
          y2 -= x_label_info_.textstyle().font_size() * text_margin_;
        }
      }
      else
      { // Use plot window.
        y1 = plot_top_;
        y2 = plot_bottom_;
      }
      image.g(detail::PLOT_X_AXIS).line(x, y1, x, y2);
    }
    draw_x_axis();
  } //  draw_axes()

  void update_image()
  { //! Calls functions to add all plot information to the image, including
    //plot window, axes, ticks, labels, grids, legend, and finally all the data series.
    clear_all(); // Removes all elements that will show up in a subsequent draw.

    // Draw plot background.
    image.g(detail::PLOT_BACKGROUND).push_back(
      new rect_element(0, 0, image.x_size(),  image.y_size()));

    calculate_plot_window();
    calculate_transform();
    draw_title(); // Call after above to the plot_x and y are defined.
    if(x_axis_.axis_line_on_)
    {
      draw_axes();
    }
    if(legend_on_)
    {
      draw_legend();
    }
    if(x_axis_.label_on_)
    {
      draw_x_label();
    }
    double y(0.); // All 1-D points are plotted are on the horizontal X axis (y = 0) axis.
    transform_y(y);
    if ((y < plot_top_) || (y > plot_bottom_))
    { // Should never happen!
      throw std::runtime_error("transform_y(y=0) outside plot window!");
    }

    for(unsigned int i = 0; i < serieses_.size(); ++i)
    { // Plot the data points for each of the data series.
      g_element& g_ptr = image.g(detail::PLOT_DATA_POINTS).g();
      g_ptr.style().stroke_color(serieses_[i].point_style_.stroke_color_);
      g_ptr.style().fill_color(serieses_[i].point_style_.fill_color_);

      for(unsigned int j = 0; j < serieses_[i].series_.size(); ++j)
      { // Draw jth point for ith serieses.
        unc ux = serieses_[i].series_[j];
        double x = ux.value();
        // TODO symbols are offset downwards because 
        // the origin of the point is the top left of the glyph.
        // Need to offset by the height and width of the font size?
        transform_x(x);
        if((x >= plot_left_) && (x <= plot_right_)) // Check point is inside plot_window.
        // May need a margin here to avoid points just over the window not being shown.
        {
          draw_plot_point(x, y, g_ptr, serieses_[i].point_style_, ux, unc(0)); // Marker. (y uncertainty is zero)
          if (x_values_on_)
          { // Show the value (& perhaps uncertainty) of the data point too.
            g_element& g_ptr_v = image.g(detail::PLOT_X_POINT_VALUES).g();
            draw_plot_point_value(x, y, g_ptr_v, x_values_style_, serieses_[i].point_style_, ux);
            // TODO separate X and Y colors?
          }
          else
          { // don't plot anything?  Might leave a marker to show an 'off the scale' value?
          }
        } // if in window
      } // for j
    } // for i all normal

    // Draw all the 'bad' or at_limit points.
    for(unsigned int i = 0; i < serieses_.size(); ++i)
    {
      g_element& g_ptr = image.g(detail::PLOT_LIMIT_POINTS);

      for (unsigned int j = 0; j != serieses_[i].series_limits_.size(); ++j)
      {
        double x = serieses_[i].series_limits_[j];
        if (limit_NaN(x))
        { // is NaN rather than too big or too small.
          double x = 0.;
          transform_x(x);
          // If include zero, OK, else plot on left or right as appropriate.
          if (x < plot_left_)
          { 
            x = plot_left_;
          }
          else if (x > plot_right_)
          { 
            x = plot_right_;
          }
          //else X axis includes zero, so x is OK.
          draw_plot_point(x, y, g_ptr, serieses_[i].limit_point_style_, 0, 0);
        }
        else
        { // Not NaN
          transform_x(x);
          if (x < plot_left_)
          {
            x = plot_left_;
          }
          else if (x > plot_right_)
          {
            x = plot_right_;
          }
          // else is inside plot window, so draw a limit point marker.
          // draw_plot_point(x, y, g_ptr, plot_point_style(lightgray, whitesmoke, s, cone)); default.
          draw_plot_point(x, y, g_ptr, serieses_[i].limit_point_style_, 0, 0);
        }
      } // for j
    } // for i limits point


  } //   void update_image()

  // ------------------------------------------------------------------------
  // write() has two versions: to an ostream and to a file.
  // The stream version first clears all unnecessary data from the graph,
  // builds the document tree, and then calls the write function for the root
  // document node, which calls all other nodes through the Visitor pattern.
  // The file version opens an ostream, and calls the stream version.
  // ------------------------------------------------------------------------
  svg_1d_plot& write(const std::string& file);
  svg_1d_plot& write(std::ostream& s_out);

  // Definitions of several versions of plot function to add data series.
  template <class T>
  svg_1d_plot_series& plot(const T& container, const std::string& title = "");
  template <class T>
  svg_1d_plot_series& plot(const T& begin, const T& end, const std::string& title = "");
  template <class T, class U>
  svg_1d_plot_series& plot(const T& begin, const T& end, const std::string& title = "", U functor = double_1d_convert);
  template <class T, class U>
  svg_1d_plot_series& plot(const T& container, const std::string& title = "", U functor = double_1d_convert);
}; // class svg_1d_plot 

// svg_1d_plot Member functions definitions.

  svg_1d_plot& svg_1d_plot::write(const std::string& file)
  { //! Write SVG image to the specified file.
    /** Write SVG image to the specified file, providing the suffix .svg if none given.
    This function uses the write to ostream function.
    */
    std::string filename(file); // Copy to avoid problems with const if need to append.
    if (filename.find(".svg") == std::string::npos)
    { // No file type suffix, so provide the default .svg.
      filename.append(".svg");
    }
    std::ofstream fout(filename.c_str());
    if(fout.fail())
    {
      throw std::runtime_error("Failed to open " + filename);
    }
    image.image_filename(filename);
    // Note filename for optional output as comment in the .svg file.
    svg_1d_plot::write(fout); // Use the ostream version.
    return *this;
  }

  svg_1d_plot& svg_1d_plot::write(std::ostream& s_out)
  { /*! Write SVG image to the specified std::ostream.
       This function also is used by the write to file function.
    */
    update_image();
    /*!
      Default stream precision 6 decimal digits is probably excessive.
      4.1 Basic data types, integer or float in decimal or scientific (using e format).
      - probably enough if image size is under 1000 x 1000.
      Reduces .svg file sizes significantly for curves represented with many data points.
      For example, if a curve is shown using 100 points,
      reducing to coord_precision(3) from default of 6 will reduce file size by 300 bytes.
    */
    image.write(s_out);
    return (svg_1d_plot&)*this;
  } // write

  template <class T>
  svg_1d_plot_series& svg_1d_plot::plot(const T& container, const std::string& title /*= "" */)
  { /*! Add a data series to the plot (by default, converting to unc doubles), with optional title.
     Note that this version assumes that *ALL* the data values in the container are used.
    */
    serieses_.push_back(
      svg_1d_plot_series(
      boost::make_transform_iterator(container.begin(), detail::unc_1d_convert()),
      boost::make_transform_iterator(container.end(), detail::unc_1d_convert()),
      title)
    );
   /*
     \code
std::vector<float> my_data; // my container.
my_data.pushback(2.f); // Fill container with some data.
my_data.pushback(3.f);
my_data.pushback(4.f);
my_1d_plot.plot(my_data, "All my container"); // Plot all data.
     \endcode
   */
    return serieses_[serieses_.size() - 1];
    //! \return Reference to data series just added.
  } // plot(const T& container, const std::string& title)

  template <class T>
  svg_1d_plot_series& svg_1d_plot::plot(const T& begin, const T& end, const std::string& title)
  { //! Add a data series to the plot (by default, converting to unc doubles), with optional title.
    /*!
    Note that this version permits a partial range, begin to end, of the container to be used.
    \return a reference to data series just added.
    */
    serieses_.push_back(
      svg_1d_plot_series(
      boost::make_transform_iterator(begin, detail::unc_1d_convert()),
      boost::make_transform_iterator(end, detail::unc_1d_convert()),
      title)
    );
    //! For example:  my_1d_plot.plot(my_data.begin(), my_data.end(), "My container");
    //! my_1d_plot.plot(&my_data[1], &my_data[4], "my_data 1 to 4"); // Add part of data series.
    //! Care: last == end  which is one past the last, so this only does 1, 2 & 3 - *not* 4!
    return serieses_[serieses_.size() - 1]; // Reference to data series just added.
  } // plot

  template <class T, class U>
  svg_1d_plot_series& svg_1d_plot::plot(const T& begin, const T& end, const std::string& title /* = ""*/, U functor /* = double_1d_convert */)
  { /*! Add a data series to the plot, with optional title. (Version with custom functor, rather than to double).
      Note that this version permits a partial range, begin to end, of the container to be used.
      \return a reference to data series just added.
    */
    serieses_.push_back(
      svg_1d_plot_series(
        boost::make_transform_iterator(container.begin(), functor),
        boost::make_transform_iterator(container.end(),   functor),
        title)
    );
    return serieses_[serieses_.size() - 1]; // Reference to data series just added.
  } // plot

  template <class T, class U>
  svg_1d_plot_series& svg_1d_plot::plot(const T& container, const std::string& title /* = "" */, U functor/*= double_1d_convert*/)
  { //! Add a data series to the plot, with optional title.
  /*!
    This version of plot includes a functor, allowing other than just convert data values to double(the default).
    \return a reference to data series just added.
  */
    serieses_.push_back(
      svg_1d_plot_series(
      boost::make_transform_iterator(container.begin(), functor),
      boost::make_transform_iterator(container.end(),   functor),
      title)
    );
    return serieses_[serieses_.size() - 1]; // Reference to data series just added.
  } // plot

// End svg_1d_plot Member functions definitions.

#if defined (BOOST_MSVC)
#  pragma warning(pop)
#endif

} // namespace svg
} // namespace boost

#endif // BOOST_SVG_SVG_1D_PLOT_HPP
