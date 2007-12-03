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

#include <string>
// using std::string;

#include <iostream> // for testing only.
// using std::cerr and std::endl

namespace boost
{
namespace svg
{
namespace detail
{

  template <class Derived>
  class axis_plot_frame
  {
    // axis_plot_frame is used as base class, for example:
    // class svg_1d_plot : public detail::axis_plot_frame<svg_1d_plot>
    // class svg_2d_plot : public detail::axis_plot_frame<svg_2d_plot>
  protected:
    // --------------------------------------------------------------------
    // We don't use the SVG coordinate transform because then text would
    // be flipped. I'm considering using it to scale the image for resizes.
    // --------------------------------------------------------------------

    // protected member functions (defined below):

    // void transform_point(double &x, double &y);
    // void transform_x(double &x);
    // void transform_y(double &y);
    // void draw_x_minor_ticks(double j, path_element& tick_path, path_element& grid_path); // (& grid).
    // void draw_x_major_ticks(double i, path_element& tick_path, path_element& grid_path); // (& grid).
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
      x = derived().x_scale * x + derived().x_shift;
      y = derived().y_scale * y + derived().y_shift;
      adjust_limits(x, y); // In case either hits max, min, infinity or NaN.
    }
    // TODO do we need to adjust_limit(x); // In case hits max, min, infinity or NaN?
    // This implies that user's choice of X-axis range is wrong?
    // So more drastic action like throwing might be least worst action?

    void transform_x(double & x)
    { // Scale and shift x only.
      x = derived().x_scale * x + derived().x_shift;
    }

    void transform_y(double & y)
    { // Scale and shift y only.
      y = derived().y_scale * y + derived().y_shift;
    }

    void draw_x_minor_ticks(double value, path_element& tick_path, path_element& grid_path)
    { // draw X-axis minor ticks, and optional grid.
      // value is NOT (yet) shown beside the minor tick.
      double x1(value);
      transform_x(x1);
      double y1(0.);
      double y2(derived().image.y_size());

      // Draw the minor grid, if wanted.
      if(derived().use_x_minor_grid_)
      {
        if(!derived().use_plot_window)
        {  // Use whole image.
           // Make space for title and X-axis labels.
          if(derived().use_title)
          { // Allow text_margin_ * font_size around text (pixels).
            y1 += derived().title_info.font_size() * derived().text_margin_;
          }
          if(derived().use_x_label)
          {
            y2 -= derived().x_label_info.font_size() * derived().text_margin_;
          }
        }
        else
        { // use_plot_window == true.
          y1 = derived().plot_y1 + 1; // Top. Why +1 and -1?
          y2 = derived().plot_y2 - 1; // Bottom. Ensure *inside* window?
        }
        // Make sure that we are drawing inside the allowed window.
        if((x1 >= derived().plot_x1) && (x1 <= derived().plot_x2)) // allow = too?
        {
          //std::cerr << "Writing draw_x_minor_ticks grid inside plot window: x1 = "
          //  << x1 << ", plot_x1 = " << derived().plot_x1 << ", plot_x2 = " << derived().plot_x2 << std::endl;
          grid_path.M(x1, y1).L(x1, y2);
        }
        else
        { // This will happen but is designed to be ignored!
          // TODO check this is best. See comment in draw_x_axis
           std::cerr << "Writing draw_x_minor_ticks grid OUTside plot window: x1 = "
             << x1 << ", plot_x1 = " << derived().plot_x1 << ", plot_x2 = " << derived().plot_x2 << std::endl;
        }
      } // use_x_minor_grid

      double x_tick_length = derived().x_minor_tick_length_;
      if(derived().use_x_ticks_on_plot_window_)
      { // Put minor ticks on the plot window border.
        y1 = derived().plot_y2; // on the window line.
        y2 = derived().plot_y2; // y1 = upper, y2 = lower end of tick.
        if(derived().use_up_ticks)
        { //
          y1 -= x_tick_length; // up.
        }
        if (derived().use_down_ticks)
        {
          y2 += x_tick_length; // down.
        }
      }
      else
      { // Internal style, draw tick up and/or down from the central X axis line.
        y1 = derived().x_axis; // X-axis line.
        y2 = derived().x_axis;
        if(derived().use_up_ticks)
        {
          y1 -=  x_tick_length; // up
        }
        if (derived().use_down_ticks)
        {
          y2 += x_tick_length; // down.
        }
      }
      // Make sure that we are drawing inside the allowed window.
      if((x1 >= derived().plot_x1) && (x1 <= derived().plot_x2)) // TODO allow < or <=
      {
         //std::cerr << "Writing draw_x_minor_ticks inside plot window: x1 = "
         // << x1 << ", plot_x1 = " << derived().plot_x1 << ", plot_x2 = " << derived().plot_x2 << std::endl;
        tick_path.M(x1, y1).L(x1, y2);
        // No value labels on minor ticks, at present.
      }
      else
      { // This will happen but is designed to be ignored!
        //  TODO check this is best. See comment in draw_x_axis
         //std::cerr << "Writing draw_x_minor_ticks OUTside plot window: x1 = "
         // << x1 << ", plot_x1 = " << derived().plot_x1 << ", plot_x2 = " << derived().plot_x2 << std::endl;
      }
    } // void draw_x_minor_ticks

