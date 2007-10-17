// svg_1d_plot.hpp

// Copyright (C) Jacob Voytko 2007
// Copyright Paul A. Bristow 2007
// Added disable warning 4180 in Boost.Parameter Sep 2007.
//
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

// -----------------------------------------------------------------

#ifndef _BOOST_SVG_SVG_1D_PLOT_HPP
#define _BOOST_SVG_SVG_1D_PLOT_HPP

#define BOOST_PARAMETER_MAX_ARITY 12

#include <boost/bind.hpp>

#if defined (BOOST_MSVC)
#  pragma warning (push)
#  pragma warning (disable: 4512) // "assignment operator could not be generated."
#  pragma warning (disable: 4180) // qualifier applied to function type has no meaning; ignored
#endif

// See also pbristow trac ticket #1097 complaining about this ;-)
// trailing const qualifiers are said to be meaningless.

#include <boost/parameter/preprocessor.hpp>
#include <boost/parameter/name.hpp>
#include <boost/iterator/transform_iterator.hpp>

#include "svg.hpp"
#include "svg_style.hpp"
#include "detail/axis_plot_frame.hpp"
#include "detail/functors.hpp"

#if defined (BOOST_MSVC)
#  pragma warning(pop)
#endif

#include <vector>
#include <ostream>
#include <sstream>
#include <fstream>
#include <string>
#include <exception>


namespace boost {
namespace svg {

// -----------------------------------------------------------------
// Parameter names for plot() function
// -----------------------------------------------------------------

#if defined (BOOST_MSVC)
#  pragma warning(push)
#  pragma warning (disable: 4512) // "assignment operator could not be generated."
#  pragma warning (disable: 4180) // qualifier applied to function type has no meaning; ignored
#endif


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

#if defined (BOOST_MSVC)
#  pragma warning(pop)
#endif

// -----------------------------------------------------------------
// This allows us to store plot state locally in svg_plot. We don't
// store it in "svg" because transforming the points after they are
// written to the document would be difficult. We store the Cartesian
// coordinates locally and transform them before we write them.
// -----------------------------------------------------------------
struct svg_plot_series
{
    std::vector<double> series;
    std::vector<double> series_limits;

    std::string title;
    plot_point_style point_style;


    // -------------------------------------------------------------
    // Scan each data point between the iterators that are passed,
    // sorting them into the correct std::vector.
    // -------------------------------------------------------------
    template <class T>
    svg_plot_series(T _begin, T _end, const std::string& _title,
                    const plot_point_style& _style):
                    title(_title), point_style(_style)
    {
        double temp;

        for(T i = _begin;
            i != _end;
            ++i)
        {
            temp = *i;

            if(detail::is_limit(temp))
            {
                series_limits.push_back(temp);
            }

            else
            {
                series.push_back(temp);
            }
        }
    }
};

class svg_1d_plot: public detail::axis_plot_frame<svg_1d_plot>
{
protected:
    // stored so as to avoid rewriting style information constantly.
    svg image;

    text_element title_info;
    text_element x_label_info;

    // border information for the plot window. Initially will be set
    // to the width and height of the graph.
    int plot_x1, plot_y1,
        plot_x2, plot_y2;

                 // axis information.
    unsigned int x_major_length,  x_major_width,
                 x_minor_length,  x_minor_width, x_num_minor,

                 // misc information.
                 legend_title_size;

    // axis information.
    // (y_axis stored as one point because this is a 1D graph).
    double       x_min,  x_max;
    double       x_axis;

    double       x_major;

    // Yes/no questions
    bool use_x_major_labels;
    bool use_x_major_grid;
    bool use_x_minor_grid;
    bool use_x_label;

    bool use_title;
    bool use_legend;
    bool use_plot_window;
    bool use_x_external_style;
    bool show_x_axis_lines;
    bool show_y_axis_lines;

private:
    friend class detail::axis_plot_frame<svg_1d_plot>;

    // where we will be storing the data points for transformation.
    std::vector<svg_plot_series> series;

    double x_scale, x_shift;
    double y_scale, y_shift;


    void _calculate_transform()
    {
        x_scale = (plot_x2-plot_x1)/(x_max-x_min);
        x_shift = plot_x1 - (x_min *(plot_x2-plot_x1)/
                                   (x_max-x_min));

        y_scale = 1.;
        y_shift = plot_y1 - (plot_y1-plot_y2)/2.;
    }

    void _draw_axis()
    {
        double x(0.), y1(0.), y2(image.get_y_size());

        _transform_x(x);

        y1 = 0.;

        //draw origin. Make sure it is in the window
        if(x > plot_x1 && x < plot_x2 && show_y_axis_lines)
        {
            if(!use_plot_window)
            {
                if(use_title)
                {
                    y1 += title_info.font_size() * 1.5;
                }
                if(use_x_label)
                {
                    y2 -= x_label_info.font_size() * 1.5;
                }
            }

            else
            {
                y1 = plot_y1;
                y2 = plot_y2;
            }

            image.get_g_element(detail::PLOT_X_AXIS).line(x, y1, x, y2);
        }
        _draw_x_axis();
    }

