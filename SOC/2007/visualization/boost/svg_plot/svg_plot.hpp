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

#include "svg.hpp"
#include "detail/svg_plot_instruction.hpp"


#include <boost/array.hpp>

namespace boost {
namespace svg {

// -----------------------------------------------------------------
// The following enums define the tree for the document. The reason
// that they are defined here is so that I can easily refer to them
// when I'm building data. It's easier to refer to:
//      document[SVG_PLOT_BACKGROUND].color()
//
// than other possibilities I can think of!
//
// "legend" is on top because either it'll be laid on top of the graph,
// or it'll be off to the side. 
// -----------------------------------------------------------------
#define SVG_PLOT_DOC_CHILDREN 4

enum svg_plot_doc_structure{SVG_PLOT_BACKGROUND, SVG_PLOT_PLOT, 
    SVG_PLOT_LEGEND, SVG_PLOT_TITLE};

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

    double y_axis;
    
    bool legend_on;
    std::vector<legend_point> legend;

    void _transform_point(double &x);
    void _clear_legend();
    void _draw_legend_header(int, int, int, int);
    void _draw_legend();

public:
    svg_plot();
    svg_plot(const std::string& file);
    
    svg_plot& x_scale(double, double);

    svg_plot& draw_axis();
    svg_plot& show_legend();
    
    svg_plot& line_color(const svg_color&);

    svg_plot& write(const std::string&);
    svg_plot& write(std::ostream&);

    svg_plot& image_size(unsigned int, unsigned int);

    svg_plot& set_title(const std::string&);
    svg_plot& set_title_font_size(unsigned int);

    svg_plot& set_background_color(svg_color_constant);
    svg_plot& set_background_color(const svg_color&);

    svg_plot& set_legend_background_color(svg_color_constant);
    svg_plot& set_legend_background_color(const svg_color&);

    svg_plot& set_axis_color(svg_color_constant);
    svg_plot& set_axis_color(const svg_color&);

    svg_plot& set_axis_thickness(unsigned int);

    void plot_range(std::vector<double>::const_iterator,
        std::vector<double>::const_iterator, const std::string&);
    
    void plot_range(std::vector<double>::const_iterator, 
        std::vector<double>::const_iterator, const std::string&, svg_color_constant);
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
svg_plot::svg_plot()
{
    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            transform_matrix[i][j] = 0;
        }
    }
    
    //to determine: reasonable default values
    image_size(100, 100);
    
    y_window_min = 0;
    y_window_max = 100;

    y_axis = (y_window_min + y_window_max) / 2.;

    x_min = -10;
    x_max = 10;

    legend_on = false;

    transform_matrix[0][0] = transform_matrix[2][2] = transform_matrix[1][1] = 1;

    transform_matrix[0][2] = x_size * (-x_min / (x_min - x_max));

    //build the document tree.. add children of the root node
    for(int i=0; i<SVG_PLOT_DOC_CHILDREN; ++i)
    {
        document.children.push_back(new g_element());
    }

    //add children of the plot node
    for(int i=0; i<SVG_PLOT_DOC_PLOT_CHILDREN; ++i)
    {
        (static_cast<g_element*>
            (&(document.children[SVG_PLOT_PLOT])))
                ->children.push_back(new g_element());
    }
    
    //add children of the legend node
    for(int i=0; i<SVG_PLOT_DOC_LEGEND_CHILDREN; ++i)
    {
        (static_cast<g_element*>
            (&(document.children[SVG_PLOT_LEGEND])))
                ->children.push_back(new g_element());
    }
}

// -----------------------------------------------------------------
// Set the scale for x values
// -----------------------------------------------------------------
svg_plot& svg_plot::x_scale(double x1, double x2)
{
    x_min = x1;
    x_max = x2;

    if(x2 <= x1)
    {
        throw "Illegal Argument: X scale: x2 < x1";
    }

    transform_matrix[0][0] = x_size/(x2-x1);
    transform_matrix[0][2] = x_size*(-x1 / (x2 - x1));

    return (svg_plot&)*this;
}

