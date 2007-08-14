#include "svg_boxplot.hpp"

#include <vector>
#include <cmath>

using std::multimap;
using std::vector;

double h(double x)
{
    return 50 / (x);
}

double f(double x)
{
    return 40 + 25 * sin(x * 50);
}

int main()
{
    using namespace boost::svg;
    std::vector<double> data1, data2;

    svg_boxplot my_plot;

    my_plot.background_border_color(black)
           .y_range(0, 150)
           .y_major_interval(20)
           .title("Boxplots of Common Functions");
    
    for(double i=.1; i < 10; i+=.1)
    {
        data1.push_back(h(i));
        data2.push_back(f(i));
    }

    my_plot.plot(data1, "[50 / x]");
    my_plot.plot(data2, "[40 + 25 * sin(50x)]");

    my_plot.write("D:\\boxplot_test.svg");

    return 0;
}
