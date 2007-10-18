// svg_boxplot.hpp
// Copyright (C) Jacob Voytko 2007
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org
// -----------------------------------------------------------------

#ifndef BOOST_SVG_SVG_BOXPLOT_HPP
#define BOOST_SVG_SVG_BOXPLOT_HPP

#define BOOST_PARAMETER_MAX_ARITY 12

#include <boost/iterator/transform_iterator.hpp>

#if defined (BOOST_MSVC)
#  pragma warning(push)
#  pragma warning(disable: 4512) // "assignment operator could not be generated."
#  pragma warning(disable: 4127) // "conditional expression is constant."
#  pragma warning(disable: 4100) // "'boost_parameter_enabler_argument' : unreferenced formal parameter"
#endif

#include <boost/parameter/preprocessor.hpp>
#include <boost/parameter/name.hpp>

#include "svg_style.hpp"
#include "detail/numeric_limits_handling.hpp"
#include "detail/svg_boxplot_detail.hpp"
#include "detail/functors.hpp"
#include "svg.hpp"

#if defined (BOOST_MSVC)
#  pragma warning(pop)
#endif

#include <vector>
#include <string>
#include <ostream>
#include <algorithm>
#include <iterator>
#include <exception>

namespace boost
{
namespace svg
{

BOOST_PARAMETER_NAME((box_style,          keyword) box);
BOOST_PARAMETER_NAME((median_style,       keyword) median);
BOOST_PARAMETER_NAME((axis_style,         keyword) axis);
BOOST_PARAMETER_NAME((min_whisker_style,  keyword) min_whisker);
BOOST_PARAMETER_NAME((max_whisker_style,  keyword) max_whisker);
BOOST_PARAMETER_NAME((mild_outlier_style, keyword) mild_outlier);
BOOST_PARAMETER_NAME((ext_outlier_style,  keyword) ext_outlier);
BOOST_PARAMETER_NAME((whisker_length,     keyword) whisker);
BOOST_PARAMETER_NAME((box_width,          keyword) width);
BOOST_PARAMETER_NAME((functor,            keyword) fnctr);
BOOST_PARAMETER_NAME((ctr,                keyword) container);
BOOST_PARAMETER_NAME((name,              keyword)  title);

struct svg_boxplot_series
{
    double whisker_min, whisker_max, q1, q3, median;
    std::vector<double> outliers, extreme_outliers;
    std::string title;

    unsigned int whisker_length, box_width;

    svg_style box_style, median_style, axis_style,
              min_whisker_style, max_whisker_style;

    plot_point_style mild_outlier, ext_outlier;

    template <class T>
    svg_boxplot_series(T _begin, T _end, const std::string& _title,
        unsigned int _whisk_length, unsigned int _box_width,
        svg_style _box_style, svg_style _median_style, svg_style _axis_style,
        svg_style _min_whisker, svg_style _max_whisker,
        plot_point_style _mild_outlier, plot_point_style _ext_outlier):
        title(_title),
        whisker_length(_whisk_length),
        box_width(_box_width), box_style(_box_style),
        median_style(_median_style), axis_style(_axis_style),
        min_whisker_style(_min_whisker),
        max_whisker_style(_max_whisker),
        mild_outlier(_mild_outlier), ext_outlier(_ext_outlier)
    {
        //std::vector used for fast lookups of quartile values.
        std::vector<double> data(_begin, _end);

        if(data.empty())
        {
            throw std::runtime_error("Empty dataset");
        }

        std::sort(data.begin(), data.end());

        size_t data_size = data.size();

        // Pth quartile in n numbers is P(n+1)/100, rounded
        // to the nearest integer.
        q1 = data[(unsigned int)((data_size + 1) / 4. + 1)];
        median = data[(unsigned int)((data_size + 1) / 2. + 1)];
        q3 = data[(unsigned int)(3*(data_size + 1) / 4. + 1)];

        double iqr(q3-q1);

        double min_ext_cutoff = q1 - 3. * iqr;
        double min_cutoff = q1 - 1.5 * iqr;
        double max_ext_cutoff = q3 + 3. * iqr;
        double max_cutoff = q3 + 1.5 * iqr;

        std::vector<double>::const_iterator i;

        for(i = data.begin(); *i < min_cutoff; ++i)
        {
            if(*i < min_ext_cutoff)
            {
                extreme_outliers.push_back(*i);
            }

            else if(*i < min_cutoff)
            {
                outliers.push_back(*i);
            }
        }

        whisker_min = *i;

        std::vector<double>::const_reverse_iterator j;

        for(j = data.rbegin(); *j > max_cutoff; ++j)
        {
            if(*j > max_ext_cutoff)
            {
                extreme_outliers.push_back(*j);
            }

            else if(*j > max_cutoff)
            {
                outliers.push_back(*j);
            }
        }

        whisker_max = *j;
    }
}; // struct svg_boxplot_series

class svg_boxplot
{
private:
    // Stored so as to avoid rewriting style information constantly.
    svg image;

