// svg_1d_plot.hpp

// Copyright (C) Jacob Voytko 2007
//
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

// ----------------------------------------------------------------- 

#ifndef _BOOST_SVG_SVG_2D_PLOT_HPP
#define _BOOST_SVG_SVG_2D_PLOT_HPP

#define BOOST_PARAMETER_MAX_ARITY 10

#include <map>
#include <string>
#include <sstream>
#include <utility>
#include <ostream>
#include <iterator>
#include <exception>

#include <boost/parameter/preprocessor.hpp>
#include <boost/parameter/name.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/bind.hpp>

#include "detail/svg_style.hpp"
#include "detail/axis_plot_frame.hpp"

#include "svg.hpp"

namespace boost {
namespace svg {

struct svg_2d_plot_series
{
    std::multimap<double, double> series;
    std::string title;
    plot_point_style point_style;
    plot_line_style line_style;

    svg_2d_plot_series(std::multimap<double, double>::const_iterator _begin, 
                       std::multimap<double, double>::const_iterator _end,
                       std::string _title,
                       const plot_point_style& _point,
                       const plot_line_style& _line):
                       series(_begin,_end),
                       title(_title),
                       point_style(_point),
                       line_style(_line)
    {

    }
};

class svg_2d_plot: public axis_plot_frame<svg_2d_plot>
{
private:
    friend class axis_plot_frame<svg_2d_plot>;
    
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

    // where we will be storing the data points for transformation
    std::vector<svg_2d_plot_series> series;

    // strings having to do with labels
    std::string  y_label;

    // axis information
    double       y_min,  y_max;

    double       y_major_tick, y_axis;

    unsigned int y_major_tick_length,  y_minor_tick_length,
                 y_num_minor_ticks;

    bool y_label_on;
    bool y_major_labels_on;

    void _draw_y_axis()
    {
        double x1(0.), x2(0.), y1(0.), y2(0.);

        // draw the axis line
        _transform_x(x1 );

        y_axis = x1;

        image.get_g_element(PLOT_Y_AXIS).line(y_axis, plot_window_y1, y_axis, plot_window_y2);

        // draw the ticks on the positive side
        for(double i = 0; i < y_max; i += y_major_tick)
        {
            //draw minor ticks
            for(double j=i+(y_major_tick / (y_num_minor_ticks+1)); 
                j<i+y_major_tick; 
                j+=y_major_tick / (y_num_minor_ticks+1))
            {
                x1 = y_axis + y_minor_tick_length/2.;
                x2 = y_axis - y_minor_tick_length/2.;

                y1=j;

                _transform_y(y1);

                //make sure that we are drawing inside of the allowed window
                if(y1 > plot_window_y1)
                {
                    image.get_g_element(PLOT_Y_MINOR_TICKS).line(x1, y1, x2, y1);
                }
            }

            //draw major tick
            y1=i;
            _transform_point(x1, y1);
            _transform_point(x2, y2);

            //make sure that we are drawing inside of the allowed window
            if(y1 > plot_window_y1)
            {
                x1 = y_axis + y_major_tick_length/2;
                x2 = y_axis - y_major_tick_length/2;

                image.get_g_element(PLOT_Y_MAJOR_TICKS).line(x1, y1, x2, y1);
                
                if(y_major_labels_on && i != 0)
                {
                    std::stringstream fmt;
                    fmt<<i;

                    image.text(x1, y1 + (2 + x_major_tick_length/2), fmt.str());
                }
            }
        }

        // draw the ticks on the negative side
        for(double i = 0; i > y_min; i -= y_major_tick)
        {
            //draw minor ticks
            for(double j=i-(y_major_tick / (y_num_minor_ticks+1)); 
                j>i-y_major_tick; 
                j-=y_major_tick / (y_num_minor_ticks+1))
            {
                x1 = y_axis + y_minor_tick_length/2.;
                x2 = y_axis - y_minor_tick_length/2.;

                y1=j;

                _transform_y(y1);

                //make sure that we are drawing inside of the allowed window
                if(y1 < plot_window_y2)
                {
                    image.get_g_element(PLOT_Y_MINOR_TICKS).line(x1, y1, x2, y1);
                }
            }

            //draw major tick
            y1=i;
            _transform_point(x1, y1);
            _transform_point(x2, y2);

            //make sure that we are drawing inside of the allowed window
            if(y1 < plot_window_y2)
            {
                x1 = y_axis + y_major_tick_length/2;
                x2 = y_axis - y_major_tick_length/2;

                image.get_g_element(PLOT_Y_MAJOR_TICKS).line(x1, y1, x2, y1);
                
                if(y_major_labels_on && i != 0)
                {
                    std::stringstream fmt;
                    fmt<<i;

                    image.text(x1, y1 + (2 + x_major_tick_length/2), fmt.str());
                }
            }
        }
    }

