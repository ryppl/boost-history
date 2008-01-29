// svg_1d_plot.hpp

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2007

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
// -----------------------------------------------------------------

#ifndef BOOST_SVG_SVG_1D_PLOT_HPP
#define BOOST_SVG_SVG_1D_PLOT_HPP

#if defined (BOOST_MSVC)
#  pragma warning (push)
#  pragma warning (disable: 4512) // "assignment operator could not be generated."
#  pragma warning (disable: 4180) // qualifier applied to function type has no meaning; ignored
#endif

#include <boost/iterator/transform_iterator.hpp>

#include "svg.hpp"
#include "svg_style.hpp"
#include "detail/axis_plot_frame.hpp"
#include "detail/functors.hpp"

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
  const std::string strip_e0s(std::string s); // Strip unncessary zeros and e and sign.
  double string_svg_length(const std::string& s, const text_style& style);

  class svg_1d_plot; // Plot.
  class svg_1d_plot_series; // plot data series.
  // see axis_plot_frame.hpp

//  static const double wh = 0.7; // font text width/height ratio.
  // Even after reading http://www.w3.org/TR/SVG/fonts.html, unclear how to
  // determine the exact width of digits, so an 
  // arbitrary average width height ratio wh = 0.7 is used as a good approximation.

//  static const double sin45 = 0.707; // Use if axis value labels are sloping.

  // x_axis_position_ and y_axis_position_  use these.
//  enum x_axis_intersect {bottom = -1, x_intersects_y = 0, top = +1};
  // bottom = X-axis free below bottom of end of Y-axis (case of all Y definitely < 0).
  // top = X-axis free above top of X-axis (case of all Y definitely > 0).
  // x_intersects_y when Y values include zero, so X intersects the Y axis.

//  enum y_axis_intersect {left = -1, y_intersects_x = 0, right = +1};
  // left = Y-axis free to left of end of X-axis (case of all X definitely < 0).
  // right = Y-axis free to left of end of X-axis (case of all X definitely > 0).
  // y_intersects_x when X values include zero, so intersects the X axis.

 // -----------------------------------------------------------------
// This allows us to store plot state locally in svg_plot. We don't
// store it in "svg" because transforming the points after they are
// written to the document would be difficult. We store the Cartesian
// coordinates locally and transform them before we write them.
// -----------------------------------------------------------------
class svg_1d_plot_series
{
public:
  std::vector<double> series; // Normal 'OK to plot' data values.
  std::vector<double> series_limits; // 'limit' values: too big, too small or NaN.

  std::string title_; // title of data series (to show on legend).
  plot_point_style point_style_; // circle, square...
  plot_point_style limit_point_style_;
  plot_line_style line_style_; // No line style for 1-D, only for 2-D.

  // -------------------------------------------------------------
  // Scan each data point between the iterators that are passed,
  // sorting them into the correct std::vector, normal or not.
  // -------------------------------------------------------------
  template <class T> // T an STL container: array, vector ...
  svg_1d_plot_series(T begin, T end, const std::string& title) //, const plot_point_style& style = )
    :
  title_(title),
  point_style_(black, blank, 10, round), // Default point style. TODO vertical line better?
  // plot_line_style(const svg_color& col = black, const svg_color& acol = true, bool on = true, bool bezier_on = false)
  limit_point_style_(grey, blank, 10, cone), // Default limit (inf or NaN) point style.
  line_style_(black, true, 2, true, false) // Default line style, no fill.
  // Meaning of line style for 1-D as yet undefined?
  {
    for(T i = begin; i != end; ++i)
    {  // No defaults for begin and end.
      double temp = *i;
      if(detail::is_limit(temp))
      {
        series_limits.push_back(temp); // 'limit' values: too big, too small or NaN.
      }
      else
      {
        series.push_back(temp); // Normal 'OK to plot' data values.
      }
    }
  } // svg_plot_series

  // Set functions for the plot series.
  svg_1d_plot_series& fill_color(const svg_color& col_)
  {
    point_style_.fill_color_ = col_;
    return *this;
  }

  svg_1d_plot_series& stroke_color(const svg_color& col_)
  {
    point_style_.stroke_color_ = col_;
    return *this;
  }

  svg_1d_plot_series& shape(point_shape shape_)
  {
    point_style_.shape_ = shape_;
    return *this;
  }

  svg_1d_plot_series& size(int size_)
  {
    point_style_.size_ = size_;
    return *this;
  }

  svg_1d_plot_series& line_color(const svg_color& col_)
  {
    line_style_.color_ = col_;
    return *this;
  }

  svg_1d_plot_series& line_width(double wid_)
  { // Doesn't have desired effect yet.
    line_style_.width_ = wid_;
    return *this;
  }

  // Get functions for the plot series.
  double line_width()
  {
    return line_style_.width_;
  }