// -----------------------------------------------------------------
// set the size of the svg image produced
// -----------------------------------------------------------------
svg_plot& svg_plot::image_size(unsigned int x, unsigned int y)
{
    svg::image_size(x, y);
    
    y_window_max = y;

    y_axis = (y + y_window_min)/2.;

    transform_matrix[0][2] = x_size * (-x_min / (x_min - x_max) );

    return (svg_plot&)*this;
}

// -----------------------------------------------------------------
// Actually draw data to the plot. Default color information
// -----------------------------------------------------------------
void svg_plot::plot_range(std::vector<double>::const_iterator begin,
                            std::vector<double>::const_iterator end,
                            const std::string& _str)
{
    double x;

    double i=0;

    g_element* g_ptr = &(document.g_tag(SVG_PLOT_PLOT).
        g_tag(SVG_PLOT_PLOT_POINTS));

    g_ptr->children.push_back(new g_element);

    // this sets the current <g> element to the one that will contain
    // the data that is being pushed back.
    g_ptr = &(g_ptr->g_tag((int)(g_ptr->size())-1));

    g_ptr->set_fill_color(black);

    for(std::vector<double>::const_iterator b = begin; b!=end; ++b)
    {
        x = *b;
        _transform_point(x);
        point(x, y_axis, 
            *g_ptr);
    }

    // no matter what, we need to store information for the legend
    // so that it's easier to deal with when when turn it on after
    // we call this function =)
    legend.push_back(legend_point(g_ptr->get_stroke_color(), black, _str));
}

// -----------------------------------------------------------------
// Actually draw data to the plot. Fill color information provided
// -----------------------------------------------------------------
void svg_plot::plot_range(std::vector<double>::const_iterator begin,
                            std::vector<double>::const_iterator end,
                            const std::string& _str,
                            svg_color_constant _col)
{
    double x;

    double i=0;

    g_element* g_ptr = &(document.g_tag(SVG_PLOT_PLOT).
        g_tag(SVG_PLOT_PLOT_POINTS));

    g_ptr->children.push_back(new g_element);

    // this sets the current <g> element to the one that will contain
    // the data that is being pushed back.
    g_ptr = &(g_ptr->g_tag((int)(g_ptr->size())-1));

    g_ptr->set_fill_color(_col);

    for(std::vector<double>::const_iterator b = begin; b!=end; ++b)
    {
        x = *b;
        _transform_point(x);
        point(x, y_axis, 
            *g_ptr);
    }

    // no matter what, we need to store information for the legend
    // so that it's easier to deal with when when turn it on after
    // we call this function =)
    legend.push_back(legend_point(g_ptr->get_stroke_color(), _col, _str));
}

// -----------------------------------------------------------------
// Sets the title. Currently, the title is centered at the top of
// the screen
// -----------------------------------------------------------------
svg_plot& svg_plot::set_title(const std::string& _title)
{
    text_element title(x_size/2., 30, _title);

    title.set_alignment(center_align);

    document.g_tag(SVG_PLOT_TITLE)
        .children.push_back(new text_element(title));

    return (svg_plot&)*this;
}

// -----------------------------------------------------------------
// Setting the font size for the title before the text has been
// written causes a runtime error
// -----------------------------------------------------------------
svg_plot& svg_plot::set_title_font_size(unsigned int _size)
{
    text_element* t_ptr = static_cast<text_element*>(&(document.
                                    g_tag(SVG_PLOT_TITLE).children[0]));

    t_ptr->set_font_size(_size);

    return (svg_plot&)*this;
}

// -----------------------------------------------------------------
// Sets the background color in the area of the document. Specifically,
// done by adding a rectangle to the background that hits the edges
// of the image
// -----------------------------------------------------------------
svg_plot& svg_plot::set_background_color(svg_color_constant _col)
{
    set_background_color(constant_to_rgb(_col));

    return (svg_plot&)*this;
}

