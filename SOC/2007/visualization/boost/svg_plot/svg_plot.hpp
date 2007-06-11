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

// -----------------------------------------------------------------
// The following enums define the tree for the document. The reason
// that they are defined here is so that I can easily refer to them
// when I'm building data. It's easier to refer to:
//      document[SVG_PLOT_BACKGROUND].color()
//
// than it is to do it any other way I can think of.
// -----------------------------------------------------------------
#define SVG_PLOT_DOC_CHILDREN 3

enum svg_plot_doc_structure{SVG_PLOT_BACKGROUND, SVG_PLOT_LEGEND,
    SVG_PLOT_PLOT};

// -----------------------------------------------------------------
// The following enums define the children for the legend. This will be
// incomplete, as the actual definition of the legend is fuzzy.
// -----------------------------------------------------------------

#define SVG_PLOT_DOC_LEGEND_CHILDREN 3

enum svg_plot_doc_legend_structure{SVG_PLOT_LEGEND_BACKGROUND,
    SVG_PLOT_LEGEND_POINTS, SVG_PLOT_LEGEND_TEXT};

// -----------------------------------------------------------------
// The following enums define the tree for the plot itself.
// -----------------------------------------------------------------

#define SVG_PLOT_DOC_PLOT_CHILDREN 5

enum svg_plot_doc_plot_structure{SVG_PLOT_PLOT_BACKGROUND, SVG_PLOT_PLOT_AXIS, 
    SVG_PLOT_PLOT_LABELS, SVG_PLOT_PLOT_LINES, SVG_PLOT_PLOT_POINTS};



class svg_plot: public svg
{
private:
    double transform_matrix[3][3];

    double x_min, x_max;

    //Still keep track of this for when we have a view window
    //for the graph
    double y_window_min, y_window_max; 
    
    //Don't let the user use this without specifying a stream.
    //I can't think of a reasonable default, and I don't think
    //they'd want the svg formatted output spit to the console
    svg_plot();

    void _x_scale(double, double);
    void _plot_image_size(unsigned int, unsigned int);

    void _plot_range(std::vector<double>::iterator,
        std::vector<double>::iterator);
    void _plot_range(std::vector<double>::iterator,
        std::vector<double>::iterator,
        svg_color);

    void _transform_point(double &x);
    void _draw_axis();
    void _line_color(const svg_color&);

public:
    using svg::operator<<;

    svg_plot(const std::string& file);
    svg_plot& operator<<(const plot_single_val&);
    svg_plot& operator<<(const plot_two_vals&);
    svg_plot& operator<<(const plot_draw_range&);
    svg_plot& operator<<(const plot_draw_col_range&);
    svg_plot& operator<<(const plot_command&);
    svg_plot& operator<<(const plot_color&);
};

// -----------------------------------------------------------------
// Constructors will be added so that the user can specify
// a stream instead of a filename
//
// This performs work for default values so that if the user starts
// adding data right away, we won't get abnormal results
//
// Note: All of the points are going to be on the same line, so
// there is no sense calculating any aspect of the y value
// -----------------------------------------------------------------
svg_plot::svg_plot(const std::string& file): svg(file)
{
    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            transform_matrix[i][j] = 0;
        }
    }
    
    //to determine: reasonable default values
    _size(100, 100);
    
    y_window_min = 0;
    y_window_max = 100;

    x_min = -10;
    x_max = 10;

    transform_matrix[0][0] = transform_matrix[2][2] = transform_matrix[1][1] = 1;

    transform_matrix[0][2] = x_size * (-x_min / (x_min - x_max));

    //build the document tree
    for(int i=0; i<SVG_PLOT_DOC_CHILDREN; ++i)
    {
        document.children.push_back(new g_element());
    }

    for(int i=0; i<SVG_PLOT_DOC_PLOT_CHILDREN; ++i)
    {
        (static_cast<g_element*>
            (&(document.children[SVG_PLOT_PLOT])))
                ->children.push_back(new g_element());
    }
    
}

// -----------------------------------------------------------------
// The chained stream operators. Each overload below deals with a
// different instruction type by cases.
// -----------------------------------------------------------------

svg_plot& svg_plot::operator<<(const plot_two_vals& rhs)
{
    switch(rhs.i_type)
    {
    case PLOT_SCALE_X:
        _x_scale(rhs.x1, rhs.x2);
        break;
    
    case PLOT_SIZE:
        _plot_image_size((unsigned int)rhs.x1, (unsigned int)rhs.x2);
        break;
    }

    return (svg_plot&)*this;
}

svg_plot& svg_plot::operator <<(const plot_single_val& rhs)
{
/*    switch(rhs.i_type)
    {
    default:
        break;
    }
*/
    return (svg_plot&)*this;
}

svg_plot& svg_plot::operator<<(const plot_command& rhs)
{
    _draw_axis();
    return (svg_plot&)*this;
}

svg_plot& svg_plot::operator<<(const plot_draw_range& rhs)
{
    _plot_range(rhs.begin, rhs.end);
    return (svg_plot&)*this;
}

svg_plot& svg_plot::operator<<(const plot_draw_col_range& rhs)
{
    _plot_range(rhs.begin, rhs.end, rhs.fill_color);
    return (svg_plot&)*this;
}

