// svg_2d_plot.hpp

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2007

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_SVG_2D_PLOT_HPP
#define BOOST_SVG_SVG_2D_PLOT_HPP

#define BOOST_PARAMETER_MAX_ARITY 12

#include <boost/bind.hpp>

#if defined (BOOST_MSVC)
#  pragma warning(push)
#  pragma warning(disable: 4512) // "assignment operator could not be generated."
#  pragma warning(disable: 4127) // "conditional expression is constant."
#  pragma warning(disable: 4100) // "'boost_parameter_enabler_argument' : unreferenced formal parameter"
#endif

#include <boost/parameter/preprocessor.hpp>
#include <boost/parameter/name.hpp>
#include <boost/iterator/transform_iterator.hpp>

#include "svg_style.hpp"
#include "detail/axis_plot_frame.hpp"
#include "detail/numeric_limits_handling.hpp"
#include "detail/functors.hpp"
#include "svg.hpp"

#if defined (BOOST_MSVC)
#  pragma warning(pop)
#endif

#include <map>
#include <string>
#include <sstream>
#include <utility>
#include <ostream>
#include <iterator>
#include <exception>
#include <iostream> // for debug

namespace boost
{
  namespace svg
  {

#ifndef BOOST_SVG_BOOST_PARAMETER_NAMES
#define BOOST_SVG_BOOST_PARAMETER_NAMES
  BOOST_PARAMETER_NAME(my_plot)
  BOOST_PARAMETER_NAME(container)
  BOOST_PARAMETER_NAME(title)
  BOOST_PARAMETER_NAME(stroke_color)
  BOOST_PARAMETER_NAME(fill_color)
  BOOST_PARAMETER_NAME(point_style)
  BOOST_PARAMETER_NAME(x_functor)
  BOOST_PARAMETER_NAME(size)
#endif
  // TODO Why are these outside the #endif? Subset?
  // Remove when class plot used instead of Boost.Parameter.
  BOOST_PARAMETER_NAME(bezier_on)
  BOOST_PARAMETER_NAME(line_on)
  BOOST_PARAMETER_NAME(line_color)
  BOOST_PARAMETER_NAME(area_fill_color)

  // -----------------------------------------------------------------
  // This allows us to store plot state locally in svg_plot. We don't
  // store it in "svg" because transforming the points after they are
  // written to the document would be difficult. We store the Cartesian
  // coordinates locally and transform them before we write them.
  // -----------------------------------------------------------------
struct svg_2d_plot_series;
class svg_2d_plot;

struct svg_2d_plot_series
{
  // 2-D Data series points to plot.
  std::multimap<double, double> series; // Normal 'OK to plot' data values.
  std::multimap<double, double> series_limits; // 'limit' values: too big or small, or NaN.
  // multimap is used rather than vector of pairs because
  // multimap sorts and ensures that lines joining data points
  // are unaffected by the order in which data is presented.
  // (For 1-D a vector of doubles can be used).

  std::string title;
  plot_point_style point_style;
  plot_line_style line_style;

  template <class T>
  svg_2d_plot_series(T begin, T end,
    std::string title, // of data series.
    const plot_point_style& point,
    const plot_line_style& line)
    :
  title(title),
    point_style(point),
    line_style(line)
  { // Constructor.
    for(T i = begin; i != end; ++i)
    { // Sort into normal and limited series.
      if(detail::pair_is_limit(*i))
      { // Either x and/or y is at limit.
        series_limits.insert(*i);
      }
      else
      { // normal point.
        series.insert(*i);
      }
    }
  } // svg_2d_plot_series
}; // struct svg_2d_plot_series

class svg_2d_plot : public detail::axis_plot_frame<svg_2d_plot>
{ // See also svg_1d_plot.hpp for 1-D version.

private:
  friend class detail::axis_plot_frame<svg_2d_plot>;
  // Contains functions common to 1 and 2-D.

  double x_scale; // Use by function transform()
  double x_shift; // to go from Cartesian to svg coordinates.
  double y_scale;
  double y_shift;

  // Stored so as to avoid rewriting style information constantly.
  svg image;

  text_element title_info; // Plot title.
  text_element legend_header_; // legend box header or title (if any).
  text_element x_label_info; // For example: "length"
  text_element x_label_value; // For example: "1.2" or "1.2e+001"
  text_element y_label_info; // For example: "volume"
  text_element x_units_info; // For example: "mm"
  text_element y_units_info; // 2-D only.
  text_element y_label_value; // For example: "1.2" or "1.2e+001"

  int x_value_precision_; // precision for tick value labels, usually 3 will suffice.
  int y_value_precision_; // precision for tick value labels, usually 3 will suffice.
  // Used to decide how much space to allow for tick values.
  int x_value_ioflags_;  // IO formatting flags for X-axis.
  int y_value_ioflags_;  // IO formatting flags for Y-axis.
  double text_margin_; // Marginal space around text items like title,
  // as a fraction of font size, (pixels) (was fixed at 1.5).
  double border_margin_; // Marginal (pixels) space around the plot border.
  double border_width_; // plot border rectangle width.

  // Border information for the plot window (<= image size).
  // Allows for title and legend to be separate.
  // Initially will be set to the width and height of the graph image.
  // calculate_plot_window() sets these values.
  double plot_x1; // TODO These (and many others) also could be float?
  double plot_x2;
  double plot_y1;
  double plot_y2;

  // X-Axis information.
  double x_min; // minimum x value (Cartesian units).
  double x_max; // maximum x value (Cartesian units).
  double x_axis; // // x-axis (y = 0) transformed into SVG coordinates.
  double x_major_interval_; // x_major_interval_ is the stride or interval for major x ticks.
  // (Cartesian units)  set/get by x_major_interval
  double x_minor_interval_; // Interval (Cartesian units) between minor ticks.
  // No set function because x_num_minor_ticks_ used to determine this instead,
  // but one could calculate x_minor_interval_.

  // text width is effectively the boldness of the font.
  // 0 is default, 1 is bolder, 2 very bold...
  // TODO SVG has also a not yet implemented boldness.
  double title_font_width_;
  double legend_font_width_;
  double legend_width_; // width of legend box in pixels.
  double legend_height_; // height of legend box in pixels.
  double legend_x1_; // Left of legend box. (Optionally set by legend_top_left).
  double legend_y1_; // Top of legend box.
  // Size of legend box is controlled by its contents,
  // but may be helpful to store bottom right coordinates.
  // legend_bottom_right() gives access.
  double legend_x2_; // right of legend box.
  double legend_y2_; // bottom of legend box.
  size_t legend_longest_; // longest string in legend box.
  double x_label_width_; // Used for
  double x_axis_width_; // X-axis horizontal line width. set/get by x_axis_width()
  double x_major_tick_length_; // pixels.
  double x_major_tick_width_; // pixels.
  double x_minor_tick_length_; // pixels.
  double x_minor_tick_width_; // pixels.
  // Avoid name clash with x_m*_tick_length and width.
  unsigned int x_num_minor_ticks_; // number of minor ticks, eg 4 gives major 0, minor 1,2,3,4, major 5
  double x_major_grid_width_; // pixels.
  double x_minor_grid_width_; // pixels.
  // grid width is set in style.stroke_width
  // unsigned int x_major_label; // pixels. Now in x_units_info font size

