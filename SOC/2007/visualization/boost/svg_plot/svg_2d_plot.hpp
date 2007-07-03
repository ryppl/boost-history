// svg_1d_plot.hpp

// Copyright (C) Jacob Voytko 2007
//
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

// ----------------------------------------------------------------- 

#ifndef _SVG_2D_PLOT_HPP
#define _SVG_2D_PLOT_HPP

#include <map>
#include <ostream>
#include <sstream>
#include <iterator>

#include "svg_1d_plot.hpp"
#include "detail/svg_plot_instruction.hpp"


namespace boost {
namespace svg {

struct svg_2d_plot_series
{
    std::multimap<double, double> series;
    std::string title;
    svg_color style;

    svg_2d_plot_series(std::multimap<double, double>::const_iterator _begin, 
                       std::multimap<double, double>::const_iterator _end,
                       std::string _title,
                       const svg_color& _style):
                       series(_begin,_end),
                       title(_title),
                       style(_style)
    {

    }
};

class svg_2d_plot: public svg_1d_plot
{
private:
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

    void _draw_y_axis();
    void _draw_axis();
    void _draw_y_label();
    void _draw_legend();

    svg_2d_plot(const svg_2d_plot&);
    svg_2d_plot& operator=(const svg_2d_plot&);

public:

    // constructors
    svg_2d_plot();

    // output
    svg_2d_plot& write(const std::string&);
    svg_2d_plot& write(std::ostream&);

    
    // plot functions
    void plot_range(std::multimap<double,double>::const_iterator,
        std::multimap<double,double>::const_iterator, const std::string&);
    
    void plot_range(std::multimap<double,double>::const_iterator, 
        std::multimap<double,double>::const_iterator, const std::string&, 
        svg_color_constant);

    // setters

    // Methods from the public interface of svg_1d_plot are
    // wrapped here so that we may correctly use chaining

    // misc
    svg_2d_plot& set_image_size(unsigned int, unsigned int);
    svg_2d_plot& set_title(const std::string&);
    svg_2d_plot& set_title_font_size(unsigned int);
    svg_2d_plot& set_legend_title_font_size(unsigned int);

    // commands
    svg_2d_plot& set_axis(bool);
    svg_2d_plot& set_legend(bool);
    svg_2d_plot& set_plot_window(bool);

    svg_2d_plot& set_x_label(bool);
    svg_2d_plot& set_x_major_labels(bool);

    svg_2d_plot& set_y_label(bool);
    svg_2d_plot& set_y_major_labels(bool);

    // color information    
    svg_2d_plot& set_title_color(svg_color_constant);
    svg_2d_plot& set_title_color(const svg_color&);

    svg_2d_plot& set_background_color(svg_color_constant);
    svg_2d_plot& set_background_color(const svg_color&);

    svg_2d_plot& set_legend_background_color(svg_color_constant);
    svg_2d_plot& set_legend_background_color(const svg_color&);

    svg_2d_plot& set_legend_border_color(svg_color_constant);
    svg_2d_plot& set_legend_border_color(const svg_color&);

    svg_2d_plot& set_plot_background_color(svg_color_constant);
    svg_2d_plot& set_plot_background_color(const svg_color&);
    
    svg_2d_plot& set_x_axis_color(svg_color_constant);
    svg_2d_plot& set_x_axis_color(const svg_color&);

    svg_2d_plot& set_y_axis_color(svg_color_constant);
    svg_2d_plot& set_y_axis_color(const svg_color&);

    svg_2d_plot& set_x_major_tick_color(svg_color_constant);
    svg_2d_plot& set_x_major_tick_color(const svg_color&);

    svg_2d_plot& set_y_major_tick_color(svg_color_constant);
    svg_2d_plot& set_y_major_tick_color(const svg_color&);

    svg_2d_plot& set_x_minor_tick_color(svg_color_constant);
    svg_2d_plot& set_x_minor_tick_color(const svg_color&);

    svg_2d_plot& set_y_minor_tick_color(svg_color_constant);
    svg_2d_plot& set_y_minor_tick_color(const svg_color&);

    // axis information
    svg_2d_plot& set_x_scale(double, double);
    svg_2d_plot& set_y_scale(double, double);

    svg_2d_plot& set_x_axis_width(unsigned int);
    svg_2d_plot& set_y_axis_width(unsigned int);

    svg_2d_plot& set_x_major_tick(double);
    svg_2d_plot& set_x_major_tick_length(unsigned int);
    svg_2d_plot& set_x_minor_tick_length(unsigned int);
    svg_2d_plot& set_x_num_minor_ticks(unsigned int);        
    svg_2d_plot& set_x_label_text(const std::string&);
    svg_2d_plot& set_x_major_tick_width(unsigned int);
    svg_2d_plot& set_x_minor_tick_width(unsigned int);

