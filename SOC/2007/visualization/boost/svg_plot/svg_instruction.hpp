// svg_instruction.hpp

// Copyright (C) Jacob Voytko 2007
//
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

// ----------------------------------------------------------------- 

#ifndef _SVG_INSTRUCTION_HPP
#define _SVG_INSTRUCTION_HPP

namespace boost {
namespace svg {

enum instruction_type{SVG_POINT, SVG_WRITE, SVG_SIZE};

// -----------------------------------------------------------------
// The svg_instruction struct is what we are using to pass the svg
// class arguments through the << operator interface.
// -----------------------------------------------------------------
struct svg_instruction
{
    instruction_type i_type;

    svg_instruction(instruction_type i): i_type(i)
    {
    }
};

struct svg_point
{
    double x, y;

    instruction_type i_type;

    svg_point(double _x, double _y, instruction_type _it): x(_x), y(_y), i_type(_it)
    {

    }
};

struct svg_line
{
    double x1, y1, x2, y2;

    svg_line(double _x1, double _y1, double _x2, double _y2):
                x1(_x1), x2(_x2), y1(_y1), y2(_y2)
    {

    }
};

struct svg_text
{
    double x, y;
    std::string text;

    svg_text(double _x, double _y, std::string _t):x(_x), y(_y), text(_t)
    {

    }
};

struct svg_stroke_color
{
    svg_color col;

    svg_stroke_color(svg_color _c):col(_c)
    {

    }
};

// -----------------------------------------------------------------
// This allows the user to set the size of the image in centimeters
// TODO: allow other unit identifiers
// -----------------------------------------------------------------
svg_point image_size(unsigned int width, unsigned int height)
{
    if( (height <= 0) || (width <= 0) )
    {
        throw "Invalid image size";
    }
    
    return svg_point(width, height, SVG_SIZE);
}

svg_instruction write()
{
    return svg_instruction(SVG_WRITE);
}

svg_point draw_point(double x, double y)
{
    return svg_point(x, y, SVG_POINT);
}

svg_line draw_line(double x1, double y1, double x2, double y2)
{
    svg_line to_ret(x1, y1, x2, y2);

    return to_ret;
}

svg_text draw_text(double x, double y, 
                          const std::string& text)
{
    return svg_text(x, y, text);
}

svg_stroke_color stroke_color(svg_color c)
{
    return svg_stroke_color(c);
}

}
}

#endif