  void draw_x_major_ticks(double value, path_element& tick_path, path_element& grid_path)
  { // draw ticks - and grid too if wanted.
    // If use_x_major_labels then value may be shown beside the major tick.
    double x1(value); //
    transform_x(x1);
    double y1(0.); // // y1 = upper,
    double y2(derived().image.x_size()); // y2 = lower end of tick.
    if(derived().use_x_major_grid_)
    { // Draw major grid vertical line.
      if(!derived().use_plot_window)
      { // Allow a modest margin around text of title and X-axis labels, if in use.
        if(derived().use_title)
        {
          y1 += derived().title_info.font_size() * derived().text_margin_;
        }
        if(derived().use_x_label)
        { // If use_x_major_labels then value may be shown beside the major tick.
          y2 -= derived().x_label_info.font_size() * derived().text_margin_;
        }
      }
      else
      { // use_plot_window == true
        y1 = derived().plot_y1; // Bottom of plot window.
        y2 = derived().plot_y2; // Top of plot window.
      }
      grid_path.M(x1, y1).L(x1, y2); // Vertical grid line.
    } // use_x_major_grid

    // Draw major tick (perhaps as well as grid - ticks might be wider than grid).
    // Make sure that we are drawing inside the allowed plot window.
    if((x1 >= derived().plot_x1) && (x1 <= derived().plot_x2)) // now <= 
    {
      double x_tick_length = derived().x_major_tick_length_;
      if(derived().use_x_ticks_on_plot_window_)
      { // Put the ticks on the plot window border (was external).
        y1 = derived().plot_y2; // on the window line.
        y2 = derived().plot_y2; // y1 = upper, y2 = lower.
        if(derived().use_up_ticks)
        {
          y1 -= x_tick_length; // up
        }
        if (derived().use_down_ticks)
        {
          y2 += x_tick_length; // down.
        }
      }
      else
      { // Internal_style, draw tick from the central X axis line.
        y1 = derived().x_axis; // X-axis line.
        y2 = derived().x_axis;
        if(derived().use_up_ticks)
        {
          y1 -=  x_tick_length; // up
        }
        if (derived().use_down_ticks)
        {
          y2 += x_tick_length; // down.
        }
      }
      tick_path.M(x1, y1).L(x1, y2);
      // Leaving current position at the bottom end of the tick.

      if(derived().use_x_major_labels)
      { // Show value by the tick as "1.2" or "3.4e+000"...
        std::stringstream label;
        label.precision(derived().x_value_precision_);
        label.flags(derived().x_value_ioflags_);
        label << value; // "1.2" or "3.4e+000"...
        double y = y2; // bottom end of the tick.
        align_style alignment = center_align;
        if(derived().use_down_ticks)
        {  // No need to shift if derived().use_up_ticks as labels are below the X-axis.
          y += derived().x_label_info.font_size();
          // Move down a font height below bottom end of tick.
        }
        if (derived().x_label_rotation_ == upward)
        { // 
          alignment = right_align;
          //y += label.str().size() * derived().x_label_info.font_size();
          // so the last digit will be by the tick.
        }
        else if((derived().x_label_rotation_ == downward)
          || (derived().x_label_rotation_ == downhill))
        { // start from tick and write down.
          alignment = left_align;
        }
        else if(derived().x_label_rotation_ == horizontal)
        {
          alignment = center_align; // center on the tick.
        }
        else if(derived().x_label_rotation_ == uphill)
        { // 45 slope up,
          alignment = left_align; // Uphill to end at tick.
          y += label.str().size() * derived().x_label_info.font_size() * 0.7;
          // sloping so need about sin(45) = 0.707 less space,
          // so the last digit is by the tick.
        }
        else
        { // 45 slope down
          alignment = left_align; // Assume downhill from tick,
          // so no need to y adjustment.
        }

        //if(derived().use_x_ticks_on_plot_window_) // External style.
        //{ // Always want all values including "0" if labelling on the external plot window.
        //  derived().image.get_g_element(detail::PLOT_VALUE_LABELS).text(
        //    x1, y, // x centered on tick, 
        //    label.str(), derived().x_label_info.font_size(), derived().x_label_info.font_family(),
        //    "", "", "", "", alignment,  // center label on the tick.
        //    derived().x_label_rotation_);
        //}
        //else
        { // ! use_x_ticks_on_plot_window_ = Internal - value labels just below horizontal X-axis.
          if (derived().use_x_ticks_on_plot_window_ || ((value != 0) && derived().use_x_axis_lines_))
          { // Avoid a "0" below the X-axis if it would be cut through by any internal vertical Y-axis line.
           derived().image.get_g_element(detail::PLOT_VALUE_LABELS).text(
              x1, y,
              label.str(), derived().x_label_info.font_size(), derived().x_label_info.font_family(),
              "", "", "", "", alignment, // center label on the tick.
              derived().x_label_rotation_);
          }
        }
      } // use_x_major_labels
    }
    else
    { // Outside plot window - so do nothing?  Warning?
      //std::cerr << "Writing draw_x_major_ticks OUTside plot window: "
      //  "x1 = " << x1 << ", plot_x1 = " << derived().plot_x1 << ", plot_x2 = " << derived().plot_x2 << std::endl;
    }
  } // draw_x_major_ticks

  void draw_x_axis()
  {
    double y1(0.); // Draw the horizontal X-axis line at y = 0.
    transform_y(y1);
    derived().x_axis = y1;
    // Access the paths for the ticks & grids, ready for additions.
    path_element& minor_tick_path = derived().image.get_g_element(PLOT_X_MINOR_TICKS).path();
    path_element& major_tick_path = derived().image.get_g_element(PLOT_X_MAJOR_TICKS).path();
    path_element& minor_grid_path = derived().image.get_g_element(PLOT_X_MINOR_GRID).path();
    path_element& major_grid_path = derived().image.get_g_element(PLOT_X_MAJOR_GRID).path();

    if(derived().use_x_axis_lines_)
    { // Draw the horizontal X-axis line the full width of the plot window.
      derived().image.get_g_element(PLOT_X_AXIS).line(
        derived().plot_x1, derived().x_axis, // y = 0
        derived().plot_x2, derived().x_axis); // y = 0
    }

    // x_minor_jump is the interval between minor ticks.
    double x_minor_jump = derived().x_major_interval_ /
      (derived().x_num_minor_ticks_ + 1.);

    // TODO Problem here when using floating point??
    // Was i < y_max; but didn't show the tick and value at y_max so now i <= y_max;
    // But may still fail if a least significant bit or few out??

    // Draw the ticks on the positive side (right of zero).
    for(double x = 0.; x <= derived().x_max; x += derived().x_major_interval_)
    {
      for(double j = x + x_minor_jump; j < x + derived().x_major_interval_; j += x_minor_jump)
      { // This will output 'orphaned' minor ticks that are beyond the plot window,
        // if the last major tick does not coincide with the plot window.
        // These are just ignored in draw_x_minor_ticks.
        // There might be 9 of them,
        // if you have the common 9 minor tick between major ticks!
        // TODO this seems ugly - as does the negative ones below.
        draw_x_minor_ticks(j, minor_tick_path, minor_grid_path);
      }
      draw_x_major_ticks(x, major_tick_path, major_grid_path);
    }

    // Draw the ticks on the negative side (left of zero).
    for(double x = 0; x >= derived().x_min; x -= derived().x_major_interval_)
    {
      // Draw minor ticks.
      for(double j = x;
        j > x-derived().x_major_interval_;
        j-= derived().x_major_interval_ / (derived().x_num_minor_ticks_+1))
      {
        draw_x_minor_ticks(j, minor_tick_path, minor_grid_path);
      }
      draw_x_major_ticks(x, major_tick_path, major_grid_path);
    }
  } // void draw_x_axis()