    void _draw_axis()
    {
        _draw_y_axis();
        _draw_x_axis();
    }

    void _draw_y_label()
    {
    /*    text_element to_use((plot_window_x2 + plot_window_x1) / 2., image.get_y_size() - 8, x_label);

        to_use.set_font_size(12);
        to_use.set_alignment(center_align);

        image.get_g_element(PLOT_X_LABEL).set_stroke_color(white);
        image.get_g_element(PLOT_X_LABEL).set_fill_color(white);


        image.get_g_element(PLOT_X_LABEL).push_back(new text_element(to_use));
    */
    }

    
    void _calculate_transform()
    {
        x_scale = (plot_window_x2-plot_window_x1)/(x_max-x_min);
        x_shift = plot_window_x1 - 
                               (x_min *(plot_window_x2-plot_window_x1)/(x_max-x_min));
     	
 	    y_scale = -(plot_window_y2-plot_window_y1)/(y_max-y_min);
 	    y_shift = plot_window_y1 - 
                  (y_max *(plot_window_y1-plot_window_y2)/(y_max-y_min));
    }

    void _calculate_plot_window()
    {
        int x_size = image.get_x_size();
        int y_size = image.get_y_size();
       
        plot_window_x1 = plot_window_y1 = 0;
        plot_window_x2 = image.get_x_size();
        plot_window_y2 = image.get_y_size();


        if(x_label_on)
        {
            plot_window_y2 -= (int)(x_label_font_size * 1.5);
        }

        if(y_label_on)
        {
            plot_window_x1 += 20;
        }

        if(title_on)
        {
            plot_window_y1 += (int)(title_font_size * 1.5);
        }

        if(plot_window_on)
        {
            // give the plot window a natural bit of padding
            plot_window_x1+=5;
            plot_window_x2-=5;
            plot_window_y1+=5;
            plot_window_y2-=5;

            
            if(legend_on)
            {
                plot_window_x2 -= 155;
            }

            image.get_g_element(PLOT_PLOT_BACKGROUND).push_back(
                    new rect_element(plot_window_x1, plot_window_y1, 
                            (plot_window_x2-plot_window_x1), plot_window_y2-plot_window_y1));
        }
    }

    void _draw_plot_lines()
    {
        double prev_x, prev_y;
     
        for(unsigned int i = 0; i < series.size(); ++i)
        {
            g_element& g_ptr = image.get_g_element(PLOT_PLOT_LINES).add_g_element();

            g_ptr.get_style_info().set_stroke_color(series[i].line_style.color)
                                  .set_fill_color(series[i].line_style.color);

            if(series[i].series.size() > 1)
            {
                std::multimap<double, double>::const_iterator j = series[i].series.begin();
                prev_x = (*j).first;
                prev_y = (*j).second;

                _transform_x(prev_x);
                _transform_y(prev_y);

                ++j;

                for(; j != series[i].series.end(); ++j)
                {
                    double temp_x((*j).first);
                    double temp_y((*j).second);

                    _transform_x(temp_x);
                    _transform_y(temp_y);

                    g_ptr.push_back(
                        new line_element(prev_x, prev_y, 
                                         temp_x, temp_y));
                    
                    prev_x = temp_x;
                    prev_y = temp_y;
                }
            }
        }
    }

public:

svg_2d_plot(): x_label(""), title("Plot of data"), y_label(""), x_min(-10), x_max(10), 
                      y_min(-10), y_max(10), 
                      x_major_tick(3), x_num_minor_ticks(2), 
                      y_major_tick(3), y_label_on(false),
                      x_minor_tick_length(10), x_major_tick_length(20),
                      x_major_tick_width(2), x_minor_tick_width(1),
                      y_minor_tick_length(10), y_num_minor_ticks(2), 
                      y_major_tick_length(20), legend_title_font_size(12),
                      title_font_size(30), x_label_font_size(12), 
                      x_major_labels_on(true), x_major_grid_on(false),
                      x_minor_grid_on(false), x_label_on(false),
                      title_on(true), legend_on(false), axis_on(true),
                      plot_window_on(false), y_major_labels_on(false)

{
    set_image_size(500, 350);

    //build the document tree.. add children of the root node
    for(int i=0; i<SVG_PLOT_DOC_CHILDREN; ++i)
    {
        image.add_g_element();
    }

    // set color defaults
    image.get_g_element(PLOT_BACKGROUND).get_style_info().set_fill_color(white);
    image.get_g_element(PLOT_PLOT_BACKGROUND).get_style_info().set_fill_color(white);
    image.get_g_element(PLOT_LEGEND_BACKGROUND).get_style_info().set_fill_color(white);

    image.get_g_element(PLOT_X_MAJOR_TICKS).get_style_info().set_stroke_width(2);
    image.get_g_element(PLOT_X_MINOR_TICKS).get_style_info().set_stroke_width(1);
}


svg_2d_plot& write(const std::string& _str)
{
    std::ofstream fout(_str.c_str());

    if(fout.fail())
    {
        throw std::runtime_error("Unable to open "+_str);
    }

    write(fout);

    return *this;
}

svg_2d_plot& write(std::ostream& s_out)
{
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
        _draw_y_axis();
        _draw_x_axis();
    }