    double y_scale, y_shift;

    text_element title_info;
    text_element x_label_info;
    text_element y_label_info;

    // Border information for the plot window.
    int plot_x1;
    int plot_x2;
    int plot_y1;
    int plot_y2;


    // Yes/no questions.
    bool use_x_label;
    bool use_x_major_labels;
    bool use_y_major_labels;
    bool use_y_label;
    bool use_title;

    // where we will be storing the data points for transformation.
    std::vector<svg_boxplot_series> series;
    std::string plot_window_clip;

    // axis information
    double       y_min,  y_max;
    double       y_major, y_axis, x_axis;
    unsigned int x_major_length, y_major_length,  y_minor_length,
                 y_num_minor;

    void _clear_all()
    {
        // TODO, fill
    }

    void _transform_y(double& y)
    {
        y = y * y_scale + y_shift;
    }

    void _draw_y_minor_ticks(double j, path_element& tick_path)
    {
        double y1(j), x1(plot_x1);
        double y_tick_length = y_minor_length / 2.;
        double x2(plot_x1 - y_tick_length / 2.);

        _transform_y(y1);

        tick_path.M(x1, y1).L(x2, y1);
    }


    void _draw_x_major_ticks(double i, path_element& tick_path,
        const std::string& str)
    {
        double x1(i), y1(0.), y2(image.get_x_size());

        // Draw major tick.
        x1=i;

        y1 = plot_y2;
        y2 = plot_y2 + x_major_length;

        tick_path.M(x1, y1).L(x1, y2);

        if(use_x_major_labels)
        {
            y1 += x_major_length;

            image.get_g_element(boxplot::PLOT_LABELS).text(x1,
                y1 + 12, str);
        }
    }

    void _draw_y_major_ticks(double i, path_element& tick_path)
    {
        double y1(i), x1(0.), x2(image.get_y_size());

        // Draw major tick.
        y1=i;

        _transform_y(y1);

        double y_tick_length = y_major_length / 2.;

        x1 = plot_x1;
        x2 = plot_x1 - y_tick_length/2.;

        tick_path.M(x1, y1).L(x2, y1);

        if(use_y_major_labels)
        {
            std::stringstream fmt;
            fmt<<i;

            x1 -= y_major_length;

            image.get_g_element(boxplot::PLOT_LABELS).text(x1 + 12,
                y1, fmt.str(), 12, center_align, -90);
        }
    }

    void _draw_y_axis()
    {
        path_element& minor_tick_path =
            image.get_g_element(boxplot::Y_MINOR_TICKS).path();

        path_element& major_tick_path =
            image.get_g_element(boxplot::Y_MAJOR_TICKS).path();

        // y_minor_jump is the interval between minor ticks.
        double y_minor_jump = y_major/((double)(y_num_minor + 1.) );

        // Draw the ticks on the positive side.
        for(double i = 0; i < y_max; i += y_major)
        {
            for(double j = i + y_minor_jump;
                       j < i + y_major;
                       j += y_minor_jump)
            {
                _draw_y_minor_ticks(j, minor_tick_path);
            }

            _draw_y_major_ticks(i, major_tick_path);
        }

        // Draw the ticks on the negative side.
        for(double i = 0; i > y_min; i -= y_major)
        {
            // draw minor ticks
            for(double j=i; j>i-y_major; j-=y_major / (y_num_minor+1))
            {
                _draw_y_minor_ticks(j, minor_tick_path);
            }

            _draw_y_major_ticks(i, major_tick_path);
        }
    }

    void _draw_x_axis()
    {
        double y1(0.);

        // Draw the axis line.
        _transform_y(y1);

        x_axis = y1;

        path_element& major_tick_path =
            image.get_g_element(boxplot::X_TICKS).path();

        // Draw the ticks on the positive side.
        for(size_t i = 0; i < series.size(); ++i)
        {
            _draw_x_major_ticks(
                plot_x1 + (plot_x2-plot_x1)*((double)(i + 1)) /
                    (double)(series.size() + 1),
                major_tick_path, series[i].title);
        }
    }

