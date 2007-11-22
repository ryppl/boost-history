// svg_boxplot_detail.hpp

// Copyright Jacob Voytko 2007

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_BOXPLOT_DETAIL_HPP
#define BOOST_SVG_BOXPLOT_DETAIL_HPP

namespace boost{
namespace svg{
namespace boxplot{

enum boxplot_doc_structure
{
  BACKGROUND, PLOT_BACKGROUND, 
    Y_MAJOR_TICKS, Y_MINOR_TICKS,
    X_TICKS, PLOT_LABELS, Y_LABEL, X_LABEL,
    BOX_AXIS, BOX, MEDIAN, WHISKER, MILD_OUTLIERS, EXTREME_OUTLIERS,
    TITLE, BOXPLOT_DOC_CHILDREN
};

} // namespace boxplot
} // namespace svg
} // namespace boost

#endif // BOOST_SVG_BOXPLOT_DETAIL_HPP
