// Generic Geometry Library - Star Comb Overlay Comparisons
//
// Copyright Barend Gehrels, 2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef COMPARE_STARCOMB_HPP_INCLUDED
#define COMPARE_STARCOMB_HPP_INCLUDED

#include <iostream>
#include <string>
#include <vector>

#include <boost/timer.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/math/constants/constants.hpp>


template <typename Polygon, typename AddFunctor>
inline void make_star(Polygon& polygon, AddFunctor functor,
            int count, double factor1, double factor2,
            double offset = 0.0,
            bool close = true,
            double orientation = 1.0)
{
    // Create star
    double cx = 25.0;
    double cy = 25.0;

    double dx = 50.0;
    double dy = 50.0;

    double a1 = factor1 * 0.5 * dx;
    double b1 = factor1 * 0.5 * dy;
    double a2 = factor2 * 0.5 * dx;
    double b2 = factor2 * 0.5 * dy;

    double delta = orientation * boost::math::constants::pi<double>() * 2.0 / (count - 1);
    double angle = offset * delta;
    double x0, y0;
    bool first = true;
    for (int i = 0; i < count - 1; i++, angle += delta)
    {
        bool even = i % 2 == 0;
        double x = cx + (even ? a1 : a2) * sin(angle);
        double y = cy + (even ? b1 : b2) * cos(angle);
        functor(polygon, x, y, i);
        if (first)
        {
            x0 = x;
            y0 = y;
            first = false;
        }

    }
    if (close)
    {
        functor(polygon, x0, y0, count);
    }
}

template <typename Vector>
void ccw_pushback(Vector& vector, double x, double y, int)
{
    vector.push_back(boost::make_tuple<double, double>(x, y));
}

template <typename Polygon, typename AddFunctor>
inline void make_comb(Polygon& polygon, AddFunctor functor,
            int count, 
            bool close = true,
            bool clockwise = true)
{
    int n = 0;

    if (! clockwise)
    {
        typedef boost::tuple<double, double>  tup;
        typedef std::vector<tup> vec;
        vec v;

        // Create in clockwise order
        make_comb(v, ccw_pushback<vec>, count, close, true);

        // Add in reverse
        for (std::vector<boost::tuple<double, double> >::const_reverse_iterator 
            it = v.rbegin(); it != v.rend(); ++it)
        {
            functor(polygon, it->get<0>(), it->get<1>(), n++);
        }
        return;
    }

    // Create comb
    functor(polygon, 25.0, 0.0, n++);
    functor(polygon, 0.0, 25.0, n++);
    functor(polygon, 25.0, 50.0, n++);
    
    // Function parameters
    double diff = (25.0 / (count - 0.5)) / 2.0;

    double b1 = -25.0;
    double b2 = 25.0 - diff * 2.0;

    double x1 = 50.0, x2 = 25.0;

    for (int i = 0; i < count - 1; i++)
    {
        functor(polygon, x1, (x1 + b1), n++); x1 -= diff;
        functor(polygon, x1, (x1 + b1), n++); x1 -= diff;
        functor(polygon, x2, (x2 + b2), n++); x2 -= diff;
        functor(polygon, x2, (x2 + b2), n++); x2 -= diff;
    }
    functor(polygon, x1, (x1 + b1), n++);

    if (close)
    {
        functor(polygon, 25.0, 0.0, 4);
    }
}


inline int parse(int argc, char** argv, int& testcount, 
            int& starcount, int& combcount, 
            double& factor1, double& factor2,
            bool& do_union)
{
    testcount = argc > 1 ? atol(argv[1]) : 1;
    starcount = argc > 2 ? atol(argv[2]) : 101;
    do_union = argc > 3 && (atol(argv[3]) == 1);

    combcount = starcount - 1;
    factor1 = 1.1;
    factor2 = 0.2;


    // Currently cannot be specified
    //combcount  = argc > 3 ? atol(argv[3]) : starcount - 1;
    //factor1 = argc > 4 ? atof(argv[4]) : 1.1;
    //factor2 = argc > 5 ? atof(argv[5]) : 0.2;
    return 3;
}


inline void report(std::string const& name, double area, boost::timer& t)
{
    std::cout
        << name << ":"
        << " area: " << area
        << " speed: " << t.elapsed()
        << std::endl;
}

#endif // COMPARE_STARCOMB_HPP_INCLUDED

