// svg_style.hpp 
// Copyright (C) Jacob Voytko 2007
//
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

// ----------------------------------------------------------------- 

#ifndef _BOOST_SVG_SVG_STYLE_DETAIL_HPP
#define _BOOST_SVG_SVG_STYLE_DETAIL_HPP

#include <string>

namespace boost{
namespace svg{
namespace detail{

enum plot_doc_structure{PLOT_BACKGROUND, PLOT_PLOT_BACKGROUND, 
    PLOT_Y_MINOR_GRID, PLOT_Y_MAJOR_GRID, PLOT_X_MINOR_GRID, 
    PLOT_X_MAJOR_GRID, PLOT_Y_AXIS, PLOT_X_AXIS, 
    PLOT_Y_MINOR_TICKS, PLOT_X_MINOR_TICKS, PLOT_Y_MAJOR_TICKS, 
    PLOT_X_MAJOR_TICKS, PLOT_PLOT_LABELS, PLOT_Y_LABEL, PLOT_X_LABEL,
    PLOT_PLOT_LINES, PLOT_PLOT_POINTS, PLOT_LIMIT_POINTS,
    PLOT_LEGEND_BACKGROUND, PLOT_LEGEND_POINTS, PLOT_LEGEND_TEXT,
    PLOT_TITLE, SVG_PLOT_DOC_CHILDREN};

std::string _document_ids[]= 
{
    "background", "plotBackground", "yMinorGrid", "yMajorGrid", 
    "xMinorGrid", "xMajorGrid", "yAxis", "xAxis", "yMinorTicks",
    "xMinorTicks", "yMajorTicks", "xMajorTicks", "plotLabels",
    "yLabel", "xLabel", "plotLines", "plotPoints", "limitPoints",
    "legendBackground", "legendPoints", "legendText", "title"
};
    
}

}
}
#endif
