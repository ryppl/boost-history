// svg_plot_instruction.hpp

// Copyright (C) Jacob Voytko 2007
//
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

// ----------------------------------------------------------------- 

#ifndef _SVG_PLOT_INSTRUCTION_HPP
#define _SVG_PLOT_INSTRUCTION_HPP

#include <iterator>
#include "svg_style.hpp"

namespace boost {
namespace svg {

// -----------------------------------------------------------------
// This struct is the smallest amount of information necessary at
// the moment to display a legend with enough information
// -----------------------------------------------------------------
struct legend_point
{
    svg_color stroke_color;
    svg_color fill_color;
    std::string text;

    legend_point(const svg_color& _stroke, const svg_color& _fill,
        std::string _text): stroke_color(_stroke), fill_color(_fill),
        text(_text)
    {

    }

    legend_point(svg_color_constant _stroke, svg_color_constant _fill,
        std::string _text): stroke_color(constant_to_rgb(_stroke)), 
        fill_color(constant_to_rgb(_fill)),
        text(_text)
    {

    }
};
}
}

#endif
