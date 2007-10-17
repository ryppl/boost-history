// svg_2d_plot.hpp

// Copyright (C) Jacob Voytko 2007
//
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

// -----------------------------------------------------------------

#ifndef _BOOST_SVG_SVG_2D_PLOT_HPP
#define _BOOST_SVG_SVG_2D_PLOT_HPP

#define BOOST_PARAMETER_MAX_ARITY 12

#include <boost/bind.hpp>

#if defined (BOOST_MSVC)
#  pragma warning(push)
#  pragma warning(disable: 4512) // "assignment operator could not be generated."
#  pragma warning(disable: 4127) // "conditional expression is constant."
#  pragma warning(disable: 4100) // "'boost_parameter_enabler_argument' : unreferenced formal parameter"
#endif

#include <boost/parameter/preprocessor.hpp>
#include <boost/parameter/name.hpp>
#include <boost/iterator/transform_iterator.hpp>

#include "svg_style.hpp"
#include "detail/axis_plot_frame.hpp"
#include "detail/numeric_limits_handling.hpp"
#include "detail/functors.hpp"
#include "svg.hpp"

#if defined (BOOST_MSVC)
#  pragma warning(pop)
#endif

#include <map>
#include <string>
#include <sstream>
#include <utility>
#include <ostream>
#include <iterator>
#include <exception>

namespace boost {
namespace svg {

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

BOOST_PARAMETER_NAME(bezier_on)
BOOST_PARAMETER_NAME(line_on)
BOOST_PARAMETER_NAME(line_color)
BOOST_PARAMETER_NAME(area_fill_color)

struct svg_2d_plot_series
{
    std::multimap<double, double> series;
    std::multimap<double, double> series_limits;

    std::string title;
    plot_point_style point_style;
    plot_line_style line_style;

    template <class T>
    svg_2d_plot_series(T _begin,
                       T _end,
                       std::string _title,
                       const plot_point_style& _point,
                       const plot_line_style& _line):
                       title(_title),
                       point_style(_point),
                       line_style(_line)
    {
        for(T i = _begin;
            i != _end;
            ++i)
        {
            if(detail::pair_is_limit(*i))
            {
                series_limits.insert(*i);
            }

            else
            {
                series.insert(*i);
            }
        }
    }
}; // struct svg_2d_plot_series

class svg_2d_plot: public detail::axis_plot_frame<svg_2d_plot>
{
private:
    friend class detail::axis_plot_frame<svg_2d_plot>;

    double x_scale, x_shift;
    double y_scale, y_shift;

    // Stored so as to avoid rewriting style information constantly.
    svg image;

    text_element title_info;
    text_element x_label_info;
    text_element y_label_info;

    // Border information for the plot window.
    // Initially will be set to the width and height of the graph.
    int plot_x1;
    int plot_x2;
    int plot_y1;
    int plot_y2;

    // X axis information.
    double       x_min,  x_max;
    double       x_axis;

    double       x_major;

    unsigned int x_major_length,  x_major_width,
                 x_minor_length, x_minor_width;
    unsigned int x_num_minor;
    unsigned int legend_title_size;

    // Yes/no questions.
    bool use_x_major_labels;
    bool use_x_major_grid;
    bool use_x_minor_grid;
    bool use_x_label;

    bool use_title;
    bool use_legend;
    bool use_axis;
    bool use_plot_window;
    bool use_x_external_style;
    bool use_y_external_style;
    bool show_x_axis_lines;
    bool show_y_axis_lines;
    bool use_y_major_grid;
    bool use_y_minor_grid;

    // Where we will be storing the data points for transformation.
    std::vector<svg_2d_plot_series> series;

    std::string plot_window_clip;

    // Y axis information.
    double       y_min,  y_max;

    double       y_major, y_axis;

    unsigned int y_major_length,  y_minor_length,
                 y_num_minor;

    bool use_y_label;
    bool use_y_major_labels;