svg_plot& svg_plot::set_background_color(const svg_color& _col)
{
    document.g_tag(SVG_PLOT_BACKGROUND).set_fill_color(_col);

    document.g_tag(SVG_PLOT_BACKGROUND)
        .children.push_back(new rect_element(0, 0, x_size, y_size));

    return (svg_plot&)*this;
}

// -----------------------------------------------------------------
// Sets the background color in the area of the legend
// -----------------------------------------------------------------
svg_plot& svg_plot::set_legend_background_color(svg_color_constant _col)
{
    set_legend_background_color(constant_to_rgb(_col));

    return (svg_plot&)*this;
}

svg_plot& svg_plot::set_legend_background_color(const svg_color& _col)
{
    document.g_tag(SVG_PLOT_LEGEND).g_tag(SVG_PLOT_LEGEND_BACKGROUND)
            .set_fill_color(_col);

    return (svg_plot&)*this;
}

// -----------------------------------------------------------------
// Currently sets the color of both axes
// -----------------------------------------------------------------
svg_plot& svg_plot::set_axis_color(svg_color_constant _col)
{
    set_axis_color(constant_to_rgb(_col));

    return (svg_plot&)*this;
}

svg_plot& svg_plot::set_axis_color(const svg_color& _col)
{
    document.g_tag(SVG_PLOT_PLOT).g_tag(SVG_PLOT_PLOT_AXIS)
            .set_fill_color(_col);

    document.g_tag(SVG_PLOT_PLOT).g_tag(SVG_PLOT_PLOT_AXIS)
            .set_stroke_color(_col);

    return (svg_plot&)*this;
}

svg_plot& svg_plot::set_axis_thickness(unsigned int _width)
{
    document.g_tag(SVG_PLOT_PLOT).g_tag(SVG_PLOT_PLOT_AXIS)
            .set_stroke_width(_width);

    return (svg_plot&)*this;
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
svg_plot& svg_plot::draw_axis()
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

    line(x1, y_mean, x2, y_mean, 
        document.g_tag(SVG_PLOT_PLOT).g_tag(SVG_PLOT_PLOT_AXIS));

    //origin
    line(x_mean, y_window_min, x_mean, y_window_max, 
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

        line(x1, y1, x1, y2, 
            document.g_tag(SVG_PLOT_PLOT).g_tag(SVG_PLOT_PLOT_AXIS));
    }

    for(double i = 0; i >= x_min; i -= 1)
    {
        y1 = y_mean+5;
        y2 = y_mean-5;
    
        x1=i;

        _transform_point(x1);

        line(x1, y1, x1, y2,
            document.g_tag(SVG_PLOT_PLOT).g_tag(SVG_PLOT_PLOT_AXIS));
    }

    return (svg_plot&)*this;
}

svg_plot& svg_plot::show_legend()
{
    legend_on = true;

    return (svg_plot&)*this;
}

// -----------------------------------------------------------------
// Set the line color. This currently gets overwritten by the
// default for <g> in _plot_range, so this will be fixed, probably
// by removing the default for <g> in the constructor (ran out of
// time this week)
// -----------------------------------------------------------------
svg_plot& svg_plot::line_color(const svg_color& _col)
{
    document.g_tag(SVG_PLOT_PLOT).g_tag(SVG_PLOT_PLOT_AXIS)
        .set_stroke_color(_col);

    return (svg_plot&)*this;
}

// -----------------------------------------------------------------
// When writing to multiple documents, the contents of the plot
// may change significantly between. Rather than figuring out what
// has and has not changed, just erase the contents of the legend
// in the document and start over.
// -----------------------------------------------------------------
void svg_plot::_clear_legend()
{
    g_element* g_ptr = &(document.g_tag(SVG_PLOT_LEGEND)
                        .g_tag(SVG_PLOT_LEGEND_POINTS));

    g_ptr->children.erase(g_ptr->children.begin(), g_ptr->children.end());

    g_ptr = &(document.g_tag(SVG_PLOT_LEGEND).g_tag(SVG_PLOT_LEGEND_TEXT));
    g_ptr->children.erase(g_ptr->children.begin(), g_ptr->children.end());
}

