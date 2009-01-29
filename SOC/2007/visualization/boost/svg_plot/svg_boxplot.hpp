/*!
  \file svg_boxplot.hpp
  \brief Create box plots in Scalable Vector Graphic (SVG) format.
  \details Provides svg_boxplot data and function to create plots, and svg_boxplot_series to allow data values to be added.
  Very many functions allow fine control of the appearance and layout of plots and data markers.
  Items common to 1D, 2D and boxplot use axis_plot_frame.
  A convenient way of graphically depicting groups of numerical data through their five-number summaries.
  Show 1st quartile, median and 3rd quartile as a box.
  http://en.wikipedia.org/wiki/Boxplot

  Some Implementations of the Boxplot
  Michael Frigge, David C. Hoaglin and Boris Iglewicz
  The American Statistician, Vol. 43, No. 1 (Feb., 1989), pp. 50-54

  See also : The Bagplot: A Bivariate Boxplot
  Peter J. Rousseeuw, Ida Ruts and John W. Tukey
  The American Statistician, Vol. 53, No. 4 (Nov., 1999), pp. 382-387

  \author Jacob Voytko & Paul A. Bristow
 */

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2008

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_SVG_BOXPLOT_HPP
#define BOOST_SVG_SVG_BOXPLOT_HPP

#include <boost/iterator/transform_iterator.hpp>

#if defined (BOOST_MSVC)
#  pragma warning(push)
#  pragma warning(disable: 4512) // assignment operator could not be generated.
#  pragma warning(disable: 4127) // conditional expression is constant.
#endif

#include "svg.hpp"
#include "svg_style.hpp"
#include "detail/numeric_limits_handling.hpp"
#include "detail/svg_boxplot_detail.hpp"
#include "detail/functors.hpp"
#include "detail/axis_plot_frame.hpp"
#include "detail/auto_axes.hpp" //
#include "quantile.hpp"

#include <vector>
#include <string>
#include <ostream>
#include <algorithm>
#include <iterator>
#include <exception>

namespace boost
{
namespace svg
{
  //static const double sin45 = 0.707; // Use if axis value labels are sloping.
  //static svg_style not_a_svg_style; // Used as a test for not set.

  //// x_axis_position_ and y_axis_position_  use these.
  //enum x_axis_intersect {bottom = -1, x_intersects_y = 0, top = +1};
  //// bottom = X-axis free below bottom of end of Y-axis (case of all Y definitely < 0).
  //// top = X-axis free above top of X-axis (case of all Y definitely > 0).
  //// x_intersects_y when Y values include zero, so X intersects the Y axis.

  //enum y_axis_intersect {left = -1, y_intersects_x = 0, right = +1};
  //// left = Y-axis free to left of end of X-axis (case of all X definitely < 0).
  //// right = Y-axis free to left of end of X-axis (case of all X definitely > 0).
  //// y_intersects_x when X values include zero, so intersects the X axis.

  //enum legend_places
  //{  // Placing of legend box, if requested by legend_on(true).
  //  nowhere = 0,
  //  inside = -1,  // Default place for inside is top left of plot window,
  //  // exact location controlled by legend_top_left().
  //  outside_left = 1, // Outside
  //  outside_right = +2,  // Default for outside.
  //  outside_top = +3,
  //  outside_bottom = +4,
  //  somewhere = +5 // legend_top_left(x, y)
  //};

  class svg_boxplot; // Box Plot that can contain several boxplot data series.
  class svg_boxplot_series; // Boxplot data series.

  // By convention, class data is suffixed by _ to permit function names to be used.
  // For example, string title_, but set with title("my title") and get with title().

  class svg_boxplot_series
  { /*! \class boost::svg::svg_boxplot_series
     \brief Information about a data series to be boxplotted.
     \details A Box Plot that can contain several boxplot data series.
     median, whiskers and outliers are computed for each series.
     */
  public: // TODO private?
    std::vector<double> series_; //!< Data series for the boxplot.
    double whisker_min_; //!<
    double whisker_max_;
    int quartile_definition_; //!< The definition of the quartile can be selected.
    double q1_; //!< 1st lower quartile.
    double median_; //!< 2nd middle quartile.
    double q3_; //!< 3rd upper quartile.
    std::vector<double> outliers_; //!< Any data values that are judged outliers.
    std::vector<double> extreme_outliers_;//!< Any data values that are judged extreme outliers.

    // std::string title_;
    text_style series_style_;
    text_element series_info_;
    double text_margin_;

    // svg_style(stroke, fill, width)
    double box_width_;
    svg_style box_style_;
    svg_style median_style_;
    svg_style axis_style_;

    double whisker_length_;
    svg_style min_whisker_style_;
    svg_style max_whisker_style_;
    value_style values_style_;

    plot_point_style mild_outlier_;
    plot_point_style ext_outlier_;

    bool outlier_values_on_;
    bool extreme_outlier_values_on_;

    template <class T> // T is data container type.
    svg_boxplot_series( // Constructor.
      T begin, T end, // Data container.
      const std::string& title, // Data series title.
      double bw,// box width
      svg_style bs, // box style
      svg_style ms, // median marker style
      svg_style as, // axis style
      double wl, // whisker length
      svg_style minws, // min whisker style
      svg_style maxws, // max whisker style
      plot_point_style os, // mild outlier style
      plot_point_style extos, // extreme outlier style
      int q_def, // Quartile definition H&F #
      value_style vs, // Style for optional display of data point values.
      text_style ss // series style (font etc) for box labels.
     )
     // All other parameters can also be added using chainable functions.
     : //
      box_width_(bw), // svg_boxplot::box_width()),
      box_style_(bs), // green, azure, 1), // stroke, fill, width, 1, true, true, margin, border_on, fill_on
      median_style_(ms), // blue, blank, 1),
      axis_style_(as), // black, blank, 1),
      whisker_length_(wl), // 30),
      min_whisker_style_(minws), // magenta, black, 1),
      max_whisker_style_(maxws), // cyan, black, 1),
      mild_outlier_(os), // brown, blank, 5, round),
      ext_outlier_(extos), //  blue, blank, 25, cone),
      values_style_(vs),
      series_style_(ss), // for series_info to control font etc of box labels.
      series_info_(0, 0, title, series_style_, center_align, horizontal),
      quartile_definition_(q_def), // Default is H&F recommendation.
      text_margin_(2.) // for axis label text, as a multiplier of the font size.
    { // Constructor.
      // std::vector copy is sorted and used for fast lookup of quartile values.
      //std::vector<double> series(begin, end); now a member
      series_.assign(begin, end);

      if(series_.empty())
      {
        std::string message = "Data series " + title + " is empty!";
        throw std::runtime_error(message.c_str());
      }
      if(series_.size() < 8)
      {
        throw std::runtime_error("Data series has < 8 values!");
      }
      std::sort(series_.begin(), series_.end());
      // Previous taken out to calculate_quantiles to allow plot to have their own quartile definition.

    } // svg_boxplot_series constructor.

    void calculate_quantiles()
    { /*! \brief Divide sorted data set into four equal parts, called quartiles,
        so each part represent 1/4th of the sampled population.
       \details
         Michael Frigge, David C. Hoaglin and Boris Iglewicz
        The American Statistician, Vol. 43, No. 1 (Feb., 1989), pp. 50-54
        Tukey, J. W. Exploratory Data Analysis, Addison Wesley (1977, p 33)

       "Some Implementations of the Boxplot"

        x[1] .. x[n] == series[0] ... series[n - 1]
        q1_ = (1 - g) * x[j] + g * [j+1];
      */
      median_ = median(series_);
      q1_ = quantile(series_, 0.25, quartile_definition_);
      q3_ = quantile(series_, 0.75, quartile_definition_);

      /*! Fences (beyond which lie outliers) are at q1 - k * (q3 - q1) and q3 - k * (q3 - q1)
        commonly k = 1.5, but can be 2.
        Extreme outlier usually use k = 3.
      */

      //std::cout << "Definition #" << quartile_definition_ << ", q1 " << q1_ << ", median " << median_ << ", q3 " << q3_ << ", IQR " << q3_ - q1_ << std::endl;

      double iqr(q3_ - q1_); //! Interquartile range.
      const double k_outlier = 1.5; // 2. also used.
      const double k_extreme_outlier = 3.;

      double min_cutoff = q1_ - k_outlier * iqr;
      double max_cutoff = q3_ + k_outlier * iqr;
      double min_ext_cutoff = q1_ - k_extreme_outlier * iqr;
      double max_ext_cutoff = q3_ + k_extreme_outlier * iqr;

      //std::cout << "Min outlier fences <" << min_cutoff << " or >" << max_cutoff << std::endl;
      //std::cout << "Min extreme fences <" << min_ext_cutoff <<  " or >" << max_ext_cutoff << std::endl;

      std::vector<double>::const_iterator i;
      for(i = series_.begin(); *i < min_cutoff; ++i)
      { //! Separate any minimum outliers and extreme outliers.
        if(*i < min_ext_cutoff)
        {
          extreme_outliers_.push_back(*i);
        }
        else if(*i < min_cutoff)
        {
          outliers_.push_back(*i);
        }
      } // for

      whisker_min_ = *i;
      std::vector<double>::const_reverse_iterator j;
      for(j = series_.rbegin(); *j > max_cutoff; ++j)
      { //! Separate any maximum outliers and extreme outliers.
        if(*j > max_ext_cutoff)
        {
          extreme_outliers_.push_back(*j);
        }
        else if(*j > max_cutoff)
        {
          outliers_.push_back(*j);
        }
      } // for
      whisker_max_ = *j;
    } //   void calculate_quantiles()

    // Set member functions for the boxplot data series class.
    // (Note identically names functions are provided for the boxplot class).

    svg_boxplot_series& title(const std::string& t); //! Set title for data series.
    const std::string title(); //! Get title for boxplot.
    svg_boxplot_series& whisker_length(double l); //! Set length of whisker line.
    double whisker_length(); //! Get length of whisker line.
    svg_boxplot_series& box_width(double l); //! Set width of box.
    double box_width(); //! Get width of box.
    svg_boxplot_series& min_whisker_color(const svg_color& col); //! Color of boxplot whisker.
    svg_color min_whisker_color(); //! Get color of boxplot whisker.
    svg_boxplot_series& min_whisker_width(double l); //! Set width of whisker line.
    double min_whisker_width(); //! Get width of whisker line.
    svg_boxplot_series& max_whisker_color(const svg_color& col); // Color of whisker.
    svg_color max_whisker_color(); //!  Color of whisker.
    svg_boxplot_series& max_whisker_width(double l); //! Width of whisker line.
    double max_whisker_width(); //!  Width of whisker line.
    svg_boxplot_series& box_fill(const svg_color& color); //! Color of box fill, not border.
    svg_color box_fill(); //! Color of box fill, not border.
    svg_boxplot_series& box_border(const svg_color& color); //! Color of border (not fill) to box.
    svg_color box_border(); //! Color of border (not fill) to box.

