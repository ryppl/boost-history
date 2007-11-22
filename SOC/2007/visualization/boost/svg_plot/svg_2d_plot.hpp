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

struct svg_2d_plot_series
{
  // 2-D Data series points to plot.
  std::multimap<double, double> series; // Normal 'OK to plot' data values.
  std::multimap<double, double> series_limits; // 'limit' values: too big or small, or NaN.
  // TODO is there a reason why multimap is used rather than vector of pairs?
  // multimap sorts, which will take time and isn't needed?

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
  text_element legend_header; // legend box header or title (if any).
  text_element x_label_info; // For example: "length"
  text_element y_label_info; // For example: "volume"
  text_element x_units_info; // For example: "mm"
  text_element y_units_info; // 2-D only.

  double text_margin; // Marginal space around text items like title,
  // as a fraction of font size, (pixels) (was fixed at 1.5).
  int border_margin; // Marginal (pixels) space around the plot border.

  // Border information for the plot window (<= image size).
  // Allows for title and legend to be separate.
  // Initially will be set to the width and height of the graph image.
  // calculate_plot_window() sets these values.
  int plot_x1; // TODO unsigned int? or double?
  int plot_x2;
  int plot_y1;
  int plot_y2;

  // X-Axis information.
  double x_min; // minimum x value (Cartesian units).
  double x_max; // maximum x value (Cartesian units).
  double x_axis; // // x-axis (y = 0) transformed into SVG coordinates.
  double x_major; // x_major is the stride or interval for major x ticks.
  // set/get by x_major_interval
  double x_minor; // Interval (Cartesian units) between minor ticks.
  // because x_num_minor_ticks_ used to determine this instead,
  // but one could calculate x_minor.

  unsigned int x_major_tick_length_; // pixels.
  unsigned int x_major_tick_width_; // pixels.
  unsigned int x_minor_tick_length_; // pixels.
  unsigned int x_minor_tick_width_; // pixels.
  // Avoid name clash with x_m*_tick_length and width.
  unsigned int x_num_minor_ticks_; // number of minor ticks, eg 4 gives major 0, minor 1,2,3,4, major 5
  unsigned int x_major_grid_width_; // pixels.
  unsigned int x_minor_grid_width_; // pixels.
  // grid width is set in style.stroke_width
  //unsigned int x_major_label; // pixels. Now in x_units_info font size

  // Y-Axis information.
  double y_min; // minimum y value (Cartesian units).
  double y_max; // maximum y value (Cartesian units).
  double y_major; // y_major is the stride or interval for major y ticks.
  double y_axis; // Y-axis (x = 0) transformed into SVG coordinates.

  unsigned int y_major_tick_length_; // pixels.
  unsigned int y_major_tick_width_; // pixels.
  unsigned int y_minor_tick_length_; // pixels.
  unsigned int y_minor_tick_width_; // pixels.
  // Avoid name clash with x_m*_tick_length and width.
  unsigned int y_num_minor_ticks_; // number of minor ticks, eg 4 gives major 0, minor 1,2,3,4, major 5
  // grid width is set in style.stroke_width
  unsigned int y_major_grid_width_; // pixels.
  unsigned int y_minor_grid_width_; // pixels.
  //unsigned int y_major_label; // pixels.

  // Yes/no options.
  bool use_title;
  bool use_legend;
  bool use_axis;
  bool use_plot_window;
  bool use_x_major_labels; // values for major ticks.
  bool use_y_major_labels;
  bool use_x_major_grid;
  bool use_x_minor_grid;
  bool use_x_label;
  bool use_y_label;
  bool use_x_label_units;
  bool use_y_label_units;

