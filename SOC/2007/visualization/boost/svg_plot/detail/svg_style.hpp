// svg_style.hpp 
// Copyright (C) Jacob Voytko 2007
//
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

// ----------------------------------------------------------------- 

#ifndef _SVG_STYLE_HPP
#define _SVG_STYLE_HPP

#include <map>

namespace boost {
namespace svg {

// -----------------------------------------------------------------
// Deals with colors that have special names. The reason that the
// underscoring does not match the normal Boost format
// is that these are the names that are specifically allowed by the
// SVG standard
// -----------------------------------------------------------------
enum svg_color_constant
{
    aliceblue, antiquewhite, aqua, aquamarine, azure, beige, 
    bisque, black, blanchedalmond, blue, blueviolet, brown, 
    burlywood, cadetblue, chartreuse, chocolate, coral, 
    cornflowerblue, cornsilk, crimson, cyan, darkblue, darkcyan, 
    darkgoldenrod, darkgray, darkgreen, darkgrey, darkkhaki, 
    darkmagenta, darkolivegreen, darkorange, darkorchid, darkred, 
    darksalmon, darkseagreen, darkslateblue, darkslategray, 
    darkslategrey, darkturquoise, darkviolet, deeppink, 
    deepskyblue, dimgray, dimgrey, dodgerblue, firebrick, 
    floralwhite, forestgreen, fuchsia, gainsboro, ghostwhite, gold, 
    goldenrod, gray, grey, green, greenyellow, honeydew, hotpink, 
    indianred, indigo, ivory, khaki, lavender, lavenderblush, 
    lawngreen, lemonchiffon, lightblue, lightcoral, lightcyan, 
    lightgoldenrodyellow, lightgray, lightgreen, lightgrey, 
    lightpink, lightsalmon, lightseagreen, lightskyblue, 
    lightslategray, lightslategrey, lightsteelblue, lightyellow, 
    lime, limegreen, linen, magenta, maroon, mediumaquamarine, 
    mediumblue, mediumorchid, mediumpurple, mediumseagreen, 
    mediumslateblue, mediumspringgreen, mediumturquoise, 
    mediumvioletred, midnightblue, mintcream, mistyrose, moccasin, 
    navajowhite, navy, oldlace, olive, olivedrab, orange, 
    orangered, orchid, palegoldenrod, palegreen, paleturquoise, 
    palevioletred, papayawhip, peachpuff, peru, pink, plum, 
    powderblue, purple, red, rosybrown, royalblue, saddlebrown, 
    salmon, sandybrown, seagreen, seashell, sienna, silver, 
    skyblue, slateblue, slategray, slategrey, snow, springgreen, 
    steelblue, tan, teal, thistle, tomato, turquoise, violet, 
    wheat, white, whitesmoke, yellow, yellowgreen
};


void constant_to_rgb(svg_color_constant _c, unsigned char &r, unsigned char &g, 
            unsigned char &b);

// -----------------------------------------------------------------
// svg_color is the struct that contains information about sRGB
// colors.
//
// For the constructor: the svg standard specifies that numbers
// outside the normal rgb range are to be accepted, but are rounded
// to acceptable values.
// -----------------------------------------------------------------
struct svg_color
{
    unsigned char r, g, b;

    svg_color(int _r, int _g, int _b)
    {
        _r = ( _r < 0 ) ? 0 : _r;
        _g = ( _g < 0 ) ? 0 : _g;
        _b = ( _b < 0 ) ? 0 : _b;

        r = (unsigned char)(( _r > 255 ) ? 255 : _r);
        g = (unsigned char)(( _g > 255 ) ? 255 : _g);
        b = (unsigned char)(( _b > 255 ) ? 255 : _b);
    }

    svg_color(svg_color_constant _col)
    {
        constant_to_rgb(_col, r, g, b);
    }

    void write(std::ostream& rhs)
    {
        rhs << "rgb(" << (unsigned int)r << "," << (unsigned int) g << ","
            << (unsigned int)b << ")" ;
    }

