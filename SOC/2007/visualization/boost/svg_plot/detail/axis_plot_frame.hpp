// svg_1d_plot.hpp

// Copyright (C) Jacob Voytko 2007
//
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

// ----------------------------------------------------------------- 

#ifndef _BOOST_SVG_AXIS_PLOT_FRAME_HPP
#define _BOOST_SVG_AXIS_PLOT_FRAME_HPP

#include <string>
#include "../svg_style.hpp"
#include "../svg.hpp"
#include "svg_tag.hpp"
#include "numeric_limits_handling.hpp"

namespace boost{
namespace svg{
namespace detail{

template <class Derived>
class axis_plot_frame
{
protected:
    // -----------------------------------------------------------------
    // We don't use the SVG coordinate transform because then text would
    // be flipped. I'm considering using it to scale the image for resizes.
    // -----------------------------------------------------------------
    void _transform_point(double &x, double &y)
    {
        x = derived().x_scale* x + derived().x_shift;
        y = derived().y_scale* y + derived().y_shift;

        _adjust_limits(x, y);
    }

    void _transform_x(double &x)
    {
        x = derived().x_scale* x + derived().x_shift;
    }

    void _transform_y(double &y)
    {
        y = derived().y_scale* y + derived().y_shift;
    }

    void _draw_x_minor_ticks(double j, path_element& tick_path,
                             path_element& grid_path)
    {
        double x1(0.), y1(0.), y2(derived().image.get_y_size());
        
        // draw the grid if needed
        if(derived().use_x_minor_grid)
        {
            _transform_x(x1 = j);

            if(!derived().use_plot_window)
            {
                // spacing for labels
                if(derived().use_title)
                {
                    y1 += derived().title_info.font_size() * 1.5;
                }

                if(derived().use_x_label)
                {
                    y2 -= derived().x_label_info.font_size() * 1.5;
                }
            }

            else
            {
                y1 = derived().plot_y1 + 1;
                y2 = derived().plot_y2 - 1;
            }

            grid_path.M(x1, y1).L(x1, y2);

        }

        double x_tick_length = derived().x_minor_length / 2.;

        if(derived().use_x_external_style)
        {
            y1 = derived().plot_y2;
            y2 = derived().plot_y2 + x_tick_length;
        }

        else
        {
            y1 = derived().x_axis + x_tick_length;
            y2 = derived().x_axis - x_tick_length;
        }
        x1=j;

        _transform_x(x1);

        //make sure that we are drawing inside of the allowed window
        if(x1 < derived().plot_x2 && x1 > derived().plot_x1)
        {
            tick_path.M(x1, y1).L(x1, y2);
        }
    }

    void _draw_x_major_ticks(double i, path_element& tick_path, path_element& grid_path)
    {
        double x1(i), y1(0.), y2(derived().image.get_x_size());

        if(derived().use_x_major_grid)
        {
            _transform_x(x1 = i);

            if(!derived().use_plot_window)
            {
                if(derived().use_title)
                {
                    y1 += derived().title_info.font_size() * 1.5;
                }

                if(derived().use_x_label)
                {
                    y2 -= derived().x_label_info.font_size() * 1.5;
                }
            }

            else
            {
                y1 = derived().plot_y1;
                y2 = derived().plot_y2;
            }
            grid_path.M(x1, y1).L(x1, y2);

        }

        //draw major tick
        x1=i;

        _transform_x(x1);

        //make sure that we are drawing inside of the allowed window
        if(x1 < derived().plot_x2 && x1 > derived().plot_x1)
        {
            double x_tick_length = derived().x_major_length / 2.;
            
            if(derived().use_x_external_style)
            {
                y1 = derived().plot_y2;
                y2 = derived().plot_y2 + x_tick_length;
            }

            else
            {
                y1 = derived().x_axis + x_tick_length;
                y2 = derived().x_axis - x_tick_length;
            }
            
            tick_path.M(x1, y1).L(x1, y2);

            if(derived().use_x_major_labels)
            {
                std::stringstream fmt;
                fmt<<i;

                if(derived().use_x_external_style)
                {
                    y1 += derived().x_major_length;
                }

                else
                {
                    y1 += (2 + derived().x_major_length/2);
                }

                derived().image.get_g_element(PLOT_PLOT_LABELS).text(x1, 
                    y1, fmt.str());
            }
        }
    }

