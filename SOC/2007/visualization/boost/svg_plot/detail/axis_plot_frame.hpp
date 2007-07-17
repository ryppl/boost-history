// svg_1d_plot.hpp

// Copyright (C) Jacob Voytko 2007
//
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

// ----------------------------------------------------------------- 

#ifndef _BOOST_SVG_AXIS_PLOT_FRAME_HPP
#define _BOOST_SVG_AXIS_PLOT_FRAME_HPP

#include <string>
#include "svg_style.hpp"
#include "svg_tag.hpp"

namespace boost{
namespace svg{

template <class Derived>
class axis_plot_frame
{
protected:
    // -----------------------------------------------------------------
    // We don't use the SVG coordinate transform because then text would
    // be flipped. I'm considering using it to scale the image for resizes
    // -----------------------------------------------------------------
    void _transform_point(double &x, double &y)
    {
        x = derived().x_scale* x + derived().x_shift;
        y = derived().y_scale* y + derived().y_shift;
    }

    void _transform_x(double &x)
    {
        x = derived().x_scale* x + derived().x_shift;
    }

    void _transform_y(double &y)
    {
        y = derived().y_scale* y + derived().y_shift;
    }

    void _draw_x_minor_ticks(double j)
    {
        double x1(0.), y1(0.), y2(derived().image.get_y_size());
        // draw the grid if needed
        if(derived().x_minor_grid_on)
        {
            _transform_x(x1 = j);

            if(!derived().plot_window_on)
            {
                // spacing for labels
                if(derived().title_on)
                {
                    y1 += derived().title_font_size * 1.5;
                }

                if(derived().x_label_on)
                {
                    y2 -= derived().x_label_font_size * 1.5;
                }
            }

            derived().image.get_g_element(PLOT_X_MINOR_GRID).line(x1, y1, x1, y2);
        }

        y1 = derived().x_axis + derived().x_minor_tick_length/2.;
        y2 = derived().x_axis - derived().x_minor_tick_length/2.;

        x1=j;

        _transform_x(x1);

        //make sure that we are drawing inside of the allowed window
        if(x1 < derived().plot_window_x2)
        {
            derived().image.get_g_element(PLOT_X_MINOR_TICKS).line(x1, y1, x1, y2);
        }
    }


    void _draw_x_major_ticks(double i)
    {
        double x1(i), y1(0.), y2(derived().image.get_x_size());

        if(derived().x_major_grid_on)
        {
            _transform_x(x1 = i);

            if(!derived().plot_window_on)
            {
                if(derived().title_on)
                {
                    y1 += derived().title_font_size * 1.5;
                }

                if(derived().x_label_on)
                {
                    y2 -= derived().x_label_font_size * 1.5;
                }
            }

            derived().image.get_g_element(PLOT_X_MAJOR_GRID).line(x1, y1, x1, y2);
        }

        //draw major tick
        x1=i;

        _transform_x(x1);

        //make sure that we are drawing inside of the allowed window
        if(x1 < derived().plot_window_x2)
        {
            y1 = derived().x_axis + derived().x_major_tick_length/2;
            y2 = derived().x_axis - derived().x_major_tick_length/2;
            
            derived().image.get_g_element(PLOT_X_MAJOR_TICKS).line(x1, y1, x1, y2);

            
            if(derived().x_major_labels_on)
            {
                std::stringstream fmt;
                fmt<<i;

                derived().image.get_g_element(PLOT_PLOT_LABELS).text(x1, 
                    y1 + (2 + derived().x_major_tick_length/2), fmt.str());
            }
        }
    }

    void _draw_x_axis()
    {
        double y1(0.);

        // draw the axis line
        _transform_y(y1);

        derived().x_axis = y1;

        derived().image.get_g_element(PLOT_X_AXIS).line(derived().plot_window_x1, derived().x_axis, 
            derived().plot_window_x2, derived().x_axis);

        // x_minor_jump is the interval between minor ticks
        double x_minor_jump = derived().x_major_tick/
                              ((double) (derived().x_num_minor_ticks + 1.) );

        // draw the ticks on the positive side
        for(double i = 0; i < derived().x_max; i += derived().x_major_tick)
        {
            for(double j = i + x_minor_jump; 
                       j < i + derived().x_major_tick; 
                       j += x_minor_jump)
            {
                _draw_x_minor_ticks(j);
            }

            _draw_x_major_ticks(i);
        }

        // draw the ticks on the negative side
        for(double i = 0; i > derived().x_min; i -= derived().x_major_tick)
        {
            // draw minor ticks
            for(double j=i; j>i-derived().x_major_tick; j-=derived().x_major_tick / (derived().x_num_minor_ticks+1))
            {
                _draw_x_minor_ticks(j);
            }

            _draw_x_major_ticks(i);
        }
    }

