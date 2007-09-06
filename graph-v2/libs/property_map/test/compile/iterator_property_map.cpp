// (C) Copyright Andrew Sutton 2007
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <boost/property_map/archetypes.hpp>
#include <boost/property_map/iterator_property_map.hpp>

int main()
{
    using namespace std;
    using namespace boost;
    using namespace boost::property_map;

    // Use this for the index map for now...
    // TODO: The requirements for iterator property maps are not fully
    // defined. I'm not sure what exactly they are, but apparently the
    // value type does not work as copy constructible and assignable.
    // It has something to do with the fact that the key type of the
    // index map has to be addable to the iterator type of the underling
    // vector. Bizarre.
    typedef null_archetype<> Key;
    typedef unsigned Value;
    typedef vector<Value> Vector;
    typedef readable_property_map_archetype<Key, Value> IndexMap;
    typedef iterator_property_map<Vector::iterator, IndexMap> IteratorMap;

    Key& k = static_object<Key>::get();
    Value& v = static_object<Value>::get();
    Vector vec;
    IndexMap& indices = static_object<IndexMap>::get();
    IteratorMap imap = make_iterator_property_map(vec.begin(), indices);

    get(imap, k);
    put(imap, k, v);
    imap[k];

    return 0;
}