    void _draw_y_minor_ticks(double j, path_element& tick_path,
                             path_element& grid_path)
    {
        double y1(0.), x1(0.), x2(image.get_y_size());

        // draw the grid if needed
        if(use_y_minor_grid)
        {
            _transform_y(y1 = j);

            if(!use_plot_window && use_y_label)
            {
                x2 -= 12 * 1.5;
            }

            else
            {
                x1 = plot_x1 + 1;
                x2 = plot_x2 - 1;
            }

            if(y1 > plot_y1)
            {
                grid_path.M(x1, y1).L(x2, y1);
            }
        }

        double y_tick_length = y_minor_length / 2.;

        if(use_y_external_style)
        {
            x1 = plot_x1;
            x2 = plot_x1 - y_tick_length / 2.;
        }

        else
        {
            x1 = y_axis + y_tick_length / 2.;
            x2 = y_axis - y_tick_length / 2.;
        }
        y1=j;

        _transform_y(y1);

        // Make sure that we are drawing inside of the allowed window.
        if(y1 < plot_y2 && y1 > plot_y1)
        {
            tick_path.M(x1, y1).L(x2, y1);
        }
    }

    void _draw_y_major_ticks(double i, path_element& tick_path, path_element& grid_path)
    {
        double y1(i), x1(0.), x2(image.get_y_size());

        if(use_y_major_grid)
        {
            _transform_y(y1 = i);

            if(!use_plot_window && use_y_label)
            {
                x1 += 12 * 1.5;
            }

            else
            {
                x1 = plot_x1 + 1;
                x2 = plot_x2 - 1;
            }

            grid_path.M(x1, y1).L(x2, y1);
        }

        //draw major tick
        y1=i;

        _transform_y(y1);

        // Make sure that we are drawing inside of the allowed window.
        if(y1 < plot_y2 && y1 > plot_y1)
        {
            double y_tick_length = y_major_length / 2.;

            if(use_y_external_style)
            {
                x1 = plot_x1;
                x2 = plot_x1 - y_tick_length/2.;
            }

            else
            {
                x1 = y_axis + y_tick_length/2.;
                x2 = y_axis - y_tick_length/2.;
            }

            tick_path.M(x1, y1).L(x2, y1);

            if(use_y_major_labels)
            {
                std::stringstream fmt;
                fmt<<i;

                if(use_y_external_style)
                {
                    x1 -= y_major_length;
                }

                else
                {
                    x1 += (2 + y_major_length/2);
                }

                if(!use_y_external_style && i != 0)
                {
                    image.get_g_element(detail::PLOT_PLOT_LABELS).text(x1,
                        y1, fmt.str());
                }

                if(use_y_external_style)
                {
                    image.get_g_element(detail::PLOT_PLOT_LABELS).text(x1 + 12,
                        y1, fmt.str(), 12, center_align, -90);

                }
            }
        }
    }

    void _draw_y_axis()
    {
        double x1(0.);

        // Draw the axis line.
        _transform_x(x1);
        image.get_g_element(detail::PLOT_Y_AXIS).line(x1, plot_y1, x1, plot_y2);

        y_axis = x1;

        path_element& minor_tick_path =
            image.get_g_element(detail::PLOT_Y_MINOR_TICKS).path();

        path_element& major_tick_path =
            image.get_g_element(detail::PLOT_Y_MAJOR_TICKS).path();

        path_element& minor_grid_path =
            image.get_g_element(detail::PLOT_Y_MINOR_GRID).path();

        path_element& major_grid_path =
            image.get_g_element(detail::PLOT_Y_MAJOR_GRID).path();

        //if(show_y_axis_lines)
        //{
            //image.get_g_element(detail::PLOT_Y_AXIS).
            //    line(plot_y1, x_axis, plot_x2, x_axis);
        //}

        // y_minor_jump is the interval between minor ticks.
        double y_minor_jump = y_major/((double)(y_num_minor + 1.) );

        // draw the ticks on the positive side
        for(double i = 0; i < y_max; i += y_major)
        {
            for(double j = i + y_minor_jump;
                       j < i + y_major;
                       j += y_minor_jump)
            {
                _draw_y_minor_ticks(j, minor_tick_path, minor_grid_path);
            }

            _draw_y_major_ticks(i, major_tick_path, major_grid_path);
        }

        // draw the ticks on the negative side.
        for(double i = 0; i > y_min; i -= y_major)
        {
            // draw minor ticks
            for(double j=i; j>i-y_major; j-=y_major / (y_num_minor+1))
            {
                _draw_y_minor_ticks(j, minor_tick_path, minor_grid_path);
            }

            _draw_y_major_ticks(i, major_tick_path, major_grid_path);
        }
    }