    svg_2d_plot& set_y_major_tick(double);
    svg_2d_plot& set_y_major_tick_length(unsigned int);
    svg_2d_plot& set_y_minor_tick_length(unsigned int);
    svg_2d_plot& set_y_num_minor_ticks(unsigned int);        
    svg_2d_plot& set_y_label_text(const std::string&);
    svg_2d_plot& set_y_major_tick_width(unsigned int);
    svg_2d_plot& set_y_minor_tick_width(unsigned int);

    // getters
    const std::string& get_title();
    unsigned int get_title_font_size();

    svg_color get_background_color();
    svg_color get_legend_background_color();
    svg_color get_axis_color();

    unsigned int get_axis_width();
};

svg_2d_plot::svg_2d_plot(): y_label(""), y_min(-10), y_max(10), 
                      y_major_tick(1), y_label_on(false),
                      y_minor_tick_length(10), y_num_minor_ticks(4), 
                      svg_1d_plot()
{

}


svg_2d_plot& svg_2d_plot::write(const std::string& _str)
{
    std::ofstream fout(_str.c_str());

    if(fout.fail())
    {
        throw "Failure opening "+_str;
    }

    write(fout);

    return *this;
}

svg_2d_plot& svg_2d_plot::write(std::ostream& s_out)
{
        int x_size = image.get_x_size();
    int y_size = image.get_y_size();
   
    plot_window_x1 = plot_window_y1 = 0;
    plot_window_x2 = image.get_x_size();
    plot_window_y2 = image.get_y_size();

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

        if(x_label_on)
        {
            plot_window_y2 -= 20;
        }

        if(y_label_on)
        {
            plot_window_x1 += 20;
        }

        //for the title. Will take into account font size soon
        plot_window_y1 +=40;

        image.get_g_element(PLOT_PLOT_BACKGROUND).push_back(
                new rect_element(plot_window_x1, plot_window_y1, 
                        (plot_window_x2-plot_window_x1), plot_window_y2-plot_window_y1));
    }

    transform_matrix[0][0] = (plot_window_x2-plot_window_x1)/(x_max-x_min);
    transform_matrix[0][2] = plot_window_x1 - (x_min *(plot_window_x2-plot_window_x1)/(x_max-x_min));

    transform_matrix[1][1] = -(plot_window_y2-plot_window_y1)/(y_max-y_min);
    transform_matrix[1][2] = plot_window_y1 - (y_max *(plot_window_y1-plot_window_y2)/(y_max-y_min));

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

    //draw points
    double x(0.), y(0.);
    for(unsigned int i=0; i<series.size(); ++i)
    {
        g_element& g_ptr = image.get_g_element(PLOT_PLOT_POINTS).add_g_element();
        g_ptr.set_fill_color(series[i].style);

        for(std::multimap<double,double>::const_iterator j = series[i].series.begin(); 
            j!=series[i].series.end(); ++j)
        {
            x = j->first;
            y = j->second;

            _transform_point(x, y);
            
            if(x > plot_window_x1 && x < plot_window_x2
             && y > plot_window_y1 && y < plot_window_y2)
            {
                image.point(x, y, g_ptr);
            }
        }
    }

    image.write(s_out);

