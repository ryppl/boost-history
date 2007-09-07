// (C) Copyright The Trustees of Indiana University 2005
// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  Authors: Ronald Garcia
//           Andrew Sutton

#include <iostream>
#include <string>
#include <map>

#include <boost/config.hpp>
#include <boost/test/minimal.hpp>
#include <boost/property_map/property_traits.hpp>
#include <boost/property_map/associative_property_map.hpp>
#include <boost/property_map/dynamic_property_map.hpp>

//
// dynamic_properties_test.cpp - test cases for the dynamic property maps.
//


using namespace std;
using namespace boost;
using namespace property_map;

// generate a dynamic_property_map that maps strings to strings
// WARNING: This code leaks memory.  For testing purposes only!
// WARNING: This code uses library internals. For testing purposes only!
auto_ptr<dynamic_property_map>
string2string_gen(const string& name, const any&, const any&)
{
    typedef map<string, string> Map;
    typedef associative_property_map< map<string, string> > PropertyMap;
    typedef property_map::detail::dynamic_property_map_adaptor<PropertyMap> PropertyAdaptor;

    Map* mymap = new Map(); // hint: leaky memory here!

    // build the property map and adpat it for dynamic properties...
    PropertyMap property_map(*mymap);
    auto_ptr<dynamic_property_map> pm(new PropertyAdaptor(property_map));

    return pm;
}

void
test_put_get(dynamic_properties& properties)
{
    // This is a series of put and get tests for a given set of dynamic
    // properties.

    // Get tests
    {
        BOOST_CHECK(get("int", properties,string("one")) == "1");
#ifndef BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS
        BOOST_CHECK(get<int>("int", properties,string("one")) == 1);
#endif
        BOOST_CHECK(get("int", properties, string("one"), type<int>()) == 1);
        BOOST_CHECK(get("double", properties, 5.3) == "five point three");
    }

    // Put tests
    {
        put("int", properties, string("five"), 6);
        BOOST_CHECK(get("int", properties, string("five")) == "6");
        put("int", properties, string("five"), string("5"));
#ifndef BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS
        BOOST_CHECK(get<int>("int", properties, string("five")) == 5);
#endif
        BOOST_CHECK(get("int", properties, string("five"), type<int>()) == 5);
        put("double", properties, 3.14, string("3.14159"));
        BOOST_CHECK(get("double", properties, 3.14) == "3.14159");
        put("double", properties, 3.14, string("pi"));
#ifndef BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS
        BOOST_CHECK(get<string>("double", properties, 3.14) == "pi");
#endif
        BOOST_CHECK(get("double", properties, 3.14, type<string>()) == "pi");
    }
}

int test_main(int,char**)
{
    typedef map<string, int> StringIntMap;
    typedef map<double, string> DoubleStringMap;

    // build property maps using associative_property_map
    StringIntMap string2int;
    DoubleStringMap double2string;
    associative_property_map< StringIntMap > imap(string2int);
    associative_property_map< DoubleStringMap > dmap(double2string);

    // add key-value information
    string2int["one"] = 1;
    string2int["five"] = 5;
    double2string[5.3] = "five point three";
    double2string[3.14] = "pi";


    // build and populate dynamic interface
    dynamic_properties properties;
    properties.property("int", imap);
    properties.property("double", dmap);

    test_put_get(properties);

    // Nonexistent property
    {
        try {
            get("nope", properties, 3.14);
            BOOST_ERROR("No exception thrown.");
        }
        catch (dynamic_get_failure&)
        { }

        try {
            put("nada", properties, 3.14, string("3.14159"));
            BOOST_ERROR("No exception thrown.");
        }
        catch (property_not_found&)
        { }
    }

    // Nonexistent property gets generated
    // This ensures that a put() to a named property that hasn't been
    // previously added to the dynamic properties actually gets added when
    // the value is put. How does this get added? The string2string_gen
    // function is called to add the map.
    {
        dynamic_properties props(&string2string_gen);
        put("nada", props, string("3.14"), string("pi"));
        BOOST_CHECK(get("nada", props, string("3.14")) == "pi");
    }

    // Use the ignore_other_properties generator
    {
        dynamic_properties props(&ignore_other_properties);
        bool value = put("nada",props,string("3.14"),string("pi"));
        BOOST_CHECK(value == false);
    }

    // Copy test. This addresses Ticket #954, about the correct copying
    // of dynamic_properties.
    {
        dynamic_properties dp1(properties);
        test_put_get(dp1);
    }


  return exit_success;
}