  svg_1d_plot_series& line_on(bool on_)
  {
    line_style_.line_on_ = on_;
    return *this;
  }

  svg_1d_plot_series& bezier_on(bool on_)
  {
    line_style_.bezier_on_ = on_;
    return *this;
  }

  bool bezier_on()
  {
    return line_style_.bezier_on_;
  }
}; //     struct svg_plot_series()

class svg_1d_plot : public detail::axis_plot_frame<svg_1d_plot>
{ // See also svg_2d_plot.hpp for 2-D version.
  friend void show_plot_settings(svg_1d_plot&);
  friend class detail::axis_plot_frame<svg_1d_plot>;

 protected:
  // Where we will be storing the data points for transformation.
  std::vector<svg_1d_plot_series> series;
  // These are sorted into two vectors for normal and abnormal (max, inf and NaN).

  svg image; // Stored so as to avoid rewriting style information constantly.

  // Member data names conventionally end with _.

  double x_scale_; // Used for transform from Cartesian to SVG coordinates.
  double x_shift_; // SVG origin is top left, Cartesian is bottom right.
  double y_scale_;
  double y_shift_;

  double text_margin_; // Marginal space around text items like title,
  // text_margin_ * font_size to get distance in svg units

  text_style a_style_; // Defaults.
  // text_style contains font size & type etc.
  text_style title_style_;
  text_style legend_style_;
  text_style x_axis_label_style_;
  text_style y_axis_label_style_;
  text_style x_value_label_style_;
  text_style y_value_label_style_; // Not used?
  text_style point_symbols_style_; // Used for data point marking.

  text_element title_info_; // Title of whole plot.
  text_element legend_header_; // legend box header or title (if any).
  text_element x_label_info_; // Example: "length of widget"
  text_element x_label_value_; // For example: "1.2" or "1.2e+001"
  text_element x_units_info_; // For example, to display, "length (meter)"
  // No Y-axis info for 1D.
  // Note that text_elements hold font_size, bold, italic...

  // Border information for the plot window (not the full image size).
  box_style image_border_; // rectangular border of all image width, color...
  box_style plot_window_border_; // rectangular border of plot window width, color...

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
  bool outside_legend_on_; // legend box outside the plot window.
  bool legend_lines_; // get/set by legend_lines(bool); data colored line type in legend box.
  bool plot_window_on_; // Use a separate plot window (not whole image).
  bool x_ticks_on_; // TODO check these are really useful.
  int x_axis_position_; // TODO should these be in axis_style?
  std::string plot_window_clip_; // = "clip_plot_window" id for clippath
  // http://www.w3.org/TR/SVG/masking.html#ClipPathElement 14.1 Introduction
  // clipping paths, which uses any combination of 'path', 'text' and basic shapes
  // to serve as the outline where everything on the "inside" of the outline
  // is allowed to show through but everything on the outside is masked out.
  // So the plot_window_clip_ limits display to a plot_window rectangle.

  // Public member functions, defined below.
  // void calculate_transform(); //
  // void draw_axes();
  // void calculate_plot_window(); // plot_x, plot_y...
  // void update_image();
  // void set_ids();

public:
  svg_1d_plot()
    : // Default constructor.
    // Many ( but not all - see below) default values here.
    text_margin_(2.), // for text was 1.5 // as a multiplier of the font size.

    title_style_(16, "Verdana", "", ""),  // last "bold" ?
    legend_style_(14, "Verdana", "", ""), // 2nd "italic"?
    x_axis_label_style_(14, "Verdana", "", ""),
    x_value_label_style_(12, "Verdana", "", ""),
    point_symbols_style_(12, "Lucida Sans Unicode"), // Used for data point marking.

    title_info_(0, 0, "Plot of data", title_style_, center_align, horizontal),
    x_label_info_(0, 0, "X Axis", x_axis_label_style_, center_align, horizontal),
    x_units_info_(0, 0, " (units)", x_value_label_style_, center_align, horizontal),
    x_label_value_(0, 0, "", x_value_label_style_, center_align, horizontal),
    x_axis_(X, -10., +10., black, 1, 0, true, false, true),
    y_axis_(Y, 0., +1., black, 1, 0, false, false, false),

    // Might fill in all values, but there are rather many for ticks_labels_style,
    x_ticks_(X),// so for defaults see ticks_labels_style.
    image_border_(yellow, white, 2, 10, true, true), // margin should be about axis label font size.
    plot_window_border_(yellow, svg_color(255, 255, 255), 2, 3, true, false),
    legend_header_(0, 0, "", legend_style_, center_align, horizontal),
    legend_width_(200), // width of legend box (pixels) // TODO isn't this calculated?
    legend_height_(0), // height of legend box (pixels)
    legend_left_(-1), legend_right_(-1),legend_top_(-1),legend_bottom_(-1), // Default top left of plot window.
    legend_place_(outside_right), // default but interacts with using plot_window.
    legend_on_(false),
    legend_longest_(0),
    outside_legend_on_(true),
    plot_window_clip_("plot_window"), // for <clipPath id="plot_window" ...
    title_on_(true),
    plot_window_on_(true),
    // Used to transform Cartesian to SVG.
    x_scale_(1.), x_shift_(0.),
    y_scale_(1.), y_shift_(0.),
    x_axis_position_(0) // Move into axis_style? 

