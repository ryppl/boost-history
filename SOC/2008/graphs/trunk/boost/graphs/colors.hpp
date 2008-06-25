
#ifndef COLORS_HPP
#define COLORS_HPP

/**
 * Default color types for this library.
 */
enum color {
    white_color,
    gray_color,
    black_color,
    red_color,
    green_color,
    blue_color
};

/**
 * A traits class for colors. Specialize this if, for some reason, you ever
 * plan to specialize the notion of colors - which may be possible.
 *
 * @todo Obviously, this will be conceptized. See below.
 */
template <typename Color>
struct color_traits
{
    static color white()   { return white_color; }
    static color gray()    { return gray_color; }
    static color black()   { return black_color; }
    static color red()     { return red_color; }
    static color green()   { return green_color; }
    static color blue()    { return blue_color; }
};



#endif