    void _draw_axis()
    {
        _draw_y_axis();
        _draw_x_axis();
    }

    void _draw_y_label()
    {
        image.get_g_element(detail::PLOT_Y_LABEL).style().stroke_color(black);

        image.get_g_element(detail::PLOT_Y_LABEL).push_back(new
            text_element(12, (plot_y2 + plot_y1)/2.,
            y_label_info.text(),
            12, center_align, -90));
    }


    void _calculate_transform()
    {
        x_scale = (plot_x2 - plot_x1) / (x_max - x_min);
        x_shift = plot_x1 - x_min *(plot_x2-plot_x1)/(x_max-x_min);

 	    y_scale = -(plot_y2-plot_y1)/(y_max-y_min);

 	    y_shift = plot_y1 - (y_max *(plot_y1-plot_y2)/(y_max-y_min));
    }

    void _calculate_plot_window()
    {
        plot_x1 = plot_y1 = 0;

        plot_x2 = image.get_x_size();
        plot_y2 = image.get_y_size();


        if(use_x_label)
        {
            plot_y2 -= (int)(x_label_info.font_size() * 1.5);
        }

        if(use_y_label)
        {
            plot_x1 += 20;
        }

        if(use_title)
        {
            plot_y1 += (int)(title_info.font_size() * 1.5);
        }

        if(use_plot_window)
        {
            // give the plot window a natural bit of padding
            plot_x1+=5;
            plot_x2-=5;
            plot_y1+=5;
            plot_y2-=5;


            if(use_legend)
            {
                plot_x2 -= 155;
            }

            if(use_y_external_style)
            {
                plot_x1 +=
                    y_major_length > y_minor_length ?
                    y_major_length :
                    y_minor_length ;
            }

            if(use_x_external_style)
            {
                plot_y2 -=
                    x_major_length > x_minor_length ?
                    x_major_length :
                    x_minor_length ;
            }

            image.get_g_element(detail::PLOT_PLOT_BACKGROUND).push_back(
                    new rect_element(plot_x1, plot_y1,
                            (plot_x2-plot_x1), plot_y2-plot_y1));
        }
    }

    void _transform_pair(std::pair<double, double>& pt)
    {
        _transform_point(pt.first, pt.second);
    }

    void _draw_bezier_lines(const svg_2d_plot_series& series)
    {
        g_element& g_ptr = image.get_g_element(detail::PLOT_PLOT_LINES)
                                .add_g_element();

        g_ptr.clip_id(plot_window_clip);

        g_ptr.style().stroke_color(series.line_style.color);
        path_element& path = g_ptr.path();

        std::pair<double, double> n_minus_2, n_minus_1, n;
        std::pair<double, double> fwd_vtr, back_vtr;

        if(series.line_style.area_fill == blank)
        {
                path._fill = false;
        }

        else
        {
            path.style().fill_color(series.line_style.area_fill);
        }

        if(series.series.size() > 2)
        {
            std::multimap<double,double>::const_iterator iter = series.series.begin();

            n_minus_1 = *(iter++);
            n = *(iter++);

            _transform_pair(n_minus_1);
            _transform_pair(n);

            path.M(n_minus_1.first, n_minus_1.second);

            for(; iter != series.series.end(); ++iter)
            {
                n_minus_2 = n_minus_1;
                n_minus_1 = n;
                n = *iter;

                _transform_pair(n);

                back_vtr.first = ((n_minus_1.first - n.first) +
                                 (n_minus_2.first - n_minus_1.first)) * .2;

                back_vtr.second = ((n_minus_1.second - n.second) +
                                  (n_minus_2.second - n_minus_1.second)) * .2;

                path.S(n_minus_1.first + back_vtr.first, n_minus_1.second + back_vtr.second,
                       n_minus_1.first, n_minus_1.second);
            }

            back_vtr.first = 0;
            back_vtr.second = (n.second - n_minus_1.second)*.2;

            path.S(n.first + back_vtr.first, n.second + back_vtr.second,
                   n.first, n.second);
        }

        else
        {
            _draw_straight_lines(series);
        }

    }

