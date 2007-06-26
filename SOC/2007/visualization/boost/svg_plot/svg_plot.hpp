                                                                     
                                                                     
                                                                     
                                             
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
enum plot_doc_structure{PLOT_BACKGROUND, PLOT_LEGEND_BACKGROUND,
    PLOT_LEGEND_POINTS, PLOT_LEGEND_TEXT, PLOT_PLOT_BACKGROUND, 
    PLOT_PLOT_AXIS, PLOT_X_MINOR_TICKS, PLOT_X_MAJOR_TICKS, 
    PLOT_PLOT_LABELS, PLOT_PLOT_LINES, PLOT_PLOT_POINTS, PLOT_X_LABEL, 
    PLOT_TITLE};

#define SVG_PLOT_DOC_CHILDREN 13

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
    svg_color style;

    svg_plot_series(std::vector<double>::const_iterator _begin, 
                    std::vector<double>::const_iterator _end,
                    std::string _title,
                    const svg_color& _style):
                    series(_begin,_end),
                    title(_title),
                    style(_style)
    {

    }
};

class svg_plot
{
private:
    //todo: replace with x_scale, x_shift, since I don't use the full matrix
    double transform_matrix[3][3];

    // stored so as to avoid rewriting style information constantly
    svg image;

    // where we will be storing the data points for transformation
    std::vector<svg_plot_series> series;

    // strings having to do with labels
    std::string  x_label, title;

    // axis information. y_axis stored as one point because this is a 1D graph
    double       x_min,  x_max;
    double       x_axis;
    
    double       x_major_tick;

    unsigned int x_major_tick_length,  x_minor_tick_length,
                 x_num_minor_ticks,    legend_title_font_size;

    // border information for the plot window. Initially will be set to the width
    // and height of the graph
    int plot_window_x1, plot_window_x2, 
        plot_window_y1, plot_window_y2;

    // Yes/no questions
    bool legend_on;
    bool axis_on;
    bool plot_window_on;
    bool x_label_on;
    bool x_major_labels_on;

    // internal helper functions
    void _transform_point(double &x);
    void _clear_legend();
    void _draw_legend_header(int, int, int);
    void _draw_legend();
    void _draw_axis();
    void _draw_x_label();

    svg_plot(const svg_plot&);
    
    svg_plot& operator=(const svg_plot&);
public:

    // constructors
    svg_plot();
    svg_plot(const std::string& file);

    // output
    svg_plot& write(const std::string&);
    svg_plot& write(std::ostream&);

    
    // plot functions
    void plot_range(std::vector<double>::const_iterator,
        std::vector<double>::const_iterator, const std::string&);
    
    void plot_range(std::vector<double>::const_iterator, 
        std::vector<double>::const_iterator, const std::string&, svg_color_constant);

    //setters

    // misc
    svg_plot& set_image_size(unsigned int, unsigned int);
    svg_plot& set_title(const std::string&);
    svg_plot& set_title_font_size(unsigned int);
    svg_plot& set_legend_title_font_size(unsigned int);

    // commands
    svg_plot& set_axis(bool);
    svg_plot& set_legend(bool);
    svg_plot& set_plot_window(bool);
    svg_plot& set_x_label(bool);
    svg_plot& set_x_major_labels(bool);

    // color information    
    svg_plot& set_title_color(svg_color_constant);
    svg_plot& set_title_color(const svg_color&);

    svg_plot& set_background_color(svg_color_constant);
    svg_plot& set_background_color(const svg_color&);

    svg_plot& set_legend_background_color(svg_color_constant);
    svg_plot& set_legend_background_color(const svg_color&);

    svg_plot& set_plot_background_color(svg_color_constant);
    svg_plot& set_plot_background_color(const svg_color&);
    
    svg_plot& set_x_axis_color(svg_color_constant);
    svg_plot& set_x_axis_color(const svg_color&);

    svg_plot& set_x_major_tick_color(svg_color_constant);
    svg_plot& set_x_major_tick_color(const svg_color&);

    svg_plot& set_x_minor_tick_color(svg_color_constant);
    svg_plot& set_x_minor_tick_color(const svg_color&);

    // axis information
    svg_plot& set_x_scale(double, double);