    void _draw_axis()
    {
        _draw_y_axis();
        _draw_x_axis();
    }

    void _draw_title()
    {
        text_element title(image.get_x_size()/2.,
                           title_info.font_size(),
                           title_info.text());

        title.alignment(center_align);
        title.font_size(title_info.font_size());
        image.get_g_element(boxplot::TITLE).push_back(new text_element(title));
    }

    void _draw_x_label()
    {
        text_element to_use((plot_x2 + plot_x1) / 2.,
            image.get_y_size() - 8, x_label_info.text());

        to_use.font_size(12);
        to_use.alignment(center_align);

        image.get_g_element(boxplot::X_LABEL).push_back(new text_element(to_use));
    }

    void _draw_y_label()
    {
        image.get_g_element(boxplot::Y_LABEL).style().stroke_color(black);

        image.get_g_element(boxplot::Y_LABEL).push_back(new
            text_element(12, (plot_y2 + plot_y1)/2.,
            y_label_info.text(),
            12, center_align, -90));
    }


    void _calculate_transform()
    {
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
            plot_y2 -= (int)(x_label_info.font_size() * 2);
        }

        if(use_y_label)
        {
            plot_x1 += (int)(y_label_info.font_size() * 1.5);
        }

        if(use_title)
        {
            plot_y1 += (int)(title_info.font_size() * 1.5);
        }

        // Give the plot window a natural bit of padding.
        plot_x1+=5;
        plot_x2-=5;
        plot_y1+=5;
        plot_y2-=5;

        plot_x1 +=
            y_major_length > y_minor_length ?
            y_major_length :
            y_minor_length ;

        plot_y2 -= x_major_length + 10;

        image.get_g_element(boxplot::PLOT_BACKGROUND).push_back(
                new rect_element(plot_x1, plot_y1,
                        (plot_x2-plot_x1), plot_y2-plot_y1));

    }

    void _draw_whiskers(double min, double max, double length, double x,
        const svg_style& min_whisker, const svg_style& max_whisker,
        const svg_style& axis_whisker)
    {
        // Set up document structure for whiskers.
        g_element& g_whisk_ptr = image.get_g_element(boxplot::WHISKER)
            .add_g_element();

        // Set colors for whiskers.
        g_whisk_ptr.add_g_element().style()
            .stroke_color(min_whisker.stroke_color())
            .fill_color(min_whisker.fill_color())
            .stroke_width(min_whisker.stroke_width());

        g_whisk_ptr.add_g_element().style()
            .stroke_color(max_whisker.stroke_color())
            .fill_color(max_whisker.fill_color())
            .stroke_width(max_whisker.stroke_width());

        // Set axis structure and colors.
        g_element& g_axis_ptr = image.get_g_element(boxplot::BOX_AXIS)
            .add_g_element();

        g_axis_ptr.style()
            .stroke_color(axis_whisker.stroke_color())
            .fill_color(axis_whisker.fill_color())
            .stroke_width(axis_whisker.stroke_width());

        _transform_y(min);
        _transform_y(max);

        double half_length = length / 2.;

        g_whisk_ptr.get_g_element(0)
            .line(x-half_length, min, x+half_length, min);

        g_whisk_ptr.get_g_element(1)
            .line(x-half_length, max, x+half_length, max);

        g_axis_ptr.line(x, min, x, max);

        // Clip elements.
        g_axis_ptr.clip_id(plot_window_clip);
        g_whisk_ptr.clip_id(plot_window_clip);
    }

    void _draw_box(double q1, double q3, double x, double width,
        const svg_style& box_style)
    {
        g_element& g_ptr = image.get_g_element(boxplot::MEDIAN)
            .add_g_element();

        g_ptr.style().stroke_color(box_style.stroke_color())
                     .stroke_width(box_style.stroke_width())
                     .fill_color(box_style.fill_color());

        _transform_y(q1); _transform_y(q3);

        double half_width = width/2;

        g_ptr.rect(x - half_width, q3, width, q1 - q3);

        // Clip elements.
        g_ptr.clip_id(plot_window_clip);
    }

