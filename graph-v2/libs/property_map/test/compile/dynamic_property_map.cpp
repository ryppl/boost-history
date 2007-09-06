// (C) Copyright Andrew Sutton 2007
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <boost/property_map/archetypes.hpp>
#include <boost/property_map/constant_property_map.hpp>
#include <boost/property_map/reference_property_map.hpp>
#include <boost/property_map/dynamic_property_map.hpp>

int main()
{
    using namespace std;
    using namespace boost;
    using namespace boost::property_map;

    // TODO: Apparently, archetypes won't work in this context since the key
    // and value types _must_ be streamable.
    typedef unsigned Key;
    typedef unsigned Value;

    // construct a couple different property maps
    Key& k = static_object<Key>::get();
    Value& v = static_object<Value>::get();
    constant_property_map<Key, Value> cmap(v);
    reference_property_map<Key, Value> rmap(v);

    dynamic_properties dp;
    dp.property("constant", cmap);
    dp.property("reference", rmap);
    v = get("reference", dp, k, type<Value>());
    v = get("constant", dp, k, type<Value>());

    return 0;
}
