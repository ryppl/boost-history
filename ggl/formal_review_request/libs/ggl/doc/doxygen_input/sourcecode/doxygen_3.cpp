// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Doxygen Examples, referred to from the sources

#include <ggl/ggl.hpp>
#include <ggl/geometries/adapted/tuple_cartesian.hpp>
#include <ggl/geometries/adapted/c_array_cartesian.hpp>
#include <ggl/geometries/adapted/std_as_linestring.hpp>



void example_distance()
{
    int a[2] = {1,2};
    int b[2] = {3,4};
    double d = ggl::distance(a, b);
    std::cout << d << std::endl;
}

void example_length1()
{
    std::vector<boost::tuple<double, double, double> > line;
    line.push_back(boost::make_tuple(1, 2, 3));
    line.push_back(boost::make_tuple(4, 5, 6));
    line.push_back(boost::make_tuple(7, 8, 9));
    double length = ggl::length(line);
    std::cout << length << std::endl;
}

void example_length2()
{
    std::vector<boost::tuple<double, double> > line;
    line.push_back(boost::make_tuple(1.1, 2.2));
    line.push_back(boost::make_tuple(3.3, 4.4));
    line.push_back(boost::make_tuple(5.5, 6.6));
    std::cout << ggl::length(
                std::make_pair(boost::begin(line), boost::end(line) + -1)
            )  
        << std::endl;
}




int main(void)
{
    example_distance();
    example_length1();
    example_length2();
    return 0;
}
