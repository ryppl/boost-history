// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// SOCI example

// b: using WKT to retrieve points

// To build and run this example, see comments in example a

#include <soci.h>
#include <soci-postgresql.h>

#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>
#include <boost/timer.hpp>
#include <boost/random.hpp>
#include <boost/tuple/tuple.hpp>

#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <exception>

#include <ggl/ggl.hpp>
#include <ggl/geometries/geometries.hpp>

#include <ggl/extensions/gis/io/wkt/wkt.hpp>


struct city
{
    ggl::point<float, 2, ggl::cs::geographic<ggl::degree> > location;
    std::string name;
};

namespace soci
{
    template <>
    struct type_conversion<city>
    {
        typedef soci::values base_type;

        static void from_base(const base_type& v, soci::indicator ind, city& value)
        {
            try
            {
                value.name = v.get<std::string>("name");
                ggl::read_wkt(v.get<std::string>("wkt"), value.location);
            }
            catch(const std::exception& e)
            {
                std::cout << e.what() << std::endl;
            }
        }

        static void to_base(const city& value, base_type& v, soci::indicator& ind)
        {
            v.set("name", value.name);
            std::ostringstream out;
            out << ggl::wkt(value.location);
            v.set("wkt", out.str());
            ind = i_ok;
        }
    };
}

int main()
{
    try
    {
        soci::session sql(soci::postgresql, "dbname=ggl user=ggl password=ggl");


        typedef std::vector<city> V;

        soci::rowset<city> rows = sql.prepare << "select name,astext(location) as wkt from cities";
        V vec;
        std::copy(rows.begin(), rows.end(), std::back_inserter(vec));

        for (V::const_iterator it = vec.begin(); it != vec.end(); ++it)
        {
            static const double sqrkm = 1000.0 * 1000.0;
            std::cout << it->name
                << "    " << ggl::dsv(it->location)
                //<< "    " << ggl::area(it->shape) / sqrkm << " km2" 
                <<  std::endl;
        }
    }
    catch (std::exception const &e)
    {
        std::cerr << "Error: " << e.what() << '\n';
    }
    return 0;
}