    void _draw_x_axis()
    {
        double y1(0.);

        // draw the axis line
        _transform_y(y1);

        derived().x_axis = y1;

        path_element& minor_tick_path = 
            derived().image.get_g_element(PLOT_X_MINOR_TICKS).path();

        path_element& major_tick_path = 
            derived().image.get_g_element(PLOT_X_MAJOR_TICKS).path();

        path_element& minor_grid_path =
            derived().image.get_g_element(PLOT_X_MINOR_GRID).path();

        path_element& major_grid_path =
            derived().image.get_g_element(PLOT_X_MAJOR_GRID).path();

        if(derived().show_x_axis_lines)
        {
        derived().image.get_g_element(PLOT_X_AXIS).line(derived().plot_x1, derived().x_axis, 
            derived().plot_x2, derived().x_axis);
        }

        // x_minor_jump is the interval between minor ticks
        double x_minor_jump = derived().x_major/
                              ((double)(derived().x_num_minor + 1.) );

        // draw the ticks on the positive side
        for(double i = 0; i < derived().x_max; i += derived().x_major)
        {
            for(double j = i + x_minor_jump; 
                       j < i + derived().x_major; 
                       j += x_minor_jump)
            {
                _draw_x_minor_ticks(j, minor_tick_path, minor_grid_path);
            }

            _draw_x_major_ticks(i, major_tick_path, major_grid_path);
        }

        // draw the ticks on the negative side
        for(double i = 0; i > derived().x_min; i -= derived().x_major)
        {
            // draw minor ticks
            for(double j=i; j>i-derived().x_major; j-=derived().x_major / (derived().x_num_minor+1))
            {
                _draw_x_minor_ticks(j, minor_tick_path, minor_grid_path);
            }

            _draw_x_major_ticks(i, major_tick_path, major_grid_path);
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
        text_element legend_header(_x+(_width/2.), _y + derived().legend_title_size + 2, "Legend");

        legend_header.alignment(center_align);
        legend_header.font_size(derived().legend_title_size);

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

        unsigned int legend_x_start(derived().plot_x2 + 5);
        unsigned int legend_y_start(derived().plot_y1);

        if((unsigned int)(derived().plot_x2) >= derived().image.get_x_size())
        {
            legend_x_start-=160;
            legend_y_start+=5;
        }

        if(derived().use_title)
        {
            // -5 removes the padding
            legend_y_start += (int)(derived().title_info.font_size() * 1.5) - 5;
        }

        // legend_height = title_spacing + (space per element)(num_elements)
        //                  + (end spacing)
        legend_height = (int)(derived().legend_title_size*1.5 + (25 * num_points) + 10);

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

            g_inner_ptr->style().fill_color(derived().series[i].point_style.fill_color)
                                         .stroke_color(derived().series[i].point_style.stroke_color);

            _draw_plot_point(legend_x_start + 25, 
                             legend_y_start + derived().legend_title_size + 20 + i*25,
                             *g_inner_ptr,
                             derived().series[i].point_style);
            
            g_inner_ptr = &(derived().image.get_g_element(PLOT_LEGEND_TEXT));

            g_inner_ptr->push_back(new text_element(legend_x_start + 40,
                            legend_y_start + derived().legend_title_size + 25 + i*25,
                            derived().series[i].title, derived().legend_title_size, 
                            left_align));
        }
    }

    void _draw_title()
    {
        if(derived().use_title)
        {
            text_element title(derived().image.get_x_size()/2., 
                               derived().title_info.font_size(), 
                               derived().title_info.text());

            title.alignment(center_align);
            title.font_size(derived().title_info.font_size());
            derived().image.get_g_element(PLOT_TITLE).push_back(new text_element(title));
        }
    }

    void _draw_x_label()
    {
        text_element to_use((derived().plot_x2 + derived().plot_x1) / 2., 
            derived().image.get_y_size() - 8, derived().x_label_info.text());

        to_use.font_size(12);
        to_use.alignment(center_align);

        derived().image.get_g_element(PLOT_X_LABEL).push_back(new text_element(to_use));
    }