    if(legend_on)
    {
        _draw_legend();
    }

    if(x_label_on)
    {
        _draw_x_label();
    }

    // draw lines

    _draw_plot_lines();

    // draw points
    double x(0.), y(0.);
    for(unsigned int i=0; i<series.size(); ++i)
    {
        g_element& g_ptr = image.get_g_element(PLOT_PLOT_POINTS).add_g_element();
        
        g_ptr.get_style_info()
             .set_fill_color(series[i].point_style.fill_color)
             .set_stroke_color(series[i].point_style.stroke_color);

        for(std::multimap<double,double>::const_iterator j = series[i].series.begin(); 
            j!=series[i].series.end(); ++j)
        {
            x = j->first;
            y = j->second;

            _transform_point(x, y);
            
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

    return *this;
}

// -----------------------------------------------------------------
// Actually draw data to the plot. Default color information
// -----------------------------------------------------------------
void plot(std::multimap<double, double> cont,
                            const std::string& _str,
                            const plot_point_style& point_style,
                            const plot_line_style& line_style)
{
    series.push_back(svg_2d_plot_series(cont.begin(), cont.end(), 
                    _str, point_style, line_style));
}

// -----------------------------------------------------------------
// Miscellaneous setter methods: those with no clear, definable home
// in another category
//
// set_image_size():      sets image size in pixels. (x,y) corresponds 
//                        to point at lower right of graph
//
// set_title():           adds the text _title to the top of the screen
//
// set_title_font_size(): uses an internal variable to save state, to
//                        avoid crashes when the title hasn't been
//                        set yet
//
// set_legend_title_font_size(): As above
// -----------------------------------------------------------------


// -----------------------------------------------------------------
// Commands: Answers to yes or no questions (Example: Show the legend?)
//
// set_axis_on(): Whether or not the axis will show
//
// set_legend_on(): Whether or not the legend will show
//
// set_plot_window_on(): Whether or not the plot will be full screen or
// 		      in its own contained window
//
// set_x_label_on(): Wrapper for 1d function
//
// set_y_label_on(): Sets the label for the y-axis
//
// set_x_major_labels_on(): Wrapper for 1d function
//
// set_y_major_labels_on(): Determines whether or not y axis major labels
//                       will be shown
// -----------------------------------------------------------------


svg_2d_plot& set_y_label_on(bool _cmd)
{
    y_label_on = _cmd;

    return *this;
}

svg_2d_plot& set_y_major_labels_on(bool _cmd)
{
    y_major_labels_on = _cmd;

    return *this;
}

// -----------------------------------------------------------------
// Color settings: Customization of colors found in the plot
//
// set_title_color(): 	   Sets the color of the plot title
//
// set_background_color(): Sets the color of the background. This is
// 			   not the same as the plot window background
//
// set_legend_background_color():
// 			   Sets the background color of the legend
//
// set_plot_background_color():
// 			   Sets the background color of the plot area.
// 			   If plot_window_on is not set true, this
// 			   does not show
//
// set_axis_color():	   Color of the x axis + origin
//
// set_x_major_tick_color(): Sets the color of the major ticks on
// 			     the x-axis
//
// set_x_minor_tick_color(): As above, but for minor ticks
// -----------------------------------------------------------------

svg_2d_plot& set_y_axis_color(svg_color_constant _col)
{
    set_y_axis_color(constant_to_rgb(_col));

    return (svg_2d_plot&)*this;
}

svg_2d_plot& set_y_axis_color(const svg_color& _col)
{
    image.get_g_element(PLOT_Y_AXIS)
            .get_style_info().set_fill_color(_col);

    image.get_g_element(PLOT_Y_AXIS)
            .get_style_info().set_stroke_color(_col);

    return *this;
}

svg_2d_plot& set_y_major_tick_color(const svg_color& _col)
{
    image.get_g_element(PLOT_Y_MAJOR_TICKS).get_style_info().set_stroke_color(_col);
    image.get_g_element(PLOT_Y_MAJOR_TICKS).get_style_info().set_fill_color(_col);
    return *this;
}

svg_2d_plot& set_y_major_tick_color(svg_color_constant _col)
{
    set_y_major_tick_color(constant_to_rgb(_col));
    return *this;
}

svg_2d_plot& set_y_minor_tick_color(const svg_color& _col)
{
    image.get_g_element(PLOT_Y_MINOR_TICKS).get_style_info().set_stroke_color(_col);
    image.get_g_element(PLOT_Y_MINOR_TICKS).get_style_info().set_fill_color(_col);
    return *this;
}

svg_2d_plot& set_y_minor_tick_color(svg_color_constant _col)
{
    set_y_minor_tick_color(constant_to_rgb(_col));
    return *this;
}

// -----------------------------------------------------------------
// Axis information: Settings for customization of axis information
//
// set_x_scale(): sets the left and right max values for the x axis
//
// set_x_axis_width(): The width of the x axis
//
// set_x_major_tick(): The distance between the ticks of the x_axis
//
// set_x_major_tick_length(): How long each tick will be
//
// set_x_minor_tick_length(): How long each tick will be
//
// set_x_num_minor_ticks(): The number of minor ticks between each
// 			    major tick
//
// set_x_label(): Labelling for the x-axis
//
// set_x_major_tick_width(): Stroke width for major ticks
// 
// set_x_minor_tick_width(): Stroke width for minor ticks
//
// All functions defined for x above are also defined for y
// -----------------------------------------------------------------

//y functions

svg_2d_plot& set_y_scale(double y1, double y2)
{
    y_min = y1;
    y_max = y2;

    if(y2 <= y1)
    {
        throw std::runtime_error("Illegal Argument: X scale: x2 < x1");
    }

    return *this;
}

svg_2d_plot& set_y_axis_width(unsigned int _width)
{
    image.get_g_element(PLOT_Y_AXIS).get_style_info().set_stroke_width(_width);

    return *this;
}

svg_2d_plot& set_y_major_tick(double _inter)
{
    y_major_tick = _inter;

    return *this;
}

svg_2d_plot& set_y_major_tick_length(unsigned int _length)
{
    y_major_tick_length = _length;
    return *this;
}

svg_2d_plot& set_y_minor_tick_length(unsigned int _length)
{
    y_minor_tick_length = _length;
    return *this;
}

svg_2d_plot& set_y_num_minor_ticks(unsigned int _num)
{
    y_num_minor_ticks = _num;
    return *this;
}

svg_2d_plot& set_y_label(const std::string& _str)
{
    y_label = _str;

    return *this;
}

svg_2d_plot& set_y_major_tick_width(unsigned int _width)
{
    image.get_g_element(PLOT_Y_MAJOR_TICKS).get_style_info().set_stroke_width(_width);

    return *this;
}

svg_2d_plot& set_y_minor_tick_width(unsigned int _width)
{
    image.get_g_element(PLOT_Y_MINOR_TICKS).get_style_info().set_stroke_width(_width);

    return *this;
}

};

class boost_default_2d_convert
{
public:
    typedef std::pair<double, double> result_type;