  // Note: can have ticks both up and/or down, and/or left and right.
  bool use_up_ticks;
  bool use_down_ticks;
  bool use_x_ticks; // = use_up_ticks || use_down_ticks
  bool use_left_ticks;
  bool use_right_ticks;
  bool use_y_ticks; // = use_left_ticks || use_right_ticks
  bool use_x_ticks_on_plot_window_; // was external style.
  bool use_y_ticks_on_plot_window_; // rather than on y-axis.
  bool use_x_axis_lines_; // ??? TODO what does this do?
  bool use_y_axis_lines_; // ??? TODO what does this do?
  bool use_y_major_grid;
  bool use_y_minor_grid;
  bool use_line; // set by line_on(bool); // Not really useful for 1-D,
  // TODO but needs Boost-Parameter removed to do properly.

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
    x_label_info(0, 0, "X Axis", 14, "verdana", "", "", "", "", center_align, horizontal),
    y_label_info(0, 0, "Y Axis", 14, "verdana", "", "", "", "", center_align, upward),
    //x_label_info(0, 0, "X Axis", 12, "Lucida Sans Console", "", "", "", "", center_align, horizontal),
    //y_label_info(0, 0, "Y Axis", 12, "Lucida Sans Console", "", "", "", "", center_align, upward),
    x_units_info(0, 0, "(units)", 14, "Lucida Sans Console", "", "", "", "", center_align, horizontal),
    y_units_info(0, 0, "(units)", 14, "Lucida Sans Console", "", "", "", "", center_align, upward),
    // Plot may look odd if font and size of label & units are different!
    // y_units_info(0, 0, "(units)", 11, "Times New Roman", "italic", "bold", "wider", "underline", right_align, upsidedown),
    // shows some values that might be useful if svg browsers are fully implemented
    // to support modification to text characters.
    text_margin(2.), // for text was 1.5 // as a fraction of the font size.
    border_margin(5), // Prevent plot window box begin right on edge.
    // plot_x1, x2, y1, 2 are set in calculate_plot_window 
    x_min(-10), x_max(10), // so plots from -10 to +10
    y_min(-10), y_max(10),
    x_major(2), // x stride
    y_major(2), // y stride
    // Ticks on axes.
    x_num_minor_ticks_(2),
    y_num_minor_ticks_(4), // suits: major 0, minor 2, 4, 6, 8, major 10
    x_major_tick_length_(10), // If both up and down, total is twice this.
    x_minor_tick_length_(5),
    x_major_tick_width_(2),
    x_minor_tick_width_(1),
    y_major_tick_length_(10), // If both left and right, total is twice this.
    y_minor_tick_length_(5),
    y_major_tick_width_(2),
    y_minor_tick_width_(1),
    // grids
    x_major_grid_width_(2),
    x_minor_grid_width_(1),
    y_major_grid_width_(2),
    y_minor_grid_width_(1),
    x_scale(1.), x_shift(0),
    y_scale(1.), y_shift(0),

    plot_window_clip("plot_window"), // for <clipPath id="plot_window" ...

    // Boolean options.
    use_down_ticks(true), // On X-axis.
    use_up_ticks(false), // external ticks only.
    use_x_ticks(false), // = use_up_ticks || use_down_ticks
    use_left_ticks(true), // On y-axis.
    use_right_ticks(false), // 
    use_y_ticks(false), // = use_left_ticks || use_right_ticks
    use_x_ticks_on_plot_window_(false), // was external_style
    use_y_ticks_on_plot_window_(false), // was external_style
    use_x_label_units(false),
    use_y_label_units(false),
    use_x_major_labels(true),
    use_y_major_labels(false),
    use_x_label(false),
    use_y_label(true), // true for 2-D
    use_title(true),
    use_legend(false), use_axis(true),
    use_x_axis_lines_(true),
    use_y_axis_lines_(true),
    use_x_major_grid(false),
    use_x_minor_grid(false),
    use_y_major_grid(false),
    use_y_minor_grid(false),
    use_plot_window(true),
    use_line(true)
    // TODO but needs Boost-Parameter removed to do properly.

