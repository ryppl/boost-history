// svg_plot_instruction.hpp

// Copyright (C) Jacob Voytko 2007
//
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

// ----------------------------------------------------------------- 

#ifndef _SVG_PLOT_INSTRUCTION_HPP
#define _SVG_PLOT_INSTRUCTION_HPP

#include <iterator>

namespace boost {
namespace svg {


enum plot_inst_type{PLOT_SCALE_X, PLOT_SCALE_Y, PLOT_START, PLOT_SIZE,
					 PLOT_INTERVAL, PLOT_DRAW_AXIS};

struct plot_command
{
	plot_inst_type i_type;

	plot_command(plot_inst_type _i): i_type(_i)
	{

	}
};

struct plot_single_val
{
	double x;

	plot_inst_type i_type;

	plot_single_val(double _x, plot_inst_type _i): x(_x), i_type(_i)
	{
	}
};

struct plot_scale
{
	double x1, x2;

	plot_inst_type i_type;

	plot_scale(double _x1, double _x2, plot_inst_type _i)
		:x1(_x1), x2(_x2), i_type(_i)
	{

	}
};

struct plot_draw_range
{
	std::vector<double>::iterator begin;
	std::vector<double>::iterator end;

	plot_draw_range(std::vector<double>::iterator _b,
					 std::vector<double>::iterator _e)
	{
		begin = _b;
		end = _e;
	}
};


plot_scale x_scale(double x1, double x2)
{
	return plot_scale(x1, x2, PLOT_SCALE_X);
}

plot_scale y_scale(double y1, double y2)
{
	return plot_scale(y1, y2, PLOT_SCALE_Y);
}

plot_scale image_size(int x, int y)
{
	return plot_scale(x, y, PLOT_SIZE);
}

plot_single_val set_start(double x)
{
	return plot_single_val(x, PLOT_START);
}

plot_single_val set_interval(double x)
{
	return plot_single_val(x, PLOT_INTERVAL);
}

plot_draw_range plot_range(std::vector<double>::iterator begin,
					std::vector<double>::iterator end)
{
	return plot_draw_range(begin, end);
}

plot_command draw_axis()
{
	return plot_command(PLOT_DRAW_AXIS);
}

}
}
#endif