    template <class T, class U>
    std::pair<double, double> operator()(const std::pair<T, U>& a) const
    {
        return std::pair<double, double>((double)(a.first), (double)(a.second));
    }
};

#ifndef BOOST_SVG_BOOST_PARAMETER_NAMES
#define BOOST_SVG_BOOST_PARAMETER_NAMES

BOOST_PARAMETER_NAME(my_plot)
BOOST_PARAMETER_NAME(container)
BOOST_PARAMETER_NAME(title)
BOOST_PARAMETER_NAME(stroke_color)
BOOST_PARAMETER_NAME(fill_color)
BOOST_PARAMETER_NAME(point_style)
BOOST_PARAMETER_NAME(x_functor)
BOOST_PARAMETER_NAME(size)
#endif

BOOST_PARAMETER_NAME(line_on)
BOOST_PARAMETER_NAME(line_color)

BOOST_PARAMETER_FUNCTION
(
    (void),
    plot_2d,
    tag,
    (required 
        (in_out(my_plot), (svg_2d_plot&))
        (container, *)
        (title, (const std::string&))
    )
    (optional
        (stroke_color, (const svg_color&), svg_color(white))
        (point_style, (point_shape), circle)
        (size, (int), 10)
        (line_on, (bool), true)
        (line_color, (const svg_color&), svg_color(black))
    )
    (deduced
        (optional
            (fill_color, (const svg_color&), svg_color(black))
            (x_functor, *, boost_default_2d_convert())
        )
    )
)
{
    std::multimap<double, double> cont;

    cont.insert(
        boost::make_transform_iterator(container.begin(), x_functor),
        boost::make_transform_iterator(container.end(),   x_functor));

    my_plot.plot(cont, title, 
        plot_point_style(fill_color, stroke_color, size, point_style),
        plot_line_style(line_color, line_on));
}

}
}

#endif
