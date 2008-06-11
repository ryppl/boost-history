// svg_2d_plot.hpp

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2007, 2008

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_SVG_2D_PLOT_HPP
#define BOOST_SVG_SVG_2D_PLOT_HPP

#ifdef _MSC_VER
#pragma warning(push)
#  pragma warning (disable : 4800) // forcing value to bool 'true' or 'false' (performance warning)
#  pragma warning (disable : 4512) // assignment operator could not be generated
#endif

#include <boost/iterator/transform_iterator.hpp>
//  using boost::make_transform_iterator;

#include "svg_style.hpp"
#include "detail/axis_plot_frame.hpp"
#include "detail/numeric_limits_handling.hpp"
#include "detail/functors.hpp"
#include "detail/auto_axes.hpp"

#include "svg.hpp"

#include <map> // for map & multimap
#include <string>
#include <sstream>
#include <utility>
#include <ostream>
#include <iterator>
#include <exception>
#include <iostream> // for debug.

namespace boost
{
  namespace svg
  {
    // Forward declarations.
    const std::string strip_e0s(std::string s); // Strip unncessary zeros and e and sign.
    class svg_2d_plot; // Plot framework.
    class svg_2d_plot_series; // plot data series.

    // -----------------------------------------------------------------
    // This allows us to store plot state locally in svg_plot. We don't
    // store it in "svg" because transforming the points after they are
    // written to the document would be difficult. We store the Cartesian
    // coordinates locally and transform them before we write them.
    // -----------------------------------------------------------------

    class svg_2d_plot_series
    {

    friend svg_2d_plot_series;
    friend void draw_straight_lines(const svg_2d_plot_series&);

    public:
      // 2-D Data series points to plot.
      std::multimap<double, double> series; // Normal 'OK to plot' data values.
      std::multimap<double, double> series_limits; // 'limit' values: too big or small, or NaN.
      // multimap is used rather than vector of pairs because
      // multimap sorts and ensures that lines joining data points
      // are unaffected by the order in which data is presented.
      // (For 1-D a vector of doubles can be used).

      std::string title_; // Title of data series (to show on legend using legend style).
      plot_point_style point_style_; // circle, square...
      plot_point_style limit_point_style_; // Default is cone pointing down.
      plot_line_style line_style_;
      bar_style bar_style_;
      histogram_style histogram_style_;

      template <class T>  // T an STL container: for example: multimap.
      svg_2d_plot_series(T begin, T end, // of data series.
        std::string title = "") // Title of data series.
        :
        title_(title),
        // plot_point_style(const svg_color& fill = blank, const svg_color& stroke = black,
        // int size = 10, point_shape shape = round, const std::string& symbols = "X")
        point_style_(black, blank, 10, round), // Default point style.
        limit_point_style_(grey, blank, 10, cone), // Default limit (inf or NaN) point style.
        line_style_(black, blank, 2, true, false), // Default line style, no fill, width 2, line_on, bezier off.
        bar_style_(black, blank, 3, no_bar), // Default black, no fill, stick width 3, no bar.
        // -2 block to Y-axis, -1 stick to Y-axis, no_bar,  +1 stick to x_axis, -2 block to X-axis.
        histogram_style_(no_histogram)

      { // Constructor.
        for(T i = begin; i != end; ++i)
        { // Sort data points into normal and limited series.
          if(detail::pair_is_limit(*i))
          { // Either x and/or y is at limit.
            series_limits.insert(*i);
          }
          else
          { // Normal data values for both x and y.
            series.insert(*i);
          }
        }
      } // svg_2d_plot_series


      // Set functions for the plot series.
      svg_2d_plot_series& fill_color(const svg_color& col_)
      { // Point fill color.
        point_style_.fill_color_ = col_;
        return *this;
      }

      svg_2d_plot_series& stroke_color(const svg_color& col_)
      {
        point_style_.stroke_color_ = col_;
        return *this;
      }

      svg_2d_plot_series& shape(point_shape shape_)
      {
        point_style_.shape_ = shape_;
        return *this;
      }

      svg_2d_plot_series& size(int size_)
      {
        point_style_.size_ = size_;
        return *this;
      }

      svg_2d_plot_series& line_color(const svg_color& col_)
      {
        line_style_.stroke_color_ = col_;
        return *this;
      }

      svg_2d_plot_series& area_fill(const svg_color& col_)
      {
        line_style_.area_fill_ = col_;
        // Note that area_fill(false) will produce a *blank* color, and so NO FILL.
        // area_fill(blank) will produce the default non-blank color (black?).
        return *this;
      }

      svg_2d_plot_series& line_width(double wid_)
      {
        line_style_.width_ = wid_; // Sets legend line width too.
        return *this; // Make chainable.
      }

      svg_2d_plot_series& line_on(bool on_)
      {
        line_style_.line_on_ = on_;
        return *this; // Make chainable.
      }

      svg_2d_plot_series& bezier_on(bool on_)
      {
        line_style_.bezier_on_ = on_;
        return *this; // Make chainable.
      }

      svg_2d_plot_series& bar(bar_option opt_)
      {
        bar_style_.bar_option_ = opt_;
        return *this; // Make chainable.
      }

      svg_2d_plot_series& bar_width(double wid_)
      {
        bar_style_.width_ = wid_;
        return *this; // Make chainable.
      }

      svg_2d_plot_series& bar_color(const svg_color& col)
      {
        bar_style_.color_ = col;
        return *this; // Make chainable.
      }

      svg_2d_plot_series& bar_area_fill(const svg_color& col)
      {
        bar_style_.area_fill_ = col;
        return *this; // Make chainable.
      }

      // Get functions for the plot series.
      plot_line_style line_style()
      {
        return line_style_;
      }

      double line_width()
      {
        return line_style_.width_;
      }

      bool bezier_on()
      {
        return line_style_.bezier_on_;
      }

      bool line_on()
      {
        return line_style_.line_on_;
      }

      svg_color& line_color()
      {
        return line_style_.stroke_color_;
      }

      svg_color& area_fill()
      {
        return line_style_.area_fill_;
      }

      int size()
      {
        return point_style_.size_;
      }

      point_shape shape()
      {
        return point_style_.shape_;
      }

      bar_option bar_option()
      {
        return bar_style_.bar_option_;
      }

      double bar_width()
      {
        return bar_style_.width_;
      }

      svg_color& bar_color()
      {
        return bar_style_.color_;
      }

      svg_color& bar_area_fill()
      {
        return bar_style_.area_fill_;
      }

      svg_2d_plot_series& histogram(histogram_option opt_)
      { // column = -1, // Bar or row line (stroke width) horizontal to Y-axis.
        // no_histogram = 0,
        // bar = +1 // Stick or column line (stroke width) vertical to X-axis.
        histogram_style_.histogram_option_ = opt_;
        return *this; // Make chainable.
      }

   }; // class svg_2d_plot_series

    class svg_2d_plot : public detail::axis_plot_frame<svg_2d_plot>
    { // See also svg_1d_plot.hpp for 1-D version.
     friend void show_plot_settings(svg_2d_plot&);
     friend svg_2d_plot_series;
     friend class detail::axis_plot_frame<svg_2d_plot>;
     // axis_plot_frame.hpp contains functions common to 1 and 2-D.

    private:
      // Member data names conventionally end with _,
      // for example: border_margin_,
      // and set & get accessor functions are named without _ suffix,
      // for example: border_margin() & border_margin(int).

      double x_scale_; // Use by function transform()
      double x_shift_; // to go from Cartesian to svg coordinates.
      double y_scale_;
      double y_shift_;

      // Stored so as to avoid rewriting style information constantly.
      svg image;

      double text_margin_; // Marginal space around text items like title,
      // text_margin_ * font_size to get distance in svg units.

      text_style a_style_; // Defaults.
      // text_style contains font size & type etc.
      text_style title_style_;
      text_style legend_style_;
      text_style x_axis_label_style_;
      text_style x_value_label_style_;
      text_style y_axis_label_style_;
      text_style y_value_label_style_;
      text_style point_symbols_style_; // Used for data point marking.
      text_element title_info_; // Plot title.
      text_element legend_header_; // legend box header or title (if any).
      text_element x_label_info_; // For example: "length"
      text_element x_label_value_; // For example: "1.2" or "1.2e+001"
      text_element y_label_info_; // For example: "volume"
      text_element x_units_info_; // For example: "mm"
      text_element y_units_info_; // 2-D only.
      text_element y_label_value_; // For example: "1.2" or "1.2e+001"

      text_style value_style_; // Used for data point value label.
      value_style x_values_style_; // Data point X value marking.
      value_style y_values_style_; // Data point Y value marking.
      //bool x_plusminus_on_; // http://en.wikipedia.org/wiki/Plus-minus_sign
      //// Unicode &#0xB1; HTML &plusmn;


      //rotate_style y_value_label_rotation_; // Direction point Y value labels written.
      //int y_value_precision_;
      //std::ios_base::fmtflags y_value_ioflags_;

      box_style image_border_; // rectangular border of all image width, color...
      box_style plot_window_border_; // rectangular border of plot window width, color...
      box_style legend_box_; // rectangular box of legend width, color...

 // TODO doubles also could be float?
      double plot_left_;
      double plot_right_;
      double plot_top_;
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
      ticks_labels_style y_ticks_;

      bool title_on_; // Provide a title for the whole plot.
      bool legend_on_; // Provide a legend box.
      bool outside_legend_on_; // Legend box outside the plot window.
      bool legend_lines_; // get/set by legend_lines(bool); data colored line type in legend box.
      bool plot_window_on_; // Use a separate plot window (not whole image).
      bool x_ticks_on_; // TODO check these are really useful.
      bool y_ticks_on_;
      bool x_values_on_; // If values of X data are shown.
      bool y_values_on_; // If values of Y data are shown.
      bool xy_values_on_; // If values of X & Y pairs are shown.

      int x_axis_position_;
      int y_axis_position_;

      bool autoscale_check_limits_; // Whether to check autoscale values for infinity, NaN, max, min.
      bool x_autoscale_; // Whether to use any X-axis autoscale values.
      bool x_include_zero_; // If autoscaled, include zero.
      int  x_min_ticks_;  // If autoscaled, set a minimum number of X ticks.
      double x_tight_;
      int  x_steps_;  // If autoscaled, set any prescaling to decimal 1, 2, 5, 10 etc.

      // Values calculated by scale_axis, and is used only if x_autoscale == true.
      double x_auto_min_value_;
      double x_auto_max_value_;
      double x_auto_tick_interval_; // tick major interval.
      int x_auto_ticks_; // Number of ticks.

      bool y_autoscale_; // Whether to use any y_axis autoscale values.
      bool y_include_zero_; // If autoscaled, include zero.
      int  y_min_ticks_;  // If autoscaled, set a minimum number of Y ticks.
      double y_tight_;
      int  y_steps_;  // If autoscaled, set any prescaling to decimal 1, 2, 5, 10 etc.

