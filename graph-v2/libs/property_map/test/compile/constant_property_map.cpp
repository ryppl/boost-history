// (C) Copyright Andrew Sutton 2007
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/property_map/archetypes.hpp>
#include <boost/property_map/constant_property_map.hpp>

int main()
{
    using namespace std;
    using namespace boost;
    using namespace boost::property_map;

    typedef null_archetype<> Key;
    typedef assignable_archetype< copy_constructible_archetype<> > Value;
    typedef constant_property_map<Key, Value> ReferenceMap;

    Key& k = static_object<Key>::get();
    Value& v = static_object<Value>::get();
    ReferenceMap rmap = make_constant_property_map<Key>(v);
    get(rmap, k);

    return 0;
}