  // Y-Axis information.
  unsigned int y_num_minor_ticks_; // number of minor ticks, eg 4 gives major 0, minor 1,2,3,4, major 5
  double y_min; // minimum y value (Cartesian units).
  double y_max; // maximum y value (Cartesian units).
  double y_major_interval_; // y_major_interval_ is the stride or interval for major y ticks.
  double y_minor_interval_; // y_minor_interval_ is the stride or interval for minor y ticks.
  double y_label_width_; // TODO used for axis width too for now.  OK?
  double y_axis; // Y-axis (x = 0) transformed into SVG coordinates.
  double y_axis_width_; // Y-axis vertical line width.  set/get by y_axis_width()
  double y_major_tick_length_; // pixels.
  double y_major_tick_width_; // pixels.
  double y_minor_tick_length_; // pixels.
  double y_minor_tick_width_; // pixels.
  // Avoid name clash with x_m*_tick_length and width.
  // grid width is set in style.stroke_width
  double y_major_grid_width_; // pixels.
  double y_minor_grid_width_; // pixels.

  // Yes/no options.
  bool use_title; // Provide a title for the whole plot.
  bool use_legend; // Provide a legend box.
  bool use_axis; // Draw x and y axes.  TODO split so can have either or both.
  bool use_plot_window; // Use a separate plot window.
  bool use_x_label; // Label axis with text.
  bool use_y_label;
  bool use_x_major_labels; // values for major ticks.
  bool use_y_major_labels;
  bool use_x_label_units;
  bool use_y_label_units;
  int x_label_rotation_; // X_axis value labels written vertically up.
  int y_label_rotation_; // Y_axis value labels written vertically up.
  bool use_x_major_grid_;
  bool use_x_minor_grid_;
  bool use_y_major_grid_;
  bool use_y_minor_grid_;

  // Note: can have ticks both up and/or down, and/or left and right.
  bool use_up_ticks;
  bool use_down_ticks;
  bool use_x_ticks; // = use_up_ticks || use_down_ticks
  bool use_left_ticks;
  bool use_right_ticks;
 // bool use_y_ticks; // = use_left_ticks || use_right_ticks
  // was external style.
  bool use_x_ticks_on_plot_window_; // Value labels & ticks on the plot window rather than on X-axis.
  bool use_y_ticks_on_plot_window_; // Value labels & ticks on the plot window rather than on Y-axis.
  bool use_x_axis_lines_; // Draw a horizontal X-axis line.
  bool use_y_axis_lines_; // Draw a vertical Y-axis line.
  bool use_line; // get/set by line_on(bool); // Not really useful for 1-D,
  // TODO but needs Boost-Parameter removed to do properly.
  bool strip_e0s_;

  // Where we will be storing the data points (series) for transformation.
  std::vector<svg_2d_plot_series> series; // Defined above.

  std::string plot_window_clip; // = "clip_plot_window" id for clippath

  // http://www.w3.org/TR/SVG/masking.html#ClipPathElement 14.1 Introduction
  // clipping paths, which uses any combination of 'path', 'text' and basic shapes
  // to serve as the outline where everything on the "inside" of the outline
  // is allowed to show through but everything on the outside is masked out.
  // So the plot_window_clip limits display to a plot_window rectangle.

public: // of class svg_2d_plot: public detail::axis_plot_frame<svg_2d_plot>

  svg_2d_plot() // Constructor, including all the very many default plot options.
    :
    // TODO check that *all* options are initialized here.
    // See documentation for default settings rationale.

    title_info(0, 0, "Plot of data", 16, "Verdana", "", "", "", "", center_align, horizontal),
    title_font_width_(1),  // text width is effectively the boldness of the font.
    legend_width_(200), // width of legend box (pixels)
    legend_header_(0, 0, "", 14, "Verdana", "italic", "", "bold", "", center_align, horizontal),
    legend_font_width_(0.5),
    x_label_info(0, 0, "X Axis", 14, "Verdana", "", "bold", "", "", center_align, horizontal),
    x_units_info(0, 0, "(units)", 14, "Verdana", "", "", "", "", center_align, horizontal),
    x_label_value(0, 0, "", 12, "Verdana", "", "", "", "", center_align, horizontal),
    // TODO use this and provide way to set'n'get separately.
    x_label_width_(2),
    x_axis_width_(2),
    y_label_info(0, 0, "Y Axis", 14, "Verdana", "", "bold", "", "", center_align, upward),
    y_units_info(0, 0, "(units)", 14, "Verdana", "", "", "", "", center_align, upward),
    y_label_value(0, 0, "", 12, "Verdana", "", "", "", "", center_align, upward),
    y_label_width_(5),
    y_axis_width_(2), // vertical line.
    x_label_rotation_(horizontal),
    y_label_rotation_(horizontal),
    // Plot may look odd if font and size of label & units are different!
    // y_units_info(0, 0, "(units)", 11, "Times New Roman", "italic", "bold", "wider", "underline", right_align, upsidedown),
    // shows some values that might be useful if svg browsers are fully implemented
    // to support font modification to text characters.
    // italic and bold seem the only ones working at present.
    x_value_precision_(3), // precision for tick value labels, usually 3 will suffice.
    x_value_ioflags_(std::ios::dec), // Provides a way of controlling scientific format, etc.
    y_value_precision_(3), // precision for tick value labels, usually 3 will suffice.
    y_value_ioflags_(std::ios::dec), // Provides a way of controlling scientific format, etc.
    // Note that ALL the flags are set, overwriting any defaults, so std::dec is wise.
    text_margin_(1.5), // for axis label text, as a multiplier of the font size.
    border_margin_(10), // Prevent plot window box begin right on edge,
    border_width_(20), // width of any border to plot window and legend box.
    // less than half the font size of label value will result in clipping.
    // plot_x1, x2, y1, 2 are set in calculate_plot_window
    legend_x1_(-1), legend_x2_(-1),legend_y1_(-1),legend_y2_(-1), // -1 Indicates not yet set.
    x_min(-10), x_max(10), // so plots range from -10 to +10
    y_min(-10), y_max(10),
    x_major_interval_(2), // x stride between major ticks & value label.
    y_major_interval_(2), // y stride
    // Ticks on axes.
    x_num_minor_ticks_(4), // suits
    y_num_minor_ticks_(4), // suits: major 0, minor 2, 4, 6, 8, major 10
    x_major_tick_length_(10.), // If both up and down, total is twice this.
    x_minor_tick_length_(5.),
    x_major_tick_width_(2.),
    x_minor_tick_width_(1.),
    y_major_tick_length_(10.), // If both left and right, total is twice this.
    y_minor_tick_length_(5.),
    y_major_tick_width_(2.),
    y_minor_tick_width_(1.),
    // grids
    x_major_grid_width_(1.),
    x_minor_grid_width_(0.5),
    y_major_grid_width_(1.),
    y_minor_grid_width_(0.5),
    // Use by transform Cartesian to SVG.
    x_scale(1.), x_shift(0.),
    y_scale(1.), y_shift(0.),
    x_minor_interval_(0), // These are calculated from x & y_num_minor_ticks_
    y_minor_interval_(0), // but given a value here for safety.

    plot_window_clip("plot_window"), // for <clipPath id="plot_window" ...

    // Boolean options.
    strip_e0s_(false),
    use_down_ticks(true), // On X-axis.
    use_up_ticks(false), // external ticks only.
    //use_x_ticks(false), // = use_up_ticks || use_down_ticks
    use_left_ticks(true), // On y-axis.
    use_right_ticks(false), // On y-axis
    //use_y_ticks(true), // = use_left_ticks || use_right_ticks  TODO get rid of this. NOT useful
    use_x_ticks_on_plot_window_(false), // was external_style
    use_y_ticks_on_plot_window_(false), // was external_style
    use_x_label_units(false),
    use_y_label_units(false),
    use_x_major_labels(true), // tick value labels.
    use_y_major_labels(true),
    use_x_label(true), // "X-axis"
    use_y_label(true), // "Y-axis" - true for 2-D, false for 1-D.
    use_title(true),
    use_legend(false), use_axis(true),
    use_x_axis_lines_(true), // draw horizontal X-axis line.
    use_y_axis_lines_(true), // draw vertical Y-axis line.
    use_x_major_grid_(true),
    use_x_minor_grid_(false),
    use_y_major_grid_(true),
    use_y_minor_grid_(false),
    use_plot_window(true),
    use_line(true)
    // TODO but needs Boost-Parameter removed to do properly.

