// svg_1d_plot.hpp

// Copyright (C) Jacob Voytko 2007
//
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

// ----------------------------------------------------------------- 

#ifndef _SVG_1D_PLOT_HPP
#define _SVG_1D_PLOT_HPP

#include <vector>
#include <ostream>
#include <sstream>
#include <iterator>

#define BOOST_PARAMETER_MAX_ARITY 7

#include <boost/parameter/preprocessor.hpp>
#include <boost/parameter/name.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/bind.hpp>

#include "svg.hpp"
#include "detail/svg_plot_instruction.hpp"


namespace boost {
namespace svg {

// -----------------------------------------------------------------
// This is the base level of the "tree" for the SVG document. It is 
// flattened in order to facilitate using other image formats in the 
// future that don't have the benefit of a document tree. It also
// greatly simplifies the logic of the methods below
// -----------------------------------------------------------------
enum plot_doc_structure{PLOT_BACKGROUND, PLOT_PLOT_BACKGROUND, 
    PLOT_X_MINOR_GRID, PLOT_X_MAJOR_GRID, PLOT_Y_AXIS, PLOT_X_AXIS, 
    PLOT_Y_MINOR_TICKS, PLOT_X_MINOR_TICKS, PLOT_Y_MAJOR_TICKS, 
    PLOT_X_MAJOR_TICKS, PLOT_PLOT_LABELS, PLOT_PLOT_LINES, PLOT_PLOT_POINTS, 
    PLOT_LEGEND_BACKGROUND, PLOT_LEGEND_POINTS, PLOT_LEGEND_TEXT,
    PLOT_Y_LABEL, PLOT_X_LABEL, PLOT_TITLE};

#define SVG_PLOT_DOC_CHILDREN 19

// there will be more!
enum point_shape{circle};

struct plot_point_style
{
    point_shape shape;
    svg_color stroke_color;
    svg_color fill_color;
    int size;

    plot_point_style(const svg_color& _fill, 
        const svg_color& _stroke):
        fill_color(_fill), stroke_color(_stroke)
    {

    }
};

class boost_default_convert
{
public:
    typedef double result_type;

    double operator()(double a) const
    {
        return a;
    }
};

// -----------------------------------------------------------------
// This allows us to store plot state locally in svg_plot. We don't
// store it in "svg" because transforming the points after they are
// written to the document would be difficult. We store the Cartesian
// coordinates locally and transform them before we write them.
// -----------------------------------------------------------------
struct svg_plot_series
{
    std::vector<double> series;
    std::string title;
    plot_point_style style;

    svg_plot_series(std::vector<double> _ctr,
                    const std::string& _title,
                    const plot_point_style& _style):
                    series(_ctr),
                    title(_title),
                    style(_style)
    {

    }
};

class svg_1d_plot
{
protected:

    //todo: replace with x_scale, x_shift, since I don't use the full matrix
    double x_scale, x_shift;
    double y_scale, y_shift;

    // stored so as to avoid rewriting style information constantly
    svg image;

    // border information for the plot window. Initially will be set to the width
    // and height of the graph
    int plot_window_x1;
    int plot_window_x2; 
    int plot_window_y1; 
    int plot_window_y2;

    void _transform_point(double &x, double &y);
    void _transform_x(double &x);
    void _transform_y(double &y);
    
    // used for text displayed on the graph
    std::string  x_label, title;

    // axis information. y_axis stored as one point because this is a 1D graph
    double       x_min,  x_max;
    double       x_axis;
    
    double       x_major_tick;

    unsigned int x_major_tick_length,  x_major_tick_width,
                 x_minor_tick_length, x_minor_tick_width;
    unsigned int x_num_minor_ticks;
    unsigned int legend_title_font_size, title_font_size, 
                 x_label_font_size;

    // Yes/no questions
    bool x_major_labels_on;
    bool x_major_grid_on;
    bool x_minor_grid_on;
    bool x_label_on;

    bool title_on;
    bool legend_on;
    bool axis_on;
    bool plot_window_on;

    // internal helper functions
    void _clear_all();
    void _clear_legend();
    void _clear_x_axis();
    void _clear_y_axis();
    void _clear_background();
    void _clear_title();
    void _clear_points();
    void _clear_plot_background();
    void _clear_grids();

    void _draw_x_axis();
    void _draw_x_minor_ticks(double);
    void _draw_x_major_ticks(double);
    void _draw_x_label();
    void _draw_legend_header(int, int, int);
    void _draw_legend();
    void _draw_axis();

private:
    // where we will be storing the data points for transformation
    std::vector<svg_plot_series> series;

public:
    // constructors
    svg_1d_plot();

    svg_1d_plot(const svg_1d_plot&);
    svg_1d_plot& operator=(const svg_1d_plot&);

    // output
    svg_1d_plot& write(const std::string&);
    svg_1d_plot& write(std::ostream&);