    svg_boxplot_series& median_color(const svg_color& color); //! Color of median line in box.
    svg_color median_color(); //! Color of median line in box.
    svg_boxplot_series& median_width(double l); //! Width of median line in box.
    double median_width(); //!  Width of median line in box.

    svg_boxplot_series& axis_color(const svg_color& color); //! Color of vertical axis whisker line in box.
    svg_color axis_color(); //! Color of vertical axis whisker line in box.
    svg_boxplot_series& axis_width(double l); //! Width of vertical axis whisker line in box.
    double axis_width(); //! Get width of vertical axis whisker line in box.

    svg_style& box_style(); //! Get reference to style of box.
    svg_boxplot_series& box_style(svg_style& bs); //! Set reference to style of box.

    svg_style& median_style(); //! Get reference to style of median marker line.
    svg_boxplot_series& median_style(svg_style& ms);
    plot_point_style& outlier_style(); //! Get reference to style of outlier marker line.
    svg_boxplot_series& outlier_style(plot_point_style& os); //! Set reference to style of outlier marker line.

    svg_boxplot_series& outlier_color(const svg_color& color); //! Set color of outlier marker.
    svg_color outlier_color(); //! Get color of outlier marker.
    svg_boxplot_series& outlier_fill(const svg_color& color); //! Set fill color of outlier marker.
    svg_color outlier_fill(); //! Get fill color of outlier marker.
    svg_boxplot_series& extreme_outlier_color(const svg_color& color); //! Set color of extreme outlier marker.
    svg_color extreme_outlier_color(); //! Get color of extreme outlier marker.
    svg_boxplot_series& extreme_outlier_fill(const svg_color& color); //! Set color of extreme outlier marker.
    svg_color extreme_outlier_fill(); // Get color of extreme outlier marker.
    svg_boxplot_series& outlier_shape(point_shape shape); //! Set shape of outlier marker.
    point_shape outlier_shape(); //! Get shape of outlier marker.
    svg_boxplot_series& outlier_size(int size); //! Set size of outlier marker.
    int outlier_size(); //! Get size of outlier marker.
    svg_boxplot_series& extreme_outlier_shape(point_shape shape); //! Shape of extreme outlier marker.                      !
    point_shape extreme_outlier_shape(); //! Get shape of extreme outlier marker.
    svg_boxplot_series& extreme_outlier_size(int size); //! Size of extreme outlier marker.
    int extreme_outlier_size(); //! Get size of extreme outlier marker.
    // Not implemented yet - settings are boxplot wide.
    //svg_boxplot_series& outlier_values_on(bool cmd);// If values to be shown alongside outlier markers.
    //bool outlier_values_on(); // If values to be shown alongside outlier markers.
    //svg_boxplot_series& extreme_outlier_values_on(bool cmd); // If values to be shown alongside extreme outlier markers.
    //bool extreme_outlier_values_on(); // If values to be shown alongside extreme outlier markers.
    svg_boxplot_series& quartile_definition(int def); //! Set H&F quartile definition.
    int quartile_definition(); //! Get H&F quartile definition.

  }; // struct svg_boxplot_series

  // svg_boxplot_series *Definitions* ready to go into separate *.ipp file.

  const std::string svg_boxplot_series::title()
  { //! Get title of a data series.
    return series_info_.text();
  }

  svg_boxplot_series& svg_boxplot_series::title(const std::string& t)
  { //! Set title of a data series.
    series_info_.text(t);
    return *this; // \return reference to svg_boxplot_series to make chainable.
  }

  svg_boxplot_series& svg_boxplot_series::whisker_length(double width)
  { // Applies to BOTH min and max whisker.
    whisker_length_ = width;
    return *this; // Chainable.
  }

  double svg_boxplot_series::whisker_length()
  { // Applies to BOTH min and max whisker.
    return whisker_length_;
  }

  svg_boxplot_series& svg_boxplot_series::min_whisker_width(double width)
  { // Line width of min whisker.
    min_whisker_style_.stroke_width(width);
    return *this;  // \return reference to svg_boxplot_series to make chainable..
  }

  double svg_boxplot_series::min_whisker_width()
  { // Line width of min whisker.
    return min_whisker_style_.stroke_width();
  }

  svg_boxplot_series& svg_boxplot_series::min_whisker_color(const svg_color& color)
  { // Color of min whisker.
    min_whisker_style_.stroke_color(color);
    return *this; // \return reference to svg_boxplot_series to make chainable.
  }
  svg_color svg_boxplot_series::min_whisker_color()
  { //  Color of min whisker.
    return min_whisker_style_.stroke_color();
  }

  svg_boxplot_series& svg_boxplot_series::max_whisker_width(double width)
  {
    max_whisker_style_.stroke_width(width);
    return *this;  // \return reference to svg_boxplot_series to make chainable.
  }

  double svg_boxplot_series::max_whisker_width()
  { // width of the box, not the margin.
    return max_whisker_style_.stroke_width();
  }

  svg_boxplot_series& svg_boxplot_series::max_whisker_color(const svg_color& color)
  { // Color of whisker.
    max_whisker_style_.stroke_color(color);
    return *this; // \return reference to svg_boxplot_series to make chainable.
  }

  svg_color svg_boxplot_series::max_whisker_color()
  { //  Color of whisker.
    return max_whisker_style_.stroke_color();
  }

  double svg_boxplot_series::box_width()
  { // width of the box, not the margin.
    return box_width_;
  }

  svg_boxplot_series&  svg_boxplot_series::box_width(double l)
  { // width of the box, not the margin.
    box_width_ = l;
    return *this; // \return reference to svg_boxplot_series to make chainable.
  }

  svg_style& svg_boxplot_series::box_style()
  {
    return box_style_;
  }

  svg_boxplot_series& svg_boxplot_series::box_style(svg_style& bs)
  {
    box_style_ = bs;
    return *this; // \return reference to svg_boxplot_series to make chainable.
  }

  svg_style& svg_boxplot_series::median_style()
  {
    return median_style_;
  }

  svg_boxplot_series& svg_boxplot_series::median_style(svg_style& ms)
  {
    median_style_ = ms;
    return *this; // \return reference to svg_boxplot_series to make chainable.
  }

  svg_boxplot_series& svg_boxplot_series::outlier_style(plot_point_style& os)
  {
    mild_outlier_ = os;
    return *this; // \return reference to svg_boxplot_series to make chainable.
  }

  plot_point_style& svg_boxplot_series::outlier_style()
  {
    return mild_outlier_;
  }

  svg_boxplot_series& svg_boxplot_series::box_fill(const svg_color& color)
  { // Color of box fill, not border.
    box_style_.fill_color(color);
    return *this; // \return reference to svg_boxplot_series to make chainable.
  }

  svg_color svg_boxplot_series::box_fill()
  { // Color of box fill, not border.
    return box_style_.fill_color();
  }

  svg_boxplot_series& svg_boxplot_series::box_border(const svg_color& color)
  { // Color of box fill, not border.
    box_style_.stroke_color(color);
    return *this; // \return reference to svg_boxplot_series to make chainable.
  }

  svg_color svg_boxplot_series::box_border()
  { // Color of box fill, not border.
    return box_style_.stroke_color();
  }

  svg_boxplot_series& svg_boxplot_series::median_color(const svg_color& color)
  { // Color of median line in box.
    median_style_.stroke_color(color);
    return *this; // \return reference to svg_boxplot_series to make chainable.
  }

  svg_color svg_boxplot_series::median_color()
  {  // Color of median line in box.
    return median_style_.stroke_color();
  }

  svg_boxplot_series& svg_boxplot_series::median_width(double width)
  { // Applies to BOTH min and max whisker.
    median_style_.stroke_width(width);
    return *this; // \return reference to svg_boxplot_series to make chainable..
  }

  double svg_boxplot_series::median_width()
  { // Applies to BOTH min and max whisker.
    return  median_style_.stroke_width();
  }

  svg_boxplot_series& svg_boxplot_series::axis_color(const svg_color& color)
  { // Color of axis line in box.
    axis_style_.stroke_color(color);
    return *this; // \return reference to svg_boxplot_series to make chainable.
  }

  svg_color svg_boxplot_series::axis_color()
  {  // Color of axis line in box.
    return axis_style_.stroke_color();
  }

  svg_boxplot_series& svg_boxplot_series::axis_width(double width)
  { // Applies to BOTH min and max whisker.
    axis_style_.stroke_width(width);
    return *this; // \return reference to svg_boxplot_series to make chainable.
  }

  double svg_boxplot_series::axis_width()
  { // Applies to BOTH min and max whisker.
    return  axis_style_.stroke_width();
  }

  svg_boxplot_series& svg_boxplot_series::outlier_color(const svg_color& color)
  { // Color of axis line in box.
    mild_outlier_.stroke_color(color);
    return *this; // \return reference to svg_boxplot_series to make chainable.
  }

  svg_color svg_boxplot_series::outlier_color()
  {  // Color of axis line in box.
    return mild_outlier_.stroke_color();
  }

  svg_boxplot_series& svg_boxplot_series::outlier_fill(const svg_color& color)
  { // Fill color of axis line in box.
    mild_outlier_.fill_color(color);
    return *this; // \return reference to svg_boxplot_series to make chainable.
  }

  svg_color svg_boxplot_series::outlier_fill()
  {  // Fill color of axis line in box.
    return mild_outlier_.fill_color();
  }

  svg_boxplot_series& svg_boxplot_series::extreme_outlier_color(const svg_color& color)
  { // Color of axis line in box.
    ext_outlier_.stroke_color(color);
    return *this; // \return reference to svg_boxplot_series to make chainable.
  }

  svg_color svg_boxplot_series::extreme_outlier_color()
  {  // Color of axis line in box.
    return ext_outlier_.stroke_color();
  }

  svg_boxplot_series& svg_boxplot_series::extreme_outlier_fill(const svg_color& color)
  { // Color of axis line in box.
    ext_outlier_.fill_color(color);
    return *this; // \return reference to svg_boxplot_series to make chainable.
  }

  svg_color svg_boxplot_series::extreme_outlier_fill()
  {  // Color of axis line in box.
    return ext_outlier_.fill_color();
  }

