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
// This allows us to process operations by case in switch statements
// -----------------------------------------------------------------
enum plot_inst_type{PLOT_NONE, PLOT_SCALE_X, PLOT_START, 
                PLOT_SIZE, PLOT_INTERVAL, PLOT_DRAW_AXIS, PLOT_LINE_COLOR,
                PLOT_RANGE};

// -----------------------------------------------------------------
// plot_instruction is the ancestor of all of the instructions
// that can be passed to a svg_plot
//
// TODO: the same for SVG
// -----------------------------------------------------------------
struct plot_instruction
{
    plot_inst_type i_type;

    plot_instruction()
    {
        i_type=PLOT_NONE;
    }

    plot_instruction(plot_inst_type _i):i_type(_i)
    {

    }
};

// -----------------------------------------------------------------
// plot_commands are the commands that answer yes/no questions,
// such as axes or no axes
// -----------------------------------------------------------------
struct plot_command: public plot_instruction
{
    plot_command(plot_inst_type _i):plot_instruction(_i)
    {
    
    }
};

// -----------------------------------------------------------------
// plot_single_vals are in response to commands that take a single
// argument, such as the starting position of plots
// -----------------------------------------------------------------
struct plot_single_val: public plot_instruction
{
    double x;
 
    plot_single_val(double _x, plot_inst_type _i): x(_x), plot_instruction(_i)
    {
    }
};

// -----------------------------------------------------------------
// plot_scale is to handle commands that take two double arguments
// -----------------------------------------------------------------
struct plot_two_vals: public plot_instruction
{
    double x1, x2;

    plot_two_vals(double _x1, double _x2, plot_inst_type _i)
        :x1(_x1), x2(_x2), plot_instruction(_i)
    {

    }
};


// -----------------------------------------------------------------
// the commands for using arguments that are based around four
// values
// -----------------------------------------------------------------
struct plot_four_vals: public plot_instruction
{
    double x1, y1, x2, y2;

    plot_four_vals(double _x1, double _y1, double _x2, double _y2, 
        plot_inst_type _i): x1(_x1), y1(_y1), x2(_x2), y2(_y2), 
                            plot_instruction(_i)
    {

    }
};

// -----------------------------------------------------------------
// For arguments of a color nature
// -----------------------------------------------------------------
struct plot_color: public plot_instruction
{
    svg_color col;

    plot_color(const svg_color& _col, plot_inst_type _i): col(_col), 
        plot_instruction(_i)
    {

    }
};

// -----------------------------------------------------------------
// 
// -----------------------------------------------------------------
struct plot_draw_range: public plot_instruction
{
    std::vector<double> data;
};

// -----------------------------------------------------------------
// fill_color defines the color of the points that are to be drawn
// -----------------------------------------------------------------
struct plot_draw_col_range: public plot_draw_range
{
    svg_color fill_color;

    plot_draw_col_range(svg_color_constant _fill_color): 
            fill_color(svg_color(_fill_color))
    {
 
    }
};

// -----------------------------------------------------------------
// The following functions return the values from the user
// interface to values that the svg_plot class can understand
// -----------------------------------------------------------------
plot_two_vals x_scale(double x1, double x2)
{
    return plot_two_vals(x1, x2, PLOT_SCALE_X);
}

plot_two_vals image_size(int x, int y)
{
    return plot_two_vals(x, y, PLOT_SIZE);
}

/*plot_draw_range plot_range(const Container<double, T>& begin,
                           const Container<double, T>& end)
{
    std::vector<double> hlal(begin, end);

    return plot_draw_range(hlal.begin(), hlal.end());
}*/

template <class container>
plot_draw_range plot_range(container begin, container end)
{
    plot_draw_range to_return;

    to_return.data.insert(to_return.data.begin(), begin, end);

    return to_return;
}

template <class container>
plot_draw_col_range plot_range(container begin,
                    container end,
                    svg_color_constant col)
{
    plot_draw_col_range to_return(col);

    to_return.data.insert(to_return.data.begin(), begin, end);

    return to_return;
}

plot_command draw_axis()
{
    return plot_command(PLOT_DRAW_AXIS);
}

plot_color line_color(svg_color c)
{
    return plot_color(c, PLOT_LINE_COLOR);
}

plot_color line_color(svg_color_constant c)
{
    return plot_color(constant_to_rgb(c), PLOT_LINE_COLOR);
}

}
}

#endif