    // plot functions
    void svg_1d_plot::plot(const std::vector<double>&,
                            const std::string&,
                            const plot_point_style&);

    //setters

    // misc
    svg_1d_plot& set_image_size(unsigned int, unsigned int);
    svg_1d_plot& set_title(const std::string&);
    svg_1d_plot& set_title_font_size(unsigned int);
    svg_1d_plot& set_legend_title_font_size(unsigned int);

    // commands
    svg_1d_plot& set_x_label_on(bool);
    svg_1d_plot& set_x_major_labels_on(bool);
    svg_1d_plot& set_x_major_grid_on(bool);
    svg_1d_plot& set_x_minor_grid_on(bool);

    svg_1d_plot& set_axis_on(bool);
    svg_1d_plot& set_legend_on(bool);
    svg_1d_plot& set_plot_window_on(bool);
    svg_1d_plot& set_title_on(bool);


    // color information    
    svg_1d_plot& set_background_border_color(svg_color_constant);
    svg_1d_plot& set_background_border_color(const svg_color&);

    svg_1d_plot& set_background_color(svg_color_constant);
    svg_1d_plot& set_background_color(const svg_color&);

    svg_1d_plot& set_legend_background_color(svg_color_constant);
    svg_1d_plot& set_legend_background_color(const svg_color&);

    svg_1d_plot& set_legend_border_color(svg_color_constant);
    svg_1d_plot& set_legend_border_color(const svg_color&);

    svg_1d_plot& set_plot_background_color(svg_color_constant);
    svg_1d_plot& set_plot_background_color(const svg_color&);
    
    svg_1d_plot& set_title_color(svg_color_constant);
    svg_1d_plot& set_title_color(const svg_color&);

    svg_1d_plot& set_x_axis_color(svg_color_constant);
    svg_1d_plot& set_x_axis_color(const svg_color&);

    svg_1d_plot& set_x_label_color(svg_color_constant);
    svg_1d_plot& set_x_label_color(const svg_color&);

    svg_1d_plot& set_x_major_grid_color(svg_color_constant);
    svg_1d_plot& set_x_major_grid_color(const svg_color&);

    svg_1d_plot& set_x_major_tick_color(svg_color_constant);
    svg_1d_plot& set_x_major_tick_color(const svg_color&);

    svg_1d_plot& set_x_minor_grid_color(svg_color_constant);
    svg_1d_plot& set_x_minor_grid_color(const svg_color&);

    svg_1d_plot& set_x_minor_tick_color(svg_color_constant);
    svg_1d_plot& set_x_minor_tick_color(const svg_color&);

    // axis information

    svg_1d_plot& set_x_axis_width(unsigned int);
    svg_1d_plot& set_x_label(const std::string&);
    svg_1d_plot& set_x_major_tick(double);
    svg_1d_plot& set_x_major_tick_length(unsigned int);
    svg_1d_plot& set_x_major_tick_width(unsigned int);
    svg_1d_plot& set_x_minor_tick_length(unsigned int);
    svg_1d_plot& set_x_minor_tick_width(unsigned int);
    svg_1d_plot& set_x_num_minor_ticks(unsigned int);        
    svg_1d_plot& set_x_scale(double, double);

    // getters
    unsigned int get_image_x_size();
    unsigned int get_image_y_size();
    unsigned int get_legend_title_font_size();
    unsigned int get_title_font_size();
    std::string  get_title();

    // commands
    bool get_axis();
    bool get_legend();
    bool get_plot_window();
    bool get_x_label();
    bool get_x_major_labels();

    // color information    
    svg_color get_background_color();
    svg_color get_background_border_color();
    svg_color get_legend_background_color();
    svg_color get_legend_border_color();
    svg_color get_plot_background_color();    
    svg_color get_title_color();
    svg_color get_x_axis_color();
    svg_color get_x_label_color();
    svg_color get_x_major_tick_color();
    svg_color get_x_minor_tick_color();
    svg_color get_x_major_grid_color();
    svg_color get_x_minor_grid_color();

    // axis information
    double get_x_min();
    double get_x_max();

    unsigned int get_x_axis_width();

    double get_x_major_tick();
    unsigned int get_x_major_tick_length();
    unsigned int get_x_minor_tick_length();
    unsigned int get_x_num_minor_ticks();   
    unsigned int get_x_major_tick_width();
    unsigned int get_x_minor_tick_width();