  svg_boxplot_series& svg_boxplot_series::outlier_shape(point_shape shape)
  { // Shape of outlier marker.
    mild_outlier_.shape_ = shape;
    return *this; // \return reference to svg_boxplot_series to make chainable.
  }

  point_shape svg_boxplot_series::outlier_shape()
  { // Color of outlier marker.
    return mild_outlier_.shape_;
  }

  svg_boxplot_series& svg_boxplot_series::outlier_size(int size)
  { // Size of outlier marker.
    mild_outlier_.size_ = size;
    return *this; // \return reference to svg_boxplot_series to make chainable.
  }

  int svg_boxplot_series::outlier_size()
  { // Size of extreme outlier marker.
    return mild_outlier_.size_;
  }

  svg_boxplot_series& svg_boxplot_series::extreme_outlier_shape(point_shape shape)
  { // Shape of extreme outlier marker.
    ext_outlier_.shape_ = shape;
    return *this; // \return reference to svg_boxplot_series to make chainable.
  }

  point_shape svg_boxplot_series::extreme_outlier_shape()
  { // Shape of extreme outlier marker.
    return ext_outlier_.shape_ ;
  }

  svg_boxplot_series& svg_boxplot_series::extreme_outlier_size(int size)
  { // Size of extreme outlier marker.
    ext_outlier_.size_ = size;
    return *this; // \return reference to svg_boxplot_series to make chainable..
  }

  int svg_boxplot_series::extreme_outlier_size()
  { // Size of extreme outlier marker.
    return ext_outlier_.size_;
  }

  svg_boxplot_series& svg_boxplot_series::quartile_definition(int def)
  { // Choice of H&F quartile definition.
    quartile_definition_ = def;
    return *this; // \return reference to svg_boxplot_series to make chainable.
  }

  int svg_boxplot_series::quartile_definition()
  { // Choice of H&F quartile definition.
    return quartile_definition_;
  }


  // Not implemented yet - so these are only available for ALL boxplots.
  //svg_boxplot_series& svg_boxplot_series::outlier_values_on(bool cmd)
  //{
  //  outlier_values_on_ = cmd;
  //  return *this;
  //}

  //svg_boxplot_series& svg_boxplot_series::extreme_outlier_values_on(bool cmd)
  //{
  //  extreme_outlier_values_on_ = cmd;
  //  return *this;
  //}

  //bool svg_boxplot_series::outlier_values_on()
  //{
  //  return outlier_values_on_;
  //}

  //bool svg_boxplot_series::extreme_outlier_values_on()
  //{
  //  return extreme_outlier_values_on_;
  //}


// End of svg_boxplot_series Definitions.

class svg_boxplot  : public detail::axis_plot_frame<svg_boxplot>
{ /*! \class boost::svg::svg_boxplot
    \brief A plot that can display boxplots of several data series.
    \details Holds all data about the plot (but not any data series - see svg_boxplot_series)
    axis_plot_frame.hpp contains functions common to 1 and 2-D, and boxplot.
  */
  friend svg_boxplot_series;
  friend class detail::axis_plot_frame<svg_boxplot>;
  // axis_plot_frame.hpp contains functions common to 1 and 2-D, and boxplot.

private:
  svg image; // Stored so as to avoid rewriting style information constantly.
  // image.image_size(500, 500);

  double x_scale_; //! Used by function transform()
  double x_shift_; //! to go from Cartesian to SVG coordinates.
  double y_scale_; //! SVG coordinates origin is at top left,
  double y_shift_;  //! and y coordinates increase DOWN the page!

  double x_axis_position_;
  double y_axis_position_;

  // Plot window.
  double plot_left_; // calculate_plot_window() sets these values.
  double plot_top_;
  double plot_right_;
  double plot_bottom_;

  // Order of these declarations (not definitions) seems to matter - styles 1st.
  text_style a_style_; // Defaults.
  text_style title_style_;
  text_style x_axis_label_style_;
  text_style x_value_label_style_;
  text_style y_axis_label_style_;
  text_style y_value_label_style_;
  text_style point_symbols_style_; // Used for data point marking.
  text_style value_style_; // Used for data point value label.

  axis_line_style x_axis_;
  axis_line_style y_axis_;

 // text_style contains font size & type etc.
  value_style values_style_; // Data point value marking.

  text_element title_info_;
  text_element x_label_info_;
  text_element y_label_info_;
  text_element x_units_info_; // For example: "mm"
  text_element y_units_info_; // 2-D only.

  // Should use axis_style?
  //    x_axis_(X, -10., +10., black, 1, 0, true, false, true),
  box_style image_border_; // rectangular border of all image width, color...
  box_style plot_window_border_; // rectangular border of plot window width, color...

  ticks_labels_style x_ticks_;
  ticks_labels_style y_ticks_; // Added to permit shared code!

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
  int y_steps_;  // If autoscaled, set any prescaling to decimal 1, 2, 5, 10 etc.

    // Values calculated by scale_axis, and is used only if y_autoscale == true.
  double y_auto_min_value_;
  double y_auto_max_value_;
  double y_auto_tick_interval_; // tick major interval.
  int y_auto_ticks_; // Number of ticks.

  double text_margin_; // Marginal space around text items like title,
  // text_margin_ * font_size to get distance in svg units.
  bool title_on_;

  std::string plot_window_clip_;
  bool plot_window_on_;
  bool median_values_on_; // If values of data are shown as well as a marker like square.
  bool outlier_values_on_; // If values of data are shown.
  bool extreme_outlier_values_on_; // If values of data are shown.

  // For storing the data points for sorting.
  std::vector<svg_boxplot_series> series;

  // Defaults (see below) for the box'n'whiskers
  // can be changed by functions like boxplot.box_width(50)
  // for all following plot(data,"title") calls
  // But can also change each plot data series separately.
  double box_width_;
  svg_style box_style_;

  svg_style median_style_;
  text_style series_style_;
  svg_style axis_style_;
  double whisker_length_;
  svg_style min_whisker_style_;
  svg_style max_whisker_style_;
  int quartile_definition_;
  plot_point_style mild_outlier_;
  plot_point_style ext_outlier_;

public:
 // text_element(double x = 0., double y = 0., const std::string text = "", text_style ts = no_style, align_style align = left_align, rotate_style rotate = horizontal)

  svg_boxplot()
  :
  title_style_(18, "Verdana", "", "", "", ""),
  value_style_(10, "Verdana", "", "", "", ""), // Data values
  title_info_(0, 0, "", title_style_, center_align, horizontal),
  title_on_(true),
  x_label_info_(0, 0,  "", x_axis_label_style_ ,center_align, horizontal),
  x_units_info_(0, 0, "", x_value_label_style_, center_align, horizontal),
  y_label_info_(0, 0, "", y_axis_label_style_, center_align, upward),
  y_units_info_(0, 0, "", y_axis_label_style_, center_align, upward),
  x_axis_(X, -10., +10., black, 1, 0, true, false, true),
  y_axis_(Y,   0.,  +1., black, 1, 0, true, false, true),
  x_axis_label_style_(14, "Verdana", "", ""),
  x_value_label_style_(12, "Verdana", "", ""),
  y_axis_label_style_(14, "Verdana", "", ""),
  y_value_label_style_(12, "Verdana", "", ""),
  point_symbols_style_(12, "Lucida Sans Unicode"), // Used for data point marking.
  values_style_(rightward, 3, std::ios::dec, true, value_style_, black, black, false, false),

  x_ticks_(X, x_value_label_style_),// so for other defaults see ticks_labels_style.
  y_ticks_(Y, y_value_label_style_),

  autoscale_check_limits_(true), // Do check all value for limits, infinity, max, min, NaN.
  x_autoscale_(false),
  x_include_zero_(false), // If autoscaled, include zero.
  x_min_ticks_(6),  // If autoscaled, set a minimum number of ticks, default 6.
  x_steps_(0),  // If autoscaled, set any prescaling to decimal 1, 2, 5, 10 etc, default none (0).
  x_tight_(1e-6), // margin that point can lie outside top and bottom tick.

  y_include_zero_(false), // If autoscaled, include zero on Y-axis.
  y_min_ticks_(6),  // If autoscaled, set a minimum number of ticks, default 6.
  y_steps_(0),  // If autoscaled, set any prescaling to decimal 1, 2, 5, 10 etc, default none.
  y_tight_(1e-6), // margin that point can lie outside top and bottom tick.

  y_autoscale_(false), // Not used for 1-D.

  text_margin_(2.), // for axis label text, as a multiplier of the font size.
  image_border_(yellow, white, 2, 10, true, true), // margin should be about axis label font size.
  plot_window_border_(yellow, svg_color(255, 255, 255), 1, 3, true, false),

  plot_window_clip_("plot_window"),
  plot_window_on_(true),
  median_values_on_(false), // If values of data are shown as well as a marker like square.
  outlier_values_on_(false), // If values of data are shown as well as a marker like square.
  extreme_outlier_values_on_(false), // If values of data are shown as well.
  quartile_definition_(8), // Default H&F quartile definition.  This might be changed to 7 to match Excel and others?
  plot_left_(0.), plot_top_(0.), plot_right_(0.), plot_bottom_(0.),

  // Used to transform Cartesian to SVG.
  x_scale_(1.), x_shift_(0.),
  y_scale_(1.), y_shift_(0.),
  x_axis_position_(bottom),  // Default, but can be changed to top.
  y_axis_position_(left), // Default, but can be changed to right.