    void _draw_median(double median, double x_offset, double box_width,
        const svg_style& median_style)
    {
        _transform_y(median);

        g_element& g_ptr = image.get_g_element(boxplot::MEDIAN)
            .add_g_element();

        g_ptr.style().stroke_color(median_style.stroke_color())
                     .stroke_width(median_style.stroke_width());

        double half_width = box_width / 2.;

        g_ptr.line(x_offset - half_width, median,
                   x_offset + half_width, median);

        // Clip elements.
        g_ptr.clip_id(plot_window_clip);
    }

    void _draw_outliers(double x, const std::vector<double>& outliers,
        const std::vector<double>& extreme_outliers,
        const plot_point_style& mild_style, const plot_point_style& extreme_style)
    {
        std::vector<double>::const_iterator i;
        double temp;

        g_element& g_mild_ptr = image.get_g_element(boxplot::MILD_OUTLIERS)
            .add_g_element();
        g_element& g_ext_ptr = image.get_g_element(boxplot::EXTREME_OUTLIERS)
            .add_g_element();

        g_mild_ptr.style().fill_color(mild_style.fill_color)
                          .stroke_color(mild_style.stroke_color);

        g_ext_ptr.style().fill_color(extreme_style.fill_color)
                         .stroke_color(extreme_style.stroke_color);

        for(i = outliers.begin(); i != outliers.end(); ++i)
        {
            _transform_y(temp = *i);
            g_mild_ptr.circle(x, temp, 2);
        }

        for(i = extreme_outliers.begin(); i != extreme_outliers.end(); ++i)
        {
            _transform_y(temp = *i);
            g_ext_ptr.circle(x, temp, 2);
        }

        // Clip elements.
        g_mild_ptr.clip_id(plot_window_clip);
        g_ext_ptr.clip_id(plot_window_clip);
    }

    void _draw_boxplot(const svg_boxplot_series& series, double x_offset)
    {
        _draw_whiskers(series.whisker_min, series.whisker_max,
                       series.whisker_length, x_offset,
                       series.min_whisker_style, series.max_whisker_style,
                       series.axis_style);

        _draw_box(series.q1, series.q3, x_offset, series.box_width,
            series.box_style);

        _draw_median(series.median, x_offset,
            series.box_width - series.box_style.stroke_width(),
            series.median_style);

        _draw_outliers(x_offset, series.outliers, series.extreme_outliers,
            series.mild_outlier, series.ext_outlier);
    }