    svg_plot& set_x_axis_width(unsigned int);

    svg_plot& set_x_major_tick(double);
    svg_plot& set_x_major_tick_length(unsigned int);
    svg_plot& set_x_minor_tick_length(unsigned int);
    svg_plot& set_x_num_minor_ticks(unsigned int);        
    svg_plot& set_x_label_text(const std::string&);
    svg_plot& set_x_major_tick_width(unsigned int);
    svg_plot& set_x_minor_tick_width(unsigned int);

    // getters
    const std::string& get_title();
    unsigned int get_title_font_size();

    svg_color get_background_color();
    svg_color get_legend_background_color();
    svg_color get_axis_color();

    unsigned int get_axis_width();
};

svg_plot::svg_plot(): x_label(""), x_min(-10), x_max(10), plot_window_x1(0), 
                      plot_window_y1(0), plot_window_x2(100), 
                      plot_window_y2(100), x_axis(50), legend_on(false),
                      axis_on(false), x_minor_tick_length(10), 
                      legend_title_font_size(12)
{
    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            transform_matrix[i][j] = 0;
        }
    }
    
    //to determine: reasonable default values
    set_image_size(100, 100);

    //build the document tree.. add children of the root node
    for(int i=0; i<SVG_PLOT_DOC_CHILDREN; ++i)
    {
        image.add_g_element();
    }
}


svg_plot& svg_plot::write(const std::string& _str)
{
    // Hold off drawing the legend until the very end.. it's
    // easier to draw the size that it needs at the end than
    // it is to 
    // Don't bother with re-adding things if we don't need to

    int x_size = image.get_x_size();
    int y_size = image.get_y_size();
   
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

        if(axis_on)
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

    transform_matrix[0][0] = (plot_window_x2-plot_window_x1)/(x_max-x_min);
    transform_matrix[0][2] = plot_window_x1 - (x_min *(plot_window_x2-plot_window_x1)/(x_max-x_min));
        
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

    double x1(0);

    //draw points
    for(unsigned int i=0; i<series.size(); ++i)
    {
        g_element& g_ptr = image.get_g_element(PLOT_PLOT_POINTS).add_g_element();
        g_ptr.set_fill_color(series[i].style);

        for(unsigned int j=0; j<series[i].series.size(); ++j)
        {
            x1 = series[i].series[j];

            _transform_point(x1);
            
            if(x1 > plot_window_x1 && x1 < plot_window_x2)
            {
                image.point(x1, x_axis, g_ptr);
            }
        }
    }

    image.write(_str);

    return *this;
}

svg_plot& svg_plot::write(std::ostream& s_out)
{
    if(legend_on)
    {
        _draw_legend();
    }

    if(axis_on)
    {
        _draw_axis();
    }
    
    image.write(s_out);
    
    return (svg_plot&)*this;
}

template <class iter>
void plot_range(svg_plot& _cont, iter _begin, iter _end, std::string _str)
{
    std::vector<double> vect(_begin, _end);

    _cont.plot_range(vect.begin(), vect.end(), _str);
}

template <class iter>
void plot_range(svg_plot& _cont, iter _begin, iter _end, std::string _str,
                      svg_color_constant _col)
{
    std::vector<double> vect(_begin, _end);

    _cont.plot_range(vect.begin(), vect.end(), _str, _col);
}

// -----------------------------------------------------------------
// Actually draw data to the plot. Default color information
// -----------------------------------------------------------------
void svg_plot::plot_range(std::vector<double>::const_iterator begin,
                            std::vector<double>::const_iterator end,
                            const std::string& _str)
{
    series.push_back(svg_plot_series(begin, end, _str, svg_color(0,0,0)));
}

