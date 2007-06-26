#include <vector>
#include <deque>
#include <cmath>
#include <boost/array.hpp>

#include "svg_plot.hpp"

using namespace boost::svg;
using namespace std;

double f(double x)
{
    return (x*x)/2.;
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

    vector<double> data1;
    deque<double> data2;
    boost::array<double, 10> data3;

    double inter = 3.1415926535 / 1.;
    
    int j=0;
    for(double i = start; i <= finish; i += inter, ++j)
    {
        data1.push_back(f(i));
        data2.push_back(g(i));
        data3[j] = h(i);
    }

    svg_plot my_plot;

    // size/scale settings
    my_plot.set_image_size(500, 350)
           .set_x_scale(-3, 10);

    // Text settings
    my_plot.set_title("Oh My!")
           .set_title_font_size(29)
           .set_x_label_text("Time in Months");

    // command settings
    my_plot.set_axis(true)
           .set_legend(true)
           .set_plot_window(true)
           .set_x_label(true)
           .set_x_major_labels(true);

    // color settings
    my_plot.set_background_color(svg_color(67, 111, 69))
           .set_legend_background_color(svg_color(207, 202,167))
           .set_plot_background_color(svg_color(136, 188, 126))
           .set_title_color(white)
           .set_x_axis_color(black)
           .set_x_major_tick_color(black)
           .set_x_minor_tick_color(black);

    //axis settings
    my_plot.set_x_major_tick(2)
           .set_x_num_minor_ticks(3)
           .set_x_major_tick_length(14)
           .set_x_minor_tick_length(7)
           .set_x_major_tick_width(1)
           .set_x_minor_tick_width(1);

    //legend settings
    my_plot.set_legend_title_font_size(15);

    plot_range(my_plot, data2.begin(), data2.end(), "Lions",  blue);
    plot_range(my_plot, data1.begin(), data1.end(), "Tigers", purple);
    plot_range(my_plot, data3.begin(), data3.end(), "Bears",  red);

    my_plot.write("./test.svg");

    return 0;
}

