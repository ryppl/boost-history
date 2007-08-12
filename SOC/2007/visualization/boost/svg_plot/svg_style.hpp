// svg_style.hpp 
// Copyright (C) Jacob Voytko 2007
//
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

// ----------------------------------------------------------------- 

#ifndef _BOOST_SVG_SVG_STYLE_HPP
#define _BOOST_SVG_SVG_STYLE_HPP

#include <ostream>
#include "svg_color.hpp"
#include "detail/svg_style_detail.hpp"

namespace boost {
namespace svg {

enum point_shape{none, circle, square, point};

struct plot_point_style
{
    point_shape shape;
    svg_color stroke_color;
    svg_color fill_color;
    int size;

    plot_point_style(const svg_color& _fill, 
        const svg_color& _stroke, int _size = 10, point_shape _shape = circle):
        fill_color(_fill), stroke_color(_stroke), size(_size), shape(_shape)
    {

    }
};

struct plot_line_style
{
    svg_color color;
    svg_color_constant area_fill;

    bool line_on;
    bool bezier_on;

    plot_line_style(const svg_color& _col, bool _on, bool _bezier_on = false):
        color(_col), line_on(_on), area_fill(blank), bezier_on(_bezier_on)
    {
    }
};

// -----------------------------------------------------------------
// This is the style information for any <g> tag. This will be
// expanded to include more data from the SVG standard when the
// time comes.
// -----------------------------------------------------------------
class svg_style
{
private:
    svg_color fill;
    svg_color stroke;

    unsigned int width;

    bool fill_on;
    bool stroke_on;
    bool width_on;
    
public:
    svg_style():fill(svg_color(0, 0, 0)), 
        stroke(svg_color(0, 0, 0)), width(0),
        fill_on(false), stroke_on(false), width_on(false)
    {

    }

    // getters
    svg_color fill_color()      const     { return svg_color(fill);   }
    svg_color stroke_color()    const     { return svg_color(stroke); }
    unsigned int stroke_width() const     { return width;             }

    svg_style(const svg_color& _fill, const svg_color& _stroke,
                     unsigned int _width = 0):
                     fill(_fill), stroke(_stroke), 
                     width(_width), fill_on(false), 
                     stroke_on(false), width_on(false)
    {

    }
    
    // setters
    svg_style& fill_color(const svg_color& _col)
    { 
        fill = _col;
        fill_on = true;
        return *this;
    }

    svg_style& stroke_color(const svg_color& _col) 
    { 
        stroke = _col;
        stroke_on = true;
        return *this;
    }

    svg_style& stroke_width(unsigned int _width)   
    { 
        width = _width;
        width_on = true;
        return *this;
    }
    
    void write(std::ostream& rhs)
    {
        if(stroke_on)
        {
            rhs << "stroke=\"";
            stroke.write(rhs);
            rhs << "\" ";
        }

        if(fill_on)
        {
            rhs << "fill=\"";
            fill.write(rhs);
            rhs<<"\" ";
        }
        if(width_on)
        {
            rhs << "stroke-width=\""
                << width
                << "\" ";
        }
    }
};

}//svg
}//boost


#endif