  {
    image_size(500, 400); // Default image size for 2-D.
    // 2-D usually needs to be squarer than 1-D.
    //use_x_ticks = use_up_ticks || use_down_ticks;
    //use_y_ticks = use_left_ticks || use_right_ticks;

    // Build the document tree by adding all children of the root node.
    for(int i = 0; i < detail::SVG_PLOT_DOC_CHILDREN; ++i)
    {
      image.add_g_element();
    }
    using namespace boost::svg::detail; // avoid detail::
    set_ids();

    // Set other SVG color, stroke & width defaults for various child PLOT nodes.

    // Tick length etc is now set above in the constructor, but width & color here.
    image.get_g_element(PLOT_BACKGROUND).style().fill_color(white);
    image.get_g_element(PLOT_BACKGROUND).style().stroke_color(linen); // TODO test only ?
    image.get_g_element(PLOT_BACKGROUND).style().stroke_width(0.5); //
    image.get_g_element(PLOT_WINDOW_BACKGROUND).style().fill_color(white);
    image.get_g_element(PLOT_WINDOW_BACKGROUND).style().stroke_width(1).stroke_color(lightyellow);
    image.get_g_element(PLOT_LIMIT_POINTS).style().stroke_color(lightslategray).fill_color(antiquewhite);
    image.get_g_element(PLOT_X_AXIS).style().stroke_color(red).stroke_width(x_axis_width_);
    image.get_g_element(PLOT_Y_AXIS).style().stroke_color(blue).stroke_width(y_axis_width_);
    image.get_g_element(PLOT_X_LABEL).style().fill_color(black);
    image.get_g_element(PLOT_Y_LABEL).style().fill_color(black);
    image.get_g_element(PLOT_VALUE_LABELS).style().fill_color(black); // text
    image.get_g_element(PLOT_LEGEND_TEXT).style().fill_color(blue);
    image.get_g_element(PLOT_TITLE).style().fill_color(black).stroke_on(false);
    // If stroke color is not set, then get a very 'thin' font and width has no effect.
    // Firefox text is rather thin compared to IE.

    // Note that widths are stored in member data *and* copied here.
    // Not sure if this is wise but ...

    // Ticks
    if(use_up_ticks || use_down_ticks)
    {
      image.get_g_element(PLOT_X_MAJOR_TICKS).style().stroke_width(x_major_tick_width_).stroke_color(black);
      image.get_g_element(PLOT_X_MINOR_TICKS).style().stroke_width(x_minor_tick_width_).stroke_color(black);
    }
    if(use_left_ticks || use_right_ticks)
    {
      image.get_g_element(PLOT_Y_MAJOR_TICKS).style().stroke_width(y_major_tick_width_).stroke_color(black);
      image.get_g_element(PLOT_Y_MINOR_TICKS).style().stroke_width(y_minor_tick_width_).stroke_color(black);
    }
    // Grids.
    //// Note: may not be needed here - they MUST be set at call time in case use_x_major_grid_ has changed.
    //if(use_x_major_grid_)
    //{ // pale blue.
    //  image.get_g_element(PLOT_X_MAJOR_GRID).style().stroke_width(x_major_grid_width_).stroke_color(svg_color(200, 220, 255));
    //}
    //if(use_x_minor_grid_)
    //{
    //  image.get_g_element(PLOT_X_MINOR_GRID).style().stroke_width(x_minor_grid_width_).stroke_color(svg_color(200, 220, 255));
    //}
    //if(use_y_major_grid_)
    //{
    //  image.get_g_element(PLOT_Y_MAJOR_GRID).style().stroke_width(y_major_grid_width_).stroke_color(svg_color(200, 220, 255));
    //}
    //if(use_y_minor_grid_)
    //{
    //  image.get_g_element(PLOT_Y_MINOR_GRID).style().stroke_width(y_minor_grid_width_).stroke_color(svg_color(200, 220, 255));
    //}

    // Default color for grid.
      image.get_g_element(PLOT_X_MAJOR_GRID).style().stroke_width(x_major_grid_width_).stroke_color(svg_color(200, 220, 255));
      image.get_g_element(PLOT_X_MINOR_GRID).style().stroke_width(x_minor_grid_width_).stroke_color(svg_color(200, 220, 255));
      image.get_g_element(PLOT_Y_MAJOR_GRID).style().stroke_width(y_major_grid_width_).stroke_color(svg_color(200, 220, 255));
      image.get_g_element(PLOT_Y_MINOR_GRID).style().stroke_width(y_minor_grid_width_).stroke_color(svg_color(200, 220, 255));
      image.get_g_element(PLOT_DATA_LINES).style().stroke_width(2);

  } // svg_2d_plot() default constructor.

private:
  // svg_2d_plot Member Functions.

  void set_ids()
  { // document ids for use in <g id = "PLOT_TITLE".../>
    for(int i = 0; i < detail::SVG_PLOT_DOC_CHILDREN; ++i)
    {
      image.get_g_element(i).id(detail::document_ids[i]);
    }
  } //  void set_ids()

  void calculate_transform()
  { // Calculate scale and shift factors for transform from Cartesian to plot.
    // SVG image is 0, 0 at top left, Cartesian at bottom left.
    x_scale = (plot_x2 - plot_x1) / (x_max - x_min);
    x_shift = plot_x1 - x_min *(plot_x2 - plot_x1) / (x_max - x_min);
    y_scale = -(plot_y2-plot_y1) / (y_max-y_min);
    y_shift = plot_y1 - (y_max * (plot_y1 - plot_y2) / (y_max - y_min));
  } //   void calculate_transform()

  void transform_pair(std::pair<double, double>& pt)
  { // Transform both x and y from Cartesian to SVG coordinates.
    // SVG image is 0, 0 at top left, Cartesian at bottom left.
    transform_point(pt.first, pt.second);
  }

