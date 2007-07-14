#include <vector>
#include <cmath>
#include <map>

#include "svg_2d_plot.hpp"
#include "svg_1d_plot.hpp"

using std::multimap;
using std::vector;
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
    std::map<double, double> data1;
    std::map<double, double> data2;
    std::vector<double> data3;

    svg_2d_plot my_2d_plot;
    svg_1d_plot my_1d_plot;

    for(double i=0; i<10; i+=1)
    {
        data1[i - 5.] = f(i);
        data2[i] = i - 5.;
        data3.push_back(h(i) - 10.);
    }

    // size/scale settings
    my_2d_plot.set_image_size(500, 350);

    my_2d_plot.set_title("Hello, operator")
           .set_plot_window_on(true)
           .set_legend_on(true);

    my_2d_plot.set_title_on(true)
           .set_x_label_on(true)
           .set_x_label("sqrt(x)")
           .set_x_major_grid_on(false);

    my_1d_plot.set_image_size(500, 350);

    my_1d_plot.set_title("Hello, operator")
           .set_plot_window_on(true)
           .set_legend_on(true);

    my_1d_plot.set_title_on(true)
           .set_x_label_on(true)
           .set_x_label("sqrt(x)")
           .set_x_major_grid_on(false);

    plot_2d(my_2d_plot, data1, "sqrt(x)");

    plot_2d(my_2d_plot, data2, "Not sqrt(x)", 
        _size = 10);
    
    plot(my_1d_plot, data3, "1D Plot");

    my_1d_plot.write("D:/test1d.svg");
    my_2d_plot.write("D:/test2d.svg");

    return 0;
}
