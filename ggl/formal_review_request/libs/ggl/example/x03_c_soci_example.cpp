// Generic Geometry Library
//
// Copyright Mateusz Loskot 2009, mateusz @loskot.net
//
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// Generic Geometry Library
// SOCI example

// c: using WKB to retrieve geometries

// SOCI is a generic C++ template interface to access relational databases

// To build and run this example, see comments in example a
// Alternatively compile composing and executing compiler command directoy in examples directory,
//    for example using GCC compiler:
//    g++ -I../../../boost -I/home/mloskot/usr/include/soci \
//        -I /home/mloskot/usr/include/soci/postgresql -I/usr/include/postgresql \
//        -L/home/mloskot/usr/lib -lsoci_core-gcc-3_0 -lsoci_postgresql-gcc-3_0 x03_c_soci_example.cpp

#include <soci.h>
#include <soci-postgresql.h>

#include <iostream>
#include <exception>

#include <ggl/ggl.hpp>
#include <ggl/geometries/geometries.hpp>
#include <ggl/extensions/gis/io/wkb/read_wkb.hpp>
#include <ggl/extensions/gis/io/wkb/utility.hpp>



struct road
{
};

namespace soci
{
    template <>
    struct type_conversion<road>
    {
        typedef soci::values base_type;

        static void from_base(const base_type& v, soci::indicator ind, road& value)
        {
            try
            {
            }
            catch(const std::exception& e)
            {
                std::cout << e.what() << std::endl;
            }
        }

        static void to_base(const road& value, base_type& v, soci::indicator& ind)
        {
        }
    };
}

int main()
{
    try
    {
        soci::session sql(soci::postgresql, "dbname=ggl user=ggl password=ggl");


        // insert sample data using plain WKT input

        // query data in WKB form and read to geometry object

        // output geometry object to WKT

        // print input and result WKT texts

    }
    catch (std::exception const &e)
    {
        std::cerr << "Error: " << e.what() << '\n';
    }
    return 0;
}