    std::string get_x_label_text();
};

// see documentation for default settings rationale
svg_1d_plot::svg_1d_plot(): x_label("X Axis"), x_min(-10), x_max(10), 
                      legend_on(false),
                      axis_on(true), plot_window_on(false), x_label_on(false),
                      x_major_grid_on(true), x_minor_grid_on(false),
                      x_major_tick(3), x_minor_tick_length(10), 
                      x_major_tick_length(20), x_num_minor_ticks(2), 
                      legend_title_font_size(12), x_label_font_size(12),
                      title_font_size(30), x_scale(1.), x_shift(0),
                      y_scale(1.), y_shift(0)
{
    set_image_size(500, 350);

    //build the document tree.. add children of the root node
    for(int i=0; i<SVG_PLOT_DOC_CHILDREN; ++i)
    {
        image.add_g_element();
    }

    // set color defaults
    image.get_g_element(PLOT_BACKGROUND).set_fill_color(white);
    image.get_g_element(PLOT_PLOT_BACKGROUND).set_fill_color(white);
    image.get_g_element(PLOT_LEGEND_BACKGROUND).set_fill_color(white);

    image.get_g_element(PLOT_X_MAJOR_TICKS).set_stroke_width(2);
    image.get_g_element(PLOT_X_MINOR_TICKS).set_stroke_width(1);
}

// -----------------------------------------------------------------
// write() has two flavors.. a file and a ostream. The file version
// opens a stream and calls the stream version. The stream version
// first clears all unnecessary data from the graph, builds the
// document tree, and then calls the write function for the root
// document node, which calls all other nodes through the Visitor
// pattern
// -----------------------------------------------------------------
svg_1d_plot& svg_1d_plot::write(const std::string& _str)
{
    std::ofstream fout(_str.c_str());

    if(fout.fail())
    {
        throw "Failed to open "+_str;
    }

    svg_1d_plot::write(fout);

    return *this;
}

//refactor a little
svg_1d_plot& svg_1d_plot::write(std::ostream& s_out)
{
    // removes all elements that will show up in a second image drawn
    _clear_all();
    // Hold off drawing the legend until the very end.. it's
    // easier to draw the size that it needs at the end than
    // it is to 
    // Don't bother with re-adding things if we don't need to

    image.get_g_element(PLOT_BACKGROUND).push_back(
                 new rect_element(0, 0, image.get_x_size(), 
                 image.get_y_size()));

    if(title_on)
    {
        text_element title(image.get_x_size()/2., title_font_size, title);

        title.set_alignment(center_align);
        title.set_font_size(title_font_size);
        image.get_g_element(PLOT_TITLE).push_back(new text_element(title));
    }

    x_axis = (plot_window_y2 + plot_window_y1)/2.;

    plot_window_x1 = plot_window_y1 = 0;
    plot_window_x2 = image.get_x_size();
    plot_window_y2 = image.get_y_size();

    if(plot_window_on)
    {
        plot_window_x1+=5;
        plot_window_x2-=5;
        plot_window_y1+=5;
        plot_window_y2-=5;

        if(legend_on)
        {
            plot_window_x2 -= 155;
        }

        if(x_label_on)
        {
            plot_window_y2 -= 20;
        }

        //for the title. Will take into account font size soon
        plot_window_y1 +=40;

        image.get_g_element(PLOT_PLOT_BACKGROUND).push_back(
                new rect_element(plot_window_x1, plot_window_y1, 
                        (plot_window_x2-plot_window_x1), plot_window_y2-plot_window_y1));
    }

    // calculate the coordinate transforms
    x_scale = (plot_window_x2-plot_window_x1)/(x_max-x_min);
    x_shift = plot_window_x1 - (x_min *(plot_window_x2-plot_window_x1)/(x_max-x_min));

    y_scale = 1.;
    y_shift = plot_window_y1 - (plot_window_y1-plot_window_y2)/2.;
        
    if(axis_on)
    {
        _draw_axis();
    }

    if(legend_on)
    {
        _draw_legend();
    }

    if(x_label_on)
    {
        _draw_x_label();
    }

    double x1(0), y1(0);

    //draw points
    for(unsigned int i=0; i<series.size(); ++i)
    {
        g_element& g_ptr = image.get_g_element(PLOT_PLOT_POINTS).add_g_element();
        g_ptr.set_fill_color(series[i].style.fill_color);
        g_ptr.set_stroke_color(series[i].style.stroke_color);

        for(unsigned int j=0; j<series[i].series.size(); ++j)
        {
            x1 = series[i].series[j];

            _transform_point(x1, y1);
            
            if(x1 > plot_window_x1 && x1 < plot_window_x2)
            {
                image.point(x1, x_axis, g_ptr);
            }
        }
    }

    image.write(s_out);
    
    return (svg_1d_plot&)*this;
}

// -----------------------------------------------------------------
// Parameter names for plot() function
// -----------------------------------------------------------------
BOOST_PARAMETER_NAME(my_plot)
BOOST_PARAMETER_NAME(container)
BOOST_PARAMETER_NAME(title)
BOOST_PARAMETER_NAME(stroke_color)
BOOST_PARAMETER_NAME(fill_color)
BOOST_PARAMETER_NAME(x_functor)

BOOST_PARAMETER_FUNCTION
(
    (void),
    plot,
    tag,
    (required 
        (in_out(my_plot), (svg_1d_plot&))
        (container, *)
        (title, (const std::string&))
    )
    (optional
        (stroke_color, (const svg_color&), svg_color(white))
    )
    (deduced
        (optional
            (fill_color, (const svg_color&), svg_color(black))
            (x_functor, *, boost_default_convert())
        )
    )
)
{
    std::vector<double> vect(container.size());

    vect.insert(vect.begin(), 
        boost::make_transform_iterator(container.begin(), x_functor),
        boost::make_transform_iterator(container.end(),   x_functor));

    my_plot.plot(vect, title, plot_point_style(fill_color, stroke_color));
}

// -----------------------------------------------------------------
// Actually draw data to the plot. Default color information
// -----------------------------------------------------------------
void svg_1d_plot::plot(const std::vector<double>& _ctr,
                            const std::string& _title,
                            const plot_point_style& _style)
{
    series.push_back(svg_plot_series(_ctr, 
                                     _title, 
                                     _style));
}

// -----------------------------------------------------------------
// Miscellaneous setter methods: those with no clear, definable home
// in another category
//
// set_image_size()
// set_title()
// set_title_font_size()
// set_legend_title_font_size()
// -----------------------------------------------------------------
svg_1d_plot& svg_1d_plot::set_image_size(unsigned int x, unsigned int y)
{
    image.image_size(x, y);
    
    return *this;
}

svg_1d_plot& svg_1d_plot::set_title(const std::string& _title)
{
    title = _title;

    return *this;
}

svg_1d_plot& svg_1d_plot::set_title_font_size(unsigned int _size)
{
    title_font_size = _size;

    return *this;
}

svg_1d_plot& svg_1d_plot::set_legend_title_font_size(unsigned int _size)
{
    legend_title_font_size = _size;

    return *this;
}

// -----------------------------------------------------------------
// Commands: Answers to yes or no questions (Example: Show the legend?)
//
// set_axis()
// set_legend()
// set_plot_window()
// set_x_label()
// set_x_major_labels()
// -----------------------------------------------------------------

svg_1d_plot& svg_1d_plot::set_axis_on(bool _cmd)
{
    axis_on = _cmd;
    return *this;
}

svg_1d_plot& svg_1d_plot::set_legend_on(bool _cmd)
{
    legend_on = _cmd;
    return *this;
}

svg_1d_plot& svg_1d_plot::set_plot_window_on(bool _cmd)
{
    plot_window_on = _cmd;
    return *this;
}

svg_1d_plot& svg_1d_plot::set_x_label_on(bool _cmd)
{
    x_label_on = _cmd;
    return *this;
}

svg_1d_plot& svg_1d_plot::set_x_major_labels_on(bool _cmd)
{
    x_major_labels_on = _cmd;
    return *this;
}

svg_1d_plot& svg_1d_plot::set_title_on(bool _cmd)
{
    title_on = _cmd;
    return *this;
}

svg_1d_plot& svg_1d_plot::set_x_major_grid_on(bool _is)
{
    x_major_grid_on = _is;
    return *this;
}

svg_1d_plot& svg_1d_plot::set_x_minor_grid_on(bool _is)
{
    x_minor_grid_on = _is;
    return *this;
}

// -----------------------------------------------------------------
// Color settings: Customization of colors found in the plot
//
// set_title_color()
// set_background_color()
// set_legend_background_color()
// set_plot_background_color()
// set_axis_color()
// set_x_major_tick_color()
// set_x_minor_tick_color()
// -----------------------------------------------------------------

svg_1d_plot& svg_1d_plot::set_title_color(svg_color_constant _col)
{
    set_title_color(constant_to_rgb(_col));
    return *this;
}

svg_1d_plot& svg_1d_plot::set_title_color(const svg_color& _col)
{
    image.get_g_element(PLOT_TITLE).set_stroke_color(_col);
    image.get_g_element(PLOT_TITLE).set_fill_color(_col);

    return *this;
}

svg_1d_plot& svg_1d_plot::set_background_color(svg_color_constant _col)
{
    set_background_color(constant_to_rgb(_col));
    return *this;
}

svg_1d_plot& svg_1d_plot::set_background_color(const svg_color& _col)
{
    image.get_g_element(PLOT_BACKGROUND).set_fill_color(_col);
    return *this;
}

svg_1d_plot& svg_1d_plot::set_legend_background_color(svg_color_constant _col)
{
    set_legend_background_color(constant_to_rgb(_col));
    return *this;
}

svg_1d_plot& svg_1d_plot::set_legend_background_color(const svg_color& _col)
{
    image.get_g_element(PLOT_LEGEND_BACKGROUND).set_fill_color(_col);
    return *this;
}

svg_1d_plot& svg_1d_plot::set_legend_border_color(svg_color_constant _col)
{
    set_legend_border_color(constant_to_rgb(_col));
    return *this;
}

svg_1d_plot& svg_1d_plot::set_legend_border_color(const svg_color& _col)
{
    image.get_g_element(PLOT_LEGEND_BACKGROUND).set_stroke_color(_col);
    return *this;
}

svg_1d_plot& svg_1d_plot::set_background_border_color(svg_color_constant _col)
{
    image.get_g_element(PLOT_BACKGROUND).set_stroke_color(_col);
    return *this;
}

svg_1d_plot& svg_1d_plot::set_background_border_color(const svg_color& _col)
{
    image.get_g_element(PLOT_BACKGROUND).set_stroke_color(_col);
    return *this;
}

svg_1d_plot& svg_1d_plot::set_plot_background_color(svg_color_constant _col)
{
    image.get_g_element(PLOT_PLOT_BACKGROUND).set_fill_color(_col);
    return *this;
}

svg_1d_plot& svg_1d_plot::set_plot_background_color(const svg_color& _col)
{
    image.get_g_element(PLOT_PLOT_BACKGROUND).set_fill_color(_col);
    return *this;
}

svg_1d_plot& svg_1d_plot::set_x_axis_color(svg_color_constant _col)
{
    set_x_axis_color(constant_to_rgb(_col));
    return *this;
}

svg_1d_plot& svg_1d_plot::set_x_axis_color(const svg_color& _col)
{
    image.get_g_element(PLOT_X_AXIS)
            .set_fill_color(_col);

    image.get_g_element(PLOT_X_AXIS)
            .set_stroke_color(_col);

    return *this;
}

svg_1d_plot& svg_1d_plot::set_x_major_tick_color(const svg_color& _col)
{
    image.get_g_element(PLOT_X_MAJOR_TICKS).set_stroke_color(_col);
    image.get_g_element(PLOT_X_MAJOR_TICKS).set_fill_color(_col);
    return *this;
}

svg_1d_plot& svg_1d_plot::set_x_major_tick_color(svg_color_constant _col)
{
    set_x_major_tick_color(constant_to_rgb(_col));
    return *this;
}

svg_1d_plot& svg_1d_plot::set_x_label_color(const svg_color& _col)
{
    image.get_g_element(PLOT_X_LABEL).set_stroke_color(_col);
    image.get_g_element(PLOT_X_LABEL).set_fill_color(_col);
    return *this;
}

svg_1d_plot& svg_1d_plot::set_x_label_color(svg_color_constant _col)
{
    set_x_label_color(constant_to_rgb(_col));
    return *this;
}

svg_1d_plot& svg_1d_plot::set_x_minor_tick_color(const svg_color& _col)
{
    image.get_g_element(PLOT_X_MINOR_TICKS).set_stroke_color(_col);
    image.get_g_element(PLOT_X_MINOR_TICKS).set_fill_color(_col);
    return *this;
}

svg_1d_plot& svg_1d_plot::set_x_minor_tick_color(svg_color_constant _col)
{
    set_x_minor_tick_color(constant_to_rgb(_col));
    return *this;
}

svg_1d_plot& svg_1d_plot::set_x_major_grid_color(const svg_color& _col)
{
    image.get_g_element(PLOT_X_MAJOR_GRID).set_stroke_color(_col);
    image.get_g_element(PLOT_X_MAJOR_GRID).set_fill_color(_col);
    return *this;
}

svg_1d_plot& svg_1d_plot::set_x_major_grid_color(svg_color_constant _col)
{
    set_x_major_grid_color(constant_to_rgb(_col));
    return *this;
}

svg_1d_plot& svg_1d_plot::set_x_minor_grid_color(const svg_color& _col)
{
    image.get_g_element(PLOT_X_MINOR_GRID).set_stroke_color(_col);
    image.get_g_element(PLOT_X_MINOR_GRID).set_fill_color(_col);
    return *this;
}

svg_1d_plot& svg_1d_plot::set_x_minor_grid_color(svg_color_constant _col)
{
    set_x_minor_grid_color(constant_to_rgb(_col));
    return *this;
}

// -----------------------------------------------------------------
// Axis information: Settings for customization of axis information
//
// set_x_axis_width()
// set_x_major_tick()
// set_x_major_tick_length()
// set_x_major_tick_width()
// set_x_minor_tick_length()
// set_x_minor_tick_width()
// set_x_label_text()
// set_x_num_minor_ticks()
// set_x_scale()
// -----------------------------------------------------------------

svg_1d_plot& svg_1d_plot::set_x_axis_width(unsigned int _width)
{
    image.get_g_element(PLOT_X_AXIS).set_stroke_width(_width);

    return *this;
}

svg_1d_plot& svg_1d_plot::set_x_label(const std::string& _str)
{
    x_label = _str;
    return *this;
}

svg_1d_plot& svg_1d_plot::set_x_major_tick(double _inter)
{
    x_major_tick = _inter;
    return *this;
}

svg_1d_plot& svg_1d_plot::set_x_major_tick_length(unsigned int _length)
{
    x_major_tick_length = _length;
    return *this;
}

svg_1d_plot& svg_1d_plot::set_x_major_tick_width(unsigned int _width)
{
    image.get_g_element(PLOT_X_MAJOR_TICKS).set_stroke_width(_width);
    return *this;
}

svg_1d_plot& svg_1d_plot::set_x_minor_tick_length(unsigned int _length)
{
    x_minor_tick_length = _length;
    return *this;
}

svg_1d_plot& svg_1d_plot::set_x_minor_tick_width(unsigned int _width)
{
    image.get_g_element(PLOT_X_MINOR_TICKS).set_stroke_width(_width);
    return *this;
}

svg_1d_plot& svg_1d_plot::set_x_num_minor_ticks(unsigned int _num)
{
    x_num_minor_ticks = _num;
    return *this;
}

svg_1d_plot& svg_1d_plot::set_x_scale(double x1, double x2)
{
    x_min = x1;
    x_max = x2;

    if(x2 <= x1)
    {
        throw "Illegal Argument: X scale: x2 < x1";
    }

    return (svg_1d_plot&)*this;
}

// -----------------------------------------------------------------
// We don't use the SVG coordinate transform because then text would
// be flipped. I'm considering using it to scale the image for resizes
// -----------------------------------------------------------------
void svg_1d_plot::_transform_point(double &x, double &y)
{
    x = x_scale* x + x_shift;
    y = y_scale* y + y_shift;
}

void svg_1d_plot::_transform_x(double &x)
{
    x = x_scale* x + x_shift;
}

void svg_1d_plot::_transform_y(double &y)
{
    y = y_scale* y + y_shift;
}

void svg_1d_plot::_draw_x_minor_ticks(double j)
{
    double x1(0.), y1(0.), y2(image.get_y_size());
    // draw the grid if needed
    if(x_minor_grid_on)
    {
        _transform_x(x1 = j);

        if(!plot_window_on)
        {
            // spacing for labels
            if(title_on)
            {
                y1 += title_font_size * 1.5;
            }

            if(x_label_on)
            {
                y2 -= x_label_font_size * 1.5;
            }
        }

        image.line(x1, y1, x1, y2, 
            image.get_g_element(PLOT_X_MINOR_GRID));
    }

    y1 = x_axis + x_minor_tick_length/2.;
    y2 = x_axis - x_minor_tick_length/2.;

    x1=j;

    _transform_x(x1);

    //make sure that we are drawing inside of the allowed window
    if(x1 < plot_window_x2)
    {
        image.line(x1, y1, x1, y2, 
            image.get_g_element(PLOT_X_MINOR_TICKS));
    }
}


void svg_1d_plot::_draw_x_major_ticks(double i)
{
    double x1(i), y1(0.), y2(image.get_x_size());

    if(x_major_grid_on)
    {
        _transform_x(x1 = i);

        if(!plot_window_on)
        {
            if(title_on)
            {
                y1 += title_font_size * 1.5;
            }

            if(x_label_on)
            {
                y2 -= x_label_font_size * 1.5;
            }
        }

        image.line(x1, y1, x1, y2, 
        image.get_g_element(PLOT_X_MAJOR_GRID));
    }

    //draw major tick
    x1=i;

    _transform_x(x1);

    //make sure that we are drawing inside of the allowed window
    if(x1 < plot_window_x2)
    {
        y1 = x_axis + x_major_tick_length/2;
        y2 = x_axis - x_major_tick_length/2;
        
        image.line(x1, y1, x1, y2, 
            image.get_g_element(PLOT_X_MAJOR_TICKS));

        
        if(x_major_labels_on)
        {
            std::stringstream fmt;
            fmt<<i;

            image.text(x1, y1 + (2 + x_major_tick_length/2), fmt.str(), 
                      image.get_g_element(PLOT_PLOT_LABELS));
        }
    }
}

void svg_1d_plot::_draw_x_axis()
{
    double y1(0.);

    // draw the axis line
    _transform_y(y1);

    x_axis = y1;

    image.line(plot_window_x1, x_axis, plot_window_x2, x_axis, 
        image.get_g_element(PLOT_X_AXIS));

    // x_minor_jump is the interval between minor ticks
    double x_minor_jump = x_major_tick/
                          ((double) (x_num_minor_ticks + 1.) );

    // draw the ticks on the positive side
    for(double i = 0; i < x_max; i += x_major_tick)
    {
        for(double j=i+x_minor_jump; j<i+x_major_tick; j+=x_minor_jump)
        {
            _draw_x_minor_ticks(j);
        }

        _draw_x_major_ticks(i);
    }

    // draw the ticks on the negative side
    for(double i = 0; i > x_min; i -= x_major_tick)
    {
        
        // draw minor ticks
        for(double j=i; j>i-x_major_tick; j-=x_major_tick / (x_num_minor_ticks+1))
        {
            _draw_x_minor_ticks(j);
        }

        _draw_x_major_ticks(i);
    }
}

//refactor
void svg_1d_plot::_draw_axis()
{
    double x1(0.), y1(0.), y2(image.get_y_size());

    _transform_point(x1, y1);
    
    y1 = 0.;

    //draw origin. Make sure it is in the window
    if(x1 > plot_window_x1 && x1 < plot_window_x2)
    {
        if(!plot_window_on)
        {
            if(title_on)
            {
                y1 += title_font_size * 1.5;
            }

            if(x_label_on)
            {
                y2 -= x_label_font_size * 1.5;
            }
        }

        image.line(x1, y1, x1, y2, 
            image.get_g_element(PLOT_X_AXIS));
    }

    _draw_x_axis();
}

// -----------------------------------------------------------------
// When writing to multiple documents, the contents of the plot
// may change significantly between. Rather than figuring out what
// has and has not changed, just erase the contents of the legend
// in the document and start over.
// -----------------------------------------------------------------
void svg_1d_plot::_clear_all()
{
    _clear_legend();
    _clear_background();
    _clear_x_axis();
    _clear_y_axis();
    _clear_title();
    _clear_points();
    _clear_plot_background();
    _clear_grids();
}

void svg_1d_plot::_clear_background()
{
    image.get_g_element(PLOT_BACKGROUND).clear();
}

void svg_1d_plot::_clear_title()
{
    image.get_g_element(PLOT_TITLE).clear();
}

void svg_1d_plot::_clear_points()
{
    image.get_g_element(PLOT_PLOT_POINTS).clear();
}

void svg_1d_plot::_clear_plot_background()
{
    image.get_g_element(PLOT_PLOT_BACKGROUND).clear();
}

void svg_1d_plot::_clear_legend()
{
    image.get_g_element(PLOT_LEGEND_BACKGROUND).clear();
    image.get_g_element(PLOT_LEGEND_POINTS).clear();
    image.get_g_element(PLOT_LEGEND_TEXT).clear();
}

void svg_1d_plot::_clear_x_axis()
{
    image.get_g_element(PLOT_X_AXIS).clear();
    image.get_g_element(PLOT_X_MINOR_TICKS).clear();
    image.get_g_element(PLOT_X_MAJOR_TICKS).clear();
    image.get_g_element(PLOT_X_LABEL).clear();
    image.get_g_element(PLOT_PLOT_LABELS).clear();
}

void svg_1d_plot::_clear_y_axis()
{
    image.get_g_element(PLOT_Y_AXIS).clear();
}

void svg_1d_plot::_clear_grids()
{
    image.get_g_element(PLOT_X_MAJOR_GRID).clear();
    image.get_g_element(PLOT_X_MINOR_GRID).clear();
}

void svg_1d_plot::_draw_legend_header(int _x, int _y, int _width)
{
    // 2 added to y argument for padding.
    text_element legend_header(_x+(_width/2.), _y + legend_title_font_size + 2, "Legend");

    legend_header.set_alignment(center_align);
    legend_header.set_font_size(legend_title_font_size);

    image.get_g_element(PLOT_LEGEND_TEXT).push_back(new text_element(legend_header));
}

// -----------------------------------------------------------------
// Important note: there are a lot of magic numbers that are temporary
// fill-ins for the time when the legend system is more configurable.
// This will happen bit-by-bit, as I give the user options to change
// these values
// -----------------------------------------------------------------
void svg_1d_plot::_draw_legend()
{
    int num_points = (int)(series.size());

    int legend_width(150);
    int legend_height(25);

    int x_size = image.get_x_size();

    // Figure out how wide the legend should be
    if(x_size < 200)
    {
       legend_width = (int)x_size; 
    }

    unsigned int legend_x_start(plot_window_x2 + 5);
    unsigned int legend_y_start(plot_window_y1);

    if((unsigned int)(plot_window_x2) >= image.get_x_size())
    {
        legend_x_start-=160;
        legend_y_start+=5;
    }

    if(title_on)
    {
        // -5 removes the padding
        legend_y_start += (int)(title_font_size * 1.5) - 5;
    }

    // legend_height = title_spacing + (space per element)(num_elements)
    //                  + (end spacing)
    legend_height = (int)(legend_title_font_size*1.5 + (25 * num_points) + 10);

    g_element* g_ptr = &(image.get_g_element(PLOT_LEGEND_BACKGROUND));

    g_ptr->push_back(new rect_element(legend_x_start, 
			             legend_y_start,
                         legend_width, 
				         legend_height));

    _draw_legend_header(legend_x_start, legend_y_start, legend_width);

    g_ptr = &(image.get_g_element(PLOT_LEGEND_POINTS));

    g_element* g_inner_ptr = g_ptr;

    for(unsigned int i=0; i<series.size(); ++i)
    {
        g_inner_ptr = &(g_ptr->add_g_element());

        g_inner_ptr->set_fill_color(series[i].style.fill_color);
        g_inner_ptr->set_stroke_color(series[i].style.stroke_color);

        g_inner_ptr->push_back(new point_element(legend_x_start + 25,
                        legend_y_start + legend_title_font_size + 20 + i*25));

        g_inner_ptr = &(image.get_g_element(PLOT_LEGEND_TEXT));

        g_inner_ptr->push_back(new text_element(legend_x_start + 40,
                        legend_y_start + legend_title_font_size + 25 + i*25,
                        series[i].title));
    }
}

void svg_1d_plot::_draw_x_label()
{
    text_element to_use((plot_window_x2 + plot_window_x1) / 2., 
        image.get_y_size() - 8, x_label);

    to_use.set_font_size(12);
    to_use.set_alignment(center_align);

    image.get_g_element(PLOT_X_LABEL).push_back(new text_element(to_use));
}

unsigned int svg_1d_plot::get_image_x_size()
{
    return image.get_x_size();
}

unsigned int svg_1d_plot::get_image_y_size()
{
    return image.get_x_size();
}

std::string svg_1d_plot::get_title()
{
    return title;
}

unsigned int svg_1d_plot::get_legend_title_font_size()
{
    return legend_title_font_size;
}