    return *this;
}

template <class iter>
void plot_range(svg_2d_plot& _cont, iter _begin, iter _end, std::string _str)
{
    std::multimap<double, double> mult(_begin, _end);

    _cont.plot_range(mult.begin(), mult.end(), _str);
}

template <class iter>
void plot_range(svg_2d_plot& _cont, iter _begin, iter _end, std::string _str,
                      svg_color_constant _col)
{
    std::multimap<double, double> vect(_begin, _end);

    _cont.plot_range(vect.begin(), vect.end(), _str, _col);
}

// -----------------------------------------------------------------
// Actually draw data to the plot. Default color information
// -----------------------------------------------------------------
void svg_2d_plot::plot_range(std::multimap<double, double>::const_iterator begin,
                            std::multimap<double, double>::const_iterator end,
                            const std::string& _str)
{
    series.push_back(svg_2d_plot_series(begin, end, _str, svg_color(0,0,0)));
}

// -----------------------------------------------------------------
// Actually draw data to the plot. Fill color information provided
// -----------------------------------------------------------------
void svg_2d_plot::plot_range(std::multimap<double, double>::const_iterator begin,
                             std::multimap<double, double>::const_iterator end,
                             const std::string& _str,
                             svg_color_constant _col)
{
    series.push_back(svg_2d_plot_series(begin, end, _str, constant_to_rgb(_col)));
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
svg_2d_plot& svg_2d_plot::set_image_size(unsigned int x, unsigned int y)
{
    svg_1d_plot::set_image_size(x, y);
    
    return *this;
}

svg_2d_plot& svg_2d_plot::set_title(const std::string& _title)
{
    svg_1d_plot::set_title(_title);

    return *this;
}

svg_2d_plot& svg_2d_plot::set_title_font_size(unsigned int _size)
{
    svg_1d_plot::set_title_font_size(_size);

    return *this;
}

svg_2d_plot& svg_2d_plot::set_legend_title_font_size(unsigned int _size)
{
    svg_1d_plot::set_legend_title_font_size(_size);

    return *this;
}

// -----------------------------------------------------------------
// Commands: Answers to yes or no questions (Example: Show the legend?)
//
// set_axis(): Whether or not the axis will show
//
// set_legend(): Whether or not the legend will show
//
// set_plot_window(): Whether or not the plot will be full screen or
// 		      in its own contained window
//
// set_x_label(): Wrapper for 1d function
//
// set_y_label(): Sets the label for the y-axis
//
// set_x_major_labels(): Wrapper for 1d function
//
// set_y_major_labels(): Determines whether or not y axis major labels
//                       will be shown
// -----------------------------------------------------------------

svg_2d_plot& svg_2d_plot::set_axis(bool _cmd)
{
    svg_1d_plot::set_axis(_cmd);

    return *this;
}

svg_2d_plot& svg_2d_plot::set_legend(bool _cmd)
{
    svg_1d_plot::set_legend(_cmd);

    return *this;
}

svg_2d_plot& svg_2d_plot::set_plot_window(bool _cmd)
{
    svg_1d_plot::set_plot_window(_cmd);

    return *this;
}

svg_2d_plot& svg_2d_plot::set_x_label(bool _cmd)
{
    svg_1d_plot::set_x_label(_cmd);

    return *this;
}

svg_2d_plot& svg_2d_plot::set_y_label(bool _cmd)
{
    y_label_on = _cmd;

    return *this;
}

svg_2d_plot& svg_2d_plot::set_x_major_labels(bool _cmd)
{
    svg_1d_plot::set_x_major_labels(_cmd);

    return *this;
}

svg_2d_plot& svg_2d_plot::set_y_major_labels(bool _cmd)
{
    y_major_labels_on = _cmd;

    return *this;
}

svg_2d_plot& svg_2d_plot::set_title_color(svg_color_constant _col)
{
    svg_1d_plot::set_title_color(constant_to_rgb(_col));

    return *this;
}

svg_2d_plot& svg_2d_plot::set_title_color(const svg_color& _col)
{
    svg_1d_plot::set_title_color(_col);

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
svg_2d_plot& svg_2d_plot::set_background_color(svg_color_constant _col)
{
    svg_1d_plot::set_background_color(constant_to_rgb(_col));

    return *this;
}

svg_2d_plot& svg_2d_plot::set_background_color(const svg_color& _col)
{
    svg_1d_plot::set_background_color(_col);

    return *this;
}

svg_2d_plot& svg_2d_plot::set_legend_background_color(svg_color_constant _col)
{
    svg_1d_plot::set_legend_background_color(constant_to_rgb(_col));

    return *this;
}

svg_2d_plot& svg_2d_plot::set_legend_background_color(const svg_color& _col)
{
    svg_1d_plot::set_legend_background_color(_col);

    return *this;
}

svg_2d_plot& svg_2d_plot::set_legend_border_color(svg_color_constant _col)
{
    svg_1d_plot::set_legend_border_color(constant_to_rgb(_col));

    return *this;
}

svg_2d_plot& svg_2d_plot::set_legend_border_color(const svg_color& _col)
{
    svg_1d_plot::set_legend_border_color(_col);

    return *this;
}

svg_2d_plot& svg_2d_plot::set_plot_background_color(svg_color_constant _col)
{
    svg_1d_plot::set_plot_background_color(constant_to_rgb(_col));

    return *this;
}

svg_2d_plot& svg_2d_plot::set_plot_background_color(const svg_color& _col)
{
    svg_1d_plot::set_plot_background_color(_col);

    return *this;
}

svg_2d_plot& svg_2d_plot::set_x_axis_color(svg_color_constant _col)
{
    svg_1d_plot::set_x_axis_color(constant_to_rgb(_col));

    return (svg_2d_plot&)*this;
}

svg_2d_plot& svg_2d_plot::set_x_axis_color(const svg_color& _col)
{
    svg_1d_plot::set_x_axis_color(_col);

    return *this;
}

svg_2d_plot& svg_2d_plot::set_y_axis_color(svg_color_constant _col)
{
    set_y_axis_color(constant_to_rgb(_col));

    return (svg_2d_plot&)*this;
}

svg_2d_plot& svg_2d_plot::set_y_axis_color(const svg_color& _col)
{
    image.get_g_element(PLOT_Y_AXIS)
            .set_fill_color(_col);

    image.get_g_element(PLOT_Y_AXIS)
            .set_stroke_color(_col);

    return *this;
}

svg_2d_plot& svg_2d_plot::set_x_major_tick_color(svg_color_constant _col)
{
    svg_1d_plot::set_x_major_tick_color(constant_to_rgb(_col));

    return *this;
}

svg_2d_plot& svg_2d_plot::set_x_major_tick_color(const svg_color& _col)
{
    svg_1d_plot::set_x_major_tick_color(_col);

    return *this;
}

svg_2d_plot& svg_2d_plot::set_y_major_tick_color(const svg_color& _col)
{
    image.get_g_element(PLOT_Y_MAJOR_TICKS).set_stroke_color(_col);
    image.get_g_element(PLOT_Y_MAJOR_TICKS).set_fill_color(_col);
    return *this;
}

svg_2d_plot& svg_2d_plot::set_y_major_tick_color(svg_color_constant _col)
{
    set_y_major_tick_color(constant_to_rgb(_col));
    return *this;
}

svg_2d_plot& svg_2d_plot::set_x_minor_tick_color(svg_color_constant _col)
{
    svg_1d_plot::set_x_minor_tick_color(constant_to_rgb(_col));

    return *this;
}

svg_2d_plot& svg_2d_plot::set_x_minor_tick_color(const svg_color& _col)
{
    svg_1d_plot::set_x_minor_tick_color(_col);
    return *this;
}

svg_2d_plot& svg_2d_plot::set_y_minor_tick_color(const svg_color& _col)
{
    image.get_g_element(PLOT_Y_MINOR_TICKS).set_stroke_color(_col);
    image.get_g_element(PLOT_Y_MINOR_TICKS).set_fill_color(_col);
    return *this;
}

svg_2d_plot& svg_2d_plot::set_y_minor_tick_color(svg_color_constant _col)
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
// set_x_label_text(): Labelling for the x-axis
//
// set_x_major_tick_width(): Stroke width for major ticks
// 
// set_x_minor_tick_width(): Stroke width for minor ticks
//
// All functions defined for x above are also defined for y
// -----------------------------------------------------------------

// x functions
svg_2d_plot& svg_2d_plot::set_x_scale(double x1, double x2)
{
    svg_1d_plot::set_x_scale(x1, x2);

    return *this;
}

svg_2d_plot& svg_2d_plot::set_x_axis_width(unsigned int _width)
{
    svg_1d_plot::set_x_axis_width(_width);

    return *this;
}

svg_2d_plot& svg_2d_plot::set_x_major_tick(double _inter)
{
    svg_1d_plot::set_x_major_tick(_inter);

    return *this;
}

svg_2d_plot& svg_2d_plot::set_x_major_tick_length(unsigned int _length)
{
    svg_1d_plot::set_x_major_tick_length(_length);
    return *this;
}

svg_2d_plot& svg_2d_plot::set_x_minor_tick_length(unsigned int _length)
{
    svg_1d_plot::set_x_minor_tick_length(_length);
    return *this;
}

svg_2d_plot& svg_2d_plot::set_x_num_minor_ticks(unsigned int _num)
{
    svg_1d_plot::set_x_num_minor_ticks(_num);
    return *this;
}

svg_2d_plot& svg_2d_plot::set_x_label_text(const std::string& _str)
{
    svg_1d_plot::set_x_label_text(_str);

    return *this;
}

svg_2d_plot& svg_2d_plot::set_x_major_tick_width(unsigned int _width)
{
    svg_1d_plot::set_x_major_tick_width(_width);

    return *this;
}

svg_2d_plot& svg_2d_plot::set_x_minor_tick_width(unsigned int _width)
{
    svg_1d_plot::set_x_minor_tick_width(_width);

    return *this;
}

//y functions

svg_2d_plot& svg_2d_plot::set_y_scale(double y1, double y2)
{
    y_min = y1;
    y_max = y2;

    if(y2 <= y1)
    {
        throw "Illegal Argument: X scale: x2 < x1";
    }

    return *this;
}

svg_2d_plot& svg_2d_plot::set_y_axis_width(unsigned int _width)
{
    image.get_g_element(PLOT_Y_AXIS).set_stroke_width(_width);

    return *this;
}

svg_2d_plot& svg_2d_plot::set_y_major_tick(double _inter)
{
    y_major_tick = _inter;

    return *this;
}

svg_2d_plot& svg_2d_plot::set_y_major_tick_length(unsigned int _length)
{
    y_major_tick_length = _length;
    return *this;
}

svg_2d_plot& svg_2d_plot::set_y_minor_tick_length(unsigned int _length)
{
    y_minor_tick_length = _length;
    return *this;
}

svg_2d_plot& svg_2d_plot::set_y_num_minor_ticks(unsigned int _num)
{
    y_num_minor_ticks = _num;
    return *this;
}

svg_2d_plot& svg_2d_plot::set_y_label_text(const std::string& _str)
{
    y_label = _str;

    return *this;
}

svg_2d_plot& svg_2d_plot::set_y_major_tick_width(unsigned int _width)
{
    image.get_g_element(PLOT_Y_MAJOR_TICKS).set_stroke_width(_width);

    return *this;
}

svg_2d_plot& svg_2d_plot::set_y_minor_tick_width(unsigned int _width)
{
    image.get_g_element(PLOT_Y_MINOR_TICKS).set_stroke_width(_width);

    return *this;
}

void svg_2d_plot::_draw_y_axis()
{
    double x1(0.), x2(0.), y1(0.), y2(0.), toss(0.);

    // draw the axis line
    _transform_point(x1, y1);

    y_axis = x1;

    image.line(y_axis, plot_window_y1, y_axis, plot_window_y2, 
        image.get_g_element(PLOT_Y_AXIS));

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

            _transform_point(toss, y1);

            //make sure that we are drawing inside of the allowed window
            if(y1 > plot_window_y1)
            {
                image.line(x1, y1, x2, y1, 
                    image.get_g_element(PLOT_Y_MINOR_TICKS));
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

            image.line(x1, y1, x2, y1, 
                image.get_g_element(PLOT_Y_MAJOR_TICKS));
            
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

            _transform_point(toss, y1);

            //make sure that we are drawing inside of the allowed window
            if(y1 < plot_window_y2)
            {
                image.line(x1, y1, x2, y1, 
                    image.get_g_element(PLOT_Y_MINOR_TICKS));
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

            image.line(x1, y1, x2, y1, 
                image.get_g_element(PLOT_Y_MAJOR_TICKS));
            
            if(y_major_labels_on && i != 0)
            {
                std::stringstream fmt;
                fmt<<i;

                image.text(x1, y1 + (2 + x_major_tick_length/2), fmt.str());
            }
        }
    }

}

//refactor
void svg_2d_plot::_draw_axis()
{
    _draw_y_axis();
    _draw_x_axis();
}

void svg_2d_plot::_draw_y_label()
{
/*    text_element to_use((plot_window_x2 + plot_window_x1) / 2., image.get_y_size() - 8, x_label);

    to_use.set_font_size(12);
    to_use.set_alignment(center_align);

    image.get_g_element(PLOT_X_LABEL).set_stroke_color(white);
    image.get_g_element(PLOT_X_LABEL).set_fill_color(white);


    image.get_g_element(PLOT_X_LABEL).push_back(new text_element(to_use));
*/
}

// -----------------------------------------------------------------
// Important note: there are a lot of magic numbers that are temporary
// fill-ins for the time when the legend system is more configurable.
// This will happen bit-by-bit, as I give the user options to change
// these values
//
// The legend will soon be a percentage of the window, which will
// remove some of the magic values
// -----------------------------------------------------------------
void svg_2d_plot::_draw_legend()
{
    _clear_legend();

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

    // legend_height = title_spacing + (space per element)(num_elements)
    //                  + (end spacing)
    legend_height = (int)(legend_title_font_size*1.5 + (25 * num_points) + 10);

    // TODO: Figure out how tall the legend should be

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

        g_inner_ptr->set_fill_color(series[i].style);
        g_inner_ptr->set_stroke_color(series[i].style);

        g_inner_ptr->push_back(new point_element(legend_x_start + 25,
                        legend_y_start + legend_title_font_size + 20 + i*25));

        g_inner_ptr->push_back(new text_element(legend_x_start + 40,
                        legend_y_start + legend_title_font_size + 25 + i*25,
                        series[i].title));
    }
}


}
}

#endif