    void _draw_straight_lines(const svg_2d_plot_series& series)
    {
        double prev_x, prev_y, temp_x(0.), temp_y;

        g_element& g_ptr = image.get_g_element(detail::PLOT_PLOT_LINES).add_g_element();

        g_ptr.clip_id(plot_window_clip);

        g_ptr.style().stroke_color(series.line_style.color);

        path_element& path = g_ptr.path();

        if(series.series.size() > 1)
        {
            std::multimap<double, double>::const_iterator j = series.series.begin();
            prev_x = (*j).first;
            prev_y = 0.;

            // If we have to fill the area under the plot, we first have to
            // move from the X-axis to the first point.

            _transform_point(prev_x, prev_y);

            if(series.line_style.area_fill != blank)
            {
                path.style().fill_color(series.line_style.area_fill);
                path.M(prev_x, prev_y);
            }

            _transform_y(prev_y = (*j).second);

            if(series.line_style.area_fill != blank)
            {
                path.style().fill_color(series.line_style.area_fill);
                path.L(prev_x, prev_y);
            }

            else
            {
                path.M(prev_x, prev_y);
            }
            ++j;

            for(; j != series.series.end(); ++j)
            {
                temp_x = (*j).first;
                temp_y = (*j).second;

                _transform_point(temp_x, temp_y);

                path.L(temp_x, temp_y);

                if(series.line_style.area_fill == blank)
                {
                    path.M(temp_x, temp_y);
                }

                prev_x = temp_x;
                prev_y = temp_y;
            }

            if(series.line_style.area_fill != blank)
            {
                _transform_y(temp_y = 0.);
                path.L(temp_x, temp_y).z();
            }
        }
    }

    void _draw_plot_lines()
    {
        for(unsigned int i = 0; i < series.size(); ++i)
        {
            if(series[i].line_style.bezier_on)
            {
                _draw_bezier_lines(series[i]);
            }

            else
            {
                _draw_straight_lines(series[i]);
            }
        }
    }

