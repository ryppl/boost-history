// svg_style.hpp

// Copyright Jacob Voytko 2007

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// ----------------------------------------------------------------- 

#ifndef BOOST_SVG_SVG_STYLE_DETAIL_HPP
#define BOOST_SVG_SVG_STYLE_DETAIL_HPP

#include <string>
// using std::string;

// This module provides an id string from named parameter
// so, for example, document_ids[PLOT_BACKGROUND] == "background".

namespace boost
{
namespace svg
{
namespace detail
{

// Caution: these two enum and ids must match because
// the enum value is used to index the array of id strings.
//  void set_ids() copies all strings to matching image.get_g_element(i).id()
// Add any new id items to both!
enum plot_doc_structure
{
  PLOT_BACKGROUND = 0, // Must be zero to index array document_ids[]
    // TODO better PLOT_IMAGE_BACKGROUND ?
    PLOT_PLOT_BACKGROUND, // 1
    // TODO better PLOT_WINDOW_BACKGROUND ???  Maybe OK?
    PLOT_Y_MINOR_GRID, PLOT_Y_MAJOR_GRID, // 2, 3
    PLOT_X_MINOR_GRID, PLOT_X_MAJOR_GRID,
    PLOT_Y_AXIS, PLOT_X_AXIS, 
    PLOT_Y_MINOR_TICKS, PLOT_X_MINOR_TICKS,
    PLOT_Y_MAJOR_TICKS, PLOT_X_MAJOR_TICKS,
    PLOT_PLOT_LABELS, // tick values 10, 20, 30 ...
    PLOT_Y_LABEL, PLOT_X_LABEL,
    PLOT_PLOT_LINES, PLOT_PLOT_POINTS, PLOT_LIMIT_POINTS,
    PLOT_LEGEND_BACKGROUND, PLOT_LEGEND_POINTS, PLOT_LEGEND_TEXT,
    PLOT_TITLE,
    SVG_PLOT_DOC_CHILDREN // Last enum value used as count of children (22).
};

std::string document_ids[]= // TODO change to document_ids_ because private member data.
{ // 
    "background", // TODO "imageBackground" better?
    "plotBackground",
    "yMinorGrid", "yMajorGrid", 
    "xMinorGrid", "xMajorGrid",
    "yAxis", "xAxis",
    "yMinorTicks", "xMinorTicks",
    "yMajorTicks", "xMajorTicks",
    "plotLabels", // TODO tickValueLabels better name???
    "yLabel", "xLabel",
    "plotLines", "plotPoints", "limitPoints",
    "legendBackground", "legendPoints", "legendText",
    "title",
    "plotDocChildren" // This last string is not used.
}; //  std::string document_ids
    
} // namespace detail
} // namespace svg
} // namespace boost
#endif // BOOST_SVG_SVG_STYLE_DETAIL_HPP