    void _calculate_plot_window()
    {
        x_axis = (plot_y2 + plot_y1)/2.;

        plot_x1 = plot_y1 = 0;
        plot_x2 = image.get_x_size();
        plot_y2 = image.get_y_size();

        if(use_plot_window)
        {
            plot_x1+=5;
            plot_x2-=5;
            plot_y1+=5;
            plot_y2-=5;

            if(use_legend)
            {
                plot_x2 -= 155;
            }

            if(use_x_label)
            {
                plot_y2 -= 20;
            }

            if(use_x_external_style)
            {
                plot_y2 -=
                    x_major_length > x_minor_length ?
                    x_major_length :
                    x_minor_length ;
            }

            //for the title. Will take into account font size soon TODO
            plot_y1 +=40;

            image.get_g_element(detail::PLOT_PLOT_BACKGROUND).push_back(
                    new rect_element(plot_x1, plot_y1,
                            (plot_x2-plot_x1), plot_y2-plot_y1));
        }
    }

    void _update_image()
    {

        // removes all elements that will show up in a subsequent draw
        _clear_all();

        // draw background
        image.get_g_element(detail::PLOT_BACKGROUND).push_back(
                     new rect_element(0, 0, image.get_x_size(),
                     image.get_y_size()));

        _draw_title();
        _calculate_plot_window();
        _calculate_transform();

        if(x_axis)
        {
            _draw_axis();
        }

        if(use_legend)
        {
            _draw_legend();
        }

        if(use_x_label)
        {
            _draw_x_label();
        }

        double x(0), y(0);

        _transform_y(y);

        //draw points
        for(unsigned int i=0; i<series.size(); ++i)
        {
            g_element& g_ptr = image.get_g_element(detail::PLOT_PLOT_POINTS).add_g_element();

            g_ptr.style().fill_color(series[i].point_style.fill_color);
            g_ptr.style().stroke_color(series[i].point_style.stroke_color);

            for(unsigned int j=0; j<series[i].series.size(); ++j)
            {
                x = series[i].series[j];
                _transform_x(x);

                if(x > plot_x1
                && x < plot_x2
                && y > plot_y1
                && y < plot_y2)
                {
                    _draw_plot_point(x, y, g_ptr, series[i].point_style);
                }
            }
        }
    }

    void _set_ids()
    {
        for(int i=0; i<detail::SVG_PLOT_DOC_CHILDREN; ++i)
        {
            image.get_g_element(i).id(detail::_document_ids[i]);
        }
    }

public:

// see documentation for default settings rationale.
svg_1d_plot():        title_info(0, 0, "Plot of data", 30),
                      x_label_info(0, 0, "X Axis", 12),
                      x_min(-10), x_max(10),
                      use_legend(false), use_title(true),
                      use_plot_window(false), use_x_label(false),
                      use_x_major_grid(false), use_x_minor_grid(false),
                      use_x_external_style(false), show_x_axis_lines(true),
                      show_y_axis_lines(true),
                      x_major(3), x_minor_length(10),
                      x_major_length(20), x_num_minor(2),
                      legend_title_size(12),
                      x_scale(1.), x_shift(0),
                      y_scale(1.), y_shift(0)
{
    image_size(500, 350);

    //build the document tree.. add children of the root node
    for(int i=0; i<detail::SVG_PLOT_DOC_CHILDREN; ++i)
    {
        image.add_g_element();
    }

    // set color defaults
    image.get_g_element(detail::PLOT_BACKGROUND)
        .style().fill_color(white);

    image.get_g_element(detail::PLOT_Y_AXIS)
        .style().stroke_color(black);

    image.get_g_element(detail::PLOT_X_AXIS)
        .style().stroke_color(black);

    image.get_g_element(detail::PLOT_X_MINOR_TICKS)
        .style().stroke_color(black);

    image.get_g_element(detail::PLOT_X_MAJOR_TICKS)
        .style().stroke_color(black).stroke_width(2);

    image.get_g_element(detail::PLOT_LIMIT_POINTS)
        .style().stroke_color(lightgray).fill_color(whitesmoke);

    image.get_g_element(detail::PLOT_X_MINOR_TICKS)
        .style().stroke_width(1);

    _set_ids();
}

// -----------------------------------------------------------------
// write() has two flavors.. a file and a ostream. The file version
// opens a stream and calls the stream version. The stream version
// first clears all unnecessary data from the graph, builds the
// document tree, and then calls the write function for the root
// document node, which calls all other nodes through the Visitor
// pattern.
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
    _update_image();

    image.write(s_out);

    return (svg_1d_plot&)*this;
}

#if defined (BOOST_MSVC)
#  pragma warning(push)
#  pragma warning (disable: 4100) // 'args' : unreferenced formal parameter
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
        (stroke_color, (const svg_color&), svg_color(white))
        (point_style, (point_shape), circle)
        (size, (int), 10)
        (x_functor, *, detail::boost_default_convert())
    )
    (deduced
        (optional
            (fill_color, (const svg_color&), svg_color(black))
        )
    )
)
{
    series.push_back(svg_plot_series(
        boost::make_transform_iterator(container.begin(), x_functor),
        boost::make_transform_iterator(container.end(),   x_functor),
        title,
        plot_point_style(stroke_color, fill_color, size, point_style)
    ));
}

#if defined (BOOST_MSVC)
#  pragma warning(pop)
#endif

}; // end svg_1d_plot

}
}


#endif
