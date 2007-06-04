// svg_tag.hpp

// Copyright (C) Jacob Voytko 2007
//
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

// ----------------------------------------------------------------- 

#ifndef _SVG_STYLE_HPP
#define _SVG_STYLE_HPP

namespace boost {
namespace svg {

// -----------------------------------------------------------------
// Deals with well-defined colors
// -----------------------------------------------------------------
enum svg_color{BLACK, GRAY, RED};


// -----------------------------------------------------------------
// This is the style information for any <g> tag. 
// -----------------------------------------------------------------
class svg_g_style
{
private:
    svg_color fill_color;
    svg_color stroke_color;
    
public:
    svg_g_style();
    svg_g_style(const svg_color&, const svg_color&);
    void set_fill_color(const svg_color&);
    void set_stroke_color(const svg_color&);
};

// -----------------------------------------------------------------
// These are the defaults that I used in the prototype, and they
// looked decent enough.
// -----------------------------------------------------------------
svg_g_style::svg_g_style():fill_color(svg_color(RED)), stroke_color(svg_color(BLACK))
{

}

// -----------------------------------------------------------------
// For changing the defaults for the colors
// -----------------------------------------------------------------
svg_g_style::svg_g_style(const svg_color& _fill, const svg_color& _stroke)
:fill_color(_fill), stroke_color(_stroke)
{

}

}//svg
}//boost

#endif