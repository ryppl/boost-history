#include "svg_1d_plot.hpp"
#include "svg_2d_plot.hpp"

#include <vector>
#include <cmath>
#include <map>
#include <iostream>
#include <limits>

using std::multimap;
using std::vector;

double f(double x)
{
    return sin(x);
}

double g(double x)
{
    return 2*cos(x);
}

double h(double x)
{
    return tan(x);
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

using namespace std;

int main()
{
    using namespace boost::svg;
    std::map<double, double> data1;
    std::map<double, double> data2;
    std::vector<double> data3;

    svg_2d_plot my_2d_plot;
    svg_1d_plot my_1d_plot;

    double pi = 3.1415926535;
  
    for(double i=0; i<10; i+=pi/8.)
    {
        data2[i] = g(i);
    }

    cout<<"Done with first part"<<endl;
    // size/scale settings
    my_2d_plot.image_size(500, 350);

    my_2d_plot.title("Hello, operator")
           .plot_window_on(true);

    my_2d_plot.title_on(true)
           .x_label_on(true)
           .y_major_labels_on(true)
           .x_label("sqrt(x)")
           .x_major_grid_on(false)
           .y_range(-1.1, 1.1)
           .x_range(-.5, 10.5)
           .y_major_interval(1)
           .x_major_interval(pi/2.);
//           .y_external_style_on(true)
//           .x_external_style_on(true);

    my_1d_plot.image_size(500, 350);

    my_1d_plot.title("Hello, operator")
           .plot_window_on(true)
           .legend_on(true);

    my_1d_plot.title_on(true)
           .x_label_on(true)
           .x_label("sqrt(x)");

    //my_2d_plot.plot(data1, "sqrt(x)", _bezier_on = true, _size = 5);

    my_2d_plot.plot(data2, "Not sqrt(x)", 
        _area_fill_color = orange,
        _size = 6,
        _point_style = square,
        _stroke_color = hotpink,
        _line_color = black,
        _fill_color = yellow);
    
//    my_2d_plot.plot(data3, "1D Plot", _bezier_on = true, _size = 5);

    my_1d_plot.write("./test1d.svg");
    my_2d_plot.write("./test2d.svg");

    return 0;
}