  // --------------------------------------------------------------------
  // Draw functions - legend & title (if any).
  // --------------------------------------------------------------------

  void draw_title()
  {
  // text_element::text_element(double x, double y,
  //  const std::string&,
  //  int size,
  //  const std::string& font,
  //  const std::string& style, const std::string& weight,
  //  const std::string& stretch, const std::string& decoration,
  //  int align, int rotate);

    // Update title_info with position.
    derived().title_info.x(derived().image.x_size() / 2.); // Center of image.
    // Assumes align = center_align.
    double y;
    if (derived().use_plot_window)
    {
       y = derived().plot_y1; // plot_y1 IS now assigned in calculate_plot_window
    // Center the title in the space between top and plot window top.
       y /= 2;
    }
    else
    { // use all image.
      y = derived().title_info.font_size() * derived().text_margin_; // Leave a linespace above.
    }
    derived().title_info.y(y);
    derived().image.get_g_element(PLOT_TITLE).push_back(new text_element(derived().title_info));
  } // void draw_title()

  void draw_legend()
  {
    size_t num_points = derived().series.size();
    int font_size = derived().legend_header_.font_size();
    int point_size =  derived().series[0].point_style.size;
    // Use whichever is the biggest of point marker and font.

    double spacing = (std::max)(font_size, point_size);
    // std::cerr << spacing <<  ' ' << font_size << ' ' << point_size << endl;
    bool is_header = (derived().legend_header_.text() != "");
    size_t longest = 0;
    for(unsigned int i = 0; i < derived().series.size(); ++i)
    { // Find the longest text in all the data series.
      std::string s = derived().series[i].title;
      size_t siz = s.size();
      if (siz > longest)
      {
        longest = siz;
      }
    }
    double legend_width = (6 + longest /2) * spacing;
    // font_size is not exact because width varies.
    // Allow for a leading space, data marker symbol, space,
    // line in color (only if 2-D and line option selected TODO)
    // space, text, trailing space before box margin.

    // legend_height must be *at least* enough for
    // the legend title and text_margin_s around it
    // (if any) plus a text_margin_ top and bottom.
    // Add more height depending on the number of lines of text.
    double legend_height = spacing;
    if ( (derived().use_title) // plot title
       && (derived().legend_header_.text() != "")) // & really is a legend title.
    {
      legend_height += 2 * spacing;
    }
    legend_height += num_points * spacing * 2; // Space for the point symbols & text.

    // x, y position of legend 'box' top left, level with top right of plot window.
    double legend_x_start(derived().plot_x2);
    legend_x_start += spacing; // leave a space between plot window and legend box.
    double legend_y_start(derived().plot_y1);

    int x_size = derived().image.x_size();
    if((legend_x_start + legend_width) > x_size)
    { // Use nearly all the image width available.
      std::cout << "Legend text line was too long by "
       << ((legend_x_start + legend_width) - x_size)
       << " pixels,  & so truncated. legend_width == " << legend_width << std::endl;
      // For example:
      // "Legend text line was too long by about 84 pixels & so truncated. legend_width == 252"
      legend_width = x_size - legend_x_start - spacing; // derived().text_margin_;
      // text_margin_ just allows the border box to show.
    }

    // Draw border box round legend.
    g_element* g_ptr = &(derived().image.get_g_element(PLOT_LEGEND_BACKGROUND));
    g_ptr->push_back(new
      rect_element(legend_x_start, legend_y_start, legend_width, legend_height));
    derived().legend_x1_ = legend_x_start; // Save for access by legend_to_left.
    derived().legend_x2_ = legend_width;
    derived().legend_y1_ = legend_y_start; // and legend_bottom_right.
    derived().legend_y2_ = legend_height;

    double legend_y_pos = legend_y_start + derived().text_margin_ * spacing;
    if (is_header)
    { // Draw the legend text.
      derived().legend_header_.x(legend_x_start + legend_width / 2.); // / 2. to center in legend box.
      derived().legend_header_.y(legend_y_pos);
      derived().image.get_g_element(PLOT_LEGEND_TEXT).push_back(new text_element(derived().legend_header_));
      legend_y_pos += 2 * spacing;
    }

    g_ptr = &(derived().image.get_g_element(PLOT_LEGEND_POINTS));
    g_element* g_inner_ptr = g_ptr;
    g_inner_ptr = &(derived().image.get_g_element(PLOT_LEGEND_TEXT));

    for(unsigned int i = 0; i < derived().series.size(); ++i)
    { // Show point marker, text info and perhaps line for all the data series.
      double legend_x_pos = legend_x_start;
      g_inner_ptr = &(g_ptr->add_g_element());
      // Use both fill & stroke colors from the point's style.
      g_inner_ptr->style()
        .fill_color(derived().series[i].point_style.fill_color)
        .stroke_color(derived().series[i].point_style.stroke_color);
      if(derived().series[i].point_style.shape != none)
      {
        draw_plot_point( // Plot point like circle, square...
          legend_x_start + spacing, // space before point marker.
          legend_y_pos,
          *g_inner_ptr,
          derived().series[i].point_style);
        legend_x_pos += 2 * spacing;
      }

      // Line markers  - only really applicable to 2-D sets plot_line_style,
      if (derived().use_line)
      { // Need to draw a short line to show color for that data series.
        g_inner_ptr->style() // Use fill & stroke colors from line style.
          .stroke_color(derived().series[i].line_style.color);
        g_inner_ptr->push_back(new line_element(
          legend_x_pos + spacing /2., // half space leading space
          legend_y_pos,
          legend_x_pos + spacing * 1.5, // line sample is one char long.
          legend_y_pos));
        legend_x_pos += 2. * spacing; // short line & space.
      } // use_line

      // Legend text for each Data Series added to the plot.
      g_inner_ptr = &(derived().image.get_g_element(PLOT_LEGEND_TEXT));
      g_inner_ptr->push_back(new text_element(
        legend_x_pos, // allow space for the marker.
        legend_y_pos,
        derived().series[i].title, // Text for this data series.
        derived().legend_header_.font_size(), // font size &
        derived().legend_header_.font_family(), // font family.
        "", "", "", "",
        left_align));
      legend_y_pos += 2 * spacing;
    } // for

    // TODO reconsider this.
    //if(derived().plot_x2 >= (int)derived().image.x_size())
    //{ // Put legend above plot because image is tall & thin.
    //  // TODO this with use control of the legend box position.
    //}
  } // void draw_legend()

