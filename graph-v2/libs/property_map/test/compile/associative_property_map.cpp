// (C) Copyright Andrew Sutton 2007
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <map>
#include <boost/property_map/archetypes.hpp>
#include <boost/property_map/associative_property_map.hpp>

int main()
{
    using namespace std;
    using namespace boost;
    using namespace boost::property_map;

    typedef unsigned Key;
    typedef unsigned Value;
    typedef map<Key, Value> Map;
    typedef associative_property_map<Map> AssociativeMap;

    Key& k = static_object<Key>::get();
    Value& v = static_object<Value>::get();
    Map m;
    AssociativeMap amap = make_associative_property_map(m);

    get(amap, k);
    put(amap, k, v);
    amap[k];

    return 0;
}
