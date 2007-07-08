#include <vector>
#include <cmath>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <map>

#include "svg_1d_plot.hpp"

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


class dConvert
{
public:
    typedef double result_type;

    double operator()(int a) const
    {
        return (double)a;
    }
};

int main()
{
    std::vector<double> data1;
    std::vector<int> data2;

    svg_1d_plot my_plot;

    for(double i=0; i<10; ++i)
    {
        data1.push_back(f(i));
        data2.push_back((int)i-5);
    }

    // size/scale settings
    my_plot.set_image_size(500, 350);

    my_plot.set_title("Hello, operator")
           .set_plot_window_on(false)
           .set_legend_on(true);

    my_plot.set_title_on(true)
           .set_x_label_on(true)
           .set_x_label("sqrt(x)")
           .set_background_border_color(yellow)
           .set_x_major_grid_on(false);

    my_plot.set_x_label_color(orange);

    plot(my_plot, data1, "sqrt(x)", plot_point_style(circle, black, blue, 12));
    plot(my_plot, data2, "Not sqrt(x)", dConvert());

    my_plot.write("./test.svg");

    return 0;
}
