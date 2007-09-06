// (C) Copyright Andrew Sutton 2007
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/concept_check.hpp>
#include <boost/property_map/concepts.hpp>
#include <boost/property_map/archetypes.hpp>

// This is a simple compiler check that feeds archetypes directly into
// their concept checks. Basically, we're testing the testing tools.

int main()
{
    using namespace boost;
    using namespace boost::property_map;

    // TODO: Are there requirements on the key and value of a property map?
    // I don't think so - either that or they vary with the intent of the map.
    // This should be fine for now...
    typedef int Key;
    typedef int Value;

    {
        typedef readable_property_map_archetype<Key, Value> ReadablePropertyMap;
        function_requires< ReadablePropertyMapConcept<ReadablePropertyMap, Key> >();
    }

    {
        typedef writable_property_map_archetype<Key, Value> WritablePropertyMap;
        function_requires< WritablePropertyMapConcept<WritablePropertyMap, Key> >();
    }

    {
        typedef read_write_property_map_archetype<Key, Value> ReadWritePropertyMap;
        function_requires< ReadWritePropertyMapConcept<ReadWritePropertyMap, Key> >();
    }

    {
        typedef lvalue_property_map_archetype<Key, Value> LvaluePropertyMap;
        function_requires< LvaluePropertyMapConcept<LvaluePropertyMap, Key> >();
    }

    {
        typedef mutable_lvalue_property_map_archetype<Key, Value> MutableLvaluePropertyMap;
        function_requires< MutableLvaluePropertyMapConcept<MutableLvaluePropertyMap, Key> >();
    }
}
