// svg_plot.hpp

// Copyright (C) Jacob Voytko 2007
//
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

// ----------------------------------------------------------------- 

#ifndef _SVG_PLOT_HPP
#define _SVG_PLOT_HPP


#include <vector>
#include <ostream>
#include <sstream>
#include <iterator>
#include <limits>

#include "svg_plot_instruction.hpp"
#include "svg.hpp"

namespace boost {
namespace svg {

class svg_plot: public svg
{
private:
	double transform_matrix[3][3];
	double plot_start, plot_interval;

	double x_min, x_max, y_min, y_max;
	
	//Don't let the user use this without specifying a stream.
	//I can't think of a reasonable default, and I don't think
	//they'd want the svg formatted output spit to the console
	svg_plot();

	void _x_scale(double, double);
	void _y_scale(double, double);
	void _plot_image_size(unsigned int, unsigned int);
	void _set_start(double);
	void _set_interval(double);
	void _plot_range(std::vector<double>::iterator,
				std::vector<double>::iterator);
	
	void _transform_point(double &x, double &y);
	void _draw_axis();
	void _set_stroke_color(svg_color);

public:
	using svg::operator<<;

	svg_plot(const std::string& file);
	void operator<<(const plot_single_val&);
	void operator<<(const plot_scale&);
	void operator<<(const plot_draw_range&);
	void operator<<(const plot_command&);
};

// -----------------------------------------------------------------
// Constructors will be added so that the user can specify
// a stream instead of a filename
//
// This performs work for default values so that if the user starts
// adding data right away, we won't get abnormal results
// -----------------------------------------------------------------
svg_plot::svg_plot(const std::string& file):svg(file)
{
	for(int i=0; i<3; ++i)
		for(int j=0; j<3; ++j)
			transform_matrix[i][j] = 0;
	
	_size(100, 100);
	x_min = y_min = 0;
	x_max = y_max = 10;

	transform_matrix[0][0] = transform_matrix[2][2] = transform_matrix[1][1] = 1;

	transform_matrix[0][2] = x_size*(-x_min / (x_min - x_max));
	transform_matrix[1][2] = y_size - y_size*(-y_min / (y_min - y_max));

	//to determine: reasonable default values
	

	plot_interval = 1;
	plot_start = 1;
}

void svg_plot::operator<<(const plot_scale& rhs)
{
	switch(rhs.i_type)
	{
	case PLOT_SCALE_X:
		_check_size();
		_x_scale(rhs.x1, rhs.x2);
		break;

	case PLOT_SCALE_Y:
		_check_size();
		_y_scale(rhs.x1, rhs.x2);
		break;
	
	case PLOT_SIZE:
		_plot_image_size((unsigned int)rhs.x1, (unsigned int)rhs.x2);
		break;
	}
}

void svg_plot::operator <<(const plot_single_val& rhs)
{
	switch(rhs.i_type)
	{
	case PLOT_INTERVAL:
		_set_interval(rhs.x);
		break;

	case PLOT_START:
		_set_start(rhs.x);
		break;
	}
}

void svg_plot::operator<<(const plot_command& rhs)
{
	_draw_axis();
}

void svg_plot::operator<<(const plot_draw_range& rhs)
{
	_plot_range(rhs.begin, rhs.end);
}

void svg_plot::_x_scale(double x1, double x2)
{
	x_min = x1;
	x_max = x2;

	if(x2 <= x1)
		throw "Illegal Argument: X scale: x2 < x1";

	transform_matrix[0][0] = x_size/(x2-x1);
	transform_matrix[0][2] = x_size*(-x1 / (x2 - x1));
}

void svg_plot::_y_scale(double y1, double y2)
{
	y_min = y1;
	y_max = y2;

	if(y2 <= y1)
		throw "Illegal Argument: Y scale: y2 < y1";

	transform_matrix[1][1] = -1. * y_size/(y2-y1);
	transform_matrix[1][2] = y_size - y_size*(-y1 / (y2 - y1));
}

void svg_plot::_plot_image_size(unsigned int x, unsigned int y)
{
	_size(x, y);
	
	transform_matrix[0][2] = x_size*(-x_min / (x_min - x_max));
	transform_matrix[1][2] = y_size - y_size*(-y_min / (y_min - y_max));
}

// -----------------------------------------------------------------
// The user needs to set the start value in order for the first
// point of the plot to render in the right place
// -----------------------------------------------------------------
void svg_plot::_set_start(double x)
{
	plot_start = x;
}

// -----------------------------------------------------------------
// The user needs to set the interval to make each subsequent
// node of the plot render in the right place
// -----------------------------------------------------------------
void svg_plot::_set_interval(double x)
{
	plot_interval = x;
}

void svg_plot::_plot_range(std::vector<double>::iterator begin,
							std::vector<double>::iterator end)
{
	double x, y, x2, y2;

	double i=0;

	if(begin+1 == end)
	{
		x = plot_start;
		y = *begin;

		_transform_point(x, y);

		_point(x, y);
	}
	else
	{
		for(std::vector<double>::iterator b = begin; b+1!=end; ++b)
		{
			x = plot_start+i;
			y = *b;
		
			x2 = plot_start+(i+1);
			y2 = *(b+1);

			_transform_point(x, y);
			_transform_point(x2, y2);

			_line(x, y, x2, y2);

			i+=plot_interval;
		}
	
		i=0;

		for(std::vector<double>::iterator b = begin; b!=end; ++b)
		{
			x = plot_start+i;
			y = *b;

			_transform_point(x, y);
			_point(x, y);
			i+=plot_interval;
		}
	}
}

// -----------------------------------------------------------------
// This transforms a Cartesian point into a svg point. We don't
// use the svg coordinate transform because sizing is a harder
// problem in the svg coordinate transforms, and text would be
// flipped if were were to mimic the Cartesian system
// -----------------------------------------------------------------
void svg_plot::_transform_point(double &x, double &y)
{
	x = transform_matrix[0][0]*x+transform_matrix[0][2];
	y = (transform_matrix[1][1]*y+transform_matrix[1][2]);
}


// -----------------------------------------------------------------
// TODO: Refactor
// -----------------------------------------------------------------
void svg_plot::_draw_axis()
{
	//two major axes
	double x1,y1,x2,y2;
	
	x1 = x2 = 0;
	y1 = y_min;
	y2 = y_max;

	_transform_point(x1,y1);
	_transform_point(x2,y2);

	_line(x1, y1, x2, y2);

	y1 = y2 = 0;
	x1 = x_min;
	x2 = x_max;

	_transform_point(x1,y1);
	_transform_point(x2,y2);

	_line(x1, y1, x2, y2);

	for(double i=x_min; i<=x_max; i+=plot_interval)
	{
		y1 = .2;
		y2 = -.2;
	
		x1=x2=i;

		_transform_point(x1,y1);
		_transform_point(x2,y2);

		_line(x1,y1,x2,y2);
	}

	for(double i=y_min; i<=y_max; i+=plot_interval)
	{
		x1 = .2;
		x2 = -.2;
	
		y1=y2=i;

		_transform_point(x1,y1);
		_transform_point(x2,y2);

		_line(x1,y1,x2,y2);
	}
}

}
}

#endif

