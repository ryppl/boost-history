// svg_boxplot_detail.hpp 
// Copyright (C) Jacob Voytko 2007
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org
// ----------------------------------------------------------------- 

#ifndef _BOOST_SVG_BOXPLOT_DETAIL_HPP
#define _BOOST_SVG_BOXPLOT_DETAIL_HPP

namespace boost{
namespace svg{
namespace boxplot{

enum boxplot_doc_structure{BACKGROUND, PLOT_BACKGROUND, 
    Y_MAJOR_TICKS, Y_MINOR_TICKS,
    X_TICKS, PLOT_LABELS, Y_LABEL, X_LABEL,
    BOX_AXIS, BOX, MEDIAN, WHISKER, MILD_OUTLIERS, EXTREME_OUTLIERS,
    TITLE, BOXPLOT_DOC_CHILDREN};

}
}
}

#endif