    // commands
bool svg_1d_plot::get_axis()
{
    return axis_on;
}

bool svg_1d_plot::get_legend()
{
    return legend_on;
}

bool svg_1d_plot::get_plot_window()
{
    return plot_window_on;
}

bool svg_1d_plot::get_x_label()
{
    return x_label_on;
}

bool svg_1d_plot::get_x_major_labels()
{
    return x_major_labels_on;
}

// color information    
svg_color svg_1d_plot::get_title_color()
{
    return image.get_g_element(PLOT_TITLE).get_fill_color();
}

svg_color svg_1d_plot::get_background_color()
{
    return image.get_g_element(PLOT_BACKGROUND).get_fill_color();
}

svg_color svg_1d_plot::get_background_border_color()
{
    return image.get_g_element(PLOT_BACKGROUND).get_stroke_color();
}

svg_color svg_1d_plot::get_legend_background_color()
{
    return image.get_g_element(PLOT_LEGEND_BACKGROUND).get_fill_color();
}

svg_color svg_1d_plot::get_legend_border_color()
{
    return image.get_g_element(PLOT_LEGEND_BACKGROUND).get_stroke_color();
}

svg_color svg_1d_plot::get_plot_background_color()
{
    return image.get_g_element(PLOT_PLOT_BACKGROUND).get_fill_color();
}   

svg_color svg_1d_plot::get_x_axis_color()
{
    return image.get_g_element(PLOT_X_AXIS).get_stroke_color();
}

svg_color svg_1d_plot::get_x_label_color()
{
    return image.get_g_element(PLOT_X_LABEL).get_fill_color();
}

svg_color svg_1d_plot::get_x_major_tick_color()
{
    return image.get_g_element(PLOT_X_MAJOR_TICKS).get_stroke_color();
}

svg_color svg_1d_plot::get_x_minor_tick_color()
{
    return image.get_g_element(PLOT_X_MINOR_TICKS).get_stroke_color();
}

svg_color svg_1d_plot::get_x_major_grid_color()
{
    return image.get_g_element(PLOT_X_MAJOR_GRID).get_stroke_color();
}

svg_color svg_1d_plot::get_x_minor_grid_color()
{
    return image.get_g_element(PLOT_X_MINOR_GRID).get_stroke_color();
}

// axis information
double svg_1d_plot::get_x_min()
{
    return x_min;
}

double svg_1d_plot::get_x_max()
{
    return x_max;
}

unsigned int svg_1d_plot::get_x_axis_width()
{
    return image.get_g_element(PLOT_X_AXIS).get_stroke_width();
}

double svg_1d_plot::get_x_major_tick()
{
    return x_major_tick;
}

unsigned int svg_1d_plot::get_x_major_tick_length()
{
    return x_major_tick_length;
}

unsigned int svg_1d_plot::get_x_minor_tick_length()
{
    return x_minor_tick_length;
}

unsigned int svg_1d_plot::get_x_num_minor_ticks()
{
    return x_num_minor_ticks;
}  

unsigned int svg_1d_plot::get_x_major_tick_width()
{
    return image.get_g_element(PLOT_X_MAJOR_TICKS).get_stroke_width();
}

unsigned int svg_1d_plot::get_x_minor_tick_width()
{
    return image.get_g_element(PLOT_X_MINOR_TICKS).get_stroke_width();
}

std::string svg_1d_plot::get_x_label_text()
{
    return x_label;
}


}
}

#endif

