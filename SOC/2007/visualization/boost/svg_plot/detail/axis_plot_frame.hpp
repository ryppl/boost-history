// axis_plot_frame.hpp

// Copyright Jacob Voytko 2007

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
//using boost::math::fpclassify for
// boost::math::
// template <class T>bool isfinite (T);
// template <class T>bool isinf (T);
// template <class T> bool isnan (T);

#include <string>
// using std::string;

#include <iostream> // for testing only.
// using std::cerr and std::endl

#include <utility>
// using std::pair

namespace boost
{
  namespace svg
  {
    //JV: G++ can't resolve names in these enums in axis_plot_frame
    //    when they are in svg_2d_plot.hpp

    static const double sin45 = 0.707; // Use if axis value labels are sloping.

    // x_axis_position_ and y_axis_position_  use these.
    enum x_axis_intersect {bottom = -1, x_intersects_y = 0, top = +1};
    // bottom = X-axis free below bottom of end of Y-axis (case of all Y definitely < 0).
    // top = X-axis free above top of X-axis (case of all Y definitely > 0).
    // x_intersects_y when Y values include zero, so X intersects the Y axis.

    enum y_axis_intersect {left = -1, y_intersects_x = 0, right = +1};
    // left = Y-axis free to left of end of X-axis (case of all X definitely < 0).
    // right = Y-axis free to left of end of X-axis (case of all X definitely > 0).
    // y_intersects_x when X values include zero, so intersects the X axis.

    enum legend_places
    {  // Placing of legend box, if requested by legend_on(true).
      nowhere = 0,
      inside = -1,  // Default place for inside is top left of plot window,
      // exact location controlled by legend_top_left().
      outside_left = 1, // Outside
      outside_right = +2,  // Default for outside.
      outside_top = +3,
      outside_bottom = +4,
      somewhere = +5 // legend_top_left(x, y)
    };

    namespace detail
    {
      template <class Derived>
      class axis_plot_frame
      {
        // axis_plot_frame is used as base class, for example:
        // class svg_1d_plot : public detail::axis_plot_frame<svg_1d_plot>
        // class svg_2d_plot : public detail::axis_plot_frame<svg_2d_plot>
        //protected:
      public:
        // --------------------------------------------------------------------
        // We don't use the SVG coordinate transform because then text would
        // be flipped. I'm considering using it to scale the image for resizes.
        // --------------------------------------------------------------------

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
        { // Scale & shift both x & y to graph coordinate.
          x = derived().x_scale_ * x + derived().x_shift_;
          y = derived().y_scale_ * y + derived().y_shift_;
          adjust_limits(x, y); // In case either hits max, min, infinity or NaN.
        }
        // TODO do we need to adjust_limit(x); // In case hits max, min, infinity or NaN?
        // This implies that user's choice of X-axis range is wrong?
        // So more drastic action like throwing might be least worst action?

        void transform_x(double & x)
        { // Scale and shift x only.
          x = derived().x_scale_ * x + derived().x_shift_;
        }

        void transform_y(double & y)
        { // Scale and shift y only.
          y = derived().y_scale_ * y + derived().y_shift_;
        }

