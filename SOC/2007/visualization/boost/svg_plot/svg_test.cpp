#include "svg_1d_plot.hpp"
#include "svg_2d_plot.hpp"

#include <vector>
#include <cmath>
#include <map>

using std::multimap;
using std::vector;

double f(double x)
{
    return sqrt(x);
}

double g(double x)
{
    return -2 + x*x;
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
    using namespace boost::svg;
    std::map<double, double> data1;
    std::map<double, double> data2;
    std::vector<double> data3;

    svg_2d_plot my_2d_plot;
    svg_1d_plot my_1d_plot;

    for(double i=0; i<10; i+=1)
    {
        data1[i - 5.] = f(i);
        data2[i] = g(i);
        data3.push_back(h(i) - 10.);
    }

    // size/scale settings
    my_2d_plot.image_size(500, 350);

    my_2d_plot.title("Hello, operator")
           .plot_window_on(true)
           .legend_on(true);

    my_2d_plot.title_on(true)
           .x_label_on(true)
           .y_major_labels_on(true)
           .x_label("sqrt(x)")
           .x_major_grid_on(false);
//           .y_external_style_on(true)
//           .x_external_style_on(true);

    my_1d_plot.image_size(500, 350);

    my_1d_plot.title("Hello, operator")
           .plot_window_on(true)
           .legend_on(true);

    my_1d_plot.title_on(true)
           .x_label_on(true)
           .x_label("sqrt(x)");

    my_2d_plot.plot_2d(data1, "sqrt(x)");

    my_2d_plot.plot_2d(data2, "Not sqrt(x)", 
        _size = 6,
        _point_style = square,
        _stroke_color = hotpink,
        _line_color = black,
        _fill_color = yellow,
        _area_fill_color = red);
    
    my_1d_plot.plot(data3, "1D Plot");

    my_1d_plot.write("./test1d.svg");
    my_2d_plot.write("./test2d.svg");

    return 0;
}