  {
    image_size(500, 400); // Default image size for 2-D.
    // 2-D needs to be squarer than 1-D.
    use_x_ticks = use_up_ticks || use_down_ticks;
    use_y_ticks = use_left_ticks || use_right_ticks;

    // Build the document tree by adding all children of the root node.
    for(int i = 0; i < detail::SVG_PLOT_DOC_CHILDREN; ++i)
    {
      image.add_g_element();
    }

    // Set other SVG color, stroke & width defaults for various child PLOT nodes.
    // Tick length etc is now set above in the constructor.
    image.get_g_element(detail::PLOT_BACKGROUND).style().fill_color(white);
    image.get_g_element(detail::PLOT_LIMIT_POINTS).style().stroke_color(lightgray).fill_color(whitesmoke);
    image.get_g_element(detail::PLOT_Y_AXIS).style().stroke_color(black);
    image.get_g_element(detail::PLOT_Y_AXIS).style().stroke_width(1);
    image.get_g_element(detail::PLOT_X_AXIS).style().stroke_color(black);
    image.get_g_element(detail::PLOT_X_MINOR_TICKS).style().stroke_color(black);
    image.get_g_element(detail::PLOT_X_MAJOR_TICKS).style().stroke_color(black);
    image.get_g_element(detail::PLOT_X_LABEL).style().stroke_color(blue); // TODO temp for tests
    image.get_g_element(detail::PLOT_Y_LABEL).style().stroke_color(green); // TODO temp for tests
    image.get_g_element(detail::PLOT_X_LABEL).style().stroke_width(1); 
    image.get_g_element(detail::PLOT_Y_LABEL).style().stroke_width(1); // TODO control of this?
    image.get_g_element(detail::PLOT_PLOT_LABELS).style().stroke_color(blue);
    image.get_g_element(detail::PLOT_PLOT_LABELS).style().stroke_width(1); // 1 is bold blue
    //image.get_g_element(detail::PLOT_PLOT_LABELS).style().stroke_width(0); // 0 is thin *black* - ignoring color!
    // control with x_axis_value_color

    // TODO separate control of data series point *values* 1.234 and their colors.

    // Note that widths are stored in member data and copied here.
    image.get_g_element(detail::PLOT_X_MAJOR_TICKS).style().stroke_width(x_major_tick_width_);
    image.get_g_element(detail::PLOT_X_MINOR_TICKS).style().stroke_width(x_minor_tick_width_);
    image.get_g_element(detail::PLOT_Y_MINOR_TICKS).style().stroke_color(black);
    image.get_g_element(detail::PLOT_Y_MAJOR_TICKS).style().stroke_color(black);
    image.get_g_element(detail::PLOT_Y_MAJOR_TICKS).style().stroke_width(y_major_tick_width_);
    image.get_g_element(detail::PLOT_Y_MINOR_TICKS).style().stroke_width(y_minor_tick_width_);
    image.get_g_element(detail::PLOT_X_MAJOR_GRID).style().stroke_width(y_major_grid_width_);
    image.get_g_element(detail::PLOT_X_MAJOR_GRID).style().stroke_color(cyan);
    image.get_g_element(detail::PLOT_X_MINOR_GRID).style().stroke_width(y_minor_grid_width_);
    image.get_g_element(detail::PLOT_X_MINOR_GRID).style().stroke_color(cyan);

    set_ids();
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
  { // transform both x and y.
    transform_point(pt.first, pt.second);
  }

  void calculate_plot_window()
  { // The plot window is used to set a clip path.
    // Start by using all the image.
    plot_x1 = 0; // left
    plot_y1 = 0; // top
    plot_x2 = image.x_size(); // right
    plot_y2 = image.y_size(); // bottom

    if(use_x_label)
    { // Leave space at bottom for X axis label.
      plot_y2 -= (int)(x_label_info.font_size() * text_margin);
    }
    if(use_y_label)
    { // Leave space at left for Y axis label.
      plot_x1 += (int)(y_label_info.font_size() * text_margin);
    }
    if(use_title)
    { // Leave space at top for title.
      // TODO what if want to put title at bottom?
      plot_y1 += (int)(title_font_size() * (text_margin +1));
    }
    if(use_plot_window)
    { // Reduce to allow for legend, axes ticks to be outside plot_window.
      // Give the plot window a small border.
      // Needed to allow any window border rectangle to show OK?
      plot_x1+= border_margin; // pixels.
      plot_x2-= border_margin;
      plot_y1+= border_margin;
      plot_y2-= border_margin;

      if(use_legend)
      { // Space for legend at right.
        // TODO what if want legend elsewhere?
        plot_x2 -= 150;  // This 150 is an arbitrary legend box width.
        // TODO size could depend on font_size & length of text?
      }
      if(use_left_ticks)
      { // Reduce plot to give space for any external left ticks.
        plot_x1 +=
          y_major_tick_length_ > y_minor_tick_length_ ? y_major_tick_length_ : y_minor_tick_length_;
      }
      if(use_down_ticks)
      { // Reduce plot to give space for any external down ticks.
        plot_y2 -=
          x_major_tick_length_ > x_minor_tick_length_ ? x_major_tick_length_ : x_minor_tick_length_;
      }
      // Draw plot window rectangle with background and/or border.
      image.get_g_element(detail::PLOT_PLOT_BACKGROUND).push_back(
        new rect_element(plot_x1, plot_y1, (plot_x2 - plot_x1), plot_y2 - plot_y1));
    } // use_plot_window
  } // calculate_plot_window

  void draw_y_minor_ticks(double value, path_element& tick_path, path_element& grid_path)
  {
    double x1(0.);
    double y1(value); // for tick position and value label.
    transform_y(y1);
    double x2(image.y_size());

    if(use_y_minor_grid)
    { // Draw the minor grid, if wanted.
      if(!use_plot_window)
      { // Use whole image, but make space for title & labels text.
        //if(use_title)
        //{ // If title at top, should make no difference, unlike X grid.
        //  x2 -= title_info.font_size() * text_margin;
        //}
        if(use_y_label)
        { // TODO ???? Don't draw over the Y axis label?
          x1 += y_label_info.font_size() * text_margin;
          //x2 -= y_label_info.font_size() * text_margin;
        }
      }
      else
      { // Use plot window.
        x1 = plot_x1 + 1; // TODO what is the +1 for?
        x2 = plot_x2 - 1; // Ensure *inside* window?
      }
      if((y1 > plot_y1) && (x1 > plot_x1) && (x2 < plot_x2) )
      { // // Make sure that we are drawing inside the allowed window.
        grid_path.M(x1, y1).L(x2, y1);
      }
      // TODO else just ignore outside plot window?
    }

    if(use_y_ticks_on_plot_window_)
    { // Put y minor ticks on the plot window border.
      // TODO this can be done slicker?
      x1 = plot_x1; // On the 
      x2 = plot_x1;
      if(use_left_ticks)
      {
        x1 -= y_minor_tick_length_;
      }
      if(use_right_ticks)
      {
        x2 += y_minor_tick_length_;
      }
    }
    else
    { // Internal style.
      x1 = y_axis; // On the Y-axis line.
      x2 = y_axis;
      if(use_left_ticks)
      { 
        x1 -= y_minor_tick_length_;
      }
      if(use_right_ticks)
      { // Right ticks.
        x2 += y_minor_tick_length_;
      }
    }
    if((x1 > plot_x1) && (x2 < plot_x2) && (y1 < plot_y2) && (y1 > plot_y1))
    { // Make sure that we are drawing inside of the allowed plot window.
      tick_path.M(x1, y1).L(x2, y1); // Draw the tick.
    }
    else
    {// Do nothing?  warn?
    }
  } // void draw_y_minor_ticks

  void draw_y_major_ticks(double value, path_element& tick_path, path_element& grid_path)
  { // Draw Y axis major ticks.
    double y1(value);
    transform_y(y1); // cartesian to SVG coordinates.
    double x1(0.); // left end of tick.
    double x2(image.y_size()); // right end of tick.

    if(use_y_major_grid)
    { // Draw horizontal major grid line.
      if(!use_plot_window)
      { // TODO is this right?
        //if (use_title())
        //{ // title has no effect if at the top.
        //  y1 += title_info.font_size() * text_margin;
        //}
        if(use_y_label)
        { // Start further right to give space for y axis value label.
          y1 -= y_label_info.font_size() * text_margin ;
        }
        if(use_left_ticks)
        { // And similarly for left ticks.
          y1 -= y_major_tick_length_;
        }
      }
      else
      { // use_plot_window
        x1 = plot_x1 + 1;
        x2 = plot_x2 - 1;
      }
      grid_path.M(x1, y1).L(x2, y1);
    } // use_y_major_grid
   
    if((y1 < plot_y2) && (y1 > plot_y1))
    { // Make sure that we are drawing inside the allowed window.
      double y_tick_length = y_major_tick_length_;
      if(use_y_ticks_on_plot_window_) // (was external_style)
      { // Start ticks on the plot window border.
        x1 = plot_x1; // x1 = left,
        x2 = plot_x1; //  x2 = right.
        if (use_left_ticks)
        {
          x1 -= y_tick_length; // left a tick.
        }
        if (use_right_ticks)
        {
          x2 += y_tick_length; // right.
        }
      }
      else
      { // Internal style on Y-axis.
        x1 = y_axis; // Y-axis line.
        x2 = y_axis;
        if(use_left_ticks)
        {
          x1 -= y_tick_length; // left
        }
        if (use_right_ticks)
        {
          x2 += y_tick_length; // right.
        }
      }
      tick_path.M(x1, y1).L(x2, y1); // Draw the major tick.

      if(use_y_major_labels)
      { // label the tick with value, for example "1.3"
        std::stringstream fmt;
        // TODO precision(?) - will be some ugly precision(6) is we do nothing.
        fmt << value; // fmt.str() == "20"
        if(use_y_ticks)
        {
          x1 -= text_margin; // Or a font height?
          if(use_left_ticks)
          {
            x1 -= y_major_tick_length_; // left, taking account of tick length.
          }
          else
          { // No need to move if right tick, or none?
          }
        }
        if(!use_x_ticks_on_plot_window_ && (value != 0))
        { // TODO orientation control?
          image.get_g_element(detail::PLOT_PLOT_LABELS).text(x1, 
            y1 + y_label_info.font_size() / 2, // shift down to center value digits on tick.
            fmt.str(), y_label_info.font_size(), "", "", "", "", "", center_align, horizontal);
        }
        if(use_y_ticks_on_plot_window_)
        {
          image.get_g_element(detail::PLOT_PLOT_LABELS).text(x1 + y_label_info.font_size(), 
            y1 + y_label_info.font_size() /2, // shift down to center digit on tick.
            fmt.str(), y_label_info.font_size(), "", "", "", "", "", center_align, upward); // -90 degree rotation.
        }
      } // if(use_y_major_labels)
    }
  } // draw_y_major_ticks

  void draw_y_axis()
  {
    double x1(0.);
    transform_x(x1);
    // Draw the vertical Y axis line (at cartesian x = 0).
    image.get_g_element(detail::PLOT_Y_AXIS).line(x1, plot_y1, x1, plot_y2);
    // <g id="yAxis" stroke="rgb(0,0,0)"><line x1="70.5" y1="53" x2="70.5" y2="357"/>
    y_axis = x1; // in SVG coordinates.
    // Access the paths for the ticks & grids, ready for additions.
    path_element& minor_tick_path = image.get_g_element(detail::PLOT_Y_MINOR_TICKS).path();
    path_element& major_tick_path = image.get_g_element(detail::PLOT_Y_MAJOR_TICKS).path();
    path_element& minor_grid_path = image.get_g_element(detail::PLOT_Y_MINOR_GRID).path();
    path_element& major_grid_path = image.get_g_element(detail::PLOT_Y_MAJOR_GRID).path();

    if(use_y_axis_lines_)
    { // TODO what does this do?
      image.get_g_element(detail::PLOT_Y_AXIS).line(plot_y1, x_axis, plot_x2, x_axis);
      // <line x1="53" y1="-9.26e+061" x2="345" y2="-9.26e+061"/></g>
      // So x_axis is undefined!!!
    }

    // y_minor_jump is the interval between minor ticks.
    double y_minor_jump = y_major / ((double)(y_num_minor_ticks_ + 1.) );
    // TODO Problem here with using floating point?
    // Was i < y_max; but didn't show the tick and value at y_max so now i <= y_max;
    // But may still fail if a ls or few bits out? Seems to fail for y = 100, for example.

    // Draw the ticks on the positive side.
    for(double i = 0; i <= y_max; i += y_major)
    {
      for(double j = i + y_minor_jump; j < i + y_major; j += y_minor_jump)
      { // Draw minor ticks.
        // This will output 'orphaned' minor ticks that are beyond the plot window,
        // if the last major tick does not coincide with the plot window.
        // These are just ignored in draw_x_minor_ticks.
        // There might be 9 of them,
        // if you have the common 9 minor tick between major ticks!
        // TODO this seems ugly - as does the negative ones below.
        draw_y_minor_ticks(j, minor_tick_path, minor_grid_path);
      }
      draw_y_major_ticks(i, major_tick_path, major_grid_path);
    }

    // Draw the ticks on the negative side.
    for(double i = 0; i >= y_min; i -= y_major)
    {
      for(double j=i; j > i-y_major; j-= y_major / (y_num_minor_ticks_ + 1))
      { // Draw minor ticks.
        draw_y_minor_ticks(j, minor_tick_path, minor_grid_path);
      }
      draw_y_major_ticks(i, major_tick_path, major_grid_path);
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
      text_element(y_label_info.font_size() * text_margin, // shift right
      (plot_y2 + plot_y1) / 2., // center on the plot window.
      label, // "Y-Axis" for example.
       y_label_info.font_size(),
       y_label_info.font_family(),
       "", "", "", "", center_align,
      upward)); // Y label must be drawn vertically.
  } // draw_y_label

  void draw_bezier_lines(const svg_2d_plot_series& series)
  {
    g_element& g_ptr = image.get_g_element(detail::PLOT_PLOT_LINES).add_g_element();
    g_ptr.clip_id(plot_window_clip);
    g_ptr.style().stroke_color(series.line_style.color);
    path_element& path = g_ptr.path();

    std::pair<double, double> n_minus_2;
    std::pair<double, double> n_minus_1, n;
    std::pair<double, double> fwd_vtr;
    std::pair<double, double> back_vtr;

    if(series.line_style.area_fill == blank)
    {
      path.fill = false;
    }
    else
    { // fill
      path.style().fill_color(series.line_style.area_fill);
    }

    if(series.series.size() > 2)
    {
      std::multimap<double, double>::const_iterator iter = series.series.begin();

      n_minus_1 = *(iter++);
      n = *(iter++);
      transform_pair(n_minus_1);
      transform_pair(n);
      path.M(n_minus_1.first, n_minus_1.second);

      for(; iter != series.series.end(); ++iter)
      {
        n_minus_2 = n_minus_1;
        n_minus_1 = n;
        n = *iter;
        transform_pair(n);

        back_vtr.first = ((n_minus_1.first - n.first) +
          (n_minus_2.first - n_minus_1.first)) * 0.2; // TODO why + 0.2

        back_vtr.second = ((n_minus_1.second - n.second) +
          (n_minus_2.second - n_minus_1.second)) * 0.2;

        // 8.3.6 The cubic Bézier curve commands path.S(x, y).
        path.S(n_minus_1.first + back_vtr.first, n_minus_1.second + back_vtr.second,
          n_minus_1.first, n_minus_1.second);
      }

      back_vtr.first = 0;
      back_vtr.second = (n.second - n_minus_1.second)* 0.2;
      path.S(n.first + back_vtr.first, n.second + back_vtr.second,
        n.first, n.second);
    }
    else
    { // Only one or two points, so no curving possible!
      draw_straight_lines(series);
    }
  } // draw_bezier_lines

  void draw_straight_lines(const svg_2d_plot_series& series)
  { // Straight line between data points (rather than a Bezier curve).
    double prev_x;
    double prev_y;
    double temp_x(0.);
    double temp_y;

    g_element& g_ptr = image.get_g_element(detail::PLOT_PLOT_LINES).add_g_element();
    g_ptr.clip_id(plot_window_clip);
    g_ptr.style().stroke_color(series.line_style.color);
    path_element& path = g_ptr.path();
    if(series.series.size() > 1)
    {
      std::multimap<double, double>::const_iterator j = series.series.begin();
      prev_x = (*j).first;
      prev_y = 0.;

      // If we have to fill the area under the plot, 
      // we first have to move from the X-axis (y = 0) to the first point,
      // and again at the end after the last point.
      transform_point(prev_x, prev_y); // TODO 0. clearer here.
      if(series.line_style.area_fill != blank)
      { // Move to 1st point.
        path.style().fill_color(series.line_style.area_fill);
        path.M(prev_x, prev_y);
      }

      transform_y(prev_y = (*j).second);
      if(series.line_style.area_fill != blank)
      { // fill wanted.
        path.style().fill_color(series.line_style.area_fill);
        path.L(prev_x, prev_y); // Line from X-axis to 1st point.
      }
      else
      { // fill == blank
        path.M(prev_x, prev_y);
      }
      ++j; // refers to 2nd point to plot.

      for(; j != series.series.end(); ++j)
      {
        temp_x = (*j).first;
        temp_y = (*j).second;
        transform_point(temp_x, temp_y);
        path.L(temp_x, temp_y); // line to next point.

        if(series.line_style.area_fill == blank)
        {
          path.M(temp_x, temp_y);
        }
        prev_x = temp_x;
        prev_y = temp_y;
      } // for j

      if(series.line_style.area_fill != blank)
      { // fill wanted.
        transform_y(temp_y = 0.); // X-axis
        path.L(temp_x, temp_y).z(); // terminate line.
      }
    }
  } // draw_straight_lines

  void draw_plot_lines()
  { // Draw line through data series, curved or straight.
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

    // Draw imagebackground (perhaps with border and/or fill color).
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

    image.get_g_element(detail::PLOT_PLOT_POINTS).clip_id(plot_window_clip);
    // Draw axes, labels & legend, as required.
    if(use_axis)
    {
      draw_y_axis(); // TODO is draw_axes used?
      draw_x_axis();
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
      g_element& g_ptr = image.get_g_element(detail::PLOT_PLOT_POINTS).add_g_element();

      g_ptr.style()
        .fill_color(series[i].point_style.fill_color)
        .stroke_color(series[i].point_style.stroke_color);

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
        draw_plot_point(x, y, g_ptr, plot_point_style(blank, blank, series[i].point_style.size, cone));
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

  svg_2d_plot& y_name_on(bool cmd)
  {
    use_y_label = cmd;
    return *this;
  }

  svg_2d_plot& y_major_labels_on(bool cmd)
  {
    use_y_major_labels = cmd;
    return *this;
  }

  svg_2d_plot& y_axis_color(const svg_color& col)
  { // Set BOTH stroke and fill to the same color.
    image.get_g_element(detail::PLOT_Y_AXIS).style().fill_color(col);
    image.get_g_element(detail::PLOT_Y_AXIS).style().stroke_color(col);
    return *this;
  }

  svg_color y_axis_color()
  { // But only return the stroke color.
    return image.get_g_element(detail::PLOT_Y_AXIS).style().stroke_color();
  }

  svg_2d_plot& y_axis_label_color(const svg_color& col)
  { // Set BOTH stroke and fill to the same color.
    image.get_g_element(detail::PLOT_Y_LABEL).style().fill_color(col);
    image.get_g_element(detail::PLOT_Y_LABEL).style().stroke_color(col);
    return *this;
  }

  svg_color y_axis_label_color()
  { // But only return the stroke color.
    return image.get_g_element(detail::PLOT_Y_LABEL).style().stroke_color();
  }

  svg_2d_plot& y_major_grid_color(const svg_color& col)
  {
    image.get_g_element(detail::PLOT_Y_MAJOR_GRID).style().stroke_color(col);
    return *this;
  }

  svg_2d_plot& y_minor_grid_color(const svg_color& col)
  {
    image.get_g_element(detail::PLOT_Y_MINOR_GRID).style().stroke_color(col);
    return *this;
  }

  svg_2d_plot& y_major_tick_color(const svg_color& col)
  {
    image.get_g_element(detail::PLOT_Y_MAJOR_TICKS).style().stroke_color(col);
    return *this;
  }

  svg_2d_plot& y_minor_tick_color(const svg_color& col)
  {
    image.get_g_element(detail::PLOT_Y_MINOR_TICKS).style().stroke_color(col);
    return *this;
  }

  svg_2d_plot& y_range(double y1, double y2)
  {
    y_min = y1;
    y_max = y2;
    if(y2 <= y1)
    {
      throw std::runtime_error("Illegal Argument: X scale: x2 < x1");
    }
    return *this;
  }

  svg_2d_plot& y_axis_width(unsigned int width)
  {
    // TODO should be stored elsewhere too?
    image.get_g_element(detail::PLOT_Y_AXIS).style().stroke_width(width);
    return *this;
  }

  svg_2d_plot& y_major_interval(double inter)
  {
    y_major = inter;
    return *this;
  }

  double y_major_interval()
  {
    return y_major;
  }

  svg_2d_plot& y_major_tick_length(unsigned int length)
  {
    y_major_tick_length_ = length;
    return *this;
  }

  unsigned int y_major_tick_length()
  {
    return y_major_tick_length_;
  }

  svg_2d_plot& y_minor_tick_length(unsigned int length)
  {
    y_minor_tick_length_ = length;
    return *this;
  }

  unsigned int y_minor_tick_length()
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

  svg_2d_plot& y_name(const std::string& str)
  { // Label for Y-axis.
    y_label_info.text(str);
    return *this;
  }

  const std::string& y_name()
  {
    return y_label_info.text();
  }

  svg_2d_plot& y_major_tick_width(unsigned int width)
  {
    y_major_tick_width_ = width;
    image.get_g_element(detail::PLOT_Y_MAJOR_TICKS).style().stroke_width(width);
    return *this;
  }

  unsigned int y_major_tick_width()
  {
    return y_major_tick_width_;
  }

  svg_2d_plot& y_minor_tick_width(unsigned int width)
  {
    y_minor_tick_width_ = width;
    image.get_g_element(detail::PLOT_Y_MINOR_TICKS).style().stroke_width(width);
    return *this;
  }

  unsigned int y_minor_tick_width()
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
    use_y_major_grid = is;
    return *this;
  }

  bool y_major_grid_on()
  {
    return use_y_major_grid;
  }

  svg_2d_plot& y_minor_grid_on(bool is)
  {
    use_y_minor_grid = is;
    return *this;
  }

  bool y_minor_grid_on()
  {
    return use_y_minor_grid;
  }

  svg_2d_plot& y_label_on(bool cmd)
  {
    use_y_label = cmd;
    return *this;
  }

  svg_2d_plot& y_label_font_size(unsigned int i)
  { // TODO May be best to tie these two font sizes together?
    y_label_info.font_size(i);
    // y_units_info.font_size(i);
    return *this;
  }

  unsigned int y_label_font_size()
  {
    return y_label_info.font_size();
  }

  svg_2d_plot& y_label_font_family(const std::string& family)
  {
    y_label_info.font_family(family);
    return *this;
  }

  const std::string& y_label_font_family()
  {
    return y_label_info.font_family();
  }

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
    (area_fill_color, (svg_color_constant), blank)
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

    plot_line_style line_style(line_color, line_on, bezier_on);

    line_style.area_fill=area_fill_color;

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

#if defined (BOOST_MSVC)
#  pragma warning(pop)
#endif

  } // namespace svg
} // namespace boost

#endif // BOOST_SVG_SVG_2D_PLOT_HPP
