// (C) Copyright Andrew Sutton 2007
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <map>
#include <boost/property_map/archetypes.hpp>
#include <boost/property_map/iterator_property_map.hpp>
#include <boost/property_map/associative_property_map.hpp>
#include <boost/property_map/reference_property_map.hpp>
#include <boost/property_map/constant_property_map.hpp>
#include <boost/property_map/identity_property_map.hpp>
#include <boost/property_map/dummy_property_map.hpp>
#include <boost/property_map/dynamic_property_map.hpp>

int main()
{
    using namespace std;
    using namespace boost;
    using namespace boost::property_map;

    // Use this for the index map for now...
    typedef size_t Key;
    typedef size_t Value;
    Value v;
    Key k;
    {
        typedef vector<Value> Vector;
        typedef readable_property_map_archetype<Key, Value> IndexMap;
        typedef iterator_property_map<Vector::iterator, IndexMap> IteratorMap;

        Vector v;
        IndexMap& indices = static_object<IndexMap>::get();
        IteratorMap imap = make_iterator_property_map(v.begin(), indices);
        get(imap, Key());
        put(imap, Key(), Value());
        imap[Key()];
    }

    {
        typedef vector<Value> Vector;
        typedef readable_property_map_archetype<Key, Value> IndexMap;
        typedef safe_iterator_property_map<Vector::iterator, IndexMap> IteratorMap;

        Vector v;
        IndexMap& indices = static_object<IndexMap>::get();
        IteratorMap imap = make_safe_iterator_property_map(v.begin(), Key(), indices);
        get(imap, Key());
        put(imap, Key(), Value());
        imap[Key()];
    }

    {
        typedef map<Key, Value> Map;
        typedef associative_property_map<Map> AssociativeMap;

        Map m;
        AssociativeMap amap = make_associative_property_map(m);
        get(amap, Key());
        put(amap, Key(), Value());
        amap[Key()];
    }

    {
        typedef reference_property_map<Key, Value> ReferenceMap;

        ReferenceMap rmap = make_reference_property_map<Key>(v);
        get(rmap, k);
        put(rmap, k, v);
        rmap[k];
        rmap[k] = v;
    }

    {
        typedef constant_property_map<Key, Value> ReferenceMap;

        ReferenceMap rmap = make_constant_property_map<Key>(v);
        get(rmap, k);
    }

    {
        typedef identity_property_map<Value> IdentityMap;

        IdentityMap imap;
        get(imap, k);
        imap[k];
        imap[k] = v;
    }

    {
        typedef dummy_property_map<Key, Value> DummyMap;

        DummyMap dmap;
        put(dmap, k, v);
    }

    {
        // construct a couple different property maps
        constant_property_map<Key, Value> cmap(v);
        reference_property_map<Key, Value> rmap(v);

        dynamic_properties dp;
        dp.property("constant", cmap);
        // dp.property("reference", rmap);

    }
}