  void calculate_plot_window()
  { // The plot window is used to set a clip path:
    // this ensures that data points and lines (and anything else)
    // outside this window is NOT drawn.
    // Start by assuming we can use all the svg image.
    // TODO Should this be reduced by width of any image border?
    plot_x1 = 0; // left
    plot_y1 = 0; // top
    plot_x2 = image.x_size(); // right
    plot_y2 = image.y_size(); // bottom

    calculate_legend_box();

    if(use_x_label)
    { // Leave space at bottom for X axis label.
      plot_y2 -= x_label_info.font_size() * text_margin_;
    }
    if(use_y_label)
    { // Leave space at left for Y axis label.
      plot_x1 += y_label_info.font_size() * text_margin_;
    }
    if(use_title)
    { // Leave space at top for title.
      // TODO what if want to put title at bottom?
      plot_y1 += title_font_size() * (text_margin_ + 1);
    }
    if(use_plot_window)
    { // Reduce to allow for legend, axes ticks to be on or outside plot_window.
      // Give the plot window a border (set with plot_border_width(double)).
      // Needed to allow any plot window border rectangle to show OK.
      // border_margin is to prevent it being right on the image border,
      // perhaps covered by any rectangle making image border.
      plot_x1 += plot_border_width() + border_margin(); // pixels.
      plot_x2 -= plot_border_width() + border_margin();
      plot_y1 += plot_border_width() + border_margin();
      plot_y2 -= plot_border_width() + border_margin();
      // Also need a >half font to allow the value label on any max major tick to avoid clipping.
      // Even after reading http://www.w3.org/TR/SVG/fonts.html, unclear how to
      // determine the width of digits, so an arbitrary width height ratio wh is used:
      const double wh = 0.7; // width compared to font height.
      static const double sin45 = 0.707; // Use if axis value labels are sloping.
      if(use_legend 
        && (legend_x1_ == -1) ) // Legend box position NOT been set by legend_top_left.
      { // Leave space for legend at right of plot window.
        plot_x2 -= legend_width_;  // legend box width.
      }
      // value_precision is 
      if (use_y_major_labels && use_y_ticks_on_plot_window_)
      { // Move plot window right to make space for value labels.
        // We would really like to know the length of the longest value label here!
        // Might convert the max and then use its .length()?
        int l = (y_value_precision_ + 1 + 1); // sign & decimal point.
         if((y_value_ioflags_ & std::ios::scientific) == std::ios::scientific)
         { // precision is number of decimal places in the fractional part (default 6).
            l += 5; // Also need space for e, sign & 3 exponent digits = 5)
         }
         else if((y_value_ioflags_ & std::ios::fixed) == std::ios::fixed)
         { // precision is 
            l += 2; // Need space for decimal point & at least 1 more digit)
         }
         else
         { // Default neither fixed nor scientific, no leading or trailing zero(s).
           // allowing sign and decimal point should probably be enough, 
           // but only a check of the actual values will be accurate.  TODO?
         }
        if (y_label_rotation_ == horizontal)
        { // Move edge right to give space for y_value_precision_ digits.
          plot_x1 += y_label_value.font_size() * (l * wh);
        }
        else if((y_label_rotation_ == upward) || (y_label_rotation_ == downward))
        { // Only need one char width from Y-axis label.
          plot_x1 += y_label_value.font_size() * 1.5;
        }
        else
        { // Assume some slope 45, so diagonally down from tick,
          // and takes a bit less room.
          plot_x1 +=  y_label_value.font_size() * l * wh * sin45;
        }
      }
      if (use_x_major_labels && use_x_ticks_on_plot_window_)
      { // Move bottom of plot window up to give space for x value labels.
        // x_value_precision_ digits etc.
        double l = x_value_precision_ + 1 + 1; // sign & decimal point.
        if(x_label_rotation_ == horizontal)
        { // Only 1 char height & 1 space needed if horizontal.
          l = 2;
        } 
        else 
        { // will need more than 2
          //l += 1; // extra space at top and bottom?
          if ((x_value_ioflags_ & std::ios::scientific) == std::ios::scientific)
          { // space for "e+000"
             l += 5;
          }
          else if((x_value_ioflags_ & std::ios::fixed) == std::ios::fixed)
          { // Need space for decimal point & sign & at least 1 more digit)
            l += 3;
          }
         if ((x_label_rotation_ == upward) || (x_label_rotation_ == downward))
          { // need space for longest.
          }
          else
          { // Assume some slope, say 45 sin(45) = 0.707.
            l *= sin45;
          }
        }
        plot_y2 -= x_label_value.font_size() * l * wh; // Move up.
      }
      if(use_left_ticks)
      { // Start left of plot to right to give space for biggest of any external left ticks.
        plot_x1 += (std::max)(y_major_tick_length_, y_minor_tick_length_);  // Avoid macro max trap!
          // TODO y_major_tick_length_ > y_minor_tick_length_ ? y_major_tick_length_ : y_minor_tick_length_;
      }
      if(use_down_ticks)
      { // Start bottom of plot higher to give space for any external down ticks.
        plot_y2 -=(std::max)(x_major_tick_length_, x_minor_tick_length_);
          //x_major_tick_length_ > x_minor_tick_length_ ? x_major_tick_length_ : x_minor_tick_length_;
      }
      // Draw plot window rectangle with background and/or border.
      image.get_g_element(detail::PLOT_WINDOW_BACKGROUND).push_back(
        new rect_element(plot_x1, plot_y1, (plot_x2 - plot_x1), plot_y2 - plot_y1));
    } // use_plot_window
  } // calculate_plot_window


  void draw_y_minor_ticks(double value, path_element& tick_path, path_element& grid_path)
  {
    double x1(0.);
    double y1(value); // for tick position and value label.
    transform_y(y1);
    double x2(image.y_size()); // right edge of image.

    if(use_y_minor_grid_)
    { // Draw the minor grid, if wanted.
      if(!use_plot_window)
      { // Use whole image, but make space for title & labels text.
        //if(use_title)
        //{ // If title at top, should make no difference, unlike X grid.
        //  x2 -= title_info.font_size() * text_margin_;
        //}
        if(use_y_label)
        {
          x1 += y_label_info.font_size() * text_margin_;
          x2 -= y_label_info.font_size() * text_margin_;
        }
      }
      else
      { // Use plot window.
        x1 = plot_x1 + 1; // TODO what is the +1 for?
        x2 = plot_x2 - 1; // Ensure *inside* window?
      }
      if((y1 >= plot_y1) && (x1 >= plot_x1) && (x2 <= plot_x2) )
      { // // Make sure that we are drawing inside the allowed window.
        grid_path.M(x1, y1).L(x2, y1);
      }
      // TODO else just ignore outside plot window?
    }

    if(use_y_ticks_on_plot_window_)
    { // Put y minor ticks on the plot window border. External style.
      x1 = plot_x1; // On the plot window border.
      x2 = plot_x1;
    }
    else
    { // Internal style.
      x1 = y_axis; // On the Y-axis line.
      x2 = y_axis;
    }
    if(use_left_ticks)
    {
      x1 -= y_minor_tick_length_;
    }
    if(use_right_ticks)
    {
      x2 += y_minor_tick_length_;
    }
    //if((x1 >= plot_x1) && (x2 <= plot_x2) && (y1 <= plot_y2) && (y1 >= plot_y1))
    // but can never be inside if left tick!
    if((y1 <= plot_y2) && (y1 >= plot_y1))
    { // Make sure that we are drawing inside of the allowed plot window.
      tick_path.M(x1, y1).L(x2, y1); // Draw the tick.
    }
    else
    {// Do nothing?  warn?
     // std::cout << "y minor tick OUTside " << x1 << ' ' << y1 << ' ' << x2 << std::endl;

    }
  } // void draw_y_minor_ticks

