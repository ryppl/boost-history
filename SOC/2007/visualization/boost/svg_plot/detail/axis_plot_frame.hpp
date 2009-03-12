 /*!  \file axis_plot_frame.hpp
  \brief SVG Plot functions common to 1D, 2D and Boxplots.
  \details Functions are derived from base class axis_plot_frame.
  \date Mar 2009
  \author Jacob Voytko and Paul A. Bristow
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2007, 2008, 2009

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_AXIS_PLOT_FRAME_HPP
#define BOOST_SVG_AXIS_PLOT_FRAME_HPP

#include "../svg_style.hpp"
#include "../svg.hpp"
#include "svg_tag.hpp"
#include "numeric_limits_handling.hpp"
// using boost::math::fpclassify for
// boost::math::
// template <class T>bool isfinite (T);
// template <class T>bool isinf (T);
// template <class T> bool isnan (T);
#include "uncertain.hpp"
// using boost::svg::unc;

#include <limits>
#include <string>
// using std::string;

#include <iostream> // for testing only.
 //using std::cerr;
 //using std::cout;
 //using std::endl;

#include <utility>
// using std::pair

namespace boost
{
  namespace svg
  {
    // JV: G++ can't resolve names in these enums in axis_plot_frame
    //    when they are in svg_2d_plot.hpp

    static const double sin45 = 0.707; //!< Use to calculate 'length' if axis value labels are sloping.
    static const double reducer = 0.9; //!< To make uncertainty and degrees of freedom testimates a bit smaller to distinguish from value.
    // (0.8 reduced from value 12, to 9 which is a bit too small).

    // x_axis_position_ and y_axis_position_ use x_axis_intersect & y_axis_intersect
    enum x_axis_intersect
    { //! \enum x_axis_intersect If and how the X axes intersects Y axis.
      bottom = -1, //!< X-axis free below bottom of end of Y-axis (case of all Y definitely < 0).
      x_intersects_y = 0, //!< x_intersects_y when Y values include zero, so X intersects the Y axis.
      top = +1 //!< X-axis free above top of X-axis (case of all Y definitely > 0).
      };

    enum y_axis_intersect
    { //! \enum y_axis_intersect  If and how the Y axes intersects X axis.
      left = -1, //!< Y-axis free to left of end of X-axis (case of all X definitely < 0).
      y_intersects_x = 0, //!< y_intersects_x when X values include zero, so intersects the X axis.
      right = +1 //!< Y-axis free to left of end of X-axis (case of all X definitely > 0).
     };

    enum legend_places
    { //! \enum legend_places Placing of legend box, if requested by legend_on == true.
      nowhere = 0, //!< Placing of legend box not requested or not calculated yet.
      inside = -1,  //!< Default place for inside is top left of plot window, (exact location controlled by legend_top_left()).
      outside_left = 1, //!< Outside on the left of the graph.
      outside_right = +2, //!< Outside right (Default).
      outside_top = +3, //!< Outside at top.
      outside_bottom = +4, //!< Outside at bottom.
      somewhere = +5 //!< legend_top_left(x, y)
    };

    namespace detail
    { //! \namespace detail Holds base class axis_plot_frame for 1D, 2D and Box plots.
      template <class Derived>
      class axis_plot_frame
      { /*! \class boost::svg::detail::axis_plot_frame
         \brief Used as base class for 1D, 2D and Box plots.
         \details For example, svg_1d_plot, svg_2d_plot, svg_boxplot\n
         class svg_1d_plot : public detail::axis_plot_frame<svg_1d_plot>\n
         class svg_2d_plot : public detail::axis_plot_frame<svg_2d_plot>
        */
        //protected:
      public:
        // We don't use the SVG coordinate transform because then text would
        // be flipped. Might use it to scale the image for resizes.

        // protected member functions (defined below):

        // void transform_point(double &x, double &y);
        // void transform_x(double &x);
        // void transform_y(double &y);
        // void draw_x_minor_tick(double j, path_element& tick_path, path_element& grid_path); // (& grid).
        // void draw_x_major_tick(double i, path_element& tick_path, path_element& grid_path); // (& grid).
        // void draw_x_axis();
        // void draw_legend();
        // void draw_title();
        // void adjust_limits(double& x, double& y);
        // void draw_plot_point(double x, double y, g_element& g_ptr, const plot_point_style& sty);
        // void draw_plot_point_value(double x, double y, g_element& g_ptr, value_style& val_style, plot_point_style& point_style, unc uvalue);
        // void draw_plot_point_values(double x, double y, g_element& x_g_ptr, g_element& y_g_ptr, const value_style& x_sty, const value_style& y_sty, unc uncx, unc uncy);


        // Clear functions.
        // void clear_all(); // Calls all the other clear_* functions.
        // void clear_legend();
        // void clear_background();
        // void clear_x_axis();
        // void clear_y_axis();
        // void clear_title();
        // void clear_points();
        // void clear_plot_background();
        // void clear_grids();
        void transform_point(double& x, double& y)
        { //! Scale & shift both X & Y to graph coordinates.
          x = derived().x_scale_ * x + derived().x_shift_;
          y = derived().y_scale_ * y + derived().y_shift_;
          adjust_limits(x, y); // In case either hits max, min, infinity or NaN.
        }
        // TODO do we need to adjust_limit(x); // In case hits max, min, infinity or NaN?
        // This implies that user's choice of X-axis range is wrong?
        // So more drastic action like throwing might be least worst action?

        void transform_x(double& x)
        { //! Scale and shift X value only.
          x = derived().x_scale_ * x + derived().x_shift_;
        }

        void transform_y(double& y)
        { //! Scale and shift Y value only.
          y = derived().y_scale_ * y + derived().y_shift_;
        }

        void draw_x_minor_tick(double value, path_element& tick_path, path_element& grid_path)
        { //! Draw X-axis minor ticks, and optional grid. (Value is NOT (yet) shown beside the minor tick).
          double x(value); // Tick position and value label,
          transform_x(x); // Convert to svg.
          double y_bottom(0.); // Start on the horizontal X-axis line.
          double y_top(derived().image.y_size()); // Image top.

          // Draw the minor grid, if wanted.
          if(derived().x_ticks_.minor_grid_on_)
          {
            if(!derived().plot_window_on_)
            { // Use whole image.
              // Make space for title and X-axis labels.
              if(derived().title_on_)
              { // Allow text_margin_ * font_size around text (pixels).
                y_bottom += derived().title_info_.textstyle().font_size() * derived().text_margin_;
              }
              if(derived().x_axis_.label_on_)
              {
                y_top -= derived().x_label_info_.textstyle().font_size() * derived().text_margin_;
              }
            }
            else
            { // plot_window_on_ == true.
              y_bottom = derived().plot_top_ + derived().plot_window_border_.width_; // Top.
              y_top = derived().plot_bottom_ - derived().plot_window_border_.width_; // Bottom. Ensure *inside* window.
            }
            // Make sure that we are drawing inside the allowed window.
            if((x >= derived().plot_left_) && (x <= derived().plot_right_)) // allow = too?
            {
              //std::cerr << "Writing draw_x_minor_tick grid inside plot window: x = "
              //  << x << ", plot_left_ = " << derived().plot_left_ << ", plot_right_ = " << derived().plot_right_ << std::endl;
              grid_path.M(x, y_bottom).L(x, y_top); // Draw grid line.
            }
            else
            { // This will happen but is designed to be ignored!
              // See comment in draw_x_axis
              // std::cerr << "Writing draw_x_minor_tick grid OUTside plot window: x = "
              //  << x << ", plot_left_ = " << derived().plot_left_ << ", plot_right_ = " << derived().plot_right_ << std::endl;
            }
          } // x_minor_grid

          // Draw x minor ticks.
          if (derived().x_ticks_.ticks_on_window_or_axis_ < 0)
          { // Put minor ticks on the plot window border bottom.
            y_bottom = derived().plot_bottom_; // on the window line.
            y_top = derived().plot_bottom_; // y_bottom = upper, y_top = lower end of tick.
          }
          else if (derived().x_ticks_.ticks_on_window_or_axis_ > 0)
          { // Put minor ticks on the plot window border top.
            y_bottom = derived().plot_top_; // on the window line.
            y_top = derived().plot_top_; // y_bottom = upper, y_top = lower end of tick.
          }
          else // derived().x_ticks_.ticks_on_window_or_axis_ == 0
          { // Internal style, draw tick up and/or down from the X-axis line.
            y_bottom = derived().x_axis_.axis_; // ON X-axis horizontal line.
            y_top = derived().x_axis_.axis_;
          }
          if(derived().x_ticks_.up_ticks_on_)
          {
            y_bottom -= derived().x_ticks_.minor_tick_length_; // up
          }
          if (derived().x_ticks_.down_ticks_on_)
          {
            y_top += derived().x_ticks_.minor_tick_length_; // down.
          }
         // Make sure that we are drawing inside the allowed window.
          if((x >= derived().plot_left_) && (x <= derived().plot_right_)) // TODO allow < or <=
          {
            tick_path.M(x, y_bottom).L(x, y_top);
            // No value labels on minor ticks, at present.
          }
          else
          { // This will happen but is designed to be ignored!
            // See comment in draw_x_axis
            //std::cerr << "Writing draw_x_minor_tick OUTside plot window: x = "
            // << x << ", plot_left_ = " << derived().plot_left_ << ", plot_right_ = " << derived().plot_right_ << std::endl;
          }
        } // void draw_x_minor_tick

        void draw_x_major_tick(double value, path_element& tick_path, path_element& grid_path)
        { //! Draw major ticks - and grid too if wanted.
          //! If major_value_labels_side then value shown beside the major tick.
          double x(value); //
          transform_x(x); // x value in svg.
          if((x < derived().plot_left_ - 0.01) || (x > derived().plot_right_ + 0.01))
          // Allow a bit extra to allow for round-off errors.
          { // tick value is way outside plot window, so nothing to do.
            //std::cout << derived().plot_left_ << ' '<< x << std::endl;
            // This *was* displayed for a plot.
            return;
          }
          double y_up(0.); // upper end of tick.
          double y_down(derived().image.x_size()); // y_down = lower end of tick.
          if(derived().x_ticks_.major_grid_on_)
          { // Draw major grid vertical line.
            if(!derived().plot_window_on_)
            { // Allow a modest margin around text of title and X-axis labels, if in use.
              if(derived().title_on_)
              {
                y_up += derived().title_info_.textstyle().font_size() * derived().text_margin_;
              }
              if(derived().x_ticks_.major_value_labels_side_ != 0)
              { // Value may be shown either side the major tick.
                y_down -= derived().x_label_info_.textstyle().font_size() * derived().text_margin_;
              }
            }
            else
            { // plot_window_on_ == true
              y_up = derived().plot_top_; // Bottom of plot window.
              y_down = derived().plot_bottom_; // Top of plot window.
            }
            //if((y_down <= derived().plot_bottom_) && (y_up >= derived().plot_top_) && (x >= derived().plot_left_) && (x <= derived().plot_right_))
            //{ // Make sure that we are drawing inside the allowed window.
              grid_path.M(x, y_up).L(x, y_down); // Vertical grid line.
            //}
          } // use_x_major_grid

          // Draw major tick (perhaps as well as grid - ticks might be wider than grid).
          // Make sure that we are drawing inside the allowed plot window.
          //if((x >= derived().plot_left_) && (x <= derived().plot_right_)) // now <=
          //{ Removed these checks as round off causes trouble.
            double x_tick_length = derived().x_ticks_.major_tick_length_;
            if(derived().x_ticks_.ticks_on_window_or_axis_ < 0)
            { // Put the ticks on the plot window border (was external).
              y_up = derived().plot_bottom_; // on the window line.
              y_down = derived().plot_bottom_; // y_up = upper, y_down = lower.
              if(derived().x_ticks_.up_ticks_on_)
              {
                y_up -= x_tick_length; // up
              }
              if (derived().x_ticks_.down_ticks_on_)
              {
                y_down += x_tick_length; // down.
              }
            }
            else if(derived().x_ticks_.ticks_on_window_or_axis_ > 0)
            { // Put the ticks on the plot window border (was external).
              y_up = derived().plot_top_; // on the window line.
              y_down = derived().plot_top_; // y_up = upper, y_down = lower.
              if(derived().x_ticks_.up_ticks_on_)
              {
                y_up -= x_tick_length; // up
              }
              if (derived().x_ticks_.down_ticks_on_)
              {
                y_down += x_tick_length; // down.
              }
            }
            else
            { // Draw tick from the central X axis line (Internal_style).
              y_up = derived().x_axis_.axis_; // X-axis line.
              y_down = derived().x_axis_.axis_;
              if(derived().x_ticks_.up_ticks_on_)
              {
                y_up -=  x_tick_length; // up
              }
              if (derived().x_ticks_.down_ticks_on_)
              {
                y_down += x_tick_length; // down.
              }
            }
            tick_path.M(x, y_up).L(x, y_down);
            // Leaving current position at the bottom end of the tick.
            // y_up and y-down are the ends of the tick.
            // These may be on the axis line, or the plot window.

            if(derived().x_ticks_.major_value_labels_side_ != 0)
            { // Show a value by the horizontal X-axis tick as "1.2" or "3.4e+000"...
              std::stringstream label;
              label.precision(derived().x_ticks_.value_precision_);
              label.flags(derived().x_ticks_.value_ioflags_);
              label << value; // "1.2" or "3.4e+000"...
              if (derived().x_ticks_.strip_e0s_)
              { // Remove unecessary e, +, leadings 0s.
                std::string v = strip_e0s(label.str());
                label.str(v);
              }

              double y = 0; // Where to start writing from, at end of bottom or top tick, if any.
              // = 0 is only to avoid unitialised warning.
              align_style alignment = center_align;
              // rotate_style rot = derived().x_ticks_.label_rotation_;  // TODO for debug only.
              // Adjustments to provide space from end of tick before or after writing label.
              if (derived().x_ticks_.label_rotation_ == upward) // vertical writing up.
              {  // Shift to center value digits and minus sign on tick.
                x += derived().x_value_label_style_.font_size() * 0.2;
                if (derived().x_ticks_.major_value_labels_side_ < 0)
                { // labels to bottom, so start a little below y_down.
                  y = y_down + derived().x_value_label_style_.font_size() * 0.6;
                  alignment = right_align;
                }
                else if(derived().x_ticks_.major_value_labels_side_ > 0)
                { // labels to top, so start a little above y_up.
                  y = y_up - derived().x_value_label_style_.font_size() * 0.5;
                  alignment = left_align;
                }
              }
              else if (derived().x_ticks_.label_rotation_ == downward)
              {
                x -= derived().x_value_label_style_.font_size() * 0.3;
                if (derived().x_ticks_.major_value_labels_side_ < 0)
                { // labels to bottom, so start a little below y_down.
                  y = y_down + derived().x_value_label_style_.font_size() * 0.5;
                  alignment = left_align;
                }
                else if(derived().x_ticks_.major_value_labels_side_ > 0)
                { // labels to top, so start a little above y_up.
                  y = y_up - derived().x_value_label_style_.font_size() * 0.5;
                  alignment = right_align;
                }
              }
              else if (derived().x_ticks_.label_rotation_ == uphill)
              { // Assume some 45 slope, so need about sqrt(2) less space.
                x += derived().x_value_label_style_.font_size() * 0.5;
                if (derived().x_ticks_.major_value_labels_side_ < 0)
                { // labels to bottom, so start a little to bottom of y_bottom.
                  y = y_down + derived().x_value_label_style_.font_size() * 0.7;
                  // Seems to need a bit more space for top than bottom if rotated.
                  alignment = right_align;
                }
                else if(derived().x_ticks_.major_value_labels_side_ > 0)
                { // labels to top, so start a little to top of y_top.
                  y = y_up - derived().x_value_label_style_.font_size() * 0.2;
                  alignment = left_align;
                }
              }
              else if (derived().x_ticks_.label_rotation_ == downhill)
              { // Assume some 45 slope, so need about sqrt(2) less space.
                x -= derived().x_value_label_style_.font_size() * 0.3;
                if (derived().x_ticks_.major_value_labels_side_ < 0)
                { // labels to bottom, so start a little to bottom of y_down.
                  y = y_down + derived().x_value_label_style_.font_size() * 0.7;
                  // Seems to need a bit more space for top than bottom if rotated.
                  alignment = left_align;
                }
                else if(derived().x_ticks_.major_value_labels_side_ > 0)
                { // labels to top, so start a little to top of y_up.
                 y = y_up - derived().x_value_label_style_.font_size() * 0.3;
                  alignment = right_align;
                }
              }
              else if (derived().x_ticks_.label_rotation_ == horizontal)
              { // Tick value label on x axis.
                if (derived().x_ticks_.major_value_labels_side_ < 0)
                { // labels to bottom, so start a little to bottom of y_down.
                  y = y_down + derived().x_value_label_style_.font_size() * 1.2;
                  alignment = center_align; // on the tick.
                }
                else if(derived().x_ticks_.major_value_labels_side_ > 0)
                { // labels to top, so start a little to top of y_up.
                 y = y_up - derived().x_value_label_style_.font_size() * 0.7;
                  alignment = center_align;
                }
              }
              else
              { // upsidedown, backup... - can't see any conceivable use for these.
                return; // Others not yet implemented.
              } // rotations
              if (x <= 0)
              { // Sanity checks on svg coordinates.
                throw std::runtime_error("X-tick X value wrong!");
              }
              if (y <= 0)
              {
                throw std::runtime_error("X-tick Y value wrong!");
              }

              if(derived().x_ticks_.ticks_on_window_or_axis_ != 0)
              { // External to plot window style bottom or top.
                // Always want all values including "0", if labeling external to plot window.
                // x_ticks_.ticks_on_window_or_axis_ == true != 0
                derived().image.g(detail::PLOT_VALUE_LABELS).text(
                  x,
                  y,
                  label.str(), derived().x_value_label_style_, alignment, derived().x_ticks_.label_rotation_);
              }
              else
              {
                if ((value != 0) && derived().x_axis_.axis_line_on_)
                { // Avoid a "0" below the X-axis if it would be cut through by any internal vertical Y-axis line.
                  derived().image.g(detail::PLOT_VALUE_LABELS).text(
                    x,
                    y,
                    label.str(),
                    derived().x_value_label_style_,
                    alignment,
                    derived().x_ticks_.label_rotation_);
                }
              } // on plot window or 'on axis'.
          }
          else
          { // Outside plot window - so do nothing?  Warning?
            //std::cerr << "Writing draw_x_major_tick OUTside plot window: "
            //  "x = " << x << ", plot_left_ = " << derived().plot_left_ << ", plot_right_ = " << derived().plot_right_ << std::endl;
          }
        } // draw_x_major_tick

        void draw_x_axis()
        { //! Draw horizontal X-axis line & plot window line to hold.
          if(derived().x_axis_.axis_line_on_)
          { // Want a horiztonal X-axis line drawn.
            double xleft = derived().plot_left_;
            double xright = derived().plot_right_;
            if (derived().x_axis_position_ == x_intersects_y)
            { // Draw the horizontal X-axis line the full width of the plot window,
              // perhaps including an addition in lieu of a major tick.
              if (derived().y_ticks_.left_ticks_on_)
              {
                if (derived().y_ticks_.ticks_on_window_or_axis_ < 0) // left
                { // Extend the horizontal line left in lieu of longest tick.
                  xleft -= (std::max)(derived().y_ticks_.minor_tick_length_, derived().y_ticks_.major_tick_length_);
                }
              }
              else if (derived().y_ticks_.right_ticks_on_)
              {
                if (derived().y_ticks_.ticks_on_window_or_axis_ > 0) // right
                { // Extend the horizontal line right in lieu of longest tick.
                  xright += (std::max)(derived().y_ticks_.minor_tick_length_, derived().y_ticks_.major_tick_length_);
                }
              }
              double y = derived().x_axis_.axis_; // y = 0, (provided y range includes zero).
              derived().image.g(PLOT_X_AXIS).line(xleft, y, xright, y);
              if (derived().x_ticks_.ticks_on_window_or_axis_ < 0) // bottom
              { // Draw a vertical line holding the ticks on the top of plot window.
                derived().image.g(PLOT_X_AXIS).line(xleft, derived().plot_bottom_, xright, derived().plot_bottom_);
              }
              else if (derived().x_ticks_.ticks_on_window_or_axis_ > 0)  // top
              {// Draw a vertical line holding the ticks on the bottom of plot window.
                derived().image.g(PLOT_X_AXIS).line(xleft, derived().plot_top_, xright, derived().plot_top_);
              }
            }
            else if (derived().x_axis_position_ == top)
            {
               derived().image.g(PLOT_X_AXIS).line(xleft, derived().plot_top_, xright, derived().plot_top_);
            }
            else if (derived().x_axis_position_ == bottom)
            {
               derived().image.g(PLOT_X_AXIS).line(xleft, derived().plot_bottom_, xright, derived().plot_bottom_);
            }
            else
            { // warn that things have gone wrong?
            }
          } // x_axis_.axis_line_on_

          // Access the paths for the ticks & grids, ready for additions.
          path_element& minor_tick_path = derived().image.g(PLOT_X_MINOR_TICKS).path();
          path_element& major_tick_path = derived().image.g(PLOT_X_MAJOR_TICKS).path();
          path_element& minor_grid_path = derived().image.g(PLOT_X_MINOR_GRID).path();
          path_element& major_grid_path = derived().image.g(PLOT_X_MAJOR_GRID).path();

          // x_minor_jump is the interval between minor ticks.
          double x_minor_jump = derived().x_ticks_.major_interval_ /
            (derived().x_ticks_.num_minor_ticks_ + 1.);

          // Draw the ticks on the positive side (right of zero).
          for(double x = 0.; x <= derived().x_axis_.max_; x += derived().x_ticks_.major_interval_)
          {
            for(double j = x + x_minor_jump;
              j < (x + derived().x_ticks_.major_interval_) * (1. - 2 * std::numeric_limits<double>::epsilon());
              j += x_minor_jump)
              // Reduce test value by a few bits to avoid accumulated rounding error
              // that intermittently puts minor ticks *at same value as* major ticks.
            { // This will output 'orphaned' minor ticks that are beyond the plot window,
              // if the last major tick does not coincide with the plot window.
              // These are just ignored in draw_x_minor_tick.
              // There might be 9 of them, so
              // if you have the common 9 minor tick between major ticks!
              // TODO this seems ugly - as does the negative ones below.
              draw_x_minor_tick(j, minor_tick_path, minor_grid_path);
            } // for j
            if ((x != 0. || !derived().y_axis_.axis_line_on_) || (derived().x_ticks_.ticks_on_window_or_axis_ != 0))
            { // Avoid a major tick at x == 0 where there *is* a vertical Y-axis line.
              // (won't be Y-axis line for 1-D where the zero tick is always wanted).
              draw_x_major_tick(x, major_tick_path, major_grid_path);
            }
          }

          // Draw the ticks on the negative side (left of zero).
          for(double x = 0.; x >= derived().x_axis_.min_; // ?? * (1. + 2 * std::numeric_limits<double>::epsilon());
            x -= derived().x_ticks_.major_interval_) // Want a close to test here?
          {
            // Draw minor ticks.
            for(double j = x - x_minor_jump;
              j > (x - derived().x_ticks_.major_interval_ + x_minor_jump) * (1. + 2 * std::numeric_limits<double>::epsilon());
              // Increase test value by a few bits to avoid accumulated rounding error
              // that intermittently puts minor ticks *at same value as* major ticks.
              j -= x_minor_jump)
            {
              if ((j != 0. || !derived().y_axis_.axis_line_on_)  || (derived().x_ticks_.ticks_on_window_or_axis_ != 0))
              { // Avoid a minor tick at x == 0 where there *is* a vertical Y-axis line.
                // (won't be Y-axis line for 1-D where the zero tick is always wanted).
                // But no tick means no value label 0 either unless on_plot_window.
                draw_x_minor_tick(j, minor_tick_path, minor_grid_path);
              }
            }
            if ((x != 0. || !derived().y_axis_.axis_line_on_) || (derived().x_ticks_.ticks_on_window_or_axis_ != 0))
            { // Avoid a major tick at x == 0 where there *is* a vertical Y-axis line.
              // (won't be Y-axis line for 1-D where the zero tick is always wanted).
              // But no tick means no value label 0 either unless on_plot_window.
              draw_x_major_tick(x, major_tick_path, major_grid_path);
            }
          }
        } // void draw_x_axis()

        void draw_title()
        { /*! Draw title (for the whole plot).
            Update title_info_ with position.
            Assumes align = center_align
            Using center_align will ensure that will title center correctly
            (even if original string is made much longer because it contains Unicode,
            for example Greek, taking about 6 characters per symbol)
            because the render engine does the centering.
          */
          derived().title_info_.x(derived().image.x_size() / 2.); // Center of image.
          double y;
          y = derived().title_info_.textstyle().font_size() * derived().text_margin_; // Leave a linespace above.
          derived().title_info_.y(y);
          derived().image.g(PLOT_TITLE).push_back(new text_element(derived().title_info_));
        } // void draw_title()


        void size_legend_box()
        { //! Calculate how big the legend box needs to be.
          if(derived().legend_on_ == false)
          { // No legend wanted, so set values to show legend positions invalid?
            //derived().legend_left_ = -1.;
            //derived().legend_right_ = -1.;
            //derived().legend_top_ = -1.;
            //derived().legend_bottom_ = -1.;
            derived().legend_height_ = 0.; // At least set the size to zero.
            derived().legend_width_ = 0.;
            return;
          }
          else
          { // legend_on_ == true
            // Work out the size the legend box needs to be to hold the
            // header, markers, lines & text.
            size_t num_series = derived().serieses_.size(); // How many data series.
            int font_size = derived().legend_header_.textstyle().font_size();
            int point_size =  derived().serieses_[0].point_style_.size();
            // Use height of whichever is the biggest of point marker and font.
            double spacing = (std::max)(font_size, point_size);
            // std::cout << spacing <<  ' ' << font_size << ' ' << point_size << std::endl;
            bool is_header = (derived().legend_header_.text() != "");
            //text_element legend_header_; // legend box header or title (if any).
            //text_style legend_style_;
            double longest = string_svg_length(derived().legend_header_.text(), derived().legend_style_);
            //std::cout << "\nLegend header " << longest << " svg units." << std::endl;
            derived().legend_width_ = 2 * (derived().legend_box_.margin() * derived().legend_box_.width());
            // Don't plan to write on either side border, or on the 'forbidden' margins of the box.
            for(size_t i = 0; i < num_series; ++i)
            { // Find the longest text (if longer than header) in all the data series.
              std::string s = derived().serieses_[i].title_;
              double siz = string_svg_length(s, derived().legend_style_);
              if (siz > longest)
              {
                longest = siz;
              }
            } // for
            // std::cout << "\nLongest legend header or data descriptor " << longest << " svg units." << std::endl;
            derived().legend_width_ += longest * 0.8; // Space for longest text.
            // Kludge factor to allow for not knowing the real length.

            // Allow width for a leading space, and trailing
              derived().legend_width_ += spacing * 2.5;

            // & trailing space before box margin.
            // if (derived().serieses_[i].line_style_.line_on_) // line joining points.
            if (derived().legend_lines_)
            { // Add for colored line marker in legend.
              derived().legend_width_ += spacing * 1.5;
            }
            if(derived().serieses_[0].point_style_.shape() != none)
            { // Add for any colored data point marker, cross, round... & space.
              derived().legend_width_ += 1.5 * derived().serieses_[0].point_style_.size();
            }
            // else no point marker.

            // legend_height must be *at least* enough for
            // any legend header and text_margin(s) around it
            // (if any) plus a text_margin_ top and bottom.
            // Add more height depending on the number of lines of text.
            derived().legend_height_ = spacing; // At top
            if ( (is_header) // is a legend header line.
              && (derived().legend_header_.text() != "") )
            {
              derived().legend_height_ += 2 * font_size; // text & space after.
            }
            derived().legend_height_ += num_series * spacing * 2; // Space for the data point symbols & text.
          } // legend_on_ == true

         //std::cout << "Legend width " << derived().legend_width_ << ", height " << derived().legend_height_ << std::endl;
        } //  void size_legend_box()

        void place_legend_box()
        { //! Place legend box (if required).
          if(derived().legend_on_ == true) // Legend box required.
          {
            derived().outside_legend_on_ = true; // Unless proves to be inside.
            //double spacing = derived().legend_box_.margin(); // Might be better to use this, but needs redoing.
            double spacing = derived().y_axis_label_style_.font_size() * 1.; // Around any legend box - beyond any border.
            switch (derived().legend_place_)
            {
            case nowhere:
              return; // Actually places it at (0, 0), probably overwriting the plot.
            case somewhere:
              // Assume legend_top_left will place it somewhere where there is nothing else.
               //derived().legend_left_; set by legend_top_left
               //derived().legend_top_;
               derived().legend_bottom_ = derived().legend_top_ + derived().legend_height_;
               derived().legend_right_ = derived().legend_left_ + derived().legend_width_;
               break;
            case inside:
              derived().outside_legend_on_ = false;
              if (derived().legend_left_ == -1)
              { // Legend box position NOT been set by legend_top_left.
                // Default inside position is top left level with plot window.
              derived().legend_left_ = derived().image_border_.width_ + derived().image_border_.margin_; // left image edge + space.
              derived().plot_left_ += derived().legend_width_ + spacing; // Push plot window right same amount to make room,
              derived().legend_right_ = derived().legend_left_ + derived().legend_width_;
              derived().legend_top_ = derived().plot_top_; // Level with top of plot window.
              derived().legend_bottom_ = derived().legend_top_ + derived().legend_height_;
            }
             else
             { // Legend position has been specified by legend_top_left.
                break;
             }
             break;
                // If outside then reserve space for legend by reducing plot window.
            case outside_right: // Default legend position is outside_right,
              // so that it isn't too close to the image edge or the plot window.
              derived().plot_right_ -= derived().legend_width_ + spacing; // Narrow plot window from right.
              derived().legend_left_ = derived().plot_right_  + spacing; // plot + border.
              derived().legend_right_ = derived().legend_left_ + derived().legend_width_;
              derived().legend_top_ = derived().plot_top_; // Level with top of plot window.
              derived().legend_bottom_ = derived().legend_top_ + derived().legend_height_;
              break;
            case outside_left:
              derived().plot_left_ += derived().legend_width_ + spacing /2 ; // Push plot window right same amount to make room,
              derived().legend_left_ = derived().image_border_.width_ + derived().image_border_.margin_; // left image edge + space.
              derived().legend_right_ = derived().legend_left_ + derived().legend_width_;
              derived().legend_top_ = derived().plot_top_; // Level with top of plot window.
              derived().legend_bottom_ = derived().legend_top_ + derived().legend_height_;
             break;
            case outside_top:
              // Centered.
               derived().legend_left_ = derived().image.x_size() / 2. - derived().legend_width_ / 2; // Center.
               derived().legend_right_ = derived().legend_left_ + derived().legend_width_;
               derived().plot_top_ += derived().legend_height_ + spacing;
               derived().legend_top_ = derived().title_info_.y() + derived().title_font_size() * derived().text_margin_;
               derived().legend_top_ += spacing;
               derived().legend_bottom_ = derived().legend_top_ + derived().legend_height_;
              break;
            case outside_bottom:
               // Centered.
               derived().legend_bottom_ = derived().image.y_size();
               derived().legend_bottom_ -= (derived().image_border_.width_ + derived().image_border_.margin_); // up
               derived().legend_top_ = derived().legend_bottom_ - derived().legend_height_;
               derived().legend_left_ = derived().image.x_size()/  2. - derived().legend_width_ / 2; // Center.
               derived().legend_right_ = derived().legend_left_ + derived().legend_width_;
               derived().plot_bottom_ = derived().legend_top_;
               derived().plot_bottom_ -= 2 * spacing;
              break;
              } // switch

            //std::cout << "Legend: left " << derived().legend_left_
            //    << ", right " << derived().legend_right_
            //    << ", top " << derived().legend_top_
            //    << ", bottom " << derived().legend_bottom_
            //    << std::endl;

              // Check if the location requested will fit,
              // now that we know the size of box needed.
              if ( (derived().legend_left_ < 0) || (derived().legend_left_ > derived().image.x_size()))
              { // left outside image?
                std::cout << "Legend top left " << derived().legend_left_
                  << " is outside image size = " << derived().image.x_size() << std::endl;
              }
              if ((derived().legend_right_ < 0) || (derived().legend_right_ > derived().image.x_size()))
              { // right outside image?
                std::cout << "Legend top right " << derived().legend_right_
                  << " is outside image size = " << derived().image.x_size() << std::endl;
              }
              if ((derived().legend_top_ < 0) || (derived().legend_top_ > derived().image.y_size()))
              { // top outside image?
                std::cout << "Legend top " << derived().legend_top_
                  << " outside image!" << derived().image.y_size() << std::endl;
              }
              if ((derived().legend_bottom_  < 0 ) || (derived().legend_bottom_ > derived().image.y_size()))
              { // bottom outside image?
                std::cout << "Legend bottom " << derived().legend_bottom_
                  << " outside " << derived().image.y_size() << std::endl;
              }

               derived().image.g(detail::PLOT_LEGEND_BACKGROUND)
              .style().fill_color(derived().legend_box_.fill()) //
              .stroke_color(derived().legend_box_.stroke())
              .stroke_width(derived().legend_box_.width())
              .stroke_on(derived().legend_box_.border_on())
              ;

              // Draw border box round legend.
              g_element* g_ptr = &(derived().image.g(PLOT_LEGEND_BACKGROUND));
              g_ptr->push_back(new
                rect_element(derived().legend_left_, derived().legend_top_, derived().legend_width_, derived().legend_height_));
            } // if legend_on_
          } //  void calculate_legend_box()

          void draw_legend()
          { //! Draw the legend border, text header (if any) and marker lines and/or shapes.
            // size_t num_points = derived().series.size();
            //cout << derived().legend_box_.width() <<  ' ' << derived().legend_box_.margin() << endl;

            int font_size = derived().legend_header_.textstyle().font_size();
            int point_size =  derived().serieses_[0].point_style_.size();
            // Use whichever is the biggest of point marker and font.
            double spacing = (std::max)(font_size, point_size);
            // std::cerr << spacing <<  ' ' << font_size << ' ' << point_size << endl;
            bool is_header = (derived().legend_header_.text() != "");

           // Assume legend box position has already been sized and positioned by function calculate_legend_box.
            double legend_x_start = derived().legend_left_; // Saved box location.
            double legend_width = derived().legend_width_;
            double legend_y_start = derived().legend_top_;
            double legend_height = derived().legend_height_;

            // Draw border box round legend.
            g_element* g_ptr = &(derived().image.g(PLOT_LEGEND_BACKGROUND));

            g_ptr->push_back(new
              rect_element(legend_x_start, legend_y_start, legend_width, legend_height));

            double legend_y_pos = legend_y_start + derived().text_margin_ * spacing;
            if (is_header)
            { // Draw the legend header text for example: "My Plot Legend".
              derived().legend_header_.x(legend_x_start + legend_width / 2.); // / 2. to center in legend box.
              // Might be better to use center_align here because will fail if legend contains symbols in Unicode.
              derived().legend_header_.y(legend_y_pos);
              derived().image.g(PLOT_LEGEND_TEXT).push_back(new text_element(derived().legend_header_));
              legend_y_pos += 2 * spacing; // Might be 1.5? - useful if many series makes the box too tall.
            }

            g_ptr = &(derived().image.g(PLOT_LEGEND_POINTS));
            g_element* g_inner_ptr = g_ptr;
            g_inner_ptr = &(derived().image.g(PLOT_LEGEND_TEXT));

            for(unsigned int i = 0; i < derived().serieses_.size(); ++i)
            { // Show point marker, perhaps line, & text info for all the data series.

              //cout << "Series " << i << endl;
              // derived().serieses_[i].point_style_ << endl;
              // cout << derived().serieses_[i].line_style_ << endl;

              double legend_x_pos = legend_x_start;
              legend_x_pos += spacing; // space before point marker and/or line & text.
              g_inner_ptr = &(g_ptr->g());
              // Use both stroke & fill colors from the points' style.
              // Applies to both shape AND line.
              g_inner_ptr->style().stroke_color(derived().serieses_[i].point_style_.stroke_color_);
              g_inner_ptr->style().fill_color(derived().serieses_[i].point_style_.fill_color_);
              g_inner_ptr->style().stroke_width(derived().serieses_[i].line_style_.width_);

              //cout << "g_inner_ptr.style().stroke_color() " << g_inner_ptr->style() << endl;
              if(derived().serieses_[i].point_style_.shape_ != none)
              { // Is some data marker shape to show.
                draw_plot_point( // Plot point like circle, square, vertical bar...
                  legend_x_pos,
                  legend_y_pos,
                  *g_inner_ptr,
                  derived().serieses_[i].point_style_);
                legend_x_pos += 1.5 * spacing;
              }

              // Line markers are only really useful for 2-D lines and curves showing functions.
              if (derived().legend_lines_)
              { // Need to draw a short line to show color for that data series.
                  // Line joining points option is true.
                  if (derived().serieses_[i].line_style_.line_on_ || derived().serieses_[i].line_style_.bezier_on_)
                  { // Use stroke color from line style.
                     g_inner_ptr->style().stroke_color(derived().serieses_[i].line_style_.stroke_color_);
                  }
                  else
                  { // Use point stroke color instead.
                    g_inner_ptr->style() .stroke_color(derived().serieses_[i].point_style_.stroke_color_); // OK with 1D
                  }
                  //std::cout << "line g_inner_ptr->style().stroke_color() " << g_inner_ptr->style().stroke_color() << std::endl;

                  g_inner_ptr->push_back(new line_element( // Draw horizontal lines with appropriate color.
                    legend_x_pos,
                    legend_y_pos,
                    legend_x_pos + spacing, // Line sample is one char long.
                    legend_y_pos));
                  legend_x_pos += 1.5 * spacing; // Total is short line & a space.
              } // legend_lines_

              // Legend text for each Data Series added to the plot.
              g_inner_ptr = &(derived().image.g(PLOT_LEGEND_TEXT));
              g_inner_ptr->push_back(new text_element(
                legend_x_pos, // allow space for the marker.
                legend_y_pos,
                derived().serieses_[i].title_, // Text for this data series.
                derived().legend_header_.textstyle(),
                left_align));
              legend_y_pos += 2 * spacing;
            } // for
          } // void draw_legend()

          void draw_x_label()
          { //! Draw the X-axis label text (for example, length), and append any required units (for example. km).
            // X-label color is set in constructor thus:
            // image.g(detail::PLOT_X_LABEL).style().stroke_color(black);
            // and changed using x_label_color(color);


            std::string label = derived().x_label_info_.text(); // x_axis_ label, and optional units.
            if (derived().x_axis_.label_units_on_ && (derived().x_units_info_.text() != ""))
            { // Append the units, if any, user providing brackets () if required.
              label += derived().x_units_info_.text();
            }

            double y = derived().plot_bottom_;
            y += derived().x_ticks_.value_label_style_.font_size() * 2.; // Shift down to suit.
            if (derived().x_ticks_.ticks_on_window_or_axis_ < 0) // bottom
            { // Ticks & value labels below X-axis.
              if (derived().x_ticks_.major_value_labels_side_ < 0) // bottom
              { // Shift down to allow for any value labels.
                y += derived().x_ticks_.label_max_space_;
              }
              if (derived().x_ticks_.down_ticks_on_)
              { // Shift down for biggest of any ticks.
                y += (std::max)(derived().x_ticks_.minor_tick_length_, derived().x_ticks_.major_tick_length_);
              }
            }

            derived().image.g(PLOT_X_LABEL).push_back(new text_element(
              ( // x position relative to the x-axis which is middle of plot window.
              derived().plot_right_ + derived().plot_left_) / 2,  // x coordinate - middle.
              y, // Down from plot window.
              label,
              derived().x_label_info_.textstyle(),
              center_align, horizontal)
              );
          } // void draw_x_label()

          void adjust_limits(double& x, double& y)
          { //! If value of a data point reaches limit of max, min, infinity,
            //! use the appropriate plot min or max value instead.
            if(detail::limit_max(x))
            {
              x = derived().plot_right_;
            }
            if(detail::limit_max(y))
            {
              y = derived().plot_top_;
            }
            if(detail::limit_min(x))
            {
              x = derived().plot_left_;
            }
            if(detail::limit_min(y))
            {
              y = derived().plot_top_;
            }
            // If value is NaN, use zero instead.
            // TODO Do we want/get a different color or shape for NaNs?
            if(detail::limit_NaN(x))
            {
              x = 0.;
              transform_x(x);
            }
            if(detail::limit_NaN(y))
            {
              y = 0.;
              transform_y(y);
            }
          } // void adjust_limits

          void draw_plot_point(double x, double y, // SVG coordinates.
            g_element& g_ptr,
            plot_point_style& sty)
          { //! Draw a plot data point marker shape whose size and stroke and fill colors are specified in plot_point_style.
            /*
              For 1-D plots, the points do not *need* to be centered on the X-axis,
              and putting them just above, or sitting on, the X-axis is much clearer.
              For 2-D plots, the symbol center should, of course,
              be centered exactly on x, y.
              circle and ellipse are naturally centered on the point.
              for rect x and y half_size offset centers square on the point.
              But symbols are in a rectangular box and the offset is different for x & y
              even assuming that the symbol is centered in the rectangle.
              the vertical and horizontal ticks are deliberately offset above the axes.
              TODO Not sure this is fully resolved.
            */
            int size = sty.size_;
            double half_size = size / 2.;


            //cout << "point style() "<< sty.style() << endl;
            // Whatever shape, text or line, want to use the point style.
            g_ptr.style().stroke_color(sty.stroke_color_);
            g_ptr.style().fill_color(sty.fill_color_);

            //cout << "g_ptr.style() " << g_ptr.style() << endl;

            switch(sty.shape_) // from enum point_shape none, round, square, point, egg
            {
            case round:
              g_ptr.circle(x, y, (int)half_size);
              break;
            case square:
              g_ptr.rect(x - half_size, y - half_size, size, size);
              break;
            case egg:
              g_ptr.ellipse(x, y, half_size, size * 2.); // Tall thin egg!
              break;

             // Offset from center is not an issue with vertical or horizontal ticks.

            case vertical_tick: // Especially neat for 1-D points.
              g_ptr.line(x, y, x , y - size); // tick up from axis.
              break;
            case vertical_line:
              g_ptr.line(x, y + size, x , y - size); // line up & down from axis.
              break;
            case horizontal_tick:
              // horizontal_tick is pretty useless for 1-D because the horizontal line is on the X-axis.
              g_ptr.line(x, y, x + size, y ); // tick right from axis.
              break;
            case horizontal_line:
              g_ptr.line(x, y - size, x + size, y ); // line left & right from axis.
              // horizontal_line is pretty useless for 1-D because the horizontal line is on the X-axis.
              break;
            case symbol:
              g_ptr.text(x, y + half_size, sty.symbols(), sty.style(), center_align, horizontal); // symbol(s), size and centre.
              // TODO Need to provide way to set style.symbols when Boost.Parameter is unravelled.

              // Unicode symbols that work on most browsers are listed at
              // boost\math_toolkit\libs\math\doc\sf_and_dist\html4_symbols.qbk,
              // http://www.htmlhelp.com/reference/html40/entities/symbols.html
              // and  http://www.alanwood.net/demos/ent4_frame.html
              // The Unicode value in decimal 9830 or hex x2666 must be prefixed with & and terminated with ;
              // for example &x2666; for xml
              // and then enveloped with "" to convert to a std::string, for example: "&#x2666;" for diamond.

              break;
            case diamond:
              g_ptr.text(x, y, "&#x2666;", sty.symbols_style_, center_align, horizontal);
              // size / 4. puts bottom tip on the X-axis,
              // size / 2. put center above the X-axis
              // x, y, put on the X-axis - probably what is needed for 2-D plots.
              // diamond, spades, clubs & hearts fill with expected fill_color.
              break;
            case asterisk:
              g_ptr.text(x, y - size / 3., "&#x2217;", sty.symbols_style_, center_align, horizontal);
              // asterisk is black filled.
              // size /3 puts the bottom tip on the X-axis.
              break;
            case lozenge:
              g_ptr.text(x, y - size / 3., "&#x25CA;", sty.symbols_style_, center_align, horizontal);
              // size / 3 to get tip of lozenge just on the X-axis.
              // lozenge seems not to fill?
              break;
            case club:
              g_ptr.text(x, y, "&#x2663;", sty.symbols_style_, center_align, horizontal);
              // x, y, puts club just on the X-axis
              break;
            case spade:
              g_ptr.text(x, y, "&#x2660;", sty.symbols_style_, center_align, horizontal);
              //
              break;
            case heart:
              g_ptr.text(x, y , "&#x2665;", sty.symbols_style_, center_align, horizontal);
              //
              break;
            case cone: // Pointing down triangle.
              {
                bool fill = (sty.fill_color() != blank);
                g_ptr.triangle(x - half_size, y - size, x + half_size, y - size, x, y, fill);
              // Last point puts the bottom tip of the triangle on the X-axis (may not be wanted for 2-D).
              }
              break;
            case cross: // Not X.
              g_ptr.line(x, y + size, x , y - size); // line up & down from axis,
              g_ptr.line(x, y - size, x + size, y ); // & line left & right from axis.
              // Cross is pretty useless for 1-D because the horizontal line is on the X-axis.
              break;
              // TODO Other point_shapes do nothing yet.
            }
          } // void draw_plot_point

          void draw_plot_point_value(double x, double y, g_element& g_ptr, value_style& val_style, plot_point_style& point_style, unc uvalue)
          { /*! Write one data point (X or Y) value as a string, for example "1.23e-2", near the data point marker.
             Unecessary e, +, \& leading exponent zeros may optionally be stripped, and the position and rotation controlled.
             Uncertainty estimate ('plus or minus') may be optionally be appended.
             Degrees of freedom estimate (number of replicates) may optionally be appended.
             For example: "3.45 +-0.1(10)"\n
             The precision and format (scientific, fixed), and color and font type and size can be controlled too.
             */
            double value = uvalue.value(); // Most likely value.
            double u = uvalue.uncertainty(); // Uncertainty or plusminus for value.
            double df = uvalue.deg_free(); // Degrees of freedom estimate for value.

            std::stringstream label;
            label.precision(val_style.value_precision_);
            label.flags(val_style.value_ioflags_);
            label << value; // "1.2" or "3.4e+001"...
            std::string stripped =  (derived().x_ticks_.strip_e0s_) ?
              // Default is to strip unecessary e, +, & leading exponent zeros.
              strip_e0s(label.str())  // "1.2" or "3.4e1"...
              :
              stripped = label.str();
            if (val_style.prefix_ != "")
            { // Want a prefix like "[" or "time = ".
              stripped = val_style.prefix_ + stripped;
            }
            int marker_size = point_style.size_; // point marker size
            int label_size = val_style.values_text_style_.font_size();
            // Offset of value label from point must be related mainly to
            // size of the data marker, less the value label font size.
            // May need to combine these two?

            rotate_style rot = val_style.value_label_rotation_;
            // http://www.w3.org/TR/SVG/coords.html#RotationDefined
            // transform="rotate(-45)" == uphill

            align_style al; // = center_align;
            switch (rot)
            {
            case horizontal: // OK
              al = center_align;
              y -= marker_size * 2;  // Up marker font size;
              // center_align means no x correction.
              break;
            case leftward: // horizontal but to left of marker.
              al = right_align;
              x -= marker_size * 1.3;  // left
              y += label_size * 0.3;  // down label font size;
              rot = horizontal;
              break;
            case rightward: // horizontal but to right of marker.
              al = left_align;
              x += marker_size * 1.1;  // right
              y += label_size * 0.3;  // down label font size;
              rot = horizontal;
              break;
            case upsidedown: // OK but upsidedown so not very useful!
              al = center_align;
              y += marker_size;  // Down marker font size;
             break;
            case slopeup: // -30 - OK
            case steepup: // -45 - OK
            case uphill: // -60 - OK
              al = left_align;
              x += label_size /3;  // Right third label font size - centers on marker.
              y -= marker_size * 0.6;  // UP marker font size;
              break;
            case upward: // -90 vertical writing up - OK.
              al = left_align;
              x += label_size /3;  // Right third label font size - centers on marker.
              y -= marker_size * 0.9;  // Up marker font size;
              break;
            case backup: // OK
              al = right_align;
              x -= marker_size * 1.5;  // Left
              y -= marker_size * 0.8;  // Up
              rot = downhill;
              break;

            case  slopedownhill: // 30 gentle slope down.
            case downhill: // 45 down.
            case steepdown: //  60 steeply down.
             al = left_align;
              x += marker_size * 0.4;  // Right;
              y += marker_size * 0.9;  // Down
              break;
            case downward: // OK
              al = left_align;
              x -= marker_size;  // Left
              y += marker_size;  // Up
             break;
            case backdown: // OK
              al = right_align;
              x -= marker_size * 0.5;  // Left
              y += marker_size * 1.5;  // down
              rot = uphill;
             break;
            } // switch
            text_element& t = g_ptr.text(x, y, stripped, val_style.values_text_style_, al, rot);  // X or Y value "1.23".
            int udf_font = static_cast<int>(val_style.values_text_style_.font_size() * reducer);
          
            std::string label_u; // Uncertainty or plusminus.
            std::string label_df; // Degrees of freedom estimate.
            std::string pm = "&#x00A0;&#x00B1;"; //! Unicode space plusminus glyph.
            // Might also use ANSI symbol for plusminus 0xF1 == '\361' or char(241)
            // but seems to vary with different codepages
            // LOCALE_SYSTEM_DEFAULT LOCALE_IDEFAULTANSICODEPAGE == 1252
            // LOCALE_SYSTEM_DEFAULT  LOCALE_IDEFAULTCODEPAGE ==  850 for country 44 (UK)
            // And seems to vary from console to printable files.
            // Spaces seem to get lost, so use 00A0 as an explicit space glyph.
            // Layout seems to vary with font - Times New Roman leaves no space after.
            //text_element& t = g_ptr.text(x, y, label_v, val_style.values_text_style_, al, rot);
           // Optionally, show uncertainty as 95% confidence plus minus:  2.1 +-0.012 (23)

            if ((val_style.plusminus_on_ == true) // Is wanted.
                && (u > 0.) // Is a valid uncertainty estimate.
              )
            {  // Uncertainty estimate usually 95% confidence interval + or - 2 standard deviation.
              label_u = sv(u, val_style, true); // stripped.
              t.tspan(pm).fill_color(val_style.plusminus_color_);
              t.tspan(label_u).fill_color(val_style.plusminus_color_).font_size(udf_font);
            }
            if (val_style.df_on_ == true // Is wanted.
                  && (df != (std::numeric_limits<unsigned short int>::max)()) // and deg_free is defined OK.
                )
            { // Degrees of freedom or number of values-1 used for this estimate of value.
              std::stringstream label;
              label.precision(4); // Might need 5 to show 65535?
              //label.flags(sty.value_ioflags_); // Leave at default.
              label << "&#x00A0;(" << df << ")"; // "123"
              // Explicit space "\&#x00A0;" seems necessary.
              label_df = label.str();
              t.tspan(label_df).fill_color(val_style.df_color_).font_size(udf_font);
            }
            if (val_style.suffix_ != "")
            { // Want a suffix like "]" or " sec]".
              t.tspan(val_style.suffix_);
            }

          } // void draw_plot_point_value(double x, double y, g_element& g_ptr, double value)

          std::string sv(double v, const value_style& sty, bool precise = false)
          { //! Strip from double value any unecessary e, +, & leading exponent zeros, reducing "1.200000" to "1.2" or "3.4e1"...
            // TODO rename fo strip_value?
            std::stringstream label;
            // Precision of uncertainty is usually less than precision of value,
            // label.precision((unc) ? ((sty.value_precision_ > 3) ?  sty.value_precision_-2 : 1) : sty.value_precision_);
            // Possible but simpler to fix at precision=2
            label.precision((precise) ? 2 : sty.value_precision_);
            label.flags(sty.value_ioflags_);
            label << v; // "1.2" or "3.4e+001"...
            return  (sty.strip_e0s_) ?
              // Default is to strip unecessary e, +, & leading exponent zeros.
              strip_e0s(label.str())  // reduce to "1.2" or "3.4e1"...
              :
              label.str();  // Leave unstripped.
          } // std::string sv(double v, const value_style& sty)

          void draw_plot_point_values(double x, double y, g_element& x_g_ptr, g_element& y_g_ptr, const value_style& x_sty, const value_style& y_sty, unc uncx, unc uncy)
          { /*! \brief Write the \b pair of data point's X and Y values as a string.
               \details If a separator, then both on the same line, for example "1.23, 3.45", or "[5.6, 7.8]
               \verbatim
                 X value_style is used to provide the prefix and separator, and Y value_style to provide the suffix.
                 For example, x_style prefix("[ X=", and separator ",<ampersand>#x00A0;Y= ", " and Y value_style = "]"
                 will produce a value label like "[X=-1.23, Y=4.56]"
                 Note the need to use a Unicode space <ampsand>#x00A0; for get space for all browsers.
                 For as long a string as this you may need to make the total image size bigger,
                 and to orient the value labels with care.
                 draw_plot_point_values is only when both x and Y pairs are wanted.
               \endverbatim
           */
            // verbatim needed to avoid a warning about using \&#x00A0; within Doxygen comment.
            using std::string;
            using std::stringstream;
            double vx = uncx.value();
            double vy = uncy.value();
            double ux = uncx.uncertainty();
            double uy = uncy.uncertainty();
            double dfx = uncx.deg_free();
            double dfy = uncy.deg_free();

            std::string label_xv = sv(vx, x_sty); //! Also strip unnecessary e, + and leading exponent zeros, if required.
            std::string label_yv = sv(vy, y_sty);
            if (x_sty.prefix_ != "")
            { // Want a prefix like "["
              label_xv = x_sty.prefix_ + label_xv;
            }
            std::string label_xu; // X uncertainty as string.
            std::string label_yu;
            std::string label_xdf; // X degrees of freedom as string.
            std::string label_ydf;

            int marker_size = derived().serieses_[0].point_style_.size_;
            int label_size = x_sty.values_text_style_.font_size();
            // Offset of value labels from point must be related mainly to
            // size of the data marker, less the value label font size.

            rotate_style rot = x_sty.value_label_rotation_;
            align_style al; // = center_align;
            switch (rot)
            {
            case horizontal: // OK
              al = center_align;
              y -= marker_size * 2;  // Up marker font size;
              // center_align means no x correction.
              break;
            case leftward: // horizontal but to left of marker. OK
              al = right_align;
              x -= marker_size * 1.3;  // left
              y += label_size * 0.3;  // down label font size;
              rot = horizontal;
              break;
            case rightward: // horizontal but to right of marker.OK
              al = left_align;
              x += marker_size * 1.1;  // right
              y += label_size * 0.3;  // down label font size;
              rot = horizontal;
              break;
            case upsidedown: // OK but upsidedown so not very useful!
              al = center_align;
              y += marker_size;  // Down marker font size;
             break;
            case slopeup: // -30 - OK
            case steepup: // -45 - OK
            case uphill: // -60 - OK
              al = left_align;
              x += label_size /3;  // Right third label font size - centers on marker.
              y -= marker_size * 0.6;  // UP marker font size;
              break;
            case upward: // -90 vertical writing up - OK.
              al = left_align;
              x += label_size /3;  // Right third label font size - centers on marker.
              y -= marker_size * 0.9;  // Up marker font size;
              break;
            case backup: // OK
              al = right_align;
              x -= marker_size * 1.5;  // Left
              y -= marker_size * 0.8;  // Up
              rot = downhill;
              break;
            case  slopedownhill: // 30 gentle slope down.
            case downhill: // 45 down.
            case steepdown: //  60 steeply down.
              al = left_align;
              x += marker_size * 0.4;  // Right;
              y += marker_size * 0.9;  // Down
              break;
            case downward: // OK
              al = left_align;
              x -= marker_size;  // Left
              y += marker_size;  // Up
             break;
            case backdown: // OK
              al = right_align;
              x -= marker_size * 0.5;  // Left
              y += marker_size * 1.5;  // down
              rot = uphill;
             break;
            } // switch

            // If would be simpler to prepare a single string like "1.23 +- -0.3, 3.45 +- -0.1(10)"
            // but this would not allow change of font size, type and color
            // something that proves to be very effective at visually separating
            // value and uncertainty, and degrees of freedom estimate.
            // So the coding complexity is judged with it (even if it may not always work right yet ;-)
            // prefix, separator and suffix are  all black at present.
            // Tasteless colors and font changes are purely proof of concept.

            int fx = static_cast<int>(x_sty.values_text_style_.font_size() * reducer);
            // Make uncertainty and df a bit smaller to distinguish from value by default (but make configurable).
            // Write X value (and optional uncertainty and df).
            text_element& t = x_g_ptr.text(x, y, label_xv, x_sty.values_text_style_, al, rot);
            // Optionally, show uncertainty as 95% confidence plus minus:  2.1 +-0.012
            // and also optionally show degrees of freedom (23).

            string pm = "&#x00A0;&#x00B1;"; //! Unicode space plusminus glyph.
            // Spaces seem to get lost, so use \&x00A0 as an explicit space glyph.
            // Layout seems to vary with font - Times New Roman leaves no space after.
            if ((x_sty.plusminus_on_ == true)
                  && (ux > 0.)
                )
            {  // Uncertainty estimate usually 95% confidence interval + or - 2 standard deviation.
              label_xu = sv(ux, x_sty, true);
              //t.tspan(pm).fill_color(darkcyan);
              // Should this be stroke_color?
              t.tspan(pm).fill_color(x_sty.plusminus_color_);
              t.tspan(label_xu).fill_color(x_sty.plusminus_color_).font_size(fx); // .font_weight("bold")
            }
            if (
                 (x_sty.df_on_ == true)  // Is wanted.
                 &&
                 (dfx != (std::numeric_limits<unsigned short int>::max)()) // and deg_free is defined OK.
               )
            { // Degrees of freedom (usually number of observations-1) used for this estimate of value.
              stringstream label;
              label.precision(4); // Might need 5 to show 65535?
              //label.flags(sty.value_ioflags_); // Leave at default.
              label << "&#x00A0;(" << dfx << ")"; // "123.5"
              // Explicit space "\&#x00A0;" seems necessary.
              label_xdf = label.str();
              t.tspan(label_xdf).fill_color(x_sty.df_color_).font_size(fx);
            }
            int fy = static_cast<int>(y_sty.values_text_style_.font_size() * reducer);
           // If there a comma is 1st char in separator, put values on the same line, else as below put below the marker.
            bool sameline = (x_sty.separator_[0] == ',');
            if (sameline)
            { // On same line so use X style for separator. But Y style for any text.
              t.tspan(x_sty.separator_).fill_color(x_sty.fill_color_).font_size(x_sty.values_text_style_.font_size());
              t.tspan(y_sty.separator_).fill_color(y_sty.fill_color_).font_size(y_sty.values_text_style_.font_size());
              if (y_sty.prefix_ != "")
              { // Want a prefix, for example: "length ="
                t.tspan(y_sty.prefix_).fill_color(y_sty.fill_color_).font_size(y_sty.values_text_style_.font_size());
              }

              t.tspan(label_yv, y_sty.values_text_style_); // Color
              if (
                   (y_sty.plusminus_on_) // Is wanted.
                   && (uy > 0.) // Is valid uncertainty estimate.
                 )
              {  // Uncertainty estimate usually 95% confidence interval + or - 2 standard deviation.
                 // Precision of uncertainty is usually less than value,
                label_yu = "&#x00A0;" + sv(uy, y_sty, true);
                t.tspan(pm).font_family("arial").font_size(fy).fill_color(green);
                t.tspan(label_yu).fill_color(y_sty.plusminus_color_).font_size(fy);
              }
              if ((y_sty.df_on_ == true)
                && (dfy != (std::numeric_limits<unsigned short int>::max)())
                )
              { // degrees of freedom or number of values -1 used for this estimate.
                std::stringstream label;
                label.precision(4);
                //label.flags(sty.value_ioflags_); // Leave at default.
                label <<"&#x00A0;(" << dfy << ")"; // "123.5"
                label_ydf = label.str();
                t.tspan(label_ydf).fill_color(y_sty.df_color_).font_size(fy);
              }
              if (y_sty.suffix_ != "")
              { // Want a suffix like "]" - with the Y values font size, (not reduced for uncertainty info), and same color as prefix.
                t.tspan(y_sty.suffix_).fill_color(y_sty.fill_color_).font_size(y_sty.values_text_style_.font_size());
              }
            }
            else
            { // Move ready to put Y value on 'newline' below point marker.
              // Problem here if orientation is changed? - Yes - doesn't line up :-(
              if (y_sty.prefix_ != "")
              { // 
                label_yv = y_sty.prefix_ + label_yv;
              }
              double dy = y_sty.values_text_style_.font_size() * 2.2; // "newline"
              // Need to start a new text_element here because tspan rotation doesn't apply to whole string?
              text_element& ty = y_g_ptr.text(x, y + dy, label_yv, y_sty.values_text_style_, al, rot);

              if ((y_sty.plusminus_on_ == true) // Is wanted.
                  && (uy > 0.) // Is valid uncertainty estimate.
                  )
              {  // Uncertainty estimate usually 95% confidence interval + or - 2 standard deviation.
                 // Precision of uncertainty is usually less than value,
                label_yu = "&#x00A0;" + sv(uy, y_sty, true);
                ty.tspan(pm).font_family("arial").font_size(fy).fill_color(y_sty.plusminus_color_);
                ty.tspan(label_yu).fill_color(y_sty.plusminus_color_).font_size(fy);
              }
              if ((y_sty.df_on_ == true)  // Is wanted.
                    && (dfy != (std::numeric_limits<unsigned short int>::max)()) // and deg_free is defined OK.
                    )
              { // degrees of freedom or number of values -1 used for this estimate.
                std::stringstream label;
                label.precision(4);
                //label.flags(sty.value_ioflags_); // Leave at default.
                label <<"&#x00A0;(" << dfy << ")"; // "123.5"
                label_ydf = label.str();
                ty.tspan(label_ydf).fill_color(y_sty.df_color_).font_size(fy);
              }
              if (y_sty.suffix_ != "")
              { // Want a suffix like "]" or "sec]" or "&#x00A0;sec]"
                ty.tspan(y_sty.suffix_).fill_color(y_sty.fill_color_).font_size(y_sty.values_text_style_.font_size());
              }
            }
          } // void draw_plot_point_values(double x, double y, g_element& g_ptr, double value)

          void clear_all()
          { //!  Clear all layers of the plot.
            /*! \details
              When writing to multiple documents, the contents of the plot
              may change significantly between. Rather than figuring out what
              has and has not changed, just erase the contents of the
              legend, title... in the document and start over.
            */
            clear_legend();
            clear_background();
            clear_x_axis();
            clear_y_axis();
            clear_title();
            clear_points();
            clear_plot_background();
            clear_grids();
          }

          void clear_background()
          { //! Clear the whole image background layer of the SVG plot.
            derived().image.g(PLOT_BACKGROUND).clear();
          }

          void clear_title()
          { //! Clear the plot title layer of the SVG plot.
            derived().image.g(PLOT_TITLE).clear();
          }

          void clear_points()
          {  //! Clear the data points layer of the SVG plot.
            derived().image.g(PLOT_DATA_POINTS).clear();
          }

          void clear_plot_background()
          { //! Clear the plot area background layer of the SVG plot.
            derived().image.g(PLOT_WINDOW_BACKGROUND).clear();
          }

          void clear_legend()
          { //! Clear the legend layer of the SVG plot.
            derived().image.g(PLOT_LEGEND_BACKGROUND).clear();
            derived().image.g(PLOT_LEGEND_POINTS).clear();
            derived().image.g(PLOT_LEGEND_TEXT).clear();
          }

          void clear_x_axis()
          { //! Clear the X axis layer of the SVG plot.
            derived().image.g(PLOT_X_AXIS).clear();
            derived().image.g(PLOT_X_MINOR_TICKS).clear();
            derived().image.g(PLOT_X_MAJOR_TICKS).clear();
            derived().image.g(PLOT_X_LABEL).clear();
            derived().image.g(PLOT_VALUE_LABELS).clear();
          }

          void clear_y_axis()
          { //! Clear the Y axis layer of the SVG plot.
            derived().image.g(PLOT_Y_AXIS).clear();
            derived().image.g(PLOT_Y_MINOR_TICKS).clear();
            derived().image.g(PLOT_Y_MAJOR_TICKS).clear();
            derived().image.g(PLOT_Y_LABEL).clear();
          }

          void clear_grids()
          { //! Clear the  grids layer of the SVG plot.
            derived().image.g(PLOT_X_MAJOR_GRID).clear();
            derived().image.g(PLOT_X_MINOR_GRID).clear();
            derived().image.g(PLOT_Y_MAJOR_GRID).clear();
            derived().image.g(PLOT_Y_MINOR_GRID).clear();
          }

        private:
          Derived& derived()
          { //! Uses Curiously Recurring Template Pattern to allow 1D and 2D to reuse common code.
            //! See http://en.wikipedia.org/wiki/Curiously_Recurring_Template_Pattern .
            return static_cast<Derived&>(*this);
          }
          const Derived& derived()const
          {  //! const version of derived()
            return static_cast<const Derived&>(*this);
          }
        public:
          // Set & get member function Declarations:
          // All set functions return derived() == *this to permit chaining,
          // for example: my_plot.background_color(red).background_border_color(blue)...

          // Shapes and glyphs can have (or may need) BOTH fill and stroke to be set.
          // Both are usually the same in this application.
          // If both are set, stroke is considered 'more important',
          // and so is returned by get functions.

          Derived& size(unsigned int x, unsigned int y);
          std::pair<double, double> image_size();
          unsigned int image_x_size();
          Derived& image_x_size(unsigned int i);
          unsigned int image_y_size();
          Derived& image_y_size(unsigned int i);
          svg_color background_color();
          Derived& background_color(const svg_color& col);
          Derived& background_border_color(const svg_color& col);
          svg_color background_border_color();
          Derived& background_border_width(double w);
          double background_border_width();
          Derived& description(const std::string d);
          const std::string& description();
          Derived& document_title(const std::string d);
          std::string document_title();
          Derived& copyright_holder(const std::string d);
          const std::string copyright_holder();
          Derived& copyright_date(const std::string d);
          const std::string copyright_date();
          Derived& license(std::string repro = "permits",
            std::string distrib = "permits",
            std::string attrib = "requires",
            std::string commercial = "permits",
            std::string derivative = "permits");
          bool license_on();
          Derived&  license_on(bool l);
          bool boost_license_on();
          Derived& boost_license_on(bool l);
          const std::string license_reproduction();
          const std::string license_distribution();
          const std::string license_attribution();
          const std::string license_commercialuse();
          Derived& coord_precision(int digits);
          int coord_precision();
          Derived& x_value_precision(int digits);
          int x_value_precision();
          Derived& x_value_ioflags(int flags);
          int x_value_ioflags();
          Derived& x_labels_strip_e0s(bool cmd);
          bool y_labels_strip_e0s();
          Derived& title(const std::string title);
          const std::string title();
          Derived& title_font_size(unsigned int i);
          unsigned int title_font_size();
          Derived& title_font_family(const std::string& family);
          const std::string& title_font_family();
          Derived& title_font_style(const std::string& style);
          const std::string& title_font_style();
          Derived& title_font_weight(const std::string& weight);
          const std::string& title_font_weight();
          Derived& title_font_stretch(const std::string& stretch);
          const std::string& title_font_stretch();
          Derived& title_font_decoration(const std::string& decoration);
          const std::string& title_font_decoration();
          Derived& title_font_rotation(rotate_style rotate);
          int title_font_rotation();
          Derived& title_font_alignment(align_style alignment);
          align_style title_font_alignment();
          // Legend.
          Derived& legend_width(double width);
          double legend_width();
          Derived& legend_title(const std::string title);
          const std::string legend_title();
          Derived& legend_font_weight(const std::string& weight);
          const std::string& legend_font_weight();
          Derived& legend_font_family(const std::string& family);
          const std::string& legend_font_family();
          Derived& legend_title_font_size(unsigned int size);
          unsigned int legend_title_font_size();
          Derived& legend_top_left(double x, double y);
          const std::pair<double, double> legend_top_left();
          const std::pair<double, double> legend_bottom_right();
          Derived& legend_lines(bool is);
          bool legend_lines();
          Derived& legend_on(bool cmd);
          bool legend_on();
          Derived& legend_place(legend_places l);
          legend_places legend_place();
          bool legend_outside();
          Derived& legend_header_font_size(int size);
          int legend_header_font_size();
          Derived& plot_window_on(bool cmd);
          bool plot_window_on();
          Derived& plot_border_color(const svg_color& col);
          svg_color plot_border_color();
          double plot_border_width();
          Derived& plot_border_width(double w);
          Derived& image_border_margin(double w);
          double image_border_margin();
          Derived& image_border_width(double w);
          double image_border_width();
          Derived& plot_window_x(double min_x, double max_x);
          Derived& plot_window_y(double min_y, double max_y);
          std::pair<double, double> plot_window_x();
          double plot_window_x_left();
          double plot_window_x_right();
          double plot_window_y_top();
          double plot_window_y_bottom();
          std::pair<double, double> plot_window_y();
          double x_minor_interval();
          double y_minor_interval();
          Derived& x_ticks_up_on(bool cmd);
          bool x_ticks_up_on();
          Derived& x_ticks_down_on(bool cmd);
          bool x_ticks_down_on();
          Derived& x_label_on(bool cmd);
          bool x_label_on();
          Derived& x_label_font_size(unsigned int i);
          unsigned int x_label_font_size();
          Derived& x_value_font_size(unsigned int i);
          unsigned int x_value_font_size();
          Derived& x_label_font_family(const std::string& family);
          const std::string& x_label_font_family();
          Derived& x_axis_label_color(const svg_color& col);
          svg_color x_axis_label_color();
          Derived& x_axis_value_color(const svg_color& col);
          svg_color x_axis_value_color();
          Derived& x_ticks_on_window_or_axis(int cmd);
          int x_ticks_on_window_or_axis();
          Derived& x_label_units_on(bool cmd);
          bool x_label_units_on();
          Derived& x_major_labels_side(int cmd);
          int x_major_labels_side();
          Derived& x_major_label_rotation(rotate_style rot);
          rotate_style x_major_label_rotation();
          Derived& title_on(bool cmd);
          bool title_on();
          Derived& x_major_grid_on(bool is);
          bool x_major_grid_on();
          Derived& x_minor_grid_on(bool is);
          bool x_minor_grid_on();
          Derived& axes_on(bool is);
          bool axes_on();
          Derived& x_axis_on(bool is);
          bool x_axis_on();
          Derived& y_axis_on(bool is);
          bool y_axis_on();
          Derived& title_color(const svg_color& col);
          svg_color title_color();
          Derived& legend_color(const svg_color& col);
          svg_color legend_color();
          Derived& legend_background_color(const svg_color& col);
          svg_color legend_background_color();
          bool legend_box_fill_on();
          Derived& legend_border_color(const svg_color& col);
          svg_color legend_border_color();
          Derived& plot_background_color(const svg_color& col);
          svg_color plot_background_color();
          const std::string x_axis_position();
          Derived& x_axis_color(const svg_color& col);
          svg_color x_axis_color();
          Derived& y_axis_color(const svg_color& col);
          svg_color y_axis_color();
          Derived& x_label_color(const svg_color& col);
          Derived& x_label_width(double width);
          double x_label_width();
          svg_color x_label_color();
          Derived& y_label_color(const svg_color& col);
          svg_color y_label_color();
          Derived& x_major_tick_color(const svg_color& col);
          svg_color x_major_tick_color();
          Derived& x_minor_tick_color(const svg_color& col);
          svg_color x_minor_tick_color();
          Derived& x_major_grid_color(const svg_color& col);
          svg_color x_major_grid_color();
          Derived& x_major_grid_width(double w);
          double x_major_grid_width();
          Derived& x_minor_grid_color(const svg_color& col);
          svg_color x_minor_grid_color();
          Derived& x_minor_grid_width(double w);
          double x_minor_grid_width();
          Derived& x_axis_width(double width);
          double x_axis_width();
          Derived& data_lines_width(double width);
          double data_lines_width();
          Derived& x_label(const std::string& str);
          std::string x_label();
          Derived& x_label_units(const std::string& str);
          std::string x_label_units();
          Derived& y_label(const std::string& str);
          std::string y_label();
          Derived& y_label_units(const std::string& str);
          std::string y_label_units();
          Derived& x_values_on(bool b);
          bool x_values_on();
          Derived& x_values_font_size(unsigned int i);
          unsigned int x_values_font_size();
          Derived& x_values_font_family(const std::string& family);
          const std::string& x_values_font_family();
          Derived& x_major_interval(double inter);

          Derived& x_values_color(const svg_color& col);
          svg_color x_values_color();
          Derived& x_values_rotation(rotate_style rotate);
          int x_values_rotation();
          Derived& x_values_precision(int p);
          int x_values_precision();
          Derived& x_values_ioflags(std::ios_base::fmtflags f);
          std::ios_base::fmtflags x_values_ioflags();
          Derived& x_plusminus_on(bool b);
          bool x_plusminus_on();
          Derived& x_plusminus_color(const svg_color& col);
          svg_color x_plusminus_color();
          Derived& x_df_on(bool b);
          bool x_df_on();
          Derived& x_df_color(const svg_color& col);
          svg_color x_df_color();
          Derived& x_decor(const std::string& pre, const std::string& sep = "", const std::string& suf = "");
          const std::string x_suffix();
          const std::string x_separator();
          const std::string x_prefix();

          double x_major_interval();
          Derived& x_major_tick_length(double length);
          double x_major_tick_length();
          Derived& x_major_tick_width(double width);
          double x_major_tick_width();
          Derived& x_minor_tick_length(double length);
          double x_minor_tick_length();
          Derived& x_minor_tick_width(double width);
          double x_minor_tick_width();
          Derived& x_major_tick(double d);
          double x_major_tick();
          Derived& x_minor_interval(double interval);
          Derived& x_num_minor_ticks(unsigned int num);
          unsigned int x_num_minor_ticks();
          Derived& x_range(double min_x, double max_x);
          std::pair<double, double> x_range();
          Derived& x_min(double min_x);
          double x_min();
          Derived& x_max(double x);
          double x_max();
          // Set & get autoscale parameters,
          // Note: all these *MUST* preceed x_autoscale(data) call.
          bool autoscale_check_limits();
          Derived& autoscale_check_limits(bool b);
          bool x_autoscale();
          Derived& x_autoscale(bool b);
          bool autoscale();
          Derived& autoscale(bool b);
          Derived& x_autoscale(std::pair<double, double> p);
          template <class T> // T an STL container: array, vector ...
          Derived& x_autoscale(const T& container); // Whole data series.
          template <class T> // T an STL container: array, vector ...
          Derived& x_autoscale(const T& begin, const T& end); // Data series using iterators.
          Derived& x_with_zero(bool b);
          bool x_with_zero();
          Derived& x_min_ticks(int min_ticks);
          int x_min_ticks();
          Derived& x_steps(int steps);
          int x_steps();
          Derived& x_tight(double tight);
          double x_tight();
          // Get results of autoscaling.
          double x_auto_min_value();
          double x_auto_max_value();
          double x_auto_tick_interval();
          int x_auto_ticks();
          Derived& limit_color(const svg_color&);
          svg_color limit_color();
          Derived& limit_fill_color(const svg_color&);
          svg_color limit_fill_color();
          Derived& draw_note(double x, double y, std::string note, text_style& tsty = no_style, align_style al = center_align, rotate_style rot = horizontal, const svg_color& = black);

          //// Stylesheet.
          // Removed for now to avoid compile warning in spirit.

          //Derived& load_stylesheet(const std::string& file)
          //{
          //  derived().image.load_stylesheet(file);
          //  return derived();
          //}

          //// Image info (& identical const version).

          //svg& get_svg()
          //{
          //  derived()._update_image();
          //  return derived().image;
          //}

          //const svg& get_svg() const
          //{
          //  derived()._update_image();
          //  return derived().image;
          //}
        }; // template <class Derived> class axis_plot_frame

        // class axis_plot_frame Member function Definitions (for .ipp file):

          template <class Derived>
          /*! \tparam Derived svg_1d_plot or svg_2d_plot
          */
          template <class T>
          /*! \tparam T an STL container: array, vector, list, map ...
          */

          Derived& axis_plot_frame<Derived>::x_autoscale(const T& begin, const T& end)
          { //! Data series (range accessed using iterators) to use to calculate autoscaled X-axis values.
              scale_axis(begin, end,
              &derived().x_auto_min_value_, &derived().x_auto_max_value_, &derived().x_auto_tick_interval_, &derived().x_auto_ticks_,
              derived().autoscale_check_limits_,
              derived().x_include_zero_, derived().x_tight_, derived().x_min_ticks_, derived().x_steps_);

            derived().x_autoscale_ = true; //! Default to use calculated values.
            return derived();
          } // x_autoscale(const T& begin, const T& end)

          template <class Derived>
          template <class T> // T an STL container: array, vector ...
          Derived& axis_plot_frame<Derived>::x_autoscale(const T& container) // Whole data series.
          { //! Data series (all values) to use to calculate autoscaled X-axis values.
              //scale_axis(container.begin(), container.end(), // All the container.
              scale_axis(container, // All the container.
              &derived().x_auto_min_value_, &derived().x_auto_max_value_, &derived().x_auto_tick_interval_, &derived().x_auto_ticks_,
              derived().autoscale_check_limits_,
              derived().x_include_zero_, derived().x_tight_, derived().x_min_ticks_, derived().x_steps_);

            derived().x_autoscale_ = true; // Default to use calculated values.
            return derived();
          } // x_autoscale(const T& container)

          template <class Derived>
          Derived& axis_plot_frame<Derived>::size(unsigned int x, unsigned int y)
          { //! Set SVG image size (SVG units, default pixels).
            // Might put default sizes here?
            // Check on sanity of these values?
            derived().image.size(x, y);
            return derived();
          }

          template <class Derived>
          std::pair<double, double> axis_plot_frame<Derived>::image_size()
          { //! \return SVG image size horizontal width and vertical height (SVG units, default pixels).
            return derived().image.size();
          }

          template <class Derived>
          unsigned int axis_plot_frame<Derived>::image_x_size()
          { //! \return SVG image X-axis size as horizontal width (SVG units, default pixels).
            return derived().image.x_size();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::image_x_size(unsigned int i)
          { //! Set SVG image X-axis size (SVG units, default pixels).
            derived().image.x_size(i);
            return derived();
          }

          template <class Derived>
          unsigned int axis_plot_frame<Derived>::image_y_size()
          { //! \return  SVG image Y-axis size as vertical height (SVG units, default pixels).
            return derived().image.y_size();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::image_y_size(unsigned int i)
          {//! Set SVG image Y-axis size (SVG units, default pixels).
            derived().image.y_size(i);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::background_color()
          { //! \return  plot background color.
            return derived().image.g(PLOT_BACKGROUND).style().fill_color();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::background_color(const svg_color& col)
          { //! Set plot background color.
            derived().image.g(PLOT_BACKGROUND).style().fill_color(col);
            return derived();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::background_border_color(const svg_color& col)
          { //! Set plot background border color.
            derived().image.g(PLOT_BACKGROUND).style().stroke_color(col);
            /*!
              background_border_color example:
              \code
svg_2d_plot my_plot(my_data, "My Data").background_border_color(red).background_color(azure);
              \endcode
            */
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::background_border_color()
          { //! \return  plot background border color.
            return derived().image.g(PLOT_BACKGROUND).style().stroke_color();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::background_border_width(double w)
          { //! Set plot background border width.
            derived().image.g(PLOT_BACKGROUND).style().stroke_width(w);
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::background_border_width()
          { //! \return  plot background border width.
            return derived().image.g(PLOT_BACKGROUND).style().stroke_width();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::description(const std::string d)
          { //! Writes description to the document for header as \verbatim <desc> My Data <\desc> \endverbatim
            derived().image.description(d);
            return derived();
          }

          template <class Derived>
          const std::string& axis_plot_frame<Derived>::description()
          { //! \return  description of the document for header as \verbatim <desc> ... <\desc> \endverbatim
            return derived().image.description();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::document_title(const std::string d)
          { //! Write document title to the SVG document for header as \verbatim <title> My Title <\title> \endverbatim
            derived().image.document_title(d);
            return derived();
          }
          template <class Derived>
          std::string axis_plot_frame<Derived>::document_title()
          { //! \return  document title to the document for header as \verbatim <title> My Title <\title> \endverbatim
            return derived().image.document_title();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::copyright_holder(const std::string d)
          { //! Writes copyright_holder to the document
            //! (for header as <!-- SVG Plot Copyright Paul A. Bristow 2007 --> )
            //! and as \verbatim metadata: meta name="copyright" content="Paul A. Bristow" /> \endverbatim
            derived().image.copyright_holder(d);
            return derived();
          }

          template <class Derived>
          const std::string axis_plot_frame<Derived>::copyright_holder()
          { //! \return copyright holder.
            return derived().image.copyright_holder();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::copyright_date(const std::string d)
          { //! Writes copyright date to the document.
            //! and as \verbatim metadata <meta name="date" content="2007" /> \endverbatim
            derived().image.copyright_date(d);
            return derived();
          }

          template <class Derived>
          const std::string axis_plot_frame<Derived>::copyright_date()
          { //! \return  copyright_date.
            return derived().image.copyright_date();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::license(std::string repro,
            std::string distrib,
            std::string attrib,
            std::string commercial,
            std::string derivative)
          { //! Set license conditions for reproduction, atribution, commercial use, and derivative works,
            //! usually "permits", "requires", or "prohibits",
            //! and set license_on == true.
            // Might check these are "permits", "requires", or "prohibits"?
            derived().image.license(repro, distrib, attrib, commercial, derivative);
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::license_on()
          { //! \return true if license conditions should be included in the SVG document.
            //! \see axis_plot_frame::license
            return derived().image.license_on();
          }

          template <class Derived>
          Derived&  axis_plot_frame<Derived>::license_on(bool l)
          { //! Set if license conditions should be included in the SVG document.
            //! \see axis_plot_frame::license
            derived().image.license_on(l);
            return derived();
          }
          template <class Derived>
          bool axis_plot_frame<Derived>::boost_license_on()
          { //! \return  if the Boost license conditions should be included in the SVG document.
            //! To set see axis_plot_frame::boost_license_on(bool).
            return derived().image.boost_license_one();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::boost_license_on(bool l)
          { //! Set if the Boost license conditions should be included in the SVG document.
            derived().image.boost_license_on(l);
            return derived();
          }

          template <class Derived>
          const std::string axis_plot_frame<Derived>::license_reproduction()
          { //! \return  reproduction license conditions, usually "permits", "requires", or "prohibits".
            return derived().image.reproduction();
          }

          template <class Derived>
          const std::string axis_plot_frame<Derived>::license_distribution()
          { //! \return  distribution license conditions, usually "permits", "requires", or "prohibits".
            return derived().image.distribution();
          }

          template <class Derived>
          const std::string axis_plot_frame<Derived>::license_attribution()
          { //! \return  attribution license conditions, usually "permits", "requires", or "prohibits".
            return derived().image.attribution();
          }

          template <class Derived>
          const std::string axis_plot_frame<Derived>::license_commercialuse()
          { //! \return  commercial use license conditions, usually "permits", "requires", or "prohibits".
            return derived().image.commercialuse();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::coord_precision(int digits)
          { /*! Precision of SVG coordinates in decimal digits (default 3).
              3 decimal digits precision is sufficient for small images.
              4 or 5 decimal digits precision will give higher quality plots,
              especially for larger images, at the expense of larger .svg files,
              particularly if there are very many data points.
           */
            derived().image.coord_precision(digits);
            return derived();
          }

          template <class Derived>
          int axis_plot_frame<Derived>::coord_precision()
          { //! \return  precision of SVG coordinates in decimal digits.
            return derived().image.coord_precision();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_value_precision(int digits)
          { /*! Precision of X tick label values in decimal digits (default 3).
              3 decimal digits precision is sufficient for small images.
              4 or 5 decimal digits precision will give more cluttered plots.
              If the range of labels is very small, then more digits will be essential.
            */

            derived().x_ticks_.value_precision_ = digits;
            return derived();
          }

          template <class Derived>
          int axis_plot_frame<Derived>::x_value_precision()
          { //! \return  precision of X tick label values in decimal digits
            return derived().x_ticks_.value_precision_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_value_ioflags(int flags)
          { /*! Set iostream std::ios::fmtflags for X value label (default decimal == 0X201).
              Mainly useful for changing to scientific, fixed or hexadecimal format.
              For example: .x_value_ioflags(std::ios::dec | std::ios::scientific)
            */
            derived().x_ticks_.value_ioflags_ = flags;
            return derived();
          }

          template <class Derived>
          int axis_plot_frame<Derived>::x_value_ioflags()
          { //! \return  stream ioflags for control of format of X value labels.
            return derived().x_ticks_.value_ioflags_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_labels_strip_e0s(bool cmd)
          { //! Set if to strip redundant zeros, signs and exponents, for example, reducing "1.2e+000" to "1.2"
            //! This markedly reduces visual clutter, and is the default.
            derived().x_ticks_.strip_e0s_ = cmd;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::y_labels_strip_e0s()
          { //! \return  if to strip redundant zeros, signs and exponents, for example, reducing "1.2e+000" to "1.2"
            return derived().x_ticks_.strip_e0s_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::title(const std::string title)
          { /*!
              Set a title for plot.
              The string may include Unicode for greek letter and symbols.
              For example a title that includes a greek omega:
              \verbatim my_plot.title("Plot of &#x3A9; function"); \endverbatim
            */
            // Plot title.  TODO
            // new text parent code pushback
            // effectively concatenates with any existing title.
            // So clear the existing string first but doesn't work,
            // so need to clear the whole g_element.
            //derived().image.g(PLOT_TITLE).clear();
            derived().title_info_.text(title);
            derived().title_on_ = true; // Assume display wanted, if bother to set title.
            return derived();
          }

          template <class Derived>
          const std::string axis_plot_frame<Derived>::title()
          { //! \return  a title for plot, whose string may include Unicode for greek letter and symbols.
            return derived().title_info_.text();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::title_font_size(unsigned int i)
          { //! Sets the font size for the title (svg units, default pixels).
            derived().title_info_.textstyle().font_size(i);
            return derived();
          }

          template <class Derived>
          unsigned int axis_plot_frame<Derived>::title_font_size()
          { //! \return  the font size for the title (svg units, default pixels).
            return derived().title_info_.textstyle().font_size();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::title_font_family(const std::string& family)
          { //! Set the font family for the title (for example: .title_font_family("Lucida Sans Unicode");
            derived().title_info_.textstyle().font_family(family);
            return derived();
          }

          template <class Derived>
          const std::string& axis_plot_frame<Derived>::title_font_family()
          { //! \return  the font family for the title
            return derived().title_info_.textstyle().font_family();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::title_font_style(const std::string& style)
          { //! Set the font style for the title (default normal).
            derived().title_info_.textstyle().font_style(style);
            return derived();
          }

          template <class Derived>
          const std::string& axis_plot_frame<Derived>::title_font_style()
          { //! \return  the font style for the title (default normal).
            return derived().title_info_.textstyle().font_style();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::title_font_weight(const std::string& weight)
          { //! Set the font weight for the title (default normal).
            derived().title_info_.textstyle().font_weight(weight);
            return derived();
          }

          template <class Derived>
          const std::string& axis_plot_frame<Derived>::title_font_weight()
          {//! \return  the font weight for the title.
            return derived().title_info_.textstyle().font_weight();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::title_font_stretch(const std::string& stretch)
          { //! Set the font stretch for the title (default normal), wider or narrow.
            derived().title_info_.textstyle().font_stretch(stretch);
            return derived();
          }

          template <class Derived>
          const std::string& axis_plot_frame<Derived>::title_font_stretch()
          { //! \return  the font stretch for the title.
            return derived().title_info_.textstyle().font_stretch();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::title_font_decoration(const std::string& decoration)
          { //! Set the font decoration for the title (default normal, or underline, overline or strike-thru).
            derived().title_info_.textstyle().font_decoration(decoration);
            return derived();
          }

           template <class Derived>
         const std::string& axis_plot_frame<Derived>::title_font_decoration()
          { //! \return  the font decoration for the title (default normal, or underline, overline or strike-thru).
            return derived().title_info_.textstyle().font_decoration();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::title_font_rotation(rotate_style rotate)
          { //! Set the rotation for the title font (degrees, 0 to 360).
            derived().title_info_.rotation(rotate);
            return derived();
          }

          template <class Derived>
          int axis_plot_frame<Derived>::title_font_rotation()
          { //! \return  the rotation for the title font (degrees).
            return derived().title_info_.rotation();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::title_font_alignment(align_style alignment)
          { //! Set the alignment for the title.
            derived().title_info_.alignment(alignment);
            return derived();
          }

          template <class Derived>
          align_style axis_plot_frame<Derived>::title_font_alignment()
          { //! \return  the alignment for the title.
            return derived().title_info_.alignment();
          }

          // Legend.

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_width(double width)
          { //! Set the width for the legend.
            derived().legend_width_ = width;
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::legend_width()
          { //! \return  the width for the legend.
            return derived().legend_width_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_title(const std::string title)
          { //! Set the title for the legend.
            derived().legend_header_.text(title);
            return derived();
          }

          template <class Derived>
          const std::string axis_plot_frame<Derived>::legend_title()
          { //! \return  the title for the legend.
            return derived().legend_header_.text();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_font_weight(const std::string& weight)
          { //! Set the font weight for the legend title.
            derived().legend_header_.textstyle().font_weight(weight);
            return derived();
          }

          template <class Derived>
          const std::string& axis_plot_frame<Derived>::legend_font_weight()
          { //! \return  the font weight for the legend title.
            return derived().legend_header_.textstyle().font_weight();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_font_family(const std::string& family)
          { //! Set the font family for the legend title.
            derived().legend_header_.textstyle().font_family(family);
            return derived();
          }

          template <class Derived>
          const std::string& axis_plot_frame<Derived>::legend_font_family()
          { //! \return  the font family for the legend title.
            return derived().legend_header_.textstyle().font_family();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_title_font_size(unsigned int size)
          { //! Set the font size for the legend title (svg units, default pixels).
            derived().legend_header_.textstyle().font_size(size);
            return derived();
          }

          template <class Derived>
          unsigned int axis_plot_frame<Derived>::legend_title_font_size()
          { //! \return  the font size for the legend title (svg units, default pixels).
            return derived().legend_header_.textstyle().font_size();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_top_left(double x, double y)
          { //! Set position of top left of legend box (svg coordinates, default pixels).
            //! Bottom right is controlled by contents, so the user cannot set it.
            if((x < 0) || (x > derived().image.x_size()) || (y < 0) || (y > derived().image.y_size()))
            {
              throw std::runtime_error("Legend box position outside image!");
            }
            derived().legend_left_ = x;
            derived().legend_top_ = y;
            return derived();
          }

          template <class Derived>
          const std::pair<double, double> axis_plot_frame<Derived>::legend_top_left()
          { //! \return  svg coordinate (default pixels) of top left of legend box.
            std::pair<double, double> r;
            r.first = derived().legend_left_;
            r.second = derived().legend_top_;
            return r;
          }

          template <class Derived>
          const std::pair<double, double> axis_plot_frame<Derived>::legend_bottom_right()
          { //! \return  svg coordinate (default pixels) of Bottom right of legend box.
            std::pair<double, double> r;
            r.first = derived().legend_right_;
            r.second = derived().legend_bottom_;
            return r;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_lines(bool is)
          { //! Set true if legend should include samples of the lines joining data points.
            //! \details This allows different series of data points to be distinguished by different color and/or width.
            //! This is especially useful to show plots of different functions and/or different parameters in different colors.
            derived().legend_lines_ = is;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::legend_lines()
          {//! \return  true if legend should include samples of the lines joining data points.
            return derived().legend_lines_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_on(bool cmd)
          { //! Set true if a legend is wanted.
            derived().legend_on_ = cmd;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::legend_on()
          { //! \return  true if a legend is wanted.
            return derived().legend_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_place(legend_places l)
          { //! Set the position of the legend, \see  boost::svg::legend_places
            derived().legend_place_ = l;
            return derived();
          }

          template <class Derived>
          legend_places axis_plot_frame<Derived>::legend_place()
          { //! \return  the position of the legend, \see  boost::svg::legend_places
            return derived().legend_place_;
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::legend_outside()
          { //! \return  if the legend should be outside the plot area.
            return derived().outside_legend_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_header_font_size(int size)
          { //! Set legend header font size (svg units, default pixels).
            derived().legend_header_.textstyle().font_size(size);
            return *this;
          }

          template <class Derived>
          int axis_plot_frame<Derived>::legend_header_font_size()
          { //! \return  legend header font size (svg units, default pixels).
            return derived().legend_header_.textstyle().font_size();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::plot_window_on(bool cmd)
          { //! Set true if a plot window is wanted (or false if the whole image is to be used).
            derived().plot_window_on_ = cmd;
            if(cmd)
            { // Set plot window.
              derived().image.g(detail::PLOT_WINDOW_BACKGROUND).style()
                .fill_color(derived().plot_window_border_.fill_) // background color and
                .stroke_color(derived().plot_window_border_.stroke_); // border color.
            }
            //derived().legend_place_ = outside_right;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::plot_window_on()
          {//! \return  true if a plot window is wanted (or false if the whole image is to be used).
            return derived().plot_window_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::plot_border_color(const svg_color& col)
          { //! Set the color for the plot window background.
            derived().plot_window_border_.stroke_ = col;
            derived().image.g(detail::PLOT_WINDOW_BACKGROUND).style().stroke_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::plot_border_color()
          { //! \return  the color for the plot window background.
            return derived().image.g(detail::PLOT_WINDOW_BACKGROUND).style().stroke_color();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::plot_border_width()
          { //! \return  the width for the plot window border (svg units, default pixels).
            return derived().image.g(detail::PLOT_WINDOW_BACKGROUND).style().stroke_width();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::plot_border_width(double w)
          { //! Set the width for the plot window border (svg units, default pixels).
            derived().plot_window_border_.width_ = w;
            derived().image.g(detail::PLOT_WINDOW_BACKGROUND).style().stroke_width(w);
            return derived();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::image_border_margin(double w)
          { //! Set the margin around the plot window border (svg units, default pixels).
            //! \details This prevents the plot window getting too close to other elements of the plot.
            derived().image_border_.margin_ = w;
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::image_border_margin()
          { //! \return  the margin around the plot window border (svg units, default pixels).
            return derived().image_border_.margin_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::image_border_width(double w)
          { //! Set the svg image border width (svg units, default pixels).
            derived().image_border_.width_ = w;
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::image_border_width()
          { //! \return  the svg image border width (svg units, default pixels).
            return derived().image_border_.width_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::plot_window_x(double min_x, double max_x)
          { //! Set the minimum and maximum (cartesian data units) for the plot window X axis.
            //! This is normally calculated from other plot values.
            if(max_x <= min_x)
            {
              throw std::runtime_error("plot_window X: x_max_ <= x_min_");
            }
            if((max_x - min_x) < std::numeric_limits<double>::epsilon() * 1000)
            { // Range too small to display.
              throw std::runtime_error("plot_window X range too small!" );
            }
            derived().plot_left_ = min_x;
            derived().plot_right_ = max_x;
            return derived();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::plot_window_y(double min_y, double max_y)
          { //! Set the minimum and maximum (cartesian data units) for the plot window Y axis.
            //! This is normally calculated from other plot values.

            if(max_y <= min_y)
            {
              throw std::runtime_error("plot_window Y: y_max_ <= x_min_");
            }
            if(max_y <= min_y)
            {
              throw std::runtime_error("plot_window Y range too small!");
            }
            derived().plot_top_ = min_y;
            derived().plot_bottom_ = max_y;
            return derived();
          }

          // Get the minimum and maximum (cartesian data units) for the plot window axes.

          template <class Derived>
          double axis_plot_frame<Derived>::plot_window_x_left()
          { //! \return  left of the plot window.
            return derived().plot_left_;
          }
          template <class Derived>
          double axis_plot_frame<Derived>::plot_window_x_right()
          { //! \return  right of the plot window.
            return derived().plot_right_;
          }

          template <class Derived>
          double axis_plot_frame<Derived>::plot_window_y_top()
          { //! \return  top of the plot window.
            return derived().plot_top_;
          }

          template <class Derived>
          double axis_plot_frame<Derived>::plot_window_y_bottom()
          { //! \return  top of the plot window.
            return derived().plot_bottom_;
          }

          template <class Derived>
          std::pair<double, double> axis_plot_frame<Derived>::plot_window_x()
          { //! \return  both the left and right (X axis) of the plot window.
            std::pair<double, double> r;
            r.first = derived().plot_left_;
            r.second = derived().plot_right_;
            return r;
          }

          template <class Derived>
          std::pair<double, double> axis_plot_frame<Derived>::plot_window_y()
          { //! \return  both the top and bottom (Y axis) of the plot window.
            std::pair<double, double> r;
            r.first = derived().plot_top_;
            r.second = derived().plot_bottom_;
            return r;
          }

          template <class Derived>
          double axis_plot_frame<Derived>::x_minor_interval()
          { //! \return  interval between X minor ticks.
            return derived().x_ticks_.minor_interval_;  // interval
          }

          template <class Derived>
          double axis_plot_frame<Derived>::y_minor_interval()
          { //! \return  interval between Y minor ticks.
            return derived().y_ticks_.minor_interval_;  // interval
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_ticks_up_on(bool cmd)
          { //! Set true if X major ticks should mark upwards.
            derived().x_ticks_.up_ticks_on_ = cmd;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::x_ticks_up_on()
          { //! \return  true if X major ticks should mark upwards.
            return derived().x_ticks_.up_ticks_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_ticks_down_on(bool cmd)
          { //! Set true if X major ticks should mark downwards.
            derived().x_ticks_.down_ticks_on_ = cmd;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::x_ticks_down_on()
          { //! \return  true if X major ticks should mark downwards.
            return derived().x_ticks_.down_ticks_on_;
          }
          // Only need y_ticks_left_on & y_ticks_right_on in 2D, so not defined here.

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_label_on(bool cmd)
          { //! Set true if want to show X-axis label text.
            //! \details Also switched on by setting label text.
            //! (on the assumption that if label text is set, display is also wanted,
            //! but can be switched off if *not* required).
            derived().x_axis_.label_on_ = cmd;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::x_label_on()
          { //! \return  true if want to show X-axis label text.
            return derived().x_axis_.label_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_label_font_size(unsigned int i)
          { //! Set X axis label font size (svg units, default pixels).
            derived().x_label_info_.textstyle().font_size(i);
            return derived();
          }

          template <class Derived>
          unsigned int axis_plot_frame<Derived>::x_label_font_size()
          { //! \return  X axis label font size (svg units, default pixels).
            return derived().x_label_info_.textstyle().font_size();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_value_font_size(unsigned int i)
          { //! Set X tick value label font size (svg units, default pixels).
            derived().x_value_value.textstyle().font_size(i);
            return derived();
          }

          template <class Derived>
          unsigned int axis_plot_frame<Derived>::x_value_font_size()
          { //! \return  X tick value label font size (svg units, default pixels).
            return derived().x_value_value.textstyle().font_size();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_label_font_family(const std::string& family)
          { //! Set X tick value label font family.
            derived().x_label_info_.textstyle().font_family(family);
            return derived();
          }

          template <class Derived>
          const std::string& axis_plot_frame<Derived>::x_label_font_family()
          { //! \return  X tick value label font family.
            return derived().x_label_info_.textstyle().font_family();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_axis_label_color(const svg_color& col)
          { //! Set X axis label color.
            // Set BOTH stroke and fill to the same color.
            derived().image.g(detail::PLOT_X_LABEL).style().fill_color(col);
            derived().image.g(detail::PLOT_X_LABEL).style().stroke_color(col);
            return *this;
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::x_axis_label_color()
          { //! \return  X axis label color.
            // But only return the stroke color.
            return derived().image.g(detail::PLOT_X_LABEL).style().stroke_color();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_axis_value_color(const svg_color& col)
          { //! Set X axis tick value label color.
            // Set BOTH stroke and fill to the same color.
            derived().image.g(detail::PLOT_VALUE_LABELS).style().fill_color(col);
            derived().image.g(detail::PLOT_VALUE_LABELS).style().stroke_color(col);
            return *this;
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::x_axis_value_color()
          { //! \return  X axis tick value label color.
            // But only return the stroke color.
            return derived().image.g(detail::PLOT_VALUE_LABELS).style().stroke_color();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_ticks_on_window_or_axis(int cmd)
          { /*!  Set  X ticks on window or axis
              \arg cmd -1 bottom of plot window,
              \arg cmd 0 on X axis.
              \arg cmd +1 top of plot window.
            */
            derived().x_ticks_.ticks_on_window_or_axis_ = cmd;
            return derived();
          }

          template <class Derived>
          int axis_plot_frame<Derived>::x_ticks_on_window_or_axis()
          { //! \return true if X axis ticks wanted on the window (rather than on axis).\n
            //! -1 bottom of plot window, 0 on X axis, +1 top of plot window.
            return derived().x_ticks_.ticks_on_window_or_axis_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_label_units_on(bool cmd)
          { //! Set true if want X axis label to include units (as well as label like "length").
            //! \see x_label_units which also sets true.
            derived().x_axis_.label_units_on_ = cmd;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::x_label_units_on()
          { //! Set true if want X axis label to include units (as well as label like "length").
            return derived().x_axis_.label_units_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_major_labels_side(int place)
          { /*! Position of labels for major ticks on horizontal X axis line.
               \arg \c place > 0 labels to left of Y axis line (default),
               \arg \c place = 0 (false) no major tick labels on Y axis.
               \arg \c place > 0 labels to right of Y axis line.
            */
            derived().x_ticks_.major_value_labels_side_ = place;
            return derived(); //! \return reference to svg_2d_plot to make chainable.
          }

          template <class Derived>
          int axis_plot_frame<Derived>::x_major_labels_side()
          { //! \return the side for X ticks major value labels.
            return derived().x_ticks_.major_value_labels_side_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_major_label_rotation(rotate_style rot)
          { /*! Set rotation for X ticks major value labels. (Default horizontal).
               \see rotate_style
            */
            derived().x_ticks_.label_rotation_ = rot;
            return derived();
          }

          template <class Derived>
          rotate_style axis_plot_frame<Derived>::x_major_label_rotation()
          { /*! \return rotation for X ticks major value labels.
                \see rotate_style
             */
            return derived().x_ticks_.label_rotation_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::title_on(bool cmd)
          { //! If set true, show a title for the plot.
            derived().title_on_ = cmd;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::title_on()
          {//! If true, will show a title for the plot.
            return derived().title_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_major_grid_on(bool is)
          { //! If set true, will include a major X-axis grid.
            derived().x_ticks_.major_grid_on_ = is;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::x_major_grid_on()
          { //! If true, will include a major X-axis grid.
            return derived().x_ticks_.major_grid_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_minor_grid_on(bool is)
          { //! If set true, will include a minor X-axis grid.
            derived().x_ticks_.minor_grid_on_ = is;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::x_minor_grid_on()
          {//! If true, will include a minor X-axis grid.
            return derived().x_ticks_.minor_grid_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::axes_on(bool is)
          { //! If set true, draw \b both x and y axes (note plural axes).
            derived().x_axis_.axis_line_on_ = is;
            derived().y_axis_.axis_line_on_ = is; // No meaning for 1D, but set anyway?
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::axes_on()
          { //! \return true if \b both x and y axis on.
            return derived().x_axis_.axis_line_on_ && derived().y_axis_.axis_line_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_axis_on(bool is)
          { //! If set true, draw a horizontal X-axis line.
            derived().x_axis_.axis_line_on_ = is;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::x_axis_on()
          { //! If true, draw a horizontal X-axis line.
          // Use X in preference to Y for 1D.
            return derived().x_axis_.axis_line_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::y_axis_on(bool is)
          { //! If set true, draw a vertical Y-axis line.
            derived().y_axis_.axis_line_on_ = is;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::y_axis_on()
          { //! If true, draw a vertical Y-axis line.
            // Should be always false for 1D.
            return derived().y_axis_.axis_line_on_;
          }

          // enums like PLOT_TITLE provide a std:string like "title"
          // colors .stroke_color, .stroke_width and font are set in the appropriate g_element.

          template <class Derived>
          Derived& axis_plot_frame<Derived>::title_color(const svg_color& col)
          { //! Set the color of any title of the plot.
            // Function title_color could set both fill (middle) and stroke (outside),
            // but just setting fill if simplest,
            // but does not allow separate inside & outside colors.
            derived().image.g(PLOT_TITLE).style().fill_color(col);
            //derived().image.g(PLOT_TITLE).style().stroke_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::title_color()
          { //! \return the color of any title of the plot.
            // Function title_color could get either fill and stroke,
            // return derived().image.g(PLOT_TITLE).style().stroke_color();
            return derived().image.g(PLOT_TITLE).style().fill_color();
          }

          //Derived& title_font_width(double width)
          //{ // Set the width of the font for the title of the plot.
          // width of text is effectively the boldness.
          //  // Not useful with current browsers, setting this may cause lower quality graphic fonts
          //  // perhaps because the font is created using graphics rather than a built-in font.
          //  derived().image.g(PLOT_TITLE).style().stroke_width(width);
          //  return derived();
          //}

         //Derived& legend_font_width(double width)
          //{ //! \return  the width of the font for the title of the plot.
            // width of text is effectively the boldness.
          //  derived().image.g(PLOT_LEGEND_TEXT).style().stroke_width(width);
          //  return derived();
          //}

          //double legend_font_width()
          //{ // Set the width of the font for the title of the legend.
          // Probably not useful at present (se above).
          //  return derived().image.g(PLOT_LEGEND_TEXT).style().stroke_width();
          //}
          //double legend_font_width()
          //{ //! \return  the width of the font for the title of the legend.
          //  return derived().image.g(PLOT_TITLE).style().stroke_width();
          //}

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_color(const svg_color& col)
          { //! Set the color of the title of the legend.
            // derived().image.g(PLOT_LEGEND_TEXT).style().stroke_color(col);
            derived().image.g(PLOT_LEGEND_TEXT).style().fill_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::legend_color()
          { //! \return  the color of the title of the legend.
            // Function legend_color sets only stroke, assuming that 'filled' text is not being used.
            // (It produces much lower quality fonts on some browsers).
            return derived().image.g(PLOT_LEGEND_TEXT).style().fill_color();
            // return derived().image.g(PLOT_LEGEND_TEXT).style().stroke_color();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_background_color(const svg_color& col)
          { //! Set the background fill color of the legend box.
            derived().legend_box_.fill(col);
            derived().image.g(PLOT_LEGEND_BACKGROUND).style().fill_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::legend_background_color()
          { //! \return  the background fill color of the legend box.
            return derived().image.g(PLOT_LEGEND_BACKGROUND).style().fill_color();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::legend_box_fill_on()
          {
            return derived().legend_box_.fill_on();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::legend_border_color(const svg_color& col)
          { //! Set the border stroke color of the legend box.
            derived().legend_box_.stroke(col);
            derived().image.g(PLOT_LEGEND_BACKGROUND).style().stroke_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::legend_border_color()
          { //! \return  the border stroke color of the legend box.
            return  derived().legend_box_.stroke();
            // return derived().image.g(PLOT_LEGEND_BACKGROUND).style().stroke_color();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::plot_background_color(const svg_color& col)
          { //! Set the fill color of the plot window background.
            derived().image.g(PLOT_WINDOW_BACKGROUND).style().fill_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::plot_background_color()
          { //! \return  the fill color of the plot window background.
            return derived().image.g(PLOT_WINDOW_BACKGROUND).style().fill_color();
          }

          template <class Derived>
          const std::string axis_plot_frame<Derived>::x_axis_position()
          { //! \return  the position (or intersection with Y-axis) of the X-axis.
            switch(derived().x_axis_position_)
            {
            case top:
              return "x_axis_position top (all Y values < 0)"; break;
            case x_intersects_y:
              return "x_axis_position intersects Y axis (Y range includes zero)"; break;
            case bottom:
              return "x_axis_position bottom (all Y values > 0)"; break;
            default:
              return "?"; break;
            }
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_axis_color(const svg_color& col)
          { //! Set the color of the X-axis line.
            // Note only stroke color is set.
            derived().image.g(PLOT_X_AXIS).style().stroke_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::x_axis_color()
          { //! \return  the color of the X-axis line.
            return derived().image.g(PLOT_X_AXIS).style().stroke_color();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::y_axis_color(const svg_color& col)
          { //! Set the color of the Y-axis line.
            derived().image.g(PLOT_Y_AXIS).style().stroke_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::y_axis_color()
          { //! \return  the color of the Y-axis line.
            return derived().image.g(PLOT_Y_AXIS).style().stroke_color();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_label_color(const svg_color& col)
          { //! Set the color of X-axis label (including any units).
            // add fill as well PAB Oct 07
            derived().image.g(PLOT_X_LABEL).style().fill_color(col);
            derived().image.g(PLOT_X_LABEL).style().stroke_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::x_label_color()
          { //! \return  the color of X-axis label (including any units).
            return derived().image.g(PLOT_X_LABEL).style().fill_color();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_label_width(double width)
          { //! Set the width (boldness) of X-axis label (including any units).
            //! (not recommended until browsers implement better).
            // width of text is effectively the boldness.
            derived().image.g(PLOT_X_LABEL).style().stroke_width(width);
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::x_label_width()
          { //! \return  the width (boldness) of X-axis label (including any units).
            return derived().image.g(PLOT_X_LABEL).style().stroke_width();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::y_label_color(const svg_color& col)
          { //! Set the color of Y-axis label (including any units).
            derived().image.g(PLOT_Y_LABEL).style().fill_color(col);
            derived().image.g(PLOT_Y_LABEL).style().stroke_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::y_label_color()
          { //! \return  the color of Y-axis label (including any units).
            return derived().image.g(PLOT_Y_LABEL).style().fill_color();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_major_tick_color(const svg_color& col)
          { //! Set the color of X-axis major ticks.
            derived().image.g(PLOT_X_MAJOR_TICKS).style().stroke_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::x_major_tick_color()
          { //! \return  the color of X-axis major ticks.
            return derived().image.g(PLOT_X_MAJOR_TICKS).style().stroke_color();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_minor_tick_color(const svg_color& col)
          { //! Set the color of X-axis minor ticks.
            derived().image.g(PLOT_X_MINOR_TICKS).style().stroke_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::x_minor_tick_color()
          { //! \return  the color of X-axis minor ticks.
            return derived().image.g(PLOT_X_MINOR_TICKS).style().stroke_color();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_major_grid_color(const svg_color& col)
          { //! Set the color of X-axis major grid lines.
            derived().image.g(PLOT_X_MAJOR_GRID).style().stroke_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::x_major_grid_color()
          { //! \return  the color of X-axis major grid lines.
            return derived().image.g(PLOT_X_MAJOR_GRID).style().stroke_color();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_major_grid_width(double w)
          { //! Set the width of X-axis major grid lines.
            derived().image.g(PLOT_X_MAJOR_GRID).style().stroke_width(w);
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::x_major_grid_width()
          { //! \return  the color of X-axis major grid lines.
            return derived().image.g(PLOT_X_MAJOR_GRID).style().stroke_width();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_minor_grid_color(const svg_color& col)
          { //! Set the color of X-axis minor grid lines.
            derived().image.g(PLOT_X_MINOR_GRID).style().stroke_color(col);
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::x_minor_grid_color()
          { //! \return  the color of X-axis minor grid lines.
            return derived().image.g(PLOT_X_MINOR_GRID).style().stroke_color();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_minor_grid_width(double w)
          { //! Set the width of X-axis minor grid lines.
            derived().image.g(PLOT_X_MINOR_GRID).style().stroke_width(w);
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::x_minor_grid_width()
          { //! \return  the width of X-axis minor grid lines.
            return derived().image.g(PLOT_X_MINOR_GRID).style().stroke_width();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_axis_width(double width)
          { //! Set the width of X-axis lines.
            derived().image.g(PLOT_X_AXIS).style().stroke_width(width);
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::x_axis_width()
          { //! \return  the width of X-axis lines.
            return derived().image.g(PLOT_X_AXIS).style().stroke_width();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::data_lines_width(double width)
          { //! Set the width of lines joining data points.
            derived().image.g(PLOT_DATA_LINES).style().stroke_width(width);
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::data_lines_width()
          { //! \return  the width of lines joining data points.
            return derived().image.g(PLOT_DATA_LINES).style().stroke_width();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_label(const std::string& str)
          { //! Set the text to label the X-axis (and set x_label_on(true)).
            derived().x_label_info_.text(str);
            derived().x_axis_.label_on_ = true; // Assume want x_label string displayed.
            // Might switch label_on false if null string?
            return derived();
          }

          template <class Derived>
          std::string axis_plot_frame<Derived>::x_label()
          { //! \return  the text to label the X-axis.
            return derived().x_label_info_.text();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_label_units(const std::string& str)
          { //! Set the text to add units to the X-axis label.
            derived().x_units_info_.text(str);
            derived().x_axis_.label_on_ = true; // Assume want x_label string displayed.
            return derived();
          }

          template <class Derived>
          std::string axis_plot_frame<Derived>::x_label_units()
          { //! \return  the text to add units to the X-axis label.
            //! The label will only be shown if  x_label_on() == true.
            return derived().x_units_info_.text();
          }

          // y_label not needed in 1D.
          template <class Derived>
          Derived& axis_plot_frame<Derived>::y_label(const std::string& str)
          { //! Set the text for the Y-axis label (and set y_label_on(true)).
            derived().y_label_info_.text(str);
            derived().y_axis_.label_on_ = true; // Assume want y_label string displayed.
            return derived();
          }

          template <class Derived>
          std::string axis_plot_frame<Derived>::y_label()
          { //! \return  the text for the Y-axis label.
            //! The label will only be shown if  y_label_on() == true.
            return derived().y_label_info_.text();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::y_label_units(const std::string& str)
          { //! Set the text to add units to the Y-axis label.
            derived().y_units_info_.text(str);
            derived().y_axis_.label_on_ = true; // Assume want y_label string displayed.
            return derived();
          }

          template <class Derived>
          std::string axis_plot_frame<Derived>::y_label_units()
          { //! \return  the text to add units to the X-axis label.
            return derived().y_units_info_.text();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_values_on(bool b)
          { //! \return true if values of X data points are shown (for example: 1.23).
            // (Want override xy_values_on that would otherwise cause overwriting).
            // So the last values_on setting will prevail.
            // But this is only defined in 2D
            //if(derived().xy_values_on())
            //{ // Would be overwritten by XY pair.
            //  derived().xy_values_on(false);
            //}
            derived().x_values_on_ = b;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::x_values_on()
          { //! If true, show data point values near data points markers.
            return derived().x_values_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_values_font_size(unsigned int i)
          { //! Set font size of data point X values near data points markers.
            derived().x_values_style_.values_text_style_.font_size(i);
            return derived();
          }

          template <class Derived>
          unsigned int axis_plot_frame<Derived>::x_values_font_size()
          { //! \return  font size of data point X values near data points markers.
            return derived().x_values_style_.values_text_style_.font_size();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_values_font_family(const std::string& family)
          { //! Set font family of data point X values near data points markers.
            derived().x_values_style_.values_text_style_.font_family(family);
            return derived();
          }

          template <class Derived>
          const std::string& axis_plot_frame<Derived>::x_values_font_family()
          { //! Set font family of data point X values near data points markers.
            return derived().x_values_style_.values_text_style_.font_family();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_major_interval(double inter)
          { //! Set the interval between X-axis major ticks.
            derived().x_ticks_.major_interval_ = inter;
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::x_major_interval()
          { //! \return  the interval between X-axis major ticks.
            return derived().x_ticks_.major_interval_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_values_color(const svg_color& col)
          { //! Set the color of data point X values near data points markers.
            // Function could set both fill (middle) and stroke (outside),
            // but just setting fill is simplest,
            // but does not allow separate inside & outside colors.
            // Might be better to set in x_values_style
            derived().image.g(PLOT_X_POINT_VALUES).style().fill_color(col);
            //derived().image.g(PLOT_X_POINT_VALUES).style().stroke_color(col);

            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::x_values_color()
          { //! \return  the color of data point X values near data points markers.
            // Function could get either fill and stroke,
            // return derived().image.g(PLOT_X_POINT_VALUES).style().stroke_color();
            return derived().image.g(PLOT_X_POINT_VALUES).style().fill_color();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_values_rotation(rotate_style rotate)
          { //! \return  the rotation (rotate_style) of data point X values near data points markers.
            //! (Degrees: 0 to 360 in 45 steps).
            derived().x_values_style_.value_label_rotation_ = rotate;
            return derived();
          }

          template <class Derived>
          int axis_plot_frame<Derived>::x_values_rotation()
          { //! \return  the rotation of data point X values near data points markers.
            return derived().x_values_style_.value_label_rotation_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_values_precision(int p)
          { //! Set iostream decimal digits precision of data point X values near data points markers.
            derived().x_values_style_.value_precision_ = p;
            return derived();
          }

          template <class Derived>
          int axis_plot_frame<Derived>::x_values_precision()
          { //! \return  iostream decimal digits precision of data point X values near data points markers.
            return derived().x_values_style_.value_precision_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_values_ioflags(std::ios_base::fmtflags f)
          { //! Set iostream format flags of data point X values near data points markers.
            //! Useful to set hexadecimal, fixed and scientific, (std::ios::scientific).
            derived().x_values_style_.value_ioflags_ = f;
            return derived();
          }

          template <class Derived>
          std::ios_base::fmtflags axis_plot_frame<Derived>::x_values_ioflags()
          { //! \return  iostream format flags of data point X values near data points markers.
            //! Might be used to set hexadecimal, fixed and scientific, (std::ios::scientific).
            return derived().x_values_style_.value_ioflags_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_plusminus_on(bool b)
          { //! Set if to append uncertainty estimate to data point X values near data points markers.
            //! (May not be implemented yet).
            derived().x_values_style_.plusminus_on_ = b;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::x_plusminus_on()
          { //! \return  if to append uncertainty estimate to data point X values near data points markers.
            //! (May not be implemented yet).
            return derived().x_values_style_.plusminus_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_plusminus_color(const svg_color& col)
          { //! Set the color of X uncertainty of value, for example, the color of 0.02 in "1.23 +-0.02 (9)".
            derived().x_values_style_.plusminus_color_ = col;
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::x_plusminus_color()
          { //! Get the color of X uncertainty of value, for example, the color of 0.02 in "1.23 +-0.02 (9)".
            return derived().x_values_style_.plusminus_color_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_df_on(bool b)
          { //! Set true if to append a degrees of freedom estimate to data point X values near data points markers.
            //! (May not be implemented yet).
            derived().x_values_style_.df_on_ = b;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::x_df_on()
          { //! \return true if to append a degrees of freedom estimate to data point X values near data points markers.
            //! (May not be implemented yet).
            return derived().x_values_style_.df_on_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_df_color(const svg_color& col)
          { //! Set the color of X degrees of freedom, for example, the color of 9 in "1.23 +-0.02 (9)".
            derived().x_values_style_.df_color_ = col;
            return derived();
          }

          template <class Derived>
          svg_color axis_plot_frame<Derived>::x_df_color()
          { //! Get the color of X degrees of freedom, for example, the color of 9 in "1.23 +-0.02 (9)".
            return derived().x_values_style_.df_color_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_decor(const std::string& pre, const std::string& sep, const std::string& suf)
          { //! Set prefix, separator and suffix for x_style
            //! Note if you want a space, you must use a Unicode space "\&#x00A0;",
            //! for example, ",\&#x00A0;" rather than ", ".
            derived().x_values_style_.prefix_ = pre;
            derived().x_values_style_.separator_ = sep;
            derived().x_values_style_.suffix_ = suf;
            return derived();
          }

          template <class Derived>
          const std::string axis_plot_frame<Derived>::x_prefix()
          { //! Get the prefix (only used if separator != "")
            return derived().x_values_style_.prefix_;
          }

          template <class Derived>
          const std::string axis_plot_frame<Derived>::x_suffix()
          { //! Get the suffix (only used if separator != "")
            return derived().x_values_style_.suffix_;
          }

          template <class Derived>
          const std::string axis_plot_frame<Derived>::x_separator()
          { //! Get separator (also controls use of the prefix & suffix - they are only used if separator != "").
            //! Note For a space, you must use a Unicode space "\&#x00A0;",
            //! for example, ",\&#x00A0;" rather than ", ".
            return derived().x_values_style_.separator_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_major_tick_length(double length)
          { //! Set length of X major ticks.
            derived().x_ticks_.major_tick_length_ = length;
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::x_major_tick_length()
          {//! \return  length of X major ticks.
            return derived().x_ticks_.major_tick_length_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_major_tick_width(double width)
          { //! Set width of X major ticks.
            derived().x_ticks_.major_tick_width_ = width; // Redundant?
            derived().image.g(PLOT_X_MAJOR_TICKS).style().stroke_width(width);
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::x_major_tick_width()
          {//! \return  width of X major ticks.
            return derived().image.g(PLOT_X_MAJOR_TICKS).style().stroke_width();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_minor_tick_length(double length)
          { //! Set length of X minor ticks.
            derived().x_ticks_.minor_tick_length_ = length;
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::x_minor_tick_length()
          { //! \return  length of X minor ticks.
            return derived().x_ticks_.minor_tick_length_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_minor_tick_width(double width)
          { //! Set width of X minor ticks.
            derived().x_ticks_.minor_tick_width_ = width;
            derived().image.g(PLOT_X_MINOR_TICKS).style().stroke_width(width);
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::x_minor_tick_width()
          { //! \return  width of X minor ticks.
            // return derived().x_minor_tick_width_; // should be the same but store in stroke_width is definitive.
            return derived().image.g(PLOT_X_MINOR_TICKS).style().stroke_width();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_major_tick(double d)
          { //! Set interval (Cartesian units) between major ticks.
            derived().x_ticks_.major_interval_ = d;
          }

          template <class Derived>
          double axis_plot_frame<Derived>::x_major_tick()
          { //! \return  interval (Cartesian units) between major ticks.
            return derived().x_ticks_.major_interval_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_minor_interval(double interval)
          { //! Set interval between X-axis minor ticks.
            // aka x_minor_tick
            derived().x_ticks_.minor_interval_ = interval;
            return derived();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_num_minor_ticks(unsigned int num)
          { //! Set number of X-axis minor ticks between major ticks.
            derived().x_ticks_.num_minor_ticks_ = num;
            return derived();
          }

          template <class Derived>
          unsigned int axis_plot_frame<Derived>::x_num_minor_ticks()
          { //! \return  number of X-axis minor ticks between major ticks.
            //! Note: NOT float or double!
            return derived().x_ticks_.num_minor_ticks_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_range(double min_x, double max_x)
          { //! Set the range of values on the X-axis.
            //! The minimum and maximum values must be finite and not too near
            //! to the minima or maxima that can be represented by floating point doubles,
            //! and the range must not be too small.
            if (!boost::math::isfinite(min_x))
            {
              throw std::runtime_error("X range: min not finite!");
            }
            if (!boost::math::isfinite(max_x))
            {
              throw std::runtime_error("X range: max not finite!");
            }
            if(max_x <= min_x)
            { // max_x <= min_x.
              throw std::runtime_error("X range: max <= min!");
            }
            if( (abs(max_x - min_x) < std::numeric_limits<double>::epsilon() * 1000 * abs(max_x))
              || (abs(max_x - min_x) < std::numeric_limits<double>::epsilon() * 1000 * abs(min_x))
              )
            { // Range too small to display.
              throw std::runtime_error("X range too small!" );
            }
            derived().x_axis_.min_ = min_x;
            derived().x_axis_.max_ = max_x;
            //derived().x_ticks_.max_ = min_x;
            //derived().y_ticks_.max_ = max_x;
            // done in calculate_plot_window, so need to duplicate here.
            derived().x_autoscale_ = false; // Because explicit range has just been set.
            return derived();
          }

          template <class Derived>
          std::pair<double, double> axis_plot_frame<Derived>::x_range()
          { //! \return  the range of values on the X-axis.
            //! (Need to use boost::svg::detail::operator<< to display this).
            std::pair<double, double> r;
            r.first = derived().x_axis_.min_;
            r.second = derived().x_axis_.max_;
            return r;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_min(double min_x)
          { //! Set the minimum value on the X-axis.
            // Not useful to check here that x_max_ > x_min_ because may not have set x_min_ yet.
            derived().x_axis_.min_ = min_x;
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::x_min()
          { //! \return  the minimum value on the X-axis.
            //! (Can also get both minimum and maximum as a std::pair).
            return derived().x_axis_.min_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_max(double x)
          { //! Set the maximum value on the X-axis.
            // Not useful to check here that x_max_ > x_min_ because may not have set x_min_ yet.
            derived().x_axis_.max_ = x;
            return derived();
          }

          template <class Derived>
          double axis_plot_frame<Derived>::x_max()
          { //! \return  the maximum value on the X-axis.
            //! (Can also get both minimum and maximum as a std::pair).
            return derived().x_axis_.max_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::autoscale_check_limits(bool b)
          { //! Set to check that values used for autoscale are within limits.
            //! Default is true, but can switch off checks for speed.
            derived().autoscale_check_limits_ = b;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::autoscale_check_limits()
          { //! \return  to check that values used for autoscale are within limits.
            //! Default is true, but can switch off checks for speed.
            return derived().autoscale_check_limits_;
          }

         template <class Derived>
          bool axis_plot_frame<Derived>::x_autoscale()
          { //! \return  true if to use autoscale value for X-axis.
           return derived().x_autoscale_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_autoscale(bool b)
          { //! Set true if to use autoscale values for X-axis.
             if (b && derived().x_auto_tick_interval_ < 0)
             { // No autoscale values have been calculated, so not safe to make x_autoscale true.
                throw std::runtime_error("X autoscale has not been calculated yet!" );
             }
            derived().x_autoscale_ = b;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::autoscale()
          { //! Set true if to use autoscale values for X-axis.
            //! autoscale() is same as x_autoscale.
           return derived().x_autoscale_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::autoscale(bool b)
          { //! Set whether to use X autoscaled values.
            //! Same as x_autoscale, and used by boxplot too.
             if (derived().x_auto_tick_interval_ < 0)
             { // No autoscale values have been calculated, so not safe to make x_autoscale true.
                throw std::runtime_error("X-axis autoscale has not been calculated yet!" );
             }
            derived().x_autoscale_ = b;
            return derived();
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_autoscale(std::pair<double, double> p)
          { // Set to use X min & max pair values to autoscale X-axis.
            scale_axis(p.first, p.second, // double min and max from pair.
              &derived().x_auto_min_value_, &derived().x_auto_max_value_, &derived().x_auto_tick_interval_, &derived().x_auto_ticks_,
              derived().autoscale_check_limits_,
              derived().x_include_zero_, derived().x_tight_, derived().x_min_ticks_, derived().x_steps_);
            derived().x_autoscale_ = true; // Default to use any calculated values?
            return derived();
          } // autoscale(pair<double, double> p)


       //  unable to match function definition to an existing declaration definition
       // 'Derived &boost::svg::detail::axis_plot_frame<Derived>::x_autoscale(const T &,const T &)'
       // existing declarations
       // 'Derived &boost::svg::detail::axis_plot_frame<Derived>::x_autoscale(const T &)'
       // 'Derived &boost::svg::detail::axis_plot_frame<Derived>::x_autoscale(const T &,const T &)'
       // 'Derived &boost::svg::detail::axis_plot_frame<Derived>::x_autoscale(std::pair<_Ty1,_Ty2>)'
       // with
       // [
       //     _Ty1=double,
       //     _Ty2=double
       // ]
       // 'Derived &boost::svg::detail::axis_plot_frame<Derived>::x_autoscale(bool)'
       //'bool boost::svg::detail::axis_plot_frame<Derived>::x_autoscale(void)'
         //
          //template <class Derived, class T> // T an STL container: array, vector ...
          //Derived& axis_plot_frame<Derived>::x_autoscale(const T& begin, const T& end)
          //{ // Data series using iterators to calculate autoscaled values.
          //    scale_axis(begin, end,
          //    &derived().x_auto_min_value_, &derived().x_auto_max_value_, &derived().x_auto_tick_interval_, &derived().x_auto_ticks_,
          //    derived().autoscale_check_limits_,
          //    derived().x_include_zero_, derived().x_tight_, derived().x_min_ticks_, derived().x_steps_);

          //  derived().x_autoscale_ = true; // Default to use calculated values.
          //  return derived();
          //} // x_autoscale(const T& begin, const T& end)

          //template <class Derived, class T> // T an STL container: array, vector ...
          //Derived& axis_plot_frame<Derived>::x_autoscale(const T& container) // Whole data series.
          //{ // to use to calculate autoscaled values.
          //    //scale_axis(container.begin(), container.end(), // All the container.
          //    scale_axis(container, // All the container.
          //    &derived().x_auto_min_value_, &derived().x_auto_max_value_, &derived().x_auto_tick_interval_, &derived().x_auto_ticks_,
          //    derived().autoscale_check_limits_,
          //    derived().x_include_zero_, derived().x_tight_, derived().x_min_ticks_, derived().x_steps_);

          //  derived().x_autoscale_ = true; // Default to use calculated values.
          //  return derived();
          //} // x_autoscale(const T& container)

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_with_zero(bool b)
          { //! Set X-axis autoscale to include zero (default = false).
            //! Must preceed x_autoscale(data) call.
            derived().x_include_zero_ = b;
            return derived();
          }

          template <class Derived>
          bool axis_plot_frame<Derived>::x_with_zero()
          { //! \return  true if X-axis autoscale to include zero (default = false).
           return derived().x_include_zero_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_min_ticks(int min_ticks)
          { //! Set X-axis autoscale to include at least minimum number of ticks (default = 6).
            //! Must preceed x_autoscale(data) call.
            derived().x_min_ticks_ = min_ticks;
            return derived();
          }

          template <class Derived>
          int axis_plot_frame<Derived>::x_min_ticks()
          { //! \return  X-axis autoscale minimum number of ticks.
           return derived().x_min_ticks_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_steps(int steps)
          { /*! Set autoscale to set ticks in steps multiples of:\n
             2,4,6,8,10, if 2\n
             or 1,5,10 if 5\n
             or 2,5,10 if 10.\n
             default = 0 (none).
             \note: Must preceed x_autoscale(data) call).
             */
            derived().x_steps_ = steps;
            return derived();
          }

          template <class Derived>
          int axis_plot_frame<Derived>::x_steps()
          { //! \return  autoscale to set ticks in steps.
           return derived().x_steps_;
          }

          template <class Derived>
          Derived& axis_plot_frame<Derived>::x_tight(double tight)
          { //! Set tolerance to autoscale to permit data points slightly outside both end ticks.
            //! default 0.
            //! Must preceed x_autoscale(data) call.
            derived().x_tight_ = tight;
            return derived();
          }

         template <class Derived>
         double axis_plot_frame<Derived>::x_tight()
         { //! \return  tolerance given to autoscale to permit data points slightly outside both end ticks.
           return derived().x_tight_;
         }

         template <class Derived>
         double axis_plot_frame<Derived>::x_auto_min_value()
         { //! \return  the X-axis minimum value computed by autoscale.
           return derived().x_auto_min_value_;
         }

         template <class Derived>
         double axis_plot_frame<Derived>::x_auto_max_value()
         { //! \return  the X-axis maximum value computed by autoscale.
           return derived().x_auto_max_value_;
         }

         template <class Derived>
         double axis_plot_frame<Derived>::x_auto_tick_interval()
         { //! \return  the X-axis major tick interal computed by autoscale.
           return derived().x_auto_tick_interval_;
         }

        template <class Derived>
        int axis_plot_frame<Derived>::x_auto_ticks()
        { //! \return  the X-axis number of major ticks computed by autoscale.
         return derived().x_auto_ticks_;
        }

        template <class Derived>
        Derived& axis_plot_frame<Derived>::limit_color(const svg_color& col)
        { //! Set the color for 'at limit' point stroke color.
          //derived().plot_window_border_.stroke_ = col;
          derived().image.g(detail::PLOT_LIMIT_POINTS).style().stroke_color(col);
          return derived();
        }

        template <class Derived>
        svg_color axis_plot_frame<Derived>::limit_color()
        { //! \return  the color for the 'at limit' point stroke color.
          return derived().image.g(detail::PLOT_LIMIT_POINTS).style().stroke_color();
        }

        template <class Derived>
        Derived& axis_plot_frame<Derived>::limit_fill_color(const svg_color& col)
        { //! Set the color for 'at limit' point fill color.
          derived().image.g(detail::PLOT_LIMIT_POINTS).style().fill_on(true);
          derived().image.g(detail::PLOT_LIMIT_POINTS).style().fill_color(col);
          return derived();
        }

        template <class Derived>
        svg_color axis_plot_frame<Derived>::limit_fill_color()
        { //! \return  the color for the 'at limit' point fill color.
          return derived().image.g(detail::PLOT_LIMIT_POINTS).style().fill_color();
        }

        template <class Derived>
        Derived& axis_plot_frame<Derived>::draw_note(double x, double y, std::string note, text_style& tsty/* = no_style*/, align_style al/* = center_align*/, rotate_style rot /*= horizontal*/, const svg_color& col)
        { /*! \brief Annotate plot with a  text string (perhaps including Unicode), putting note at SVG Coordinates X, Y.
            \details Defaults color black, rotation horizontal and align = center_align
            Using center_align is recommended as it will ensure that will center correctly
            (even if original string is made much longer because it contains Unicode,
            for example Greek or math symbols, taking about 6 characters per symbol)
            because the render engine does the centering.
          */

           // g_element* g_ptr = &(derived().image.g(detail::PLOT_NOTES));
           g_element* g = &(derived()).image.g(); // New group
           g->style().fill_color(col);
           g->push_back(new text_element(x, y, note, tsty, al, rot));

           //g_ptr->style().fill_color(red);
           //g_ptr->push_back(new text_element(x, y, note, tsty, al, rot));
          //derived().image.g(detail::PLOT_NOTES).text(x, y,
          //gptr.text(x, y,
          //          note, // string
          //          tsty, // no_style,
          //          al, //center_align,
          //          rot); // horizontal);

          // No checks on X or Y - leave to SVG to not draw outside image.
          // Could warn if X and/or Y outside - but even if OK, then text could still stray outside image.
          return derived();
        } // void draw_note()


      } // detail
    } // svg
  } // boost

#endif // BOOST_SVG_AXIS_PLOT_FRAME_HPP