// -----------------------------------------------------------------
// Factored out to make _draw_legend() cleaner
// -----------------------------------------------------------------
void svg_plot::_draw_legend_header(int _x, int _y, int _width, int _height)
{
    g_element* g_ptr = &(document.g_tag(SVG_PLOT_LEGEND)
                            .g_tag(SVG_PLOT_LEGEND_TEXT));

    text_element legend_header(_x+(_width/2), _y + 20, "Legend");

    legend_header.set_alignment(center_align);

    g_ptr->children.push_back(new text_element(legend_header));
}

// -----------------------------------------------------------------
// Important note: there are a lot of magic numbers that are temporary
// fill-ins for the time when the legend system is more configurable.
// This will happen bit-by-bit, as I give the user options to change
// these values
// -----------------------------------------------------------------
void svg_plot::_draw_legend()
{
    _clear_legend();

    int num_points = (int)(legend.size());

    int legend_width(200);
    int legend_height(25);

    // Figure out how wide the legend should be
    if(x_size < 200)
    {
       legend_width = x_size; 
    }

    int legend_x_start(x_size-legend_width-20);
    int legend_y_start(40);

    // legend_height = title_spacing + (space per element)(num_elements)
    //                  + (end spacing)
    legend_height = 25 + (25 * num_points) + 10;

    // TODO: Figure out how tall the legend should be

    g_element* g_ptr = &(document.g_tag(SVG_PLOT_LEGEND)
                         .g_tag(SVG_PLOT_LEGEND_BACKGROUND));

    g_ptr->children.push_back(new rect_element(legend_x_start, legend_y_start,
                                               legend_width, legend_height));

    _draw_legend_header(legend_x_start, legend_y_start, legend_width, legend_height);

    g_ptr = &(document.g_tag(SVG_PLOT_LEGEND)
                         .g_tag(SVG_PLOT_LEGEND_POINTS));

    g_element* g_inner_ptr;
    int i=0;

    for(std::vector<legend_point>::iterator iter = legend.begin(); 
        iter!=legend.end(); ++iter, ++i)
    {
        g_ptr -> children.push_back( new g_element() );
        g_inner_ptr = &(g_ptr->g_tag((int)(g_ptr->children.size())-1));
        
        g_inner_ptr->set_fill_color((*iter).fill_color);
        g_inner_ptr->set_stroke_color((*iter).stroke_color);

        g_inner_ptr->children.push_back(new point_element(legend_x_start + 25,
                                        legend_y_start + 36 + i*25));

        g_inner_ptr->children.push_back(new text_element(legend_x_start + 40,
                                        legend_y_start + 42 + i*25, (*iter).text));
    }
}

svg_plot& svg_plot::write(const std::string& _str)
{
    // Hold off drawing the legend until the very end.. it's
    // easier to draw the size that it needs at the end than
    // it is to 
    // Don't bother with re-adding things if we don't need to
    if(legend_on)
    {
        _draw_legend();
    }

    svg::write(_str);

    return (svg_plot&)*this;
}

svg_plot& svg_plot::write(std::ostream& s_out)
{
    if(legend_on)
    {
        _draw_legend();
    }
    
    svg::write(s_out);
    
    return (svg_plot&)*this;
}

template <class iter>
void plot_range(svg_plot& _cont, iter _begin, iter _end, std::string _str)
{
    vector<double> vect(_begin, _end);

    _cont.plot_range(vect.begin(), vect.end(), _str);
}

template <class iter>
void plot_range(svg_plot& _cont, iter _begin, iter _end, std::string _str,
 	                    svg_color_constant _col)
{
    vector<double> vect(_begin, _end);

    _cont.plot_range(vect.begin(), vect.end(), _str, _col);
}

}
}

#endif

