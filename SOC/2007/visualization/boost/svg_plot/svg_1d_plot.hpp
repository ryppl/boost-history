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

//#if defined (BOOST_MSVC)
//#  pragma warning (disable: 4005) // 'BOOST_PARAMETER_MAX_ARITY' : macro redefinition
//#endif

#define BOOST_PARAMETER_MAX_ARITY 12

#include <boost/bind.hpp>

#if defined (BOOST_MSVC)
#  pragma warning (push)
#  pragma warning (disable: 4512) // "assignment operator could not be generated."
#  pragma warning (disable: 4180) // qualifier applied to function type has no meaning; ignored
#endif

// Added disable warning 4180 in Boost.Parameter Sep 2007.
// See also pbristow trac ticket #1097 complaining about this ;-)
// trailing const qualifiers are said to be meaningless.

#include <boost/parameter/preprocessor.hpp>
#include <boost/parameter/name.hpp>
#include <boost/iterator/transform_iterator.hpp>

#include "svg.hpp"
#include "svg_style.hpp"
#include "detail/axis_plot_frame.hpp"
#include "detail/functors.hpp"

#if defined (BOOST_MSVC)
#  pragma warning(pop)
#endif

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
  #if defined (BOOST_MSVC)
  #  pragma warning(push)
  #  pragma warning (disable: 4512) // "assignment operator could not be generated."
  #  pragma warning (disable: 4180) // qualifier applied to function type has no meaning; ignored
  #endif

  // -----------------------------------------------------------------
  // Parameter names for plot() function
  // -----------------------------------------------------------------
  #ifndef BOOST_SVG_BOOST_PARAMETER_NAMES
  #define BOOST_SVG_BOOST_PARAMETER_NAMES
      BOOST_PARAMETER_NAME(my_plot)
      BOOST_PARAMETER_NAME(container)
      BOOST_PARAMETER_NAME(title)
      BOOST_PARAMETER_NAME(stroke_color)
      BOOST_PARAMETER_NAME(fill_color)
      BOOST_PARAMETER_NAME(point_style)
      BOOST_PARAMETER_NAME(size)
      BOOST_PARAMETER_NAME(x_functor)
  #endif

  #if defined (BOOST_MSVC)
  #  pragma warning(pop)
  #endif

// -----------------------------------------------------------------
// This allows us to store plot state locally in svg_plot. We don't
// store it in "svg" because transforming the points after they are
// written to the document would be difficult. We store the Cartesian
// coordinates locally and transform them before we write them.
// -----------------------------------------------------------------
struct svg_plot_series
{
  std::vector<double> series; // Normal 'OK to plot' data values.
  std::vector<double> series_limits; // 'limit' values: too big, too small or NaN.

  std::string title; // title of data series (to show on legend).
  plot_point_style point_style; // circle, square...

