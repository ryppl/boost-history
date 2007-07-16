// svg_1d_plot.hpp

// Copyright (C) Jacob Voytko 2007
//
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

// ----------------------------------------------------------------- 

#ifndef _BOOST_SVG_SVG_1D_PLOT_HPP
#define _BOOST_SVG_SVG_1D_PLOT_HPP

#include <vector>
#include <ostream>
#include <sstream>
#include <fstream>
#include <string>
#include <exception>

#define BOOST_PARAMETER_MAX_ARITY 10

#include <boost/parameter/preprocessor.hpp>
#include <boost/parameter/name.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/bind.hpp>

#include "svg.hpp"
#include "detail/svg_style.hpp"
#include "detail/axis_plot_frame.hpp"

namespace boost {
namespace svg {

// -----------------------------------------------------------------
// This functor allows any data convertible to doubles to be plotted
// -----------------------------------------------------------------
class boost_default_convert
{
public:
    typedef double result_type;

    template <class T>
    double operator()(T val) const
    {
        return (double)val;
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
    plot_point_style point_style;

    svg_plot_series(std::vector<double> _ctr, const std::string& _title,
                    const plot_point_style& _style):
                    series(_ctr), title(_title), point_style(_style)
    {
    }
};

class svg_1d_plot: public axis_plot_frame<svg_1d_plot>
{
protected:
    // stored so as to avoid rewriting style information constantly
    svg image;

    // border information for the plot window. Initially will be set 
    // to the width and height of the graph
    int plot_window_x1, plot_window_y1,
        plot_window_x2, plot_window_y2;

                 // axis information
    unsigned int x_major_tick_length,  x_major_tick_width,
                 x_minor_tick_length,  x_minor_tick_width,
                 x_label_font_size,    x_num_minor_ticks,

                 // misc information
                 legend_title_font_size, title_font_size; 
    
    // used for text displayed on the graph
    std::string  x_label, title;

    // double axis information. y_axis stored as one point because this 
    // is a 1D graph
    double       x_min,  x_max;
    double       x_axis;
    
    double       x_major_tick;

    // Yes/no questions
    bool x_major_labels_on;
    bool x_major_grid_on;
    bool x_minor_grid_on;
    bool x_label_on;

    bool title_on;
    bool legend_on;
    bool axis_on;
    bool plot_window_on;

private:
    friend class axis_plot_frame<svg_1d_plot>;

    // where we will be storing the data points for transformation
    std::vector<svg_plot_series> series;

    double x_scale, x_shift;
    double y_scale, y_shift;

    
    void _calculate_transform()
    {
        x_scale = (plot_window_x2-plot_window_x1)/(x_max-x_min);
        x_shift = plot_window_x1 - (x_min *(plot_window_x2-plot_window_x1)/
                                   (x_max-x_min));

        y_scale = 1.;
        y_shift = plot_window_y1 - (plot_window_y1-plot_window_y2)/2.;
    }

void _draw_axis()
{
    double x(0.), y1(0.), y2(image.get_y_size());

    _transform_x(x);
    
    y1 = 0.;

    //draw origin. Make sure it is in the window
    if(x > plot_window_x1 && x < plot_window_x2)
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

        else
        {
            y1 = plot_window_y1;
            y2 = plot_window_y2;
        }

        image.get_g_element(PLOT_X_AXIS).line(x, y1, x, y2);
    }
    _draw_x_axis();
}
    void _calculate_plot_window()
    {
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
    }

public:

// see documentation for default settings rationale
svg_1d_plot(): x_label("X Axis"), title("Plot of data"), x_min(-10), x_max(10), 
                      legend_on(false), title_on(true),
                      axis_on(true), plot_window_on(false), x_label_on(false),
                      x_major_grid_on(false), x_minor_grid_on(false),
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
    image.get_g_element(PLOT_BACKGROUND)
        .get_style_info().set_fill_color(white);

    image.get_g_element(PLOT_PLOT_BACKGROUND)
        .get_style_info().set_fill_color(white);

    image.get_g_element(PLOT_LEGEND_BACKGROUND)
        .get_style_info().set_fill_color(white);

    image.get_g_element(PLOT_X_MAJOR_TICKS)
        .get_style_info().set_stroke_width(2);

    image.get_g_element(PLOT_X_MINOR_TICKS)
        .get_style_info().set_stroke_width(1);
}

// -----------------------------------------------------------------
// write() has two flavors.. a file and a ostream. The file version
// opens a stream and calls the stream version. The stream version
// first clears all unnecessary data from the graph, builds the
// document tree, and then calls the write function for the root
// document node, which calls all other nodes through the Visitor
// pattern
// -----------------------------------------------------------------
svg_1d_plot& write(const std::string& _str)
{
    std::ofstream fout(_str.c_str());

    if(fout.fail())
    {
        throw std::runtime_error("Failed to open "+_str);
    }

    svg_1d_plot::write(fout);

    return *this;
}

svg_1d_plot& write(std::ostream& s_out)
{
    // removes all elements that will show up in a subsequent draw
    _clear_all();

    // draw background
    image.get_g_element(PLOT_BACKGROUND).push_back(
                 new rect_element(0, 0, image.get_x_size(), 
                 image.get_y_size()));

    _draw_title();
    _calculate_plot_window();
    _calculate_transform();

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

    double x(0), y(0);

    _transform_y(y);
        
    //draw points
    for(unsigned int i=0; i<series.size(); ++i)
    {
        g_element& g_ptr = image.get_g_element(PLOT_PLOT_POINTS).add_g_element();

        g_ptr.get_style_info().set_fill_color(series[i].point_style.fill_color);
        g_ptr.get_style_info().set_stroke_color(series[i].point_style.stroke_color);

        for(unsigned int j=0; j<series[i].series.size(); ++j)
        {
            x = series[i].series[j];
            _transform_x(x);

            if(x > plot_window_x1 
            && x < plot_window_x2
            && y > plot_window_y1 
            && y < plot_window_y2)
            {
                _draw_plot_point(x, y, g_ptr, series[i].point_style);
            }
        }
    }
    image.write(s_out);

    return (svg_1d_plot&)*this;
}

void plot(const std::vector<double>& _ctr,
                            const std::string& _title,
                            const plot_point_style& _style)
{
    series.push_back(svg_plot_series(_ctr, 
                                     _title, 
                                     _style));
}

}; // end svg_1d_plot

// -----------------------------------------------------------------
// Parameter names for plot() function
// -----------------------------------------------------------------

// These should be moved to their own namespace

#ifndef BOOST_SVG_BOOST_PARAMETER_NAMES
#define BOOST_SVG_BOOST_PARAMETER_NAMES

BOOST_PARAMETER_NAME(my_plot)
BOOST_PARAMETER_NAME(container)
BOOST_PARAMETER_NAME(title)
BOOST_PARAMETER_NAME(stroke_color)
BOOST_PARAMETER_NAME(fill_color)
BOOST_PARAMETER_NAME(point_style)
BOOST_PARAMETER_NAME(size)
BOOST_PARAMETER_NAME(x_functor)

#endif

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
        (point_style, (point_shape), circle)
        (size, (int), 10)
        (x_functor, *, boost_default_convert())
    )
    (deduced
        (optional
            (fill_color, (const svg_color&), svg_color(black))
        )
    )
)
{
    std::vector<double> vect(container.size());

    vect.insert(vect.begin(), 
        boost::make_transform_iterator(container.begin(), x_functor),
        boost::make_transform_iterator(container.end(),   x_functor));

    my_plot.plot(vect, title, 
           plot_point_style(fill_color, stroke_color, size, point_style));
}


}
}

#endif