  // Box'n'whiskers default values (used in svg_boxplot_series construction)
  // Can be changed for all plots using svg_boxplot member functions.
  // Can be changed for individual data series using svg_boxplot_series member functions.
  box_width_(30), // can be changed with svg_boxplot::box_width()),
  series_style_(x_axis_label_style_), // want to use the x_label style here
  whisker_length_(30),
  box_style_(green, azure, 1), // stroke, fill, width, 1, true, true, margin, border_on, fill_on
  median_style_(blue, blank, 1),
  axis_style_(black, blank, 1),
  min_whisker_style_(magenta, black, 1),
  max_whisker_style_(cyan, black, 1),
  mild_outlier_(brown, blank, 5, round),
  ext_outlier_(red, blank, 5, cone)
  {
    using std::cout;
    using std::endl;
    image_size(500, 350);

   // Build the document tree. add children of the root node.
    for(int i = 0; i < boxplot::BOXPLOT_DOC_CHILDREN; ++i)
    {
      image.g();
    }
    //set_ids();
    //  void set_ids()  // This only used once in constructor and IS now inlined.
   // document ids for use in <g id = "PLOT_TITLE".../>
    for(int i = 0; i < boxplot::BOXPLOT_DOC_CHILDREN; ++i)
    { // Order determines the painting order.
      image.g(i).id(boxplot::document_ids[i]);
    }

    // Set boxplot color defaults.
    image.g(boxplot::PLOT_BACKGROUND).style().fill_color(image_border_.fill_);
    image.g(boxplot::PLOT_BACKGROUND).style().stroke_color(image_border_.stroke_);
    image.g(boxplot::PLOT_BACKGROUND).style().stroke_width(image_border_.width_); //

    image.g(boxplot::PLOT_WINDOW_BACKGROUND).style().fill_color(plot_window_border_.fill_);
    image.g(boxplot::PLOT_WINDOW_BACKGROUND).style().stroke_width(plot_window_border_.width_).stroke_color(plot_window_border_.stroke_);
    image.g(boxplot::X_AXIS).style().stroke_color(black).stroke_width(x_axis_.width());
    image.g(boxplot::Y_AXIS).style().stroke_color(black).stroke_width(y_axis_.width());

    // Ticks & grids.
    if(x_ticks_.use_up_ticks() || x_ticks_.use_down_ticks())
    {
      image.g(boxplot::X_TICKS).style().stroke_width(x_ticks_.minor_tick_width_).stroke_color(black);
    }
    if(y_ticks_.left_ticks_on_ || y_ticks_.right_ticks_on_)
    {
      image.g(boxplot::Y_MAJOR_TICKS).style().stroke_width(y_ticks_.major_tick_width_).stroke_color(black);
      image.g(boxplot::Y_MINOR_TICKS).style().stroke_width(y_ticks_.minor_tick_width_).stroke_color(black);
      image.g(boxplot::Y_MAJOR_GRID).style().stroke_width(y_ticks_.major_grid_width_).stroke_color(svg_color(200, 220, 255));
      image.g(boxplot::Y_MINOR_GRID).style().stroke_width(y_ticks_.minor_grid_width_).stroke_color(svg_color(200, 220, 255));
    }

    image.g(boxplot::X_LABEL).style().fill_color(black); // for text only specify fill_color (NOT stroke color).
    image.g(boxplot::Y_LABEL).style().fill_color(black);
    image.g(boxplot::VALUE_LABELS).style().fill_color(black);
    image.g(boxplot::PLOT_TITLE).style().fill_color(black);

    image.g(boxplot::WHISKER).style().stroke_color(black);
    image.g(boxplot::BOX_AXIS).style().stroke_color(black);
    image.g(boxplot::BOX).style().stroke_color(black).fill_color(ghostwhite);
    image.g(boxplot::MEDIAN).style().stroke_color(red).stroke_width(2);
    image.g(boxplot::EXTREME_OUTLIERS).style().stroke_color(purple).fill_color(white);
    image.g(boxplot::MILD_OUTLIERS).style().stroke_color(pink).fill_color(black);
    image.g(boxplot::DATA_VALUE_LABELS).style().fill_color(black).stroke_on(false);

  } // boxplot constructor.

  // Clear Functions.
  // When writing to multiple documents, the contents of the plot
  // may change significantly between. Rather than figuring out what
  // has and has not changed, just erase the contents of the
  // legend, title... in the document and start over.
  void clear_all()
  { // TODO?
    //clear_legend();
    //clear_background();
    //clear_x_axis();
    //clear_y_axis();
    //clear_title();
    //clear_points();
    //clear_plot_background();
    //clear_grids();
  }

  void transform_x(double& x)
  { // X coordinate from Cartesian to SVG.
    x = x * x_scale_ + x_shift_;
  }

  void transform_y(double& y)
  {// Y coordinate from Cartesian to SVG.
    y = y * y_scale_ + y_shift_;
  }

  void calculate_plot_window()
  {
    // Start by assuming we can use all the svg image,
    // but reduce by the width of any image border.
    plot_left_ = 0. + image_border_.width_; // Top left of image.
    plot_top_ = 0. + image_border_.width_;
    plot_right_ = image.x_size() - image_border_.width_; // Bottom right of image.
    plot_bottom_ = image.y_size() - image_border_.width_;
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
    if(title_on_)
    { // Allow space for plot title (at top).
      plot_top_ += title_info_.style().font_size() * (text_margin_ + 0.5);
    }
    // Assume that axis labels are always at bottom and left.
    if(x_axis_.label_on_)
    { // Move bottom up to leave space at bottom for X axis label.
       plot_bottom_ -= x_axis_label_style_.font_size() * (text_margin_ + 0.5);
    }
    if(y_axis_.label_on_)
    { // Leave space at left for Y axis label.
      plot_left_ += y_axis_label_style_.font_size() * text_margin_;
    }
    if (y_autoscale_) // TODO elsewhere - implement Y autoscale (no x autoscale of course).
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
    // Y axis position is NOT determined by the range of X values.
    //x_axis_position_ = bottom;  // Assume X-axis will intersect Y-axis (X range includes zero).
    //y_axis_position_ = left;  // Assume X-axis will intersect Y-axis (X range includes zero).
    // Default in constructor, but could be changed to top and/or left.

    // Ensure both axis and ticks have the *same* range.
    // (To use the separation, made to give the potential for different ranges,
    // one would have to *not* do this,
    // but to make sure they are both assigned correctly).
    x_ticks_.max_ = x_axis_.max_;
    x_ticks_.min_ = x_axis_.min_;
    y_ticks_.max_ = y_axis_.max_;
    y_ticks_.min_ = y_axis_.min_;

    // Calculate the number of chars of the longest value labels.
    // x_ticks_.longest_label(); // Can't use this for boxplot because
    // it is the length of the value labels.
    y_ticks_.longest_label(); // Updates y_label_max_length_
    double longest = 0;
    for (size_t i = 0; i < series.size(); i++)
    {

      double l = series[i].series_info_.text().size(); // Length of an X label.
      if(l > longest)
      {
        longest = l;
      }
    }
    if (longest == 0)
    {
      std::cout << "No X-axis labels!" << std::endl;
      // No labels provided so switch off labelling.
      x_ticks_.major_value_labels_side_ = 0;
    }
    x_ticks_.label_max_length_ = longest;

    // Check that labels won't collide and advise if they will?
    // Change rotation to avoid collision?

    y_ticks_.label_max_space_ = 0.; // Space for Y axis labels, depending on orientation.
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

    if (y_ticks_.major_value_labels_side_ != 0) // != none
    { // Major tick value labels wanted left or right.
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
    { // Only 1 char height & 1 space needed if labels are horizontal, regardless of length of string.
      x_ticks_.label_max_space_ = 2 * x_value_label_style_.font_size() * wh; // 2 SVG chars
    }
    else if ((x_ticks_.label_rotation_ == upward) || (x_ticks_.label_rotation_ == downward))
    { // Not horizontal so will need more than 2 chars worth.
        x_ticks_.label_max_space_ += x_ticks_.label_max_length_ ; // Number of SVG chars.
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
        )// && !(x_ticks_.ticks_on_window_or_axis_ < 0) ) // & not already at bottom.
      { // y_min_ > 0 so X-axis will not intersect Y-axis, so use plot window.
        x_axis_.axis_ = plot_bottom_; // Put X-axis on bottom.
      }
      else if ((x_axis_position_ == top)  // All Y values definitely < zero.
        && !(x_ticks_.ticks_on_window_or_axis_ > 0) ) // & not already at top.
      { // // y_max_ < 0 so X-axis will not intersect Y-axis, so use plot window.
         x_axis_.axis_ = plot_top_; // Put X-axis on top.
      }
      else
      { // y_axis_position_ == y_intersects_x
        // Calculate below after transform is calculated.
      }
    } // if (use_x_axis_line_)

    if (y_axis_.axis_line_on_)
    { // Want a vertical Y-axis line,
      // For boxplot do not check if range includes zero, so axes intersect,
      // and y_axis_ is svg coordinate of X-axis (usually x = 0).
      // Instead fix axis to left (or right) of the plot window.
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
    //else
    // for boxplot, Y-axis is always to right or left.
    //{
    //  x_axis_position_ = x_intersects_y;
    //  // Calculate below after transform is calculated.
    //}
    } // if (use_y_axis_line_)

    // plot window now calculated, so make some checks (might also check it fits into image?).
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
    y_scale_ = -(plot_bottom_ - plot_top_) / (y_axis_.max_ - y_axis_.min_);
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
    // For boxplot, Y axis is only ever left or right.
    //if (y_axis_.axis_line_on_)
    //{ // May need to calculate axes, if will intersect.
    //  if(y_axis_position_ == y_intersects_x)
    //  { // X Range *does* include zero, so y_axis_ not yet calculated.
    //    double x(0.);
    //    transform_x(x);
    //    y_axis_.axis_ = x; // SVG x coordinate of vertical Y-axis.
    //  }
    //}
    if (plot_window_on_)
    { // Draw plot window rectangle with border and/or background.
      image.g(detail::PLOT_WINDOW_BACKGROUND).push_back(
        new rect_element(plot_left_, plot_top_, (plot_right_ - plot_left_), plot_bottom_ - plot_top_));
    }
  } //  void calculate_plot_window()

  void draw_title()
  { // Update title_info_ with position.
    title_info_.x(image.x_size() / 2.); // Center of image.
    /*! \verbatim
      Assumes align = center_align.
      Note: center_align will ensure that will center correctly
      even if original string is long because contains Unicode like 
      because the browser render engine does the centering.
      \endverbatim
    */
    double y = title_info_.style().font_size() * text_margin_; // Leave a linespace above.
    title_info_.y(y); // Vertical position.
    image.g(boxplot::PLOT_TITLE).push_back(new text_element(title_info_));
  } // void draw_title()

  void draw_x_axis()
  { // Draw the horizontal X-axis line.
    if (x_axis_.axis_line_on_)
    { // Want a X-axis line.
      if (x_axis_position_ == top)
      { // horizontal line at top of plot window.
         image.g(boxplot::X_AXIS).line(plot_left_, plot_top_, plot_right_, plot_top_);
      }
      else if (x_axis_position_ == bottom)
      {// horizontal line at bottom of plot window.
         image.g(boxplot::X_AXIS).line(plot_left_, plot_bottom_, plot_right_, plot_bottom_);
      }
      else
      { // horizontal line at y = 0
        double y(0.);
        transform_y(y);
        //x_axis_. = y;
         image.g(boxplot::X_AXIS).line(plot_left_, y, plot_right_, y);
      }
    }
    path_element& major_tick_path = image.g(boxplot::X_TICKS).path();
    for(size_t i = 0; i < series.size(); ++i)
    { // Draw a ticks for each series, evenly spaced along X axis.
      draw_x_major_tick(
        plot_left_ + (plot_right_- plot_left_) *
        ((double)(i + 1)) / (double)(series.size() + 1),
        major_tick_path, series[i].series_info_);
    }
  } // void draw_x_axis()