// -----------------------------------------------------------------
// Actually draw data to the plot. Fill color information provided
// -----------------------------------------------------------------
void svg_plot::plot_range(std::vector<double>::const_iterator begin,
                            std::vector<double>::const_iterator end,
                            const std::string& _str,
                            svg_color_constant _col)
{
    series.push_back(svg_plot_series(begin, end, _str, constant_to_rgb(_col)));
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
svg_plot& svg_plot::set_image_size(unsigned int x, unsigned int y)
{
    image.image_size(x, y);
    
    return *this;
}

svg_plot& svg_plot::set_title(const std::string& _title)
{
    text_element title(image.get_x_size()/2., 30, _title);

    title.set_alignment(center_align);

    image.get_g_element(PLOT_TITLE).push_back(new text_element(title));

    return *this;
}

svg_plot& svg_plot::set_title_font_size(unsigned int _size)
{
    text_element* t_ptr = static_cast<text_element*>(
                    &(image.get_g_element(PLOT_TITLE)[0]));

    t_ptr->set_font_size(_size);

    return *this;
}

svg_plot& svg_plot::set_legend_title_font_size(unsigned int _size)
{
    legend_title_font_size = _size;

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
// set_x_label(): Whether or not the label for the X axis will show
//
// set_x_major_labels(): Whether or not the major ticks will have labels
// 			 on the x-axis
//
//  
// -----------------------------------------------------------------

svg_plot& svg_plot::set_axis(bool _cmd)
{
    axis_on = _cmd;
    return (svg_plot&)*this;
}

svg_plot& svg_plot::set_legend(bool _cmd)
{
    legend_on = _cmd;

    return (svg_plot&)*this;
}

svg_plot& svg_plot::set_plot_window(bool _cmd)
{
    plot_window_on = _cmd;

    return (svg_plot&)*this;
}

svg_plot& svg_plot::set_x_label(bool _cmd)
{
    x_label_on = _cmd;

    return *this;
}

svg_plot& svg_plot::set_x_major_labels(bool _cmd)
{
    x_major_labels_on = _cmd;

    return *this;
}

svg_plot& svg_plot::set_title_color(svg_color_constant _col)
{
    set_title_color(constant_to_rgb(_col));

    return *this;
}

svg_plot& svg_plot::set_title_color(const svg_color& _col)
{
    image.get_g_element(PLOT_TITLE).set_stroke_color(_col);
    image.get_g_element(PLOT_TITLE).set_fill_color(_col);

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
svg_plot& svg_plot::set_background_color(svg_color_constant _col)
{
    set_background_color(constant_to_rgb(_col));

    return *this;
}

svg_plot& svg_plot::set_background_color(const svg_color& _col)
{
    image.get_g_element(PLOT_BACKGROUND).set_fill_color(_col);

    image.get_g_element(PLOT_BACKGROUND).clear();
    image.get_g_element(PLOT_BACKGROUND).push_back(
                     new rect_element(0, 0, image.get_x_size(), 
                     image.get_y_size()));

    return *this;
}

svg_plot& svg_plot::set_legend_background_color(svg_color_constant _col)
{
    set_legend_background_color(constant_to_rgb(_col));

    return (svg_plot&)*this;
}

svg_plot& svg_plot::set_legend_background_color(const svg_color& _col)
{
    image.get_g_element(PLOT_LEGEND_BACKGROUND)
            .set_fill_color(_col);

    return *this;
}

svg_plot& svg_plot::set_plot_background_color(svg_color_constant _col)
{
    image.get_g_element(PLOT_PLOT_BACKGROUND).set_fill_color(_col);

    return *this;
}

svg_plot& svg_plot::set_plot_background_color(const svg_color& _col)
{
    image.get_g_element(PLOT_PLOT_BACKGROUND).set_fill_color(_col);

    return *this;
}

svg_plot& svg_plot::set_x_axis_color(svg_color_constant _col)
{
    set_x_axis_color(constant_to_rgb(_col));

    return (svg_plot&)*this;
}

svg_plot& svg_plot::set_x_axis_color(const svg_color& _col)
{
    image.get_g_element(PLOT_PLOT_AXIS)
            .set_fill_color(_col);

    image.get_g_element(PLOT_PLOT_AXIS)
            .set_stroke_color(_col);

    return *this;
}

svg_plot& svg_plot::set_x_major_tick_color(const svg_color& _col)
{
    image.get_g_element(PLOT_X_MAJOR_TICKS).set_stroke_color(_col);
    image.get_g_element(PLOT_X_MAJOR_TICKS).set_fill_color(_col);
    return *this;
}

svg_plot& svg_plot::set_x_major_tick_color(svg_color_constant _col)
{
    set_x_major_tick_color(constant_to_rgb(_col));
    return *this;
}

svg_plot& svg_plot::set_x_minor_tick_color(const svg_color& _col)
{
    image.get_g_element(PLOT_X_MINOR_TICKS).set_stroke_color(_col);
    image.get_g_element(PLOT_X_MINOR_TICKS).set_fill_color(_col);
    return *this;
}


svg_plot& svg_plot::set_x_minor_tick_color(svg_color_constant _col)
{
    set_x_minor_tick_color(constant_to_rgb(_col));
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
// -----------------------------------------------------------------

svg_plot& svg_plot::set_x_scale(double x1, double x2)
{
    x_min = x1;
    x_max = x2;

    if(x2 <= x1)
    {
        throw "Illegal Argument: X scale: x2 < x1";
    }

    return (svg_plot&)*this;
}

svg_plot& svg_plot::set_x_axis_width(unsigned int _width)
{
    image.get_g_element(PLOT_PLOT_AXIS).set_stroke_width(_width);

    return *this;
}

svg_plot& svg_plot::set_x_major_tick(double _inter)
{
    x_major_tick = _inter;

    return *this;
}

svg_plot& svg_plot::set_x_major_tick_length(unsigned int _length)
{
    x_major_tick_length = _length;
    return *this;
}

svg_plot& svg_plot::set_x_minor_tick_length(unsigned int _length)
{
    x_minor_tick_length = _length;
    return *this;
}

svg_plot& svg_plot::set_x_num_minor_ticks(unsigned int _num)
{
    x_num_minor_ticks = _num;
    return *this;
}

svg_plot& svg_plot::set_x_label_text(const std::string& _str)
{
    x_label = _str;

    return *this;
}

svg_plot& svg_plot::set_x_major_tick_width(unsigned int _width)
{
    image.get_g_element(PLOT_X_MAJOR_TICKS).set_stroke_width(_width);

    return *this;
}

svg_plot& svg_plot::set_x_minor_tick_width(unsigned int _width)
{
    image.get_g_element(PLOT_X_MINOR_TICKS).set_stroke_width(_width);

    return *this;
}

// -----------------------------------------------------------------
// We don't use the SVG coordinate transform because then text would
// be flipped. I'm considering using it to scale the image for resizes
// -----------------------------------------------------------------
void svg_plot::_transform_point(double &x)
{
    x = transform_matrix[0][0] * x + transform_matrix[0][2];
}

//refactor
void svg_plot::_draw_axis()
{
    // one major axis. We just need to draw a vertical line through
    // the origin for now. We will make that an option later.
    double x1, y1, y2;

    x_axis = (plot_window_y1 + plot_window_y2) / 2.;

    image.line(plot_window_x1, x_axis, plot_window_x2, x_axis, 
        image.get_g_element(PLOT_PLOT_AXIS));

    x1 = 0;

    _transform_point(x1);
    
    image.line(x1, plot_window_y1, x1, plot_window_y2, 
        image.get_g_element(PLOT_PLOT_AXIS));

    int major_tick_len = x_major_tick_length/2;

    for(double i = 0; i < x_max; i += x_major_tick)
    {
        for(double j=i; j<i+x_major_tick; j+=x_major_tick / (x_num_minor_ticks+1))
        {
            y1 = x_axis + x_minor_tick_length/2.;
            y2 = x_axis - x_minor_tick_length/2.;

            x1=j;

            _transform_point(x1);

            //make sure that we are drawing inside of the allowed window
            if(x1 < plot_window_x2)
            {
                image.line(x1, y1, x1, y2, 
                    image.get_g_element(PLOT_X_MINOR_TICKS));
            }
        }

        y1 = x_axis + x_major_tick_length/2;
        y2 = x_axis - x_major_tick_length/2;
    
        x1=i;

        _transform_point(x1);

        //make sure that we are drawing inside of the allowed window
        if(x1 < plot_window_x2)
        {
            image.line(x1, y1, x1, y2, 
                image.get_g_element(PLOT_X_MAJOR_TICKS));

            
            if(x_major_labels_on)
            {
                std::stringstream fmt;
                fmt<<i;

                image.text(x1, y1 + (2 + x_major_tick_length/2), fmt.str());
            }
        }
    }

    for(double i = 0; i > x_min; i -= x_major_tick)
    {
        for(double j=i; j>i-x_major_tick; j-=x_major_tick / (x_num_minor_ticks+1))
        {
            y1 = x_axis + x_minor_tick_length/2.;
            y2 = x_axis - x_minor_tick_length/2.;

            x1=j;

            _transform_point(x1);

            //make sure that we are drawing inside of the allowed window
            if(x1 > plot_window_x1)
            {
                image.line(x1, y1, x1, y2, 
                    image.get_g_element(PLOT_X_MINOR_TICKS));
            }
        }
        y1 = x_axis+major_tick_len;
        y2 = x_axis-major_tick_len;
    
        x1=i;

        _transform_point(x1);

        if(x1 > plot_window_x1)
        {
            image.line(x1, y1, x1, y2, 
                image.get_g_element(PLOT_X_MAJOR_TICKS));

            if(x_major_labels_on)
            {
                std::stringstream fmt;
                fmt<<i;

                image.text(x1, y1 + (2 + x_major_tick_length/2), fmt.str());
            }
        }
    }
}

// -----------------------------------------------------------------
// When writing to multiple documents, the contents of the plot
// may change significantly between. Rather than figuring out what
// has and has not changed, just erase the contents of the legend
// in the document and start over.
// -----------------------------------------------------------------
void svg_plot::_clear_legend()
{
    g_element* g_ptr = &(image.get_g_element(PLOT_LEGEND_POINTS));

    g_ptr->clear();

    g_ptr = &(image.get_g_element(PLOT_LEGEND_TEXT));
    g_ptr->clear();
}

// -----------------------------------------------------------------
// Factored out to make _draw_legend() cleaner
//
// This function has some "magic" values that could be removed
// or abstracted
// -----------------------------------------------------------------
void svg_plot::_draw_legend_header(int _x, int _y, int _width)
{
    // 2 added to y argument for padding.
    text_element legend_header(_x+(_width/2), _y + legend_title_font_size + 2, "Legend");

    legend_header.set_alignment(center_align);
    legend_header.set_font_size(legend_title_font_size);

    image.get_g_element(PLOT_LEGEND_TEXT).push_back(new text_element(legend_header));
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
void svg_plot::_draw_legend()
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

    int legend_x_start(plot_window_x2 + 5);
    int legend_y_start(plot_window_y1);

    // legend_height = title_spacing + (space per element)(num_elements)
    //                  + (end spacing)
    legend_height = (int)(legend_title_font_size*1.5 + (25 * num_points) + 10);

    // TODO: Figure out how tall the legend should be

    image.get_g_element(PLOT_LEGEND_BACKGROUND).set_stroke_color(svg_color(102, 102, 84));
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

void svg_plot::_draw_x_label()
{
    text_element to_use((plot_window_x2 + plot_window_x1) / 2., image.get_y_size() - 8, x_label);

    to_use.set_font_size(12);
    to_use.set_alignment(center_align);

    image.get_g_element(PLOT_X_LABEL).set_stroke_color(white);
    image.get_g_element(PLOT_X_LABEL).set_fill_color(white);


    image.get_g_element(PLOT_X_LABEL).push_back(new text_element(to_use));
}

const std::string& svg_plot::get_title()
{
    return title;
}

unsigned int svg_plot::get_title_font_size()
{
    return (static_cast<text_element*>(
                &(image.get_g_element(PLOT_TITLE)[0])))->get_font_size();
    return 0;
}

svg_color svg_plot::get_background_color()
{
    return image.get_g_element(PLOT_BACKGROUND).get_fill_color();
    return svg_color(0,0,0);
}

svg_color svg_plot::get_legend_background_color()
{
    return image.get_g_element(PLOT_LEGEND_BACKGROUND).get_fill_color();
    return svg_color(0,0,0);
}

svg_color svg_plot::get_axis_color()
{
    return image.get_g_element(PLOT_PLOT_AXIS).get_stroke_color();
    return svg_color(0,0,0);
}

unsigned int svg_plot::get_axis_width()
{
    return image.get_g_element(PLOT_PLOT_AXIS).get_stroke_width();
}


}
}

#endif

