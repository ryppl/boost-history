#include <map>
#include <cmath>
#include <boost/array.hpp>

#include "svg_1d_plot.hpp"
#include "svg_2d_plot.hpp"

using std::multimap;
using std::deque;
using boost::array;
using namespace boost::svg;

double f(double x)
{
    return sqrt(x);
}

double g(double x)
{
    return -2 + x;
}

double h(double x)
{
    return 2*x;
}

int main()
{

    double start = 0, finish = 10;

    multimap<double, double> data;

    svg_2d_plot my_plot;

    for(double i=0; i<10; ++i)
    {
        data.insert(std::pair<double,double>(i,f(i)));
    }

    // size/scale settings
    my_plot.set_image_size(500, 350)
           .set_x_scale(-1, 10)
           .set_y_scale(-1, 5);

    // Text settings
    my_plot.set_title("2D Graph Test")
           .set_title_font_size(29)
           .set_x_label_text("Time in Months");

    // command settings
    my_plot.set_axis(true)
           .set_legend(true)
           .set_plot_window(true)
           .set_x_label(true)
           .set_x_major_labels(true)
           .set_y_major_labels(true);

    // color settings
    my_plot.set_background_color(svg_color(67, 111, 69))
           .set_legend_background_color(svg_color(207, 202,167))
           .set_plot_background_color(svg_color(136, 188, 126))
           .set_title_color(white)

           .set_x_axis_color(black)
           .set_y_axis_color(black)

           .set_x_major_tick_color(black)
           .set_y_major_tick_color(black)

           .set_legend_border_color(svg_color(102, 102, 84))

           .set_x_minor_tick_color(black)
           .set_y_minor_tick_color(black);

    //axis settings
    my_plot.set_x_major_tick(2)
           .set_x_num_minor_ticks(2)
           .set_x_major_tick_length(14)
           .set_x_minor_tick_length(7)
           .set_x_major_tick_width(2)
           .set_x_minor_tick_width(1)

           .set_y_major_tick(3)
           .set_y_num_minor_ticks(2)
           .set_y_major_tick_length(20)
           .set_y_minor_tick_length(10)
           .set_y_major_tick_width(2)
           .set_y_minor_tick_width(1);

    //legend settings
    my_plot.set_legend_title_font_size(15);

    plot_range(my_plot, data.begin(), data.end(), "sqrt(x)",  blue);

    my_plot.write("D:/test.svg");

    return 0;
}

