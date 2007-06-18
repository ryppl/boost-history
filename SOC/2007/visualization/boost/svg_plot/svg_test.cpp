#include <vector>
#include <deque>
#include <cmath>

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

int main()
{
  double start = 0, finish = 10;

    vector<double> data1;
    deque<double> data2;


    double inter = 3.1415926535 / 4.;
    
    for(double i = start; i <= finish; i += inter)
        data1.push_back(f(i));

    for(double i=start; i <= finish; i += inter)
        data2.push_back(g(i));

    svg_plot my_plot("D:\\1D_legend_demo.svg");

    my_plot.image_size(500, 350).x_scale(-2, 10);
    
    my_plot.draw_axis().show_legend();

    my_plot.set_background_color(lightgray)
           .set_legend_background_color(whitesmoke);

    
    plot_range(my_plot, data2.begin(), data2.end(), "Lions", blue);
    plot_range(my_plot, data1.begin(), data1.end(), "Tigers", limegreen);
    
    my_plot.write();

    return 0;
}