  void draw_x_label()
  {
    // color is set in constructor.
    //image.get_g_element(detail::PLOT_X_LABEL).style().stroke_color(black);
    // and using y_label_color(color)

    std::string label = derived().x_label_info.text(); // x_axis label, and optional units.
    if (derived().use_x_label_units && (derived().x_units_info.text() != ""))
    { // Append the units, if any, providing brackets ().
      label += "  (" + derived().x_units_info.text() + ")";
    }
    // Simplest to start from the bottom of the image.
    // and move up to give enough sapce for the X-axis label.

    //double y(0.);
    //transform_y(y);
    //derived().x_axis = y; // X-axis line at y = 0.
    //y += derived().x_label_font_size() * 2.5; // values & label & half line space.
    //if (derived().use_down_ticks)
    //{ // Make space for ticks down.
    //  y += (std::max)(derived().x_major_tick_length(), derived().x_minor_tick_length());
    //  // Brackets avoid trouble with any nasty macro max.
    //  // Use max in case some joker uses longer minor ticks than major.
    //}
    //if (derived().x_ticks_on_plot_window_on())
    //{ // Allow space for the values labelling the ticks.
    //  y += derived().x_label_font_size() * derived().text_margin_;
    //}
    // Bottom of plot window plus two char height.
    // derived().plot_y2 + (derived().x_label_font_size() * derived().text_margin_),

    double y = derived().image.y_size();  // bottom edge of image.
    y -= derived().x_label_font_size(); // Up enough for a space underneath label.
    derived().image.get_g_element(PLOT_X_LABEL).push_back(new text_element(
      ( // x position relative to the x-axis which is middle of plot window.
				derived().plot_x2 + derived().plot_x1) / 2,  // x coordinate - middle.
      y, // Up from image bottom edge.
      label,
      derived().x_label_font_size(),
      derived().x_label_font_family(), "", "", "", "", center_align, horizontal)
    );
  } // void draw_x_label()

  // TODO split into adjust_limit(double& x) and call twice?,
  // and TODO use to check 1-D for limit values too?
  void adjust_limits(double& x, double& y)
  { // If value reaches limit of max, min, infinity,
    // use the appropriate plot min or max.
    if(detail::limit_max(x))
    {
      x = derived().plot_x2;
    }
    if(detail::limit_max(y))
    {
      y = derived().plot_y1;
    }
    if(detail::limit_min(x))
    {
      x = derived().plot_x1;
    }
    if(detail::limit_min(y))
    {
      y = derived().plot_y1;
    }
    // If value is NaN, use zero instead.
    // TODO Do we want/get a different color or shape for NaNs??
    if(detail::limit_NaN(x))
    {
      x = 0;
      transform_x(x);
    }
    if(detail::limit_NaN(y))
    {
      y = 0;
      transform_y(y);
    }
  } // void adjust_limits