  // See documentation for default settings rationale.
  {
    image_size(500, 200); // Default image size.
    // Only need to be quite shallow (y_size) for a 1-D plot.
    // 200 barely leaves enough room for five data series in the legend).
    // (2-D usually needs to be much more rectangular).


    using namespace boost::svg::detail; // avoid detail::

    // Build the document tree & add all the children of the root node.
    for(int i = 0; i < detail::SVG_PLOT_DOC_CHILDREN; ++i)
    {
      image.g();
    }

    // Set other SVG color, stroke & width defaults for various child PLOT nodes.
    image.g(PLOT_BACKGROUND).style().fill_color(image_border_.fill_);
    image.g(PLOT_BACKGROUND).style().stroke_color(image_border_.stroke_);
    image.g(PLOT_BACKGROUND).style().stroke_width(image_border_.width_); //
    image.g(PLOT_WINDOW_BACKGROUND).style().fill_color(plot_window_border_.fill_);
    image.g(PLOT_WINDOW_BACKGROUND).style().stroke_width(plot_window_border_.width_).stroke_color(plot_window_border_.stroke_);
    image.g(PLOT_LIMIT_POINTS).style().stroke_color(lightslategray).fill_color(antiquewhite);
    image.g(PLOT_X_AXIS).style().stroke_color(black).stroke_width(x_axis_.width());
    image.g(PLOT_X_LABEL).style().fill_color(black);
    image.g(PLOT_VALUE_LABELS).style().fill_color(black);
    image.g(PLOT_LEGEND_TEXT).style().fill_color(black);
    image.g(PLOT_TITLE).style().fill_color(black).stroke_on(false);

    // Note that widths are stored in member data *and* copied here.
    // Not sure if this is wise but ...
    // Font info defaults are set by the constructor above.
    // Ticks
    if(x_ticks_.use_up_ticks() || x_ticks_.use_down_ticks())
    {
      image.g(PLOT_X_MAJOR_TICKS).style().stroke_width(x_ticks_.major_tick_width_).stroke_color(black);
      image.g(PLOT_X_MINOR_TICKS).style().stroke_width(x_ticks_.minor_tick_width_).stroke_color(black);
    }
    // Grids.
    // Default color & width for grid, used or not.
    image.g(PLOT_X_MAJOR_GRID).style().stroke_width(x_ticks_.major_grid_width_).stroke_color(svg_color(200, 220, 255));
    BOOST_ASSERT(image.g(PLOT_X_MAJOR_GRID).style().stroke_color() == svg_color(200, 220, 255));
    image.g(PLOT_X_MINOR_GRID).style().stroke_width(x_ticks_.minor_grid_width_).stroke_color(svg_color(200, 220, 255));
    //image.g(PLOT_DATA_LINES).style().stroke_width(2); // default width.
    // Alter with plot.data_lines_width(4); 

    legend_place_ = (plot_window_on_) ? outside_right : inside; // Defaults.
    // Note if set plot_window_on() then also need to set legend_place.
    // (if set a default in plot_window then call to set legend place must to come *after* it.
    // So not set a default?  But leaving it as inside is worse?)

    x_ticks_on_ = x_ticks_.up_ticks_on_ || x_ticks_.down_ticks_on_;
    // Only 2D has left and right ticks.
    set_ids();
  } // svg_1d_plot() Default constructor.

    
  void calculate_plot_window()
  { // For 1-D
    // All calculation use svg units, pixels by default.

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

    size_legend_box(); // depending on its contents.
    place_legend_box();
    // Assume that axis labels are always at bottom and left.
    if(x_axis_.label_on_)
    { // Leave space at bottom for X axis label.
      plot_bottom_ -= x_axis_label_style_.font_size() * (text_margin_);
    }

    x_axis_.axis_ = (plot_bottom_ + plot_top_) / 2.; // Put X-axis halfway up plot window.

    if(plot_window_on_)
    { // Needed to allow any plot window border rectangle to show OK.
      // A small margin is to prevent it overlapping the image border.
      // Also allows for axis value labels that mark the min and max
      // that must extend half a font width beyond the plot window border.
      plot_left_ +=  image_border_.margin_;
      plot_right_ -=  image_border_.margin_;
      plot_top_ += image_border_.margin_;
      plot_bottom_ -=  image_border_.margin_;

      // Calculate the number of chars of the longest value label.
      x_ticks_.longest_label();
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
    {
      // Draw plot window rect.
      image.g(detail::PLOT_WINDOW_BACKGROUND).push_back(
        new rect_element(plot_left_, plot_top_, (plot_right_ - plot_left_), plot_bottom_ - plot_top_));
    } // plot_window_on_
  } //  void calculate_plot_window()


  //void calculate_transform()
  //{ // Calculate scale and shift factors for transform from Cartesian to plot.
  //  // SVG image is 0, 0 at top left, Cartesian at bottom left.
  //  x_scale_ = (plot_right_ - plot_left_) / (x_max_ - x_min_);
  //  x_shift_ = plot_left_ - (x_min_ * (plot_right_ - plot_left_) / (x_max_ - x_min_));
  //  y_scale_ = 1.;
  //  y_shift_ = plot_top_ - (plot_top_ - plot_bottom_) / 2.;
  //} // void calculate_transform()

  void draw_axes()
  { // For 1-D, there is, of course, only the X-axis!
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
          y1 += title_info_.style().font_size() * text_margin_;
        }
        if(x_axis_.label_on_)
        {// Allow space for x tick values, taking account of font size.
          y2 -= x_label_info_.style().font_size() * text_margin_;
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
  {
    // Removes all elements that will show up in a subsequent draw.
    clear_all();

    // Draw plot background.
    image.g(detail::PLOT_BACKGROUND).push_back(
      new rect_element(0, 0, image.x_size(),  image.y_size()));

    calculate_plot_window();
    //calculate_transform();
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
    double y(0.); // All 1-D points are plotted are on the y = 0 axis.
    transform_y(y);
    for(unsigned int i = 0; i < series.size(); ++i)
    { // For each of the data series.
      g_element& g_ptr = image.g(detail::PLOT_DATA_POINTS).g();

      g_ptr.style().stroke_color(series[i].point_style_.stroke_color_);
      g_ptr.style().fill_color(series[i].point_style_.fill_color_);

      for(unsigned int j = 0; j < series[i].series.size(); ++j)
      { // Draw points for jth series.
        double x = series[i].series[j];
        transform_x(x);
        if( // Check point is inside plot_window (otherwise ignore it).
             (x >= plot_left_)
          && (x <= plot_right_)
          && (y >= plot_top_)
          && (y <= plot_bottom_) )
        {
          draw_plot_point(x, y, g_ptr, series[i].point_style_);
        }
      } // for j
    } // for
  } //   void update_image()

  void set_ids()
  { // document ids for use in <g id = "PLOT_TITLE".../>
    for(int i = 0; i < detail::SVG_PLOT_DOC_CHILDREN; ++i)
    {
      image.g(i).id(detail::document_ids[i]);
    }
  } //  void set_ids()

  // ------------------------------------------------------------------------
  // write() has two flavors, a file and a ostream.
  // The file version opens an ostream, and calls the stream version.
  // The stream version first clears all unnecessary data from the graph,
  // builds the document tree, and then calls the write function for the root
  // document node, which calls all other nodes through the Visitor pattern.
  // ------------------------------------------------------------------------

  svg_1d_plot& write(std::string& file)
  {
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

  svg_1d_plot& write(std::ostream& s_out)
  {
    update_image();
    // Default stream precision 6 decimal digits is probably excessive.
    // 4.1 Basic data types, integer or float in decimal or scientific (using e format).
    // - probably enough if image size is under 1000 x 1000.
    // Reduces .svg file sizes significantly for curves represented with many data points.
    // For example, if a curve is shown using 100 points,
    // reducing to coord_precision(3) from default of 6 will reduce file size by 300 bytes.
    image.write(s_out);
    return (svg_1d_plot&)*this;
  }
  
  template <class T>
  svg_1d_plot_series& plot(const T& container, const std::string& title)
  {
    series.push_back(
      svg_1d_plot_series(
      boost::make_transform_iterator(container.begin(), detail::boost_default_convert()),
      boost::make_transform_iterator(container.end(), detail::boost_default_convert()),
      title)
    );
    return series[series.size()-1];
  }

  template <class T, class U>
  svg_1d_plot_series& plot(const T& container, const std::string& title, U functor)
  { // Version with functor.
    series.push_back(
      svg_1d_plot_series(
      boost::make_transform_iterator(container.begin(), functor),
      boost::make_transform_iterator(container.end(),   functor),
      title)
    );
    return series[series.size()-1];
  }

}; // end svg_1d_plot

#if defined (BOOST_MSVC)
#  pragma warning(pop)
#endif


} // namespace svg
} // namespace boost


#endif // BOOST_SVG_SVG_1D_PLOT_HPP