        void draw_x_minor_tick(double value, path_element& tick_path, path_element& grid_path)
        { // Draw X-axis minor ticks, and optional grid.
          // Value is NOT (yet) shown beside the minor tick.
          double x(value); // Tick position and value label,
          transform_x(x); // convert to svg.
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
                y_bottom += derived().title_info_.style().font_size() * derived().text_margin_;
              }
              if(derived().x_axis_.label_on_)
              {
                y_top -= derived().x_label_info_.style().font_size() * derived().text_margin_;
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
        { // draw ticks - and grid too if wanted.
          // If major_value_labels_side then value shown beside the major tick.
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
                y_up += derived().title_info_.style().font_size() * derived().text_margin_;
              }
              if(derived().x_ticks_.major_value_labels_side_ != 0)
              { // Value may be shown either side the major tick.
                y_down -= derived().x_label_info_.style().font_size() * derived().text_margin_;
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
        { // Draw horizontal X-axis line & plot window line to hold.
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

        // --------------------------------------------------------------------
        // Draw functions - legend & title (if any).
        // --------------------------------------------------------------------

        void draw_title()
        {
          // Update title_info_ with position.
          derived().title_info_.x(derived().image.x_size() / 2.); // Center of image.
          // Assumes align = center_align.
          // And center_align will ensure that will center correctly
          // even if original string is long because contains Unicode like &#x3A9;
          // because the render engine does the centering.
          double y;
          y = derived().title_info_.style().font_size() * derived().text_margin_; // Leave a linespace above.
          derived().title_info_.y(y);
          derived().image.g(PLOT_TITLE).push_back(new text_element(derived().title_info_));
        } // void draw_title()

        void size_legend_box()
        { // Calculate how big the legend box needs to be.
          // Store in legend_width_ and legend_height_
          if(derived().legend_on_ == false)
          { // No legend, so set values to show legend positions invalid?
            //derived().legend_left_ = -1.;
            //derived().legend_right_ = -1.;
            //derived().legend_top_ = -1.;
            //derived().legend_bottom_ = -1.;
            derived().legend_height_ = 0.;
            derived().legend_width_ = 0.;
            return;
          }
          else
          { // legend_on_ == true
            // Work out the size the legend box needs to be to hold the
            // header, markers, lines & text.
            size_t num_series = derived().series.size(); // How many data series.
            int font_size = derived().legend_header_.style().font_size();
            int point_size =  derived().series[0].point_style_.size();
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
              std::string s = derived().series[i].title_;
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
            if (derived().legend_lines_)
            { // Add for colored line marker in legend.
              derived().legend_width_ += spacing * 1.5;
            }
            if(derived().series[0].point_style_.shape() != none)
            { // Add for any colored data point marker, cross, round... & space.
              derived().legend_width_ += 1.5 * derived().series[0].point_style_.size();
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
        {
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
          {
            // size_t num_points = derived().series.size();
            //cout << derived().legend_box_.width() <<  ' ' << derived().legend_box_.margin() << endl;

            int font_size = derived().legend_header_.style().font_size();
            int point_size =  derived().series[0].point_style_.size();
            // Use whichever is the biggest of point marker and font.
            double spacing = (std::max)(font_size, point_size);
            // std::cerr << spacing <<  ' ' << font_size << ' ' << point_size << endl;
            bool is_header = (derived().legend_header_.text() != "");

           // Assume legend box position has already been sized by calculate_legend_box.
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
              derived().legend_header_.y(legend_y_pos);
              derived().image.g(PLOT_LEGEND_TEXT).push_back(new
                text_element(derived().legend_header_));
              legend_y_pos += 2 * spacing; // Might be 1.5? - useful if many series makes the box too tall.
            }

            g_ptr = &(derived().image.g(PLOT_LEGEND_POINTS));
            g_element* g_inner_ptr = g_ptr;
            g_inner_ptr = &(derived().image.g(PLOT_LEGEND_TEXT));

            for(unsigned int i = 0; i < derived().series.size(); ++i)
            { // Show point marker, perhaps line, & text info for all the data series.
              double legend_x_pos = legend_x_start;
              legend_x_pos += spacing; // space before point marker and/or line & text.
              g_inner_ptr = &(g_ptr->g());
              // Use both stroke & fill colors from the point's style.
              g_inner_ptr->style().stroke_color(derived().series[i].point_style_.stroke_color_);
              g_inner_ptr->style().fill_color(derived().series[i].point_style_.fill_color_);
              g_inner_ptr->style().stroke_width(derived().series[i].line_style_.width_); // Applies to shape AND line.

              if(derived().series[i].point_style_.shape_ != none)
              { // Is a data marker shape to show.
                draw_plot_point( // Plot point like circle, square...
                  legend_x_pos,
                  legend_y_pos,
                  *g_inner_ptr,
                  derived().series[i].point_style_);
                legend_x_pos += 1.5 * spacing;
              }

              // Line markers  - only really applicable to 2-D sets plot_line_style,
              if (derived().legend_lines_)
              { // Need to draw a short line to show color for that data series.
                g_inner_ptr->style() // Use stroke colors from line style.
                  .stroke_color(derived().series[i].line_style_.stroke_color_);
               // Use stroke colors from line style.
               // == image.g(PLOT_DATA_LINES).style().stroke_width(width);
                // but this sets width for BOTH point and line :-(
                g_inner_ptr->push_back(new line_element(
                  legend_x_pos,
                  legend_y_pos,
                  legend_x_pos + spacing, // line sample is one char long.
                  legend_y_pos));
                legend_x_pos += 1.5 * spacing; // short line & a space.
              } // legend_lines_

              // Legend text for each Data Series added to the plot.
              g_inner_ptr = &(derived().image.g(PLOT_LEGEND_TEXT));
              g_inner_ptr->push_back(new text_element(
                legend_x_pos, // allow space for the marker.
                legend_y_pos,
                derived().series[i].title_, // Text for this data series.
                derived().legend_header_.style(),
                left_align));
              legend_y_pos += 2 * spacing;
            } // for
          } // void draw_legend()

          void draw_x_label()
          {
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
              derived().x_label_info_.style(),
              center_align, horizontal)
              );
          } // void draw_x_label()

          void adjust_limits(double& x, double& y)
          { // If value reaches limit of max, min, infinity,
            // use the appropriate plot min or max.
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
            // TODO Do we want/get a different color or shape for NaNs??
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
          {
            int size = sty.size_;
            double half_size = size / 2.;

            // For 1-D plots, the points do not *need* to be centered on the X-axis,
            // and putting them just above, or sitting on, the X-axis is much clearer.
            // For 2-D plots, the symbol center should, of course,
            // be *centered exactly* on x, y.
            // circle and ellipse are naturally centered on the point.
            // for rect x and y half_size offset centers square on the point.
            // But symbols are in a rectangular box and the offset is different for x & y
            // even assuming that the symbol is centered in the rectangle.
            // the vertical and horizontal ticks are deliberately offset above the axes.
            // TODO Not sure this is fully resolved.

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
              // TODO stroke color of line seems to be FILL color, not the stroke color.
              // This is OK-ish, but I'm not sure why.

              //svg_color sc = sty.stroke_color;
              //svg_color fc = sty.fill_color;

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
              g_ptr.triangle(x - half_size, y - size, x + half_size, y - size, x, y, false);
              // Last point puts the bottom tip of the triangle on the X-axis.
              // This may not be wanted for 2-D.
              break;
            case cross:
              g_ptr.line(x, y + size, x , y - size); // line up & down from axis,
              g_ptr.line(x, y - size, x + size, y ); // & line left & right from axis.
              // Cross is pretty useless for 1-D because the horizontal line is on the X-axis.
              break;
              // TODO Other point_shapes do nothing yet.
            }
          } // void draw_plot_point

          void draw_plot_point_value(double x, double y, g_element& g_ptr, value_style& sty, double value)
          { // Write the value as string of a data point, X or Y .
            std::stringstream label;
            label.precision(sty.value_precision_);
            label.flags(sty.value_ioflags_);
            label << value; // "1.2" or "3.4e+001"...
            std::string stripped =  (derived().x_ticks_.strip_e0s_) ?
              // Default is to strip unecessary e, +, & leading exponent zeros.
              strip_e0s(label.str())  // "1.2" or "3.4e1"...
              :
              stripped = label.str();
            //plot_point_style& sty)
            //int size = sty.size_;
            int marker_size = derived().series[0].point_style_.size_;
            int label_size = sty.values_text_style_.font_size();
            // Offset of value label from point must be related mainly to
            // size of the data marker, less the value label font size.

            //enum rotate_style
            //{ // Rotation in degrees from horizontal.
            //  horizontal = 0, // normal left to right.
            //  uphill = -45, // slope up.
            //  upward = -90, // vertical writing up.
            //  backup = -135, // slope up backwards - upside down!
            //  downhill = 45, // slope down.
            //  downward = 90,  // vertical writing down.
            //  backdown = 135, // slope down backwards.
            //  upsidedown = 180 // == -180
            //};
            rotate_style rot = sty.value_label_rotation_;
            // http://www.w3.org/TR/SVG/coords.html#RotationDefined
            // transform="rotate(-45)"

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
            case uphill: // -45 - OK
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
            case downhill: // OK
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

             g_ptr.text(x, y, stripped, sty.values_text_style_, al, rot);
             //g_ptr.text(x, y, stripped, derived().x_values_style_, al, rot);
          } // void draw_plot_point_value(double x, double y, g_element& g_ptr, double value)

          // TODO draw_plot_point_values function should probably be in svg_2d_plot as never used in 1D.

          std::string sv(double v, const value_style& sty, bool unc = false)
          { // Build // Strip unecessary e, +, & leading exponent zeros, reducing to "1.2, 3.4" or "3.4e1, 5.6e1"...
            std::stringstream label;
            // Precision of uncertainty is usually less than precision of value,
            // label.precision((unc) ? ((sty.value_precision_ > 3) ?  sty.value_precision_-2 : 1) : sty.value_precision_);
            // Possible but simpler to fix at precision=2
            label.precision((unc) ? 2 : sty.value_precision_);
            label.flags(sty.value_ioflags_);
            label << v; // "1.2, 3.4" or "3.4e+001, 5.6e+001"...
            return  (sty.strip_e0s_) ?
              // Default is to strip unecessary e, +, & leading exponent zeros.
              strip_e0s(label.str())  // reduce to "1.2, 3.4" or "3.4e1, 5.6e1"...
              :
              label.str();  // Unstripped.
          } // std::string sv(double v, const value_style& sty)

          void draw_plot_point_values(double x, double y, g_element& x_g_ptr, g_element& y_g_ptr, const value_style& x_sty, const value_style& y_sty, double vx, double vy)
          { // Write the *pair* of values as string of a data point.

            std::string label_xv = sv(vx, x_sty);
            std::string label_yv = sv(vy, y_sty);
            std::string label_xu;
            std::string label_yu;
            std::string label_xdf;
            std::string label_ydf;

            int marker_size = derived().series[0].point_style_.size_;
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
            case uphill: // -45 - OK
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
            case downhill: // OK
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


            // Unclear how to get this uncertainty information into this function,
            // so these arepurely imaginary for now.
            double ux = 0.0123;
            double uy = 0.00321;
            double dfx = 23;
            double dfy = 6;

            // Tasteless colors and font changes are purely proof of concept.

             int fx = static_cast<int>(y_sty.values_text_style_.font_size() * 0.8);
           // X value, & optional uncertainty & df.
            text_element& t = x_g_ptr.text(x, y,
              label_xv, x_sty.values_text_style_, al, rot);
           // Optionally, show uncertainty as 95% confidence plus minus:  2.1 +-0.012 (23)
           // TODO comma separator ? might want inside brackets [], or another separator?

            std::string pm = "&#x00A0;&#x00B1;"; // Unicode space plusminus glyph.
            // Spaces seem to get lost, so use 00A0 as an explicit space glyph.
            // Layout seems to vary with font Times New Roman leaves no space after.
            if (x_sty.plusminus_on_)
            {  // Uncertainty estimate usually 95% confidence interval + or - 2 standard deviation.
              label_xu = sv(ux, x_sty, true);
              t.tspan(pm).fill_color(darkcyan);
              t.tspan(label_xu).fill_color(darkcyan).font_weight("bold").font_size(fx);
            }
            if (x_sty.df_on_)
            { // Degrees of freedom or number of values-1 used for this estimate of value.
              std::stringstream label;
              label.precision(4);
              //label.flags(sty.value_ioflags_); // Leave at default.
              label << "&#x00A0;(" << dfx << ")"; // "123.5"
              label_xdf = label.str();
              t.tspan(label_xdf + ',').fill_color(brown).font_size(fx);
            }

            // Put Y value on the line below using font_size.
            // Problem here if orientation is changed!
            text_element& ty = y_g_ptr.text(x, y + y_sty.values_text_style_.font_size() * 1.2,
              label_yv, y_sty.values_text_style_, al, rot);

            int fy = static_cast<int>(y_sty.values_text_style_.font_size() * 0.8);
            if (y_sty.plusminus_on_)
            {  // Uncertainty estimate usually 95% confidence interval + or - 2 standard deviation.
               // Precision of uncertainty is usually less than value,
              label_yu = "&#x00A0;" + sv(uy, y_sty, true);
              ty.tspan(pm).font_family("arial").font_size(fy).fill_color(green);
              ty.tspan(label_yu).fill_color(green).font_size(fy);
            }
            if (y_sty.df_on_)
            { // degrees of freedom or number of values -1 used for this estimate.
              std::stringstream label;
              label.precision(4);
              //label.flags(sty.value_ioflags_); // Leave at default.
              label <<"&#x00A0;(" << dfy << ")"; // "123.5"
              label_ydf = label.str();
              ty.tspan(label_ydf).fill_color(lime).font_size(fy);
            }
          } // void draw_plot_point_values(double x, double y, g_element& g_ptr, double value)

          // -----------------------------------------------------------------
          // Clear Functions.
          // When writing to multiple documents, the contents of the plot
          // may change significantly between. Rather than figuring out what
          // has and has not changed, just erase the contents of the
          // legend, title... in the document and start over.
          // -----------------------------------------------------------------

          void clear_all()
          {
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
          {
            derived().image.g(PLOT_BACKGROUND).clear();
          }

          void clear_title()
          {
            derived().image.g(PLOT_TITLE).clear();
          }

          void clear_points()
          {
            derived().image.g(PLOT_DATA_POINTS).clear();
          }

          void clear_plot_background()
          {
            derived().image.g(PLOT_WINDOW_BACKGROUND).clear();
          }

          void clear_legend()
          {
            derived().image.g(PLOT_LEGEND_BACKGROUND).clear();
            derived().image.g(PLOT_LEGEND_POINTS).clear();
            derived().image.g(PLOT_LEGEND_TEXT).clear();
          }

          void clear_x_axis()
          {
            derived().image.g(PLOT_X_AXIS).clear();
            derived().image.g(PLOT_X_MINOR_TICKS).clear();
            derived().image.g(PLOT_X_MAJOR_TICKS).clear();
            derived().image.g(PLOT_X_LABEL).clear();
            derived().image.g(PLOT_VALUE_LABELS).clear();
          }

          void clear_y_axis()
          {
            derived().image.g(PLOT_Y_AXIS).clear();
          }

          void clear_grids()
          {
            derived().image.g(PLOT_X_MAJOR_GRID).clear();
            derived().image.g(PLOT_X_MINOR_GRID).clear();
            derived().image.g(PLOT_Y_MAJOR_GRID).clear();
            derived().image.g(PLOT_Y_MINOR_GRID).clear();
          }

        private:
          Derived& derived()
          {
            return static_cast<Derived&>(*this);
            // http://en.wikipedia.org/wiki/Curiously_Recurring_Template_Pattern
          }
          const Derived& derived()const
          {
            return static_cast<const Derived&>(*this);
          }
        public:
          // Set & get member function Declarations:
          // See svg_fwd.hpp
          // -----------------------------------------------------------------

          // Get functions:
          // =========== Image & plot window ==============
          // Derived& image_size(unsigned int x, unsigned int y);
          // unsigned int image_x_size();
          // unsigned int image_y_size()
          // bool plot_window_on()
          // svg_color background_color()
          // svg_color background_border_color()
          // svg_color plot_background_color()
          //
          // std::pair<double, double> plot_window_x()
          // std::pair<double, double> plot_window_y()

          // ========= Title & legend ============
          // bool title_on()
          // const std::string title()
          // unsigned int title_font_size()
          // const std::string title_font()
          // svg_color title_color()
          // bool legend_on()
          // const std::string legend_title()
          // svg_color legend_background_color()
          // svg_color legend_border_color()
          // unsigned int legend_title_font_size()
          // ========= Axes & Ticks ============
          // bool x_axis_on()
          // unsigned int x_axis_width()
          // bool x_external_style_on()
          // bool x_ticks_up_on()
          // bool x_ticks_down_on()
          // std::pair<double, double> x_range()
          // double x_minimum()
          // double x_maximum()
          // double x_major_interval()
          // unsigned int x_num_minor__ticks()
          // double x_major_tick()
          // svg_color x_major_tick_color()
          // svg_color x_minor_tick_color()
          // unsigned int x_major_tick_length()
          // unsigned int x_major_tick_width_()
          // unsigned int x_minor_tick_length_()
          // unsigned int x_minor_tick_width_()
          // ========= Labels ============
          // bool x_label_on() // Show X-axis label text.
          // std::string x_label()
          // bool x_label_units_on() //
          // std::string x_label_units() // Show X-axis units text.
          // int x_major_labels_side()
          // svg_color x_label_color()
          // bool axes_on()
          // svg_color x_axis_color()
          // bool y_axis_on()
          // svg_color y_axis_color()
          // std::string y_label()
          // svg_color y_label_color()
          // ========= grid ============
          // bool x_major_grid_on()
          // bool x_minor_grid_on()
          // unsigned int x_major_grid_width()
          // unsigned int x_minor_grid_width()
          // svg_color x_major_grid_color()
          // svg_color x_minor_grid_color()

          // Set functions:

          //Derived& image_size(unsigned int x, unsigned int y)
          //Derived& image_x_size(unsigned int x);
          //Derived& image_y_size(unsigned int y);

          //Derived& title(const std::string& title)

          //Derived& document_title(const std::string&);
          //Derived& description(const std::string&);
          //Derived& copyright_date(const std::string&);
          //Derived& copyright_holder(const std::string&);

          //Derived& title_font_size(unsigned int size)
          //Derived& legend_title_font_size(unsigned int size)
          //Derived& legend_on(bool cmd)
          //Derived& plot_window_on(bool cmd)
          //Derived& plot_window_x(unsigned int min_x, unsigned int max_x)
          //Derived& plot_window_y(unsigned int min_y, unsigned int max_y)
          //Derived& x_external_style_on(bool cmd)
          //Derived& x_ticks_up_on(bool cmd)
          //Derived& x_ticks_down_on(bool cmd)
          //Derived& x_label_on(bool cmd)
          //Derived& x_label_units_on(bool cmd)
          //Derived& x_major_labels_side(int cmd)
          //Derived& title_on(bool cmd)
          //Derived& x_major_grid_on(bool is)
          //Derived& x_minor_grid_on(bool is)
          //Derived& axes_on(bool is)
          //Derived& x_axis_on(bool is)
          //Derived& y_axis_on(bool is);
          //Derived& title_color(const svg_color& col)
          //Derived& background_color(const svg_color& col)
          //Derived& legend_background_color(const svg_color& col)
          //Derived& legend_border_color(const svg_color& col)
          //Derived& legend_title_font_size(unsigned int size)
          //Derived& legend_color(const svg_color& col)
          //Derived& legend_font_family(const std::string& family)
          //Derived& legend_font_weight(const std::string& weight)
          //Derived& background_border_color(const svg_color& col)
          //Derived& background_border_width(double width)
          //Derived& plot_background_color(const svg_color& col)
          //Derived& x_axis_color(const svg_color& col)
          //Derived& y_axis_color(const svg_color& col)
          //Derived& x_label_color(const svg_color& col)
          //Derived& x_label_color(const svg_color& col)
          //Derived& y_label_color(const svg_color& col)
          //Derived& x_major_tick_color(const svg_color& col)
          //Derived& x_minor_tick_color(const svg_color& col)
          //Derived& x_major_grid_color(const svg_color& col)
          //Derived& x_major_grid_width(unsigned int w)
          //Derived& x_minor_grid_color(const svg_color& col)
          //Derived& x_minor_grid_width(unsigned int w)
          //Derived& x_axis_width(unsigned int width)
          //Derived& x_label(const std::string& str)
          //Derived& x_label_units(const std::string& str)
          //Derived& y_label(const std::string& str)
          //Derived& x_major_interval(double inter)
          //Derived& x_major_tick_length(unsigned int length)
          //Derived& x_major_tick_width_(unsigned int width)
          //Derived& x_minor_tick_length_(unsigned int length)
          //Derived& x_minor_tick_width_(unsigned)
          //Derived& x_major_tick(double d) int width)
          //Derived& x_num_minor_ticks(unsigned int num)
          //Derived& x_range(double min_x, double max_x)
          //Derived& x_minimum(double min_x)
          //Derived& x_maximum(double max_x)
          //Derived& load_stylesheet(const std::string& file)
          // svg& get_svg()

          // Shapes and glyphs need BOTH fill and stroke to be set.
          // Both are usually the same in this application.
          // If both are set, stroke is considered 'more important',
          // and so is returned by get functions.


          // Member functions to set plot options.
          // All return derived() == *this to permit chaining.

          //-------------------------------------------------------

          Derived& image_size(unsigned int x, unsigned int y)
          { // Might put default sizes here?
            // Check on sanity of these values?
            derived().image.image_size(x, y);
            return derived();
          }

          unsigned int image_x_size()
          {
            return derived().image.x_size();
          }

          Derived& image_x_size(unsigned int i)
          {
            derived().image.x_size(i);
            return derived();
          }

          unsigned int image_y_size()
          {
            return derived().image.y_size();
          }

          Derived& image_y_size(unsigned int i)
          {
            derived().image.y_size(i);
            return derived();
          }

          svg_color background_color()
          {
            return derived().image.g(PLOT_BACKGROUND).style().fill_color();
          }

          Derived& background_border_color(const svg_color& col)
          {
            derived().image.g(PLOT_BACKGROUND).style().stroke_color(col);
            return derived();
          }

          svg_color background_border_color()
          {
            return derived().image.g(PLOT_BACKGROUND).style().stroke_color();
          }

          Derived& background_border_width(double w)
          {
            derived().image.g(PLOT_BACKGROUND).style().stroke_width(w);
            return derived();
          }

          double background_border_width()
          {
            return derived().image.g(PLOT_BACKGROUND).style().stroke_width();
          }

          Derived& description(const std::string d)
          { // Writes description to the document(for header as <desc>).
            derived().image.description(d);
            return derived();
          }

          const std::string& description()
          { // Gets description of the document(for header as <desc>).
            return derived().image.description();
          }

          Derived& document_title(const std::string d)
          { // Writes document title to the document(for header as <title>)..
            derived().image.document_title(d);
            return derived();
          }
          std::string document_title()
          { // Get document title to the document(for header as <title>)..
            return derived().image.document_title();
          }

          Derived& copyright_holder(const std::string d)
          { // Writes copyright_holder to the document
            // (for header as <!-- SVG Plot Copyright Paul A. Bristow 2007 --> )
            // and as metadata: meta name="copyright" content="Paul A. Bristow" />
            derived().image.copyright_holder(d);
            return derived();
          }

          const std::string copyright_holder()
          { // Get copyright_holder.
            return derived().image.copyright_holder();
          }

          Derived& copyright_date(const std::string d)
          { // Writes copyright_date to the document.
            // and as metadata <meta name="date" content="2007" />
            derived().image.copyright_date(d);
            return derived();
          }

          const std::string copyright_date()
          { // Get copyright_date.
            return derived().image.copyright_date();
          }

          Derived& license(std::string repro = "permits",
            std::string distrib = "permits",
            std::string attrib = "requires",
            std::string commercial = "permits",
            std::string derivative = "permits")
          { // Might check these are "permits", "requires", or "prohibits"?
            derived().image.license(repro, distrib, attrib, commercial, derivative);
            return derived();
          }

          bool license_on()
          {
            return derived().image.license_on();
          }

          Derived&  license_on(bool l)
          {
            derived().image.license_on(l);
            return derived();
          }
          bool boost_license_on()
          {
            return derived().image.boost_license_one();
          }

          Derived& boost_license_on(bool l)
          {
            derived().image.boost_license_on(l);
            return derived();
          }

          const std::string license_reproduction()
          { // Get copyright_date.
            return derived().image.reproduction();
          }

          const std::string license_distribution()
          { // Get copyright_date.
            return derived().image.distribution();
          }

          const std::string license_attribution()
          { // Get copyright_date.
            return derived().image.attribution();
          }

          const std::string license_commercialuse()
          { // Get copyright_date.
            return derived().image.commercialuse();
          }

          Derived& coord_precision(int digits)
          { // Precision of coordinates in decimal digits (default 3).
            derived().image.coord_precision(digits);
            return derived();
          }

          int coord_precision()
          { //
            return derived().image.coord_precision();
          }

          Derived& x_value_precision(int digits)
          { // Precision of X tick label values in decimal digits (default 3).
            derived().x_ticks_.value_precision_ = digits;
            return derived();
          }

          int x_value_precision()
          { //
            return derived().x_ticks_.value_precision_;
          }

          Derived& x_value_ioflags(int flags)
          { // IO flags of X tick label values (default 0X201).
            derived().x_ticks_.value_ioflags_ = flags;
            return derived();
          }

          int x_value_ioflags()
          { // ALL stream ioflags for control of format of X value labels.
            return derived().x_ticks_.value_ioflags_;
          }

          Derived& x_labels_strip_e0s(bool cmd)
          {
            derived().x_ticks_.strip_e0s_ = cmd;
            return derived();
          }

          bool y_labels_strip_e0s()
          {
            return derived().x_ticks_.strip_e0s_;
          }

          Derived& title(const std::string title)
          { // Plot title.  TODO
            // new text parent code pushback
            // effectively concatenates with any existing title.
            // So clear the existing string first but doesn't work,
            // so need to clear the whole g_element.
            //derived().image.g(PLOT_TITLE).clear();
            derived().title_info_.text(title);
            derived().title_on_ = true; // Assume display wanted, if bother to set title.
            return derived();
          }

          const std::string title()
          {
            return derived().title_info_.text();
          }

          Derived& title_font_size(unsigned int i)
          {
            derived().title_info_.style().font_size(i);
            return derived();
          }

          unsigned int title_font_size()
          {
            return derived().title_info_.style().font_size();
          }

          Derived& title_font_family(const std::string& family)
          {
            derived().title_info_.style().font_family(family);
            return derived();
          }

          const std::string& title_font_family()
          {
            return derived().title_info_.style().font_family();
          }

          Derived& title_font_style(const std::string& style)
          {
            derived().title_info_.style().font_style(style);
            return derived();
          }

          const std::string& title_font_style()
          {
            return derived().title_info_.style().font_style();
          }

          Derived& title_font_weight(const std::string& weight)
          {
            derived().title_info_.style().font_weight(weight);
            return derived();
          }

          const std::string& title_font_weight()
          {
            return derived().title_info_.style().font_weight();
          }

          Derived& title_font_stretch(const std::string& stretch)
          {
            derived().title_info_.style().font_stretch(stretch);
            return derived();
          }

          const std::string& title_font_stretch()
          {
            return derived().title_info_.style().font_stretch();
          }

          Derived& title_font_decoration(const std::string& decoration)
          {
            derived().title_info_.style().font_decoration(decoration);
            return derived();
          }

          const std::string& title_font_decoration()
          {
            return derived().title_info_.style().font_decoration();
          }

          Derived& title_font_rotation(rotate_style rotate)
          { // Degrees (0 to 360).
            derived().title_info_.rotation(rotate);
            return derived();
          }

          int title_font_rotation()
          {
            return derived().title_info_.rotation();
          }

          Derived& title_font_alignment(align_style alignment)
          {
            derived().title_info_.alignment(alignment);
            return derived();
          }

          align_style title_font_alignment()
          {
            return derived().title_info_.alignment();
          }

          // Legend.

          Derived& legend_width(double width)
          {
            derived().legend_width_ = width;
            return derived();
          }

          double legend_width()
          {
            return derived().legend_width_;
          }

          Derived& legend_title(const std::string title)
          {
            derived().legend_header_.text(title);
            return derived();
          }

          const std::string legend_title()
          {
            return derived().legend_header_.text();
          }

          Derived& legend_font_weight(const std::string& weight)
          {
            derived().legend_header_.style().font_weight(weight);
            return derived();
          }

          const std::string& legend_font_weight()
          {
            return derived().legend_header_.style().font_weight();
          }

          Derived& legend_font_family(const std::string& family)
          {
            derived().legend_header_.style().font_family(family);
            return derived();
          }

          const std::string& legend_font_family()
          {
            return derived().legend_header_.style().font_family();
          }

          Derived& legend_title_font_size(unsigned int size)
          {
            derived().legend_header_.style().font_size(size);
            return derived();
          }

          unsigned int legend_title_font_size()
          {
            return derived().legend_header_.style().font_size();
          }

          Derived& legend_top_left(double x, double y)
          { // Position of top left of legend box (svg coordinates).
            // Bottom right is controlled by contents, so cannot set it.
            if((x < 0) || (x > derived().image.x_size()) || (y < 0) || (y > derived().image.y_size()))
            {
              throw std::runtime_error("Legend box position outside image!");
            }
            derived().legend_left_ = x;
            derived().legend_top_ = y;
            return derived();
          }

          const std::pair<double, double> legend_top_left()
          {// Top left of legend box.
            std::pair<double, double> r;
            r.first = derived().legend_left_;
            r.second = derived().legend_top_;
            return r;
          }

          const std::pair<double, double> legend_bottom_right()
          {// Bottom right of legend box.
            std::pair<double, double> r;
            r.first = derived().legend_right_;
            r.second = derived().legend_bottom_;
            return r;
          }

          Derived& legend_lines(bool is)
          { // If legend should include samples of the lines joining data points.
            derived().legend_lines_ = is;
            return derived();
          }

          bool legend_lines()
          {
            return derived().legend_lines_;
          }

          Derived& legend_on(bool cmd)
          {
            derived().legend_on_ = cmd;
            return derived();
          }

          bool legend_on()
          {
            return derived().legend_on_;
          }

          Derived& legend_place(legend_places l)
          {
            derived().legend_place_ = l;
            return derived();
          }

          legend_places legend_place()
          {
            return derived().legend_place_;
          }

          bool legend_outside()
          {
            return derived().outside_legend_on_;
          }


          Derived& legend_header_font_size(int size)
          {
            derived().legend_header_.style().font_size(size);
            return *this;
          }

          int legend_header_font_size()
          {
            return derived().legend_header_.style().font_size();
          }

          Derived& plot_window_on(bool cmd)
          {
            derived().plot_window_on_ = cmd;

            if(cmd)
            { // Set plot window
              derived().image.g(detail::PLOT_WINDOW_BACKGROUND).style()
                .fill_color(derived().plot_window_border_.fill_) // background color and
                .stroke_color(derived().plot_window_border_.stroke_); // border color.
            }
            //derived().legend_place_ = outside_right;
            return derived();
          }

          bool plot_window_on()
          {
            return derived().plot_window_on_;
          }

          Derived& plot_border_color(const svg_color& col)
          {
            derived().plot_window_border_.stroke_ = col;
            derived().image.g(detail::PLOT_WINDOW_BACKGROUND).style().stroke_color(col);
            return derived();
          }

          svg_color plot_border_color()
          {
            return derived().image.g(detail::PLOT_WINDOW_BACKGROUND).style().stroke_color();
          }

          double plot_border_width()
          {
            return derived().image.g(detail::PLOT_WINDOW_BACKGROUND).style().stroke_width();
          }

          Derived& plot_border_width(double w)
          {
            derived().plot_window_border_.width_ = w;
            derived().image.g(detail::PLOT_WINDOW_BACKGROUND).style().stroke_width(w);
            return derived();
          }

          Derived& image_border_margin(double w)
          {
            derived().image_border_.margin_ = w;
            return derived();
          }

          double image_border_margin()
          {
            return derived().image_border_.margin_;
          }

          Derived& image_border_width(double w)
          {
            derived().image_border_.width_ = w;
            return derived();
          }

          double image_border_width()
          {
            return derived().image_border_.width_;
          }

          Derived& plot_window_x(double min_x, double max_x)
          { // This is normally calculated from other plot values.
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

          Derived& plot_window_y(double min_y, double max_y)
          { // This is normally calculated from other plot values.

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

          std::pair<double, double> plot_window_x()
          {
            std::pair<double, double> r;
            r.first = derived().plot_left_;
            r.second = derived().plot_right_;
            return r;
          }

          double plot_window_x_left()
          {
            return derived().plot_left_;
          }
          double plot_window_x_right()
          {
            return derived().plot_right_;
          }
          double plot_window_y_top()
          {
            return derived().plot_top_;
          }
          double plot_window_y_bottom()
          {
            return derived().plot_bottom_;
          }

          std::pair<double, double> plot_window_y()
          {
            std::pair<double, double> r;
            r.first = derived().plot_top_;
            r.second = derived().plot_bottom_;
            return r;
          }

          double x_minor_interval()
          {
            return derived().x_ticks_.minor_interval_;  // interval
          }

          double y_minor_interval()
          {
            return derived().y_ticks_.minor_interval_;  // interval
          }

          Derived& x_ticks_up_on(bool cmd)
          {
            derived().x_ticks_.up_ticks_on_ = cmd;
            return derived();
          }

          bool x_ticks_up_on()
          {
            return derived().x_ticks_.up_ticks_on_;
          }

          Derived& x_ticks_down_on(bool cmd)
          {
            derived().x_ticks_.down_ticks_on_ = cmd;
            return derived();
          }

          bool x_ticks_down_on()
          {
            return derived().x_ticks_.down_ticks_on_;
          }
          // Only need y_ticks_left_on & y_ticks_right_on in 2D

          Derived& x_label_on(bool cmd)
          { // Show X-axis label text, or not.
            // Also switched on by setting label text.
            // (on the assumption that if label text is set, display is also wanted,
            // but can be switched off if *not* required).
            derived().x_axis_.label_on_ = cmd;
            return derived();
          }

          bool x_label_on()
          {
            return derived().x_axis_.label_on_;
          }

          Derived& x_label_font_size(unsigned int i)
          {
            derived().x_label_info_.style().font_size(i);
            return derived();
          }

          unsigned int x_label_font_size()
          {
            return derived().x_label_info_.style().font_size();
          }

          Derived& x_value_font_size(unsigned int i)
          {
            derived().x_value_value.style().font_size(i);
            return derived();
          }

          unsigned int x_value_font_size()
          {
            return derived().x_value_value.style().font_size();
          }

          Derived& x_label_font_family(const std::string& family)
          {
            derived().x_label_info_.style().font_family(family);
            return derived();
          }

          const std::string& x_label_font_family()
          {
            return derived().x_label_info_.style().font_family();
          }

         Derived& x_axis_label_color(const svg_color& col)
          { // Set BOTH stroke and fill to the same color.
            derived().image.g(detail::PLOT_X_LABEL).style().fill_color(col);
            derived().image.g(detail::PLOT_X_LABEL).style().stroke_color(col);
            return *this;
          }

          svg_color x_axis_label_color()
          { // But only return the stroke color.
            return derived().image.g(detail::PLOT_X_LABEL).style().stroke_color();
          }

          Derived& x_axis_value_color(const svg_color& col)
          { // Set BOTH stroke and fill to the same color.
            derived().image.g(detail::PLOT_VALUE_LABELS).style().fill_color(col);
            derived().image.g(detail::PLOT_VALUE_LABELS).style().stroke_color(col);
            return *this;
          }

          svg_color x_axis_value_color()
          { // But only return the stroke color.
            return derived().image.g(detail::PLOT_VALUE_LABELS).style().stroke_color();
          }

          Derived& x_ticks_on_window_or_axis(int cmd)
          { // External style, top = +1, bottom = -1 (default).
            derived().x_ticks_.ticks_on_window_or_axis_ = cmd;
            return derived();
          }

          int x_ticks_on_window_or_axis()
          { // External style = true.
            return derived().x_ticks_.ticks_on_window_or_axis_;
          }

          Derived& x_label_units_on(bool cmd)
          {
            derived().x_axis_.label_units_on_ = cmd;
            return derived();
          }

          bool x_label_units_on()
          {
            return derived().x_axis_.label_units_on_;
          }

          Derived& x_major_value_labels_side(int cmd)
          {
            derived().x_ticks_.major_value_labels_side_ = cmd;
            return derived();
          }

          int x_major_value_labels_side()
          {
            return derived().x_ticks_.major_value_labels_side_;
          }

          Derived& x_major_label_rotation(rotate_style rot)
          {
            derived().x_ticks_.label_rotation_ = rot;
            return derived();
          }

          rotate_style x_major_label_rotation()
          {
            return derived().x_ticks_.label_rotation_;
          }

          Derived& title_on(bool cmd)
          {
            derived().title_on_ = cmd;
            return derived();
          }

          bool title_on()
          {
            return derived().title_on_;
          }

          Derived& x_major_grid_on(bool is)
          {
            derived().x_ticks_.major_grid_on_ = is;
            return derived();
          }

          bool x_major_grid_on()
          {
            return derived().x_ticks_.major_grid_on_;
          }

          Derived& x_minor_grid_on(bool is)
          {
            derived().x_ticks_.minor_grid_on_ = is;
            return derived();
          }

          bool x_minor_grid_on()
          {
            return derived().x_ticks_.minor_grid_on_;
          }

          Derived& axes_on(bool is)
          { // Draw *both* x and y axes (note plural).
            derived().x_axis_.axis_line_on_ = is;
            derived().y_axis_.axis_line_on_ = is; // Unsuitable for 1D?
            return derived();
          }

          bool axes_on()
          { // Used X in preference to Y for 1D, but now require *both* x and y axis on.
            return derived().x_axis_.axis_line_on_ && derived().y_axis_.axis_line_on_;
          }

          Derived& x_axis_on(bool is)
          { // Draw a horizontal x_axis_ line.
            derived().x_axis_.axis_line_on_ = is;
            return derived();
          }

          bool x_axis_on()
          { // Use X in preference to Y for 1D
            return derived().x_axis_.axis_line_on_;
          }

          Derived& y_axis_on(bool is)
          {// Draw a vertical y_axis_ line.
            derived().y_axis_.axis_line_on_ = is;
            return derived();
          }

          bool y_axis_on()
          { // Should be always false for 1D.
            return derived().y_axis_.axis_line_on_;
          }

          // enums like PLOT_TITLE provide a std:string like "title"
          // colors .stroke_color, .stroke_width and font are set in the appropriate g_element.

          Derived& title_color(const svg_color& col)
          { // Function title_color could set both fill (middle) and stroke (outside),
            // but just setting fill if simplest,
            // but does not allow separate inside & outside colors.
            derived().image.g(PLOT_TITLE).style().fill_color(col);
            //derived().image.g(PLOT_TITLE).style().stroke_color(col);
            return derived();
          }

          svg_color title_color()
          { // Function title_color could get either fill and stroke,
            // return derived().image.g(PLOT_TITLE).style().stroke_color();
            return derived().image.g(PLOT_TITLE).style().fill_color();
          }

          //Derived& title_font_width(double width)
          //{ // width of text is effectively the boldness.
          //  // Not useful with current browsers, setting this may cause lower quality graphic fonts
          //  // perhaps because the font is created using graphics rather than a built-in font.
          //  derived().image.g(PLOT_TITLE).style().stroke_width(width);
          //  return derived();
          //}

          //double title_font_width()
          //{
          //  return derived().image.g(PLOT_TITLE).style().stroke_width();
          //}

          Derived& legend_color(const svg_color& col)
          {
            // derived().image.g(PLOT_LEGEND_TEXT).style().stroke_color(col);
            derived().image.g(PLOT_LEGEND_TEXT).style().fill_color(col);
            return derived();
          }

          svg_color legend_color()
          { // Function legend_color sets only stroke, assuming that 'filled' text is not being used.
            // (It produces much lower quality fonts on some browsers).
            return derived().image.g(PLOT_LEGEND_TEXT).style().fill_color();
            // return derived().image.g(PLOT_LEGEND_TEXT).style().stroke_color();
          }

          //Derived& legend_font_width(double width)
          //{ // width of text is effectively the boldness.
          //  derived().image.g(PLOT_LEGEND_TEXT).style().stroke_width(width);
          //  return derived();
          //}

          //double legend_font_width()
          //{ // Probably not useful at present (se above).
          //  return derived().image.g(PLOT_LEGEND_TEXT).style().stroke_width();
          //}

          Derived& background_color(const svg_color& col)
          { // plot background
            derived().plot_window_border_.fill(col);
            derived().image.g(PLOT_BACKGROUND).style().fill_color(col);
            return derived();
          }

          Derived& legend_background_color(const svg_color& col)
          {
            derived().legend_box_.fill(col);
            derived().image.g(PLOT_LEGEND_BACKGROUND).style().fill_color(col);
            return derived();
          }

          svg_color legend_background_color()
          {
            return derived().image.g(PLOT_LEGEND_BACKGROUND).style().fill_color();
          }

          bool legend_box_fill_on()
          {
            return derived().legend_box_.fill_on();
          }

          Derived& legend_border_color(const svg_color& col)
          {
            derived().legend_box_.stroke(col);
            derived().image.g(PLOT_LEGEND_BACKGROUND).style().stroke_color(col);
            return derived();
          }

          svg_color legend_border_color()
          {
            return  derived().legend_box_.stroke();
            // return derived().image.g(PLOT_LEGEND_BACKGROUND).style().stroke_color();
          }

          Derived& plot_background_color(const svg_color& col)
          {
            derived().image.g(PLOT_WINDOW_BACKGROUND).style().fill_color(col);
            return derived();
          }

          svg_color plot_background_color()
          {
            return derived().image.g(PLOT_WINDOW_BACKGROUND).style().fill_color();
          }

          const std::string x_axis_position()
          { // Return the position of the X-axis.
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

          Derived& x_axis_color(const svg_color& col)
          { // Note only stroke color is set.
            derived().image.g(PLOT_X_AXIS).style().stroke_color(col);
            return derived();
          }

          svg_color x_axis_color()
          {
            return derived().image.g(PLOT_X_AXIS).style().stroke_color();
          }

          Derived& y_axis_color(const svg_color& col)
          {
            derived().image.g(PLOT_Y_AXIS).style().stroke_color(col);
            return derived();
          }

          svg_color y_axis_color()
          {
            return derived().image.g(PLOT_Y_AXIS).style().stroke_color();
          }

          Derived& x_label_color(const svg_color& col)
          { // add fill as well PAB Oct 07
            derived().image.g(PLOT_X_LABEL).style().fill_color(col);
            derived().image.g(PLOT_X_LABEL).style().stroke_color(col);
            return derived();
          }

          // Removed until browsers implement better.
          //Derived& x_label_width(double width)
          //{ // width of text is effectively the boldness.
          //  derived().image.g(PLOT_X_LABEL).style().stroke_width(width);
          //  return derived();
          //}

          //double x_label_width()
          //{
          //  return derived().image.g(PLOT_X_LABEL).style().stroke_width();
          //}

          svg_color x_label_color()
          {
            return derived().image.g(PLOT_X_LABEL).style().fill_color();
          }

          Derived& y_label_color(const svg_color& col)
          {
            derived().image.g(PLOT_Y_LABEL).style().fill_color(col);
            derived().image.g(PLOT_Y_LABEL).style().stroke_color(col);
            return derived();
          }

          svg_color y_label_color()
          {
            return derived().image.g(PLOT_Y_LABEL).style().fill_color();
          }

          Derived& x_major_tick_color(const svg_color& col)
          {
            derived().image.g(PLOT_X_MAJOR_TICKS).style().stroke_color(col);
            return derived();
          }

          svg_color x_major_tick_color()
          {
            return derived().image.g(PLOT_X_MAJOR_TICKS).style().stroke_color();
          }

          Derived& x_minor_tick_color(const svg_color& col)
          {
            derived().image.g(PLOT_X_MINOR_TICKS).style().stroke_color(col);
            return derived();
          }

          svg_color x_minor_tick_color()
          {
            return derived().image.g(PLOT_X_MINOR_TICKS).style().stroke_color();
          }

          Derived& x_major_grid_color(const svg_color& col)
          {
            derived().image.g(PLOT_X_MAJOR_GRID).style().stroke_color(col);
            return derived();
          }

          svg_color x_major_grid_color()
          {
            return derived().image.g(PLOT_X_MAJOR_GRID).style().stroke_color();
          }

          Derived& x_major_grid_width(double w)
          {
            derived().image.g(PLOT_X_MAJOR_GRID).style().stroke_width(w);
            return derived();
          }

          double x_major_grid_width()
          {
            return derived().image.g(PLOT_X_MAJOR_GRID).style().stroke_width();
          }

          Derived& x_minor_grid_color(const svg_color& col)
          {
            derived().image.g(PLOT_X_MINOR_GRID).style().stroke_color(col);
            return derived();
          }

          svg_color x_minor_grid_color()
          {
            return derived().image.g(PLOT_X_MINOR_GRID).style().stroke_color();
          }

          Derived& x_minor_grid_width(double w)
          {
            derived().image.g(PLOT_X_MINOR_GRID).style().stroke_width(w);
            return derived();
          }

          double x_minor_grid_width()
          {
            return derived().image.g(PLOT_X_MINOR_GRID).style().stroke_width();
          }

          Derived& x_axis_width(double width)
          {
            derived().image.g(PLOT_X_AXIS).style().stroke_width(width);
            return derived();
          }

          double x_axis_width()
          {
            return derived().image.g(PLOT_X_AXIS).style().stroke_width();
          }

          Derived& data_lines_width(double width)
          {
            derived().image.g(PLOT_DATA_LINES).style().stroke_width(width);
            return derived();
          }

          double data_lines_width()
          {
            return derived().image.g(PLOT_DATA_LINES).style().stroke_width();
          }

          Derived& x_label(const std::string& str)
          {
            derived().x_label_info_.text(str);
            derived().x_axis_.label_on_ = true; // Assume want x_label string displayed.
            // Might switch label_on false if null string?
            return derived();
          }

          std::string x_label()
          {
            return derived().x_label_info_.text();
          }

          Derived& x_label_units(const std::string& str)
          {
            derived().x_units_info_.text(str);
            derived().x_axis_.label_on_ = true; // Assume want x_label string displayed.
            return derived();
          }

          std::string x_label_units()
          {
            return derived().x_units_info_.text();
          }

          // y_label not needed in 1D.
          Derived& y_label(const std::string& str)
          {
            derived().y_label_info_.text(str);
            derived().y_axis_.label_on_ = true; // Assume want y_label string displayed.
            return derived();
          }

          std::string y_label()
          {
            return derived().y_label_info_.text();
          }

          Derived& y_label_units(const std::string& str)
          {
            derived().y_units_info_.text(str);
            derived().y_axis_.label_on_ = true; // Assume want y_label string displayed.
            return derived();
          }

          std::string y_label_units()
          {
            return derived().y_units_info_.text();
          }

          Derived& x_values_on(bool b)
          { // Show values near data points.
            derived().x_values_on_ = b;
            return derived();
          }

          bool x_values_on()
          { // Label data points with X values.
            return derived().x_values_on_;
          }

          Derived& x_values_font_size(unsigned int i)
          {
            derived().x_values_style_.values_text_style_.font_size(i);
            return derived();
          }

          unsigned int x_values_font_size()
          {
            return derived().x_values_style_.values_text_style_.font_size();
          }

          Derived& x_values_font_family(const std::string& family)
          {
            derived().x_values_style_.values_text_style_.font_family(family);
            return derived();
          }

          const std::string& x_values_font_family()
          {
            return derived().x_values_style_.values_text_style_.font_family();
          }

          Derived& x_major_interval(double inter)
          {
            derived().x_ticks_.major_interval_ = inter;
            return derived();
          }

          Derived& x_values_color(const svg_color& col)
          { // Function could set both fill (middle) and stroke (outside),
            // but just setting fill if simplest,
            // but does not allow separate inside & outside colors.
            derived().image.g(PLOT_X_POINT_VALUES).style().fill_color(col);
            //derived().image.g(PLOT_X_POINT_VALUES).style().stroke_color(col);
            return derived();
          }

          svg_color x_values_color()
          { // Function could get either fill and stroke,
            // return derived().image.g(PLOT_X_POINT_VALUES).style().stroke_color();
            return derived().image.g(PLOT_X_POINT_VALUES).style().fill_color();
          }

          Derived& x_values_rotation(rotate_style rotate)
          { // Degrees (0 to 360).
            derived().x_values_style_.value_label_rotation_ = rotate;
            return derived();
          }

          int x_values_rotation()
          {
            return derived().x_values_style_.value_label_rotation_;
          }

          Derived& x_values_precision(int p)
          { // set iostream precision
            derived().x_values_style_.value_precision_ = p;
            return derived();
          }

          int x_values_precision()
          {
            return derived().x_values_style_.value_precision_;
          }

          Derived& x_values_ioflags(std::ios_base::fmtflags f)
          { // set iostream format flags
            derived().x_values_style_.value_ioflags_ = f;
            return derived();
          }

          std::ios_base::fmtflags x_values_ioflags()
          {
            return derived().x_values_style_.value_ioflags_;
          }

          Derived& x_plusminus_on(bool b)
          { // set if uncertainty to be append to X values labels.
            derived().x_values_style_.plusminus_on_ = b;
            return derived();
          }

          bool x_plusminus_on()
          {
            return derived().x_values_style_.plusminus_on_;
          }
          Derived& x_df_on(bool b)
          { // set if uncertainty to be append to X values labels.
            derived().x_values_style_.df_on_ = b;
            return derived();
          }

          bool x_df_on()
          {
            return derived().x_values_style_.df_on_;
          }

          double x_major_interval()
          {
            return derived().x_ticks_.major_interval_;
          }

          Derived& x_major_tick_length(double length)
          {
            derived().x_ticks_.major_tick_length_ = length;
            return derived();
          }

          double x_major_tick_length()
          {
            return derived().x_ticks_.major_tick_length_;
          }

          Derived& x_major_tick_width(double width)
          {
            derived().x_ticks_.major_tick_width_ = width; // Redundant?
            derived().image.g(PLOT_X_MAJOR_TICKS).style().stroke_width(width);
            return derived();
          }

          double x_major_tick_width()
          {
            return derived().image.g(PLOT_X_MAJOR_TICKS).style().stroke_width();
          }

          Derived& x_minor_tick_length(double length)
          {
            derived().x_ticks_.minor_tick_length_ = length;
            return derived();
          }

          double x_minor_tick_length()
          {
            return derived().x_ticks_.minor_tick_length_;
          }

          Derived& x_minor_tick_width(double width)
          {
            derived().x_ticks_.minor_tick_width_ = width;
            derived().image.g(PLOT_X_MINOR_TICKS).style().stroke_width(width);
            return derived();
          }

          double x_minor_tick_width()
          {
            // return derived().x_minor_tick_width_; // should be the same but store in stroke_width is definitive.
            return derived().image.g(PLOT_X_MINOR_TICKS).style().stroke_width();
          }

          Derived& x_major_tick(double d)
          { // Interval (Cartesian units) between major ticks.
            derived().x_ticks_.major_interval_ = d;
          }

          double x_major_tick()
          { // Interval (Cartesian units) between major ticks.
            return derived().x_ticks_.major_interval_;
          }

          Derived& x_minor_interval(double interval)
          { // aka x_minor_tick
            derived().x_ticks_.minor_interval_ = interval;
            return derived();
          }

          Derived& x_num_minor_ticks(unsigned int num)
          {
            derived().x_ticks_.num_minor_ticks_ = num;
            return derived();
          }

          unsigned int x_num_minor_ticks()
          { // NB NOT float or double!
            return derived().x_ticks_.num_minor_ticks_;
          }

          Derived& x_range(double min_x, double max_x)
          {
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
            // done in calculate_plot_window.
            // TODO May be best to combine these?
            derived().x_autoscale_ = false; // because range has just been set.
            return derived();
          }

          std::pair<double, double> x_range()
          { // Need to use boost::svg::detail::operator<< to display this.
            std::pair<double, double> r;
            r.first = derived().x_axis_.min_;
            r.second = derived().x_axis_.max_;
            return r;
          }

          Derived& x_min(double min_x)
          {
            // Not useful to check here that x_max_ > x_min_ because may not have set x_min_ yet.
            derived().x_axis_.min_ = min_x;
            return derived();
          }

          double x_min()
          {
            return derived().x_axis_.min_;
          }

          Derived& x_max(double x)
          {
            // Not useful to check here that x_max_ > x_min_ because may not have set x_min_ yet.
            derived().x_axis_.max_ = x;
            return derived();
          }

          double x_max()
          {
            return derived().x_axis_.max_;
          }

          bool autoscale_check_limits()
          {
            return derived().autoscale_check_limits_;
          }

          Derived& autoscale_check_limits(bool b)
          { // Default is true, but can switch off checks for speed.
            derived().autoscale_check_limits_ = b;
            return derived();
          }

          bool x_autoscale()
          { // Get autoscale setting.
           return derived().x_autoscale_;
          }

          Derived& x_autoscale(bool b)
          { // set whether to use autoscaled values.
             if (b && derived().x_auto_tick_interval_ < 0)
             { // No autoscale values have been calculated, so not safe to make x_autoscale true.
                throw std::runtime_error("X autoscale has not been calculated yet!" );
             }
            derived().x_autoscale_ = b;
            return derived();
          }

          bool autoscale()
          { // AKA x_autoscale.
           return derived().x_autoscale_;
          }

          Derived& autoscale(bool b)
          { // AKA x_autoscale - set whether to use X autoscaled values.

             if (derived().x_auto_tick_interval_ < 0)
             { // No autoscale values have been calculated, so not safe to make x_autoscale true.
                throw std::runtime_error("X-axis autoscale has not been calculated yet!" );
             }
            derived().x_autoscale_ = b;
            return derived();
          }

          Derived& x_autoscale(std::pair<double, double> p)
          { // Use X min & max pair values to autoscale.
              scale_axis(p.first, p.second, // double min and max from pair.
              &derived().x_auto_min_value_, &derived().x_auto_max_value_, &derived().x_auto_tick_interval_, &derived().x_auto_ticks_,
              derived().autoscale_check_limits_,
              derived().x_include_zero_, derived().x_tight_, derived().x_min_ticks_, derived().x_steps_);
            derived().x_autoscale_ = true; // Default to use any calculated values?
            return derived();
          } // autoscale(pair<double, double> p)

          template <class T> // T an STL container: array, vector ...
          Derived& x_autoscale(const T& begin, const T& end) // Data series using iterators
          { // to calculate autoscaled values.
              scale_axis(begin, end,
              &derived().x_auto_min_value_, &derived().x_auto_max_value_, &derived().x_auto_tick_interval_, &derived().x_auto_ticks_,
              derived().autoscale_check_limits_,
              derived().x_include_zero_, derived().x_tight_, derived().x_min_ticks_, derived().x_steps_);

            derived().x_autoscale_ = true; // Default to use calculated values.
            return derived();
          } // x_autoscale(const T& begin, const T& end)

          template <class T> // T an STL container: array, vector ...
          Derived& x_autoscale(const T& container) // Whole data series.
          { // to use to calculate autoscaled values.
              //scale_axis(container.begin(), container.end(), // All the container.
              scale_axis(container, // All the container.
              &derived().x_auto_min_value_, &derived().x_auto_max_value_, &derived().x_auto_tick_interval_, &derived().x_auto_ticks_,
              derived().autoscale_check_limits_,
              derived().x_include_zero_, derived().x_tight_, derived().x_min_ticks_, derived().x_steps_);

            derived().x_autoscale_ = true; // Default to use calculated values.
            return derived();
          } // x_autoscale(const T& container)

          // Set & get autoscale parameters,
          // Note: all these *MUST* preceed x_autoscale(data) call.

          Derived& x_with_zero(bool b)
          { // Set autoscale to include zero (default = false).
            // Must preceed x_autoscale(data) call.
            derived().x_include_zero_ = b;
            return derived();
          }

          bool x_with_zero()
          { //
           return derived().x_include_zero_;
          }

          Derived& x_min_ticks(int min_ticks)
          { // Set autoscale to include at least min_ticks (default = 6).
            // Must preceed x_autoscale(data) call.
            derived().x_min_ticks_ = min_ticks;
            return derived();
          }

          int x_min_ticks()
          { //
           return derived().x_min_ticks_;
          }

          Derived& x_steps(int steps)
          { // Set autoscale to set ticks in steps 2,4,6,8,10, or 1,5,10 or 2,5,10.
            // default = 0 (none)
            // Must preceed x_autoscale(data) call.
            derived().x_steps_ = steps;
            return derived();
          }

          int x_steps()
          { //
           return derived().x_steps_;
          }

          Derived& x_tight(double tight)
          { // Set autoscale to include permit data points slightly outside both end ticks.
            // default 0.
            // Must preceed x_autoscale(data) call.
            derived().x_tight_ = tight;
            return derived();
          }

          double x_tight()
          { //
           return derived().x_tight_;
          }

          // Get results of autoscaling.
          double x_auto_min_value()
          {
           return derived().x_auto_min_value_;
          }

          double x_auto_max_value()
          {
           return derived().x_auto_max_value_;
          }

          double x_auto_tick_interval()
          {
           return derived().x_auto_tick_interval_;
          }

           int x_auto_ticks()
          {
           return derived().x_auto_ticks_;
          }

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

      } // detail
    } // svg
  } // boost

#endif // BOOST_SVG_AXIS_PLOT_FRAME_HPP
