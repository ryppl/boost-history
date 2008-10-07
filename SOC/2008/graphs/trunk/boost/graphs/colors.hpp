
#ifndef BOOST_GRAPHS_COLORS_HPP
#define BOOST_GRAPHS_COLORS_HPP

namespace boost { namespace graphs {

// This seems a bit extensive for a simple enumeration of colors. However, it
// is certainly possible that somebody wants to use their own enumeration type
// as the colors for an algorithm. All they have to do is provide a
// specialization of the color_traits class in order to return the colors that
// they want.

/** Default color types for this library. */
enum color {
    white_color, gray_color, black_color,
    red_color, green_color, blue_color
};

/**
 * A traits class for colors. Specialize this if, for some reason, you ever
 * plan to specialize the notion of colors - which may be possible.
 * @todo This should be conceptized. See below.
 */
template <typename Color>
struct color_traits
{
    static color white()
    { return white_color; }

    static color gray()
    { return gray_color; }

    static color black()
    { return black_color; }

    static color red()
    { return red_color; }

    static color green()
    { return green_color; }

    static color blue()
    { return blue_color; }
};

} } /* namespace boost::graphs */

#endif
