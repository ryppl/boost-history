// svg_style.hpp

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2008

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
// to be used as a SVG group is thus: <g id="background" ... /g>

namespace boost
{
namespace svg
{
namespace detail
{

// Caution: these two enum and ids must match because
// the enum value is used to index the array of id strings.
// void set_ids() copies all strings to matching image.get_g_element(i).id()
// Add any new id items to both!
enum plot_doc_structure
{ // Order control the painting order, later ones overwriting earlier layers.
  PLOT_BACKGROUND = 0, // Must be zero to index array document_ids[]
    PLOT_WINDOW_BACKGROUND, // the smaller plot window (if used).
    PLOT_Y_MINOR_GRID, PLOT_Y_MAJOR_GRID, // 2, 3
    PLOT_X_MINOR_GRID, PLOT_X_MAJOR_GRID,
    PLOT_Y_AXIS, PLOT_X_AXIS, // the X and Y axis lines.
    PLOT_Y_MINOR_TICKS, PLOT_X_MINOR_TICKS,
    PLOT_Y_MAJOR_TICKS, PLOT_X_MAJOR_TICKS,
    PLOT_VALUE_LABELS, // tick values 10, 20, 30 ...
    PLOT_Y_LABEL, PLOT_X_LABEL, // axis text labels "length (cm)"
    PLOT_DATA_LINES, // lines joing data points.
    PLOT_DATA_POINTS, // normal data point markers.
    PLOT_LIMIT_POINTS, // at limit or NaN data point markers.
    PLOT_LEGEND_BACKGROUND, // legend box.
    PLOT_LEGEND_POINTS, // data series point markers, circle, cross...
    PLOT_LEGEND_TEXT, // text describing each data series.
    PLOT_TITLE, // of the whole plot.
    PLOT_X_POINT_VALUES,  PLOT_Y_POINT_VALUES, // Data point value labels.
    PLOT_FUNCTIONS, // Lines and curves, often to show a fit to the data.
    PLOT_NOTES, // Free text and shapes to annotate diagram.
    SVG_PLOT_DOC_CHILDREN // Last enum value used as count of children (22).
};

std::string document_ids[]= // TODO change to document_ids_ because private member data.
{ // 
    "imageBackground", // the whole svg image.
    "plotBackground", // // the smaller plot window (if used).
    "yMinorGrid", "yMajorGrid", 
    "xMinorGrid", "xMajorGrid",
    "yAxis", "xAxis", // the X and Y axis lines.
    "yMinorTicks", "xMinorTicks",
    "yMajorTicks", "xMajorTicks",
    "plotLabels", // TODO tickValueLabels better name???
    "yLabel", "xLabel",   // axis text labels "length (cm)"
    "plotLines", // normal data point markers.
    "plotPoints", // normal data point markers.
    "limitPoints",  // at limit or NaN data point markers
    "legendBackground", // legend box.
    "legendPoints", // data series point markers, circle, cross...
    "legendText", // text describing each data series.
    "title",  // of the whole plot.
    "plotXValues","plotYValues", // Data point value labels.
    "plotFunctions", // Lines and curves, often to show a fit to the data.
    "plotNotes", // Free text and shapes to annotate diagram.
    "plotDocChildren" // This last string is not used.
}; //  std::string document_ids
    
} // namespace detail
} // namespace svg
} // namespace boost
#endif // BOOST_SVG_SVG_STYLE_DETAIL_HPP