    // -----------------------------------------------------------------
    // When writing to multiple documents, the contents of the plot
    // may change significantly between. Rather than figuring out what
    // has and has not changed, just erase the contents of the legend
    // in the document and start over.
    // -----------------------------------------------------------------
    void _clear_all()
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

    void _clear_background()
    {
        derived().image.get_g_element(PLOT_BACKGROUND).clear();
    }

    void _clear_title()
    {
        derived().image.get_g_element(PLOT_TITLE).clear();
    }

    void _clear_points()
    {
        derived().image.get_g_element(PLOT_PLOT_POINTS).clear();
    }

    void _clear_plot_background()
    {
        derived().image.get_g_element(PLOT_PLOT_BACKGROUND).clear();
    }

    void _clear_legend()
    {
        derived().image.get_g_element(PLOT_LEGEND_BACKGROUND).clear();
        derived().image.get_g_element(PLOT_LEGEND_POINTS).clear();
        derived().image.get_g_element(PLOT_LEGEND_TEXT).clear();
    }

    void _clear_x_axis()
    {
        derived().image.get_g_element(PLOT_X_AXIS).clear();
        derived().image.get_g_element(PLOT_X_MINOR_TICKS).clear();
        derived().image.get_g_element(PLOT_X_MAJOR_TICKS).clear();
        derived().image.get_g_element(PLOT_X_LABEL).clear();
        derived().image.get_g_element(PLOT_PLOT_LABELS).clear();
    }

    void _clear_y_axis()
    {
        derived().image.get_g_element(PLOT_Y_AXIS).clear();
    }

    void _clear_grids()
    {
        derived().image.get_g_element(PLOT_X_MAJOR_GRID).clear();
        derived().image.get_g_element(PLOT_X_MINOR_GRID).clear();
    }

    void _draw_legend_header(int _x, int _y, int _width)
    {
        // 2 added to y argument for padding.
        text_element legend_header(_x+(_width/2.), _y + derived().legend_title_font_size + 2, "Legend");

        legend_header.set_alignment(center_align);
        legend_header.set_font_size(derived().legend_title_font_size);

        derived().image.get_g_element(PLOT_LEGEND_TEXT).push_back(new text_element(legend_header));
    }

    // -----------------------------------------------------------------
    // Important note: there are a lot of magic numbers that are temporary
    // fill-ins for the time when the legend system is more configurable.
    // This will happen bit-by-bit, as I give the user options to change
    // these values
    // -----------------------------------------------------------------
    void _draw_legend()
    {
        int num_points = (int)(derived().series.size());

        int legend_width(150);
        int legend_height(25);

        int x_size = derived().image.get_x_size();

        // Figure out how wide the legend should be
        if(x_size < 200)
        {
           legend_width = x_size; 
        }

        unsigned int legend_x_start(derived().plot_window_x2 + 5);
        unsigned int legend_y_start(derived().plot_window_y1);

        if((unsigned int)(derived().plot_window_x2) >= derived().image.get_x_size())
        {
            legend_x_start-=160;
            legend_y_start+=5;
        }

        if(derived().title_on)
        {
            // -5 removes the padding
            legend_y_start += (int)(derived().title_font_size * 1.5) - 5;
        }

        // legend_height = title_spacing + (space per element)(num_elements)
        //                  + (end spacing)
        legend_height = (int)(derived().legend_title_font_size*1.5 + (25 * num_points) + 10);

        g_element* g_ptr = &(derived().image.get_g_element(PLOT_LEGEND_BACKGROUND));

        g_ptr->push_back(new rect_element(legend_x_start, 
			                 legend_y_start,
                             legend_width, 
				             legend_height));

        _draw_legend_header(legend_x_start, legend_y_start, legend_width);

        g_ptr = &(derived().image.get_g_element(PLOT_LEGEND_POINTS));

        g_element* g_inner_ptr = g_ptr;

        for(unsigned int i=0; i<derived().series.size(); ++i)
        {
            g_inner_ptr = &(g_ptr->add_g_element());

            g_inner_ptr->get_style_info().set_fill_color(derived().series[i].point_style.fill_color)
                                         .set_stroke_color(derived().series[i].point_style.stroke_color);

            _draw_plot_point(legend_x_start + 25, 
                             legend_y_start + derived().legend_title_font_size + 20 + i*25,
                             *g_inner_ptr,
                             derived().series[i].point_style);
            
            g_inner_ptr = &(derived().image.get_g_element(PLOT_LEGEND_TEXT));

            g_inner_ptr->push_back(new text_element(legend_x_start + 40,
                            legend_y_start + derived().legend_title_font_size + 25 + i*25,
                            derived().series[i].title));
        }
    }

