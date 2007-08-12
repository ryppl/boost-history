#include "svg_boxplot.hpp"

#include <vector>
#include <cmath>

using std::multimap;
using std::vector;

double h(double x)
{
    return 50 / (x);
}

int main()
{
    using namespace boost::svg;
    std::vector<double> data;

    svg_boxplot my_plot;

    my_plot.background_border_color(black)
           .y_range(0, 150)
           .y_major_interval(20);
    
    for(double i=.1; i < 10; i+=.1)
    {
        data.push_back(h(i));
    }

    my_plot.plot(data, "50 / (x)");

    my_plot.write("D:\\test2d.svg");

    return 0;
}