  void draw_plot_point(double x, double y,
    g_element& g_ptr,
    const plot_point_style& sty)
  {
    int size = sty.size;
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

    switch(sty.shape) // from enum point_shape none, round, square, point, egg
    {
    case round:
      g_ptr.circle(x, y, half_size);
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
      g_ptr.text(x, y + half_size, sty.symbols, size, "Lucida Sans Unicode"); // symbol(s), size and centre.
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
      g_ptr.text(x, y, "&#x2666;", size, "Lucida Sans Unicode");
      // size / 4. puts bottom tip on the X-axis,
      // size / 2. put center above the X-axis
      // x, y, put on the X-axis - probably what is needed for 2-D plots.
      // diamond, spades, clubs & hearts fill with expected fill_color.
      break;
    case asterisk:
      g_ptr.text(x, y - size / 3., "&#x2217;", size, "Lucida Sans Unicode");
      // asterisk is black filled.
      // size /3 puts the bottom tip on the X-axis.
      break;
    case lozenge:
      g_ptr.text(x, y - size / 3., "&#x25CA;", size, "Lucida Sans Unicode");
      // size / 3 to get tip of lozenge just on the X-axis.
      // lozenge seems not to fill?
      break;
    case club:
      g_ptr.text(x, y, "&#x2663;", size, "Lucida Sans Unicode");
      // x, y, puts club just on the X-axis
      break;
    case spade:
      g_ptr.text(x, y, "&#x2660;", size, "Lucida Sans Unicode");
      //
      break;
    case heart:
      g_ptr.text(x, y , "&#x2665;", size, "Lucida Sans Unicode");
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
    derived().image.get_g_element(PLOT_BACKGROUND).clear();
  }

  void clear_title()
  {
    derived().image.get_g_element(PLOT_TITLE).clear();
  }

  void clear_points()
  {
    derived().image.get_g_element(PLOT_DATA_POINTS).clear();
  }

  void clear_plot_background()
  {
    derived().image.get_g_element(PLOT_WINDOW_BACKGROUND).clear();
  }

  void clear_legend()
  {
    derived().image.get_g_element(PLOT_LEGEND_BACKGROUND).clear();
    derived().image.get_g_element(PLOT_LEGEND_POINTS).clear();
    derived().image.get_g_element(PLOT_LEGEND_TEXT).clear();
  }

  void clear_x_axis()
  {
    derived().image.get_g_element(PLOT_X_AXIS).clear();
    derived().image.get_g_element(PLOT_X_MINOR_TICKS).clear();
    derived().image.get_g_element(PLOT_X_MAJOR_TICKS).clear();
    derived().image.get_g_element(PLOT_X_LABEL).clear();
    derived().image.get_g_element(PLOT_VALUE_LABELS).clear();
  }

  void clear_y_axis()
  {
    derived().image.get_g_element(PLOT_Y_AXIS).clear();
  }

  void clear_grids()
  {
    derived().image.get_g_element(PLOT_X_MAJOR_GRID).clear();
    derived().image.get_g_element(PLOT_X_MINOR_GRID).clear();
    // TODO don't we need to clear Y grids too??????
    derived().image.get_g_element(PLOT_Y_MAJOR_GRID).clear();
    derived().image.get_g_element(PLOT_Y_MINOR_GRID).clear();
  }

private:
  Derived& derived()
  {
    return static_cast<Derived&>(*this); // Why is cast required?
    //  error C2440: 'return' : cannot convert from 'boost::svg::detail::axis_plot_frame<Derived>' to 'boost::svg::svg_1d_plot &'
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
  // bool x_major_labels_on()
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
//Derived& x_major_labels_on(bool cmd)
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
//Derived& background_border_color(const svg_color& col)
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
    // Might also alow to set x and y separately?
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
    return derived().image.get_g_element(PLOT_BACKGROUND).style().fill_color();
  }

  Derived& background_border_color(const svg_color& col)
  {
    derived().image.get_g_element(PLOT_BACKGROUND).style().stroke_color(col);
    return derived();
  }

  svg_color background_border_color()
  {
    return derived().image.get_g_element(PLOT_BACKGROUND).style().stroke_color();
  }

  Derived& background_border_width(double w)
  {
    derived().image.get_g_element(PLOT_BACKGROUND).style().stroke_width(w);
    return derived();
  }

  double background_border_width()
  {
    return derived().image.get_g_element(PLOT_BACKGROUND).style().stroke_width();
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
    std::string commercial = "permits")
  { // Might check these are "permits", "requires", or "prohibits"?
    derived().image.license(repro, distrib, attrib, commercial);
    return derived();
  }

  const bool license_on()
  {
    return derived().image.is_license();
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
    derived().x_value_precision_ = digits;
    return derived();
  }

  int x_value_precision()
  { //
    return derived().x_value_precision_;
  }

  const std::string title()
  {
    return derived().title_info.text();
  }

  Derived& title_font_size(unsigned int i)
  {
    derived().title_info.font_size(i);
    return derived();
  }

  unsigned int title_font_size()
  {
    return derived().title_info.font_size();
  }

  Derived& title_font_family(const std::string& family)
  {
    derived().title_info.font_family(family);
    return derived();
  }

  const std::string& title_font_family()
  {
    return derived().title_info.font_family();
  }

  Derived& title_font_style(const std::string& style)
  {
    derived().title_info.font_style(style);
    return derived();
  }

  const std::string& title_font_style()
  {
    return derived().title_info.font_style();
  }

  Derived& title_font_weight(const std::string& weight)
  {
    derived().title_info.font_weight(weight);
    return derived();
  }

  const std::string& title_font_weight()
  {
    return derived().title_info.font_weight();
  }

  Derived& title_font_stretch(const std::string& stretch)
  {
    derived().title_info.font_stretch(stretch);
    return derived();
  }

  const std::string& title_font_stretch()
  {
    return derived().title_info.font_stretch();
  }

  Derived& title_font_decoration(const std::string& decoration)
  {
    derived().title_info.font_decoration(decoration);
    return derived();
  }

  const std::string& title_font_decoration()
  {
    return derived().title_info.font_decoration();
  }

  Derived& title_font_rotation(int rotate)
  { // Degrees (0 to 360).
    derived().title_info.font_rotation(rotate);
    return derived();
  }

  int title_font_rotation()
  {
    return derived().title_info.font_rotation();
  }

  Derived& title_font_alignment(align_style alignment)
  {
    derived().title_info.font_alignment(alignment);
    return derived();
  }

  align_style title_font_alignment()
  {
    return derived().title_info.font_alignment();
  }

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

  Derived& legend_font_family(const std::string& family)
  {
    derived().legend_header_.font_family(family);
    return derived();
  }

  const std::string& legend_font_family()
  {
    return derived().legend_header_.font_family();
  }

  Derived& legend_title_font_size(unsigned int size)
  {
    derived().legend_header_.font_size(size);
    return derived();
  }

  unsigned int legend_title_font_size()
  {
    return derived().legend_header_.font_size();
  }

  Derived& legend_top_left(double x, double y)
  { // Position of top left of legend box (svg coordinates).
    // Bottom right is controlled by contents, so cannot set it.
    if((x < 0) || (x > derived().image.x_size()) || (y < 0) || (y > derived().image.y_size()))
    {
      throw std::runtime_error("Illegal legend box position.");
    }
    derived().legend_x1_ = x;
    derived().legend_y1_ = y;
    return derived();
  }

  std::pair<double, double> legend_top_left()
  {// Top left of legend box
    std::pair<double, double> r;
    r.first = derived().legend_x1_; 
    r.second = derived().legend_y1_;
    return r;
  }

  std::pair<double, double> legend_bottom_right()
  {// Top left of legend box
    std::pair<double, double> r;
    r.first = derived().legend_x2_; 
    r.second = derived().legend_y2_;
    return r;
  }

  Derived& line_on(bool is)
  {
    derived().use_line = is;
    return derived();
  }

  bool line_on()
  {
    return derived().use_line;
  }

  Derived& legend_on(bool cmd)
  {
    derived().use_legend = cmd;
    if(cmd)
    {
      derived().image.get_g_element(detail::PLOT_LEGEND_BACKGROUND)
        .style().fill_color(white)
        .stroke_color(black);
    }
    return derived();
  }

  bool legend_on()
  {
    return derived().use_legend;
  }

  Derived& plot_window_on(bool cmd)
  {
    derived().use_plot_window = cmd;

    if(cmd)
    { // set plot window color and border color.
      // TODO - allow user to change these.
      derived().image.get_g_element(detail::PLOT_WINDOW_BACKGROUND)
        .style().fill_color(white)
        .stroke_color(black);
    }
    return derived();
  }

  bool plot_window_on()
  {
    return derived().use_plot_window;
  }

  Derived& plot_border_color(const svg_color& col)
  {
    derived().image.get_g_element(detail::PLOT_WINDOW_BACKGROUND).style().stroke_color(col);
    return derived();
  }

  svg_color plot_border_color()
  { 
    return derived().image.get_g_element(detail::PLOT_WINDOW_BACKGROUND).style().stroke_color();
  }

  double plot_border_width()
  {
      return derived().image.get_g_element(detail::PLOT_WINDOW_BACKGROUND).style().stroke_width();
  }

  Derived& plot_border_width(double w)
  {
    derived().image.get_g_element(detail::PLOT_WINDOW_BACKGROUND).style().stroke_width(w);
    return derived();
  }

  Derived& border_margin(double w)
  {
    derived().border_margin_ = w;
    return derived();
  }

  double border_margin()
  {
    return derived().border_margin_;
  }

  Derived& border_width(double w)
  {
    derived().border_width_ = w;
    return derived();
  }

  double border_width()
  {
    return derived().border_width_;
  }

  Derived& plot_window_x(double min_x, double max_x)
  { // This is normally calculated from other plot values.
    if(max_x <= min_x)
    {
      throw std::runtime_error("plot_window_x: X range: x_max < x_min");
    }
    derived().plot_x1 = min_x;
    derived().plot_x2 = max_x;
    return derived();
  }

  Derived& plot_window_y(double min_y, double max_y)
  { // This is normally calculated from other plot values.
    if(max_y <= min_y)
    {
      throw std::runtime_error("plot_window_y : Y range: y_max < y_min");
    }
    derived().plot_y1 = min_y;
    derived().plot_y2 = max_y;
    return derived();
  }

  std::pair<double, double> plot_window_x()
  {
    std::pair<double, double> r;
    r.first = derived().plot_x1;
    r.second = derived().plot_x2;
    return r;
  }

  double plot_window_x_left()
  {
    return derived().plot_x1;
  }
  double plot_window_x_right()
  {
    return derived().plot_x2;
  }
  double plot_window_y_top()
  {
    return derived().plot_y1;
  }
  double plot_window_y_bottom()
  {
    return derived().plot_y2;
  }

  std::pair<double, double> plot_window_y()
  {
    std::pair<double, double> r;
    r.first = derived().plot_y1;
    r.second = derived().plot_y2;
    return r;
  }

  double x_minor_interval()
  {
    return derived().x_minor_interval_;  // interval
  }

  double y_minor_interval()
  {
    return derived().y_minor_interval_;  // interval
  }

  Derived& x_ticks_up_on(bool cmd)
  {
    derived().use_up_ticks = cmd;
    return derived();
  }

  bool x_ticks_up_on()
  {
    return derived().use_up_ticks;
  }

  Derived& x_ticks_down_on(bool cmd)
  {
    derived().use_down_ticks = cmd;
    return derived();
  }

  bool x_ticks_down_on()
  {
    return derived().use_down_ticks;
  }
  // Only need y_ticks_left_on & y_ticks_right_on in 2D

  Derived& x_label_on(bool cmd)
  { // Show X-axis label text, or not.
    derived().use_x_label = cmd;
    return derived();
  }

  bool x_label_on()
  {
    return derived().use_x_label;
  }

  Derived& x_label_font_size(unsigned int i)
  { // TODO May be best to tie these two font sizes together?
    derived().x_label_info.font_size(i);
    derived().x_units_info.font_size(i);
    return derived();
  }

  unsigned int x_label_font_size()
  {
    return derived().x_label_info.font_size();
  }

  Derived& x_label_font_family(const std::string& family)
  {
    derived().x_label_info.font_family(family);
    return derived();
  }

  const std::string& x_label_font_family()
  {
    return derived().x_label_info.font_family();
  }

  Derived& x_value_ioflags(int flags)
  { // IO flags of X tick label values (default 0X201).
    derived().x_value_ioflags_ = flags;
    return derived();
  }

  int x_value_ioflags()
  { // ALL stream ioflags for control of format of X value labels.
    return derived().x_value_ioflags_;
  }

  Derived& title(const std::string title)
  { // Plot title.
    derived().title_info.text(title);
    return derived();
  }


  Derived& x_axis_label_color(const svg_color& col)
  { // Set BOTH stroke and fill to the same color.
    derived().image.get_g_element(detail::PLOT_X_LABEL).style().fill_color(col);
    derived().image.get_g_element(detail::PLOT_X_LABEL).style().stroke_color(col);
    return *this;
  }

  svg_color x_axis_label_color()
  { // But only return the stroke color.
    return derived().image.get_g_element(detail::PLOT_X_LABEL).style().stroke_color();
  }

  Derived& x_axis_value_color(const svg_color& col)
  { // Set BOTH stroke and fill to the same color.
    derived().image.get_g_element(detail::PLOT_VALUE_LABELS).style().fill_color(col);
    derived().image.get_g_element(detail::PLOT_VALUE_LABELS).style().stroke_color(col);
    return *this;
  }

  svg_color x_axis_value_color()
  { // But only return the stroke color.
    return derived().image.get_g_element(detail::PLOT_VALUE_LABELS).style().stroke_color();
  }

  Derived& x_ticks_on_plot_window_on(bool cmd)
  { // External style.
    derived().use_x_ticks_on_plot_window_ = cmd;
    return derived();
  }

  bool x_ticks_on_plot_window_on()
  { // External style = true.
    return derived().use_x_ticks_on_plot_window_;
  }

  Derived& x_label_units_on(bool cmd)
  {
    derived().use_x_label_units = cmd;
    return derived();
  }

  bool x_label_units_on()
  {
    return derived().use_x_label_units;
  }

  Derived& x_major_labels_on(bool cmd)
  {
    derived().use_x_major_labels = cmd;
    return derived();
  }

  bool x_major_labels_on()
  {
    return derived().use_x_major_labels;
  }

  Derived& x_major_label_rotation(int rot)
  {
    derived().x_label_rotation_ = rot;
    return derived();
  }

  int x_major_label_rotation()
  {
    return derived().x_label_rotation_;
  }

  Derived& title_on(bool cmd)
  {
    derived().use_title = cmd;
    return derived();
  }

  bool title_on()
  {
    return derived().use_title;
  }

  Derived& x_major_grid_on(bool is)
  {
    derived().use_x_major_grid_ = is;
    return derived();
  }

  bool x_major_grid_on()
  {
    return derived().use_x_major_grid_;
  }

  Derived& x_minor_grid_on(bool is)
  {
    derived().use_x_minor_grid_ = is;
    return derived();
  }

  bool x_minor_grid_on()
  {
    return derived().use_x_minor_grid_;
  }

  Derived& axes_on(bool is)
  { // Draw *both* x and y axes (note plural).
    derived().use_x_axis_lines_ = is;
    derived().use_y_axis_lines_ = is;
    return derived();
  }

  bool axes_on()
  { // Used X in preference to Y for 1D, but now require *both* x and y axis on.
    return derived().use_x_axis_lines_ && derived().use_y_axis_lines_;
  }

  Derived& x_axis_on(bool is)
  { // Draw a horizontal x_axis line.
    derived().use_x_axis_lines_ = is;
    return derived();
  }

  bool x_axis_on()
  { // Use X in preference to Y for 1D
    return derived().use_x_axis_lines_;
  }

  Derived& y_axis_on(bool is)
  {// Draw a vertical y_axis line.
    derived().use_y_axis_lines_ = is;
    return derived();
  }

  bool y_axis_on()
  { // Should be always false for 1D.
    return derived().use_y_axis_lines_;
  }

  // enums like PLOT_TITLE provide a std:string like "title"
  // colors .stroke_color, .stroke_width and font are set in the appropriate g_element.

  Derived& title_color(const svg_color& col)
  {
    derived().image.get_g_element(PLOT_TITLE).style().stroke_color(col);
    derived().image.get_g_element(PLOT_TITLE).style().fill_color(col);
    return derived();
  }

  svg_color title_color()
  { // Function title_color sets both fill and stroke,
    // but stroke (outside) is considered 'more important'.
    return derived().image.get_g_element(PLOT_TITLE).style().stroke_color();
  }

  Derived& title_font_width(double width)
  { // width of text is effectively the boldness
    derived().image.get_g_element(PLOT_TITLE).style().stroke_width(width); 
    return derived();
  }

  double title_font_width()
  {
    return derived().image.get_g_element(PLOT_TITLE).style().stroke_width();
  }

  Derived& legend_color(const svg_color& col)
  {
    derived().image.get_g_element(PLOT_LEGEND_TEXT).style().fill_color(col);
    derived().image.get_g_element(PLOT_LEGEND_TEXT).style().stroke_color(col);
    return derived();
  }

  svg_color legend_color()
  { // Function legend_color sets both fill and stroke,
    // but stroke (outside) is considered 'more important'.
    return derived().image.get_g_element(PLOT_LEGEND_TEXT).style().stroke_color();
  }

  Derived& legend_font_width(double width)
  { // width of text is effectively the boldness.
    derived().image.get_g_element(PLOT_LEGEND_TEXT).style().stroke_width(width);
    return derived();
  }

  double legend_font_width()
  {
    return derived().image.get_g_element(PLOT_LEGEND_TEXT).style().stroke_width();
  }

  Derived& background_color(const svg_color& col)
  {
    derived().image.get_g_element(PLOT_BACKGROUND).style().fill_color(col);
    return derived();
  }

  Derived& legend_background_color(const svg_color& col)
  {
    derived().image.get_g_element(PLOT_LEGEND_BACKGROUND).style().fill_color(col);
    return derived();
  }

  svg_color legend_background_color()
  {
    return derived().image.get_g_element(PLOT_LEGEND_BACKGROUND).style().fill_color();
  }

  Derived& legend_border_color(const svg_color& col)
  {
    derived().image.get_g_element(PLOT_LEGEND_BACKGROUND).style().stroke_color(col);
    return derived();
  }

  svg_color legend_border_color()
  {
    return derived().image.get_g_element(PLOT_LEGEND_BACKGROUND).style().stroke_color();
  }

  Derived& plot_background_color(const svg_color& col)
  {
    derived().image.get_g_element(PLOT_WINDOW_BACKGROUND).style().fill_color(col);
    return derived();
  }

  svg_color plot_background_color()
  {
    return derived().image.get_g_element(PLOT_WINDOW_BACKGROUND).style().fill_color();
  }

  Derived& x_axis_color(const svg_color& col)
  { // Note BOTH fill and stroke color are set (and are the same).
    // stroke is the outside of any character, fill the center color.
    // TODO But the axis is only a line, so set both for tidyness?
    derived().image.get_g_element(PLOT_X_AXIS).style().fill_color(col);
    derived().image.get_g_element(PLOT_X_AXIS).style().stroke_color(col);
    return derived();
  }

  svg_color x_axis_color()
  { // Assumes that fill and stroke colors are the same.
    return derived().image.get_g_element(PLOT_X_AXIS).style().stroke_color();
  }

  Derived& y_axis_color(const svg_color& col)
  {
    derived().image.get_g_element(PLOT_Y_AXIS).style().fill_color(col);
    derived().image.get_g_element(PLOT_Y_AXIS).style().stroke_color(col);
    return derived();
  }

  svg_color y_axis_color()
  {
    return derived().image.get_g_element(PLOT_Y_AXIS).style().stroke_color();
  }

  Derived& x_label_color(const svg_color& col)
  { // add fill as well PAB Oct 07
    derived().image.get_g_element(PLOT_X_LABEL).style().fill_color(col);
    derived().image.get_g_element(PLOT_X_LABEL).style().stroke_color(col);
    return derived();
  }

  Derived& x_label_width(double width)
  { // width of text is effectively the boldness.
    x_label_width_ = width;
    derived().image.get_g_element(PLOT_X_LABEL).style().stroke_width(width);
    return derived();
  }

  double x_label_width()
  {
    //return x_label_width_;
    return derived().image.get_g_element(PLOT_X_LABEL).style().stroke_width();
  }

  svg_color x_label_color()
  {
    return derived().image.get_g_element(PLOT_X_LABEL).style().fill_color();
  }

  Derived& y_label_color(const svg_color& col)
  {
    derived().image.get_g_element(PLOT_Y_LABEL).style().fill_color(col);
    derived().image.get_g_element(PLOT_Y_LABEL).style().stroke_color(col);
    return derived();
  }

  svg_color y_label_color()
  {
    return derived().image.get_g_element(PLOT_Y_LABEL).style().fill_color();
  }

  Derived& x_major_tick_color(const svg_color& col)
  {
    derived().image.get_g_element(PLOT_X_MAJOR_TICKS).style().stroke_color(col);
    return derived();
  }

  svg_color x_major_tick_color()
  {
    return derived().image.get_g_element(PLOT_X_MAJOR_TICKS).style().stroke_color();
  }

  Derived& x_minor_tick_color(const svg_color& col)
  {
    derived().image.get_g_element(PLOT_X_MINOR_TICKS).style().stroke_color(col);
    return derived();
  }

  svg_color x_minor_tick_color()
  {
    return derived().image.get_g_element(PLOT_X_MINOR_TICKS).style().stroke_color();
  }

  Derived& x_major_grid_color(const svg_color& col)
  {
    derived().image.get_g_element(PLOT_X_MAJOR_GRID).style().stroke_color(col);
    return derived();
  }

  svg_color x_major_grid_color()
  {
    return derived().image.get_g_element(PLOT_X_MAJOR_GRID).style().stroke_color();
  }

  Derived& x_major_grid_width(double w)
  {
    derived().image.get_g_element(PLOT_X_MAJOR_GRID).style().stroke_width(w);
    return derived();
  }

  double x_major_grid_width()
  {
    return derived().image.get_g_element(PLOT_X_MAJOR_GRID).style().stroke_width();
  }

  Derived& x_minor_grid_color(const svg_color& col)
  {
    derived().image.get_g_element(PLOT_X_MINOR_GRID).style().stroke_color(col);
    return derived();
  }

  svg_color x_minor_grid_color()
  {
    return derived().image.get_g_element(PLOT_X_MINOR_GRID).style().stroke_color();
  }

  Derived& x_minor_grid_width(double w)
  {
    derived().image.get_g_element(PLOT_X_MINOR_GRID).style().stroke_width(w);
    return derived();
  }

  double x_minor_grid_width()
  {
    return derived().image.get_g_element(PLOT_X_MINOR_GRID).style().stroke_width();
  }

  Derived& x_axis_width(double width)
  {
    derived().image.get_g_element(PLOT_X_AXIS).style().stroke_width(width);
    return derived();
  }

  double x_axis_width()
  {
    return derived().image.get_g_element(PLOT_X_AXIS).style().stroke_width();
  }

  Derived& x_label(const std::string& str)
  {
    derived().x_label_info.text(str);
    return derived();
  }

  std::string x_label()
  {
    return derived().x_label_info.text();
  }

  Derived& x_label_units(const std::string& str)
  {
    derived().x_units_info.text(str);
    return derived();
  }

  std::string x_label_units()
  {
    return derived().x_units_info.text();
  }

  // y_label not needed in 1D.
  Derived& y_label(const std::string& str)
  {
    derived().y_label_info.text(str);
    return derived();
  }

  std::string y_label()
  {
    return derived().y_label_info.text();
  }

  Derived& y_label_units(const std::string& str)
  {
    derived().y_units_info.text(str);
    return derived();
  }

  std::string y_label_units()
  {
    return derived().y_units_info.text();
  }

  Derived& x_major_interval(double inter)
  {
    derived().x_major_interval_ = inter;
    return derived();
  }

  double x_major_interval()
  {
    return derived().x_major_interval_;
  }

  Derived& x_major_tick_length(double length)
  {
    derived().x_major_tick_length_ = length;
    return derived();
  }

  double x_major_tick_length()
  {
    return derived().x_major_tick_length_;
  }

  Derived& x_major_tick_width(double width)
  {
    derived().x_major_tick_width_ = width; // Redundant?
    derived().image.get_g_element(PLOT_X_MAJOR_TICKS).style().stroke_width(width);
    return derived();
  }

  double x_major_tick_width()
  {
    return derived().image.get_g_element(PLOT_X_MAJOR_TICKS).style().stroke_width();
  }

  Derived& x_minor_tick_length(double length)
  {
    derived().x_minor_tick_length_ = length;
    return derived();
  }

  double x_minor_tick_length()
  {
    return derived().x_minor_tick_length_;
  }

  Derived& x_minor_tick_width(double width)
  {
    derived().x_minor_tick_width_ = width;
    derived().image.get_g_element(PLOT_X_MINOR_TICKS).style().stroke_width(width);
    return derived();
  }

  double x_minor_tick_width()
  {
   // return derived().x_minor_tick_width_; // should be the same but store in stroke_width is definitive.
   return derived().image.get_g_element(PLOT_X_MINOR_TICKS).style().stroke_width();
  }

  Derived& x_major_tick(double d)
  { // Interval (Cartesian units) between major ticks.
    derived().x_major_interval_ = d;
  }

  double x_major_tick()
  { // Interval (Cartesian units) between major ticks.
    return derived().x_major_interval_;
  }

  Derived& x_minor_interval(double interval)
  { // aka x_minor_tick
    derived().x_minor_interval_ = interval;
    return derived();
  }

  Derived& x_num_minor_ticks(unsigned int num)
  {
    derived().x_num_minor_ticks_ = num;
    return derived();
  }

  unsigned int x_num_minor_ticks()
  { // NB NOT float or double!
    return derived().x_num_minor_ticks_;
  }

  Derived& x_range(double min_x, double max_x)
  {
    if(max_x <= min_x)
    {
      throw std::runtime_error("Illegal Argument: X range: x_max < x_min");
    }
    derived().x_min = min_x;
    derived().x_max = max_x;
    return derived();
  }

  std::pair<double, double> x_range()
  {
    std::pair<double, double> r;
    r.first = derived().x_min;
    r.second = derived().x_max;
    return r;
  }

  // Avoid clashes with class svg_1d_plot variable x_min & x_max,
  // so use longer x_minimum, x_maximum ...
  Derived& x_minimum(double min_x)
  {
    // Can't check here that x_max > x_min because may not have set x_max yet.
    // TODO ensure that there is another check somewhere.
    derived().x_min = min_x;
    return derived();
  }

  double x_minimum()
  {
    return derived().x_min;
  }

  Derived& x_maximum(double x)
  {
    // Can't check here that x_max > x_min because may not have set x_min yet.
    // TODO check that there is another check somewhere.
    derived().x_max = x;
    return derived();
  }

  double x_maximum()
  {
    return derived().x_max;
  }

  // Stylesheet.

  Derived& load_stylesheet(const std::string& file)
  {
    derived().image.load_stylesheet(file);
    return derived();
  }

  // Image info (& identical const version).

  svg& get_svg()
  {
    derived()._update_image();
    return derived().image;
  }

  const svg& get_svg() const
  {
    derived()._update_image();
    return derived().image;
  }
}; // template <class Derived> class axis_plot_frame

} // detail
} // svg
} // boost

#endif // BOOST_SVG_AXIS_PLOT_FRAME_HPP