    void _draw_title()
    {
        if(derived().title_on)
        {
            text_element title(derived().image.get_x_size()/2., 
                               derived().title_font_size, 
                               derived().title);

            title.set_alignment(center_align);
            title.set_font_size(derived().title_font_size);
            derived().image.get_g_element(PLOT_TITLE).push_back(new text_element(title));
        }
    }

    void _draw_x_label()
    {
        text_element to_use((derived().plot_window_x2 + derived().plot_window_x1) / 2., 
            derived().image.get_y_size() - 8, derived().x_label);

        to_use.set_font_size(12);
        to_use.set_alignment(center_align);

        derived().image.get_g_element(PLOT_X_LABEL).push_back(new text_element(to_use));
    }

    void _draw_plot_point(double _x, double _y, 
                          g_element& g_ptr, const plot_point_style& _sty)
    {

        int size = _sty.size;
        double half_size = size / 2.;

        switch(_sty.shape)
        {
            case circle:
                g_ptr.circle(_x, _y, half_size);
                break;
            case square:
                g_ptr.rect(_x - half_size, _y - half_size, size, size);
                break;
        }
    }

private:
    Derived& derived() { return static_cast<Derived&>(*this); }
    const Derived& derived()const{return static_cast<const Derived&>(*this); }
public:

    Derived& set_image_size(unsigned int x, unsigned int y)
    {
        derived().image.image_size(x, y);
        return derived();
    }

    Derived& set_title(const std::string& _title)
    {
        derived().title = _title;
        return derived();
    }

    Derived& set_title_font_size(unsigned int _size)
    {
        derived().title_font_size = _size;
        return derived();
    }

