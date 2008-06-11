// svg_boxplot_detail.hpp

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2008

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_BOXPLOT_DETAIL_HPP
#define BOOST_SVG_BOXPLOT_DETAIL_HPP

// See also svg_style_detail.hpp for enum plot_doc_structure.
// Caution: these two enum and ids must match because
// the enum value is used to index the array of id strings.
// void set_ids() copies all strings to matching image.get_g_element(i).id()
// Add any new id items to both!

namespace boost{
namespace svg{
namespace boxplot
{
  enum boxplot_doc_structure
  {
    PLOT_BACKGROUND, // the whole svg image.
    PLOT_WINDOW_BACKGROUND, // the smaller plot window (if used).
    X_AXIS, Y_AXIS,
    X_TICKS, 
    Y_MAJOR_TICKS, Y_MINOR_TICKS,
    Y_MAJOR_GRID, Y_MINOR_GRID,
    VALUE_LABELS,
    Y_LABEL, X_LABEL,
    BOX_AXIS, BOX,
    MEDIAN, WHISKER, MILD_OUTLIERS, EXTREME_OUTLIERS,
    DATA_VALUE_LABELS, // 
    PLOT_TITLE,
    PLOT_NOTES, // Free text and shapes to annotate diagram.
    BOXPLOT_DOC_CHILDREN
  };

  std::string document_ids[] = // TODO change to document_ids_ because private member data.
  { // 
      "imageBackground", // the whole svg image.
      "plotBackground", // the smaller plot window (if used).
      "X-axis", "Y-axis",
      "xMajorTicks", // Probably not used for Boxplots.
      "yMajorTicks", "yMinorTicks",
      "YGrid", "yMinorGrid",
      "valueLabels", // tick Value Labels
      "yLabel", "xLabel",  // axis text labels "length (cm)"
      "boxAxis", "Box", // vertical Box axis line and box.
      "Median", "whisker", "mild_outliers", "extreme_outliers",
      "Data Values",
      "title",  // of the whole plot.
      "plotNotes", // Free text and shapes to annotate diagram.
      "boxplotDocChildren" // This last string is not used.
  }; //  std::string document_ids


} // namespace boxplot
} // namespace svg
} // namespace boost

#endif // BOOST_SVG_BOXPLOT_DETAIL_HPP