    bool operator==(const svg_color& rhs)
    {
        if(r == rhs.r && g == rhs.g && b == rhs.b)
            return true;

        return false;
    }
};

// -----------------------------------------------------------------
// To facilitate quick lookup of the RGB values of constants
// -----------------------------------------------------------------
svg_color color_array[] =
{
    svg_color(240, 248, 255), // aliceblue
    svg_color(250, 235, 215), // antiquewhite
    svg_color(0  , 255, 255), // aqua
    svg_color(127, 255, 212), // aquamarine
    svg_color(240, 255, 255), // azure
    svg_color(245, 245, 220), // beige
    svg_color(255, 228, 196), // bisque
    svg_color(0  , 0  , 0  ), // black
    svg_color(255, 235, 205), // blanchedalmond. Who thinks of these?
    svg_color(0  , 0  , 255), // blue
    svg_color(138, 43 , 226), // blueviolet
    svg_color(165, 42 , 42 ), // brown
    svg_color(222, 184, 135), // burlywood
    svg_color(95 , 158, 160), // cadetblue
    svg_color(127, 255, 0  ), // chartreuse
    svg_color(210, 105, 30 ), // chocolate
    svg_color(255, 127, 80 ), // coral
    svg_color(100, 149, 237), // cornflowerblue
    svg_color(255, 248, 220), // cornsilk
    svg_color(220, 20 , 60 ), // crimson
    svg_color(0  , 255, 255), // cyan
    svg_color(0  , 0  , 139), // darkblue
    svg_color(0  , 139, 139), // darkcyan
    svg_color(184, 134, 11 ), // darkgoldenrod
    svg_color(169, 169, 169), // darkgray
    svg_color(0  , 100, 0  ), // darkgreen
    svg_color(169, 169, 169), // darkgrey
    svg_color(189, 183, 107), // darkkhaki
    svg_color(139, 0  , 139), // darkmagenta
    svg_color(85 , 107, 47 ), // darkolivegreen
    svg_color(255, 140, 0  ), // darkorange
    svg_color(153, 50 , 204), // darkorchid
    svg_color(139, 0  , 0  ), // darkred
    svg_color(233, 150, 122), // darksalmon
    svg_color(143, 188, 143), // darkseagreen
    svg_color(72 , 61 , 139), // darkslateblue
    svg_color(47 , 79 , 79 ), // darkslategray
    svg_color(47 , 79 , 79 ), // darkslategrey
    svg_color(0  , 206, 209), // darkturquoise
    svg_color(148, 0  , 211), // darkviolet
    svg_color(255, 20 , 147), // deeppink
    svg_color(0  , 191, 255), // deepskyblue
    svg_color(105, 105, 105), // dimgray
    svg_color(105, 105, 105), // dimgrey
    svg_color(30 , 144, 255), // dodgerblue
    svg_color(178, 34 , 34 ), // firebrick
    svg_color(255, 250, 240), // floralwhite
    svg_color(34 , 139, 34 ), // forestgreen
    svg_color(255, 0  , 255), // fuchsia
    svg_color(220, 220, 220), // gainsboro
    svg_color(248, 248, 255), // ghostwhite
    svg_color(255, 215, 0  ), // gold
    svg_color(218, 165, 32 ), // goldenrod
    svg_color(128, 128, 128), // gray
    svg_color(128, 128, 128), // grey
    svg_color(0  , 128, 0  ), // green
    svg_color(173, 255, 47 ), // greenyellow
    svg_color(240, 255, 240), // honeydew
    svg_color(255, 105, 180), // hotpink
    svg_color(205, 92 , 92 ), // indianred
    svg_color(75 , 0  , 130), // indigo
    svg_color(255, 255, 240), // ivory
    svg_color(240, 230, 140), // khaki
    svg_color(230, 230, 250), // lavender
    svg_color(255, 240, 245), // lavenderblush
    svg_color(124, 252, 0  ), // lawngreen
    svg_color(255, 250, 205), // lemonchiffon
    svg_color(173, 216, 230), // lightblue
    svg_color(240, 128, 128), // lightcoral
    svg_color(224, 255, 255), // lightcyan
    svg_color(250, 250, 210), // lightgoldenrodyellow
    svg_color(211, 211, 211), // lightgray
    svg_color(144, 238, 144), // lightgreen
    svg_color(211, 211, 211), // lightgrey
    svg_color(255, 182, 193), // lightpink
    svg_color(255, 160, 122), // lightsalmon
    svg_color(32 , 178, 170), // lightseagreen
    svg_color(135, 206, 250), // lightskyblue
    svg_color(119, 136, 153), // lightslategray
    svg_color(119, 136, 153), // lightslategrey
    svg_color(176, 196, 222), // lightsteelblue
    svg_color(255, 255, 224), // lightyellow
    svg_color(0  , 255, 0  ), // lime
    svg_color(50 , 205, 50 ), // limegreen
    svg_color(250, 240, 230), // linen
    svg_color(255, 0  , 255), // magenta
    svg_color(128, 0  , 0  ), // maroon
    svg_color(102, 205, 170), // mediumaquamarine
    svg_color(0  , 0  , 205), // mediumblue
    svg_color(186, 85 , 211), // mediumorchid
    svg_color(147, 112, 219), // mediumpurple
    svg_color(60 , 179, 113), // mediumseagreen
    svg_color(123, 104, 238), // mediumslateblue
    svg_color(0  , 250, 154), // mediumspringgreen
    svg_color(72 , 209, 204), // mediumturquoise
    svg_color(199, 21 , 133), // mediumvioletred
    svg_color(25 , 25 , 112), // midnightblue
    svg_color(245, 255, 250), // mintcream
    svg_color(255, 228, 225), // mistyrose
    svg_color(255, 228, 181), // moccasin
    svg_color(255, 222, 173), // navajowhite
    svg_color(0  , 0  , 128), // navy
    svg_color(253, 245, 230), // oldlace
    svg_color(128, 128, 0  ), // olive
    svg_color(107, 142, 35 ), // olivedrab
    svg_color(255, 165, 0  ), // orange
    svg_color(255, 69 , 0  ), // orangered
    svg_color(218, 112, 214), // orchid
    svg_color(238, 232, 170), // palegoldenrod
    svg_color(152, 251, 152), // palegreen
    svg_color(175, 238, 238), // paleturquose
    svg_color(219, 112, 147), // palevioletred
    svg_color(255, 239, 213), // papayawhip
    svg_color(255, 218, 185), // peachpuff
    svg_color(205, 133, 63 ), // peru
    svg_color(255, 192, 203), // pink
    svg_color(221, 160, 221), // plum
    svg_color(176, 224, 230), // powderblue
    svg_color(128, 0  , 128), // purple
    svg_color(255, 0  , 0  ), // red
    svg_color(188, 143, 143), // rosybrown
    svg_color(65 , 105, 225), // royalblue
    svg_color(139, 69 , 19 ), // saddlebrown
    svg_color(250, 128, 114), // salmon
    svg_color(244, 164, 96 ), // sandybrown
    svg_color(46 , 139, 87 ), // seagreen
    svg_color(255, 245, 238), // seashell
    svg_color(160, 82 , 45 ), // sienna
    svg_color(192, 192, 192), // silver
    svg_color(135, 206, 235), // skyblue
    svg_color(106, 90 , 205), // slateblue
    svg_color(112, 128, 144), // slategray
    svg_color(112, 128, 144), // slategrey
    svg_color(255, 250, 250), // snow
    svg_color(0  , 255, 127), // springgreen
    svg_color(70 , 130, 180), // steelblue
    svg_color(210, 180, 140), // tan
    svg_color(0  , 128, 128), // teal
    svg_color(216, 191, 216), // thistle
    svg_color(255, 99 , 71 ), // tomato
    svg_color(64 , 224, 208), // turquoise
    svg_color(238, 130, 238), // violet
    svg_color(245, 222, 179), // wheat
    svg_color(255, 255, 255), // white
    svg_color(245, 245, 245), // whitesmoke
    svg_color(255, 255, 0  ), // yellow
    svg_color(154, 205, 50 ), // yellowgreen
};


void constant_to_rgb(svg_color_constant _c, unsigned char &r, unsigned char &g, 
            unsigned char &b)
{
    r = color_array[(unsigned int)_c].r;
    g = color_array[(unsigned int)_c].g;
    b = color_array[(unsigned int)_c].b;
}

svg_color constant_to_rgb(svg_color_constant _c)
{
    return svg_color(color_array[(unsigned int)_c].r
        ,color_array[(unsigned int)_c].g
        ,color_array[(unsigned int)_c].b);
}

// -----------------------------------------------------------------
// This is the style information for any <g> tag. This will be
// expanded to include more data from the SVG standard when the
// time comes.
// -----------------------------------------------------------------
class svg_style
{
private:
    svg_color fill_color;
    svg_color stroke_color;