  // -------------------------------------------------------------
  // Scan each data point between the iterators that are passed,
  // sorting them into the correct std::vector, normal or not.
  // -------------------------------------------------------------
  template <class T>
  svg_plot_series(T begin, T end, const std::string& title, const plot_point_style& style)
    :
  title(title),
  point_style(style)
  // No line style for 1-D, only for 2-D.
    // No defaults for begin and end.
  {
    for(T i = begin; i != end; ++i)
    {
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
}; //     struct svg_plot_series()

class svg_1d_plot : public detail::axis_plot_frame<svg_1d_plot>
{
 protected:
  // Stored so as to avoid rewriting style information constantly.
  svg image;

  text_element title_info; // Title of whole plot.
  text_element legend_header; // legend box header (if any).
  text_element x_label_info; // Example: "length of widget"
  text_element x_units_info; // For example, to display, "length (meter)"
  // No Y-axis info for 1D.
  // Note that text_elements hold font_size.

  // Border information for the plot window (not the full image size).
  // Allows for title and legend to be separate.
  // Initially set to the width and height of the image.
  // TODO Should this be unsigned int??? to be consistent.
  int plot_x1; // calculate_plot_window() sets these values.
  int plot_y1;
  int plot_x2;
  int plot_y2;

  // X-Axis information.
  unsigned int x_major_tick_length_; // pixels.
  unsigned int x_major_tick_width_; // pixels.
  unsigned int x_minor_tick_length_; // pixels.
  unsigned int x_minor_tick_width_; // pixels.
  // Avoid name clash with x_m*_tick_length and width.
  unsigned int x_num_minor_ticks_; // number of minor ticks, eg 4 gives major 0, minor 1,2,3,4, major 5
  // grid width is also set in style.stroke_width()
  // TODO Not sure it is wise to store in two places?
  unsigned int x_major_grid_width_; // pixels.
  unsigned int x_minor_grid_width_; // pixels.
  //unsigned int x_major_label; // pixels. Now in x_units_info font size

  // Other information.
  double text_margin; // Marginal space around text items like title,
  // as a fraction of font size, (pixels) (was fixed at 1.5).
  int border_margin; // Marginal (pixels) space around the plot border.

  // X-Axis information.
  // (Y_axis stored as one point because this is a 1D graph).
  double x_min; // minimum x (Cartesian units).
  double x_max; // maximum x  (Cartesian units).
  double x_axis; // stroke width (pixels). ????
  double x_major; // Interval (Cartesian units) between major ticks.
    // set/get by x_major_interval
  double x_minor; // Interval (Cartesian units) between minor ticks.
  // because x_num_minor_ticks_ used to determine this instead,
  // but one could calculate x_minor.

  // Yes/no options.
  bool use_title; // Show plot title.
  bool use_legend; // Show legend box.
  bool use_plot_window; // rather than whole image.
  bool use_x_axis_lines_; // = x_axis_on()
  bool use_y_axis_lines_; // Note: is needed in 1D version too in draw_axes.
  bool use_x_major_labels; // For example, to show value (like 1.2) by ticks.
  bool use_x_label_units;  // For example, to show, "length (meter)"
  bool use_x_major_grid; // provide major vertical lines.
  bool use_x_minor_grid;// provide minor vertical lines.
  bool use_x_label; // Show label text for the axis, like "X-axis"
  bool use_up_ticks; // ticks only up from Y = 0 horizontal X-axis.
  bool use_down_ticks;// ticks only down from Y = 0 horizontal X-axis.
  //bool use_left_ticks; // only for 2D.
  //bool use_right_ticks;
  bool use_x_ticks; // = use_up_ticks || use_down_ticks
  // bool use_y_ticks; // = use_left_ticks || use_right_ticks only for 2-D
 // Note: can have ticks both up and down (the original style).
  bool use_x_ticks_on_plot_window_; // rather than on Y = 0 horizontal axis.
  bool use_line; // set by line_on(bool); // Not really useful for 1-D,
  // TODO but needs Boost-Parameter removed to do properly.

private:
  friend class detail::axis_plot_frame<svg_1d_plot>;

  // Where we will be storing the data points for transformation.
  std::vector<svg_plot_series> series;
  // These are sorted into two vectors for normal and abnormal (max, inf and NaN).

  double x_scale; // Used for transform from Cartesian to SVG coordinates.
  double x_shift; // SVG origin is top left, Cartesian is bottom right.
  double y_scale;
  double y_shift;

  // Public member functions, defined below.
  // void calculate_transform(); //
  // void draw_axes();
  // void calculate_plot_window(); // plot_x, plot_y...
  // void update_image();
  // void set_ids();

public:
  svg_1d_plot() : // Default constructor.
    // Many ( but not all - see below) default values here.
    title_info(0, 0, "Plot of data", 20, "Verdana", "", "", "", "", center_align, horizontal),
    legend_header(0, 0, "Legend", 16, "Arial", "", "", "", "", center_align, horizontal),
    x_label_info(0, 0, "X Axis", 14, "Lucida Sans Console", "", "", "", "", center_align, horizontal), //
    x_units_info(0, 0, "(units)", 14, "Lucida Sans Console", "", "", "", "", right_align, horizontal),
    //x_units_info(0, 0, "(units)", 14, "Times New Roman", "italic", "bold", "wider", "underline", right_align, horizontal),
    text_margin(2.), // for text was 1.5 // as a fraction of the font size.
    border_margin(5), // Prevent plot window box begin right on edge.
    x_min(-10), x_max(10),
    use_legend(false),
    use_title(true),
    use_plot_window(false),
    use_x_label(false), // Label like "volume".
    use_x_label_units(false),
    use_x_major_labels(true), // Values on major ticks.
    x_major_grid_width_(3),
    x_minor_grid_width_(1),
    use_up_ticks(false),
    use_down_ticks(true),
    use_x_ticks_on_plot_window_(false), // was external_style
    // use_y_ticks_on_plot_window_(false), // was external_style
    use_x_ticks(true), // use_up_ticks || use_down_ticks
    // 2D has left and right ticks too.
    use_line(true),
    use_x_major_grid(false),
    // x_minor_grid_width(1),
    use_x_minor_grid(false),
    use_x_axis_lines_(true),
    use_y_axis_lines_(false), // Not needed for 1D, but leave at false.
    x_major(2), // interval between x major ticks
    x_major_tick_width_(3),
    x_minor_tick_width_(1),
    x_major_tick_length_(10), // If both up and down, total is twice this.
    x_minor_tick_length_(5), // If both up and down, total is twice this.
    x_num_minor_ticks_(2),
    // legend_title_size(12), TD remove - now in text_element
    x_scale(1.), x_shift(0),
    y_scale(1.), y_shift(0)
  // See documentation for default settings rationale.
  {
    image_size(500, 200); // Default image size.
    // Only need to be quite shallow (y_size) for a 1-D plot.
    // 200 barely leaves enough room for five data series in the legend).
    // (2-D usually needs to be much more rectangular).
    use_x_ticks = use_up_ticks || use_down_ticks;
    // Only 2D has left and right ticks.

    // Build the document tree & add all the children of the root node.
    for(int i = 0; i < detail::SVG_PLOT_DOC_CHILDREN; ++i)
    {
      image.add_g_element();
    }

    // Set stroke color & width defaults.
    image.get_g_element(detail::PLOT_BACKGROUND).style().fill_color(white);
    //image.get_g_element(detail::PLOT_Y_AXIS).style().stroke_color(black); TODO needed? Harmful?
    image.get_g_element(detail::PLOT_X_AXIS).style().stroke_color(black);
    image.get_g_element(detail::PLOT_X_MAJOR_TICKS).style().stroke_color(black).stroke_width(x_major_tick_width_);
    image.get_g_element(detail::PLOT_X_MINOR_TICKS).style().stroke_color(black).stroke_width(x_minor_tick_width_);
    image.get_g_element(detail::PLOT_X_MAJOR_GRID).style().stroke_color(blue); // TODO temporary cyan is good.
    image.get_g_element(detail::PLOT_X_MAJOR_GRID).style().stroke_width(x_major_grid_width_);
    image.get_g_element(detail::PLOT_X_MINOR_GRID).style().stroke_color(cyan);
    image.get_g_element(detail::PLOT_X_MINOR_GRID).style().stroke_width(x_minor_grid_width_);
    image.get_g_element(detail::PLOT_LIMIT_POINTS).style().stroke_color(lightgray).fill_color(whitesmoke);

    // Font info defaults are set by the constructor above.

    set_ids();
  } // svg_1d_plot()

  void calculate_transform()
  { // Calculate scale and shift factors for transform from Cartesian to plot.
    // SVG image is 0, 0 at top left, Cartesian at bottom left.
    x_scale = (plot_x2 - plot_x1) / (x_max - x_min);
    x_shift = plot_x1 - (x_min * (plot_x2 - plot_x1) / (x_max - x_min));
    y_scale = 1.;
    y_shift = plot_y1 - (plot_y1 - plot_y2) / 2.;
  } // void calculate_transform()

  void draw_axes()
  { // For 1-D, there is, of course, only the X-axis!
    double x(0.);
    double y1(0.);
    double y2(image.y_size());
    transform_x(x);
    // Draw origin, making sure it is in the plot window.
    if(use_x_axis_lines_ && (x > plot_x1) && (x < plot_x2))
    { // TODO >= and <= ? instead.
      if(!use_plot_window)
      { // Use whole image.
        if(use_title)
        { // Allow space for title, taking account of font size.
          y1 += title_info.font_size() * (text_margin +1);
        }
        if(use_x_label)
        {// Allow space for x tick values, taking account of font size.
          y2 -= x_label_info.font_size() * text_margin;
        }
      }
      else
      { // Use plot window.
        y1 = plot_y1;
        y2 = plot_y2;
      }
      image.get_g_element(detail::PLOT_X_AXIS).line(x, y1, x, y2);
    }
    draw_x_axis();
  } //  draw_axes()

  void calculate_plot_window()
  { // For 1-D
    x_axis = (plot_y2 + plot_y1) / 2.; // Put X-axis halfway up plot window.
    plot_x1 = 0; // Top left of image.
    plot_y1 = 0;
    plot_x2 = image.x_size(); // Bottom right of image.
    plot_y2 = image.y_size();

    if(use_plot_window)
    {
      plot_x1 += border_margin; // small margin around border.
      plot_x2 -= border_margin; //
      plot_y1 += border_margin; //
      plot_y2 -= border_margin;

      if(use_title)
      {
        // Allow a blank line, title lines and a line space.
        plot_y1 += title_font_size() * 2;
      }
      if(use_legend)
      { // Allow space for legend at right.
        plot_x2 -= 150;  // This 150 is an arbitrary legend box width.
        // TODO size could depend on font_size & length of text?
      }
      if(use_x_label)
      {// Allow space for x_label at bottom.
        plot_y2 -= static_cast<int>(x_label_info.font_size() * text_margin);
      }
      if(use_down_ticks)
      { // Allow space for the downward ticks
        // (so don't go into/over the border margin).
        // TODO use std::max here.
        plot_y2 -=
          (x_major_tick_length_ > x_minor_tick_length_) ?
            x_major_tick_length_ : x_minor_tick_length_;
      }
      // Draw plot window rect.
      image.get_g_element(detail::PLOT_PLOT_BACKGROUND).push_back(
        new rect_element(plot_x1, plot_y1, (plot_x2 - plot_x1), plot_y2 - plot_y1));
      // <g id="plotBackground" fill="rgb(248,248,255)"><rect x="43" y="53" width="302" height="304"/></g>
    } // use_plot_window
  } //  void calculate_plot_window()

  void update_image()
  {
    // Removes all elements that will show up in a subsequent draw.
    clear_all();

    // Draw plot background.
    image.get_g_element(detail::PLOT_BACKGROUND).push_back(
      new rect_element(0, 0, image.x_size(),  image.y_size()));

    calculate_plot_window();
    calculate_transform();
    draw_title(); // Call after above to the plot_x and y are defined.
    if(use_x_axis_lines_)
    {
      draw_axes();
    }
    if(use_legend)
    {
      draw_legend();
    }
    if(use_x_label)
    {
      draw_x_label();
    }
    double y(0.); // All 1-D points are plotted are on the y = 0 axis.
    transform_y(y);
    for(unsigned int i = 0; i < series.size(); ++i)
    { // For each of the data series.
      g_element& g_ptr = image.get_g_element(detail::PLOT_PLOT_POINTS).add_g_element();

      g_ptr.style().stroke_color(series[i].point_style.stroke_color);
      g_ptr.style().fill_color(series[i].point_style.fill_color);

      for(unsigned int j = 0; j < series[i].series.size(); ++j)
      { // Draw points for jth series.
        double x = series[i].series[j];
        transform_x(x);
        if( // Check point is inside plot_window (otherwise ignore it).
             (x > plot_x1) // TODO <=?
          && (x < plot_x2)
          && (y > plot_y1)
          && (y < plot_y2) )
        {
          draw_plot_point(x, y, g_ptr, series[i].point_style);
        }
      } // for j
    } // for
  } //   void update_image()

  void set_ids()
  { // document ids for use in <g id = "PLOT_TITLE".../>
    for(int i = 0; i < detail::SVG_PLOT_DOC_CHILDREN; ++i)
    {
      image.get_g_element(i).id(detail::document_ids[i]);
    }
  } //  void set_ids()

  // ------------------------------------------------------------------------
  // write() has two flavors, a file and a ostream.
  // The file version opens an ostream, and calls the stream version.
  // The stream version first clears all unnecessary data from the graph,
  // builds the document tree, and then calls the write function for the root
  // document node, which calls all other nodes through the Visitor pattern.
  // ------------------------------------------------------------------------

  svg_1d_plot& write(const std::string& filename)
  {
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
    // This will reduce .svg file sizes significantly for curves represented with many data points.
    // For example, if a curve is shown using 100 points,
    // reducing to coord_precision(3) from 6 will reduce file size by 300 bytes.
    image.write(s_out);
    return (svg_1d_plot&)*this;
  }

  //
  #if defined (BOOST_MSVC)
  #  pragma warning(push) // disable warnings in Boost.parameter
  // (delete this and pop below if not used in future.)
  #  pragma warning (disable: 4100) // 'args' : unreferenced formal parameter
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
    (stroke_color, (const svg_color&), svg_color(white))
    (point_style, (point_shape), round)
    (size, (int), 10)
    (x_functor, *, detail::boost_default_convert())
    )
    (deduced
    (optional
    (fill_color, (const svg_color&), svg_color(black))
    )
    )
    )
  {
    series.push_back(svg_plot_series(
      boost::make_transform_iterator(container.begin(), x_functor),
      boost::make_transform_iterator(container.end(),   x_functor),
      title,
      plot_point_style(stroke_color, fill_color, size, point_style)
      ));
  }

  #if defined (BOOST_MSVC)
  #  pragma warning(pop)
  #endif

}; // end svg_1d_plot

} // namespace svg
} // namespace boost


#endif // BOOST_SVG_SVG_1D_PLOT_HPP