    void _adjust_limits(double& _x, double& _y)
    {
        if(detail::limit_max(_x))
        {
            _x = derived().plot_x2;
        }
        if(detail::limit_max(_y))
        {
            _y = derived().plot_y1;
        }
        if(detail::limit_min(_x))
        {
            _x = derived().plot_x1;
        }
        if(detail::limit_min(_y))
        {
            _y = derived().plot_y1;
        }
        if(detail::limit_NaN(_x))
        {
            _x = 0;
            _transform_x(_x);
        }
        if(detail::limit_NaN(_y))
        {
            _y = 0;
            _transform_y(_y);
        }
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

    Derived& image_size(unsigned int x, unsigned int y)
    {
        derived().image.image_size(x, y);
        return derived();
    }

    Derived& title(const std::string& _title)
    {
        derived().title_info.text(_title);
        return derived();
    }

    Derived& title_font_size(unsigned int _size)
    {
        derived().title_info.font_size(_size);
        return derived();
    }

    Derived& legend_title_font_size(unsigned int _size)
    {
        derived().legend_title_size = _size;
        return derived();
    }

    Derived& legend_on(bool _cmd)
    {
        derived().use_legend = _cmd;

        if(_cmd)
        {
            derived().image.get_g_element(detail::PLOT_LEGEND_BACKGROUND)
                .style().fill_color(white)
                                 .stroke_color(black);
        }
        return derived();
    }

    Derived& plot_window_on(bool _cmd)
    {
        derived().use_plot_window = _cmd;
        
        if(_cmd)
        {
            derived().image.get_g_element(detail::PLOT_PLOT_BACKGROUND)
                 .style().fill_color(white)
                                    .stroke_color(black);
        }
        return derived();
    }

    Derived& x_external_style_on(bool _cmd)
    {
        derived().use_x_external_style = _cmd;
        return derived();
    }

    Derived& x_label_on(bool _cmd)
    {
        derived().use_x_label = _cmd;
        return derived();
    }

    Derived& x_major_labels_on(bool _cmd)
    {
        derived().use_x_major_labels = _cmd;
        return derived();
    }

    Derived& title_on(bool _cmd)
    {
        derived().use_title = _cmd;
        return derived();
    }

    Derived& x_major_grid_on(bool _is)
    {
        derived().use_x_major_grid = _is;
        return derived();
    }

    Derived& x_minor_grid_on(bool _is)
    {
        derived().use_x_minor_grid = _is;
        return derived();
    }

    Derived& axis_on(bool _is)
    {
        derived().show_x_axis_lines = _is;
        derived().show_y_axis_lines = _is;
        return derived();
    }

    Derived& x_axis_on(bool _is)
    {
        derived().show_x_axis_lines = _is;
        return derived();
    }
    
    Derived& y_axis_on(bool _is)
    {
        derived().show_y_axis_lines = _is;
        return derived();
    }
    // -----------------------------------------------------------------
    // Color settings: Customization of colors found in the plot
    //
    // title_color()
    // background_color()
    // legend_background_color()
    // plot_background_color()
    // axis_color()
    // x_major_tick_color()
    // x_minor_tick_color()
    // -----------------------------------------------------------------

    Derived& title_color(const svg_color& _col)
    {
        derived().image.get_g_element(PLOT_TITLE).style().stroke_color(_col);
        derived().image.get_g_element(PLOT_TITLE).style().fill_color(_col);
        return derived();
    }

    Derived& background_color(const svg_color& _col)
    {
        derived().image.get_g_element(PLOT_BACKGROUND).style().fill_color(_col);
        return derived();
    }

    Derived& legend_background_color(const svg_color& _col)
    {
        derived().image.get_g_element(PLOT_LEGEND_BACKGROUND).style().fill_color(_col);
        return derived();
    }


    Derived& legend_border_color(const svg_color& _col)
    {
        derived().image.get_g_element(PLOT_LEGEND_BACKGROUND).style().stroke_color(_col);
        return derived();
    }

    Derived& background_border_color(const svg_color& _col)
    {
        derived().image.get_g_element(PLOT_BACKGROUND).style().stroke_color(_col);
        return derived();
    }

    Derived& plot_background_color(const svg_color& _col)
    {
        derived().image.get_g_element(PLOT_PLOT_BACKGROUND).style().fill_color(_col);
        return derived();
    }

    Derived& x_axis_color(const svg_color& _col)
    {
        derived().image.get_g_element(PLOT_X_AXIS).style()
                .fill_color(_col);

        derived().image.get_g_element(PLOT_X_AXIS).style()
                .stroke_color(_col);

        return derived();
    }

    Derived& x_major_tick_color(const svg_color& _col)
    {
        derived().image.get_g_element(PLOT_X_MAJOR_TICKS).style().stroke_color(_col);
        return derived();
    }

    Derived& x_label_color(const svg_color& _col)
    {
        derived().image.get_g_element(PLOT_X_LABEL).style().stroke_color(_col);
        return derived();
    }

    Derived& x_minor_tick_color(const svg_color& _col)
    {
        derived().image.get_g_element(PLOT_X_MINOR_TICKS).style().stroke_color(_col);
        return derived();
    }

    Derived& x_major_grid_color(const svg_color& _col)
    {
        derived().image.get_g_element(PLOT_X_MAJOR_GRID).style().stroke_color(_col);
        return derived();
    }

    Derived& x_minor_grid_color(const svg_color& _col)
    {
        derived().image.get_g_element(PLOT_X_MINOR_GRID).style().stroke_color(_col);
        return derived();
    }

    // -----------------------------------------------------------------
    // Axis information: Settings for customization of axis information
    //
    // x_axis_width()
    // x_major_tick()
    // x_major_tick_length()
    // x_major_tick_width()
    // x_minor_tick_length()
    // x_minor_tick_width()
    // x_label_text()
    // x_num_minor_ticks()
    // x_scale()
    // -----------------------------------------------------------------

    Derived& x_axis_width(unsigned int _width)
    {
        derived().image.get_g_element(PLOT_X_AXIS).style().stroke_width(_width);
        return derived();
    }

    Derived& x_label(const std::string& _str)
    {
        derived().x_label_info.text(_str);
        return derived();
    }

    Derived& x_major_interval(double _inter)
    {
        derived().x_major = _inter;
        return derived();
    }

    Derived& x_major_tick_length(unsigned int _length)
    {
        derived().x_major_length = _length;
        return derived();
    }

    Derived& x_major_tick_width(unsigned int _width)
    {
        derived().image.get_g_element(PLOT_X_MAJOR_TICKS).style().stroke_width(_width);
        return derived();
    }

    Derived& x_minor_tick_length(unsigned int _length)
    {
        derived().x_minor_length = _length;
        return derived();
    }

    Derived& x_minor_tick_width(unsigned int _width)
    {
        derived().image.get_g_element(PLOT_X_MINOR_TICKS).style().stroke_width(_width);
        return derived();
    }

    Derived& x_num_minor_ticks(unsigned int _num)
    {
        derived().x_num_minor = _num;
        return derived();
    }

    Derived& x_range(double x1, double x2)
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

    bool get_legend()
    {
        return derived().use_legend;
    }

    bool get_plot_window()
    {
        return derived().use_plot_window;
    }

    bool get_x_label()
    {
        return derived().use_x_label;
    }

    bool get_x_major_labels()
    {
        return derived().use_x_major_labels;
    }

    // color information    
    svg_color get_title_color()
    {
        return derived().image.get_g_element(PLOT_TITLE).style().get_fill_color();
    }

    svg_color get_background_color()
    {
        return derived().image.get_g_element(PLOT_BACKGROUND).style().get_fill_color();
    }

    svg_color get_background_border_color()
    {
        return derived().image.get_g_element(PLOT_BACKGROUND).style().get_stroke_color();
    }

    svg_color get_legend_background_color()
    {
        return derived().image.get_g_element(PLOT_LEGEND_BACKGROUND).style().get_fill_color();
    }

    svg_color get_legend_border_color()
    {
        return derived().image.get_g_element(PLOT_LEGEND_BACKGROUND).style().get_stroke_color();
    }

    svg_color get_plot_background_color()
    {
        return derived().image.get_g_element(PLOT_PLOT_BACKGROUND).style().get_fill_color();
    }   

    svg_color get_x_axis_color()
    {
        return derived().image.get_g_element(PLOT_X_AXIS).style().get_stroke_color();
    }

    svg_color get_x_label_color()
    {
        return derived().image.get_g_element(PLOT_X_LABEL).style().get_fill_color();
    }

    svg_color get_x_major_tick_color()
    {
        return derived().image.get_g_element(PLOT_X_MAJOR_TICKS).style().get_stroke_color();
    }

    svg_color get_x_minor_tick_color()
    {
        return derived().image.get_g_element(PLOT_X_MINOR_TICKS).style().get_stroke_color();
    }

    svg_color get_x_major_grid_color()
    {
        return derived().image.get_g_element(PLOT_X_MAJOR_GRID).style().get_stroke_color();
    }

    svg_color get_x_minor_grid_color()
    {
        return derived().image.get_g_element(PLOT_X_MINOR_GRID).style().get_stroke_color();
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
        return derived().image.get_g_element(PLOT_X_AXIS).style().get_stroke_width();
    }

    double get_x_major_tick()
    {
        return derived().x_major;
    }

    unsigned int get_x_major_tick_length()
    {
        return derived().x_major_length;
    }

    unsigned int get_x_minor_tick_length()
    {
        return derived().x_minor_length;
    }

    unsigned int get_x_num_minor_ticks()
    {
        return derived().x_num_minor;
    }  

    unsigned int get_x_major_tick_width()
    {
        return derived().image.get_g_element(PLOT_X_MAJOR_TICKS).style().get_stroke_width();
    }

    unsigned int get_x_minor_tick_width()
    {
        return derived().image.get_g_element(PLOT_X_MINOR_TICKS).style().get_stroke_width();
    }

    std::string get_x_label_text()
    {
        return derived().x_label;
    }

    svg& get_svg()
    {
        derived()._update_image();

        return derived().image;
    }

    const svg& get_svg() const
    {
        derived()._update_image();

        return derived().image;
    }
};

} // detail
} // svg
} // boost

#endif