  void draw_y_axis()
  {
    if (y_axis_.axis_line_on_)
    { // Want a vertical Y-axis line, for boxplot only ever left or right.
      if (y_axis_position_ == left)
      { // Vertical line at left of plot window.
         image.g(boxplot::Y_AXIS).line(plot_left_, plot_bottom_, plot_left_, plot_top_);
      }
      else if (y_axis_position_ == right)
      {// Vertical line at right of plot window.
         image.g(boxplot::Y_AXIS).line(plot_right_, plot_bottom_, plot_right_, plot_top_);
      }
    }

    path_element& minor_tick_path = image.g(boxplot::Y_MINOR_TICKS).path();
    path_element& major_tick_path = image.g(boxplot::Y_MAJOR_TICKS).path();
    path_element& major_grid_path = image.g(boxplot::Y_MAJOR_GRID).path();
    path_element& minor_grid_path = image.g(boxplot::Y_MINOR_GRID).path();

    // y_minor_jump is the interval between minor ticks.
    double y_minor_jump = y_ticks_.major_interval_ / ((double)(y_ticks_.num_minor_ticks_ + 1.) );

    // Draw the ticks on the positive side.
    for(double y = 0; y <= y_axis_.max_; y += y_ticks_.major_interval_)
    {
      for(double j = y + y_minor_jump;
        j < (y + y_ticks_.major_interval_) * (1. - 2 * std::numeric_limits<double>::epsilon());
        j += y_minor_jump)
      {
        draw_y_minor_tick(j, minor_tick_path, minor_grid_path);
      }
      draw_y_major_tick(y, major_tick_path, major_grid_path);
    }

    // Draw the ticks on the negative side.
    for(double y = 0; y >= y_axis_.min_; y -= y_ticks_.major_interval_)
    {
      // draw minor ticks
      for(double j = y;
        j > y - y_ticks_.major_interval_;
        j -= y_ticks_.major_interval_ / (y_ticks_.num_minor_ticks_ + 1.))
      {
        draw_y_minor_tick(j, minor_tick_path, minor_grid_path);
      }
      draw_y_major_tick(y, major_tick_path, major_grid_path);
    }
  } // void draw_y_axis()

  void draw_x_major_tick(double i, path_element& tick_path, text_element& series_info)
  { // Draw X axis major tick, and optional boxplot label.
    double x_left = i;
    double y_top = plot_bottom_; // Top of tick.
    if (x_ticks_.up_ticks_on_)
    { //
      y_top -= (std::max)(x_ticks_.major_tick_length_, x_ticks_.minor_tick_length_);
    }
    double y_bottom = plot_bottom_;
    if (x_ticks_.down_ticks_on_)
    { // Move down for tick.
      y_bottom += (std::max)(x_ticks_.major_tick_length_, x_ticks_.minor_tick_length_);
    }
    tick_path.M(x_left, y_top).L(x_left, y_bottom); // leaving current position at bottom or tick.
    if(x_axis_.label_on_) //    use_x_major_labels)
    {
      series_info.x(x_left); // Horizontal position.
      double y = y_bottom + series_info.style().font_size() * (text_margin_ * 0.7); // Leave a linespace above.
      series_info.y(y); // Vertical position.
      image.g(boxplot::VALUE_LABELS).push_back(new text_element(series_info));
    }
  } // void draw_x_major_tick