  void draw_y_major_ticks(double value, path_element& tick_path, path_element& grid_path)
  { // Draw Y axis major ticks, tick value labels & grids.
    double y1(value);
    transform_y(y1); // cartesian to SVG coordinates.
    double x1(0.); // left end of tick.
    double x2(image.y_size()); // right end of tick.
    if(use_y_major_grid_)
    { // Draw horizontal major grid line.
      if(!use_plot_window)
      { // TODO is this right?
        //if (use_title())
        //{ // title has no effect if at the top.
        //  y1 += title_info.font_size() * text_margin_;
        //}
        if(use_y_label)
        { // Start further right to give space for y axis value label.
          y1 -= y_label_value.font_size() * text_margin_ ;
        }

        if(use_left_ticks)
        { // And similarly for left ticks.
          y1 -= y_major_tick_length_;
        }
      }
      else
      { // Do use_plot_window.
        x1 = plot_x1 + 1;
        x2 = plot_x2 - 1;
      }
      grid_path.M(x1, y1).L(x2, y1);
    } // use_y_major_grid_

    if((y1 <= plot_y2) && (y1 >= plot_y1))
    { // Make sure that we are drawing inside the allowed window.
      double y_tick_length = y_major_tick_length_;
      if(use_y_ticks_on_plot_window_) // (was external_style)
      { // Start ticks on the plot window border.
        x1 = plot_x1; // x1 = left,
        x2 = plot_x1; //  x2 = right.
      }
      else
      { // Internal style ticks on vertical Y-axis.
        x1 = y_axis; // Y-axis line.
        x2 = y_axis;
      }
      if(use_left_ticks)
      {
        x1 -= y_tick_length; // left
      }
      if (use_right_ticks)
      {
        x2 += y_tick_length; // right.
      }
      tick_path.M(x1, y1).L(x2, y1); // Draw the major tick.
      // leaving x1 at the left most end of any tick.
    }

    if(use_y_major_labels)
    { // Label the tick with value, for example "1.2"
      std::stringstream label;
      label.precision(y_value_precision_);
      label.flags(y_value_ioflags_); // set ALL IOflags.
      label << value; // Example: label.str() == "20" or "0.25" or "1.2e+015"
      if(use_left_ticks)
      {
        if (y_minor_tick_length_ > y_minor_tick_length_)
        { // In case some joker has made the minor ticks longer than the major,
          // we might need to move left more for the longer tick.
          x1 -= (y_minor_tick_length_ - y_minor_tick_length_);
        }
        x1 -= y_label_value.font_size(); // move left by a font width.
      }
      else
      { // No need to move if right tick, or none.
      }
      // Need to work out how much space value labels will need.
      double y = y1;
      align_style alignment = center_align;
      if(use_y_ticks_on_plot_window_)
      { // External to plot window style.
        if(y_label_rotation_ == horizontal)
        {  // Just shift down half a digit to center value digits on tick.
          alignment = right_align;
          y += y_label_value.font_size() / 2;
        }
        else if ((y_label_rotation_ == upward) || (y_label_rotation_ == downward))
        {// Tick value label straight up or down vertically on y axis.
         // No shift y down to center.
          alignment = center_align;
        }
        else
        { // Assume some 45 slope, so need about sqrt(2) less space?
           x1 += y_label_value.font_size() * 0.5; // move left by half a font width.
          // no y shift needed.
          alignment = right_align;
        }
        // Always want all values including "0", if labeling external to plot window.
        image.get_g_element(detail::PLOT_VALUE_LABELS).text(
          x1,
          y,
          label.str(), y_label_value.font_size(), "", "", "", "", "", alignment, y_label_rotation_);
      }
      else
      { // ! use_y_ticks_on_plot_window_Internal - value labels close to left of vertical Y-axis.
        if ((value != 0) && use_x_axis_lines_)
        { // Avoid a zero ON the Y-axis as it would be cut through by the horizontal X-axis line.
          y += y_label_value.font_size() / 2;
          image.get_g_element(detail::PLOT_VALUE_LABELS).text(x1,
            y, // Just shift down half a digit to center value digits on tick.
            label.str(), y_label_value.font_size(), y_label_value.font_family(), "", "", "", "",
            center_align, // on the tick ???
            y_label_rotation_);
        }
      }
    } // if(use_y_major_labels)
  } // draw_y_major_ticks

  void draw_y_axis()
  {
    double x1(0.);
    transform_x(x1);
    y_axis = x1; // in SVG coordinates.
    if(use_y_axis_lines_)
    { // Draw the vertical Y-axis line (at cartesian x = 0).
      double ybottom = plot_y2;
      if (use_down_ticks)
      { // Extend the vertical line down in lieu of a tick.
        ybottom += (std::max)(x_minor_tick_length_, x_major_tick_length_);
      }
      image.get_g_element(detail::PLOT_Y_AXIS).line(x1, plot_y1, x1, ybottom);
    // <g id="yAxis" stroke="rgb(0,0,0)"><line x1="70.5" y1="53" x2="70.5" y2="357"/>
    }
    // Access the paths for the ticks & grids, ready for additions.
    path_element& minor_tick_path = image.get_g_element(detail::PLOT_Y_MINOR_TICKS).path();
    path_element& major_tick_path = image.get_g_element(detail::PLOT_Y_MAJOR_TICKS).path();
    path_element& minor_grid_path = image.get_g_element(detail::PLOT_Y_MINOR_GRID).path();
    path_element& major_grid_path = image.get_g_element(detail::PLOT_Y_MAJOR_GRID).path();

    if(use_y_ticks_on_plot_window_)
    { // TODO confirm that labels allow space.
      // Was y_external style - so both labels (and ticks) are OUTSIDE the plot window.
      //image.get_g_element(detail::PLOT_Y_AXIS).line(plot_y1, x_axis, plot_x2, x_axis);
      // TODO this looks rubbish.
    }

    // y_minor_jump is the interval between minor ticks.
    double y_minor_jump = y_major_interval_ / ((double)(y_num_minor_ticks_ + 1.) );
    // TODO Problem here with using floating point?
    // Was i < y_max; but didn't show the tick and value at y_max so now i <= y_max;
    // But may still fail if a ls or few bits out? Seems to fail for y = 100, for example.

    // Draw the ticks on the positive side.
    for(double y = 0; y <= y_max; y += y_major_interval_)
    {
      for(double j = y + y_minor_jump;
        j < (y + y_major_interval_) * (1. - 2 * std::numeric_limits<double>::epsilon());
        j += y_minor_jump)
      { // Draw minor ticks.
        // This will output 'orphaned' minor ticks that are beyond the plot window,
        // if the last major tick does not coincide with the plot window.
        // These are just ignored in draw_x_minor_ticks.
        // There might be 9 of them,
        // if you have the common 9 minor tick between major ticks!
        // TODO this seems ugly - as does the negative ones below.
        if (j != 0. || ! use_x_axis_lines_)
        { // Avoid a major tick at y == 0 where there *is* a horizontal X-axis line.
          // (won't be X-axis line for 1-D where the zero tick is always wanted).
          draw_y_minor_ticks(j, minor_tick_path, minor_grid_path);
        }
      }
      if ((y != 0. || ! use_x_axis_lines_) || use_y_ticks_on_plot_window_)
      { // Avoid a major tick at y == 0 where there *is* a horizontal X-axis line.
        // (won't be X-axis line for 1-D where the zero tick is always wanted).
        draw_y_major_ticks(y, major_tick_path, major_grid_path);
      }
    }

    // Draw the ticks on the negative side.
    for(double y = 0; y >= y_min; y -= y_major_interval_)
    {
      for(double j=y; j > y-y_major_interval_; j-= y_major_interval_ / (y_num_minor_ticks_ + 1))
      { // Draw minor ticks.
        if ((j != 0. || ! use_x_axis_lines_) || use_y_ticks_on_plot_window_)
        { // Avoid a major tick at y == 0 where there *is* a horizontal X-axis line.
          // (won't be X-axis line for 1-D where the zero tick is always wanted).
          draw_y_minor_ticks(j, minor_tick_path, minor_grid_path);
        }
      }
      if ((y != 0. || ! use_x_axis_lines_) || use_y_ticks_on_plot_window_)
      { // Avoid a major tick at y == 0 where there *is* a horizontal X-axis line.
        // (won't be X-axis line for 1-D where the zero tick is always wanted).
        draw_y_major_ticks(y, major_tick_path, major_grid_path);
      }
    }
  } // draw_y_axis