    unsigned int stroke_width;
    
public:
    svg_style();
    svg_style(const svg_color&, const svg_color&);

    void set_fill_color(const svg_color&);
    void set_stroke_color(const svg_color&);
    
    void set_stroke_width(unsigned int);
    void write(std::ostream&);

    svg_color get_fill_color();
    svg_color get_stroke_color();

    unsigned int get_stroke_width();
};

// -----------------------------------------------------------------
// Black seems to me to be as good a default as any
// -----------------------------------------------------------------
svg_style::svg_style():fill_color(svg_color(0, 0, 0)), 
    stroke_color(svg_color(0, 0, 0)), stroke_width(0)
{

}


// -----------------------------------------------------------------
// For changing the defaults for the colors
// -----------------------------------------------------------------
svg_style::svg_style(const svg_color& _fill, const svg_color& _stroke)
:fill_color(_fill), stroke_color(_stroke), stroke_width(0)
{

}

void svg_style::set_stroke_color(const svg_color& rhs)
{
    stroke_color = rhs;
}

void svg_style::set_fill_color(const svg_color& rhs)
{
    fill_color = rhs;
}

void svg_style::set_stroke_width(unsigned int _width)
{
    stroke_width = _width;
}

svg_color svg_style::get_fill_color()
{
    return svg_color(fill_color);
}

svg_color svg_style::get_stroke_color()
{
    return svg_color(stroke_color);
}

unsigned int svg_style::get_stroke_width()
{
    return stroke_width;
}

void svg_style::write(std::ostream& rhs)
{
    rhs << "stroke=\"";
    stroke_color.write(rhs);
    rhs << "\" fill=\"";
    fill_color.write(rhs);
    rhs<<"\" ";

    if(stroke_width > 0)
    {
        rhs << "stroke-width=\""
            << stroke_width
            << "\" ";
    }
}

}//svg
}//boost


#endif