    void _update_image()
    {
        _clear_all();

        // Draw background.
        image.get_g_element(detail::PLOT_BACKGROUND).push_back(
                     new rect_element(0, 0, image.get_x_size(),
                     image.get_y_size()));

        _draw_title();
        _calculate_plot_window();
        _calculate_transform();

        // Define the clip path for the plot window.
        // We don't want to allow overlap of the plot window lines, thus the
        // minor adjustments.

        image.clip_path(rect_element(plot_x1 + 1, plot_y1 + 1,
                                     plot_x2 - plot_x1 - 2, plot_y2 - plot_y1 - 2),
                        plot_window_clip);

        image.get_g_element(detail::PLOT_PLOT_POINTS).clip_id(plot_window_clip);

        if(use_axis)
        {
            _draw_y_axis();
            _draw_x_axis();
        }

        if(use_legend)
        {
            _draw_legend();
        }

        if(use_x_label)
        {
            _draw_x_label();
        }

        if(use_y_label)
        {
            _draw_y_label();
        }

        // Draw lines between non-limit points.

        _draw_plot_lines();

        // Draw non-limit points.
        double x(0.), y(0.);
        for(unsigned int i=0; i<series.size(); ++i)
        {
            g_element& g_ptr = image.get_g_element(detail::PLOT_PLOT_POINTS).add_g_element();

            g_ptr.style()
                 .fill_color(series[i].point_style.fill_color)
                 .stroke_color(series[i].point_style.stroke_color);

            for(std::multimap<double,double>::const_iterator j = series[i].series.begin();
                j!=series[i].series.end(); ++j)
            {
                x = j->first;
                y = j->second;

                _transform_point(x, y);

                if(x > plot_x1  && x < plot_x2
                && y > plot_y1  && y < plot_y2)
                {
                   _draw_plot_point(x, y, g_ptr, series[i].point_style);
                }
            }
        }

        // Draw limit points.
        for(unsigned int i=0; i<series.size(); ++i)
        {
            g_element& g_ptr = image.get_g_element(detail::PLOT_LIMIT_POINTS);

            for(std::multimap<double,double>::const_iterator j = series[i].series_limits.begin();
                j!=series[i].series_limits.end(); ++j)
            {
                x = j->first;
                y = j->second;

                _transform_point(x, y);

                _draw_plot_point(x, y, g_ptr, plot_point_style(blank, blank, 10, circle));
            }
        }
    }

public:

svg_2d_plot():        title_info(0, 0, "Plot of data", 30),
                      x_label_info(0, 0, "X Axis", 12),
                      y_label_info(0, 0, "Y Axis", 12),
                      x_min(-10), x_max(10),
                      y_min(-10), y_max(10),
                      x_major(3), x_num_minor(2),
                      y_major(3), use_y_label(false),
                      x_minor_length(10), x_major_length(20),
                      x_major_width(2), x_minor_width(1),
                      y_minor_length(10), y_num_minor(2),
                      y_major_length(20), legend_title_size(12),
                      plot_window_clip("__clip_window"),
                      use_x_major_labels(true), use_x_major_grid(false),
                      use_x_minor_grid(false), use_x_label(false),
                      use_title(true), use_legend(false), use_axis(true),
                      use_plot_window(false), use_y_major_labels(false),
                      use_x_external_style(false), use_y_external_style(false),
                      show_x_axis_lines(true), show_y_axis_lines(true),
                      use_y_major_grid(false), use_y_minor_grid(false)

{
    image_size(500, 350);

    // Build the document tree.. add children of the root node.
    for(int i=0; i<detail::SVG_PLOT_DOC_CHILDREN; ++i)
    {
        image.add_g_element();
    }

    // Set color defaults.
    image.get_g_element(detail::PLOT_BACKGROUND)
        .style().fill_color(white);

    image.get_g_element(detail::PLOT_LIMIT_POINTS)
        .style().stroke_color(lightgray).fill_color(whitesmoke);

    image.get_g_element(detail::PLOT_Y_AXIS)
        .style().stroke_color(black);

    image.get_g_element(detail::PLOT_X_AXIS)
        .style().stroke_color(black);

    image.get_g_element(detail::PLOT_X_MINOR_TICKS)
        .style().stroke_color(black);

    image.get_g_element(detail::PLOT_X_MAJOR_TICKS)
        .style().stroke_color(black);

    image.get_g_element(detail::PLOT_X_MAJOR_TICKS)
        .style().stroke_width(2);

    image.get_g_element(detail::PLOT_X_MINOR_TICKS)
        .style().stroke_width(1);

    image.get_g_element(detail::PLOT_Y_MINOR_TICKS)
        .style().stroke_color(black);

    image.get_g_element(detail::PLOT_Y_MAJOR_TICKS)
        .style().stroke_color(black);

    image.get_g_element(detail::PLOT_Y_MAJOR_TICKS)
        .style().stroke_width(2);

    image.get_g_element(detail::PLOT_Y_MINOR_TICKS)
        .style().stroke_width(1);

    image.get_g_element(detail::PLOT_X_MAJOR_TICKS).style().stroke_width(2);
    image.get_g_element(detail::PLOT_X_MINOR_TICKS).style().stroke_width(1);
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
    _update_image();

    image.write(s_out);

    return *this;
}

svg_2d_plot& y_name_on(bool _cmd)
{
    use_y_label = _cmd;
    return *this;
}

svg_2d_plot& y_major_labels_on(bool _cmd)
{
    use_y_major_labels = _cmd;
    return *this;
}

svg_2d_plot& y_axis_color(const svg_color& _col)
{
    image.get_g_element(detail::PLOT_Y_AXIS)
            .style().fill_color(_col);

    image.get_g_element(detail::PLOT_Y_AXIS)
            .style().stroke_color(_col);

    return *this;
}

svg_2d_plot& y_major_grid_color(const svg_color& _col)
{
    image.get_g_element(detail::PLOT_Y_MAJOR_GRID).style().stroke_color(_col);
    return *this;
}

svg_2d_plot& y_minor_grid_color(const svg_color& _col)
{
    image.get_g_element(detail::PLOT_Y_MINOR_GRID).style().stroke_color(_col);
    return *this;
}

svg_2d_plot& y_major_tick_color(const svg_color& _col)
{
    image.get_g_element(detail::PLOT_Y_MAJOR_TICKS).style().stroke_color(_col);
    return *this;
}

svg_2d_plot& y_minor_tick_color(const svg_color& _col)
{
    image.get_g_element(detail::PLOT_Y_MINOR_TICKS).style().stroke_color(_col);
    return *this;
}

svg_2d_plot& y_range(double y1, double y2)
{
    y_min = y1;
    y_max = y2;

    if(y2 <= y1)
    {
        throw std::runtime_error("Illegal Argument: X scale: x2 < x1");
    }

    return *this;
}

svg_2d_plot& y_axis_width(unsigned int _width)
{
    image.get_g_element(detail::PLOT_Y_AXIS).style().stroke_width(_width);

    return *this;
}

svg_2d_plot& y_major_interval(double _inter)
{
    y_major = _inter;

    return *this;
}

svg_2d_plot& y_major_tick_length(unsigned int _length)
{
    y_major_length = _length;
    return *this;
}

svg_2d_plot& y_minor_tick_length(unsigned int _length)
{
    y_minor_length = _length;
    return *this;
}

svg_2d_plot& y_num_minor_ticks(unsigned int _num)
{
    y_num_minor = _num;
    return *this;
}

svg_2d_plot& y_name(const std::string& _str)
{
    y_label_info.text(_str);
    return *this;
}

svg_2d_plot& y_major_tick_width(unsigned int _width)
{
    image.get_g_element(detail::PLOT_Y_MAJOR_TICKS).style().stroke_width(_width);

    return *this;
}

svg_2d_plot& y_minor_tick_width(unsigned int _width)
{
    image.get_g_element(detail::PLOT_Y_MINOR_TICKS).style().stroke_width(_width);

    return *this;
}

svg_2d_plot& y_label_on(bool _cmd)
{
    use_y_label = _cmd;
    return *this;
}

svg_2d_plot& axis_external_style(bool _is)
{
    use_x_external_style = _is;
    use_y_external_style = _is;
    return *this;
}

svg_2d_plot& x_external_style_on(bool _is)
{
    use_x_external_style = _is;

    return *this;
}

svg_2d_plot& y_external_style_on(bool _is)
{
    use_y_external_style = _is;
    return *this;
}

svg_2d_plot& y_major_grid_on(bool _is)
{
    use_y_major_grid = _is;
    return *this;
}

svg_2d_plot& y_minor_grid_on(bool _is)
{
    use_y_minor_grid = _is;
    return *this;
}

#if defined (BOOST_MSVC)
#  pragma warning(push)
#  pragma warning(disable: 4100) // "'boost_parameter_enabler_argument' : unreferenced formal parameter"
#endif

BOOST_PARAMETER_MEMBER_FUNCTION
(
    (void),
    plot,
    tag,
    (required
        (container, *)
        (title, (const std::string&))
    )
    (optional
        (fill_color, (const svg_color&), white)
        (stroke_color, (const svg_color&), black)
        (line_color, (const svg_color&), black)
        (area_fill_color, (svg_color_constant), blank)
        (point_style, (point_shape), circle)
        (size, (int), 10)
        (line_on, (bool), true)
        (bezier_on, (bool), false)
        (x_functor, *, detail::boost_default_2d_convert())
    )
)
{
    // This line has the error in GCC. Also, operator() in boost_default_2d_convert
    // has been changed so that the value of i is not updated. I would like
    // the start to be set, as well as i to update in operator().
    // x_functor.start(1.);

    plot_line_style line_style(line_color, line_on, bezier_on);

    line_style.area_fill=area_fill_color;

    series.push_back(
        svg_2d_plot_series(
        boost::make_transform_iterator(container.begin(), x_functor),
        boost::make_transform_iterator(container.end(),   x_functor),
        title,
        plot_point_style(fill_color, stroke_color, size, point_style),
        line_style
        ));
}
};

#if defined (BOOST_MSVC)
#  pragma warning(pop)
#endif


}
}

#endif