  void draw_y_label()
  {
    // image.get_g_element(detail::PLOT_Y_LABEL).style().stroke_color(black);
    // Now set in constructor, and can be changed with member function .

    std::string label = y_label_info.text(); // x_axis label, and optional units.
    if (use_x_label_units && (y_units_info.text() != ""))
    { // Append the units, if any, providing brackets ().
      // TODO Is this the best policy - or should the user provide the ()s????
      label += "  (" + y_units_info.text() + ")";
    }

    image.get_g_element(detail::PLOT_Y_LABEL).push_back(new
      text_element(y_label_info.font_size() * 1.5,
      // shift over one char height to right from left edge of image.
      (plot_y2 + plot_y1) / 2., // center on the plot window.
      label, // "Y-Axis" for example.
       y_label_info.font_size(),
       y_label_info.font_family(),
       "", "", "", "",
       center_align, // One might want it to left or right_align?
       upward)); // Y label must be drawn vertically.
  } // draw_y_label

  void draw_straight_lines(const svg_2d_plot_series& series)
  { // Straight line between data points (rather than a Bezier curve).
    double prev_x; // Previous data points.
    double prev_y;
    double temp_x(0.);
    double temp_y;

    g_element& g_ptr = image.get_g_element(detail::PLOT_DATA_LINES).add_g_element();
    g_ptr.clip_id(plot_window_clip);
    g_ptr.style().stroke_color(series.line_style.color);
    g_ptr.style().fill_color(series.line_style.area_fill);
    path_element& path = g_ptr.path();
    path.style().fill_color(series.line_style.area_fill);

    bool is_fill = !series.line_style.area_fill.blank;
    path.fill = is_fill; // Ensure includes a fill="none".

    if(series.series.size() > 1)
    {
      std::multimap<double, double>::const_iterator j = series.series.begin();

      // If we have to fill the area under the plot,
      // we first have to move from the X-axis (y = 0) to the first point,
      // and again at the end after the last point.
      prev_x = (*j).first;
      prev_y = 0.;
      transform_point(prev_x, prev_y);
      if(is_fill)
      { // Move to 1st point.
        path.style().fill_color(series.line_style.area_fill);
        path.M(prev_x, prev_y);
      }
      transform_y(prev_y = (*j).second);
      if(is_fill)
      { // fill wanted.
        path.style().fill_color(series.line_style.area_fill); // TOD why again?
        path.L(prev_x, prev_y); // Line from X-axis to 1st point.
      }
      else
      { // fill == blank
        path.M(prev_x, prev_y);
      }
      ++j; // so now refers to 2nd point to plot.

      for(; j != series.series.end(); ++j)
      {
        temp_x = (*j).first;
        temp_y = (*j).second;
        transform_point(temp_x, temp_y);
        path.L(temp_x, temp_y); // line to next point.

        if(is_fill)
        {
          path.M(temp_x, temp_y);
        }
        prev_x = temp_x;
        prev_y = temp_y;
      } // for j

      if(is_fill)
      { // fill wanted.
        transform_y(temp_y = 0.); // X-axis line.
        path.L(temp_x, temp_y).z(); // closepath with Z to terminate line.
      }
    }
  } // draw_straight_lines

  void draw_bezier_lines(const svg_2d_plot_series& series)
  {
    g_element& g_ptr = image.get_g_element(detail::PLOT_DATA_LINES).add_g_element();
    g_ptr.clip_id(plot_window_clip);
    g_ptr.style().stroke_color(series.line_style.color);
    path_element& path = g_ptr.path();

    std::pair<double, double> n; // current point.
    std::pair<double, double> n_minus_1; // penultimate.
    std::pair<double, double> n_minus_2;
    std::pair<double, double> fwd_vtr;
    std::pair<double, double> back_vtr;

    bool is_fill = !series.line_style.area_fill.blank;
    if(is_fill == false)
    {
      path.fill = false; // default path constructor is true - TODO why??
    }
    else
    { // fill
      path.style().fill_color(series.line_style.area_fill);
    }

    if(series.series.size() > 2)
    { // Need >= 3 for a cubic curve (start point, 2 control points, and end point).
      std::multimap<double, double>::const_iterator iter = series.series.begin();
      n_minus_1 = *(iter++);  // begin() 
      transform_pair(n_minus_1);
      n = *(iter++); // middle
      transform_pair(n);
      path.M(n_minus_1.first, n_minus_1.second); // move m_minus_1, the 1st data point.

      double control = 0.1;  
      //0.2 is a scaling factor that Jake used to define the magnitude of the
      //vector of the current control point to be placed. I was basically
      //taking advantage of the auto-drawing of Bezier curves that exists in
      //the SVG format, and this is my attempt to give the control point the
      //proper length.

      // Experiment suggests that 0.2 gives distorsions with exp curves.
      // 0.05 is just visually OK with 50 points, but 100 are better.

      for(; iter != series.series.end(); ++iter)
      {
        n_minus_2 = n_minus_1; 
        n_minus_1 = n;
        n = *iter;
        transform_pair(n);

        back_vtr.first = ((n_minus_1.first - n.first) + // (x diff - x previous diff) * control
          (n_minus_2.first - n_minus_1.first)) * control; 
        back_vtr.second = ((n_minus_1.second - n.second) + // y
          (n_minus_2.second - n_minus_1.second)) * control;

        // 8.3.6 The cubic Bézier curve commands path.S(x, y).
        // Start point, end point, & two control points.
        // Example: S378.5,519.3 381,519.3 ...
        // S end_control_point, end point
        // Start is reflection of last point's control point.
        path.S(n_minus_1.first + back_vtr.first, // x
               n_minus_1.second + back_vtr.second, // y - end control point
               n_minus_1.first, n_minus_1.second); // x, y - end point.
      } // for
      // Last point.
      back_vtr.first = 0.; 
      back_vtr.second = (n.second - n_minus_1.second) * control;
      path.S(n.first + back_vtr.first, // x
             n.second + back_vtr.second, // y
             n.first, n.second); // x, y
    }
    else
    { // Only one or two points, so no curving possible!
      draw_straight_lines(series);
    }
  } // draw_bezier_lines

  void draw_plot_lines()
  { // Draw line through data series, Bezier curved or straight.
    for(unsigned int i = 0; i < series.size(); ++i)
    {
      if(series[i].line_style.bezier_on)
      { // curved.
        draw_bezier_lines(series[i]);
      }
      else
      {
        draw_straight_lines(series[i]);
      }
    }
  } // draw_plot_lines