        // Values calculated by scale_axis, and is used only if y_autoscale == true.
      double y_auto_min_value_;
      double y_auto_max_value_;
      double y_auto_tick_interval_; // tick major interval.
      int y_auto_ticks_; // Number of ticks.

      // Where we will be storing the data points (series) for transformation.
      std::vector<svg_2d_plot_series> series; // Defined above.

      std::string plot_window_clip_; // = "clip_plot_window" id for clippath
      // http://www.w3.org/TR/SVG/masking.html#ClipPathElement 14.1 Introduction
      // clipping paths, which uses any combination of 'path', 'text' and basic shapes
      // to serve as the outline where everything on the "inside" of the outline
      // is allowed to show through but everything on the outside is masked out.
      // So the plot_window_clip_ limits display to a plot_window rectangle.

    public: // of class svg_2d_plot: public detail::axis_plot_frame<svg_2d_plot>

      svg_2d_plot() // Constructor, including all the very many default plot options,
        // some of which use some or all of the class defaults.
        :
        // See documentation for default settings rationale.
        // text_styles:
        title_style_(18, "Verdana", "", ""),  // last "bold" ?
        legend_style_(14, "Verdana", "", ""), // 2nd "italic"?
        x_axis_label_style_(14, "Verdana", "", ""),
        x_value_label_style_(14, "Verdana", "", ""),
        // Separate x and y to allow axes to have different styles.
        y_axis_label_style_(14, "Verdana", "", ""),
        y_value_label_style_(12, "Verdana", "", ""),
        point_symbols_style_(12, "Lucida Sans Unicode"), // Used for data point marking.
        //x_values_style_(10, "Verdana", "", ""), // Used for data point X values.
        //y_values_style_(10, "Verdana", "", ""), // Used for data point Y values.
        title_info_(0, 0, "", title_style_, center_align, horizontal),
        x_label_info_(0, 0, "", x_axis_label_style_, center_align, horizontal),
        x_units_info_(0, 0, "", x_value_label_style_, center_align, horizontal),
        x_label_value_(0, 0, "", x_value_label_style_, center_align, horizontal),
        x_axis_(X, -10., +10., black, 1, 0, true, false, true),
        y_axis_(Y, -10., +10., black, 1, 0, true, false, true),
        // Might fill in all values, but there are rather many for ticks_labels_style,
        // And this would separate any changes in styles defaults from plot default.  TODO?
        x_ticks_(X, x_value_label_style_),// so for other defaults see ticks_labels_style.
        y_ticks_(Y, y_value_label_style_),
        y_label_info_(0, 0, "", y_axis_label_style_, center_align, upward),
        y_units_info_(0, 0, "", y_axis_label_style_, center_align, upward),
        y_label_value_(0, 0, "", y_value_label_style_, center_align, upward),
        text_margin_(2.), // for axis label text, as a multiplier of the font size.
        image_border_(yellow, white, 2, 10, true, true), // margin should be about axis label font size.
        plot_window_border_(lightslategray, svg_color(255, 255, 255), 2, 3, true, false),
        legend_box_(yellow, white, 1, 2, true, true),
        legend_header_(0, 0, "", legend_style_, center_align, horizontal),
        legend_width_(200), // width of legend box (pixels) // TODO isn't this calculated?
        legend_height_(0), // height of legend box (pixels)
        legend_left_(-1), legend_right_(-1), legend_top_(-1), legend_bottom_(-1), // Default top left of plot window.
        legend_place_(outside_right), // default but interacts with using plot_window.
        legend_on_(false),
        legend_longest_(0),
        outside_legend_on_(true),
        plot_window_clip_("plot_window"), // for <clipPath id="plot_window" ...
        title_on_(true),
        plot_window_on_(true),
        // Can have both X and Y value shown.
        x_values_on_(false), // If X values of data are shown.
        y_values_on_(false), // If Y values of data are shown.
        xy_values_on_(false), // If X & Y values of data are shown as a pair.
        x_values_style_(horizontal, 3, std::ios::dec, true, value_style_, black, black, false, false),
        y_values_style_(horizontal, 3, std::ios::dec, true, value_style_, black, black, false, false),

        // Autoscaling defaults.
        autoscale_check_limits_(true), // Do check all value for limits, infinity, max, min, NaN.
        x_autoscale_(false),
        x_include_zero_(false), // If autoscaled, include zero on X-axis.
        x_min_ticks_(6),  // If autoscaled, set a minimum number of ticks, default 6.
        x_steps_(0),  // If autoscaled, set any prescaling to decimal 1, 2, 5, 10 etc, default none.
        x_tight_(1e-6), // margin that point can lie outside top and bottom tick.
        y_autoscale_(false),
        y_include_zero_(false), // If autoscaled, include zero on Y-axis.
        y_min_ticks_(6),  // If autoscaled, set a minimum number of ticks, default 6.
        y_steps_(0),  // If autoscaled, set any prescaling to decimal 1, 2, 5, 10 etc, default none.
        y_tight_(1e-6), // margin that point can lie outside top and bottom tick.

        // Used to transform Cartesian to SVG.
        x_scale_(1.), x_shift_(0.),
        y_scale_(1.), y_shift_(0.),
        x_axis_position_(0),
        y_axis_position_(0)
      {
        image_size(500, 400); // Default image size for 2-D.
        // 2-D usually needs to be squarer than 1-D.

        using namespace boost::svg::detail; // Avoid need for prefix detail::

        // Build the document tree by adding all children of the root node.
        for(int i = 0; i < SVG_PLOT_DOC_CHILDREN; ++i)
        {
          image.g();
        }
        set_ids();

        // Set other SVG color, stroke & width defaults for various child PLOT nodes.

        image.g(PLOT_BACKGROUND).style().fill_color(image_border_.fill_);
        image.g(PLOT_BACKGROUND).style().stroke_color(image_border_.stroke_);
        image.g(PLOT_BACKGROUND).style().stroke_width(image_border_.width_); //
        image.g(PLOT_WINDOW_BACKGROUND).style().fill_color(plot_window_border_.fill_);
        image.g(PLOT_WINDOW_BACKGROUND).style().stroke_width(plot_window_border_.width_).stroke_color(plot_window_border_.stroke_);
        image.g(PLOT_LIMIT_POINTS).style().stroke_color(lightslategray).fill_color(antiquewhite);
        image.g(PLOT_X_AXIS).style().stroke_color(black).stroke_width(x_axis_.width());
        image.g(PLOT_Y_AXIS).style().stroke_color(black).stroke_width(y_axis_.width());

        // Note that widths are stored in member data *and* copied here.
        // Not sure if this is wise but ...

        // Ticks
        if(x_ticks_.use_up_ticks() || x_ticks_.use_down_ticks())
        {
          image.g(PLOT_X_MAJOR_TICKS).style().stroke_width(x_ticks_.major_tick_width_).stroke_color(black);
          image.g(PLOT_X_MINOR_TICKS).style().stroke_width(x_ticks_.minor_tick_width_).stroke_color(black);
        }
        if(y_ticks_.left_ticks_on_ || y_ticks_.right_ticks_on_)
        {
          image.g(PLOT_Y_MAJOR_TICKS).style().stroke_width(y_ticks_.major_tick_width_).stroke_color(black);
          image.g(PLOT_Y_MINOR_TICKS).style().stroke_width(y_ticks_.minor_tick_width_).stroke_color(black);
        }
        // Grids.
        // Default color & width for grid, used or not.
        // Might avoid empty grid stuff if this was only done if grid used?  TODO
        image.g(PLOT_X_MAJOR_GRID).style().stroke_width(x_ticks_.major_grid_width_).stroke_color(svg_color(200, 220, 255));
        image.g(PLOT_X_MINOR_GRID).style().stroke_width(x_ticks_.minor_grid_width_).stroke_color(svg_color(200, 220, 255));
        image.g(PLOT_Y_MAJOR_GRID).style().stroke_width(y_ticks_.major_grid_width_).stroke_color(svg_color(200, 220, 255));
        image.g(PLOT_Y_MINOR_GRID).style().stroke_width(y_ticks_.minor_grid_width_).stroke_color(svg_color(200, 220, 255));
        image.g(PLOT_DATA_LINES).style().stroke_width(2); // default width.
        // Alter with plot.data_lines_width(4);

        legend_place_ = (plot_window_on_) ? outside_right : inside; // Defaults.
        // Note if set plot_window_on() then also need to set legend_place.
        // (if set a default in plot_window then call to set legend place must to come *after* it.
        // So not set a default?  But leaving it as inside is worse?)

        x_ticks_on_ = x_ticks_.up_ticks_on_ || x_ticks_.down_ticks_on_;
        y_ticks_on_ = y_ticks_.left_ticks_on_ || y_ticks_.right_ticks_on_;

      } // svg_2d_plot() default constructor.

    private:
      // svg_2d_plot Member Functions.

      void set_ids()  // This only used once in constructor and should be inlined.
      { // document ids for use in <g id = "PLOT_TITLE".../>
        for(int i = 0; i < detail::SVG_PLOT_DOC_CHILDREN; ++i)
        {	// Order determines the painting order.
          image.g(i).id(detail::document_ids[i]);
        }
      } //  void set_ids()