    Derived& set_legend_title_font_size(unsigned int _size)
    {
        derived().legend_title_font_size = _size;
        return derived();
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

    Derived& set_axis_on(bool _cmd)
    {
        derived().axis_on = _cmd;
        return derived();
    }

    Derived& set_legend_on(bool _cmd)
    {
        derived().legend_on = _cmd;
        return derived();
    }

    Derived& set_plot_window_on(bool _cmd)
    {
        derived().plot_window_on = _cmd;
        return derived();
    }

    Derived& set_x_label_on(bool _cmd)
    {
        derived().x_label_on = _cmd;
        return derived();
    }

    Derived& set_x_major_labels_on(bool _cmd)
    {
        derived().x_major_labels_on = _cmd;
        return derived();
    }

    Derived& set_title_on(bool _cmd)
    {
        derived().title_on = _cmd;
        return derived();
    }

    Derived& set_x_major_grid_on(bool _is)
    {
        derived().x_major_grid_on = _is;
        return derived();
    }

    Derived& set_x_minor_grid_on(bool _is)
    {
        derived().x_minor_grid_on = _is;
        return derived();
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

    Derived& set_title_color(svg_color_constant _col)
    {
        set_title_color(constant_to_rgb(_col));
        return derived();
    }

    Derived& set_title_color(const svg_color& _col)
    {
        derived().image.get_g_element(PLOT_TITLE).get_style_info().set_stroke_color(_col);
        derived().image.get_g_element(PLOT_TITLE).get_style_info().set_fill_color(_col);
        return derived();
    }

    Derived& set_background_color(svg_color_constant _col)
    {
        set_background_color(constant_to_rgb(_col));
        return derived();
    }

    Derived& set_background_color(const svg_color& _col)
    {
        derived().image.get_g_element(PLOT_BACKGROUND).get_style_info().set_fill_color(_col);
        return derived();
    }

    Derived& set_legend_background_color(svg_color_constant _col)
    {
        set_legend_background_color(constant_to_rgb(_col));
        return derived();
    }

    Derived& set_legend_background_color(const svg_color& _col)
    {
        derived().image.get_g_element(PLOT_LEGEND_BACKGROUND).get_style_info().set_fill_color(_col);
        return derived();
    }

    Derived& set_legend_border_color(svg_color_constant _col)
    {
        set_legend_border_color(constant_to_rgb(_col));
        return derived();
    }

    Derived& set_legend_border_color(const svg_color& _col)
    {
        derived().image.get_g_element(PLOT_LEGEND_BACKGROUND).get_style_info().set_stroke_color(_col);
        return derived();
    }

    Derived& set_background_border_color(svg_color_constant _col)
    {
        derived().image.get_g_element(PLOT_BACKGROUND).get_style_info().set_stroke_color(_col);
        return derived();
    }

    Derived& set_background_border_color(const svg_color& _col)
    {
        derived().image.get_g_element(PLOT_BACKGROUND).get_style_info().set_stroke_color(_col);
        return derived();
    }

    Derived& set_plot_background_color(svg_color_constant _col)
    {
        derived().image.get_g_element(PLOT_PLOT_BACKGROUND).get_style_info().set_fill_color(_col);
        return derived();
    }

    Derived& set_plot_background_color(const svg_color& _col)
    {
        derived().image.get_g_element(PLOT_PLOT_BACKGROUND).get_style_info().set_fill_color(_col);
        return derived();
    }

    Derived& set_x_axis_color(svg_color_constant _col)
    {
        set_x_axis_color(constant_to_rgb(_col));
        return derived();
    }

    Derived& set_x_axis_color(const svg_color& _col)
    {
        derived().image.get_g_element(PLOT_X_AXIS).get_style_info()
                .set_fill_color(_col);

        derived().image.get_g_element(PLOT_X_AXIS).get_style_info()
                .set_stroke_color(_col);

        return derived();
    }

    Derived& set_x_major_tick_color(const svg_color& _col)
    {
        derived().image.get_g_element(PLOT_X_MAJOR_TICKS).get_style_info().set_stroke_color(_col);
        derived().image.get_g_element(PLOT_X_MAJOR_TICKS).get_style_info().set_fill_color(_col);
        return derived();
    }

    Derived& set_x_major_tick_color(svg_color_constant _col)
    {
        set_x_major_tick_color(constant_to_rgb(_col));
        return derived();
    }

    Derived& set_x_label_color(const svg_color& _col)
    {
        derived().image.get_g_element(PLOT_X_LABEL).get_style_info().set_stroke_color(_col);
        derived().image.get_g_element(PLOT_X_LABEL).get_style_info().set_fill_color(_col);
        return derived();
    }

    Derived& set_x_label_color(svg_color_constant _col)
    {
        set_x_label_color(constant_to_rgb(_col));
        return derived();
    }

    Derived& set_x_minor_tick_color(const svg_color& _col)
    {
        derived().image.get_g_element(PLOT_X_MINOR_TICKS).get_style_info().set_stroke_color(_col);
        derived().image.get_g_element(PLOT_X_MINOR_TICKS).get_style_info().set_fill_color(_col);
        return derived();
    }

    Derived& set_x_minor_tick_color(svg_color_constant _col)
    {
        set_x_minor_tick_color(constant_to_rgb(_col));
        return derived();
    }

    Derived& set_x_major_grid_color(const svg_color& _col)
    {
        derived().image.get_g_element(PLOT_X_MAJOR_GRID).get_style_info().set_stroke_color(_col);
        derived().image.get_g_element(PLOT_X_MAJOR_GRID).get_style_info().set_fill_color(_col);
        return derived();
    }

    Derived& set_x_major_grid_color(svg_color_constant _col)
    {
        set_x_major_grid_color(constant_to_rgb(_col));
        return derived();
    }

    Derived& set_x_minor_grid_color(const svg_color& _col)
    {
        derived().image.get_g_element(PLOT_X_MINOR_GRID).get_style_info().set_stroke_color(_col);
        derived().image.get_g_element(PLOT_X_MINOR_GRID).get_style_info().set_fill_color(_col);
        return derived();
    }

    Derived& set_x_minor_grid_color(svg_color_constant _col)
    {
        set_x_minor_grid_color(constant_to_rgb(_col));
        return derived();
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

    Derived& set_x_axis_width(unsigned int _width)
    {
        derived().image.get_g_element(PLOT_X_AXIS).get_style_info().set_stroke_width(_width);
        return derived();
    }

    Derived& set_x_label(const std::string& _str)
    {
        derived().x_label = _str;
        return derived();
    }

    Derived& set_x_major_tick(double _inter)
    {
        derived().x_major_tick = _inter;
        return derived();
    }

    Derived& set_x_major_tick_length(unsigned int _length)
    {
        derived().x_major_tick_length = _length;
        return derived();
    }

    Derived& set_x_major_tick_width(unsigned int _width)
    {
        derived().image.get_g_element(PLOT_X_MAJOR_TICKS).get_style_info().set_stroke_width(_width);
        return derived();
    }

    Derived& set_x_minor_tick_length(unsigned int _length)
    {
        derived().x_minor_tick_length = _length;
        return derived();
    }

    Derived& set_x_minor_tick_width(unsigned int _width)
    {
        derived().image.get_g_element(PLOT_X_MINOR_TICKS).get_style_info().set_stroke_width(_width);
        return derived();
    }

    Derived& set_x_num_minor_ticks(unsigned int _num)
    {
        derived().x_num_minor_ticks = _num;
        return derived();
    }

    Derived& set_x_scale(double x1, double x2)
    {
        if(x2 <= x1)
        {
            throw std::runtime_error("Illegal Argument: X scale: x2 < x1");
        }

        derived().x_min = x1;
        derived().x_max = x2;

        return derived();
    }

    unsigned int get_image_x_size()
    {
        return derived().image.get_x_size();
    }

    unsigned int get_image_y_size()
    {
        return derived().image.get_x_size();
    }

    std::string get_title()
    {
        return derived().title;
    }

    unsigned int get_legend_title_font_size()
    {
        return derived().legend_title_font_size;
    }

    // commands
    bool get_axis()
    {
        return derived().axis_on;
    }

    bool get_legend()
    {
        return derived().legend_on;
    }

    bool get_plot_window()
    {
        return derived().plot_window_on;
    }

    bool get_x_label()
    {
        return derived().x_label_on;
    }

    bool get_x_major_labels()
    {
        return derived().x_major_labels_on;
    }

    // color information    
    svg_color get_title_color()
    {
        return derived().image.get_g_element(PLOT_TITLE).get_style_info().get_fill_color();
    }

    svg_color get_background_color()
    {
        return derived().image.get_g_element(PLOT_BACKGROUND).get_style_info().get_fill_color();
    }

    svg_color get_background_border_color()
    {
        return derived().image.get_g_element(PLOT_BACKGROUND).get_style_info().get_stroke_color();
    }

    svg_color get_legend_background_color()
    {
        return derived().image.get_g_element(PLOT_LEGEND_BACKGROUND).get_style_info().get_fill_color();
    }

    svg_color get_legend_border_color()
    {
        return derived().image.get_g_element(PLOT_LEGEND_BACKGROUND).get_style_info().get_stroke_color();
    }

    svg_color get_plot_background_color()
    {
        return derived().image.get_g_element(PLOT_PLOT_BACKGROUND).get_style_info().get_fill_color();
    }   

    svg_color get_x_axis_color()
    {
        return derived().image.get_g_element(PLOT_X_AXIS).get_style_info().get_stroke_color();
    }

    svg_color get_x_label_color()
    {
        return derived().image.get_g_element(PLOT_X_LABEL).get_style_info().get_fill_color();
    }

    svg_color get_x_major_tick_color()
    {
        return derived().image.get_g_element(PLOT_X_MAJOR_TICKS).get_style_info().get_stroke_color();
    }

    svg_color get_x_minor_tick_color()
    {
        return derived().image.get_g_element(PLOT_X_MINOR_TICKS).get_style_info().get_stroke_color();
    }

    svg_color get_x_major_grid_color()
    {
        return derived().image.get_g_element(PLOT_X_MAJOR_GRID).get_style_info().get_stroke_color();
    }

    svg_color get_x_minor_grid_color()
    {
        return derived().image.get_g_element(PLOT_X_MINOR_GRID).get_style_info().get_stroke_color();
    }

    // axis information
    double get_x_min()
    {
        return derived().x_min;
    }

    double get_x_max()
    {
        return derived().x_max;
    }

    unsigned int get_x_axis_width()
    {
        return derived().image.get_g_element(PLOT_X_AXIS).get_style_info().get_stroke_width();
    }

    double get_x_major_tick()
    {
        return derived().x_major_tick;
    }

    unsigned int get_x_major_tick_length()
    {
        return derived().x_major_tick_length;
    }

    unsigned int get_x_minor_tick_length()
    {
        return derived().x_minor_tick_length;
    }

    unsigned int get_x_num_minor_ticks()
    {
        return derived().x_num_minor_ticks;
    }  

    unsigned int get_x_major_tick_width()
    {
        return derived().image.get_g_element(PLOT_X_MAJOR_TICKS).get_style_info().get_stroke_width();
    }

    unsigned int get_x_minor_tick_width()
    {
        return derived().image.get_g_element(PLOT_X_MINOR_TICKS).get_style_info().get_stroke_width();
    }

    std::string get_x_label_text()
    {
        return derived().x_label;
    }
};

}
}

#endif