  void update_image()
  {
    clear_all();
    // svg paint rules are that later 'painting' writes over previous
    // painting, so the order of drawing is important.

    // Draw image background (perhaps with border and/or fill color).
    image.get_g_element(detail::PLOT_BACKGROUND).push_back(
      new rect_element(0, 0, image.x_size(),  image.y_size()));

    calculate_plot_window();
    calculate_transform();
    draw_title(); // Moved to ensure plot_X and Y are valid.

    // Define the clip path for the plot window.
    // We don't want to allow overlap of the plot window border lines,
    // thus the minor adjustments.
    // TODO should this be border thickness?

    image.clip_path(rect_element(plot_x1 + 1, plot_y1 + 1,
      plot_x2 - plot_x1 - 2, plot_y2 - plot_y1 - 2),
      plot_window_clip);
    // <clipPath id="plot_window"><rect x="35" y="38" width="309" height="322"/></clipPath>

    image.get_g_element(detail::PLOT_DATA_POINTS).clip_id(plot_window_clip);

    // Draw axes, labels & legend, as required.
    if(use_axis)
    {
      draw_x_axis(); // Must do X-axis first.
      draw_y_axis(); // TODO is draw_axes used?
    }
    if(use_legend)
    {
      draw_legend();
    }
    if(use_x_label)
    {
      draw_x_label();
    }
    if(use_y_label)
    {
      draw_y_label();
    }

    draw_plot_lines(); // Draw lines between points.

    // Draw normal 'good' non-limit points.
    double x(0.);
    double y(0.);
    for(unsigned int i = 0; i < series.size(); ++i)
    {
      g_element& g_ptr = image.get_g_element(detail::PLOT_DATA_POINTS).add_g_element();

      g_ptr.style()
        .fill_color(series[i].point_style.fill_color_)
        .stroke_color(series[i].point_style.stroke_color_);

      for(std::multimap<double, double>::const_iterator j = series[i].series.begin();
        j != series[i].series.end(); ++j)
      {
        x = j->first;
        y = j->second;
        transform_point(x, y);
        if((x > plot_x1)  && (x < plot_x2) && (y > plot_y1)  && (y < plot_y2))
        { // Onside plot window, so draw a point.
          draw_plot_point(x, y, g_ptr, series[i].point_style);
        }
      } // for
    } // for normal points

    // Draw all the 'bad' at_limit points.
    for(unsigned int i = 0; i < series.size(); ++i)
    {
      g_element& g_ptr = image.get_g_element(detail::PLOT_LIMIT_POINTS);

      for(std::multimap<double,double>::const_iterator j = series[i].series_limits.begin();
        j!=series[i].series_limits.end(); ++j)
      {
        x = j->first;
        y = j->second;
        transform_point(x, y);
        draw_plot_point(x, y, g_ptr, plot_point_style(blank, blank, series[i].point_style.size_, cone));
        // TODO why are fill and stroke blank? How is point style for 'bad' values determined?
      }
    } // limits point
  } // void update_image()


public: // Member functions
  // -----------------------------------------------------------------
  // write() has two flavors, a file and a ostream.
  // The file version opens an ostream, and calls the stream version.
  // The stream version first clears all unnecessary data from the graph,
  // builds the document tree, and then calls the write function for the root
  // document node, which calls all other nodes through the Visitor pattern.
  // -----------------------------------------------------------------

  svg_2d_plot& write(const std::string& filename)
  { // Write the plot image to a named file.
    std::ofstream fout(filename.c_str());
    if(fout.fail())
    {
      throw std::runtime_error("Unable to open " + filename);
    }
    write(fout); // Using the ostream version.
    return *this;
  }

  svg_2d_plot& write(std::ostream& s_out)
  { // Write the image to an ostream.
    update_image();
    image.write(s_out); // Use the ostream version of write.
    return *this;
  }

  // Member functions to set plot options.
  // All return *this to permit chaining.

  // These below only refer to 2D plot.
  // See axis_plot_label.hpp for all the many 1D functions X-Axis.

  svg_2d_plot& y_label_on(bool cmd)
  { // Y axis name or label.
    use_y_label = cmd;
    return *this;
  }

  bool y_label_on()
  {
    return use_y_label;
  }

  svg_2d_plot& x_label_on(bool cmd)
  {
    use_x_label = cmd;
    return *this;
  }

  bool x_label_on()
  {
    return use_x_label;
  }

  svg_2d_plot& y_major_labels_on(bool cmd)
  {
    use_y_major_labels = cmd;
    return *this;
  }

  bool y_major_labels_on()
  {
    return use_y_major_labels;
  }

  svg_2d_plot& y_major_label_rotation(int rot)
  {
    y_label_rotation_ = rot;
    return *this;
  }

  int y_major_label_rotation()
  {
    return y_label_rotation_;
  }

  svg_2d_plot& y_axis_width(double width)
  {
    image.get_g_element(detail::PLOT_Y_AXIS).style().stroke_width(width);
    return *this;
  }

  double y_axis_width()
  {
    return image.get_g_element(detail::PLOT_Y_AXIS).style().stroke_width();
  }

  svg_2d_plot& y_value_precision(int digits)
  { // Precision of Y tick label values in decimal digits (default 3).
    y_value_precision_ = digits;
    return *this;
  }

  int y_value_precision()
  { // Precision of Y tick label values in decimal digits (default 3).
    return y_value_precision_;
  }

  svg_2d_plot& y_value_ioflags(int flags)
  { // IO flags of Y tick label values (default 0X201).
    y_value_ioflags_ = flags;
    return *this;
  }

  int y_value_ioflags()
  { // ALL stream ioflags for control of format of Y value labels.
    return y_value_ioflags_;
  }

  svg_2d_plot& y_axis_color(const svg_color& col)
  { // Set only stroke color.
    image.get_g_element(detail::PLOT_Y_AXIS).style().stroke_color(col);
    return *this;
  }

  svg_color y_axis_color()
  { // eturn the stroke color.
    return image.get_g_element(detail::PLOT_Y_AXIS).style().stroke_color();
  }

  svg_2d_plot& y_axis_label_color(const svg_color& col)
  { // Set BOTH stroke and fill to the same color.
    image.get_g_element(detail::PLOT_VALUE_LABELS).style().fill_color(col);
    image.get_g_element(detail::PLOT_VALUE_LABELS).style().stroke_color(col);
    return *this;
  }

  svg_color y_axis_label_color()
  { // But only return the stroke color.
    return image.get_g_element(detail::PLOT_VALUE_LABELS).style().stroke_color();
  }

  svg_2d_plot& y_label_units_on(bool b)
  {
    use_y_label_units = b;
    return *this;
  }

  bool y_label_units_on()
  { // But only return the stroke color.
    return use_y_label_units;
  }

  svg_2d_plot& y_axis_value_color(const svg_color& col)
  { // Set BOTH stroke and fill to the same color.
    image.get_g_element(detail::PLOT_VALUE_LABELS).style().fill_color(col);
    image.get_g_element(detail::PLOT_VALUE_LABELS).style().stroke_color(col);
    return *this;
  }

  svg_color y_axis_value_color()
  { // But only return the stroke color.
    return image.get_g_element(detail::PLOT_VALUE_LABELS).style().stroke_color();
  }

  svg_2d_plot& y_label_width(double width)
  { // width of text is effectively the boldness
    image.get_g_element(detail::PLOT_Y_LABEL).style().stroke_width(width);
    return *this;
  }

  double y_label_width()
  {
    return image.get_g_element(detail::PLOT_Y_LABEL).style().stroke_width();
  }

  svg_2d_plot& y_major_grid_color(const svg_color& col)
  {
    image.get_g_element(detail::PLOT_Y_MAJOR_GRID).style().stroke_color(col);
    return *this;
  }

  const svg_color& y_major_grid_color()
  {
    return image.get_g_element(detail::PLOT_Y_MAJOR_GRID).style().stroke_color();
  }

  svg_2d_plot& y_minor_grid_color(const svg_color& col)
  {
    image.get_g_element(detail::PLOT_Y_MINOR_GRID).style().stroke_color(col);
    return *this;
  }

  const svg_color& y_minor_grid_color()
  {
    return image.get_g_element(detail::PLOT_Y_MINOR_GRID).style().stroke_color();
  }

  svg_2d_plot& y_major_tick_color(const svg_color& col)
  {
    image.get_g_element(detail::PLOT_Y_MAJOR_TICKS).style().stroke_color(col);
    return *this;
  }

  const svg_color& y_major_tick_color()
  {
    return image.get_g_element(detail::PLOT_Y_MAJOR_TICKS).style().stroke_color();
  }