      void transform_pair(std::pair<double, double>& pt)
      { // Transform both x and y from Cartesian to SVG coordinates.
        // SVG image is 0, 0 at top left, Cartesian at bottom left.
        transform_point(pt.first, pt.second);
      }
      void calculate_plot_window()
      { // The plot window is used to set a clip path:
        // this ensures that data points and lines (and anything else)
        // outside this window are NOT drawn.

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

       // Assume that axis labels are always at bottom and left.
       if(x_axis_.label_on_)
       { // Leave space at bottom for X axis label.
          plot_bottom_ -= x_axis_label_style_.font_size() * text_margin_;
       }
        if(y_axis_.label_on_)
        { // Leave space at left for Y axis label.
          plot_left_ += x_axis_label_style_.font_size() * text_margin_;
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
        size_legend_box(); // Size depends on its contents.
        place_legend_box(); // according to options chosen.

        // Because there may be several datasets,
        // and the scaling can be done by anyone or all of them.
        // my_plot.x_autoscale(my_data1) // for 1 dataset.
        // or my_plot.x_autoscale(my_datas) // for a vector of several datasets.
        // calculates the min & max, increments & ticks.
        if (x_autoscale_)
        { // Use calculated autoscale values.
          // Autoscale has been done in my_data.x_autoscale(my_data);
          // and saved in x_auto_min_value_, x_auto_max_value_, & x_auto_tick_interval_,
          // so copy these values to use them:
          x_axis_.min_ = x_auto_min_value_;
          x_axis_.max_ = x_auto_max_value_;
          x_ticks_.major_interval_  = x_auto_tick_interval_;
          // else ignore auto values, even if have been calculated.
        }

        if (y_autoscale_)
        { // Use calculated autoscale values.
          // Autoscale has been done in my_data.y_autoscale(my_data);
          // and saved in y_auto_min_value_, y_auto_max_value_, & y_auto_tick_interval_,
          // so copy these values to use them:
          y_axis_.min_ = y_auto_min_value_;
          y_axis_.max_ = y_auto_max_value_;
          y_ticks_.major_interval_  = y_auto_tick_interval_;
          // else ignore auto values, even if have been calculated.
        }

        // Check if the axes will intersect.
        // X axis position is determined by the range of Y min and max label values.
        x_axis_position_ = x_intersects_y;  // Assume X-axis will intersect Y-axis (range of Y values includes zero).
        if (y_axis_.min_ > std::numeric_limits<double>::min()) // all Y values definitely > zero.
        { // y_min_ > 0, so X-axis will not intersect Y-axis, so use bottom plot window.
          x_axis_position_ = bottom; // X-axis to bottom of plot window.
          x_ticks_.ticks_on_window_or_axis_ = -1; // bottom = true;
        }
        else if(y_axis_.max_ < -std::numeric_limits<double>::min())  // all Y values definitely < zero.
        { // // y_max_ < 0, so X-axis will not intersect Y-axis, so use top plot window.
          x_axis_position_ = top; // X-axis to top of plot window.
          x_ticks_.ticks_on_window_or_axis_ = +1; // top = true;
        }
        // Y axis position is determined by the range of X values.
        y_axis_position_ = y_intersects_x;  // Assume Y-axis will intersect X-axis (X range includes zero).
        if (x_axis_.min_ > std::numeric_limits<double>::min()) // X values all definitely > zero.
        { // Y-axis > 0, so will not intersect X-axis.
          y_axis_position_ = left; // Y-axis free to left of end of X-axis.
          y_ticks_.ticks_on_window_or_axis_ = -1; // left true; // because floating off end of X-axis.
          // so need to put the labels on the plot window instead of the X-axis.
        }
        else if (x_axis_.max_ < -std::numeric_limits<double>::min()) // X all definitely < zero.
        { // Y-axis < 0, so will not intersect X-axis.
          y_axis_position_ = right;
          y_ticks_.ticks_on_window_or_axis_ = +1; // right = true;
        }

        // Ensure both axis and ticks have the *same* range.
        // (To use the separation, made to give the potential for different ranges,
        // one would have to *not* do this,
        // but to make sure they are both assigned correctly).
        x_ticks_.max_ = x_axis_.max_;
        x_ticks_.min_ = x_axis_.min_;
        y_ticks_.max_ = y_axis_.max_;
        y_ticks_.min_ = y_axis_.min_;

        // Calculate the number of chars of the longest value labels.
        x_ticks_.longest_label(); // Updates label_max_length_
        y_ticks_.longest_label();

        // Check that labels won't collide and advise if they will.
        // Change rotation to avoid collision?

        y_ticks_.label_max_space_ = 0.; // Space for y labels, depending on orientation.
        if (y_ticks_.label_rotation_ == horizontal)
        { // Move plot left edge right to give space for y_value_precision_ digits.
           y_ticks_.label_max_space_ += y_ticks_.label_max_length_; // SVG units.
        }
        else if((y_ticks_.label_rotation_ == upward) || (y_ticks_.label_rotation_ == downward))
        { // Only need one char & 1 space width from Y-axis label.
          y_ticks_.label_max_space_ += 2 * y_value_label_style_.font_size() * wh;
        }
        else
        { // Assume some slope 45, so diagonally down from tick,
          // and takes a bit less room.
          y_ticks_.label_max_space_ = y_ticks_.label_max_length_ * sin45;
        }

        if (y_ticks_.major_value_labels_side_ != 0)
        { // Some major tick value labels wanted.
          if ((y_ticks_.ticks_on_window_or_axis_ < 0) // On left of plot window.
            && (y_ticks_.major_value_labels_side_ < 0) ) // & labels on left.
          {  // Contract plot window left edge to right to make space for value labels on left.
            plot_left_ += y_ticks_.label_max_space_;
          }
          else if ((y_ticks_.ticks_on_window_or_axis_ > 0) // On right of plot window.
            && (y_ticks_.major_value_labels_side_ > 0) ) // & labels to right.
          {  // Contract plot window right to left to make space for value labels on right.
           plot_right_ -= y_ticks_.label_max_space_;
          }
          else
          { // y_ticks_.ticks_on_window_or_axis_ == 0
            // no value labels on plot window (may be on mid-plot Y-axis line).
            // Ignore the unusual case of Y-axis line too close to the axis label.
            // In this case the value labels may overflow the plot window
            // and collide with the axis label!
            // User must change to put value label downward, or on other side of the axis line.
            // using major_value_labels_side(int d)
            // to set tick value labels to left (<0), none (==0) or right (>0).
          }
        } // y_ticks_. major_value_labels_side

        x_ticks_.label_max_space_ = 0; // Work out the longest value label for X-Axis.
        if (x_ticks_.label_rotation_ == horizontal)
        { // Only 1 char height & 1 space needed if labels are horizontal.
          x_ticks_.label_max_space_ = 2 * x_value_label_style_.font_size() * wh; // 2 SVG chars
        }
        else if ((x_ticks_.label_rotation_ == upward) || (x_ticks_.label_rotation_ == downward))
        { // ! horizontal so will need more than 2 chars worth.
            x_ticks_.label_max_space_ += x_ticks_.label_max_length_ ; // SVG chars.
        }
        else
        { // Assume label is sloping, say 45, so * sin(45) = 0.707.
            x_ticks_.label_max_space_ += x_ticks_.label_max_length_ * sin45; // SVG 'chars'.
        }

        if (x_ticks_.major_value_labels_side_ != 0)
        { // Some tick value labels.
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
            // See also notes above on case where labels can overwrite axis.
          }
        } // x_ticks_. major_value_labels_side

        // Make space for any ticks.
        if(y_ticks_.left_ticks_on_)
        { // Start left of plot to right to give space for biggest of any external left ticks.
          plot_left_ += (std::max)(y_ticks_.major_tick_length_, y_ticks_.minor_tick_length_); // Avoid macro max trap!
        }
        if(x_ticks_.down_ticks_on_)
        { // Start bottom of plot higher to give space for any external down ticks.
          plot_bottom_ -= (std::max)(x_ticks_.major_tick_length_, x_ticks_.minor_tick_length_);// Avoid macro max trap!
        }

        if (x_axis_.axis_line_on_)
        { // Want an horizontal X-axis line, so check if range includes zero, so axes intersect,
          // and x_axis_ is svg coordinate of Y-axis (usually y = 0).
          // If not fix axis to bottom (or top) of the plot window.
          if ((x_axis_position_ == bottom) // All Y values definitely > zero.
            && !(x_ticks_.ticks_on_window_or_axis_ < 0) ) // & not already at bottom.
          { // y_min_ > 0 so X-axis will not intersect Y-axis, so use plot window.
            plot_bottom_ -= x_ticks_.label_max_space_; // Move up for the value labels.
            x_axis_.axis_ = plot_bottom_; // Put X-axis on bottom.
          }
          else if ((x_axis_position_ == top)  // All Y values definitely < zero.
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

        if (y_axis_.axis_line_on_)
        { // Want a vertical Y-axis line, so check if range includes zero, so axes intersect,
          // and y_axis_ is svg coordinate of X-axis (usually x = 0).
          // If not fix axis to left (or right) of the plot window.
          if ((y_axis_position_ == left) // All X values definitely > zero.
             //&& !(y_ticks_.ticks_on_window_or_axis_ < 0) // & not already at left.
             )
          { // Y-axis will not intersect X -axis, so put Y-axis line on plot window.
            y_axis_.axis_ = plot_left_; // Y-axis to left,
            //plot_left_ += 2 * y_label_info_.font_size(); // with a space.
          }
          else if ((y_axis_position_ == right) // All X values definitely < zero.
            //&& !(y_ticks_.ticks_on_window_or_axis_ > 0) // & not already at right.
            )
          {
            y_axis_.axis_ = plot_right_; // Y-axis to right of plot window,
          }
          else
          { // x_axis_position_ == x_intersects_y
            // Calculate below after transform is calculated.
          }
        } // if (use_y_axis_line_)

        if (plot_right_ <= plot_left_)
        {
          std::cout << "plot window left " << plot_left_ << ", right " << plot_right_ << std::endl;
          throw std::runtime_error("Plot window right <= left!");
        }

        if (plot_top_ >= plot_bottom_)
        {
          std::cout << "plot window top " << plot_top_ << ", bottom " << plot_bottom_ << std::endl;
          throw std::runtime_error("Plot window top >= bottom!");
        }

        // Calculate scale and shift factors for transform from Cartesian to plot.
        // SVG image is 0, 0 at top left,y increase *downwards*
        // Cartesian 0, 0 at bottom left, y increasing upwards.
        x_scale_ = (plot_right_ - plot_left_) / (x_axis_.max_ - x_axis_.min_);
        x_shift_ = plot_left_ - x_axis_.min_ * (plot_right_ - plot_left_) / (x_axis_.max_ - x_axis_.min_);
        y_scale_ = -(plot_bottom_-plot_top_) / (y_axis_.max_-y_axis_.min_);
        y_shift_ = plot_top_ - (y_axis_.max_ * (plot_top_ - plot_bottom_) / (y_axis_.max_ - y_axis_.min_));

        if (x_axis_.axis_line_on_)
        {
          if(x_axis_position_ == x_intersects_y)
          { // Y Range *does* include zero, so x_axis_ not yet calculated.
            double y(0.); // Use y = 0
            transform_y(y);
            x_axis_.axis_ = y; // svg Y coordinate of horizontal X-axis line.
          }
        }
        if (y_axis_.axis_line_on_)
        { // May need to calculate axes, if will intersect.
          if(y_axis_position_ == y_intersects_x)
          { // X Range *does* include zero, so y_axis_ not yet calculated.
            double x(0.);
            transform_x(x);
            y_axis_.axis_ = x; // SVG x coordinate of vertical Y-axis.
          }
        }
        if (plot_window_on_)
        { // Draw plot window rectangle with border and/or background.
          image.g(detail::PLOT_WINDOW_BACKGROUND).push_back(
            new rect_element(plot_left_, plot_top_, (plot_right_ - plot_left_), plot_bottom_ - plot_top_));
        }

      } // calculate_plot_window

      void draw_y_axis()
      {
        double x;
        x = y_axis_.axis_; // Y-axis (x = 0) transformed into X SVG coordinates.
        // Perhaps to left or right of plot window if X values do not include zero.

        if (y_axis_.axis_line_on_)
        { // Want a Y-axis line.
          if (y_axis_position_ == y_intersects_x)
          { // Draw the vertical Y-axis line at cartesian x = 0).
            double ybottom = plot_bottom_;
            double ytop = plot_top_;
            if (x_axis_position_ == x_intersects_y)
            {
              if (x_ticks_.down_ticks_on_)
              {
                if (x_ticks_.ticks_on_window_or_axis_ < 0) // at bottom
                { // Extend the vertical line down in lieu of longest tick.
                  ybottom += (std::max)(x_ticks_.minor_tick_length_, x_ticks_.major_tick_length_);// Avoid macro max trap!
                }
              }
              else if (x_ticks_.up_ticks_on_)
              {
                if (x_ticks_.ticks_on_window_or_axis_ > 0) // at top
                { // Extend the vertical line up in lieu of longest tick.
                  ytop += (std::max)(x_ticks_.minor_tick_length_, x_ticks_.major_tick_length_);// Avoid macro max trap!
                }
              }
            }
            image.g(detail::PLOT_Y_AXIS).line(x, ytop, x, ybottom);
            // <g id="yAxis" stroke="rgb(0,0,0)"><line x1="70.5" y1="53" x2="70.5" y2="357"/>
            if (y_ticks_.ticks_on_window_or_axis_ < 0) //(y_axis_position_ == left)
            { // Draw vertical line holding the ticks on the left of plot window.
              image.g(detail::PLOT_Y_AXIS).line(plot_left_, plot_top_, plot_left_, plot_bottom_);
            }
            else
            {// Draw vertical line holding the ticks on the right of plot window.
              image.g(detail::PLOT_Y_AXIS).line(plot_right_, plot_top_, plot_right_, plot_bottom_);
            }
          }
          else if (y_axis_position_ == left)
          { // Draw on the left of plot window.
            image.g(detail::PLOT_Y_AXIS).line(plot_left_, plot_top_, plot_left_, plot_bottom_);
          }
          else if (y_axis_position_ == right)
          {// Draw on the right of plot window.
            image.g(detail::PLOT_Y_AXIS).line(plot_right_, plot_top_, plot_right_, plot_bottom_);
          }
          else
          { // ??? Warn that things have gone wrong?
          }
        }

        // Access the paths for the ticks & grids, ready for additions.
        path_element& minor_tick_path = image.g(detail::PLOT_Y_MINOR_TICKS).path();
        path_element& major_tick_path = image.g(detail::PLOT_Y_MAJOR_TICKS).path();
        path_element& minor_grid_path = image.g(detail::PLOT_Y_MINOR_GRID).path();
        path_element& major_grid_path = image.g(detail::PLOT_Y_MAJOR_GRID).path();

        // y_minor_jump is the interval between minor ticks.
        double y_minor_jump = y_ticks_.major_interval_ / ((double)(y_ticks_.num_minor_ticks_ + 1.) );
        // TODO Problem here with using floating point?
        // Was i < y_max_; but didn't show the tick and value at y_max_ so now i <= y_max_;
        // But may still fail if a ls or few bits out? Seems to fail for y = 100, for example.

        // Draw the ticks on the positive side.
        for(double y = 0; y <= y_axis_.max_; y += y_ticks_.major_interval_)
        {
          for(double j = y + y_minor_jump;
            j < (y + y_ticks_.major_interval_) * (1. - 2 * std::numeric_limits<double>::epsilon());
            j += y_minor_jump)
          { // Draw minor tick.
            // This will output 'orphaned' minor ticks that are beyond the plot window,
            // if the last major tick does not coincide with the plot window.
            // These are just ignored in draw_x_minor_tick.
            // There might be 9 of them,
            // if you have the common 9 minor tick between major ticks!
            // TODO this seems ugly - as does the negative ones below.
            if (j != 0. || ! x_axis_.axis_line_on_)
            { // Avoid a major tick at y == 0 where there *is* a horizontal X-axis line.
              // (won't be X-axis line for 1-D where the zero tick is always wanted).
              draw_y_minor_tick(j, minor_tick_path, minor_grid_path);
            }
          }
          // Draw major tick.
          if ((y != 0. || ! x_axis_.axis_line_on_) // axis line requested.
            || (y_ticks_.ticks_on_window_or_axis_ != 0)) // ticks & labels on plot window.
          { // Avoid a major tick at y == 0 where there *is* a horizontal X-axis line.
            // (won't be X-axis line for 1-D where the zero tick is always wanted).
            draw_y_major_tick(y, major_tick_path, major_grid_path);
          }
          else
          {
            // std::cout << "Missed y " << y << std::endl; // Only miss 0s
          }
        }

        // Draw the ticks on the negative side.
        for(double y = 0; y >= y_axis_.min_; y -= y_ticks_.major_interval_)
        {
          for(double j = y; j > y - y_ticks_.major_interval_; j-= y_ticks_.major_interval_ / (y_ticks_.num_minor_ticks_ + 1))
          { // Draw minor ticks.
            if ( (j != 0. || ! y_axis_.axis_line_on_)
              || (y_ticks_.ticks_on_window_or_axis_ != 0) // ticks & labels on plot window.
              )
            { // Avoid a major tick at y == 0 where there *is* a horizontal X-axis line.
              // (won't be X-axis line for 1-D where the zero tick is always wanted).
              draw_y_minor_tick(j, minor_tick_path, minor_grid_path);
            }
          }
          if ((y != 0. || ! x_axis_.axis_line_on_)
            || (y_ticks_.ticks_on_window_or_axis_ != 0) ) // ticks & labels on plot window.
          { // Avoid a major tick at y == 0 where there *is* a horizontal X-axis line.
            // (won't be X-axis line for 1-D where the zero tick is always wanted).
            draw_y_major_tick(y, major_tick_path, major_grid_path);
          }
        }
      } // draw_y_axis

      void draw_y_label()
      { // Draw vertical y_axis_ label, and optional y units.
        // Y-label color is set in constructor thus:
        // image.g(detail::PLOT_Y_LABEL).style().stroke_color(black);
        // and changed using y_label_color(color);

        std::string label = y_label_info_.text();
        if (y_axis_.label_units_on_ && (y_units_info_.text() != ""))
        { // Append the units, user must provide any enclosing ()s, if wanted.
          label += y_units_info_.text() ;
        }

        double x = plot_left_;
        x -= y_axis_label_style_.font_size() * 0.7; // Shift left to suit.
        if (y_ticks_.ticks_on_window_or_axis_ < 0)
        { // Ticks & value labels to left of Y-axis.
          if (y_ticks_.down_ticks_on_)
          { // Shift left for biggest of any ticks.
            x -= (std::max)(y_ticks_.minor_tick_length_, y_ticks_.major_tick_length_);
          }
          if (y_ticks_.major_value_labels_side_ < 0)
          { // Shift left to allow for any value labels.
            x -= y_ticks_.label_max_space_;
          }
        }

        image.g(detail::PLOT_Y_LABEL).push_back(new
          text_element(x,
          (plot_bottom_ + plot_top_) / 2., // center on the plot window.
          label, // "Y-Axis" for example.
          y_axis_label_style_,
          center_align, // One might want it to left or right_align?
          upward)); // Y label must be drawn vertically.

      } // draw_y_label

      void draw_y_major_tick(double value, path_element& tick_path, path_element& grid_path)
      { // Draw a Y axis major tick, tick value labels & grids.
        double y(value); // for tick and/or grid.
        transform_y(y); // Cartesian to SVG coordinates.
        if((y < plot_top_ - 0.01) || (y > plot_bottom_ + 0.01))
          // Allow a bit extra to allow for round-off errors.
        { // tick value is way outside plot window, so nothing to do.
          return;
        }
        double x_left(0.); // Left end of tick.
        double x_right(image.y_size()); // Right end of tick.
        if(y_ticks_.major_grid_on_ == true)
        { // Draw horizontal major Y grid line.
          if(!plot_window_on_ == true)
          {
            if(y_ticks_.major_value_labels_side_ < 0) // left
            { // Start further right to give space for y axis value label.
              y -= y_value_label_style_.font_size() * text_margin_;
            }

            if(y_ticks_.left_ticks_on_ == true)
            { // And similarly space for left ticks.
              y -= y_ticks_.major_tick_length_;
            }
          }
          else
          { // plot_window_on_ to use full width of plot window.
            x_left = plot_left_ + plot_window_border_.width_; // Don't write over either border.
            x_right = plot_right_ - plot_window_border_.width_;
          }
          grid_path.M(x_left, y).L(x_right, y); // Horizontal grid line.
       } // y_major_grid_on

        // Draw major ticks & value label, if necessary.
        double y_tick_length = y_ticks_.major_tick_length_;
        if (y_ticks_.ticks_on_window_or_axis_ < 0)
        { // Start ticks on the plot window border left.
          x_left = plot_left_; // x_left = left,
          x_right = plot_left_; //  x_right = right.
        }
        else if (y_ticks_.ticks_on_window_or_axis_ > 0)
        { // Start ticks on the plot window border right.
          x_left = plot_right_;
          x_right = plot_right_;
        }
        else // y_ticks_.ticks_on_window_or_axis_== 0
        { // Internal style ticks on vertical Y-axis.
          x_left = y_axis_.axis_; // Y-axis line.
          x_right = y_axis_.axis_;
        }
        if(y_ticks_.left_ticks_on_)
        {
          x_left -= y_tick_length; // left
        }
        if (y_ticks_.right_ticks_on_)
        {
          x_right += y_tick_length; // right.
        }
        //if ((y <= (plot_bottom_ - 0.01)) && (y >= (plot_top_ + 0.01)))
        //{ // Make sure that we are drawing inside the allowed window.
        // TODO remove this completely.  Should not be necessary.
        tick_path.M(x_left, y).L(x_right, y); // Draw the major tick.
        // leaving x_left at the left most end of any tick,
        // and x_right at the rightmost end of any tick.
        // These may be on the axis line.
        // y is the vertical tick position.
        //} // in plot window

        if(y_ticks_.major_value_labels_side_ != 0)
        { // Label the tick with a value, for example "1.2"
          std::stringstream label;
          label.precision(y_ticks_.value_precision_);
          label.flags(y_ticks_.value_ioflags_); // set ALL IOflags.
          label << value; // Example: label.str() == "20" or "0.25" or "1.2e+015"
					if (y_ticks_.strip_e0s_)
					{ // Remove unecessary e, +, leadings 0s.
						std::string v = strip_e0s(label.str());
						label.str(v);
					}

          double x = 0; // Where to start writing from, at end of left or right tick, if any.
          // = 0 is only to avoid unitialised warning.
          align_style alignment = center_align;
          // Adjustments to provide space from end of tick before or after writing label.
          if (y_ticks_.label_rotation_ == horizontal)
          {  // Just shift up to center value digits on tick.
            if (y_ticks_.major_value_labels_side_ < 0)
            { // labels to left, so start a little to left of x_left.
              y += y_value_label_style_.font_size() * 0.2;
              x = x_left - y_value_label_style_.font_size() * 0.5;
              alignment = right_align;
            }
            else if(y_ticks_.major_value_labels_side_ > 0)
            { // labels to right, so start a little to right of x_right.
             y += y_value_label_style_.font_size() * 0.2;
             x = x_right + y_value_label_style_.font_size() * 0.5;
              alignment = left_align;
            }
          }
          else if (y_ticks_.label_rotation_ == upsidedown)
           {  // Just shift up to center value digits on tick.
            if (y_ticks_.major_value_labels_side_ < 0)
            { // labels to left, so start a little to left of x_left.
              y -= y_value_label_style_.font_size() * 0.1;
              x = x_left - y_value_label_style_.font_size() * 0.5;
              alignment = left_align;
            }
            else if(y_ticks_.major_value_labels_side_ > 0)
            { // labels to right, so start a little to right of x_right.
              y -= y_value_label_style_.font_size() * 0.1;
              x = x_right + y_value_label_style_.font_size() * 0.5;
              alignment = right_align;
            }
          }
          else if (y_ticks_.label_rotation_ == uphill)
          { // Assume some 45 slope, so need about sqrt(2) less space.
            if (y_ticks_.major_value_labels_side_ < 0)
            { // labels to left, so start a little to left of x_left.
              y -= y_value_label_style_.font_size() * 0.2;
              x = x_left - y_value_label_style_.font_size() * 0.2;
              // Seems to need a bit more space for right than left if rotated.
              alignment = right_align;
            }
            else if(y_ticks_.major_value_labels_side_ > 0)
            { // labels to right, so start a little to right of x_right.
              y += y_value_label_style_.font_size() * 0.2;
              x = x_right + y_value_label_style_.font_size() * 0.7;
              alignment = left_align;
            }
          }
          else if (y_ticks_.label_rotation_ == downhill)
          { // Assume some 45 slope, so need about sqrt(2) less space.
            if (y_ticks_.major_value_labels_side_ < 0)
            { // labels to left, so start a little to left of x_left.
              y += y_value_label_style_.font_size() * 0.3;
              x = x_left - y_value_label_style_.font_size() * 0.7;
              // Seems to need a bit more space for right than left if rotated.
              alignment = right_align;
            }
            else if(y_ticks_.major_value_labels_side_ > 0)
            { // labels to right, so start a little to right of x_right.
              y -= y_value_label_style_.font_size() * 0.3;
              x = x_right + y_value_label_style_.font_size() * 0.1;
              alignment = left_align;
            }
          }
          else if (y_ticks_.label_rotation_ == upward)
          { // Tick value label straight up vertically on Y-axis.
            y -= y_value_label_style_.font_size() * 0.1;
            if (y_ticks_.major_value_labels_side_ < 0)
            { // labels to left, so start a little to left of x_left.
              x = x_left - y_value_label_style_.font_size() * 0.7;
              // Seems to need a bit more space for right than left if rotated.
              alignment = center_align;
            }
            else if(y_ticks_.major_value_labels_side_ > 0)
            { // labels to right, so start a little to right of x_right.
              x = x_right + y_value_label_style_.font_size() * 1.5;
              alignment = center_align;
            }
					}
					else if (y_ticks_.label_rotation_ == downward)
					{ // Tick value label straight down vertically on Y-axis.
            y -= y_value_label_style_.font_size() * 0.1;
            if (y_ticks_.major_value_labels_side_ < 0)
            { // labels to left, so start a little to left of x_left.
              x = x_left - y_value_label_style_.font_size() * 1.2;
              // Seems to need a bit more space for right than left if rotated.
              alignment = center_align;
            }
            else if(y_ticks_.major_value_labels_side_ > 0)
            { // labels to right, so start a little to right of x_right.
              x = x_right + y_value_label_style_.font_size() * 0.7;
              alignment = center_align;
            }
					}
					else
					{ // Others not yet implemented.
            return; // Without any value label.
         } // All rotations.

        if (x <= 0)
        { // Sanity checks on svg coordinates.
          throw std::runtime_error("Y-tick X value wrong!");
        }
        if (y <= 0)
        {
          throw std::runtime_error("Y-tick Y value wrong!");
        }
        if(y_ticks_.ticks_on_window_or_axis_ != 0)
          { // External to plot window style left or right.
            // Always want all values including "0", if labeling external to plot window.
            // y_ticks_.ticks_on_window_or_axis_ == true != 0
            image.g(detail::PLOT_VALUE_LABELS).text(
              x,
              y,
              label.str(), y_value_label_style_, alignment, y_ticks_.label_rotation_);
          }
          else
          { // ! y_ticks_.y_ticks_on_plot_window_ == 0 'Internal' - value labels either side of vertical Y-axis.
            if ((value != 0) && y_axis_.axis_line_on_)
            { // Avoid a zero ON the Y-axis if it would be cut through by any horizontal X-axis line.
              image.g(detail::PLOT_VALUE_LABELS).text(
                x,
                y,
                label.str(),
                y_value_label_style_,
                alignment,
                y_ticks_.label_rotation_);
            }
          } // either on plot window or 'on axis'.
        } // want value label on tick
      } // draw_y_major_tick

      void draw_y_minor_tick(double value, path_element& tick_path, path_element& grid_path)
      { // Draw a Y-axis minor tick and optional grid.
        double x_left(0.); // Start on vertical Y axis line.
        double x_right(image.y_size()); // right edge of image.
        double y(value); // Tick position and value label,
        transform_y(y); // convert to svg.

        if(y_ticks_.minor_grid_on_)
        { // Draw the minor grid, if wanted.
          if(!plot_window_on_)
          {
            if(x_axis_.label_on_)
            {
              x_left += y_value_label_style_.font_size() * text_margin_;
              x_right -= y_value_label_style_.font_size() * text_margin_;
            }
          }
          else
          { // plot_window_on_
            x_left = plot_left_ + plot_window_border_.width_;
            x_right = plot_right_ - plot_window_border_.width_; // Ensure just *inside* window?
          }
          if((y >= plot_top_) && (y <= plot_bottom_) && (x_left >= plot_left_) && (x_right <= plot_right_) )
          { // Make sure that we are drawing inside the allowed plot window.
            // Note comparisons are 'upside-down' - y is increasing downwards!
            grid_path.M(x_left, y).L(x_right, y); // Draw grid line.
          }
          else
          {
            // Just ignore outside plot window
          }
        } // y_minor_grid

        // Draw y minor ticks.
        if(y_ticks_.ticks_on_window_or_axis_ < 0)
        { // Put y minor ticks on the plot window border left.
          x_left = plot_left_;
          x_right = plot_left_;
        }
        else if (y_ticks_.ticks_on_window_or_axis_ > 0)
        { // Put y minor ticks on the plot window border left.
          x_left = plot_right_;
          x_right = plot_right_;
        }
        else
        { // Internal style, y_ticks_.ticks_on_window_or_axis_ == 0
          x_left = y_axis_.axis_; // On the Y-axis line itself.
          x_right = y_axis_.axis_;
        }
        if(y_ticks_.left_ticks_on_)
        {
          x_left -= y_ticks_.minor_tick_length_;
        }
        if(y_ticks_.right_ticks_on_)
        {
          x_right += y_ticks_.minor_tick_length_;
        }
        //if((x_left >= plot_left_) && (x_right <= plot_right_) && (y <= plot_bottom_) && (y >= plot_top_))
        // but can never be inside if left tick!
        if((y <= plot_bottom_) && (y >= plot_top_))
        { // Make sure that we are drawing inside of the allowed plot window.
          tick_path.M(x_left, y).L(x_right, y); // Draw the tick.
        }
        else
        {// Do nothing?  warn?
          // std::cout << "y minor tick OUTside " << x_left << ' ' << y << ' ' << x_right << std::endl;
        }
      } // void draw_y_minor_tick

      void draw_straight_lines(const svg_2d_plot_series& series)
      { // Straight line between data points (rather than a Bezier curve).
        double prev_x; // Previous data points.
        double prev_y;
        double temp_x(0.);
        double temp_y;

        g_element& g_ptr = image.g(detail::PLOT_DATA_LINES).g();
        g_ptr.clip_id(plot_window_clip_);
        g_ptr.style().stroke_color(series.line_style_.stroke_color_);
        g_ptr.style().fill_color(series.line_style_.area_fill_); // Now set in path below too.
        g_ptr.style().stroke_width(series.line_style_.width_);
        path_element& path = g_ptr.path();
        path.style().fill_color(series.line_style_.area_fill_);

        bool is_fill = !series.line_style_.area_fill_.is_blank;
        path.style().fill_on(is_fill); // Ensure includes a fill="none" if no fill.

        if(series.series.size() > 1)
        { // Need at least two points for a line ;-)
          std::multimap<double, double>::const_iterator j = series.series.begin();

          // If we have to fill the area under the plot,
          // we first have to move from the X-axis (y = 0) to the first point,
          // and again at the end after the last point.
          prev_x = (*j).first;
          prev_y = 0.; // X-axis.
          transform_point(prev_x, prev_y);
          if(is_fill)
          { // Move to 1st point.
            //path.style().fill_color(series.line_style_.area_fill_); // Duplicates so no longer needed?
            path.M(prev_x, prev_y);
          }
          transform_y(prev_y = (*j).second);
          if(is_fill)
          { // Area fill wanted.
            // path.style().fill_color(series.line_style_.area_fill_); // Duplicates so no longer needed?
            path.L(prev_x, prev_y); // Line from X-axis to 1st point.
          }
          else
          { // Area fill == blank
            path.M(prev_x, prev_y);
          }
          ++j; // so now refers to 2nd point to plot.

          for(; j != series.series.end(); ++j)
          {
            temp_x = (*j).first;
            temp_y = (*j).second;
            transform_point(temp_x, temp_y);
            path.L(temp_x, temp_y); // Line to next point.
            //if(is_fill) // This seems to stop area-fill and is not needed anyway.
            //{
            //  //path.M(temp_x, temp_y);
            //}
            prev_x = temp_x;
            prev_y = temp_y;
          } // for j'th point

          if(is_fill)
          { // Area fill wanted.
            transform_y(temp_y = 0.); // X-axis line coordinate.
            path.L(temp_x, temp_y).z(); // Draw line to X-axis & closepath with Z.
          }
        }
      } // draw_straight_lines

      void draw_bezier_lines(const svg_2d_plot_series& series)
      {
        g_element& g_ptr = image.g(detail::PLOT_DATA_LINES).g();
        g_ptr.clip_id(plot_window_clip_);
        g_ptr.style().stroke_color(series.line_style_.stroke_color_);
        path_element& path = g_ptr.path();

        std::pair<double, double> n; // current point.
        std::pair<double, double> n_minus_1; // penultimate.
        std::pair<double, double> n_minus_2;
        std::pair<double, double> fwd_vtr;
        std::pair<double, double> back_vtr;

        bool is_fill = !series.line_style_.area_fill_.is_blank;
        if(is_fill == false)
        {
          path.style().fill_on(false); // default path constructor is false
        }
        else
        { // !is_blank so do want area fill.
          path.style().fill_color(series.line_style_.area_fill_);
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
      { // Draw line through data series, Bezier curved or straight, or none.
        for(unsigned int i = 0; i < series.size(); ++i)
        {
          if(series[i].line_style_.bezier_on_)
          { // curved.
            draw_bezier_lines(series[i]);
          }
          else if(series[i].line_style_.line_on_)
          {
            draw_straight_lines(series[i]);
          }
          else
          { // No line joining points.
          }
        }
      } // draw_plot_lines

      void draw_plot_points()
      { // Draw normal 'good' non-limit points.
        double x(0.);
        double y(0.);
        for(unsigned int i = 0; i < series.size(); ++i)
        {
          g_element& g_ptr = image.g(detail::PLOT_DATA_POINTS).g();

          g_ptr.style()
            .fill_color(series[i].point_style_.fill_color_)
            .stroke_color(series[i].point_style_.stroke_color_);

          for(std::multimap<double, double>::const_iterator j = series[i].series.begin();
            j != series[i].series.end(); ++j)
          {
            x = j->first;
            double vx = x; // Note the true x value.
            y = j->second;
            double vy = y; // Note the true y value.
            transform_point(x, y);
            if((x > plot_left_)  && (x < plot_right_) && (y > plot_top_)  && (y < plot_bottom_))
            { // Is inside plot window, so draw a point.
              draw_plot_point(x, y, g_ptr, series[i].point_style_);
              g_element& g_ptr_vx = image.g(detail::PLOT_X_POINT_VALUES).g();
              if (x_values_on_)
              { // Show the value of the X data point too.
// void draw_plot_point_value(double x, double y, g_element& g_ptr, value_style& val_style, plot_point_style& point_style, double value)

                draw_plot_point_value(x, y, g_ptr_vx, x_values_style_, series[i].point_style_, vx);
              }
              g_element& g_ptr_vy = image.g(detail::PLOT_Y_POINT_VALUES).g();
              if (y_values_on_)
              { // show the value of the Y data point too.
                draw_plot_point_value(x, y, g_ptr_vy, y_values_style_,series[i].point_style_, vy);
              }

              if (xy_values_on_)
              { // show the values of the X & Y data as a pair.
                draw_plot_point_values(x, y, g_ptr_vx, g_ptr_vy, x_values_style_, y_values_style_, vx, vy);
              }

            }
          } // for
        } // for normal points

        // Draw all the 'bad' at_limit points.
        for(unsigned int i = 0; i < series.size(); ++i)
        {
          g_element& g_ptr = image.g(detail::PLOT_LIMIT_POINTS);

          for(std::multimap<double,double>::const_iterator j = series[i].series_limits.begin();
            j!=series[i].series_limits.end(); ++j)
          {
            x = j->first;
            y = j->second;
            transform_point(x, y);
            if((x > plot_left_)  && (x < plot_right_) && (y > plot_top_)  && (y < plot_bottom_))
            { // Is inside plot window, so draw a point.
             // draw_plot_point(x, y, g_ptr, plot_point_style(blank, blank, s, cone)); default.
              draw_plot_point(x, y, g_ptr, series[i].limit_point_style_);
            }
          }
        } // limits point
      } //  void draw_plot_points()

      void draw_bars()
      { // Draw normal bar chart for 'good' non-limit points.
        g_element& g_ptr = image.g(detail::PLOT_DATA_POINTS).g(); // Moved up out of loop.
        double x(0.);
        double y(0.); // Cartesian coord y = 0.
        double x0(0.); // Y-axis line.
        double y0(.0); // X-axis line.
        transform_y(y0); // SVG coordinate of horizontal X-axis line.
        transform_x(x0); // SVG coordinate of vertical Y-axis line.
        for(unsigned int i = 0; i < series.size(); ++i)
        {
          if (series[i].bar_style_.bar_option_ == no_bar)
          { // No bars wanted for this series.
            continue;
          }
          g_ptr.style().stroke_color(series[i].bar_style_.color_); // stroke color of stick or rectangle block.
            //.fill_color(series[i].bar_style_.area_fill_); // Only used for rectangle stroke.
            //.stroke_width(series[i].bar_style_.width_) // Used for width of stick and rectangle block.
          path_element& path = g_ptr.path();
          //path.fill(series[i].bar_style_.area_fill_ != blank);
          path.fill_on(false);

          double h_w = series[i].bar_style_.width_; // For block bar chart.
          //double h_h = 0.;
          for(std::multimap<double, double>::const_iterator j = series[i].series.begin();
            j != series[i].series.end(); ++j)
          { // All the 'good' data points.
            x = j->first;
            y = j->second;
            transform_point(x, y);
            if((x > plot_left_)  && (x < plot_right_) && (y > plot_top_)  && (y < plot_bottom_))
            { // Is inside plot window, so some bar to draw.
              switch(series[i].bar_style_.bar_option_)
              { // -2 block to Y-axis,-1 stick to Y-axis, none, +1 stick to X-axis, -2 block to X-axis.
              case y_block: // Draw a rectangle centered on the data point horizontally to Y-axis.
                 {
                   g_ptr.style().stroke_width(series[i].line_style_.width_) // line_width used for rectangle line width.
                     .fill_color(series[i].bar_style_.area_fill_);
                   double h_left = x;
                   double h_top = y - h_w / 2; // Start a half-width above the data point center.
                   path.M(h_left, h_top).L(h_left, h_top + h_w).L(x0, h_top + h_w).L(x0, h_top).z();
                 }
                 break;
              case y_stick:
                 path.style().stroke_width(series[i].bar_style_.width_); // bar_width used for stick line width.
                 path.M(x, y).L(x0, y); // Draw a line from point horizontally to Y-axis.
                 break;
              case none:
                 break; // Already handled above, so should not get here.
              case x_stick:
                 path.style().stroke_width(series[i].bar_style_.width_); // bar_width used for stick line width.
                 path.M(x, y).L(x, y0); // Draw a line from point vertically to X-axis.
                 break;
              case x_block: // Draw a rectangle centered on the data point vertically to X-axis.
               {
                 g_ptr.style().stroke_width(series[i].line_style_.width_) // line_width used for rectangle line width.
                   .fill_color(series[i].bar_style_.area_fill_);
                 double h_left = x - h_w / 2; // Start a half width left of the data point center.
                 double h_top = y;
                 path.M(h_left, h_top).L(h_left + h_w, h_top).L(h_left + h_w, y0).L(h_left, y0).z();
               }
                break;
              } // switch
            } // for
          } // for normal points
        }
        // Ignore all the 'bad' at_limit points.
      } //  void draw_bars()

      void draw_histogram()
      { // Draw a histogram with variable width but contiguous bins.
        // Histograms differ from bar charts in the the *area* denotes the value,
        // whereas the bar *height* denotes the value for a bar chart.
        // bin widths are provided from the X-axis data series values.
        // The 1st data X-value provides the start of the 1st bin,
        // the 2nd data X-value provides the end of the 1st bin,
        // and the 1st Y-value the area of the 1st bin,
        // and the start of the second bin, and so on, until the
        // width of last bin is calculated from the last data point in series,
        // that must have a zero area.  ? NaN
        // Bins can be the same (most common) or different widths.
        // Intervals must not overlap and bins must be adjacent.
        // http://en.wikipedia.org/wiki/Histogram

        // Attempts to allow a row or horizontal were abandoned because of complications
        // with the use of map which orders the x values providing the bins.
        // Using the y values for the bins implies changing the Y axes labeling and scaling too.

        g_element& g_ptr = image.g(detail::PLOT_DATA_POINTS).g(); // Moved up out of loop.
        for(unsigned int i = 0; i < series.size(); ++i)
        { // for each data series.
          if (series[i].histogram_style_.histogram_option_ == no_histogram)
          { // No histogram wanted for this series.
            continue;
          }
          // Get the color scheme.
          g_ptr.style().stroke_color(series[i].line_style_.stroke_color_); // stroke color around bin blocks.
          g_ptr.style().fill_color(series[i].line_style_.area_fill_);
          g_ptr.style().stroke_width(series[i].line_style_.width_); // line_width used for stick line width.

          path_element& path = g_ptr.path();
          path.fill_on(series[i].line_style_.area_fill_ != blank);
          if (path.fill_on() == true)
          {
            path.style().fill_color(series[i].line_style_.area_fill_);
          }
          else
          {
            path.style().fill_color(blank);
          }

          std::multimap<double, double>::const_iterator last = series[i].series.end();
          last--; // Final pair with first the last bin end, and value zero or NaN.
          if (last->second != 0)
          {
            std::cout << "Last bin end " << last->first << " should have zero value! but is "  << last->second << std::endl;
            // Or Throw? or skip this series?
          }
          for(std::multimap<double, double>::const_iterator j = series[i].series.begin();
            j != last; ++j)
          { // All the 'good' 'real' data points.
            double x = j->first;
            double y = j->second;
            std::multimap<double, double>::const_iterator j_next = j;
            j_next++;
            if (j != last)
            { // Draw a column (perhaps filled) to show bin.
              double x_next = j_next->first;
              double w = x_next - x;
              double h = y / w;
              // std::cout << x << ' ' << y << ' ' << w << ' '  << h << std::endl;
              double yy = h;
              double y0(0.); // X-axis line.
              transform_y(y0); // SVG y coordinate of horizontal X-axis line.
              transform_x(x); // SVG x coordinate of start of bin,
              transform_x(x_next);  // SVG x coordinate of end of bin,
              transform_y(yy); // SVG y coordinate of height of bin.
              //if((x > plot_left_)  && (x < plot_right_) && (y > plot_top_)  && (y < plot_bottom_))
              //{ // Is inside plot window, so some columns to draw. TODO checks?
                path.M(x, y0).L(x, yy) // Draw a line from point vertically from X-axis.
                  .L(x_next, yy) // & horizonally to next bin end (next x value).
                  .L(x_next, y0) // back to X-axis.
                  .Z(); // So will fill.
             } // if
          } // for series
        } // for normal points.
        // Ignore all the 'bad' at_limit points.
      } //  void draw_histogram()

      void update_image()
      {
        clear_all();
        // svg paint rules are that later 'painting' writes over
        // previous painting, so the order of drawing is important.

        // Draw image background (perhaps with border and/or fill color).
        image.g(detail::PLOT_BACKGROUND).push_back(
          new rect_element(0, 0, image.x_size(),  image.y_size()));

        calculate_plot_window();
        draw_title(); // Moved to ensure plot_X and Y are valid.

        // Define the clip path for the plot window.
        // We don't want to allow overlap of the plot window border lines,
        // thus the minor adjustments.
        // TODO should this be border thickness?
        // Actually we DO want to allow a small amount of overlap
        // so round point can lie on the axis line without being chopped in half or not show at all!!!
        image.clip_path(rect_element(plot_left_ + 1, plot_top_ + 1,
          plot_right_ - plot_left_ - 2, plot_bottom_ - plot_top_ - 2),
          plot_window_clip_);
        // <clipPath id="plot_window"><rect x="35" y="38" width="309" height="322"/></clipPath>

        image.g(detail::PLOT_DATA_POINTS).clip_id(plot_window_clip_);

        // Draw axes, labels & legend, as required.
        draw_x_axis(); // Must do X-axis first.
        draw_y_axis();
        if(legend_on_)
        {
          draw_legend();
        }
        if(x_axis_.label_on_)
        {
          draw_x_label();
        }
        if(y_axis_.label_on_)
        {
          draw_y_label();
        }

        draw_plot_lines(); // Draw lines between points.
        draw_plot_points();
        draw_bars();
        draw_histogram();
    } // void update_image()


    public: // Member functions
      // -----------------------------------------------------------------
      // write() has two flavors, a file and a ostream.
      // The file version opens an ostream, and calls the stream version.
      // The stream version first clears all unnecessary data from the graph,
      // builds the document tree, and then calls the write function for the root
      // document node, which calls all other nodes through the Visitor pattern.
      // -----------------------------------------------------------------

      svg_2d_plot& write(const std::string& file)
      { // Write the plot image to a named file.
        std::string filename(file); // Copy to avoid problem with const if try to append.
        if (filename.find(".svg") == std::string::npos)
        { // No file type suffix, so provide the default .svg.
          filename.append(".svg");
        }

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

      // Member functions to set and get plot options.
      // All return *this to permit chaining.

      // These below only refer to 2D plot.
      // See axis_plot_label.hpp for all the many 1D functions X-Axis.

      axis_line_style& x_axis()
      {
        return x_axis_;
      }

      axis_line_style& y_axis()
      {
        return y_axis_;
      }

      ticks_labels_style& x_ticks()
      {
        return x_ticks_;
      }

      ticks_labels_style& y_ticks()
      {
        return y_ticks_;
      }

      svg_2d_plot& y_label_on(bool cmd)
      { // If Y axis name or label.
        y_axis_.label_on_ = cmd;
        return *this; // Make chainable.
      }

      bool y_label_on()
      {
        return y_axis_.label_on_;
      }

      svg_2d_plot& x_label_on(bool cmd)
      {
        x_axis_.label_on_ = cmd;
        return *this;
      }

      bool x_label_on()
      {
        return x_axis_.label_on_;
      }

      svg_2d_plot& y_major_labels_on(int cmd)
      { //< 0 means to left or down (default), 0 (false) means none, > 0 means to right (or top).
        y_ticks_.major_value_labels_side_ = cmd;
        return *this;
      }

      int y_major_labels_on()
      {
        return y_ticks_.major_value_labels_side_;
      }

      svg_2d_plot& y_major_label_rotation(rotate_style rot)
      {
        y_ticks_.label_rotation_ = rot;
        return *this;
      }

      int y_major_label_rotation()
      {
        return  y_ticks_.label_rotation_ ;
      }

      svg_2d_plot& y_axis_width(double width)
      {
        image.g(detail::PLOT_Y_AXIS).style().stroke_width(width);
        return *this;
      }

      double y_axis_width()
      {
        return image.g(detail::PLOT_Y_AXIS).style().stroke_width();
      }

      svg_2d_plot& y_value_precision(int digits)
      { // Precision of Y tick label values in decimal digits (default 3).
        y_ticks_.value_precision_ = digits;
        return *this;
      }

      int y_value_precision()
      { // Precision of Y tick label values in decimal digits (default 3).
        return y_ticks_.value_precision_;
      }

      svg_2d_plot& y_value_ioflags( std::ios_base::fmtflags flags)
      { // IO flags of Y tick label values (default 0X201).
        y_ticks_.value_ioflags_ = flags;
        return *this;
      }

      int y_value_ioflags()
      { // ALL stream ioflags for control of format of Y value labels.
        return y_ticks_.value_ioflags_;
      }

      svg_2d_plot& y_labels_strip_e0s(bool cmd)
      {
        y_ticks_.strip_e0s_ = cmd;
        return *this;
      }

      bool y_labels_strip_e0s()
      {
        return y_ticks_.strip_e0s_;
      }

      svg_2d_plot& y_axis_color(const svg_color& col)
      { // Set only stroke color.
        image.g(detail::PLOT_Y_AXIS).style().stroke_color(col);
        return *this;
      }

      svg_color y_axis_color()
      { // return the stroke color.
        return image.g(detail::PLOT_Y_AXIS).style().stroke_color();
      }

      svg_2d_plot& y_axis_label_color(const svg_color& col)
      { // Set stroke color.
        image.g(detail::PLOT_VALUE_LABELS).style().stroke_color(col);
        return *this;
      }

      svg_color y_axis_label_color()
      { // But only return the stroke color.
        //return y_label_info_.style().stroke_color();
        return image.g(detail::PLOT_VALUE_LABELS).style().stroke_color();
      }

      svg_2d_plot& y_label_units_on(bool b)
      {
        y_axis_.label_units_on_ = b;
        return *this;
      }

      bool y_label_units_on()
      {
        return y_axis_.label_units_on_;
      }

      svg_2d_plot& y_axis_value_color(const svg_color& col)
      {
        image.g(detail::PLOT_VALUE_LABELS).style().stroke_color(col);
        return *this;
      }

      svg_color y_axis_value_color()
      { // Only return the stroke color.
        return image.g(detail::PLOT_VALUE_LABELS).style().stroke_color();
      }

      svg_2d_plot& y_label_width(double width)
      {
        image.g(detail::PLOT_Y_LABEL).style().stroke_width(width);
        return *this;
      }

      double y_label_width()
      {
        return image.g(detail::PLOT_Y_LABEL).style().stroke_width();
      }

      svg_2d_plot& y_major_grid_color(const svg_color& col)
      {
        image.g(detail::PLOT_Y_MAJOR_GRID).style().stroke_color(col);
        return *this;
      }

      const svg_color y_major_grid_color()
      {
        return image.g(detail::PLOT_Y_MAJOR_GRID).style().stroke_color();
      }

      svg_2d_plot& y_minor_grid_color(const svg_color& col)
      {
        image.g(detail::PLOT_Y_MINOR_GRID).style().stroke_color(col);
        return *this;
      }

      const svg_color y_minor_grid_color()
      {
        return image.g(detail::PLOT_Y_MINOR_GRID).style().stroke_color();
      }

      svg_2d_plot& y_major_tick_color(const svg_color& col)
      {
        image.g(detail::PLOT_Y_MAJOR_TICKS).style().stroke_color(col);
        return *this;
      }

      const svg_color y_major_tick_color()
      {
        return image.g(detail::PLOT_Y_MAJOR_TICKS).style().stroke_color();
      }

      svg_2d_plot& y_minor_tick_color(const svg_color& col)
      {
        image.g(detail::PLOT_Y_MINOR_TICKS).style().stroke_color(col);
        return *this;
      }

      const svg_color y_minor_tick_color()
      {
        return image.g(detail::PLOT_Y_MINOR_TICKS).style().stroke_color();
      }

      const std::string y_axis_position()
      {
        switch(y_axis_position_)
        {
        case left:
          return "y_axis_position left (all X values < 0)"; break;
        case y_intersects_x:
          return "y_axis_position intersects X axis (X range includes zero)"; break;
        case right:
          return "y_axis_position right (all X values > 0)"; break;
        default:
          return "?"; break;
        }
      }

      svg_2d_plot& y_range(double min_y, double max_y)
      { // Set the range (max and min) for Y values.
        if (!boost::math::isfinite(min_y))
        {
          throw std::runtime_error("Y range: min not finite!");
        }
        if (!boost::math::isfinite(max_y))
        {
          throw std::runtime_error("Y range: max not finite!");
        }

        if(max_y <= min_y)
        { // max <= min.
          throw std::runtime_error("Y range: y max <= y min!");
        }
        if((max_y - min_y) < std::numeric_limits<double>::epsilon() * 1000)
        { // Range too small to display.
          throw std::runtime_error("Y range too small!" );
        }
        y_axis_.min_ = min_y;
        y_axis_.max_ = max_y;
        y_autoscale_ = false;
       return *this;
      }

      std::pair<double, double> y_range()
      {
        std::pair<double, double> r;
        r.first = y_axis_.min_;
        r.second = y_axis_.max_;
        return r;
      }

      double y_min()
      { // Can't check finite ness and relation to y_max.
        return y_axis_.min_;
      }

      double y_max()
      { // Can't check finite ness and relation to y_min.
        return y_axis_.max_;
      }

      bool y_autoscale()
      {
        return y_autoscale_;
      }

      svg_2d_plot& y_autoscale(bool b)
      {
        y_autoscale_ = b;
        return *this;
      }

      bool y_values_on()
      {
        return y_values_on_;
      }

      svg_2d_plot& y_values_on(bool b)
      {
        y_values_on_ = b;
        return *this;
      }

      bool xy_values_on()
      {
        return xy_values_on_;
      }

      svg_2d_plot& xy_values_on(bool b)
      {
        xy_values_on_ = b;
        return *this;
      }

      bool y_plusminus_on()
      {
        return y_values_style_.plusminus_on_;
      }

      svg_2d_plot& y_plusminus_on(bool b)
      {
        y_values_style_.plusminus_on_ = b;
        return *this;
      }

      bool y_df_on()
      {
        return y_values_style_.df_on_;
      }

      svg_2d_plot& y_df_on(bool b)
      {
        y_values_style_.df_on_ = b;
        return *this;
      }

      svg_2d_plot& y_autoscale(std::pair<double, double> p)
      { // Use Y min & max pair values to autoscale.
        scale_axis(p.first, p.second, // double min and max from pair.
        &y_auto_min_value_, &y_auto_max_value_, &y_auto_tick_interval_, &y_auto_ticks_,
        autoscale_check_limits_,
        y_include_zero_, y_tight_, y_min_ticks_, y_steps_);
        y_autoscale_ = true;  // Default to use calculated values.
        return *this;
      } // autoscale(pair<double, double> p)

      template <class T> // T an STL container: array, vector ...
      svg_2d_plot& y_autoscale(const T& begin, const T& end) // Data series using iterators to
      { // to use to calculate autoscaled values.
        scale_axis(begin, end,
        &y_auto_min_value_, &y_auto_max_value_, &y_auto_tick_interval_, &y_auto_ticks_,
        autoscale_check_limits_,
        y_include_zero_, y_tight_, y_min_ticks_, y_steps_);
        y_autoscale_ = true; // Default to use calculated values.
        return *this;
      }

      template <class T> // T an STL container: array, vector ...
      svg_2d_plot& y_autoscale(const T& container) // Whole data series.
      { // to use to calculate autoscaled values.
        scale_axis(container.begin(), container.end(), // All the container.
        &y_auto_min_value_, &y_auto_max_value_, &y_auto_tick_interval_, &y_auto_ticks_,
        autoscale_check_limits_,
        y_include_zero_, y_tight_, y_min_ticks_, y_steps_);
        y_autoscale_ = true;  // Default to use calculated values.
        return *this;
      }

      template <class T> // T a 2D STL container: array, vector ...
      svg_2d_plot& xy_autoscale(const T& container) // Whole data series,
      { // to use to calculate autoscaled values for *both* X and Y axes.
        scale_axis(container, // All the container.
          &x_auto_min_value_, &x_auto_max_value_, &x_auto_tick_interval_, &x_auto_ticks_,
          &y_auto_min_value_, &y_auto_max_value_, &y_auto_tick_interval_, &y_auto_ticks_,
         autoscale_check_limits_,
         x_include_zero_, x_tight_, x_min_ticks_, x_steps_,
          y_include_zero_, y_tight_, y_min_ticks_, y_steps_);
        x_autoscale_ = true; // Default to use calculated values.
        y_autoscale_ = true; // Can be switch off with autoscale(false);
        return *this;
      } // xy_autoscale

      bool xy_autoscale()
      {
        return y_autoscale_ && x_autoscale_;
      }

      svg_2d_plot& y_major_interval(double inter)
      {
        y_ticks_.major_interval_ = inter;
        return *this;
      }

      double y_major_interval()
      {
        return y_ticks_.major_interval_;
      }

      svg_2d_plot& y_major_tick_length(double length)
      {
        y_ticks_.major_tick_length_ = length;
        return *this;
      }

      double y_major_tick_length()
      {
        return y_ticks_.major_tick_length_;
      }

      svg_2d_plot& y_minor_tick_length(double length)
      {
        y_ticks_.minor_tick_length_ = length;
        return *this;
      }

      double y_minor_tick_length()
      {
        return y_ticks_.minor_tick_length_;
      }

      svg_2d_plot& y_num_minor_ticks(unsigned int num)
      {
        y_ticks_.num_minor_ticks_ = num;
        return *this;
      }

      unsigned int y_num_minor_ticks()
      {
        return y_ticks_.num_minor_ticks_;
      }

      svg_2d_plot& y_label_axis(const std::string& str)
      { // Set label for Y-axis.
        y_label_info_.text(str);
        return *this;
      }

      std::string y_label_axis()
      { // text to label Y axis.
        return y_label_info_.text();
      }

      svg_2d_plot& y_major_tick_width(double width)
      {
        y_ticks_.major_tick_width_ = width;
        image.g(detail::PLOT_Y_MAJOR_TICKS).style().stroke_width(width);
        return *this;
      }

      double y_major_tick_width()
      {
        return y_ticks_.major_tick_width_;
      }

      svg_2d_plot& y_minor_tick_width(double width)
      {
        y_ticks_.minor_tick_width_ = width;
        image.g(detail::PLOT_Y_MINOR_TICKS).style().stroke_width(width);
        return *this;
      }

      double y_minor_tick_width()
      {
        return y_ticks_.minor_tick_width_;
      }

      svg_2d_plot& x_ticks_on_window_or_axis(int is)
      {
        x_ticks_.ticks_on_window_or_axis_ = is;
        return *this;
      }

      int x_ticks_on_window_or_axis()
      {
        return x_ticks_.ticks_on_window_or_axis_;
      }

      svg_2d_plot& x_major_value_labels_side(int is)
      { // Label values side for major ticks left -1, (right +1 or none 0).
        x_ticks_.major_value_labels_side_ = is;
        return *this;
      }

      int x_major_value_labels_side()
      { // Label values side for major ticks left -1, (right +1 or none 0).
        return x_ticks_.major_value_labels_side_;
      }

      svg_2d_plot& y_ticks_on_window_or_axis(int is)
      {
        y_ticks_.ticks_on_window_or_axis_ = is;
        return *this;
      }

      int y_ticks_on_window_or_axis()
      {
        return y_ticks_.ticks_on_window_or_axis_;
      }

      svg_2d_plot& y_major_value_labels_side(int is)
      {  // Label values side for major ticks left -1, (right +1 or none 0).
        y_ticks_.major_value_labels_side_ = is;
        return *this;
      }

      int y_major_value_labels_side()
      { // Label values side for major ticks left -1, (right +1 or none 0).
        return y_ticks_.major_value_labels_side_;
      }

      svg_2d_plot& y_ticks_left_on(bool cmd)
      {
        y_ticks_.left_ticks_on_ = cmd;
        return *this;
      }

      bool y_ticks_left_on()
      {
        return y_ticks_.left_ticks_on_;
      }

      svg_2d_plot& y_ticks_right_on(bool cmd)
      {
        y_ticks_.right_ticks_on_ = cmd;
        return *this;
      }

      bool y_ticks_right_on()
      {
        return y_ticks_.right_ticks_on_;
      }
      // Only need y_ticks_left_on & y_ticks_right_on in 2D


      svg_2d_plot& y_major_grid_on(bool is)
      {
        y_ticks_.major_grid_on_ = is;
        return *this;
      }

      bool y_major_grid_on()
      {
        return y_ticks_.major_grid_on_;
      }

      svg_2d_plot& y_minor_grid_on(bool is)
      {
        y_ticks_.minor_grid_on_ = is;
        return *this;
      }

      bool y_minor_grid_on()
      {
        return y_ticks_.minor_grid_on_;
      }

      svg_2d_plot& y_minor_grid_width(double width)
      {
        y_ticks_.minor_grid_width_ = width;
        image.g(detail::PLOT_Y_MINOR_GRID).style().stroke_width(width);
        return *this;
      }

      double y_minor_grid_width()
      {
        return y_ticks_.minor_grid_width_;
      }

      svg_2d_plot& y_major_grid_width(double width)
      {
        y_ticks_.major_grid_width_ = width;
        image.g(detail::PLOT_Y_MAJOR_GRID).style().stroke_width(width);
        return *this;
      }

      double y_major_grid_width()
      {
        return y_ticks_.major_grid_width_;
      }

      svg_2d_plot& y_label_font_size(unsigned int i)
      { // May be best to tie label & unit font sizes together?
        x_axis_label_style_.font_size(i);
        // y_units_info_.font_size(i);
        return *this;
      }

      unsigned int y_label_font_size()
      {
        return y_axis_label_style_.font_size();
      }

      // Note useful until browsers support.
      //svg_2d_plot& y_label_weight(std::string s)
      //{ // "bold" is only one that works so far.
      //  x_axis_label_style_.font_weight(s);
      //  return *this;
      //}

      //const std::string& y_label_weight()
      //{
      //  return x_axis_label_style_.font_weight();
      //}



      svg_2d_plot& y_label_font_family(const std::string& family)
      {
        x_axis_label_style_.font_family(family);
        return *this;
      }

      const std::string& y_label_font_family();
      //const std::string& y_label_font_family()
      //{
      //  return y_label_info_.font_family();
      //}

      // Example of declaration but definition below.
      // TODO Probably better done this way,


      svg_2d_plot& y_values_font_size(unsigned int i)
      {
        y_values_style_.values_text_style_.font_size(i);
        return *this;
      }

      unsigned int y_values_font_size()
      {
        return y_values_style_.values_text_style_.font_size();
      }

      svg_2d_plot& y_values_font_family(const std::string& family)
      {
        y_values_style_.values_text_style_.font_family(family);
        return *this;
      }

      const std::string& y_values_font_family()
      {
        return y_values_style_.values_text_style_.font_family();
      }

      svg_2d_plot& y_values_color(const svg_color& col)
      { // Function could set both fill (middle) and stroke (outside),
        // but just setting fill if simplest,
        // but does not allow separate inside & outside colors.
        image.g(detail::PLOT_Y_POINT_VALUES).style().fill_color(col);
        //svg_2d_plot().image.g(PLOT_Y_POINT_VALUES).style().stroke_color(col);
        return *this;
      }

      svg_color y_values_color()
      { // Function could get either fill and stroke,
        // return svg_2d_plot().image.g(PLOT_Y_POINT_VALUES).style().stroke_color();
        return image.g(detail::PLOT_Y_POINT_VALUES).style().fill_color();
      }

      svg_2d_plot& y_values_rotation(rotate_style rotate)
      { // Degrees (0 to 360).
        y_values_style_.value_label_rotation_ = rotate;
        return *this;
      }

      int y_values_rotation()
      {
        return y_values_style_.value_label_rotation_;
      }

      svg_2d_plot& y_values_precision(int p)
      { // set iostream precision
        y_values_style_.value_precision_ = p;
        return *this;
      }

      int y_values_precision()
      {
        return y_values_style_.value_precision_;
      }

      svg_2d_plot& y_values_ioflags(std::ios_base::fmtflags f)
      { // set iostream format flags
        y_values_style_.value_ioflags_ = f;
        return *this;
      }

      std::ios_base::fmtflags y_values_ioflags()
      {
        return y_values_style_.value_ioflags_;
      }


      // Versions of plot functions to add data series, all or part.

      template <class T>
      svg_2d_plot_series& plot(const T& container, const std::string& title = "")
      { // Version converting to double.
        // Note that this version assumes that *ALL* the data value in the container is used.
        series.push_back(
          svg_2d_plot_series(
          boost::make_transform_iterator(container.begin(), detail::boost_default_2d_convert()),
          boost::make_transform_iterator(container.end(), detail::boost_default_2d_convert()),
          title)
        );
        return series[series.size()-1]; // Number of data series added so far.
      }

      template <class T, class U>
      svg_2d_plot_series& plot(const T& container, const std::string& title = "", U functor = boost_default_2d_convert)
      { // Version with functor.
        // Note that this version assumes that *ALL* the data value in the container is used.
        series.push_back(
          svg_2d_plot_series(
          boost::make_transform_iterator(container.begin(), functor),
          boost::make_transform_iterator(container.end(),   functor),
          title)
        );
        return series[series.size()-1]; // Number of data series added so far.
      }

      template <class T>
      svg_2d_plot_series& plot(const T& begin, const T& end, const std::string& title = "")
      { // Add a data series to the plot (by default, converting to doubles).
        // Note that this version permits a *partial range*,
        // begin to end, of the container to be used.
        series.push_back(
          svg_2d_plot_series(
          boost::make_transform_iterator(begin, detail::boost_default_convert()),
          boost::make_transform_iterator(end, detail::boost_default_convert()),
          title)
        );
        // For example:  my_2d_plot.plot(my_data.begin(), my_data.end(), "My container");
        // my_2d_plot.plot(&my_data[1], &my_data[4], "my_data 1 to 4"); // Add part of data series.
        // Care: last == end  which is one past the last, so this only does 1, 2 & 3 - *not* 4!
        return series[series.size() - 1]; // Number of data series added so far.
      }

      template <class T, class U>
      svg_2d_plot_series& plot(const T& begin, const T& end, const std::string& title = "", U functor = boost_default_2d_convert)
      { // Add a data series to the plot. (Version with custom functor, rather than to double).
        // Note that this version permits a partial range,
        // begin to end, of the container to be used.
        series.push_back(
          svg_2d_plot_series(
          boost::make_transform_iterator(container.begin(), functor),
          boost::make_transform_iterator(container.end(),   functor),
          title)
        );
        return series[series.size() - 1]; // Number of data series added so far.
      }

 }; // class svg_2d_plot : public detail::axis_plot_frame<svg_2d_plot>

    // sample of declared function, externally defined in another .cpp file.
    // TODO May be useful to refactor all functions this way.
    const std::string& svg_2d_plot::y_label_font_family()
    {
      return y_axis_label_style_.font_family();
    }

#if defined (BOOST_MSVC)
#  pragma warning(pop)
#endif

  } // namespace svg
} // namespace boost

#endif // BOOST_SVG_SVG_2D_PLOT_HPP