    void _update_image()
    {
        _clear_all();

        // Draw background.
        image.get_g_element(boxplot::BACKGROUND).push_back(
                     new rect_element(0, 0, image.get_x_size(),
                     image.get_y_size()));

        _draw_title();
        _calculate_plot_window();
        _calculate_transform();

        // Define the clip path for the plot window.
        // We don't want to allow overlap of the plot window lines,
        // thus the minor adjustments.

        image.clip_path(rect_element(plot_x1 + 1, plot_y1 + 1,
                                     plot_x2 - plot_x1 - 2, plot_y2 - plot_y1 - 2),
                                     plot_window_clip);

        _draw_y_axis();
        _draw_x_axis();

        if(use_x_label)
        {
            _draw_x_label();
        }

        if(use_y_label)
        {
            _draw_y_label();
        }

        for(unsigned int i=0; i<series.size(); ++i)
        {
            _draw_boxplot(series[i],
                plot_x1 + (plot_x2-plot_x1)*((double)(i + 1)) / (double)(series.size() + 1));
        }
    }

public:

svg_boxplot():        title_info(0, 0, "Plot of data", 30),
                      x_label_info(0, 0, "X Axis", 12),
                      y_label_info(0, 0, "Y Axis", 12),
                      y_min(0), y_max(100),
                      y_major(10), use_y_label(true),
                      use_x_label(true), x_major_length(10),
                      y_minor_length(5), y_num_minor(1),
                      y_major_length(20),
                      plot_window_clip("__clip_window"),
                      use_x_major_labels(true),
                      use_title(true), use_y_major_labels(true)

{
    image.image_size(500, 350);

    // Build the document tree.. add children of the root node.
    for(int i=0; i<boxplot::BOXPLOT_DOC_CHILDREN; ++i)
    {
        image.add_g_element();
    }

    // Set boxplot color defaults.
    image.get_g_element(boxplot::BACKGROUND)
        .style().fill_color(white);

    image.get_g_element(boxplot::PLOT_BACKGROUND)
        .style().fill_color(white).stroke_color(black);

    image.get_g_element(boxplot::X_TICKS)
        .style().stroke_color(black).stroke_width(2);

    image.get_g_element(boxplot::Y_MINOR_TICKS)
        .style().stroke_color(black).stroke_width(1);

    image.get_g_element(boxplot::Y_MAJOR_TICKS)
        .style().stroke_color(black).stroke_width(2);

    image.get_g_element(boxplot::X_LABEL).style().stroke_color(black);
    image.get_g_element(boxplot::Y_LABEL).style().stroke_color(black);
    image.get_g_element(boxplot::TITLE).style().stroke_color(black);
    image.get_g_element(boxplot::WHISKER).style().stroke_color(black);
    image.get_g_element(boxplot::BOX_AXIS).style().stroke_color(black);
    image.get_g_element(boxplot::BOX).style().stroke_color(black).fill_color(white);
    image.get_g_element(boxplot::MEDIAN).style().stroke_color(black).stroke_width(2);
    image.get_g_element(boxplot::EXTREME_OUTLIERS).style().stroke_color(black).fill_color(white);
    image.get_g_element(boxplot::MILD_OUTLIERS).style().stroke_color(black).fill_color(black);
}

svg_boxplot& load_stylesheet(const std::string& file)
{
    image.load_stylesheet(file);
    return *this;
}

svg_boxplot& write(const std::string& _str)
{
    std::ofstream fout(_str.c_str());

    if(fout.fail())
    {
        throw std::runtime_error("Unable to open "+_str);
    }

    write(fout);

    return *this;
}

svg_boxplot& write(std::ostream& s_out)
{
    _update_image();

    image.write(s_out);

    return *this;
}

svg_boxplot& title_on(bool _cmd)
{
    use_title = _cmd;
    return *this;
}

svg_boxplot& y_label_on(bool _cmd)
{
    use_y_label = _cmd;
    return *this;
}

svg_boxplot& x_label_on(bool _cmd)
{
    use_x_label = _cmd;
    return *this;
}

svg_boxplot& y_major_labels_on(bool _cmd)
{
    use_y_major_labels = _cmd;
    return *this;
}

svg_boxplot& x_labels_on(bool _cmd)
{
    use_x_major_labels = _cmd;
    return *this;
}

svg_boxplot& y_major_tick_color(const svg_color& _col)
{
    image.get_g_element(boxplot::Y_MAJOR_TICKS).style().stroke_color(_col);
    return *this;
}

svg_boxplot& x_tick_color(const svg_color& _col)
{
    image.get_g_element(boxplot::X_TICKS).style().stroke_color(_col);
    return *this;
}

svg_boxplot& y_minor_tick_color(const svg_color& _col)
{
    image.get_g_element(detail::PLOT_Y_MINOR_TICKS).style().stroke_color(_col);
    return *this;
}

svg_boxplot& title_color(const svg_color& _col)
{
    image.get_g_element(boxplot::TITLE).style().stroke_color(_col);
    return *this;
}

svg_boxplot& background_color(const svg_color& _col)
{
    image.get_g_element(boxplot::BACKGROUND).style().fill_color(_col);
    return *this;
}

svg_boxplot& background_border_color(const svg_color& _col)
{
    image.get_g_element(boxplot::BACKGROUND).style().stroke_color(_col);
    return *this;
}

svg_boxplot& plot_background_color(const svg_color& _col)
{
    image.get_g_element(boxplot::PLOT_BACKGROUND).style().fill_color(_col);
    return *this;
}

svg_boxplot& plot_background_border_color(const svg_color& _col)
{
    image.get_g_element(boxplot::PLOT_BACKGROUND).style().stroke_color(_col);
    return *this;
}

svg_boxplot& y_range(double y1, double y2)
{
    y_min = y1;
    y_max = y2;

    if(y2 <= y1)
    {
        throw std::runtime_error("Illegal Argument: X scale: x2 < x1");
    }

    return *this;
}

svg_boxplot& y_label(const std::string& _str)
{
    y_label_info.text(_str);
    return *this;
}

svg_boxplot& image_size(unsigned int x, unsigned int y)
{
    image.image_size(x, y);
    return *this;
}

svg_boxplot& y_label_size(unsigned int _size)
{
    y_label_info.font_size(_size);
    return *this;
}

svg_boxplot& y_label_color(const svg_color& _col)
{
    image.get_g_element(boxplot::Y_LABEL).style().stroke_color(_col);
    return *this;
}

svg_boxplot& title(const std::string& _str)
{
    y_label_info.text(_str);
    return *this;
}

svg_boxplot& title_size(unsigned int _size)
{
    y_label_info.font_size(_size);
    return *this;
}

svg_boxplot& x_label(const std::string& _str)
{
    x_label_info.text(_str);
    return *this;
}

svg_boxplot& x_label_size(unsigned int _size)
{
    x_label_info.font_size(_size);
    return *this;
}

svg_boxplot& x_label_color(const svg_color& _col)
{
    image.get_g_element(boxplot::X_LABEL).style().stroke_color(_col);
    return *this;
}

svg_boxplot& y_major_interval(double _inter)
{
    y_major = _inter;

    return *this;
}

svg_boxplot& x_tick_length(unsigned int _length)
{
    x_major_length = _length;
    return *this;
}

svg_boxplot& y_major_tick_length(unsigned int _length)
{
    y_major_length = _length;
    return *this;
}

svg_boxplot& y_minor_tick_length(unsigned int _length)
{
    y_minor_length = _length;
    return *this;
}

svg_boxplot& y_num_minor_ticks(unsigned int _num)
{
    y_num_minor = _num;
    return *this;
}

svg_boxplot& x_tick_width(unsigned int _width)
{
    image.get_g_element(boxplot::X_TICKS).style().stroke_width(_width);

    return *this;
}

svg_boxplot& y_major_tick_width(unsigned int _width)
{
    image.get_g_element(boxplot::Y_MAJOR_TICKS).style().stroke_width(_width);

    return *this;
}

svg_boxplot& y_minor_tick_width(unsigned int _width)
{
    image.get_g_element(boxplot::Y_MINOR_TICKS).style().stroke_width(_width);

    return *this;
}


unsigned int get_image_x_size()
{
    return image.get_x_size();
}

unsigned int get_image_y_size()
{
    return image.get_x_size();
}

std::string get_title()
{
    return title_info.text();
}

bool get_x_label()
{
    return use_x_label;
}

bool get_x_major_labels()
{
    return use_x_major_labels;
}

// color information
svg_color get_title_color()
{
    return image.get_g_element(boxplot::TITLE).style().stroke_color();
}

svg_color get_background_color()
{
    return image.get_g_element(boxplot::BACKGROUND).style().fill_color();
}

svg_color get_background_border_color()
{
    return image.get_g_element(boxplot::BACKGROUND).style().stroke_color();
}

svg_color get_plot_background_color()
{
    return image.get_g_element(boxplot::PLOT_BACKGROUND).style().fill_color();
}

svg_color get_x_label_color()
{
    return image.get_g_element(boxplot::X_LABEL).style().stroke_color();
}

svg_color get_x_tick_color()
{
    return image.get_g_element(boxplot::X_TICKS).style().stroke_color();
}

unsigned int get_x_tick_length()
{
    return x_major_length;
}

unsigned int get_x_major_tick_width()
{
    return image.get_g_element(boxplot::X_TICKS).style().stroke_width();
}

std::string get_x_label_text()
{
    return x_label_info.text();
}

svg& get_svg()
{
    _update_image();

    return image;
}

#if defined (BOOST_MSVC)
#  pragma warning(push)
#  pragma warning(disable: 4100) // "'boost_parameter_enabler_argument' : unreferenced formal parameter"
#endif

BOOST_PARAMETER_MEMBER_FUNCTION
(
    (void),
    plot,
    keyword,
    (required
        (container, *)
        (title, (const std::string&))
    )
    (optional
        (box, (const svg_style&), svg_style(white, black, 1))
        (median, (const svg_style&), svg_style(white, black, 1))
        (axis, (const svg_style&), svg_style(white, black, 1))
        (min_whisker, (const svg_style&), svg_style(white, black, 1))
        (max_whisker, (const svg_style&), svg_style(white, black, 1))
        (mild_outlier, (const plot_point_style&), plot_point_style(black, black, 0))
        (ext_outlier, (const plot_point_style&), plot_point_style(white, black, 1))
        (whisker, (unsigned int), 30)
        (width, (unsigned int), 60)
        (fnctr, *, detail::boost_default_convert())
    )
)
{
    series.push_back(
        svg_boxplot_series(
        boost::make_transform_iterator(container.begin(), fnctr),
        boost::make_transform_iterator(container.end(),   fnctr), title,
        whisker, width, box, median, axis, min_whisker, max_whisker, mild_outlier,
        ext_outlier
        ));
}
}; // class svg_boxplot

#if defined (BOOST_MSVC)
#  pragma warning(pop)
#endif

} // namespace svg
} // namespace boost
#endif // BOOST_SVG_SVG_BOXPLOT_HPP