  svg_2d_plot& y_minor_tick_color(const svg_color& col)
  {
    image.get_g_element(detail::PLOT_Y_MINOR_TICKS).style().stroke_color(col);
    return *this;
  }

  const svg_color& y_minor_tick_color()
  {
    return image.get_g_element(detail::PLOT_Y_MINOR_TICKS).style().stroke_color();
  }

  svg_2d_plot& y_range(double y1, double y2)
  { // Set the range (max and min) for Y values.
    y_min = y1;
    y_max = y2;
    if(y2 <= y1)
    {
      throw std::runtime_error("Illegal Argument: X scale: x2 < x1");
    }
    return *this;
  }

  std::pair<double, double> y_range()
  {
    std::pair<double, double> r;
    r.first = y_min;
    r.second = y_max;
    return r;
  }

  double y_minimum()
  {
    return y_min;
  }

  double y_maximum()
  {
    return y_max;
  }

  svg_2d_plot& y_major_interval(double inter)
  {
    y_major_interval_ = inter;
    return *this;
  }

  double y_major_interval()
  {
    return y_major_interval_;
  }

  svg_2d_plot& y_major_tick_length(double length)
  {
    y_major_tick_length_ = length;
    return *this;
  }

  double y_major_tick_length()
  {
    return y_major_tick_length_;
  }

  svg_2d_plot& y_minor_tick_length(double length)
  {
    y_minor_tick_length_ = length;
    return *this;
  }

  double y_minor_tick_length()
  {
    return y_minor_tick_length_;
  }

  svg_2d_plot& y_num_minor_ticks(unsigned int num)
  {
    y_num_minor_ticks_ = num;
    return *this;
  }

  unsigned int y_num_minor_ticks()
  {
    return y_num_minor_ticks_;
  }

  svg_2d_plot& y_label_axis(const std::string& str)
  { // Label for Y-axis.
    y_label_info.text(str);
    return *this;
  }

  std::string y_label_axis()
  {
    return y_label_info.text();
  }

  svg_2d_plot& y_major_tick_width(double width)
  {
    y_major_tick_width_ = width;
    image.get_g_element(detail::PLOT_Y_MAJOR_TICKS).style().stroke_width(width);
    return *this;
  }

  double y_major_tick_width()
  {
    return y_major_tick_width_;
  }

  svg_2d_plot& y_minor_tick_width(double width)
  {
    y_minor_tick_width_ = width;
    image.get_g_element(detail::PLOT_Y_MINOR_TICKS).style().stroke_width(width);
    return *this;
  }

  double y_minor_tick_width()
  {
    return y_minor_tick_width_;
  }

  svg_2d_plot& x_ticks_on_plot_window_on(bool is)
  {
    use_x_ticks_on_plot_window_ = is;
    return *this;
  }

  bool x_ticks_on_plot_window_on()
  {
    return use_x_ticks_on_plot_window_;
  }

  svg_2d_plot& y_ticks_on_plot_window_on(bool is)
  {
    use_y_ticks_on_plot_window_ = is;
    return *this;
  }

  bool y_ticks_on_plot_window_on()
  {
    return use_y_ticks_on_plot_window_;
  }


  svg_2d_plot& y_ticks_left_on(bool cmd)
  {
    use_left_ticks = cmd;
    return *this;
  }

  bool y_ticks_left_on()
  {
    return use_left_ticks;
  }

  svg_2d_plot& y_ticks_right_on(bool cmd)
  {
    use_right_ticks = cmd;
    return *this;
  }

  bool y_ticks_right_on()
  {
    return use_right_ticks;
  }
  // Only need y_ticks_left_on & y_ticks_right_on in 2D


  svg_2d_plot& y_major_grid_on(bool is)
  {
    use_y_major_grid_ = is;
    return *this;
  }

  bool y_major_grid_on()
  {
    return use_y_major_grid_;
  }

  svg_2d_plot& y_minor_grid_on(bool is)
  {
    use_y_minor_grid_ = is;
    return *this;
  }

  bool y_minor_grid_on()
  {
    return use_y_minor_grid_;
  }

  svg_2d_plot& y_minor_grid_width(double width)
  {
    y_minor_grid_width_ = width;
    image.get_g_element(detail::PLOT_Y_MINOR_GRID).style().stroke_width(width);
    return *this;
  }

  double y_minor_grid_width()
  {
    return y_minor_grid_width_;
  }

  svg_2d_plot& y_major_grid_width(double width)
  {
    y_major_grid_width_ = width;
    image.get_g_element(detail::PLOT_Y_MAJOR_GRID).style().stroke_width(width);
    return *this;
  }

  double y_major_grid_width()
  {
    return y_major_grid_width_;
  }

  svg_2d_plot& y_label_font_size(unsigned int i)
  { // May be best to tie label & unit font sizes together?
    y_label_info.font_size(i);
    // y_units_info.font_size(i);
    return *this;
  }

  unsigned int y_label_font_size()
  {
    return y_label_info.font_size();
  }

  svg_2d_plot& y_label_weight(std::string s)
  { // "bold2 is only one that works so far.
    y_label_info.font_weight(s);
    return *this;
  }

  const std::string& y_label_weight()
  {
    return y_label_info.font_weight();
  }

  svg_2d_plot& y_label_font_family(const std::string& family)
  {
    y_label_info.font_family(family);
    return *this;
  }

  const std::string& y_label_font_family();
  //const std::string& y_label_font_family()
  //{
  //  return y_label_info.font_family();
  //}

  // Example of declaration but definition below.
  // TODO Probably better done this way,
  // but wait until parameter system removed.








  // plot member function, with several parameters, using Boost.Parameter,
  // to add data series to the plot window.
  // TODO replace this with chainable member functions.

#if defined (BOOST_MSVC)
#  pragma warning(push)
#  pragma warning(disable: 4100) // "'boost_parameter_enabler_argument' : unreferenced formal parameter"
#endif

  BOOST_PARAMETER_MEMBER_FUNCTION
    (
    (void),
    plot,
    tag,
    (required
    (container, *)
    (title, (const std::string&))
    )
    (optional
    (fill_color, (const svg_color&), white)
    (stroke_color, (const svg_color&), black)
    (line_color, (const svg_color&), black)
    (area_fill_color, (const svg_color&), true) // == is blank
   // (area_fill_color, (svg_color_constant), blank)
    (point_style, (point_shape), round)
    (size, (int), 10)
    (line_on, (bool), true)
    (bezier_on, (bool), false)
    (x_functor, *, detail::boost_default_2d_convert())
    )
    )
  {
    // This line has the error in GCC. Also, operator() in boost_default_2d_convert
    // has been changed so that the value of i is not updated. I would like
    // the start to be set, as well as i to update in operator().
    // x_functor.start(1.);

    plot_line_style line_style(line_color, blank, line_on, bezier_on);

    line_style.area_fill = area_fill_color;

    series.push_back(
      svg_2d_plot_series(
      boost::make_transform_iterator(container.begin(), x_functor),
      boost::make_transform_iterator(container.end(),   x_functor),
      title,
      plot_point_style(fill_color, stroke_color, size, point_style),
      line_style
      ));
  }
}; // class svg_2d_plot : public detail::axis_plot_frame<svg_2d_plot>

// sample of declared function, externally defined in another .cpp file.
// TODO May be useful to refactor all functions this way.
  const std::string& svg_2d_plot::y_label_font_family()
  {
    return y_label_info.font_family();
  }


#if defined (BOOST_MSVC)
#  pragma warning(pop)
#endif

  } // namespace svg
} // namespace boost

#endif // BOOST_SVG_SVG_2D_PLOT_HPP