svg_plot& svg_plot::operator<<(const plot_color& rhs)
{
    switch(rhs.i_type)
    {
    case PLOT_LINE_COLOR:
        _line_color(rhs.col);
        break;
    }

    return (svg_plot&)*this;
}

// -----------------------------------------------------------------
// Set the scale for x values
// -----------------------------------------------------------------
void svg_plot::_x_scale(double x1, double x2)
{
    x_min = x1;
    x_max = x2;

    if(x2 <= x1)
    {
        throw "Illegal Argument: X scale: x2 < x1";
    }

    transform_matrix[0][0] = x_size/(x2-x1);
    transform_matrix[0][2] = x_size*(-x1 / (x2 - x1));
}

// -----------------------------------------------------------------
// set the size of the svg image produced
// -----------------------------------------------------------------
void svg_plot::_plot_image_size(unsigned int x, unsigned int y)
{
    _size(x, y);
    
    y_window_max = y;

    transform_matrix[0][2] = x_size * (-x_min / (x_min - x_max) );
}

// -----------------------------------------------------------------
// Actually draw data to the plot. Default color information
// -----------------------------------------------------------------
void svg_plot::_plot_range(std::vector<double>::iterator begin,
                            std::vector<double>::iterator end)
{
    double x;

    double i=0;

    double y_point = (y_window_min + y_window_max) / 2.;
    
    for(std::vector<double>::iterator b = begin; b!=end; ++b)
    {
        x = *b;
        _transform_point(x);
        _point(x, y_point, 
            document.g_tag(SVG_PLOT_PLOT).g_tag(SVG_PLOT_PLOT_POINTS));
    }
}

// -----------------------------------------------------------------
// Actually draw data to the plot. Fill color information provided
// -----------------------------------------------------------------
void svg_plot::_plot_range(std::vector<double>::iterator begin,
                            std::vector<double>::iterator end,
                            svg_color _col)
{
    double x;

    double i=0;

    double y_point = (y_window_min + y_window_max) / 2.;

    g_element* g_ptr = &(document.g_tag(SVG_PLOT_PLOT).
        g_tag(SVG_PLOT_PLOT_POINTS));

    g_ptr->children.push_back(new g_element);

    // this sets the current <g> element to the one that will contain
    // the data that is being pushed back.
    g_ptr = &(g_ptr->g_tag((int)(g_ptr->size())-1));

    g_ptr->set_fill_color(_col);

    for(std::vector<double>::iterator b = begin; b!=end; ++b)
    {
        x = *b;
        _transform_point(x);
        _point(x, y_point, 
            *g_ptr);
    }
}
// -----------------------------------------------------------------
// This transforms a 1-D Cartesian point into a svg point. We don't
// use the svg-defined coordinate transform because sizing is a harder
// problem in the svg coordinate transforms, and text would be
// flipped if were were to mimic the Cartesian system
// -----------------------------------------------------------------
void svg_plot::_transform_point(double &x)
{
    x = transform_matrix[0][0] * x + transform_matrix[0][2];
}


// -----------------------------------------------------------------
// TODO: Refactor
// -----------------------------------------------------------------
void svg_plot::_draw_axis()
{
    // one major axis. We just need to draw a verticle line through
    // the origin for now. We will make that an option later.

    double x1, x2, y1, y2;
    
    x1 = x_min;
    x2 = x_max;

    _transform_point(x2);
    _transform_point(x1);

    double y_mean = (y_window_min + y_window_max)/2.;
    double x_mean = (x1 + x2)/2.;

    _line(x1, y_mean, x2, y_mean, 
        document.g_tag(SVG_PLOT_PLOT).g_tag(SVG_PLOT_PLOT_AXIS));

    //origin
    _line(x_mean, y_window_min, x_mean, y_window_max, 
        document.g_tag(SVG_PLOT_PLOT).g_tag(SVG_PLOT_PLOT_AXIS));

    y1 = y2 = 0;
    x1 = x_min;
    x2 = x_max;

    for(double i = 0; i <= x_max; i += 1)
    {
        y1 = y_mean+5;
        y2 = y_mean-5;
    
        x1=i;

        _transform_point(x1);
        _transform_point(x2);

        _line(x1, y1, x1, y2, 
            document.g_tag(SVG_PLOT_PLOT).g_tag(SVG_PLOT_PLOT_AXIS));
    }

    for(double i = 0; i >= x_min; i -= 1)
    {
        y1 = y_mean+5;
        y2 = y_mean-5;
    
        x1=i;

        _transform_point(x1);

        _line(x1, y1, x1, y2,
            document.g_tag(SVG_PLOT_PLOT).g_tag(SVG_PLOT_PLOT_AXIS));
    }
}

// -----------------------------------------------------------------
// Set the line color. This currently gets overwritten by the
// default for <g> in _plot_range, so this will be fixed, probably
// by removing the default for <g> in the constructor (ran out of
// time this week)
// -----------------------------------------------------------------
void svg_plot::_line_color(const svg_color& _col)
{
    document.g_tag(SVG_PLOT_PLOT).g_tag(SVG_PLOT_PLOT_AXIS)
        .set_stroke_color(_col);
}

}
}

#endif