   void draw_y_major_tick(double value, path_element& tick_path, path_element& grid_path)
  { // Draw a Y axis major tick, tick value labels & horizontal grid.
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

    // Draw major ticks & tick value label, if necessary.
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
    tick_path.M(x_left, y).L(x_right, y); // Draw the major tick.
    // leaving x_left at the left most end of any tick,
    // and x_right at the rightmost end of any tick.
    // These may be on the axis line: y is the vertical tick position.

    if(y_ticks_.major_value_labels_side_ != 0)
    { // Label the tick with a value, for example "sin(x)"
      // (more likely to be a text string for boxplot, where 2D will be numeric tick value label).
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
    { // Sanity checks on svg coordinate.
      throw std::runtime_error("Y-tick X value wrong!");
    }
    if (y <= 0)
    { // Sanity checks on svg coordinate.
      throw std::runtime_error("Y-tick Y value wrong!");
    }
    if(y_ticks_.ticks_on_window_or_axis_ != 0)
      { // External to plot window style left or right.
        // Always want all values including "0", if labeling external to plot window.
        // y_ticks_.ticks_on_window_or_axis_ == true != 0
        image.g(boxplot::VALUE_LABELS).text(
          x,
          y,
          label.str(), y_value_label_style_, alignment, y_ticks_.label_rotation_);
      }
      else
      { // ! y_ticks_.y_ticks_on_plot_window_ == 0 'Internal' - value labels either side of vertical Y-axis.
        if ((value != 0) && y_axis_.axis_line_on_)
        { // Avoid a zero ON the Y-axis if it would be cut through by any horizontal X-axis line.
          image.g(boxplot::VALUE_LABELS).text(
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

  void draw_x_label()
  { // Draw an axis label (and optional units) for example "length (km)".
    // X-label color is set in constructor thus:
    // image.g(detail::PLOT_X_LABEL).style().stroke_color(black);
    // and changed using x_label_color(color);

    std::string label = x_label_info_.text(); // x_axis_ label, and optional units.
    if (x_axis_.label_units_on_ && (x_units_info_.text() != ""))
    { // Append the units, if any, user providing brackets () if required.
      label += x_units_info_.text();
    }

    double y = plot_bottom_;
    y += x_ticks_.value_label_style_.font_size() * 2.; // Shift down to suit ticks?
    if (x_ticks_.ticks_on_window_or_axis_ < 0) // bottom == -1
    { // Ticks & value labels below X-axis.
      if (x_ticks_.major_value_labels_side_ < 0) // - is bottom.
      { // Shift down to allow for any value labels.
        y += x_ticks_.label_max_space_;
      }
      if (x_ticks_.down_ticks_on_)
      { // Shift down for biggest of any ticks.
        y += (std::max)(x_ticks_.minor_tick_length_, x_ticks_.major_tick_length_);
      }
    }

    image.g(boxplot::X_LABEL).push_back(new text_element(
      ( // x position relative to the x-axis which is middle of plot window.
      plot_right_ + plot_left_) / 2,  // x coordinate - middle.
      y, // Down from plot window.
      label,
      x_label_info_.style(),
      center_align, horizontal)
      );
  } // void draw_x_label()

  void draw_y_label()
  { // Draw vertical y_axis label, and optional y units.
    // Y-label color is set in constructor thus:
    // image.g(boxplot::Y_LABEL).style().fill_color(black);
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

    image.g(boxplot::Y_LABEL).push_back(new
      text_element(x,
      (plot_bottom_ + plot_top_) / 2., // center on the plot window.
      label, // "Y-Axis" for example.
      y_axis_label_style_,
      center_align, // One might want it to left or right_align?
      upward)); // Y label must be drawn vertically.

  } // draw_y_label

  void draw_box(double q1, double q3, // Quartiles
  double x, double width,
  const svg_style& box_styl)
  {
    g_element& g_ptr = image.g(boxplot::MEDIAN).g();

    g_ptr.style().stroke_color(box_styl.stroke_color())
      .stroke_width(box_styl.stroke_width())
      .fill_color(box_styl.fill_color());
    transform_y(q1);
    transform_y(q3);
    double half_width = width/2;
    g_ptr.rect(x - half_width, q3, width, q1 - q3);
    g_ptr.clip_id(plot_window_clip_); // Clip elements.
  } //  void draw_box

  void draw_median(double median, double x_offset, double box_width,
    const svg_style& median_style, const value_style& values_style)
  { // Draw the median of the data series line within the box,
    // and optionally the median value.
    double median_y = median;
    transform_y(median_y); // SVG coordinate of median line.
    g_element& g_ptr = image.g(boxplot::MEDIAN).g();
    g_ptr.style().stroke_color(median_style.stroke_color())
      .stroke_width(median_style.stroke_width());

    double half_width = box_width / 2;
    g_ptr.line(x_offset - half_width, median_y, x_offset + half_width, median_y);
    //  void draw_plot_point_value(double x, double y, g_element& g_ptr, value_style& val_style, plot_point_style& point_style, double value)
    if (median_values_on_)
    { // Show the value of the median too.
      g_element& g_ptr_median = image.g(boxplot::DATA_VALUE_LABELS).g();
      draw_plot_point_value(x_offset + half_width, median_y, g_ptr_median, const_cast<value_style&>(values_style), mild_outlier_, median);
      // Share the mild_outlier point style?
      g_ptr_median.clip_id(plot_window_clip_);
      // TODO this doesn't have the desired effect???
    }
    // Clip elements.
    g_ptr.clip_id(plot_window_clip_);
  } // void draw_median

  void draw_whiskers(double min, double max, double length, double x,
    const svg_style& min_whisker, const svg_style& max_whisker,
    const svg_style& axis_whisker)
  { // Set up document structure for whiskers:
    g_element& g_whisk_ptr = image.g(boxplot::WHISKER).g();

    // Set colors for min and max whiskers.
    g_whisk_ptr.g().style()
      .stroke_color(min_whisker.stroke_color())
      .fill_color(min_whisker.fill_color())
      .stroke_width(min_whisker.stroke_width());

    g_whisk_ptr.g().style()
      .stroke_color(max_whisker.stroke_color())
      .fill_color(max_whisker.fill_color())
      .stroke_width(max_whisker.stroke_width());

    // Set axis structure and colors.
    g_element& g_axis_ptr = image.g(boxplot::BOX_AXIS).g();

    g_axis_ptr.style()
      .stroke_color(axis_whisker.stroke_color())
      .fill_color(axis_whisker.fill_color())
      .stroke_width(axis_whisker.stroke_width());

    transform_y(min);
    transform_y(max);

    double half_length = length / 2.;
    g_whisk_ptr.g(0).line(x - half_length, min, x + half_length, min);
    g_whisk_ptr.g(1).line(x - half_length, max, x + half_length, max);
    g_axis_ptr.line(x, min, x, max);

    // Clip elements.
    g_axis_ptr.clip_id(plot_window_clip_);
    g_whisk_ptr.clip_id(plot_window_clip_);
  } // void draw_whiskers()

  void draw_outliers(double x, const std::vector<double>& outliers,
    const std::vector<double>& extreme_outliers,
    const plot_point_style& mild_style, const plot_point_style& extreme_style,
    const value_style& values_style)
  { // Draw marker points for any mild and/or extreme outliers.
    g_element& g_mild_ptr = image.g(boxplot::MILD_OUTLIERS).g();
    g_mild_ptr.style().fill_color(mild_style.fill_color_)
      .stroke_color(mild_style.stroke_color_);

//  void draw_plot_point_value(double x, double y, g_element& g_ptr, value_style& sty, double value)
//  void draw_plot_point(double x, double y, g_element& g_ptr,  plot_point_style& sty)
//  using boost::svg::detail::axis_plot_frame<Derived>::draw_plot_point in axis_plot_frame.hpp.

    std::vector<double>::const_iterator i;
    double y;
    for(i = outliers.begin(); i != outliers.end(); ++i)
    { // Mild outliers.
      double value = *i;  // Save in case want to display value.
      if ((value <= y_axis_.max_) && (value >= y_axis_.min_))
      { // Inside plot window so want to show.
        y = value;
        transform_y(y); // to SVG.
        draw_plot_point(x, y, g_mild_ptr, const_cast<plot_point_style&>(mild_style)); // Kludge!
        if (outlier_values_on_)
        { // Show the value of the data point too.
          draw_plot_point_value(x, y, image.g(boxplot::DATA_VALUE_LABELS).g(), const_cast<value_style&>(values_style), mild_outlier_, value);
        }
      } // In window OK.
    }
    g_element& g_ext_ptr = image.g(boxplot::EXTREME_OUTLIERS).g();
    g_ext_ptr.style().fill_color(extreme_style.fill_color_)
      .stroke_color(extreme_style.stroke_color_);

    for(i = extreme_outliers.begin(); i != extreme_outliers.end(); ++i)
    { // Extreme outliers.
      double value = *i; // Save in case want to display value.
      if ((value <= y_axis_.max_) && (value >= y_axis_.min_))
      { // Inside plot window so want to show.
        y = value;
        transform_y(y); // to SVG.
        draw_plot_point(x, y, g_ext_ptr, const_cast<plot_point_style&>(extreme_style)); // Kludge!
        if (extreme_outlier_values_on_) // This isn't a series setting - but might be.
        { // Show the value of the data point too.
          draw_plot_point_value(x, y, image.g(boxplot::DATA_VALUE_LABELS).g(), const_cast<value_style&>(values_style), ext_outlier_, value);
        }
      } // in window.
    }

    // Clip elements to prevent any ouliers outside the plot window being shown.
    // But actually it may chop them in half!
    g_mild_ptr.clip_id(plot_window_clip_);
    g_ext_ptr.clip_id(plot_window_clip_);
  } // void draw_outliers

  void draw_boxplot(svg_boxplot_series& series, double x_offset)
  { // Draw a whole boxplot, box, median line, axis whiskers, and outliers.

    // const here causes trouble
     // Need to calculate quartiles here to permit individual plot quartile_definition.

    series.calculate_quantiles();

    draw_whiskers(series.whisker_min_, series.whisker_max_,
      series.whisker_length_, x_offset,
      series.min_whisker_style_, series.max_whisker_style_,
      series.axis_style_);

    draw_box(series.q1_, series.q3_, x_offset, series.box_width_,
      series.box_style_);

    draw_median(series.median_, x_offset,
      series.box_width_ - series.box_style_.stroke_width(),
      series.median_style_, series.values_style_);

    draw_outliers(x_offset, series.outliers_, series.extreme_outliers_,
      series.mild_outlier_, series.ext_outlier_, series.values_style_);
  } // void draw_boxplot

  void update_image()
  {
    clear_all();

    // Draw image background & border, if any.
    image.g(boxplot::PLOT_BACKGROUND).push_back(
      new rect_element(0, 0, image.x_size(), image.y_size()) );

    draw_title();
    calculate_plot_window();

    // Define the clip path for the plot window.
    // We don't want to allow too much overlap of the plot window lines,
    // so allow for the border.

    double margin = plot_window_border_.width_ * 5.; // Or more?
    // This controls how much points can overlap the plot window.
    // Might also make margin relative to data value font size and/or data marker size?
    image.clip_path(rect_element(plot_left_ - margin, // margin left
      plot_top_ - margin, // margin above top
      plot_right_ - plot_left_ + margin, // increased lengths for rect (not x, y)
      plot_bottom_ - plot_top_ + margin),
      plot_window_clip_); // clip id

    draw_x_axis(); // Must do X-axis first.
    draw_y_axis();

    if(x_axis_.label_on_)
    {
      draw_x_label();
    }
    if(y_axis_.label_on_)
    {
      draw_y_label();
    }
    for(unsigned int i=0; i < series.size(); ++i)
    { // Draw box'n'whiskers for each data series.
      draw_boxplot(series[i],
        plot_left_ + (plot_right_ - plot_left_) * ((double)(i + 1)) / (double)(series.size() + 1));
      // Spaced evenly across the plot window width.
      // Actually this leaves rather lot of space at the outside compared to spacing in middle?
    }
  } // void update_image()

  svg& get_svg()
  {
    update_image();
    return image;
  }

  // svg_boxplot& load_stylesheet(const std::string& file);  // Removed pending reimplementation of stylesheets.

  // Declarations of user boxplot functions.

  svg_boxplot& write(const std::string& file); //! Write SVG boxplot to file.
  svg_boxplot& write(std::ostream& s_out); //! Write SVG boxplot to ostream.
  svg_boxplot& title_on(bool cmd); //! true if to include title in plot.
  svg_boxplot& x_label_on(bool cmd);
  svg_boxplot& median_values_on(bool cmd);
  bool median_values_on();
  svg_boxplot& outlier_values_on(bool cmd);
  bool outlier_values_on();
  svg_boxplot& extreme_outlier_values_on(bool cmd);
  bool extreme_outlier_values_on();
  svg_boxplot& y_label_on(bool cmd);
  svg_boxplot& y_major_labels_on(int cmd);
  svg_boxplot& x_major_labels_on(int cmd);
  svg_boxplot& y_major_tick_color(const svg_color& col);
  svg_boxplot& x_tick_color(const svg_color& col);
  svg_boxplot& y_minor_tick_color(const svg_color& col);
  svg_boxplot& title_color(const svg_color& col);
  svg_boxplot& background_color(const svg_color& col);
  svg_boxplot& background_border_color(const svg_color& col);
  svg_boxplot& plot_background_color(const svg_color& col);
  svg_boxplot& plot_border_color(const svg_color& col);
  svg_boxplot& y_range(double min_y, double max_y);
  std::pair<double, double> y_range();
  svg_boxplot& y_label(const std::string& str);
  svg_boxplot& image_size(unsigned int x, unsigned int y);
  svg_boxplot& y_label_font_size(unsigned int size);
  svg_boxplot& y_label_color(const svg_color& col);
  svg_boxplot& title(const std::string& str);
  svg_boxplot& title_size(unsigned int size);
  svg_boxplot& x_label(const std::string& str);
  svg_boxplot& x_label_size(unsigned int size);
  svg_boxplot& x_label_color(const svg_color& col);
  svg_boxplot& y_major_interval(double inter);
  svg_boxplot& x_tick_length(unsigned int length);
  svg_boxplot& y_major_tick_length(unsigned int length);

  svg_boxplot& y_minor_tick_length(unsigned int length);
  svg_boxplot& y_num_minor_ticks(unsigned int num);
  svg_boxplot& x_tick_width(unsigned int width);
  svg_boxplot& y_major_tick_width(unsigned int width);
  svg_boxplot& y_minor_tick_width(unsigned int width);
  unsigned int image_x_size();
  unsigned int image_y_size();
  std::string title(); // title assigned to boxplot.
  bool x_label_on();
  int x_major_labels();
  // get color information
  svg_color title_color(); // Color of title text.
  svg_color background_color(); // Fill color of whole image background.
  svg_color background_border_color(); // Color of image border.
  svg_color plot_background_color();
  svg_color plot_border_color();
  svg_color x_label_color();
  svg_color x_tick_color();
  double x_tick_length();
  double x_major_tick_width();
  std::string x_label_text();
  std::string y_label_text();

  // Member Functions to control all box and whisker settings.

  svg_boxplot& whisker_length(double width);
  double whisker_length();
  svg_boxplot& box_width(double width); // Width of the box, not the border.
  double box_width(); // Width of the box, not the border.
  svg_boxplot& x_axis_position(int pos); // Position of the horizontal X-axis line (on the border).
  double x_axis_position(); // Position of the horizontal X-axis line (on the border).
  svg_boxplot& y_axis_position(int pos); // Position of the vertical Y-axis line (on the border).
  double y_axis_position(); // Position of the vertical Y-axis line (on the border).
  svg_boxplot& box_fill(const svg_color& color); // Color of box fill, not border.
  svg_color box_fill(); // Color of box fill, not border.
  svg_boxplot& box_border(const svg_color& color); // Color of border (not fill) to box.
  svg_color box_border(); // Color of border (not fill) to box.
  svg_boxplot& median_color(const svg_color& color); // Color of median line in box.
  svg_color median_color(); // Color of median line in box.
  svg_boxplot& median_width(double l); // Width of median line in box.
  double median_width(); //  Width of median line in box.
  svg_boxplot& axis_color(const svg_color& color);
  svg_color axis_color(); // Color of median line in box.
  svg_boxplot& axis_width(double l); // Width of vertical whisker axis line in box.
  double axis_width(); //  Width of vertical whisker axis line in box.

  plot_point_style& outlier_style();
  svg_boxplot& outlier_style(plot_point_style& os);

  svg_boxplot& outlier_color(const svg_color& color); // Color of outlier marker.
  svg_color outlier_color(); // Color of outlier marker.
  svg_boxplot& outlier_fill(const svg_color& color); // Fill color of outlier marker.
  svg_color outlier_fill(); // Fill color of outlier marker.
  svg_boxplot& extreme_outlier_color(const svg_color& color); // Color of extreme outlier marker.
  svg_color extreme_outlier_color(); // Color of extreme outlier marker.
  svg_boxplot& extreme_outlier_fill(const svg_color& color); // Fill color of extreme outlier marker.
  svg_color extreme_outlier_fill(); // Fill color of extreme outlier marker.

  svg_boxplot& outlier_shape(point_shape shape); // Shape of outlier marker.
  point_shape outlier_shape(); // Shape of outlier marker.

  svg_boxplot& outlier_size(int size); // Size of outlier marker.
  int outlier_size(); // Size of outlier marker.

  svg_boxplot& extreme_outlier_shape(point_shape shape); // Shape of extreme outlier marker.
  point_shape extreme_outlier_shape(); // Shape of extreme outlier marker.

  svg_boxplot& extreme_outlier_size(int size); // Size of extreme outlier marker.
  int extreme_outlier_size(); // Size of extreme outlier marker.

  svg_boxplot& quartile_definition(int def); // H&F quartile definition.
  int quartile_definition(); // H&F quartile definition.

  bool y_autoscale(); // If to use y_autoscaled values.
  svg_boxplot& y_autoscale(bool b); // If to use y_autoscaled values.

  svg_boxplot& y_autoscale(double first, double second);// Autoscale using two doubles.

  svg_boxplot& y_autoscale(std::pair<double, double> p); // Autoscale using minmax pair of doubles.

  template <class T> // T an STL container: array, vector ...
  svg_boxplot& y_autoscale(const T& begin, const T& end); // Autoscale using iterators into container.
  // explicit svg_boxplot::y_autoscale causes C2442 here, but omitting svg_boxplot:: is OK????

  template <class T> // T an STL container: array, vector ...
  svg_boxplot& y_autoscale(const T& container); //// Autoscale using whole container.

  // Plot member function to add a Data Series Boxplot.

  // Use overloaded functions to provide access to boxplot's non-static members values.
  // Version with NO default values.
  template <class T>
  svg_boxplot_series& plot(const T& container, // Add a data series to boxplot.
    const std::string& title,
    double bw,
    svg_style bs, // box_style_, // box width & style
    svg_style ms, // median marker style
    svg_style as, // axis style
    double wl, // whisker length
    svg_style minws, // min whisker style
    svg_style maxws, // max whisker style
    plot_point_style os, // mild outlier style
    plot_point_style extos, // extreme outlier style
    int q_def, // Quartile definition H&F #
    value_style vs,  // style for data values.
    text_style ss // series style - for series title.
    )
  {
    series.push_back( // Add a new data series for a boxplot.
      svg_boxplot_series(
        boost::make_transform_iterator(container.begin(), detail::boost_default_convert()),
        boost::make_transform_iterator(container.end(), detail::boost_default_convert()),
        title, bw, bs, ms, as,
        wl, minws, maxws,
        os, extos, q_def, vs, ss
        )
        // Get all styles etc from parent boxplot.
      );
      return series[series.size() - 1]; // Number of data series added so far.
  } // svg_boxplot_series& plot(const T& container, const std::string& title )

  // Version copying box'n'whiskers parameters from parent boxplot.
  template <class T>
  svg_boxplot_series& plot(const T& container, // Add a data series to boxplot.
    const std::string& title = "") // Warning given if this default is used.
  {
    if (title.size() == 0)
    {
      std::cout << "Data series has no title!" << std::endl;
    }
    series.push_back( // Add a new data series for a boxplot.
      svg_boxplot_series(
        boost::make_transform_iterator(container.begin(), detail::boost_default_convert()),
        boost::make_transform_iterator(container.end(), detail::boost_default_convert()),
        title, box_width_, box_style_, median_style_, axis_style_,
        whisker_length_, min_whisker_style_, max_whisker_style_,
        mild_outlier_, ext_outlier_, quartile_definition_, values_style_, series_style_
        )
        // Get all styles etc from parent boxplot.
      );
      return series[series.size() - 1]; // Reference to data series just added.
  } // svg_boxplot_series& plot(const T& container, const std::string& title)

}; // class svg_boxplot

// Definitions to match declarations above for separate file.

  //svg_boxplot& svg_boxplot::load_stylesheet(const std::string& file)
  //{ // removed until new implementation supports stylesheets.
  //  image.load_stylesheet(file);
  //  return *this;
  //}

  svg_boxplot& svg_boxplot::write(const std::string& file)
  {
    std::string filename(file); // Copy to avoid problems with const if need to append.
    if (filename.find(".svg") == std::string::npos)
    { // No file type suffix, so provide the default .svg.
      filename.append(".svg");
    }
    std::ofstream fout(filename.c_str());
    if(fout.fail())
    {
      throw std::runtime_error("Unable to open "+ filename);
    }

    image.image_filename(filename);
    // Note filename for optional output as comment in the .svg file.
    svg_boxplot::write(fout); // Use the ostream version.
    return *this;
  } // svg_boxplot& write(const std::string& file)

  svg_boxplot& svg_boxplot::write(std::ostream& s_out)
  { // ostream version used by filename version.
    update_image();
    image.write(s_out);
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_boxplot& svg_boxplot::title_on(bool cmd)
  {
    title_on_ = cmd;
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_boxplot& svg_boxplot::x_label_on(bool cmd)
  { // If Y axis name or label, for example: "length of thing".
      x_axis_.label_on_ = cmd;
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_boxplot& svg_boxplot::y_label_on(bool cmd)
  {
      y_axis_.label_on_ = cmd;
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_boxplot& svg_boxplot::y_major_labels_on(int cmd)
  { //< 0 means to left or down (default), 0 (false) means none, > 0 means to right (or top).
    y_ticks_.major_value_labels_side_ = cmd;
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_boxplot& svg_boxplot::x_major_labels_on(int cmd)
  { //< 0 means to left or down (default), 0 (false) means none, > 0 means to right (or top).
    x_ticks_.major_value_labels_side_ = cmd;
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_boxplot& svg_boxplot::median_values_on(bool cmd)
  {
    median_values_on_ = cmd;
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_boxplot& svg_boxplot::outlier_values_on(bool cmd)
  {
    outlier_values_on_ = cmd;
    return *this; // \return reference to svg_boxplot to make chainable.
  }
  svg_boxplot& svg_boxplot::extreme_outlier_values_on(bool cmd)
  {
    extreme_outlier_values_on_ = cmd;
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  bool svg_boxplot::median_values_on()
  {
    return outlier_values_on_;
  }

  bool svg_boxplot::outlier_values_on()
  {
    return outlier_values_on_;
  }
  bool svg_boxplot::extreme_outlier_values_on()
  {
    return extreme_outlier_values_on_;
  }

  svg_boxplot& svg_boxplot::y_major_tick_color(const svg_color& col)
  {
    image.g(boxplot::Y_MAJOR_TICKS).style().stroke_color(col);
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_boxplot& svg_boxplot::x_tick_color(const svg_color& col)
  {
    image.g(boxplot::X_TICKS).style().stroke_color(col);
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_boxplot& svg_boxplot::y_minor_tick_color(const svg_color& col)
  {
    image.g(detail::PLOT_Y_MINOR_TICKS).style().stroke_color(col);
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_boxplot& svg_boxplot::title_color(const svg_color& col)
  {
    image.g(boxplot::PLOT_TITLE).style().stroke_color(col);
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_boxplot& svg_boxplot::background_color(const svg_color& col)
  { // "imageBackground"
    image.g(boxplot::PLOT_BACKGROUND).style().fill_color(col);
    return *this;
  }
  svg_boxplot& svg_boxplot::background_border_color(const svg_color& col)
  { // "imageBackground"
    image.g(boxplot::PLOT_BACKGROUND).style().stroke_color(col);
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_boxplot& svg_boxplot::plot_background_color(const svg_color& col)
  { // Plot Window "plotBackground"
    image.g(boxplot::PLOT_WINDOW_BACKGROUND).style().fill_color(col);
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_boxplot& svg_boxplot::plot_border_color(const svg_color& col)
  { // Plot window "plotBackground"
    image.g(boxplot::PLOT_WINDOW_BACKGROUND).style().stroke_color(col);
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_boxplot& svg_boxplot::y_range(double min_y, double max_y)
  {
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
    return *this; // \return reference to svg_boxplot to make chainable.
  } // svg_boxplot& y_range(double y1, double y2)

  std::pair<double, double> svg_boxplot::y_range()
  { //! Set y_range using a pair of doubles.
    std::pair<double, double> r;
    r.first = y_axis_.min_;
    r.second = y_axis_.max_;
    return r;
  } //   std::pair<double, double> svg_boxplot::y_range()

  svg_boxplot& svg_boxplot::y_label(const std::string& str)
  {
    y_label_info_.text(str);
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_boxplot& svg_boxplot::image_size(unsigned int x, unsigned int y)
  {
    image.image_size(x, y);
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_boxplot& svg_boxplot::y_label_font_size(unsigned int size)
  {
     y_axis_label_style_.font_size(size);
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_boxplot& svg_boxplot::y_label_color(const svg_color& col)
  {
    image.g(boxplot::Y_LABEL).style().fill_color(col);
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_boxplot& svg_boxplot::title(const std::string& str)
  {
    title_info_.text(str);
    return *this; // \return reference to svg_boxplot to make chainable.
  }
  svg_boxplot& svg_boxplot::title_size(unsigned int size)
  {
    y_label_info_.style().font_size(size);
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_boxplot& svg_boxplot::x_axis_position(int pos)
  {  /*! Position of the horizontal X-axis line (on the border).
      But controlled by the intersection with Y-axis,
      so this only changes the default position from bottom to top,
      but will be changed if X-axis intersects the Y-axis
      (that is if Y-axis includes zero).
     */
     x_axis_position_ = pos; // top or bottom
    return *this;  // \return reference to svg_boxplot to make chainable.
  }

  double svg_boxplot::x_axis_position()
  {  // Position of the horizontal X-axis line (on the border).
    return x_axis_position_; // top or bottom
  }

  svg_boxplot& svg_boxplot::y_axis_position(int pos)
  { // Position of the vertical Y-axis line (on the border).
     y_axis_position_ = pos; // left or right
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  double svg_boxplot::y_axis_position()
  { // Position of the vertical Y-axis line (on the border).
    return y_axis_position_; // left or right
  }

  svg_boxplot& svg_boxplot::x_label(const std::string& str)
  {
    x_label_info_.text(str);
    return *this; // \return reference to svg_boxplot to make chainable.
  }
  svg_boxplot& svg_boxplot::x_label_size(unsigned int size)
  {
    x_label_info_.style().font_size(size);
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_boxplot& svg_boxplot::x_label_color(const svg_color& col)
  {
    image.g(boxplot::X_LABEL).style().fill_color(col);
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_boxplot& svg_boxplot::y_major_interval(double inter)
  {
    y_ticks_.major_interval_ = inter;
    return *this;
  }  svg_boxplot& svg_boxplot::x_tick_length(unsigned int length)
  {
    x_ticks_.major_tick_length_ = length;
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_boxplot& svg_boxplot::y_major_tick_length(unsigned int length)
  {
    y_ticks_.major_tick_length_ = length;
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_boxplot& svg_boxplot::y_minor_tick_length(unsigned int length)
  {
    y_ticks_.minor_tick_length_ = length;
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_boxplot& svg_boxplot::y_num_minor_ticks(unsigned int num)
  {
    y_ticks_.num_minor_ticks_ = num;
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_boxplot& svg_boxplot::x_tick_width(unsigned int width)
  {
    image.g(boxplot::X_TICKS).style().stroke_width(width);
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_boxplot& svg_boxplot::y_major_tick_width(unsigned int width)
  {
    image.g(boxplot::Y_MAJOR_TICKS).style().stroke_width(width);
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_boxplot& svg_boxplot::y_minor_tick_width(unsigned int width)
  {
    image.g(boxplot::Y_MINOR_TICKS).style().stroke_width(width);
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  unsigned int svg_boxplot::image_x_size()
  {
    return image.x_size();
  }

  unsigned int svg_boxplot::image_y_size()
  {
    return image.x_size();
  }

  std::string svg_boxplot::title()
  {
    return title_info_.text();
  }

  bool svg_boxplot::x_label_on()
  {
    return x_axis_.label_on_;
  }

  int svg_boxplot::x_major_labels()
  {
    return y_ticks_.major_value_labels_side_;
  }

  svg_color svg_boxplot::title_color()
  {
    return image.g(boxplot::PLOT_TITLE).style().fill_color();
  }

  svg_color svg_boxplot::background_color()
  {
    return image.g(boxplot::PLOT_BACKGROUND).style().fill_color();
  }

  svg_color svg_boxplot::background_border_color()
  {
    return image.g(boxplot::PLOT_BACKGROUND).style().stroke_color();
  }

  svg_color svg_boxplot::plot_background_color()
  {
    return image.g(boxplot::PLOT_WINDOW_BACKGROUND).style().fill_color();
  }

  svg_color svg_boxplot::plot_border_color()
  {
    return image.g(boxplot::PLOT_WINDOW_BACKGROUND).style().stroke_color();
  }

  svg_color svg_boxplot::x_label_color()
  {
    return image.g(boxplot::X_LABEL).style().fill_color();
  }

  svg_color svg_boxplot::x_tick_color()
  {
    return image.g(boxplot::X_TICKS).style().stroke_color();
  }

  double svg_boxplot::x_tick_length()
  {
    return x_ticks_.major_tick_length_;
  }

  double svg_boxplot::x_major_tick_width()
  {
    return image.g(boxplot::X_TICKS).style().stroke_width();
  }

  std::string svg_boxplot::x_label_text()
  {
    return x_label_info_.text();
  }

  std::string svg_boxplot::y_label_text()
  {
    return y_label_info_.text();
  }

  svg_boxplot& svg_boxplot::whisker_length(double width)
  {
    whisker_length_ = width;
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  double svg_boxplot::whisker_length()
  { // width of the box, not the margin.
    return whisker_length_;
  }

 svg_boxplot& svg_boxplot::box_width(double width)
  { // Width of the box, not the border.
    box_width_ = width;
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  double svg_boxplot::box_width()
  { // Width of the box, not the border.
    return box_width_;
  }

  svg_boxplot& svg_boxplot::box_fill(const svg_color& color)
  { // Color of box fill, not border.
    box_style_.fill_color(color);
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_color svg_boxplot::box_fill()
  { // Color of box fill, not border.
    return box_style_.fill_color();
  }

  svg_boxplot& svg_boxplot::box_border(const svg_color& color)
  { // Color of box fill, not border.
    box_style_.stroke_color(color);
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_color svg_boxplot::box_border()
  { // Color of box fill, not border.
    return box_style_.stroke_color();
  }

  svg_boxplot& svg_boxplot::median_color(const svg_color& color)
  { // Color of median line in box.
    median_style_.stroke_color(color);
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_color svg_boxplot::median_color()
  { // Color of median line in box.
    return median_style_.stroke_color();
  }

  svg_boxplot& svg_boxplot::median_width(double width)
  { // Width of the box, not the border.
    median_style_.stroke_width(width);
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  double svg_boxplot::median_width()
  { // Width of the box, not the border.
    return median_style_.stroke_width();
  }

  svg_boxplot& svg_boxplot::axis_color(const svg_color& color)
  { // Color of vertical whisker axis line in box.
    axis_style_.stroke_color(color);
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_color svg_boxplot::axis_color()
  { // Color of vertical whisker axis line in box.
    return axis_style_.stroke_color();
  }

  svg_boxplot& svg_boxplot::axis_width(double width)
  { // Width of the box, not the border.
    median_style_.stroke_width(width);
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  double svg_boxplot::axis_width()
  { // Width of the box, not the border.
    return median_style_.stroke_width();
  }

  svg_boxplot& svg_boxplot::outlier_color(const svg_color& color)
  { // Color of axis line in box.
    mild_outlier_.stroke_color(color);
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_color svg_boxplot::outlier_color()
  {  // Color of axis line in box.
    return mild_outlier_.stroke_color();
  }

  svg_boxplot& svg_boxplot::outlier_fill(const svg_color& color)
  { // Color of axis line in box.
    mild_outlier_.fill_color(color);
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_color svg_boxplot::outlier_fill()
  {  // Color of axis line in box.
    return mild_outlier_.fill_color();
  }

  svg_boxplot& svg_boxplot::extreme_outlier_color(const svg_color& color)
  { // Color of axis line in box.
    ext_outlier_.stroke_color(color);
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_color svg_boxplot::extreme_outlier_color()
  {  // Color of axis line in box.
    return ext_outlier_.stroke_color();
  }

  svg_boxplot& svg_boxplot::extreme_outlier_fill(const svg_color& color)
  { // Color of axis line in box.
    ext_outlier_.fill_color(color);
    return *this;  // \return reference to svg_boxplot to make chainable.
  }

  svg_color svg_boxplot::extreme_outlier_fill()
  {  // Color of axis line in box.
    return ext_outlier_.fill_color();
  }

  svg_boxplot& svg_boxplot::outlier_shape(point_shape shape)
  { // Shape of outlier marker.
    mild_outlier_.shape_ = shape;
    return *this;  // \return reference to svg_boxplot to make chainable.
  }

  point_shape svg_boxplot::outlier_shape()
  { // Color of outlier marker.
    return mild_outlier_.shape_;
  }

  svg_boxplot& svg_boxplot::extreme_outlier_shape(point_shape shape)
  { // Shape of extreme outlier marker.
    ext_outlier_.shape_ = shape;
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  point_shape svg_boxplot::extreme_outlier_shape()
  { // Shape of extreme outlier marker.
    return ext_outlier_.shape_ ;
  }

  svg_boxplot&  svg_boxplot::outlier_size(int size)
  { // Size of outlier marker.
    mild_outlier_.size(size);
    return *this;  // \return reference to svg_boxplot to make chainable.
  }

  int svg_boxplot::outlier_size()
  {
    return mild_outlier_.size();
  }
  // Size of outlier marker.

  svg_boxplot& svg_boxplot::extreme_outlier_size(int size)
  { // Size of extreme outlier marker.
    ext_outlier_.size(size);
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  int svg_boxplot::extreme_outlier_size()
  { // Size of extreme outlier marker.
    return ext_outlier_.size();
  }

  svg_boxplot& svg_boxplot::quartile_definition(int def)
  { //! Set size of extreme outlier marker.
    quartile_definition_ = def;
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  int svg_boxplot::quartile_definition()
  { // Size of extreme outlier marker.
    return quartile_definition_;
  }

  bool svg_boxplot::y_autoscale()
  {
    return y_autoscale_;
  }

  svg_boxplot& svg_boxplot::y_autoscale(bool b)
  {
    y_autoscale_ = b;
    return *this; // \return reference to svg_boxplot to make chainable.
  }

  svg_boxplot& svg_boxplot::y_autoscale(std::pair<double, double> p)
  { // Use Y min & max pair values to autoscale.
    scale_axis(p.first, p.second, // double min and max from pair.
    &y_auto_min_value_, &y_auto_max_value_, &y_auto_tick_interval_, &y_auto_ticks_,
    autoscale_check_limits_,
    y_include_zero_, y_tight_, y_min_ticks_, y_steps_);
    y_autoscale_ = true;  // Change (from default false) to use calculated values.
    return *this;  // \return reference to svg_boxplot to make chainable.
  } // autoscale(pair<double, double> p)

  svg_boxplot& svg_boxplot::y_autoscale(double first, double second)
  { // Use Y min & max pair values to autoscale.
    scale_axis(first, second, // double min and max from two doubles.
    &y_auto_min_value_, &y_auto_max_value_, &y_auto_tick_interval_, &y_auto_ticks_,
    autoscale_check_limits_,
    y_include_zero_, y_tight_, y_min_ticks_, y_steps_);
    y_autoscale_ = true;  // Change (from default false) to use calculated values.
    return *this;  // \return reference to svg_boxplot to make chainable.
  } // autoscale(pair<double, double> p)

  template <class T> // T an STL container: array<double>, vector<double> ...
  svg_boxplot& svg_boxplot::y_autoscale(const T& begin, const T& end) // Data series using iterators to
  { // to use to calculate autoscaled values.
    scale_axis(begin, end,
    &y_auto_min_value_, &y_auto_max_value_, &y_auto_tick_interval_, &y_auto_ticks_,
    autoscale_check_limits_,
    y_include_zero_, y_tight_, y_min_ticks_, y_steps_);
    y_autoscale_ = true; // Change (from default false) to use calculated values.
    return *this;  // \return reference to svg_boxplot to make chainable.
  } // y_autoscale(const T& begin, const T& end)

  template <class T> // T an STL container: array, vector ...
  svg_boxplot& svg_boxplot::y_autoscale(const T& container) // Whole data series.
  { // to use to calculate autoscaled values.
    scale_axis(container.begin(), container.end(), // All the container.
    &y_auto_min_value_, &y_auto_max_value_, &y_auto_tick_interval_, &y_auto_ticks_,
    autoscale_check_limits_,
    y_include_zero_, y_tight_, y_min_ticks_, y_steps_);
    y_autoscale_ = true;  // Change (from default false) to use calculated values.
    return *this; // \return reference to svg_boxplot to make chainable.
  } // y_autoscale(const T& container)


#if defined (BOOST_MSVC)
#  pragma warning(pop)
#endif

} // namespace svg
} // namespace boost
#endif // BOOST_SVG_SVG_BOXPLOT_HPP
